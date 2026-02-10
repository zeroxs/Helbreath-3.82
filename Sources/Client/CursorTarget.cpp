// CursorTarget.cpp: Cursor targeting and object focus implementation
//
//////////////////////////////////////////////////////////////////////

#include "CursorTarget.h"
#include "IInput.h"
#include <cstring>
#include <chrono>

// Internal state (static, not exposed)
namespace {
    // Mouse position (cached from Input:: at BeginFrame)
    int s_mouseX = 0;
    int s_mouseY = 0;

    // Current frame's focused object
    FocusedObject s_focusedObject;
    int s_bestHitY = -99999;  // Depth sorting - higher Y = closer

    // Ground item state
    bool s_overGroundItem = false;

    // Cursor state
    CursorType s_cursorType = CursorType::Arrow;

    // Item cursor animation
    int64_t s_itemAnimTime = 0;
    int s_itemAnimFrame = 1;

    // UI Selection state (for item/dialog dragging)
    SelectedObjectType s_selectedType = SelectedObjectType::None;
    short s_selectedID = 0;
    short s_dragDistX = 0;
    short s_dragDistY = 0;
    uint32_t s_selectClickTime = 0;
    short s_clickX = 0;
    short s_clickY = 0;
    short s_prevX = 0;
    short s_prevY = 0;

    // Cursor interaction status (mouse button state machine)
    CursorStatus s_cursorStatus = CursorStatus::Null;
}

//-----------------------------------------------------------------------------
// Frame Lifecycle
//-----------------------------------------------------------------------------

void CursorTarget::BeginFrame()
{
    // Cache mouse position
    s_mouseX = Input::GetMouseX();
    s_mouseY = Input::GetMouseY();

    // Reset focus state
    s_focusedObject = FocusedObject{};
    s_bestHitY = -99999;

    // Reset ground item state
    s_overGroundItem = false;

    // Reset cursor (will be determined in EndFrame)
    s_cursorType = CursorType::Arrow;
}

void CursorTarget::EndFrame(EntityRelationship relationship, int commandType, bool commandAvailable, bool isGetPointingMode)
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    // Check pointing mode FIRST - spell/item targeting takes priority over ground items
    if (isGetPointingMode) {
        // Spell targeting mode (100-199) - takes priority over everything
        if (commandType >= 100 && commandType < 200) {
            if (commandAvailable) {
                if (s_focusedObject.valid && IsHostile(relationship))
                    s_cursorType = CursorType::SpellHostile;
                else
                    s_cursorType = CursorType::SpellFriendly;
            } else {
                s_cursorType = CursorType::Unavailable;
            }
            return;
        }

        // Item use mode (0-49)
        if (commandType >= 0 && commandType < 50) {
            s_cursorType = CursorType::ItemUse;
            return;
        }
    }

    // Normal mode - show target cursor based on focus.
    // Mobs/NPCs/players take priority over ground items on the same tile.
    if (s_focusedObject.valid) {
        // Holding Control treats neutral targets as hostile (for force-attack)
        if (IsHostile(relationship) || Input::IsCtrlDown())
            s_cursorType = CursorType::TargetHostile;
        else
            s_cursorType = CursorType::TargetNeutral;
        return;
    }

    // Ground item cursor - only when no mob/NPC/player is focused
    if (s_overGroundItem) {
        // Animate every 200ms
        if (now - s_itemAnimTime > 200) {
            s_itemAnimTime = now;
            s_itemAnimFrame = (s_itemAnimFrame == 1) ? 2 : 1;
        }
        s_cursorType = (s_itemAnimFrame == 1) ?
            CursorType::ItemGround1 : CursorType::ItemGround2;
        return;
    }

    // Default
    s_cursorType = CursorType::Arrow;
}

//-----------------------------------------------------------------------------
// Object Testing
//-----------------------------------------------------------------------------

void CursorTarget::TestObject(const SpriteLib::BoundRect& bounds, const TargetObjectInfo& info, int screenY, int maxScreenY)
{
    // Skip invalid bounds
    if (bounds.top == -1) return;

    // Skip if mouse is below the valid targeting area (UI region)
    if (s_mouseY > maxScreenY) return;

    // Hit test - check if mouse is within sprite bounds
    // Note: Original code used < and > exclusively, not <= and >=
    if (s_mouseX > bounds.left && s_mouseX < bounds.right &&
        s_mouseY > bounds.top && s_mouseY < bounds.bottom) {

        // Last valid hit wins (tiles are processed back-to-front)
        // Copy info to focused object
        s_focusedObject.valid = true;
        s_focusedObject.objectID = info.objectID;
        s_focusedObject.mapX = info.mapX;
        s_focusedObject.mapY = info.mapY;
        s_focusedObject.screenX = info.screenX;
        s_focusedObject.screenY = info.screenY;
        s_focusedObject.dataX = info.dataX;
        s_focusedObject.dataY = info.dataY;
        s_focusedObject.ownerType = info.ownerType;
        s_focusedObject.type = info.type;
        s_focusedObject.action = info.action;
        s_focusedObject.direction = info.direction;
        s_focusedObject.frame = info.frame;
        s_focusedObject.appearance = info.appearance;
        s_focusedObject.status = info.status;

        // Copy name
        std::memset(s_focusedObject.name, 0, sizeof(s_focusedObject.name));
        if (info.name) {
            std::snprintf(s_focusedObject.name, sizeof(s_focusedObject.name), "%s", info.name);
        }
    }
}

