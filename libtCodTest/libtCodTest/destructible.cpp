#include <stdio.h>
#include "main.h"

tDestructible::tDestructible(float maxHP, float defense, const char* corpseName) :
m_MaxHP(maxHP),
m_CurrentHP(maxHP),
m_Defense(defense)
{
    m_pCorpseName = strdup(corpseName);
}

tDestructible::~tDestructible()
{
    delete m_pCorpseName;
}

float tDestructible::takeDamage(tActor* pOwner, float damage)
{
	damage -= m_Defense;
	if (damage > 0)
	{
		m_CurrentHP -= damage;
		if (m_CurrentHP <= 0)
		{
			die(pOwner);
		}
	}
	else
	{
		damage = 0;
	}
	return damage;
}

float tDestructible::heal(float amount)
{
    m_CurrentHP += amount;
    if(m_CurrentHP > m_MaxHP)
    {
        amount -= m_CurrentHP - m_MaxHP;
        m_CurrentHP = m_MaxHP;
    }
    return amount;
}

void tDestructible::die(tActor* pOwner)
{
	//transform actor into a corpse
	pOwner->m_AsciiCode = '%';
	pOwner->m_Color = TCODColor::darkRed;
	pOwner->m_pName = m_pCorpseName;
	pOwner->m_Blocks = false;
	// make sure corpses are drawn before living actors
	engine.sendToBack(pOwner);
}

tMonsterDestructible::tMonsterDestructible(float maxHp, float defense, const char* corpseName) :
tDestructible(maxHp, defense, corpseName)
{

}

tPlayerDestructible::tPlayerDestructible(float maxHp, float defense, const char* corpseName) :
tDestructible(maxHp, defense, corpseName)
{

}

void tMonsterDestructible::die(tActor* pOwner)
{
	//transform into a corpse.  doesnt block, cant attack, and doesnt move
	engine.m_pGui->message(TCODColor::lightGrey, "%s is dead\n", pOwner->m_pName);
	tDestructible::die(pOwner);
}

void tPlayerDestructible::die(tActor* pOwner)
{
	engine.m_pGui->message(TCODColor::red, "You died!\n");
	tDestructible::die(pOwner);
	engine.m_GameStatus = tEngine::eGS_DEFEAT;
}