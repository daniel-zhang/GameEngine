#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "reference.h"
#include "RenderTarget.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

// A thin layer upon d3d11
class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

    bool init();

    // State query
    bool isInitialized();

    // Operations

    void attachRenderTarget(RenderTarget* rt);
    void setRenderTarget();
    void setViewport();

    void setShader();

    void setIndexBuffer(IndexBuffer* pIB){}
    void setVertexBuffer(VertexBuffer* pVB){}
    

public:
    ID3D11Device*				mDevice;
    ID3D11DeviceContext*		mCtx;

    bool mInitialized;
};

#endif