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

void TestHandler::bind( Game* game )
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

void Win32EventHander::bind( Game* game )
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
    // Heap resource section: end

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
    if (mMainWindow == NULL) return false;
    if (mMainWindow->init(L"Test Main Window", 0, 0, rc.screen_width, rc.screen_height, mEHI) == false) return false;

    // Init RenderCore
    if (!mRenderCore)
    {
        mRenderCore = new RenderCore();
        mRenderCore->init();
    }

    Singleton<Profiler>::getInstance().endTimer(mInitTimer);
    float time = mInitTimer->totalTime();

    // Have to bind after RenderCore is initialized
    mEventHandler.bind(this);
    return true;
}

bool Game::exit()
{
    if (mRenderCore)
    {
        mRenderCore->exit();
        delete mRenderCore;
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
        float avgMisc = avgFrame - avgFrame - avgGame;

        mFrameTimeSum = 0.f;
        mGameTimeSum = 0.f;
        mRenderTimeSum = 0.f;
        mFrameCounter = 0;

        mRenderCore->getFrameDebugInfo(mRenderCoreDebugInfo);
        mFrameDebugInfo << L"[" << mRenderCoreDebugInfo << "] | Frame: " << avgFrame 
            << L"ms | Game: " << avgGame 
            << L"ms | Render: " << avgRendering 
            << L"ms | Misc: " << avgMisc
            << L"ms | FPS: " << avgFPS;
        mRenderCore->getActiveRenderWindow()->setTitleBarText(mFrameDebugInfo.str());
        mFrameDebugInfo.str(L"");
    }
}

RenderCore* Game::getRenderCore()
{
    return mRenderCore;
}

