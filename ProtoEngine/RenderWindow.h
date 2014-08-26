#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <string>
#include "reference.h"

class EventHandlerInterface;
class RenderWindow
{
public:
    RenderWindow();
    ~RenderWindow();

    bool isInitialized(){return mInitialized;}
    void hookEventHandler(EventHandlerInterface* evi);
    void unhookEventHandler();
    bool init(std::wstring title, int x, int y, int width, int height);
    void setTitleBarText(std::wstring& input);
    HWND getReference();
    uint32 getWidth();
    uint32 getHeight();

    static LRESULT CALLBACK winProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    enum EWindowState{
        ws_normal = 0,
        ws_minimized, 
        ws_maximized,
        ws_resizing,
        ws_lostfocus,
        ws_uninitialized
    };
    EWindowState mState;
    bool mInitialized;

    EventHandlerInterface* mCurrentHandler;
    std::wstring mTitle, mClassName;
    int mX, mY;
    int mWidth, mHeight;
    HWND mHwnd;
};

#endif
