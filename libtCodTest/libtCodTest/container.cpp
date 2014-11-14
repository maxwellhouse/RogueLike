#include "main.h"

tContainer::tContainer(int size):
m_Size(size)
{

}

tContainer::~tContainer()
{
    m_Inventory.clearAndDelete();
}

bool tContainer::add(tActor* pActor)
{
    if( m_Size > 0 && m_Inventory.size() >= m_Size )
    {
        //inventory full
        return false;
    }
    m_Inventory.push(pActor);
    return true;
}

void tContainer::remove(tActor* pActor)
{
    m_Inventory.remove(pActor);
}