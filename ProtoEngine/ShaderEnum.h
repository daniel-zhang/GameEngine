#ifndef SHADER_ENUM_H
#define SHADER_ENUM_H

#include <string>
#include <vector>

//
//
//
template<typename EType>
struct NamedEnumItem
{
    NamedEnumItem(){}
    NamedEnumItem(EType inEnumVal, std::string& inString): mEnumVal(inEnumVal), mEnumString(inString) {}
    EType mEnumVal;
    std::string mEnumString;
};

template<typename EType>
class NamedEnumeration
{
public:
    typedef NamedEnumItem<EType> EnumElement;

    EnumElement* query(const char* str)
    {
        std::string target(str);
        for (std::size_t i = 0; i < mElements.size(); ++i)
        {
            if (mElements[i].mEnumString.compare(target) == 0)
                return &mElements[i];
        }
        return NULL;
    }

protected:
    std::vector<EnumElement> mElements;
};

//
//
//
enum EnumEffectType
{
    fx_basic = 0,
    fx_shadowmap
};
class EffectTypes : public NamedEnumeration<enum EnumEffectType>
{
public:
    EffectTypes();
};

enum EnumBasicTechniqueTypes
{
    tech_default = 0,
    tech_default_unlit
};
class BasicTechniqueTypes : public NamedEnumeration<enum EnumBasicTechniqueTypes>
{
public:
    BasicTechniqueTypes();
    const EnumEffectType mEnumEffect;
};


//
//
//
enum EnumShaderVarTag
{
    cam_pos = 0,
    local_to_world, 
    world_to_view,
    view_to_proj,
    world_view_proj,
    tex_transform,
    world_inv_transpose,
    plain_material,
    texture,
    anisotropic_sampler
};


void named_enum_test();
#endif