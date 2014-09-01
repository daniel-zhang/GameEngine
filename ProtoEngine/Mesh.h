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

    friend class Mesh;
    Mesh* mMeshRef;

    uint32 mIndexCount;
    uint32 mIndexOffset;
};

class Mesh 
{
public:
    ~Mesh();
    void drawSelf(RenderInterface* ri, Entity* entity);
    bool onAttachedToEntity(RenderInterface* ri);
    uint32 getFaceNum();
    void addSubMesh(std::string& defaultShaderName, uint32 indexCount, uint32 indexOffset);
public:
    IndexBuffer mIndexBuffer;
    VertexBuffer<e_pos_normal_tan_tex> mVertexBuffer;
    std::vector<SubMesh*> mSubMeshes;
};

#endif