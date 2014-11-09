
//Predefine class
class TCODColor;
class TCODMap;

struct tTile 
{
    bool explored; // can we walk through this tile?
    tTile() : explored(false) {}
};
 
class tMap 
{
public :
     tMap(int width, int height);
    ~tMap();
    bool isWall(int x, int y) const;
    bool isInFov(int x, int y) const;
    bool isExplored(int x, int y) const;
    void computeFov();
    void render() const;
    bool canWalk(int x, int y) const;
    void addMonster(int x, int y);

protected :  
    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2);

public:
    int m_Width;
    int m_Height;

protected :
    tTile * m_pTiles;
    TCODMap *m_pMap;
    friend class BspListener;

private:
    static const TCODColor m_cDarkWallColor;
    static const TCODColor m_cDarkGroundColor;
    static const TCODColor m_cLightWall;
    static const TCODColor m_cLightGround;
};
