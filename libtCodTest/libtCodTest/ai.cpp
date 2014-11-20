#include <stdio.h>
#include <math.h>
#include "main.h"

static const int INVENTORY_WIDTH = 50;
static const int INVENTORY_HEIGHT = 28;
static TCODConsole gConsole(INVENTORY_WIDTH, INVENTORY_HEIGHT);

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
	case TCODK_CHAR:
	{
        handleActionKey(pOwner, engine.m_LastKey.c);
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

void tPlayerAi::handleActionKey(tActor* pOwner, int ascii)
{
    switch(ascii)
    {
        case 'g': // get item
        {
            bool found = false;
            for(tActor** iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); ++iterator)
            {
                tActor* pActor = *iterator;
                if(pActor->m_pPickable && pActor->m_XPosition == pOwner->m_XPosition &&
                   pActor->m_YPosition == pOwner->m_YPosition)
                   {
                        if(pActor->m_pPickable->pick(pActor, pOwner))
                        {
                            found = true;
                            engine.m_pGui->message(TCODColor::lightGrey, "You pick up the %s.", pActor->m_pName);
                        }
                        else if (found == false)
                        {
                            found = true;
                            engine.m_pGui->message(TCODColor::red, "Your inventory is full.");
                        }
                   }
            }
            if(found == false)
            {
                engine.m_pGui->message(TCODColor::lightGrey, "There is nothing here.");
            }
            engine.m_GameStatus = tEngine::eGS_NEWTURN;
            break;
        }
        case 'i' : // display inventory
        {
            tActor *pActor = choseFromInventory(pOwner);
            if ( pActor )
            {
                pActor->m_pPickable->use(pActor,pOwner);
                engine.m_GameStatus = tEngine::eGS_NEWTURN;
            }
            break;
        }
        case 'd': // drop item
        {
            tActor* pActor = choseFromInventory(pOwner);
            if(pActor)
            {
                pActor->m_pPickable->drop(pActor, pOwner);
                engine.m_GameStatus = tEngine::eGS_NEWTURN;
            }
        }
        break;
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
		bool corpseOrItem = (pActor->m_pDestructible && pActor->m_pDestructible->isDead()
            || pActor->m_pPickable);
		if (corpseOrItem == true &&
			pActor->m_XPosition == targetX && pActor->m_YPosition == targetY)
		{
			engine.m_pGui->message(TCODColor::lightGrey,"There is a %s here\n", pActor->m_pName);
		}
	}
	pOwner->m_XPosition = targetX;
	pOwner->m_YPosition = targetY;
	return true;
}

tActor* tPlayerAi::choseFromInventory(tActor* pOwner)
{
    // display inventory frame
    gConsole.setDefaultForeground(TCODColor(200,180,50));
    gConsole.printFrame(0,0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "inventory");

    // display the items with their keyboard shortcut
    gConsole.setDefaultForeground(TCODColor::white);
    int shortcut = 'a';
    int y = 1;
    for(tActor** iterator = pOwner->m_pContainer->m_Inventory.begin();
        iterator != pOwner->m_pContainer->m_Inventory.end(); ++iterator)
    {
        tActor* pActor = *iterator;
        gConsole.print(2,y,"%c, %s", shortcut, pActor->m_pName);
        y++;
        shortcut++;
    }
    // blit the inventory console on the root console
    TCODConsole::blit(&gConsole, 0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,
        TCODConsole::root, engine.m_ScreenWidth/2 - INVENTORY_WIDTH/2,
        engine.m_ScreenHeight/2-INVENTORY_HEIGHT/2);
    TCODConsole::flush();

    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_KEY_PRESSED, &key, NULL, true);
    if(key.vk == TCODK_CHAR)
    {
        int actorIndex = key.c - 'a';
        if(actorIndex >= 0 && actorIndex < pOwner->m_pContainer->m_Inventory.size())
        {
            return pOwner->m_pContainer->m_Inventory.get(actorIndex);
        }
    }
    return NULL;
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

	if (distance >= 2)
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
	}
	else if (pOwner->m_pAttacker)
	{
		pOwner->m_pAttacker->attack(pOwner, engine.m_pPlayer);
	}
}

tConfusedMonsterAi::tConfusedMonsterAi(int nbTurns, tAi* pOldAi):
m_nbTurns(nbTurns),
m_pAi(pOldAi)
{
}

void tConfusedMonsterAi::update(tActor* pOwner)
{
    TCODRandom* pRng = TCODRandom::getInstance();
    int dx = pRng->getInt(-1,1);
    int dy = pRng->getInt(-1,1);
    if( dx != 0 || dy != 0)
    {
        int destx = pOwner->m_XPosition + dx;
        int desty = pOwner->m_YPosition + dy;
        if(engine.m_pMap->canWalk(destx, desty))
        {
            pOwner->m_XPosition = destx;
            pOwner->m_YPosition = desty;
        }
        else
        {
            tActor* pTarget = engine.getActor(destx, desty);
            if(pTarget)
            {
                pOwner->m_pAttacker->attack(pOwner, pTarget);
            }
        }
    }
    m_nbTurns--;
    if(m_nbTurns == 0)
    {
        pOwner->m_pAI = m_pAi;
        delete this;
    }
}