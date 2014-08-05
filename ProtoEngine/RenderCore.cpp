#include "RenderCore.h"
#include "RenderInterface.h"
#include "RTWindow.h"
#include <windows.h>
#include "Singleton.h"

RenderCore::RenderCore()
{
    mRI = new RenderInterface;
    mDefaultTarget = new RTWindow;
}

RenderCore::~RenderCore()
{
    delete mRI;
    delete mDefaultTarget;
}

bool RenderCore::init()
{
    if( mRI->init(mConfig) == false)
        return false;

    if ( mDefaultTarget->init(mRI, mConfig) == false)
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
	return true;
}

