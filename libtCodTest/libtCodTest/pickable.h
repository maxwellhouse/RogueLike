class tPickable
{
public:
    bool pick(tActor* pOwner, tActor* pWearer);
    virtual bool use(tActor* pOwner, tActor* pWearer);
};

class tHealer : public tPickable
{
public:
    tHealer(const float amount);
    bool use(tActor* pOwner, tActor* pWearer);
    float m_Amount;
};
