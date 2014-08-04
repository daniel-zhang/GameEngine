#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "reference.h"
#include <Windows.h>
#include <string>

class RenderWindow
{
public:
    RenderWindow();
    ~RenderWindow();

    bool init(std::wstring title, int x, int y, int width, int height );
    HWND getReference();

    static LRESULT CALLBACK winProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    std::wstring mTitle, mClassName;
    int mX, mY, mWidth, mHeight;
    HWND mHwnd;
};

#endif
