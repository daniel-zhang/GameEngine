#include "MaterialAttribute.h"

struct MeshMaterial
{
    MeshMaterial() { ZeroMemory(this, sizeof(this)); }

    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular; // w = SpecPower
    XMFLOAT4 Reflect;
};

void test()
{
    MaterialAttr<MeshMaterial> attr1(plain_material);

    //MaterialAttr<XMFLOAT4X4> attr2(local_to_world);
}