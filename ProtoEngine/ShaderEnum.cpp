#include "ShaderEnum.h"

#define DECL_ENUM(e) mElements.push_back(EnumElement(e, std::string(#e)))

EffectTypes::EffectTypes()
{
    DECL_ENUM(fx_basic);
    DECL_ENUM(fx_shadowmap);
}

BasicTechniqueTypes::BasicTechniqueTypes() :mEnumEffect(fx_basic)
{
    DECL_ENUM(tech_default);
    DECL_ENUM(tech_default_unlit);
}

void named_enum_test()
{
    EffectTypes nefx;
    EffectTypes::EnumElement* element =  nefx.query("fx_fancy");
    if (element) { }
}
