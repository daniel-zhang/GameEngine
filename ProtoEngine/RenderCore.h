#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include <string>
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
class Viewport;
class TestHandler;

class RenderCore
{
public:
    RenderCore();
    ~RenderCore();

    bool isInitialized();
    bool init(RenderWindow* rw);
    bool exit();
    int draw();

    void pause();
    void restore();

protected:
    RenderInterface* mRI;
    RenderWindow* mMainWindow;
    bool mInitialized;
};

#endif