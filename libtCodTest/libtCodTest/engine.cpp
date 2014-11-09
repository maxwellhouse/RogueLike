#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "engine.h"

tEngine::tEngine() :
m_FovRadius(10),
m_GameStatus(eGS_STARTUP)
{
    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
    m_pPlayer = new tActor(40, 25, '@', "player", TCODColor::white);
    m_Actors.push(m_pPlayer);
    m_pMap = new tMap(80,45);
}

tEngine::~tEngine() 
{
    m_Actors.clearAndDelete();
    delete m_pMap;
}

void tEngine::update() 
{
    if(m_GameStatus = eGS_STARTUP)
    {
        m_pMap->computeFov();
    }
    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    int dx = 0;
    int dy = 0;
    switch(key.vk) 
    {
        case TCODK_UP : 
            {
                dy = -1;
            }
        break;
        case TCODK_DOWN : 
            {
                dy = 1;
            }
        break;
        case TCODK_LEFT : 
            {
                dx = -1;
            }
        break;
        case TCODK_RIGHT : 
            {
                dx = 1;
            }
        break;
        default:
        break;
    }
    if ( dx != 0 || dy != 0 ) 
    {
        m_GameStatus = eGS_NEWTURN;
        if ( m_pPlayer->moveOrAttack(m_pPlayer->m_XPosition + dx, m_pPlayer->m_YPosition + dy) ) 
        {
            m_pMap->computeFov();
        }
    }
    if ( m_GameStatus == eGS_NEWTURN ) 
    {
        for (tActor **iterator = m_Actors.begin(); iterator != m_Actors.end(); iterator++) 
        {
            tActor *pActor=*iterator;
            if ( pActor != m_pPlayer ) 
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