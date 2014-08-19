#ifndef VERTEX_FORMAT_MGR_H
#define VERTEX_FORMAT_MGR_H

#include <vector>
#include "reference.h"

class RenderInterface;
class Shader;

struct VERTEX_POS_NORMAL_TEX
{
    VERTEX_POS_NORMAL_TEX() { ZeroMemory(this, sizeof(VERTEX_POS_NORMAL_TEX)); }
    XMFLOAT3 pos;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};

enum EnumVertexFormat
{
    VF_POS_NORMAL_TEX = 0
};

class VertexFormat
{
public:
    VertexFormat(class RenderInterface* ri, Shader* inShader, EnumVertexFormat eVFormat, D3D11_PRIMITIVE_TOPOLOGY eTopology, const D3D11_INPUT_ELEMENT_DESC* pDesc, uint32 numDesc );
    ~VertexFormat();

    void attach(RenderInterface* ri);

public:
    EnumVertexFormat mEnumVF;
    ID3D11InputLayout* mInputLayout;
    D3D11_PRIMITIVE_TOPOLOGY mEnumTopology;
};

/*
VertexFormat vf = getFromSomeWhere<VERTEX_BLAH_BLAH>;
VertexFormat vf = getFromSomeWhere(ENUM_VF_XX_XX);
RI->set(&vf)
*/

#include "Singleton.h"
class VertexFormatMgr : public Singleton<VertexFormatMgr>
{
public:
    VertexFormatMgr();
    ~VertexFormatMgr();

    bool init(RenderInterface* ri);
    bool destroy();

    VertexFormat* get(EnumVertexFormat evf);

protected:
    std::vector<VertexFormat> mVFormats;
};
#endif