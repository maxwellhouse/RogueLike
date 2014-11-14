class tActor;

class tContainer
{
public:
    tContainer(int size);
    ~tContainer();

    bool add(tActor* pActor);
    void remove(tActor* pActor);

    int m_Size;
    TCODList<tActor*> m_Inventory;
};