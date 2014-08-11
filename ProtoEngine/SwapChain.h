#ifndef RT_WINDOW_H
#define RT_WINDOW_H

#include <d3d11.h>
#include "RenderTarget.h"

class RenderInterface;

class RenderWindow;

class SwapChain: public RenderTarget
{
public:
    SwapChain();
    ~SwapChain();
    bool init(RenderInterface* ri, RenderWindow* rw);

protected:
    bool mInitialized;
    IDXGISwapChain* mSwapChain;
};

#endif