#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include "ForwardDecls.h"
#include <vector>

class SwapChain;
class RenderWindow;

class RenderCore
{
public:
    RenderCore();
    ~RenderCore();

    bool init();
	bool exit();
	int draw();

public:
    RenderInterface* mRI;

private:
    std::vector<RenderTarget*> mTargets;
    SwapChain* mDefaultTarget;
    RenderWindow* mMainWindow;
};

#endif