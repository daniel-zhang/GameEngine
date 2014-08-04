#include "RenderTarget.h"
#include "D3DUtilities.h"

RenderTarget::RenderTarget()
{
    mWidth              = 0;
    mHeight             = 0;
    mDepthStencilView   = NULL;
    mRenderTargetView   = NULL;
    mDepthStencilBuffer = NULL;
}

RenderTarget::~RenderTarget()
{
    safe_release(&mDepthStencilView);
    safe_release(&mRenderTargetView);
    safe_release(&mDepthStencilBuffer);
}


