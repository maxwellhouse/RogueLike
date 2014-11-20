class tPickable
{
public:
    bool pick(tActor* pOwner, tActor* pWearer);
    void drop(tActor* pOwner, tActor* pWearer);
    virtual bool use(tActor* pOwner, tActor* pWearer);
};

class tHealer : public tPickable
{
public:
    tHealer(const float amount);
    bool use(tActor* pOwner, tActor* pWearer);

    float m_Amount;
};

class tLightningBolt: public tPickable
{
public:
    tLightningBolt(const float range, const float damage);

    float m_Range;
    float m_Damage;
    bool use(tActor* pOwner, tActor* pWearer);
};

class tFireball: public tLightningBolt
{
public:
    tFireball(const float range, const float damage);

    bool use(tActor* pOwner, tActor* pWearer);
};

class tConfuse: public tPickable
{
public:
    tConfuse(int nbTurns, int range);
    bool use(tActor* pOwner, tActor* pWearer);

    int m_nbTurns;
    int m_Range;
};