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

void Mesh::drawSelf( RenderInterface* ri, Entity* entity )
{
    // TODO: input layout... and primitive topology ?
    mVertexBuffer.bind(ri);
    mIndexBuffer.bind(ri);

    ri->mCtx->IASetInputLayout(mSubMeshes[0]->mDefaultMat->getInputLayout());
    ri->mCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ShaderDataReference& sceneShaderData = entity->getSceneRef()->getSceneShaderData();
    ShaderDataReference& entityShaderData = entity->getEntityShaderData();

    SubMesh* subMesh = NULL; 
    for (uint32 i = 0; i < mSubMeshes.size(); ++i)
    {
        subMesh = mSubMeshes[i];
        subMesh->mDefaultMat->apply(ri, sceneShaderData, entityShaderData);
        ri->drawIndexedPrimtive(subMesh->mIndexCount, subMesh->mIndexOffset);
    }
}

bool Mesh::onAttachedToEntity( RenderInterface* ri )
{
    bool ret0, ret1;
    ret0 = mIndexBuffer.createStaticGpuDataFromCache(ri);
    ret1 = mVertexBuffer.createStaticGpuDataFromCache(ri);
    return (ret0 && ret1);
}

Mesh::~Mesh()
{
    for (uint32 i = 0; i < mSubMeshes.size(); ++i)
    {
        release_raw_ptr(&mSubMeshes[i]);
    }
}

uint32 Mesh::getFaceNum()
{
    uint32 faceNum = 0;
    for (uint32 i  = 0; i < mSubMeshes.size(); ++i)
    {
        faceNum += mSubMeshes[i]->getFaceNum();
    }
    return faceNum;
}

void Mesh::addSubMesh( std::string& defaultShaderName, uint32 indexCount, uint32 indexOffset )
{
    SubMesh* subMesh = new SubMesh;
    subMesh->init(defaultShaderName, this, indexCount, indexOffset);
    mSubMeshes.push_back(subMesh);
}
