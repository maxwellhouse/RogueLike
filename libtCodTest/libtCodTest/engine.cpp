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
    m_pGui = new tGui();
    m_pGui->message(TCODColor::red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
}

tEngine::tEngine(int screenWidth, int screenHeight) :
m_FovRadius(10),
m_GameStatus(eGS_STARTUP),
m_ScreenWidth(screenWidth),
m_ScreenHeight(screenHeight)
{
	TCODConsole::initRoot(m_ScreenWidth, m_ScreenHeight, "libtcod C++ tutorial", false);
	m_pPlayer = new tActor(40, 25, '@', "player", TCODColor::white);
	m_pPlayer->m_pDestructible = new tPlayerDestructible(1, 2, "your cadaver");
	m_pPlayer->m_pAttacker = new tAttacker(5);
	m_pPlayer->m_pAI = new tPlayerAi();
	m_Actors.push(m_pPlayer);
	m_pMap = new tMap(m_ScreenWidth, m_ScreenHeight);
    m_pGui = new tGui();
    m_pGui->message(TCODColor::red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
}

tEngine::~tEngine() 
{
    m_Actors.clearAndDelete();
    delete m_pMap;
    delete m_pGui;
}

void tEngine::update() 
{
	if (m_GameStatus == eGS_STARTUP)
	{
		m_pMap->computeFov();
	}
	m_GameStatus = eGS_IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, &m_LastKey, &m_Mouse);
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
    m_pGui->render();
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