#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "D3D11.h"
#include "typedefs.h"

class RenderTarget
{
public:
    RenderTarget();
    RenderTarget(uint32 width, uint32 height);
    virtual ~RenderTarget();
    
public:
    UINT mWidth;
    UINT mHeight;

    ID3D11DepthStencilView*		mDepthStencilView;
    ID3D11RenderTargetView*		mRenderTargetView;
    ID3D11Texture2D*			mDepthStencilBuffer;
};

#endif