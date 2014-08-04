#ifndef RENDER_CORE_H
#define RENDER_CORE_H

#include "ForwardDecls.h"

#include "RenderConfig.h"
#include <vector>

class RTWindow;

class RenderCore
{
public:
    RenderCore();
    ~RenderCore();

    bool init();

    int run();

public:
    RenderInterface* mRI;
    RenderConfig mConfig;

private:
    RTWindow* mDefaultTarget;
    std::vector<RenderTarget*> mTargets;
};

#endif