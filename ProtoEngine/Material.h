#ifndef MATERIAL_H
#define MATERIAL_H

#include "reference.h"
#include <string>
class Material
{
public:
    void setParams();

};

class IRenderable
{
public:
    virtual void drawSelf(class RenderInterface* ri) = 0;
    virtual void setTransform() = 0;
    virtual void setMaterial() = 0;

protected:

};

#endif