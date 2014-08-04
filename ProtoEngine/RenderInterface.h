#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "reference.h"
#include "RenderTarget.h"
#include "RenderConfig.h"

class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

    bool init(RenderConfig& config);

    // State query
    bool isInitialized();

    // Operations
    void attachRenderTarget(RenderTarget* rt);

    ID3D11Device*				mDevice;
    ID3D11DeviceContext*		mCtx;

    bool mInitialized;
};

#endif