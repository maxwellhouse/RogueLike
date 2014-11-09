// libtCodTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "engine.h"

tEngine engine;

int _tmain(int argc, _TCHAR* argv[])
{
    int playerX = 40;
    int playerY = 25;
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    while ( !TCODConsole::isWindowClosed() ) 
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    return 0;
}

