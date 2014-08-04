#ifndef RT_WINDOW_H
#define RT_WINDOW_H

#include <d3d11.h>
#include "RenderTarget.h"

class RenderWindow;
struct RenderConfig;
class RenderInterface;

class RTWindow: public RenderTarget
{
public:
    RTWindow();
    ~RTWindow();
    bool init(RenderInterface* ri, RenderConfig& config );

protected:
    bool mInitialized;
    IDXGISwapChain* mSwapChain;
    RenderWindow* mRenderWindow;
};

#endif