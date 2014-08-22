#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "reference.h"
#include "MaterialAttribute.h"

struct MeshMaterial
{
    MeshMaterial() { ZeroMemory(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular; // w = SpecPower
    XMFLOAT4 Reflect;
};

class RenderInterface;
class Entity;
class Shader;

class MaterialInterface
{
public:
    MaterialInterface(std::string& shaderName);
    virtual ~MaterialInterface();

    virtual void apply(RenderInterface* ri, Entity* entity) = 0;

public:
    std::vector<MaterialAttributeInterface*> mAttributes;
    Shader* mShader;

    // Render States
    // Blend State
    // DepthStencil State
    // Rasterizer State
    // Sampler State
};

class DefaultMaterial : public MaterialInterface
{
public:
    DefaultMaterial(std::string& shaderName);
    virtual void apply(RenderInterface* ri, Entity* entity);

protected:
    void setDefault();

public:
    MaterialAttr<XMFLOAT4X4>* mpLocalToWorld;
    MaterialAttr<ID3D11ShaderResourceView>* mpDiffuseMap;
    MaterialAttr<MeshMaterial>* mpObjMaterial; 
};

#endif