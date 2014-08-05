#ifndef WIN_LAUNCHER_H
#define WIN_LAUNCHER_H

#include "Game.h"
#include "Launcher.h"

class WinLauncher : public Launcher
{
public:
	int run()
	{
		mGame.init();
		int retval = mGame.runWin32();
		mGame.exit();
		return retval;
	}

protected:
	Game mGame;
};


#endif