
//Predefine classes
class tMap;
class tActor;

class tEngine 
{
public :
    tEngine();
	tEngine(int screenWidth, int screenHeight);
    ~tEngine();
    void update();
    void render();
	void sendToBack(tActor* pActor);

public:
    TCODList<tActor *> m_Actors;
    tActor * m_pPlayer;
    tMap * m_pMap;
    int m_FovRadius;
	int m_ScreenWidth;
	int m_ScreenHeight;
	TCOD_key_t m_LastKey;

public:
enum eGameStatus {
        eGS_STARTUP,
        eGS_IDLE,
        eGS_NEWTURN,
        eGS_VICTORY,
        eGS_DEFEAT
    } m_GameStatus;
};
 
extern tEngine engine;