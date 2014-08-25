#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include <vector>
#include "reference.h"

class ColorPalette
{
public:
    static XMVECTORF32 LightSteelBlue;
};


class SwapChain;
class RenderWindow;
class RenderInterface;
class SwapChainRT;
class RenderTarget;

class RenderCore
{
public:
    RenderCore();
    ~RenderCore();

    bool init();
	bool exit();
	int draw();

    RenderWindow* getActiveRenderWindow(){return mMainWindow;}

public:
    RenderInterface* mRI;

private:
    SwapChainRT* mDefaultTarget;
    RenderWindow* mMainWindow;
};

#endif