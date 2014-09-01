#include <string>
#include "Game.h"
#include "Profiler.h"
#include "ConfigMgr.h"
#include "RenderInterface.h"
#include "TestHook.h"

/////////////////////////////////////////////////////////////////////////

Win32EventHander::Win32EventHander()
{
    mGame = NULL;
}

void Win32EventHander::bindImple( Game* game )
{
    mGame = game;
}
bool Win32EventHander::isValid()
{
    return (mGame != NULL );
}

void Win32EventHander::pause()
{
    mGame->onPause();
}

void Win32EventHander::restore()
{
    mGame->onRestore();
}

void Win32EventHander::resize( int newWidth, int newHeight )
{
    mGame->onResize((float)newWidth, (float)newHeight);
}


/////////////////////////////////////////////////////////////////////////
Game::Game()
{
    // Heap resource section: begin
    mRenderCore = NULL;
    mGameWindow = NULL;
    mScene      = NULL;
    // Heap resource section: end

    mInitialized      = false;
    mGamePaused       = false;
    mRenderCorePaused = false;

    mFrameDebugInfo.precision(4);
    mGameTimer           = NULL;
    mRenderTimer         = NULL;
    mInitTimer           = NULL; 
    mEnableStat          = true;
    mFrameTimeSum        = 0.f;
    mGameTimeSum         = 0.f;
    mRenderTimeSum       = 0.f; 
    mStatInterval        = 300.f; // Gives average stat every 300ms
    mFrameCounter        = 0;
    mRenderCoreDebugInfo = L"";
}

Game::~Game()
{
    release_raw_ptr(&mScene);
    release_raw_ptr(&mRenderCore);
    release_raw_ptr(&mGameWindow);
}

bool Game::init()
{
    // Prevent double init
    if (mInitialized)
        return false;

    test_hook(at_game_init);
    initPhaseOneSingletons();

    mGameTimer = Singleton<Profiler>::getInstance().createTimer(L"LogicStats", L"Logic Stats");
    mRenderTimer = Singleton<Profiler>::getInstance().createTimer(L"RenderStats", L"Render Stats");
    mInitTimer = Singleton<Profiler>::getInstance().createTimer(L"InitTimer", L"Init Timer");

    Singleton<Profiler>::getInstance().startTimer(mInitTimer);

    //
    // Extract config obj
    //
    RenderConfig& rc = Singleton<ConfigMgr>::getInstance().root.render_config;

    //
    // Init RenderWindow
    //
    mGameWindow = new RenderWindow();
    if (false == mGameWindow->init(L"Test Main Window", 0, 0, rc.screen_width, rc.screen_height) ) return false;

    //
    // Init RenderCore
    //
    mRenderCore = new RenderCore();
    if (false == mRenderCore->init(mGameWindow)) return false;

    //
    // Init Scene
    //
    mScene = new Scene();
    mScene->initFromBuilder(&buildDefaultScene, mRenderCore->getRenderInterface());

    //
    // After init: bind after RenderCore is initialized
    //
    mEventHandler.bindImple(this);
    mGameWindow->hookHandler(&mEventHandler);

    Singleton<Profiler>::getInstance().endTimer(mInitTimer);
    float time = mInitTimer->totalTime();

    mInitialized = true;
    return mInitialized;
}

bool Game::exit()
{
    mGameWindow->unhookHandler();

    if (mRenderCore && mRenderCore->isInitialized())
    {
        mRenderCore->exit();
        delete mRenderCore;
        mRenderCore = NULL;
    }

    clearPhaseOneSingletons();

    mInitialized = false;
    return true;
}

int32 Game::runWin32()
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
            mTicker.tick();

            //
            // Game Frame
            //
            Singleton<Profiler>::getInstance().startTimer(mGameTimer);
            step(mTicker.delta_second());
            Singleton<Profiler>::getInstance().endTimer(mGameTimer);

            //
            // Render Frame
            //
            Singleton<Profiler>::getInstance().startTimer(mRenderTimer);
            driveRenderCore();
            Singleton<Profiler>::getInstance().endTimer(mRenderTimer);

            //
            // Calculate and display frame info text
            //
            calcFrameTime();
        }
    }
    return (int)msg.wParam;
}

void Game::step(float delta)
{
    //if (mGamePaused) return;
    
    mScene->update(delta);
}

void Game::driveRenderCore()
{
    //if (mRenderCorePaused) return;

    mScene->drawSelf(mRenderCore->getRenderInterface());
    //mRenderCore->draw();
}

void Game::calcFrameTime()
{
    mFrameTimeSum  += mTicker.delta_millisecond();
    mGameTimeSum   += mGameTimer->lastDelta_millisecond();
    mRenderTimeSum += mRenderTimer->lastDelta_millisecond();
    mFrameCounter++;

    if (mFrameTimeSum > mStatInterval)
    {
        float avgFrame = mFrameTimeSum / mFrameCounter;
        float avgGame = mGameTimeSum / mFrameCounter;
        float avgRendering = mRenderTimeSum / mFrameCounter;
        float avgFPS = 1000.f / avgFrame;
        float avgMisc = avgFrame - (avgRendering + avgGame);

        mFrameTimeSum = 0.f;
        mGameTimeSum = 0.f;
        mRenderTimeSum = 0.f;
        mFrameCounter = 0;

        mFrameDebugInfo 
            << L"Frame: " << avgFrame 
            << L"ms | Game: " << avgGame 
            << L"ms | Render: " << avgRendering 
            << L"ms | Misc: " << avgMisc
            << L"ms | FPS: " << avgFPS;
        mGameWindow->setTitleBarText(mFrameDebugInfo.str());
        mFrameDebugInfo.str(L"");
    }
}

RenderCore* Game::getRenderCore()
{
    return mRenderCore;
}

void Game::onPause()
{
    mGamePaused = true;
    mRenderCorePaused = true;
}

void Game::onRestore()
{
    mGamePaused = false;
    mRenderCorePaused = false;
}

void Game::onResize( float width, float height )
{
    RenderInterface* ri = mRenderCore->getRenderInterface();
    ri->resizeViewport(width, height);
    mScene->getActiveCam().updateAspect(ri->getViewportAspect());
}

