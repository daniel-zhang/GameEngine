#include "RenderCore.h"
#include "RenderInterface.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "ConfigMgr.h"
#include "Singleton.h"
#include "Game.h"

#include "TestHook.h"

XMVECTORF32 ColorPalette::LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};


RenderCore::RenderCore()
{
    // Owned stuff
    mRI = NULL;
    mInitialized = false;

    // Referenced stuff
    mMainWindow = NULL;
}

RenderCore::~RenderCore()
{
    if (mRI)
    {
        delete mRI;
        mRI = NULL;
    }
}

bool RenderCore::init(RenderWindow* rw)
{
    if (mInitialized) return false;
    test_hook(at_render_core_init);

    mMainWindow = rw;

    // Init RenderInterface
    mRI = new RenderInterface();
    if (mRI == NULL) return false;
    if (mRI->init() == false) return false;

    // Phase 2 singletons are dependent of RI
    initPhaseTwoSingletons(mRI);
    test_hook(after_phase_2_singletons);

    // Create viewport from game window
    if(mRI->createViewport(mMainWindow) == false) return false;

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

bool RenderCore::isInitialized()
{
    return mInitialized;
}

RenderInterface* RenderCore::getRenderInterface()
{
    return mRI;
}

