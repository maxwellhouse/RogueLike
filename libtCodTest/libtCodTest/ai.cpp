#include "main.h"

void tPlayerAi::update(tActor* pOwner)
{
	if (pOwner->m_pDestructible && pOwner->m_pDestructible->isDead())
	{
		return;
	}

	int dx = 0;
	int dy = 0;
	switch (engine.m_LastKey)
	{
	default:
		break;
	}
}