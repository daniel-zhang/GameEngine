#include "Game.h"
#include "Profiler.h"
#include "TestHook.h"
#include "RenderWindow.h"
#include <sstream>

Game::Game()
{
    mRenderCore = NULL;
}

Game::~Game()
{

}

bool Game::init()
{
    test_hook(at_game_init);

    initPhaseOneSingletons();

    Timer* initTimer = Singleton<Profiler>::getInstance().createTimer(L"InitTimer", L"Init Timer");
    Singleton<Profiler>::getInstance().startTimer(initTimer);

    if (!mRenderCore)
    {
        mRenderCore = new RenderCore();
        mRenderCore->init();
    }

    Singleton<Profiler>::getInstance().endTimer(initTimer);
    float time = initTimer->totalTime();

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
    Timer* gameTimer = Singleton<Profiler>::getInstance().createTimer(L"LogicStats", L"Logic Stats");
    Timer* renderingTimer = Singleton<Profiler>::getInstance().createTimer(L"RenderStats", L"Render Stats");
    std::wostringstream frameInfo; 
    frameInfo.precision(4);
    static float frameTimeSum = 0.f;
    static float gameTimeSum = 0.f;
    static float renderingTimeSum = 0.f; 
    static float smoothRange = 300.f; // 300ms, 3 updates per sec 
    static uint32 frameCounter = 0;

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

            Singleton<Profiler>::getInstance().startTimer(gameTimer);
            step(mTicker.delta_second());
            Singleton<Profiler>::getInstance().endTimer(gameTimer);

            Singleton<Profiler>::getInstance().startTimer(renderingTimer);
            driveRenderCore();
            Singleton<Profiler>::getInstance().endTimer(renderingTimer);

            // Display frame info text

            frameTimeSum += mTicker.delta_millisecond();
            gameTimeSum += gameTimer->lastDelta_millisecond();
            renderingTimeSum += renderingTimer->lastDelta_millisecond();
            frameCounter++;
            //Sleep(30);
            if (frameTimeSum > smoothRange)
            {
                float avgFrame = frameTimeSum / frameCounter;
                float avgGame = gameTimeSum / frameCounter;
                float avgRendering = renderingTimeSum / frameCounter;
                float avgFPS = 1000.f / avgFrame;

                frameTimeSum = 0.f;
                gameTimeSum = 0.f;
                renderingTimeSum = 0.f;
                frameCounter = 0;

                frameInfo << L"Frame: " << avgFrame << L"ms | Game: " << avgGame << L"ms | Render: " << avgRendering << L"ms | FPS: " << avgFPS;
                mRenderCore->getActiveRenderWindow()->setTitleBarText(frameInfo.str());
                frameInfo.str(L"");
            }
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

