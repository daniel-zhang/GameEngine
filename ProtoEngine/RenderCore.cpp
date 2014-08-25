#include "RenderCore.h"
#include "RenderInterface.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include <windows.h>
#include "ConfigMgr.h"
#include "Singleton.h"

#include "TestHook.h"

XMVECTORF32 ColorPalette::LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};


RenderCore::RenderCore()
{
    mRI = new RenderInterface;
    mMainWindow = new RenderWindow;
}

RenderCore::~RenderCore()
{
    delete mMainWindow;
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
    test_hook(at_render_core_init);

    // Extract config obj
    RenderConfig& rc = Singleton<ConfigMgr>::getInstance().root.render_config;

    // Create device/context
    if( mRI->init() == false)
        return false;

    initPhaseTwoSingletons(mRI);
    test_hook(after_phase_2_singletons);

    // Init render window
    if( mMainWindow->init(L"Test Main Window", 0, 0, rc.screen_width, rc.screen_height) == false)
        return false;

    // Init default render target
    /*
    if ( mDefaultTarget->init(mRI, mMainWindow) == false)
        return false;

    mRI->setRenderTarget(mDefaultTarget);
    mRI->setSwapChain(mDefaultTarget);
    */
    Viewport* vp = mRI->createViewport(mMainWindow);
    mRI->setViewport(vp);

    return true;
}

int RenderCore::draw()
{
    // place holder 
    mRI->clearBackground(XMFLOAT4(0.69f, 0.77f, 0.87f, 1.0f));
    mRI->presentBackBuffer();

    return 0;
}

bool RenderCore::exit()
{
    clearPhaseTwoSingletons(mRI);
    return true;
}


