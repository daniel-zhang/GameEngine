#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <string>
#include "reference.h"

class InputHandlerInterface;
class RenderWindow
{
public:
    RenderWindow();
    ~RenderWindow();

    bool isInitialized(){return mInitialized;}
    void hookHandler(InputHandlerInterface* evi);
    void unhookHandler();
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

    InputHandlerInterface* mCurrentHandler;
    std::wstring mTitle, mClassName;
    int mX, mY;
    int mWidth, mHeight;
    HWND mHwnd;
};

#endif
