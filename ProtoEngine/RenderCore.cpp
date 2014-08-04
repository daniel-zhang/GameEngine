#include "RenderCore.h"
#include "RenderInterface.h"
#include "RTWindow.h"
#include <windows.h>

RenderCore::RenderCore()
{
    mRI = new RenderInterface;
    mDefaultTarget = new RTWindow;
}

RenderCore::~RenderCore()
{
    delete mRI;
    delete mDefaultTarget;
}

bool RenderCore::init()
{
    if( mRI->init(mConfig) == false)
        return false;

    if ( mDefaultTarget->init(mRI, mConfig) == false)
        return false;

    mRI->attachRenderTarget(mDefaultTarget);

    return true;
}

int RenderCore::run()
{
    MSG msg = {0};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Sleep(100);
        }
    }
    // release 
    return (int)msg.wParam;
}
