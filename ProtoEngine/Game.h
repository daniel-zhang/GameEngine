#ifndef GAME_H
#define GAME_H

#include <windows.h>

#include "reference.h"
#include "typedefs.h"
#include "singleton.h"

#include "RenderCore.h"
#include "Ticker.h"

class Game
{
public:
	Game();
	~Game();

	bool init();
	bool exit();
	int32 runWin32();

protected:
	void step(float delta);
	void driveRenderCore();

protected:
	Ticker mTicker;

	RenderCore* mRenderCore;
};
#endif