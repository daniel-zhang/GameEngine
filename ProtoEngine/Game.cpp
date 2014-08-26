#include <string>
#include "Game.h"
#include "Profiler.h"
#include "ConfigMgr.h"
#include "TestHook.h"

TestHandler::TestHandler()
{
    bStarted = true;
    mDebugInfo = L"Started";
}

void TestHandler::bindImple( Game* game )
{

}

void TestHandler::pause()
{
    mDebugInfo = L"paused";
}

void TestHandler::restore()
{
    mDebugInfo = L"started";
}

void TestHandler::resize( int newWidth, int newHeight )
{

}

/////////////////////////////////////////////////////////////////////////

Win32EventHander::Win32EventHander()
{
    mGame = NULL;
    mRenderCore = NULL;
}

void Win32EventHander::bindImple( Game* game )
{
    mGame = game;
    mRenderCore = mGame->getRenderCore();
}

void Win32EventHander::pause()
{

}

void Win32EventHander::restore()
{

}

void Win32EventHander::resize( int newWidth, int newHeight )
{

}

/////////////////////////////////////////////////////////////////////////
Game::Game()
{
    // Heap resource section: begin
    mRenderCore = NULL;
    mMainWindow = NULL;
    // Heap resource section: end

    mInitialized = false;

    mGameTimer = NULL;
    mRenderTimer = NULL;
    mInitTimer = NULL; 
    mEnableStat = true;
    mFrameDebugInfo.precision(4);
    mFrameTimeSum = 0.f;
    mGameTimeSum = 0.f;
    mRenderTimeSum = 0.f; 
    mStatInterval = 300.f; // Gives average stat every 300ms
    mFrameCounter = 0;
    mRenderCoreDebugInfo = L"";
}

Game::~Game()
{
    if (mRenderCore)
    {
        delete mRenderCore;
        mRenderCore = NULL;
    }
    if (mMainWindow)
    {
        delete mMainWindow;
        mMainWindow = NULL;
    }
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

    // Extract config obj
    RenderConfig& rc = Singleton<ConfigMgr>::getInstance().root.render_config;

    // Init RenderWindow
    mMainWindow = new RenderWindow();
    if (NULL == mMainWindow) return false;
    if (false == mMainWindow->init(L"Test Main Window", 0, 0, rc.screen_width, rc.screen_height) ) return false;

    // Init RenderCore
    mRenderCore = new RenderCore();
    if (NULL == mRenderCore) return false;
    if (false == mRenderCore->init(mMainWindow)) return false;

    Singleton<Profiler>::getInstance().endTimer(mInitTimer);
    float time = mInitTimer->totalTime();

    // Have to bind after RenderCore is initialized
    mEventHandler.bindImple(this);
    //mMainWindow->hookEventHandler(&mEventHandler);
    mMainWindow->hookEventHandler(&mTestHandler);

    mInitialized = true;
    return mInitialized;
}

bool Game::exit()
{
    mMainWindow->unhookEventHandler();

    if (mRenderCore && mRenderCore->isInitialized())
    {
        mRenderCore->exit();
        delete mRenderCore;
        mRenderCore = NULL;
    }

    clearPhaseOneSingletons();
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
    //
}

void Game::driveRenderCore()
{
    mRenderCore->draw();
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
        mMainWindow->setTitleBarText(mFrameDebugInfo.str());
        mFrameDebugInfo.str(L"");
    }
}

RenderCore* Game::getRenderCore()
{
    return mRenderCore;
}

