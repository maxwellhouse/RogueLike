
//Predefine class
class TCODColor;
class tAttacker;
class tDestructible;
class tAi;
class tPickable;
class tContainer;

class tActor 
{
public :
    tActor(int xPos, int yPos, int asciiCode, const char *name, const TCODColor &color);
    ~tActor();

    void update();
    void render() const;

    float getDistance(int cx, int cy) const;

    int m_XPosition;
    int m_YPosition; // position on map
    int m_AsciiCode; // ascii code
    TCODColor m_Color; // color
    const char *m_pName; // the actor's name
	bool m_Blocks; // can we walk on this actor
	tAttacker *m_pAttacker; //something that deals damage
	tDestructible *m_pDestructible; // something that can be damaged
	tAi* m_pAI; // something self updating
    tPickable* m_pPickable;  // something that can be picked up and used
    tContainer* m_pContainer; // something that can contain other actors
};