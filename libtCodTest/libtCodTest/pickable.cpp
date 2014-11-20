#include "main.h"

bool tPickable::pick(tActor* pOwner, tActor* pWearer)
{
    if(pWearer->m_pContainer && pWearer->m_pContainer->add(pOwner))
    {
        engine.m_Actors.remove(pOwner);
        return true;
    }
    return false;
}

bool tPickable::use(tActor* pOwner, tActor* pWearer)
{
    if(pWearer->m_pContainer)
    {
        pWearer->m_pContainer->remove(pOwner);
        delete pOwner;
        return true;
    }
    return false;
}

void tPickable::drop(tActor* pOwner, tActor* pWearer)
{
    if(pWearer->m_pContainer)
    {
        pWearer->m_pContainer->remove(pOwner);
        engine.m_Actors.push(pOwner);
        pOwner->m_XPosition = pWearer->m_XPosition;
        pOwner->m_YPosition = pWearer->m_YPosition;
        engine.m_pGui->message(TCODColor::lightGrey, "%s drops a %s.", pWearer->m_pName, pOwner->m_pName);
    }
}

tHealer::tHealer(const float amount):
m_Amount(amount)
{
}

bool tHealer::use(tActor* pOwner, tActor* pWearer)
{
    if(pWearer->m_pDestructible)
    {
        float amountHealed = pWearer->m_pDestructible->heal(m_Amount);
        if(amountHealed > 0)
        {
            return tPickable::use(pOwner, pWearer);
        }
    }
    return false;
}

tLightningBolt::tLightningBolt(const float range, const float damage):
m_Range(range),
m_Damage(damage)
{
}

bool tLightningBolt::use(tActor* pOwner, tActor* pWearer)
{
    tActor* pClosestMonster = engine.getClosestMonster(pWearer->m_XPosition, pWearer->m_YPosition, m_Range);
    if( !pClosestMonster )
    {
        engine.m_pGui->message(TCODColor::lightGrey, "No enemy is close enough to strike.");
        return false;
    }
    else
    {
        engine.m_pGui->message(TCODColor::lightBlue, "A lightning bolt strikes the %s with a loud thunder\n%g damage!", pClosestMonster->m_pName, m_Damage);
        pClosestMonster->m_pDestructible->takeDamage(pClosestMonster, m_Damage);
        return tPickable::use(pOwner, pWearer);
    }
}

tFireball::tFireball(const float range, const float damage):
tLightningBolt(range,damage)
{
}

bool tFireball::use(tActor* pOwner, tActor* pWearer)
{
    engine.m_pGui->message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
    int x,y;
    if(engine.chooseATile(&x,&y,m_Range) == false)
    {
        return false;
    }
    engine.m_pGui->message(TCODColor::orange, "The fireball explodes, burning everything within %g tiles!", m_Range);
    for(tActor** iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); ++iterator)
    {
        tActor* pActor = *iterator;
        if(pActor->m_pDestructible && pActor->m_pDestructible->isDead() == false && pActor->getDistance(x,y) <= m_Range)
        {
            engine.m_pGui->message(TCODColor::orange, "The %s gets burned for %g damage.", pActor->m_pName, m_Damage);
            pActor->m_pDestructible->takeDamage(pActor, m_Damage);
        }
    }
    tPickable::use(pOwner, pWearer);
    return true;
}

tConfuse::tConfuse(int nbTurns, int range):
m_nbTurns(nbTurns),
m_Range(range)
{
}

bool tConfuse::use(tActor* pOwner, tActor* pWearer)
{
    engine.m_pGui->message(TCODColor::cyan, "Left-click and enemy to confuse it, or right-click to cancel.");
    int x,y;
    if(engine.chooseATile(&x,&y,static_cast<float>(m_Range)) == false)
    {
        return false;
    }
    tActor* pActor = engine.getActor(x,y);
    if(pActor)
    {
        tConfusedMonsterAi* newAi = new tConfusedMonsterAi( m_nbTurns, pOwner->m_pAI);
        pOwner->m_pAI = newAi;
    }
    else
    {
        return false;
    }
    engine.m_pGui->message(TCODColor::lightGreen, "The eyes of the %s look vacant,\nas he starts to stumble around!", pActor->m_pName);
    return tPickable::use(pOwner, pWearer);
}