#include "main.h"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;

const TCODColor tMap::m_cDarkWallColor(0,0,100);
const TCODColor tMap::m_cDarkGroundColor(50,50,150);
const TCODColor tMap::m_cLightWall(130,110,50);
const TCODColor tMap::m_cLightGround(200,180,50);

class BspListener : public ITCODBspCallback
{
private :
    tMap &map; // a map to dig
    int roomNum; // room number
    int lastx,lasty; // center of the last room

public :
    BspListener(tMap &map) : map(map), roomNum(0) {}

bool visitNode(TCODBsp *node, void *userData)
{
    if ( node->isLeaf() )
    {
        int x,y,w,h;
        // dig a room
        TCODRandom *rng = TCODRandom::getInstance();
        w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
        h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
        x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
        y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
        map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);
        if ( roomNum != 0 )
        {
            // dig a corridor from last room
            map.dig(lastx, lasty, x + w / 2, lasty);
            map.dig(x + w / 2, lasty, x + w / 2, y + h / 2);
        }
            lastx = x + w / 2;
            lasty = y + h / 2;
            roomNum++;
        }
        return true;
    }
};

tMap::tMap(int width, int height) :
    m_Width(width),
    m_Height(height)
{
    m_pTiles = new tTile[m_Width * m_Height];
    m_pMap = new TCODMap(m_Width, m_Height);
    TCODBsp bsp(0, 0, m_Width, m_Height);
    bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener,NULL);
}

tMap::~tMap()
{
    delete [] m_pTiles;
    delete m_pMap;
}

bool tMap::isWall(int x, int y) const
{
    return ! (m_pMap->isWalkable(x,y));
}

bool tMap::canWalk(int x, int y) const
{
    if(isWall(x,y))
    {
        return false;
    }
    for (tActor **iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); iterator++)
    {
        tActor* pActor = *iterator;
        if ( pActor->m_Blocks && pActor->m_XPosition == x && pActor->m_YPosition == y )
        {
            // there is an actor there. cannot walk
            return false;
        }
    }
    return true;
}

bool tMap::isExplored(int x, int y) const
{
    return m_pTiles[x + y * m_Width].explored;
}

bool tMap::isInFov(int x, int y) const
{
    if( x < 0 || x > m_Width || y < 0 || y > m_Height )
    {
        return false;
    }
    if ( m_pMap->isInFov(x,y) )
    {
        m_pTiles[x + y * m_Width].explored = true;
        return true;
    }
    return false;
}

void tMap::computeFov()
{
    m_pMap->computeFov(engine.m_pPlayer->m_XPosition, engine.m_pPlayer->m_YPosition, engine.m_FovRadius);
}

void tMap::dig(int x1, int y1, int x2, int y2)
{
    if ( x2 < x1 )
    {
        int tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    if ( y2 < y1 )
    {
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }
    for (int tilex = x1; tilex <= x2; tilex++)
    {
        for (int tiley = y1; tiley <= y2; tiley++)
        {
            m_pMap->setProperties(tilex, tiley, true, true);
        }
    }
}

void tMap::createRoom(bool first, int x1, int y1, int x2, int y2)
{
    dig (x1,y1,x2,y2);
    if ( first )
    {
        // put the player in the first room
        engine.m_pPlayer->m_XPosition = (x1 + x2) / 2;
        engine.m_pPlayer->m_YPosition = (y1 + y2) / 2;
    }
    else
    {
        TCODRandom *rng = TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbMonsters > 0)
        {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if ( canWalk(x,y) )
            {
                addMonster(x,y);
            }
            nbMonsters--;
        }

        int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
        while(nbItems > 0)
        {
            int x = rng->getInt(x1,x2);
            int y = rng->getInt(y1,y2);
            if(canWalk(x,y))
            {
                addItem(x,y);
            }
            nbItems--;
        }
    }
}

void tMap::render() const
{
    for (int x = 0; x < m_Width; x++)
    {
        for (int y = 0; y < m_Height; y++)
        {
            if ( isInFov(x,y) )
            {
                TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? m_cLightWall :m_cLightGround );
            }
            else if ( isExplored(x,y) )
            {
                TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? m_cDarkWallColor : m_cDarkGroundColor );
            }
        }
    }
}

void tMap::addMonster(int x, int y)
{
    TCODRandom *rng=TCODRandom::getInstance();
    if ( rng->getInt(0,100) < 80 )
    {
        // create an orc
		tActor* pOrc = new tActor(x, y, 'o', "orc", TCODColor::desaturatedGreen);
		pOrc->m_pDestructible = new tMonsterDestructible(10, 0, "dead orc");
		pOrc->m_pAttacker = new tAttacker(3);
		pOrc->m_pAI = new tMonsterAi();
        engine.m_Actors.push(pOrc);
    }
    else
    {
        // create a troll
		tActor* pTroll = new tActor(x, y, 'T', "troll", TCODColor::darkerGreen);
		pTroll->m_pDestructible = new tMonsterDestructible(16, 1, "dead troll");
		pTroll->m_pAttacker = new tAttacker(4);
		pTroll->m_pAI = new tMonsterAi();
		engine.m_Actors.push(pTroll);
    }
}

void tMap::addItem(int x, int y)
{
    tActor* pHealthPotion = new tActor(x,y, '!', "Health Potion", TCODColor::violet);
    pHealthPotion->m_Blocks = false;
    pHealthPotion->m_pPickable = new tHealer(5);
    engine.m_Actors.push(pHealthPotion);
}
