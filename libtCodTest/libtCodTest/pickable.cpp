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
