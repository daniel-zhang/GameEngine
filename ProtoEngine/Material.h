#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "reference.h"
#include "ShaderDataReference.h"

class RenderInterface;
class Entity;
class ShaderEffect;

struct MeshMaterial
{
    MeshMaterial() { ZeroMemory(this, sizeof(this)); }

    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
    XMFLOAT4 specular; // w = SpecPower
    XMFLOAT4 reflect;
};

// Render States
// Blend State
// DepthStencil State
// Rasterizer State
// Sampler State

/* In a Nutshell
{
    material.buildReference();
    entity.buildReference();
    scene.buildReference();
    // Draw SubMesh
    effect = Material.getCurrEffect;
    ri.set(effect.getInputLayout)
    effect.setShaderData(Material.getShaderData); // texture, mesh_material
    effect.setShaderData(entity.getShaderData); // pos
    effect.setShaderData(scene.getShaderData); // cam, light, fog,
    effect.apply()
    ri.draw()
}
*/
class MaterialInterface
{
public:
    virtual void apply() = 0;
};

class DefaultMaterial 
{
public:
    DefaultMaterial(std::string& shaderName);
    ~DefaultMaterial();

    //void apply(RenderInterface* ri, Entity* entity);
    void apply(RenderInterface* ri, ShaderDataReference& inSceneShaderData, ShaderDataReference& inMeshShaderData)
    {
        mShader->setShaderData(inSceneShaderData);
        mShader->setShaderData(inMeshShaderData);
        mShader->setShaderData(mMaterialShaderData);
        mShader->apply(ri);
    } 

    ShaderDataReference& getMaterialShaderData()
    {
        return mMaterialShaderData;
    }

    ID3D11InputLayout* getInputLayout(){return mShader->mInputLayout;}
protected:
    void determinShaderToLink();
    void buildDefaultAttributes();
    void buildMaterialShaderDataReference();

protected:
    std::string mDefaultShaderName;
    ShaderEffect* mShader;
    ShaderDataReference mMaterialShaderData;

    ShaderDataReference mSceneShaderData;

    // Shader Data - Begin
    ID3D11ShaderResourceView* mDiffuseMap;
    MeshMaterial mMeshMaterial;
    // Shader Data - End
};

#endif