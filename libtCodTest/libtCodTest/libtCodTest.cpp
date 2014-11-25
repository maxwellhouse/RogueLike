// libtCodTest.cpp : Defines the entry point for the console application.
//

#include "main.h"

tEngine engine(80,50);

int main(int argc, wchar_t* argv[])
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    engine.load();
    while ( !TCODConsole::isWindowClosed() )
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    engine.save();
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

