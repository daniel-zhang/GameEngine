#include "RenderWindow.h"
#include "Game.h"
#include <WindowsX.h>	// To enable GET_X_LPARAM and GET_Y_LPARAM macros

InputHandlerInterface gNullHandler;
RenderWindow::RenderWindow()
{
    mClassName = L"RenderWindow";
    mHwnd = NULL;
    mX = mY = 0;
    mWidth = mHeight = 0;
    mState = ws_uninitialized;
    mInitialized = false;
    mCurrentHandler = &gNullHandler; // Use null handler by default
}

RenderWindow::~RenderWindow()
{
}

bool RenderWindow::init( std::wstring title, int x, int y, int width, int height )
{
    if (mInitialized) return false;
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
    //wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hCursor       = LoadCursor(0, IDC_CROSS);
    wc.lpszMenuName  = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// Set default background as white.

    RegisterClass(&wc);

    mHwnd = CreateWindowEx(
        0, mClassName.c_str(), mTitle.c_str(), WS_OVERLAPPEDWINDOW, x, y,
        width, height, 0, 0, GetModuleHandle(NULL), this);

    ShowWindow(mHwnd, SW_SHOWNORMAL);
    UpdateWindow(mHwnd);

    if (mHwnd)
    {
        mState = ws_normal;
        mInitialized = true;
        return mInitialized;
    }
    return false;
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

        //Sent to both the window being activated and the window being deactivated
        case WM_ACTIVATE:
            if( LOWORD(wParam) == WA_INACTIVE )
            {
                pThis->mState = ws_lostfocus;
                pThis->mCurrentHandler->pause();
            }
            else
            {
                pThis->mState = ws_normal;
                pThis->mCurrentHandler->restore();
            }
            return 0;

        case WM_SIZE:
            // Keep local width/height in sync with UI
            pThis->mWidth = LOWORD(lParam);
            pThis->mHeight = HIWORD(lParam);
            // min button clicked
            if(wParam == SIZE_MINIMIZED)
            {
                pThis->mState = ws_minimized;
                pThis->mCurrentHandler->pause();
            }
            // max button clicked
            else if (wParam == SIZE_MAXIMIZED)
            {
                pThis->mState = ws_maximized;
                pThis->mCurrentHandler->resize(pThis->mWidth, pThis->mHeight);
                pThis->mCurrentHandler->restore();
            }
            else if( wParam == SIZE_RESTORED )
            {
                // restore button or task bar clicked
                if (pThis->mState == ws_minimized || pThis->mState == ws_maximized)
                {
                    pThis->mState = ws_normal;
                    pThis->mCurrentHandler->resize(pThis->mWidth, pThis->mHeight);
                    pThis->mCurrentHandler->restore();
                }
                else if(pThis->mState == ws_resizing)
                {
                    
                }
                else
                    pThis->mCurrentHandler->resize(pThis->mWidth, pThis->mHeight);
            }
            return 0;
            
        case WM_ENTERSIZEMOVE:
            pThis->mState = ws_resizing;
            pThis->mCurrentHandler->pause();
            return 0;

        case WM_EXITSIZEMOVE:
            pThis->mState = ws_normal;
            pThis->mCurrentHandler->resize(pThis->mWidth, pThis->mHeight);
            pThis->mCurrentHandler->restore();
            return 0;

        // The WM_MENUCHAR message is sent when a menu is active and the user presses 
        // a key that does not correspond to any mnemonic or accelerator key. 
        case WM_MENUCHAR:
            // Don't beep when we alt-enter.
            return MAKELRESULT(0, MNC_CLOSE);

        // Catch this message so to prevent the window from becoming too small.
        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
            return 0;

        case WM_LBUTTONDOWN:
            if (wParam & MK_LBUTTON)
            {
                pThis->mCurrentHandler->onLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            return 0;

        case WM_LBUTTONUP:
            pThis->mCurrentHandler->onLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;

        case WM_RBUTTONDOWN:
            if (wParam & MK_RBUTTON)
            {
                pThis->mCurrentHandler->onRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
            return 0;

        case WM_RBUTTONUP:
            pThis->mCurrentHandler->onRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;

        case WM_MOUSEMOVE:
            pThis->mCurrentHandler->onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
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

void RenderWindow::hookHandler( InputHandlerInterface* evi )
{
    mCurrentHandler = evi;
}

void RenderWindow::unhookHandler()
{
    mCurrentHandler = &gNullHandler;
}
