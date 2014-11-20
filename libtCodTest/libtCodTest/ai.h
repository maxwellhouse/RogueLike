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
	void handleActionKey(tActor* pOwner, int ascii);

protected:
	bool moveOrAttack(tActor* pOwner, int targetX, int targetY);
	tActor* choseFromInventory(tActor* pOwner);
};

class tMonsterAi : public tAi
{
public:
	void update(tActor* pOwner);

protected:
	void moveOrAttack(tActor* pOwner, int targetX, int targetY);

    int m_MoveCount;
};

class tConfusedMonsterAi : public tMonsterAi
{
public:
    tConfusedMonsterAi(int nbTurns, tAi* pOldAi);
    void update(tActor* pOwner);

protected:
    int m_nbTurns;
    tAi* m_pAi;
};