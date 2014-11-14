#include "main.h"
#include <stdio.h>
#include <stdarg.h>

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH + 2;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;

tGui::tGui()
{
    m_pConsole = new TCODConsole(engine.m_ScreenWidth, PANEL_HEIGHT);
}

tGui::~tGui()
{
    delete m_pConsole;
    m_MessageLog.clearAndDelete();
}

void tGui::render()
{
    //Clear the gui console
    m_pConsole->setDefaultBackground(TCODColor::black);
    m_pConsole->clear();

    renderBar( 1, 1, BAR_WIDTH, engine.m_pPlayer->m_pName, engine.m_pPlayer->m_pDestructible->m_CurrentHP, engine.m_pPlayer->m_pDestructible->m_MaxHP, TCODColor::lightRed, TCODColor::darkerRed );

    //Draw the message log
    int y = 1;
    float colorCoef = 0.4f;
    for(tMessage** iterator = m_MessageLog.begin(); iterator != m_MessageLog.end(); ++iterator)
    {
        tMessage* pMessage = *iterator;
        m_pConsole->setDefaultForeground(pMessage->color * colorCoef);
        m_pConsole->print(MSG_X, y, pMessage->text);
        y++;
        if( colorCoef < 1.0f )
        {
            colorCoef += 0.3f;
        }
    }

    renderMouseLook();

    //Blit the bar onto the console
    TCODConsole::blit(m_pConsole, 0,0, engine.m_ScreenWidth, PANEL_HEIGHT, TCODConsole::root, 0, engine.m_ScreenHeight - PANEL_HEIGHT);
}

void tGui::renderBar(const int x, const int y, const int width, const char* name, const float value, const float maxValue, const TCODColor &barColor, const TCODColor& backColor)
{
    m_pConsole->setDefaultBackground(backColor);
    m_pConsole->rect(x,y, width, 1, false, TCOD_BKGND_SET);

    int barWidth = static_cast<int>(value / maxValue * width);
    if(barWidth > 0)
    {
        //draw the bar
        m_pConsole->setDefaultBackground(barColor);
        m_pConsole->rect(x,y,barWidth,1, false, TCOD_BKGND_SET);
        // write the values on the bar
        m_pConsole->setDefaultForeground(TCODColor::white);
        m_pConsole->printEx(x+width/2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g / %g", name, value, maxValue);
    }
}

void tGui::renderMouseLook()
{
    if( engine.m_pMap->isInFov(engine.m_Mouse.cx, engine.m_Mouse.cy) )
    {
        return;
    }
    char buf[129] = "";
    bool first = true;
    for(tActor** iterator = engine.m_Actors.begin(); iterator != engine.m_Actors.end(); ++iterator)
    {
        tActor* pActor = *iterator;
        if(pActor->m_XPosition == engine.m_Mouse.cx && pActor->m_YPosition == engine.m_Mouse.cy)
        {
            if(first == false)
            {
                strcat_s(buf, ", ");
            }
            else
            {
                first = false;
            }
            strcat_s(buf, pActor->m_pName);
        }
    }
    //display the list of actors under the mouse cursor
    m_pConsole->setDefaultForeground(TCODColor::lightGrey);
    m_pConsole->print(1,0,buf);
}

void tGui::message(const TCODColor& color, const char* text, ...)
{
    //build the text
    va_list ap;
    va_start(ap, text);
    int size =_vscprintf(text, ap);
    char* buf = new char[size];
    vsnprintf_s(buf, size, _TRUNCATE, text, ap);
    va_end(ap);

    char* pLineBegin = buf;
    char* pLineEnd;
    do
    {
        //make room for the next message
        if(m_MessageLog.size() == MSG_HEIGHT)
        {
            tMessage* toRemove = m_MessageLog.get(0);
            m_MessageLog.remove(toRemove);
            delete toRemove;
        }
        //detect the end of the line
        pLineEnd = strchr(pLineBegin, '\n');
        if(pLineEnd)
        {
            *pLineEnd = '\0';
        }
        // add new message to log
        tMessage* newMessage = new tMessage(pLineBegin, color);
        m_MessageLog.push(newMessage);
        pLineBegin = pLineEnd+1;
    } while( pLineEnd );

    delete buf;
}

tGui::tMessage::tMessage(const char* text, const TCODColor& color):
color(color)
{
    this->text = new char[strlen(text)+1];
    strcpy_s(this->text, strlen(text)+1, text);
}

tGui::tMessage::~tMessage()
{
    delete[] text;
}