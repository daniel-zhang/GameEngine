#ifndef MATERIAL_H
#define MATERIAL_H

#include "reference.h"
#include <string>
#include <vector>

#include "MaterialAttribute.h"

class Material
{
public:
    Material(){}

    Shader* mShader;
    std::vector<MaterialAttributeInterface*> mAttributes;
};

class IRenderable
{
public:
    virtual void drawSelf(class RenderInterface* ri) = 0;
    virtual void setTransform() = 0;
    virtual void setMaterial() = 0;

protected:
    Material* mMaterial;
};

#endif