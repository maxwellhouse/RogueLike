// libtCodTest.cpp : Defines the entry point for the console application.
//

#include "main.h"

tEngine engine(80,50);

int main(int argc, wchar_t* argv[])
{
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    engine.load();
    while ( !TCODConsole::isWindowClosed() )
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    engine.save();
    return 0;
}

