#include <stdio.h>
#include <math.h>
#include "main.h"

// how many turns the monster chases the player
// after losing his sight
static const int TRACKING_TURNS = 3;

double round(double number)
{
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

void tPlayerAi::update(tActor* pOwner)
{
	if (pOwner->m_pDestructible && pOwner->m_pDestructible->isDead())
	{
		return;
	}

	int dx = 0;
	int dy = 0;
	switch (engine.m_LastKey.vk)
	{
	case TCODK_UP:
	{
		dy = -1;
		break;
	}
	case TCODK_DOWN:
	{
		dy = 1;
		break;
	}
	case TCODK_LEFT:
	{
		dx = -1;
		break;
	}
	case TCODK_RIGHT:
	{
		dx = 1;
		break;
	}
	default:
		break;
	}
	if (dx != 0 || dy != 0)
	{
		engine.m_GameStatus = tEngine::eGS_NEWTURN;
		if (moveOrAttack(pOwner, pOwner->m_XPosition + dx, pOwner->m_YPosition + dy))
		{
			engine.m_pMap->computeFov();
		}
	}
}

bool tPlayerAi::moveOrAttack(tActor* pOwner, int targetX, int targetY)
{
	if (engine.m_pMap->isWall(targetX, targetY))
	{
		return false;
	}
	// look for living actor to attack
	for (tActor** iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); ++iterator)
	{
		tActor* pActor = *iterator;
		if (pActor->m_pDestructible && pActor->m_pDestructible->isDead() == false &&
			pActor->m_XPosition == targetX && pActor->m_YPosition == targetY)
		{
			pOwner->m_pAttacker->attack(pOwner, pActor);
			return false;
		}
	}
	// look for corpses
	for (tActor** iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); ++iterator)
	{
		tActor* pActor = *iterator;
		if (pActor->m_pDestructible && pActor->m_pDestructible->isDead() == true &&
			pActor->m_XPosition == targetX && pActor->m_YPosition == targetY)
		{
			printf("There is a %s here\n", pActor->m_pName);
		}
	}
	pOwner->m_XPosition = targetX;
	pOwner->m_YPosition = targetY;
	return true;
}

void tMonsterAi::update(tActor* pOwner)
{
	if (pOwner->m_pDestructible && pOwner->m_pDestructible->isDead())
	{
		return;
	}
	if (engine.m_pMap->isInFov(pOwner->m_XPosition, pOwner->m_YPosition))
	{
		// we can see the player, so move towards him
		m_MoveCount = TRACKING_TURNS;
	}
    else
    {
        m_MoveCount--;
    }
    if(m_MoveCount > 0)
    {
        moveOrAttack(pOwner, engine.m_pPlayer->m_XPosition, engine.m_pPlayer->m_YPosition);
    }
}

void tMonsterAi::moveOrAttack(tActor* pOwner, int targetX, int targetY)
{
	int dx = targetX - pOwner->m_XPosition;
	int dy = targetY - pOwner->m_YPosition;
    int stepdx = ( dx > 0 ? 1: -1 );
    int stepdy = ( dy > 0 ? 1: -1 );
	float distance = sqrtf(static_cast<float>(dx*dx + dy*dy));

	if (distance > 2)
	{
		dx = static_cast<int>(round(dx / distance));
		dy = static_cast<int>(round(dy / distance));

		if (engine.m_pMap->canWalk(pOwner->m_XPosition + dx, pOwner->m_YPosition + dy))
		{
			pOwner->m_XPosition += dx;
			pOwner->m_YPosition += dy;
		}
        else if(engine.m_pMap->canWalk(pOwner->m_XPosition + stepdx, pOwner->m_YPosition))
        {
            pOwner->m_XPosition += stepdx;
        }
        else if(engine.m_pMap->canWalk(pOwner->m_XPosition, pOwner->m_YPosition + stepdy))
        {
            pOwner->m_YPosition += stepdy;
        }
		else if (pOwner->m_pAttacker)
		{
			pOwner->m_pAttacker->attack(pOwner, engine.m_pPlayer);
		}
	}
}