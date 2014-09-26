#ifndef MESH_H
#define MESH_H

#include <vector>
#include "reference.h"
#include "GraphicBuffer.h"
#include "ShaderDataReference.h"

class Entity;
class RenderInterface;
class MaterialInterface;
class DefaultMaterial;
class Mesh;

class SubMesh 
{
public:
    SubMesh();
    ~SubMesh();
        
    void init(std::string& shaderName, Mesh* inMesh, uint32 indexCount, uint32 indexOffset);
    void drawSelf(RenderInterface* ri);
    uint32 getFaceNum(){return mIndexCount/3;}

public:
    DefaultMaterial* mDefaultMat;

    Mesh* mMeshRef;

    uint32 mIndexCount;
    uint32 mIndexOffset;
};

class Mesh 
{
public:
    Mesh();
    ~Mesh();
    uint32 numTriangles();
    uint32 numVertIndices();
    uint32 numVertices();

    void addSubMesh(std::string& defaultShaderName, uint32 indexCount, uint32 indexOffset);
    bool createGpuBuffer(RenderInterface* ri);
    void drawSelf(Entity* entity);

    void enableWireframe(bool enabled);
    void enableBackfaceCulling(bool enabled);

public:
    IndexBuffer mIndexBuffer;
    VertexBuffer<e_pos_normal_tan_tex> mVertexBuffer;

    std::vector<SubMesh*> mSubMeshes;

protected:
    bool mEnableWireframe;
    bool mEnableBackfaceCulling;

    RenderInterface* mRI;
};


#endif