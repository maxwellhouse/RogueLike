#include "libtcod.hpp"
#include "main.h"

tEngine::tEngine() :
m_FovRadius(10),
m_GameStatus(eGS_STARTUP)
{
    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
    m_pPlayer = new tActor(40, 25, '@', "player", TCODColor::white);
    m_Actors.push(m_pPlayer);
    m_pMap = new tMap(80,45);
}

tEngine::tEngine(int screenWidth, int screenHeight) :
m_FovRadius(10),
m_GameStatus(eGS_STARTUP),
m_ScreenWidth(screenWidth),
m_ScreenHeight(screenHeight)
{
	TCODConsole::initRoot(m_ScreenWidth, m_ScreenHeight, "libtcod C++ tutorial", false);
	m_pPlayer = new tActor(40, 25, '@', "player", TCODColor::white);
	m_pPlayer->m_pDestructible = new tPlayerDestructible(30, 2, "your cadaver");
	m_pPlayer->m_pAttacker = new tAttacker(5);
	m_pPlayer->m_pAI = new tPlayerAi();
	m_Actors.push(m_pPlayer);
	m_pMap = new tMap(m_ScreenWidth, m_ScreenHeight);
}

tEngine::~tEngine() 
{
    m_Actors.clearAndDelete();
    delete m_pMap;
}

void tEngine::update() 
{
	if (m_GameStatus == eGS_STARTUP)
	{
		m_pMap->computeFov();
	}
	m_GameStatus = eGS_IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &m_LastKey, NULL);
	m_pPlayer->update();
	if (m_GameStatus == eGS_NEWTURN)
	{
		for (tActor** iterator = m_Actors.begin(); iterator != m_Actors.end(); ++iterator)
		{
			tActor* pActor = *iterator;
			if (pActor != m_pPlayer)
			{
				pActor->update();
			}
		}
	}
}

void tEngine::render() 
{
    TCODConsole::root->clear();
    // draw the map
    m_pMap->render();
	// draw the player stats
	TCODConsole::root->print(1, m_ScreenHeight - 2, "HP : %d/%d", 
		static_cast<int>(m_pPlayer->m_pDestructible->m_CurrentHP,
		static_cast<int>(m_pPlayer->m_pDestructible->m_MaxHP)));
    // draw the actors
    for (tActor **iterator = m_Actors.begin(); iterator != m_Actors.end(); iterator++) 
    {
        tActor* pActor = *iterator;
        if ( m_pMap->isInFov(pActor->m_XPosition, pActor->m_YPosition) ) 
        {
            pActor->render();
        }
    }
}

void tEngine::sendToBack(tActor* pActor)
{
	m_Actors.remove(pActor);
	m_Actors.insertBefore(pActor, 0);
}