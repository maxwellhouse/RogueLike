class tActor;

class tDestructible
{
public:
	float m_MaxHP;  // maximum number of health points
	float m_CurrentHP; // current number of health points
	float m_Defense; // hit points deflected
	const char* m_pCorpseName; // the actor's name once destroyed

	tDestructible(float maxHP, float defense, const char* corpseName);
	inline bool isDead() { return m_CurrentHP <= 0; }

	float takeDamage(tActor* pOwner, float damage);
    float heal(float amount);
	virtual void die(tActor* pOwner);
};

class tMonsterDestructible : public tDestructible
{
public:
	tMonsterDestructible(float maxHP, float defense, const char* corpseName);
	void die(tActor* pOwner);
};

class tPlayerDestructible : public tDestructible
{
public:
	tPlayerDestructible(float maxHP, float defense, const char* corpseName);
	void die(tActor* pOwner);
};