#ifndef VERTEX_FORMAT_H
#define VERTEX_FORMAT_H

#include <string>
#include <vector>
#include "reference.h"

class RenderInterface;
class Shader;

enum EnumVertexFormat
{
    VF_POS_NORMAL_TEX = 0,
    VF_SKINNED,
    VF_TOTAL_NUM
};

template<EnumVertexFormat N>
class VertexFormat
{
public:
    VertexFormat(RenderInterface* ri);
    void bind(RenderInterface* ri);

protected:
    ID3D11InputLayout* mInputLayout;
    static const D3D11_INPUT_ELEMENT_DESC formatDesc[];
};

template<>
class VertexFormat<VF_POS_NORMAL_TEX>
{
public:
    VertexFormat()
    {
    }
    struct POS_NORMAL_TEX
    {
        POS_NORMAL_TEX() { ZeroMemory(this, sizeof(POS_NORMAL_TEX)); }
        XMFLOAT3 pos;
        XMFLOAT3 normal;
        XMFLOAT2 texcoord;
    };
    typedef POS_NORMAL_TEX VType;
};

class VertexFormatFactory
{
public:
    void init(RenderInterface* ri)
    {
        for (uint32 i = 0; i < VF_TOTAL_NUM; ++i )
        {

        }

    }
    //VertexFormat<EnumVertexFormat>* get(EnumVertexFormat evf);

protected:
    //std::vector<VertexFormat<EnumVertexFormat>*> mVFormats;
};

    /*
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
*/
#endif