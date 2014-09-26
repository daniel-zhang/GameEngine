#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "reference.h"
#include "GraphicBuffer.h"

class RenderWindow;
class SwapChainRT;
class Viewport;
// A thin layer upon d3d11
class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

    bool init();

    // State query
    bool isInitialized();

    // Event handlers
    void resizeViewport(float width, float height);

    // Operations
    void presentBackBuffer();

    /*
    void setRenderTarget(RenderTarget* rt);
    RenderTarget* getRenderTarget();
    void setSwapChain(SwapChainRT* inSwapChain);
    */
    void drawIndexedPrimtive(uint32 indexCount, uint32 indexOffset);

    bool createViewport(RenderWindow* rw);
    float getViewportAspect();
    void setViewport(Viewport* vp);
    void setViewportByIndex(uint32 index);

    void clearBackground(XMFLOAT4& color);

    /*
    void setIndexBuffer(IndexBuffer* pIB){}
    void setVertexBuffer(VertexBuffer<VertexFactory<e_pos_normal_tan_tex>>* pVB){}
    */

    void resetRenderState(){mCtx->RSSetState(0);}
    void enableWireFrame(){mCtx->RSSetState(mWireframe);}
    void enableBackfaceRendering(){mCtx->RSSetState(mNoBackfaceCulling);}
    void enableWireFrameAndNoBackfaceRendering(){mCtx->RSSetState(mNoBackfaceCulling);}

public:
    ID3D11Device*				mDevice;
    ID3D11DeviceContext*		mCtx;

    bool mInitialized;
    //std::vector<Viewport*> mViewports;
    Viewport* mViewport;

protected:
    // Render states
    ID3D11RasterizerState* mWireframe;
    ID3D11RasterizerState* mNoBackfaceCulling;
    ID3D11RasterizerState* mWireframe_NoBackfaceCulling;
};

#endif