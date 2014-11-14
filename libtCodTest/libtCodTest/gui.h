class TCODConsole;
class TCODColor;

class tGui
{
public:
    tGui();
    ~tGui();
    void render();
    void message(const TCODColor& color, const char* text, ...);

protected:
    TCODConsole* m_pConsole;
    void renderBar(const int x, const int y, const int width, const char* name, const float value, const float maxValue, const TCODColor &barColor, const TCODColor& backColor);
    void renderMouseLook();
    struct tMessage
    {
        char* text;
        TCODColor color;
        tMessage(const char* text, const TCODColor& color);
        ~tMessage();
    };
    TCODList<tMessage*> m_MessageLog;
};
