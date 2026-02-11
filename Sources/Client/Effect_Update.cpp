// Effect_Update.cpp: UpdateEffects implementation
//
//////////////////////////////////////////////////////////////////////

#include "EffectManager.h"
#include "Game.h"
#include "ISprite.h"
#include "Effect.h"
#include "GlobalDef.h"
#include "Misc.h"

extern int _iAttackerHeight[];

void EffectManager::UpdateEffectsImpl()
{
	int i, x;
	uint32_t dwTime;

	short sAbsX, sAbsY, sDist;
	char  cDir;
	long lPan;
	dwTime = m_pGame->m_dwCurTime;
	dwTime += m_pGame->m_pMapData->m_dwFrameAdjustTime;
	for (i = 0; i < game_limits::max_effects; i++)
		if (m_pEffectList[i] != 0) {
			if ((dwTime - m_pEffectList[i]->m_dwTime) > m_pEffectList[i]->m_dwFrameTime)
			{
				m_pEffectList[i]->m_dwTime = dwTime;
				m_pEffectList[i]->m_cFrame++;

				m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
				m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;
				switch (m_pEffectList[i]->m_sType) {
				case EffectType::NORMAL_HIT: // coup normal
					if (m_pEffectList[i]->m_cFrame == 1)
					{
						for (int j = 1; j <= m_pEffectList[i]->m_iV1; j++) AddEffectImpl(EffectType::BURST_SMALL_GRENADE, m_pEffectList[i]->m_mX + 15 - (rand() % 30), m_pEffectList[i]->m_mY + 15 - (rand() % 30), 0, 0, -1 * (rand() % 2), 0);
					}
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::ARROW_FLYING:	// (Arrow missing target ?)
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32 - 40,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 70);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::GOLD_DROP: // Gold Drop ,33,69,70
				case EffectType::IMPACT_EFFECT: //
				case EffectType::LIGHT_EFFECT_1:
				case EffectType::LIGHT_EFFECT_2:
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::FIREBALL_EXPLOSION:
				case EffectType::MASS_FIRE_STRIKE_CALLER1:
				case EffectType::MASS_FIRE_STRIKE_CALLER3: // Fire Explosion
				case EffectType::SALMON_BURST_IMPACT:
					if (m_pEffectList[i]->m_cFrame == 1)
					{
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
					}
					if (m_pEffectList[i]->m_cFrame == 7)
					{
						AddEffectImpl(EffectType::RED_CLOUD_PARTICLES, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0, 0);
						AddEffectImpl(EffectType::RED_CLOUD_PARTICLES, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0, 0);
						AddEffectImpl(EffectType::RED_CLOUD_PARTICLES, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0, 0);
					}
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::ENERGY_BOLT_EXPLOSION: // Lightning Bolt Burst
					if (m_pEffectList[i]->m_cFrame == 1)
					{
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
					}
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::MAGIC_MISSILE_EXPLOSION: // Magic Missile Burst
					if (m_pEffectList[i]->m_cFrame == 1)
					{
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
					}
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BURST_MEDIUM:  // Burst Type 2
				case EffectType::BURST_SMALL_GRENADE: // Burst Type 3
					m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
					m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
					m_pEffectList[i]->m_rY++;
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::LIGHTNING_ARROW_EXPLOSION: // Lightning Arrow Burst
					if (m_pEffectList[i]->m_cFrame == 1)
					{
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
					}
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BURST_LARGE: // Burst Type 4
					m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
					m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BUBBLES_DRUNK: // Bulles druncncity
					if (m_pEffectList[i]->m_cFrame < 15)
					{
						if ((rand() % 2) == 0)
							m_pEffectList[i]->m_mX++;
						else m_pEffectList[i]->m_mX--;
						m_pEffectList[i]->m_mY--;
					}
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::PROJECTILE_GENERIC: //
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 40);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
					{
						AddEffectImpl(EffectType::IMPACT_BURST, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0, 0); // testcode 0111 18
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						AddEffectImpl(EffectType::BURST_MEDIUM, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::ICE_STORM: // Ice-Storm
					cDir = CMisc::cGetNextMoveDir(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, m_pEffectList[i]->m_mX3, m_pEffectList[i]->m_mY3);
					switch (cDir) {
					case 1: // North
						m_pEffectList[i]->m_rY -= 2;
						break;
					case 2: // NorthEast
						m_pEffectList[i]->m_rY -= 2;
						m_pEffectList[i]->m_rX += 2;
						break;
					case 3: // East
						m_pEffectList[i]->m_rX += 2;
						break;
					case 4: // SouthEast
						m_pEffectList[i]->m_rX += 2;
						m_pEffectList[i]->m_rY += 2;
						break;
					case 5: // South
						m_pEffectList[i]->m_rY += 2;
						break;
					case 6: // SouthWest
						m_pEffectList[i]->m_rX -= 2;
						m_pEffectList[i]->m_rY += 2;
						break;
					case 7: // West
						m_pEffectList[i]->m_rX -= 2;
						break;
					case 8: // NorthWest
						m_pEffectList[i]->m_rX -= 2;
						m_pEffectList[i]->m_rY -= 2;
						break;
					}
					if (m_pEffectList[i]->m_rX < -10) m_pEffectList[i]->m_rX = -10;
					if (m_pEffectList[i]->m_rX > 10) m_pEffectList[i]->m_rX = 10;
					if (m_pEffectList[i]->m_rY < -10) m_pEffectList[i]->m_rY = -10;
					if (m_pEffectList[i]->m_rY > 10) m_pEffectList[i]->m_rY = 10;
					m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
					m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;
					m_pEffectList[i]->m_mY3--;
					if (m_pEffectList[i]->m_cFrame > 10)
					{
						m_pEffectList[i]->m_cFrame = 0;
						if (abs(m_pEffectList[i]->m_sY - m_pEffectList[i]->m_mY3) > 100)
						{
							delete m_pEffectList[i];
							m_pEffectList[i] = 0;
						}
					}
					break;

				case EffectType::CRITICAL_STRIKE_1: // Critical strike with a weapon
				case EffectType::CRITICAL_STRIKE_2:
				case EffectType::CRITICAL_STRIKE_3:
				case EffectType::CRITICAL_STRIKE_4:
				case EffectType::CRITICAL_STRIKE_5:
				case EffectType::CRITICAL_STRIKE_6:
				case EffectType::CRITICAL_STRIKE_7:
				case EffectType::CRITICAL_STRIKE_8: // Critical strike with a weapon
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32 - 40,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0, 0);//-1*(rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0, 0);//-1*(rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0, 0);//-1*(rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0, 0);//-1*(rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0, 0);//-1*(rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
						(abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BLOODY_SHOCK_STRIKE: //
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::IMPACT_EFFECT, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, -1 * (rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
						(abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
					{
						AddEffectImpl(EffectType::IMPACT_EFFECT, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0, 0); //7
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;


				case EffectType::CHILL_WIND_IMPACT:
				case EffectType::MASS_CHILL_WIND:
					if (m_pEffectList[i]->m_cFrame == 9)
					{
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
					}
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::ICE_STRIKE_VARIANT_1: //Large Type 1, 2, 3, 4
				case EffectType::ICE_STRIKE_VARIANT_2:
				case EffectType::ICE_STRIKE_VARIANT_3:
				case EffectType::ICE_STRIKE_VARIANT_4:
				case EffectType::ICE_STRIKE_VARIANT_5: // Small Type 1, 2
				case EffectType::ICE_STRIKE_VARIANT_6:
					if (m_pEffectList[i]->m_cFrame >= 7)
					{
						m_pEffectList[i]->m_mX--;
						m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
						m_pEffectList[i]->m_iV1++;
					}

					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						if ((m_pEffectList[i]->m_sType != EffectType::ICE_STRIKE_VARIANT_5) && (m_pEffectList[i]->m_sType != EffectType::ICE_STRIKE_VARIANT_6))
						{
							AddEffectImpl(EffectType::SMOKE_DUST, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
							AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
							AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
							AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
							AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
							AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
						}
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BLIZZARD_VARIANT_1: // Blizzard
				case EffectType::BLIZZARD_VARIANT_2:
				case EffectType::BLIZZARD_VARIANT_3:
					if (m_pEffectList[i]->m_cFrame >= 7)
					{
						m_pEffectList[i]->m_mX--;
						m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
						m_pEffectList[i]->m_iV1 += 4;
					}
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						if (m_pEffectList[i]->m_sType == EffectType::BLIZZARD_VARIANT_3)
							AddEffectImpl(EffectType::BLIZZARD_IMPACT, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
						else AddEffectImpl(EffectType::SMOKE_DUST, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
						AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
						AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
						AddEffectImpl(EffectType::FOOTPRINT, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);

						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
						AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::METEOR_FLYING: //
				case EffectType::METEOR_STRIKE_DESCENDING: // Meteor-Strike
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						AddEffectImpl(EffectType::FIRE_AURA_GROUND, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
						AddEffectImpl(EffectType::FIRE_EXPLOSION_CRUSADE, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						AddEffectImpl(EffectType::BURST_LARGE, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2), 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else if (m_pEffectList[i]->m_cFrame >= 0)
					{
						m_pEffectList[i]->m_mX -= 30;
						m_pEffectList[i]->m_mY += 46;
						AddEffectImpl(EffectType::METEOR_IMPACT, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
					}
					break;

				case EffectType::METEOR_IMPACT:
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else if (m_pEffectList[i]->m_cFrame >= 0)
					{
						m_pEffectList[i]->m_mX += (rand() % 3) - 1;
						m_pEffectList[i]->m_mY += (rand() % 3) - 1;
					}
					break;

				case EffectType::MS_CRUSADE_CASTING: // Building fire after MS (crusade) 65 & 67
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else if (m_pEffectList[i]->m_cFrame >= 0)
					{
						m_pEffectList[i]->m_mX += (rand() % 3) - 1;
						m_pEffectList[i]->m_mY -= 4 + (rand() % 2);
					}
					break;

				case EffectType::MS_CRUSADE_EXPLOSION:
				case EffectType::EXPLOSION_FIRE_APOCALYPSE:
				case EffectType::CRACK_OBLIQUE:
				case EffectType::CRACK_HORIZONTAL:
				case EffectType::STEAMS_SMOKE:
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::WORM_BITE:
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else if (m_pEffectList[i]->m_cFrame == 11)
					{
						m_pGame->SetCameraShakingEffect(m_pEffectList[i]->m_iV1, 2);
					}
					break;

				case EffectType::BLIZZARD_PROJECTILE:
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::BLIZZARD_VARIANT_2, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 0);
					AddEffectImpl(EffectType::SPARKLE_SMALL, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
						(abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
					{
						AddEffectImpl(EffectType::BLIZZARD_VARIANT_3, m_pEffectList[i]->m_mX/* + (rand() % 30) - 15*/, m_pEffectList[i]->m_mY/* + (rand() % 30) - 15*/, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::STORM_BLADE: // Snoopy: Added StromBlade
					CMisc::GetPoint(m_pEffectList[i]->m_mX
						, m_pEffectList[i]->m_mY
						, m_pEffectList[i]->m_dX * 32
						, m_pEffectList[i]->m_dY * 32
						, &m_pEffectList[i]->m_mX
						, &m_pEffectList[i]->m_mY
						, &m_pEffectList[i]->m_iErr
						, 10);
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::MAGIC_MISSILE_FLYING: // Magic Missile
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));

					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
						(abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32/* - 40*/)) <= 2))
					{
						AddEffectImpl(EffectType::MAGIC_MISSILE_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::ENERGY_BOLT_FLYING: // Enegy-Bolt
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{
						AddEffectImpl(EffectType::ENERGY_BOLT_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0); // 6 testcode 0111
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::FIRE_BALL_FLYING: // Fire Ball
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{
						AddEffectImpl(EffectType::FIREBALL_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::FIRE_STRIKE_FLYING: // Fire Strike
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{
						AddEffectImpl(EffectType::FIREBALL_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						AddEffectImpl(EffectType::FIREBALL_EXPLOSION, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 0);
						AddEffectImpl(EffectType::FIREBALL_EXPLOSION, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5, 0);
						AddEffectImpl(EffectType::FIREBALL_EXPLOSION, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::LIGHTNING_ARROW_FLYING: // Lightning Arrow
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{
						AddEffectImpl(EffectType::LIGHTNING_ARROW_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::LIGHTNING: // Lightning
				case EffectType::LIGHTNING_BOLT: // Lightning-Bolt
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						AddEffectImpl(EffectType::LIGHTNING_ARROW_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						m_pEffectList[i]->m_rX = 5 - (rand() % 10);
						m_pEffectList[i]->m_rY = 5 - (rand() % 10);
					}
					break;

				case EffectType::CHILL_WIND: // Chill-Wind
					AddEffectImpl(EffectType::CHILL_WIND_IMPACT, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
					AddEffectImpl(EffectType::CHILL_WIND_IMPACT, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -10, 0);
					AddEffectImpl(EffectType::CHILL_WIND_IMPACT, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -6, 0);
					AddEffectImpl(EffectType::CHILL_WIND_IMPACT, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
					break;

				case EffectType::TRIPLE_ENERGY_BOLT:  // Triple-Energy-Bolt
					AddEffectImpl(EffectType::ENERGY_BOLT_FLYING, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						m_pEffectList[i]->m_dX - 1, m_pEffectList[i]->m_dY - 1, 0);
					AddEffectImpl(EffectType::ENERGY_BOLT_FLYING, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						m_pEffectList[i]->m_dX + 1, m_pEffectList[i]->m_dY - 1, 0);
					AddEffectImpl(EffectType::ENERGY_BOLT_FLYING, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
						m_pEffectList[i]->m_dX + 1, m_pEffectList[i]->m_dY + 1, 0);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX) * LOGICAL_WIDTH();
					m_pGame->PlayGameSound('E', 1, sDist, lPan);
					AddEffectImpl(EffectType::MAGIC_MISSILE_EXPLOSION, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
					break;

				case EffectType::MASS_LIGHTNING_ARROW: // Mass-Lightning-Arrow
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::LIGHTNING_ARROW_FLYING, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
							m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0);
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX;
						else sDist = sAbsY;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX) * LOGICAL_WIDTH();
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::ICE_STRIKE: // Ice-Strike
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_1, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
					for (x = 0; x < 14; x++)
					{
						AddEffectImpl(static_cast<EffectType>(41 + (rand() % 3)), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1);
					}
					for (x = 0; x < 6; x++)
					{
						AddEffectImpl(static_cast<EffectType>(45 + (rand() % 2)), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1 - 10);
					}
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
					break;

				case EffectType::ENERGY_STRIKE: // Energy-Strike
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::PROJECTILE_GENERIC, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
							m_pEffectList[i]->m_dX * 32 + 50 - (rand() % 100), m_pEffectList[i]->m_dY * 32 + 50 - (rand() % 100), 0);
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX;
						else sDist = sAbsY;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::MASS_FIRE_STRIKE_FLYING: // Mass-Fire-Strike
					CMisc::GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
						m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
						&m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
						&m_pEffectList[i]->m_iErr, 50);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{
						AddEffectImpl(EffectType::MASS_FIRE_STRIKE_CALLER1, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
						AddEffectImpl(EffectType::MASS_FIRE_STRIKE_CALLER3, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 0);
						AddEffectImpl(EffectType::MASS_FIRE_STRIKE_CALLER3, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5, 0);
						AddEffectImpl(EffectType::MASS_FIRE_STRIKE_CALLER3, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::MASS_CHILL_WIND_SPELL: // Mass-Chill-Wind Chill-Wind
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -10, 0);
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -6, 0);
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 0);
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
					AddEffectImpl(EffectType::MASS_CHILL_WIND, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
					break;

				case EffectType::WORM_BITE_MASS: // worm-bite
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						AddEffectImpl(EffectType::WORM_BITE, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0); // testcode 0111 18
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BLOODY_SHOCK_WAVE: // Bloody-Shock-Wave
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else if ((m_pEffectList[i]->m_cFrame % 2) == 0)
					{
						AddEffectImpl(EffectType::BLOODY_SHOCK_STRIKE, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
							m_pEffectList[i]->m_dX * 32 + 30 - (rand() % 60), m_pEffectList[i]->m_dY * 32 + 30 - (rand() % 60), 0);
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX;
						else sDist = sAbsY;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::MASS_ICE_STRIKE: // Mass-Ice-Strike
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 0);
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
					for (x = 0; x < 16; x++)
					{
						AddEffectImpl(EffectType::ICE_STRIKE_VARIANT_4, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * x - 1, 0);
					}
					for (x = 0; x < 8; x++)
					{
						AddEffectImpl(static_cast<EffectType>(45 + (rand() % 2)), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1 - 10);
					}
					delete m_pEffectList[i];
					m_pEffectList[i] = 0;
					break;

				case EffectType::LIGHTNING_STRIKE: // Lightning-Strike
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::LIGHTNING_BOLT, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
							m_pEffectList[i]->m_dX + (rand() % 3) - 1, m_pEffectList[i]->m_dY + (rand() % 3) - 1, 0);
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX;
						else sDist = sAbsY;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::MASS_MAGIC_MISSILE_FLYING: // Mass-Magic-Missile
					CMisc::GetPoint(m_pEffectList[i]->m_mX
						, m_pEffectList[i]->m_mY
						, m_pEffectList[i]->m_dX * 32
						, m_pEffectList[i]->m_dY * 32
						, &m_pEffectList[i]->m_mX
						, &m_pEffectList[i]->m_mY
						, &m_pEffectList[i]->m_iErr
						, 50);
					AddEffectImpl(EffectType::BURST_SMALL, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
					{	// JLE 0043132A
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::MASS_MAGIC_MISSILE_AURA1, m_pEffectList[i]->m_dX * 32 + 22, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 1);
						AddEffectImpl(EffectType::MASS_MAGIC_MISSILE_AURA2, m_pEffectList[i]->m_dX * 32 - 22, m_pEffectList[i]->m_dY * 32 - 7, 0, 0, -7, 1);
						AddEffectImpl(EffectType::MASS_MAGIC_MISSILE_AURA2, m_pEffectList[i]->m_dX * 32 + 30, m_pEffectList[i]->m_dY * 32 - 22, 0, 0, -5, 1);
						AddEffectImpl(EffectType::MASS_MAGIC_MISSILE_AURA2, m_pEffectList[i]->m_dX * 32 + 12, m_pEffectList[i]->m_dY * 32 + 22, 0, 0, -3, 1);
					}
					break;

				case EffectType::BLIZZARD: // Blizzard
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::BLIZZARD_PROJECTILE, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
							m_pEffectList[i]->m_dX * 32 + (rand() % 120) - 60, m_pEffectList[i]->m_dY * 32 + (rand() % 120) - 60, 0);
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX;
						else sDist = sAbsY;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::EARTH_SHOCK_WAVE: // Earth-Shock-Wave
					CMisc::GetPoint(m_pEffectList[i]->m_mX
						, m_pEffectList[i]->m_mY
						, m_pEffectList[i]->m_dX * 32
						, m_pEffectList[i]->m_dY * 32
						, &m_pEffectList[i]->m_mX
						, &m_pEffectList[i]->m_mY
						, &m_pEffectList[i]->m_iErr
						, 40);
					AddEffectImpl(EffectType::EARTH_SHOCK_WAVE_PARTICLE, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 1);
					AddEffectImpl(EffectType::EARTH_SHOCK_WAVE_PARTICLE, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, 0, 0);
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						sAbsX = abs(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						sAbsY = abs(((m_pGame->m_Camera.GetY() / 32) + VIEW_CENTER_TILE_Y()) - m_pEffectList[i]->m_dY);
						if (sAbsX > sAbsY) sDist = sAbsX - 10;
						else sDist = sAbsY - 10;
						lPan = -(((m_pGame->m_Camera.GetX() / 32) + VIEW_CENTER_TILE_X()) - m_pEffectList[i]->m_dX);
						m_pGame->PlayGameSound('E', 1, sDist, lPan);
					}
					break;

				case EffectType::SHOTSTAR_FALL_1:
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::EXPLOSION_FIRE_APOCALYPSE, m_pEffectList[i]->m_sX + 40, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_OBLIQUE, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_HORIZONTAL, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
						AddEffectImpl(EffectType::STEAMS_SMOKE, m_pEffectList[i]->m_sX - 7, m_pEffectList[i]->m_sY + 27, 0, 0, 0, 0);
						AddEffectImpl(EffectType::SHOTSTAR_FALL_2, (rand() % (LOGICAL_WIDTH() / 4)) + LOGICAL_WIDTH() / 2, (rand() % (LOGICAL_HEIGHT() / 4)) + LOGICAL_HEIGHT() / 2, 0, 0, 0, 1);
						AddEffectImpl(EffectType::SHOTSTAR_FALL_3, (rand() % (LOGICAL_WIDTH() / 4)) + LOGICAL_WIDTH() / 2, (rand() % (LOGICAL_HEIGHT() / 4)) + LOGICAL_HEIGHT() / 2, 0, 0, 0, 1);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::SHOTSTAR_FALL_2:
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::EXPLOSION_FIRE_APOCALYPSE, m_pEffectList[i]->m_sX + 110, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_OBLIQUE, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_HORIZONTAL, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
						AddEffectImpl(EffectType::SHOTSTAR_FALL_3, (rand() % (LOGICAL_WIDTH() / 4)) + LOGICAL_WIDTH() / 2, (rand() % (LOGICAL_HEIGHT() / 4)) + LOGICAL_HEIGHT() / 2, 0, 0, 0, 1);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::SHOTSTAR_FALL_3:
					if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::EXPLOSION_FIRE_APOCALYPSE, m_pEffectList[i]->m_sX + 65, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_OBLIQUE, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
						AddEffectImpl(EffectType::CRACK_HORIZONTAL, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
						AddEffectImpl(EffectType::STEAMS_SMOKE, m_pEffectList[i]->m_sX - 7, m_pEffectList[i]->m_sY + 27, 0, 0, 0, 0);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::GATE_ROUND: // Gate round
					CMisc::GetPoint(m_pEffectList[i]->m_mX
						, m_pEffectList[i]->m_mY
						, m_pEffectList[i]->m_dX * 32
						, m_pEffectList[i]->m_dY * 32 - 40
						, &m_pEffectList[i]->m_mX
						, &m_pEffectList[i]->m_mY
						, &m_pEffectList[i]->m_iErr
						, 10);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::SALMON_BURST: // Salmon burst (effect11s)
					CMisc::GetPoint(m_pEffectList[i]->m_mX
						, m_pEffectList[i]->m_mY
						, m_pEffectList[i]->m_dX * 32
						, m_pEffectList[i]->m_dY * 32
						, &m_pEffectList[i]->m_mX
						, &m_pEffectList[i]->m_mY
						, &m_pEffectList[i]->m_iErr
						, 50);
					if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
						&& (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					else
					{
						AddEffectImpl(EffectType::SALMON_BURST_IMPACT, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 1);
						AddEffectImpl(EffectType::SALMON_BURST_IMPACT, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 1);
						AddEffectImpl(EffectType::SALMON_BURST_IMPACT, m_pEffectList[i]->m_dX * 32 - 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5, 1);
						AddEffectImpl(EffectType::SALMON_BURST_IMPACT, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 1);
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;

				case EffectType::BURST_SMALL:
				case EffectType::FOOTPRINT:
				case EffectType::RED_CLOUD_PARTICLES:
				case EffectType::IMPACT_BURST:
				case EffectType::FOOTPRINT_RAIN:
				case EffectType::MASS_MAGIC_MISSILE_AURA1: //
				case EffectType::MASS_MAGIC_MISSILE_AURA2: //
				case EffectType::SMOKE_DUST:
				case EffectType::SPARKLE_SMALL:
				case EffectType::PROTECTION_RING:
				case EffectType::HOLD_TWIST:
				case EffectType::STAR_TWINKLE:
				case EffectType::UNUSED_55:
				case EffectType::BUFF_EFFECT_LIGHT:
				case EffectType::FIRE_AURA_GROUND:
				case EffectType::FIRE_EXPLOSION_CRUSADE:
				case EffectType::WHITE_HALO:
				case EffectType::MS_FIRE_SMOKE:
				case EffectType::BLIZZARD_IMPACT:
				case EffectType::AURA_EFFECT_1:
				case EffectType::AURA_EFFECT_2:
				case EffectType::ICE_GOLEM_EFFECT_1:
				case EffectType::ICE_GOLEM_EFFECT_2:
				case EffectType::ICE_GOLEM_EFFECT_3:
				case EffectType::EARTH_SHOCK_WAVE_PARTICLE: //
				case EffectType::GATE_APOCALYPSE: //

				case EffectType::HEAL:
				case EffectType::CREATE_FOOD:
				case EffectType::STAMINA_DRAIN:
				case EffectType::RECALL:
				case EffectType::DEFENSE_SHIELD:
				case EffectType::GREAT_HEAL:
				case EffectType::UNUSED_122:
				case EffectType::STAMINA_RECOVERY: // Stamina Rec
				case EffectType::PROTECT_FROM_NM:
				case EffectType::HOLD_PERSON:
				case EffectType::POSSESSION:
				case EffectType::POISON:
				case EffectType::GREAT_STAMINA_RECOVERY: // Gr Stamina Rec
				case EffectType::SUMMON_CREATURE:
				case EffectType::INVISIBILITY:
				case EffectType::PROTECT_FROM_MAGIC:
				case EffectType::DETECT_INVISIBILITY:
				case EffectType::PARALYZE:
				case EffectType::CURE:
				case EffectType::CONFUSE_LANGUAGE:
				case EffectType::GREAT_DEFENSE_SHIELD:
				case EffectType::BERSERK: // Berserk : Cirlcle 6 magic
				case EffectType::POLYMORPH: // Polymorph
				case EffectType::MASS_POISON:
				case EffectType::CONFUSION:
				case EffectType::ABSOLUTE_MAGIC_PROTECTION:
				case EffectType::ARMOR_BREAK:
				case EffectType::MASS_CONFUSION:
				case EffectType::CANCELLATION: //
				case EffectType::ILLUSION_MOVEMENT: //

				case EffectType::ILLUSION:
				case EffectType::INHIBITION_CASTING: //
				case EffectType::MAGIC_DRAIN: // EP's Magic Drain
				case EffectType::MASS_ILLUSION:
				case EffectType::ICE_RAIN_VARIANT_1:
				case EffectType::ICE_RAIN_VARIANT_2:
				case EffectType::RESURRECTION:
				case EffectType::MASS_ILLUSION_MOVEMENT:
				case EffectType::MAGE_HERO_SET: // Mage hero effect
				case EffectType::WAR_HERO_SET: // War hero effect
				case EffectType::MASS_MM_AURA_CASTER: // Snoopy: Moved for new spells: Caster aura for Mass MagicMissile
					if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
					{
						delete m_pEffectList[i];
						m_pEffectList[i] = 0;
					}
					break;
				}
			}
		}

}
