#include <stdio.h>
#include "main.h"

tDestructible::tDestructible(float maxHP, float defense, const char* corpseName) :
m_MaxHP(maxHP),
m_CurrentHP(maxHP),
m_Defense(defense),
m_pCorpseName(corpseName)
{}

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

void tMonsterDestructible::die(tActor* pOwner)
{
	//transform into a corpse.  doesnt block, cant attack, and doesnt move
	printf("%s is dead\n", pOwner->m_pName);
	tDestructible::die(pOwner);
}

void tPlayerDestructible::die(tActor* pOwner)
{
	printf("You died!\n");
	tDestructible::die(pOwner);
	engine.m_GameStatus = tEngine::eGS_DEFEAT;
}