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