void CursorTarget::TestGroundItem(int screenX, int screenY, int maxScreenY)
{
    // Skip if mouse is below the valid targeting area
    if (s_mouseY > maxScreenY) return;

    // Check circular proximity (13px radius)
    if (PointInCircle(s_mouseX, s_mouseY, screenX, screenY, 13)) {
        s_overGroundItem = true;
    }
}

void CursorTarget::TestDynamicObject(const SpriteLib::BoundRect& bounds, short mapX, short mapY, int maxScreenY)
{
    // Skip invalid bounds
    if (bounds.top == -1) return;

    // Skip if mouse is below the valid targeting area
    if (s_mouseY > maxScreenY) return;

    // Hit test
    if (s_mouseX >= bounds.left && s_mouseX < bounds.right &&
        s_mouseY >= bounds.top && s_mouseY < bounds.bottom) {

        // Dynamic objects (minerals) set focus without full object info
        s_focusedObject.valid = true;
        s_focusedObject.mapX = mapX;
        s_focusedObject.mapY = mapY;
        s_focusedObject.type = FocusedObjectType::DynamicObject;
        s_focusedObject.status.Clear();
        std::memset(s_focusedObject.name, 0, sizeof(s_focusedObject.name));
    }
}

//-----------------------------------------------------------------------------
// Query Functions
//-----------------------------------------------------------------------------

const FocusedObject& CursorTarget::GetFocusedObject()
{
    return s_focusedObject;
}

bool CursorTarget::HasFocusedObject()
{
    return s_focusedObject.valid;
}

CursorType CursorTarget::GetCursorType()
{
    return s_cursorType;
}

int CursorTarget::GetCursorFrame()
{
    return static_cast<int>(s_cursorType);
}

short CursorTarget::GetFocusedMapX()
{
    return s_focusedObject.valid ? s_focusedObject.mapX : 0;
}

short CursorTarget::GetFocusedMapY()
{
    return s_focusedObject.valid ? s_focusedObject.mapY : 0;
}

const char* CursorTarget::GetFocusedName()
{
    return s_focusedObject.name;
}

bool CursorTarget::IsOverGroundItem()
{
    return s_overGroundItem;
}

const PlayerStatus& CursorTarget::GetFocusStatus()
{
    return s_focusedObject.status;
}

//-----------------------------------------------------------------------------
// Focus Highlight Data
//-----------------------------------------------------------------------------

bool CursorTarget::GetFocusHighlightData(
    short& outScreenX, short& outScreenY,
    uint16_t& outObjectID,
    short& outOwnerType, char& outAction, char& outDir, char& outFrame,
    PlayerAppearance& outAppearance, PlayerStatus& outStatus,
    short& outDataX, short& outDataY)
{
    if (!s_focusedObject.valid) {
        return false;
    }

    outScreenX = s_focusedObject.screenX;
    outScreenY = s_focusedObject.screenY;
    outObjectID = s_focusedObject.objectID;
    outOwnerType = s_focusedObject.ownerType;
    outAction = s_focusedObject.action;
    outDir = s_focusedObject.direction;
    outFrame = s_focusedObject.frame;
    outAppearance = s_focusedObject.appearance;
    outStatus = s_focusedObject.status;
    outDataX = s_focusedObject.dataX;
    outDataY = s_focusedObject.dataY;

    return true;
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

bool CursorTarget::PointInRect(int x, int y, const SpriteLib::BoundRect& rect)
{
    return x >= rect.left && x < rect.right &&
           y >= rect.top && y < rect.bottom;
}

bool CursorTarget::PointInCircle(int x, int y, int cx, int cy, int radius)
{
    int dx = x - cx;
    int dy = y - cy;
    return (dx * dx + dy * dy) <= (radius * radius);
}

//-----------------------------------------------------------------------------
// UI Selection State
//-----------------------------------------------------------------------------

void CursorTarget::SetSelection(SelectedObjectType type, short objectID, short distX, short distY)
{
    s_selectedType = type;
    s_selectedID = objectID;
    s_dragDistX = distX;
    s_dragDistY = distY;
}

void CursorTarget::ClearSelection()
{
    s_selectedType = SelectedObjectType::None;
    s_selectedID = 0;
    s_dragDistX = 0;
    s_dragDistY = 0;
}

SelectedObjectType CursorTarget::GetSelectedType()
{
    return s_selectedType;
}

short CursorTarget::GetSelectedID()
{
    return s_selectedID;
}

short CursorTarget::GetDragDistX()
{
    return s_dragDistX;
}

short CursorTarget::GetDragDistY()
{
    return s_dragDistY;
}

bool CursorTarget::HasSelection()
{
    return s_selectedType != SelectedObjectType::None;
}

void CursorTarget::RecordSelectionClick(short x, short y, uint32_t time)
{
    s_selectClickTime = time;
    s_clickX = x;
    s_clickY = y;
}

void CursorTarget::ResetSelectionClickTime()
{
    s_selectClickTime = 0;
}

uint32_t CursorTarget::GetSelectionClickTime()
{
    return s_selectClickTime;
}

short CursorTarget::GetSelectionClickX()
{
    return s_clickX;
}

short CursorTarget::GetSelectionClickY()
{
    return s_clickY;
}

void CursorTarget::SetPrevPosition(short x, short y)
{
    s_prevX = x;
    s_prevY = y;
}

short CursorTarget::GetPrevX()
{
    return s_prevX;
}

short CursorTarget::GetPrevY()
{
    return s_prevY;
}

//-----------------------------------------------------------------------------
// Cursor Interaction Status
//-----------------------------------------------------------------------------

void CursorTarget::SetCursorStatus(CursorStatus status)
{
    s_cursorStatus = status;
}

CursorStatus CursorTarget::GetCursorStatus()
{
    return s_cursorStatus;
}
