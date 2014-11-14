class tPickable
{
public:
    bool pick(tActor* pOwner, tActor* pWearer);
    virtual bool use(tActor* pOwner, tActor* pWearer);
};