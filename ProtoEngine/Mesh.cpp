#include "Mesh.h"
#include "Scene.h"
#include "Entity.h"
#include "Material.h"
#include "RenderInterface.h"

void SubMesh::drawSelf( RenderInterface* ri )
{
}

SubMesh::SubMesh()
{
    mDefaultMat = NULL;
    mMeshRef = NULL;
    mIndexCount = 0;
    mIndexOffset = 0;
}

SubMesh::~SubMesh()
{
    release_raw_ptr(&mDefaultMat);
}

void SubMesh::init( std::string& shaderName, Mesh* inMesh, uint32 indexCount, uint32 indexOffset )
{
    mDefaultMat = new DefaultMaterial(shaderName);
    //mDefaultMat = new DefaultMaterial();
    mMeshRef = inMesh;
    mIndexCount = indexCount;
    mIndexOffset = indexOffset;
}

void Mesh::drawSelf( Entity* entity )
{
    // TODO: input layout... and primitive topology ?
    mVertexBuffer.bindGpuBuffer(mRI);
    mIndexBuffer.bindGpuBuffer(mRI);

    mRI->mCtx->IASetInputLayout(mSubMeshes[0]->mDefaultMat->getInputLayout());
    mRI->mCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ShaderDataReference& sceneShaderData = entity->getSceneRef()->getSceneShaderData();
    ShaderDataReference& entityShaderData = entity->getEntityShaderData();

    if (mEnableWireframe) mRI->enableWireFrame();
    if (mEnableBackfaceCulling) mRI->enableBackfaceRendering();

    SubMesh* subMesh = NULL; 
    for (uint32 i = 0; i < mSubMeshes.size(); ++i)
    {
        subMesh = mSubMeshes[i];
        subMesh->mDefaultMat->apply(mRI, sceneShaderData, entityShaderData);
        mRI->drawIndexedPrimtive(subMesh->mIndexCount, subMesh->mIndexOffset);
    }

    mRI->resetRenderState();
}

bool Mesh::createGpuBuffer( RenderInterface* ri )
{
    mRI = ri;

    bool ret0, ret1;
    ret0 = mIndexBuffer.createStaticGpuBuffer(mRI);
    //ret1 = mVertexBuffer.createStaticGpuBuffer(ri);
    ret1 = mVertexBuffer.createDynamicGpuBuffer(mRI);
    mVertexBuffer.updateGpuBuffer(mRI);

    return (ret0 && ret1);
}

Mesh::Mesh()
{
    mEnableWireframe = false;
    mEnableBackfaceCulling = false;
}

Mesh::~Mesh()
{
    for (uint32 i = 0; i < mSubMeshes.size(); ++i)
    {
        release_raw_ptr(&mSubMeshes[i]);
    }
}

uint32 Mesh::numTriangles()
{
    return mIndexBuffer.numTriangles();
}

void Mesh::addSubMesh( std::string& defaultShaderName, uint32 indexCount, uint32 indexOffset )
{
    SubMesh* subMesh = new SubMesh;
    subMesh->init(defaultShaderName, this, indexCount, indexOffset);
    mSubMeshes.push_back(subMesh);
}

uint32 Mesh::numVertIndices()
{
    return mIndexBuffer.numVertIndices();
}

uint32 Mesh::numVertices()
{
    return mVertexBuffer.numVerts();
}

void Mesh::enableWireframe( bool enabled )
{
    mEnableWireframe = enabled;
}

void Mesh::enableBackfaceCulling( bool enabled )
{
    mEnableBackfaceCulling = enabled;
}

