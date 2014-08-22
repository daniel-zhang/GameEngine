#include "ShaderEnum.h"

void named_enum_test()
{
    SmartEnum_ShaderVarTag def;
    std::string testStr("view_to_proj");
    bool ret = def.validate(std::string(testStr));
    NativeEnum_ShaderVarTag eTag; 
    if (ret)
    {
        eTag = def.toEnum(testStr);

        if (eTag == e_view_to_proj)
        {
            ret = true;
        }
    }
}

