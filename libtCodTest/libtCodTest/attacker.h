class tActor;

class tAttacker
{
public:
	float m_Power; // hit points given

	tAttacker(float power);
	void attack(tActor* pOwner, tActor* pTarget)
};