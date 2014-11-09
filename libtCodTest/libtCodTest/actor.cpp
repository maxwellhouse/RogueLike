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
	m_pAI(NULL)
{
}

void tActor::update() 
{
	if (m_pAi)
	{ 
		ai->update(this);
	}
}

void tActor::render() const 
{
    TCODConsole::root->setChar(m_XPosition,m_YPosition,m_AsciiCode);
    TCODConsole::root->setCharForeground(m_XPosition,m_YPosition,m_Color);
}
