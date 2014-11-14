#include <stdio.h>

#include "main.h"

tActor::tActor(int x, int y, int ch, const char *name, const TCODColor &col) :
    m_XPosition(x),
    m_YPosition(y),
    m_AsciiCode(ch),
    m_Color(col),
    m_pName(name),
	m_Blocks(true),
	m_pAttacker(NULL),
	m_pDestructible(NULL),
	m_pAI(NULL),
    m_pPickable(NULL),
    m_pContainer(NULL)
{
}

tActor::~tActor()
{
	if(m_pAttacker)
        delete m_pAttacker;
    if(m_pDestructible)
        delete m_pDestructible;
	if(m_pAI)
        delete m_pAI;
    if(m_pPickable)
        delete m_pPickable;
    if(m_pContainer)
        delete m_pContainer;
}

void tActor::update() 
{
	if (m_pAI)
	{ 
		m_pAI->update(this);
	}
}

void tActor::render() const 
{
    TCODConsole::root->setChar(m_XPosition,m_YPosition,m_AsciiCode);
    TCODConsole::root->setCharForeground(m_XPosition,m_YPosition,m_Color);
}
