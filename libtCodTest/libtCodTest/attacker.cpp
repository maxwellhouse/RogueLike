#include <stdio.h>
#include "main.h"

tAttacker::tAttacker(float power) :
m_Power(power)
{

}
void tAttacker::attack(tActor* pOwner, tActor* pTarget)
{
	if (pTarget->m_pDestructible && pTarget->m_pDestructible->isDead() == false)
	{
		if (m_Power - pTarget->m_pDestructible->m_Defense > 0)
		{
			engine.m_pGui->message(pOwner==engine.m_pPlayer ? TCODColor::red : TCODColor::lightGrey,
                                   "%s attacks %s for %g hit points.\n", pOwner->m_pName, pTarget->m_pName, m_Power - pTarget->m_pDestructible->m_Defense);
		}
		else
		{
			engine.m_pGui->message(TCODColor::lightGrey,"%s attacks %s but it has no effect!\n", pOwner->m_pName, pTarget->m_pName);
		}
		pTarget->m_pDestructible->takeDamage(pTarget, m_Power);
	}
	else
	{
		engine.m_pGui->message(TCODColor::lightGrey, "%s attacks %s in vain.\n", pOwner->m_pName, pTarget->m_pName);
	}
}