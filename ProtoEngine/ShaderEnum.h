#ifndef SHADER_ENUM_H
#define SHADER_ENUM_H

#include <string>
#include <vector>
#include <map>

//
//
//
/*
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
*/


template <typename NATIVE_ENUM_TYPE>
class SmartEnumInterface
{
public:
    typedef std::map<std::string, NATIVE_ENUM_TYPE> TEnumMap;
    typedef std::pair<std::string, NATIVE_ENUM_TYPE> TPair;

    NATIVE_ENUM_TYPE toEnum(std::string& inStr)
    {
        TEnumMap::const_iterator citer;
        citer = eMap.find(inStr);
        if (citer != eMap.end())
            return citer->second;
        else
            return (NATIVE_ENUM_TYPE)-1;
    }
    std::string& toString(NATIVE_ENUM_TYPE e)
    {
        return eNames[e];
    }

    bool validate(std::string& inStr)
    {
        TEnumMap::const_iterator citer;
        citer = eMap.find(inStr);
        return (citer != eMap.end());
    }

protected:
    std::vector<std::string> eNames;
    std::map<std::string, NATIVE_ENUM_TYPE> eMap;
};

// Helper macro begin
#define  _define_native_enum_(x) e_##x,
#define _create_enum_reflection_(x)\
    eNames.push_back(#x);\
    eMap.insert(TPair(#x, e_##x));

#define DEFINE_NATIVE_ENUM(EnumTemplate) enum NativeEnum_##EnumTemplate{ EnumTemplate(_define_native_enum_) };
#define CREATE_ENUM_CLASS(EnumTemplate) \
class SmartEnum_##EnumTemplate: public SmartEnumInterface<NativeEnum_##EnumTemplate>\
{\
public:\
    SmartEnum_##EnumTemplate()\
    {\
        EnumTemplate(_create_enum_reflection_)\
    }\
};\

#define MAKE_SMART_ENUM(x)\
    DEFINE_NATIVE_ENUM(x)\
    CREATE_ENUM_CLASS(x)
// Helper macro end

//
// Enum Declaration
//
#define ShaderVarTag(decl)\
    decl(cam_pos)\
    decl(local_to_world)\
    decl(world_to_view)\
    decl(view_to_proj)\
    decl(world_view_proj)\
    decl(tex_transform)\
    decl(world_inv_transpose)\
    decl(mesh_material)\
    decl(texture)\
    decl(anisotropic_sample)\
    decl(main_light)
MAKE_SMART_ENUM(ShaderVarTag)
#undef ShaderVarTag

void named_enum_test();
#endif