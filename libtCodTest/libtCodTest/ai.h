class tActor;

class tAi
{
public:
	virtual void update(tActor* pOwner) = 0;
};

class tPlayerAi : public tAi
{
public:
	void update(tActor* pOwner);

protected:
	bool moveOrAttack(tActor* pOwner, int targetX, int targetY);
};