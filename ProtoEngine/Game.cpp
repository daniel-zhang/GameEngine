#include "Game.h"
#include "Profiler.h"

Game::Game()
{
	mRenderCore = NULL;
}

Game::~Game()
{

}

bool Game::init()
{
	initAllSingletons();

	if (!mRenderCore)
	{
		mRenderCore = new RenderCore();
		mRenderCore->init();
	}

	return true;
}

bool Game::exit()
{

	if (mRenderCore)
	{
		mRenderCore->exit();
		delete mRenderCore;
	}

	destroyAllSingletons();
	return true;
}

int32 Game::runWin32()
{
	Timer* logicTimer = Singleton<Profiler>::instance->createTimer(L"LogicStats", L"Logic Stats");
	Timer* renderTimer = Singleton<Profiler>::instance->createTimer(L"RenderStats", L"Render Stats");

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

			Singleton<Profiler>::instance->startTimer(logicTimer);
			step(mTicker.deltaTime());
			Singleton<Profiler>::instance->endTimer(logicTimer);

			Singleton<Profiler>::instance->startTimer(renderTimer);
			driveRenderCore();
			Singleton<Profiler>::instance->endTimer(renderTimer);
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

