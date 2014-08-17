#include "RenderCore.h"
#include "RenderInterface.h"
#include "SwapChain.h"
#include "RenderWindow.h"
#include <windows.h>
#include "ConfigMgr.h"
#include "Singleton.h"

RenderCore::RenderCore()
{
    mRI = new RenderInterface;
    mDefaultTarget = new SwapChain;
    mMainWindow = new RenderWindow;
}

RenderCore::~RenderCore()
{
    delete mMainWindow;
    delete mDefaultTarget;
    delete mRI;
}

/*
init order
. config reading
. create device and context
. render window
. render target
*/
bool RenderCore::init()
{
    // Extract config obj
    RenderConfig& rc = Singleton<ConfigMgr>::getInstance().root.render_config;

    // Create device/context
    if( mRI->init() == false)
        return false;

    initPhaseTwoSingletons(mRI);

    // Init render window
    if( mMainWindow->init(L"Test Main Window", 0, 0, rc.screen_width, rc.screen_height))
        return false;

    // Init default render target
    if ( mDefaultTarget->init(mRI, mMainWindow) == false)
        return false;

    mRI->attachRenderTarget(mDefaultTarget);

    return true;
}

int RenderCore::draw()
{
    // place holder 
    return 0;
}

bool RenderCore::exit()
{
    clearPhaseTwoSingletons(mRI);
    return true;
}

