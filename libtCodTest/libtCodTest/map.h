
//Predefine class
class TCODColor;
class TCODMap;
class roguelike_google_protocol::engine_tMap;

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
    void addItem(int x, int y);
    void init(bool withActors);
    roguelike_google_protocol::engine_tMap save();
    void load(roguelike_google_protocol::engine::tMap map);

protected :
    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);

public:
    int m_Width;
    int m_Height;

protected :
    tTile * m_pTiles;
    TCODMap *m_pMap;
    TCODRandom* m_pRandomNumGenerator;
    long m_Seed;
    friend class BspListener;

private:
    static const TCODColor m_cDarkWallColor;
    static const TCODColor m_cDarkGroundColor;
    static const TCODColor m_cLightWall;
    static const TCODColor m_cLightGround;
};
