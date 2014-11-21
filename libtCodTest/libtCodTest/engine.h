
//Predefine classes
class tMap;
class tActor;
class tGui;

class tEngine
{
public :
	tEngine(int screenWidth, int screenHeight);
    ~tEngine();
    void update();
    void render();
	void sendToBack(tActor* pActor);
    tActor* getClosestMonster(int x, int y, float range) const;
    bool chooseATile(int* x, int* y, float maxRange = 0.0f);
    tActor* getActor(int x, int y);
    void init();
    void load();
    void save();

public:
    TCODList<tActor *> m_Actors;
    tActor * m_pPlayer;
    tMap * m_pMap;
    int m_FovRadius;
	int m_ScreenHeight;
	int m_ScreenWidth;
    tGui* m_pGui;
	TCOD_key_t m_LastKey;
    TCOD_mouse_t m_Mouse;

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
