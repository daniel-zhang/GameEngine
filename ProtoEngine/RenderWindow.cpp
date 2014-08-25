#include "RenderWindow.h"

RenderWindow::RenderWindow()
{
    mClassName = L"RenderWindow";
    mHwnd = NULL;
    mX = mY = 0;
    mWidth = mHeight = 0;
}

RenderWindow::~RenderWindow()
{

}

bool RenderWindow::init( std::wstring title, int x, int y, int width, int height )
{
    mTitle = title;
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;

    WNDCLASS wc = {0};
    wc.lpfnWndProc   = RenderWindow::winProc;
    wc.hInstance     = GetModuleHandle(NULL);	// Get the current handle(in a single-threaded environment)

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.lpszClassName = mClassName.c_str();
    wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.lpszMenuName  = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// Set default background as white.

    RegisterClass(&wc);

    mHwnd = CreateWindowEx(
        0, mClassName.c_str(), mTitle.c_str(), WS_OVERLAPPEDWINDOW, x, y,
        width, height, 0, 0, GetModuleHandle(NULL), this);

    ShowWindow(mHwnd, SW_SHOWNORMAL);
    UpdateWindow(mHwnd);


    return mHwnd ? true : false;
}

HWND RenderWindow::getReference()
{
    return mHwnd;
}

LRESULT CALLBACK RenderWindow::winProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    RenderWindow* pThis = NULL;
    if (uMsg == WM_NCCREATE)
    {
        //Extract "this" pointer. 
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<RenderWindow*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        //mHwnd is better assigned here, so our handleMsg() can process WM_NCCREATE
        //and WM_CREATE messages.
        pThis->mHwnd = hwnd;
    }
    else
    {
        pThis = (RenderWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        switch(uMsg)
        {
            // WM_CLOSE is sent when user clicks close button.
            // By default, the DefWindowProc function calls DestroyWindow(...) 
            // to destroy the window.
        case WM_CLOSE:
            DestroyWindow(pThis->getReference()); // Post a WM_Destory msg
            return 0;

            // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            PostQuitMessage(0);//Post a WM_QUIT, which makes GetMessage() to return 0.
            return 0;

        default:
            return DefWindowProc(pThis->getReference(), uMsg, wParam, lParam);
        }
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void RenderWindow::setTitleBarText( std::wstring& input )
{
    SetWindowText(mHwnd, input.c_str());
}

uint32 RenderWindow::getWidth()
{
    return mWidth;
}

uint32 RenderWindow::getHeight()
{
    return mHeight;
}
