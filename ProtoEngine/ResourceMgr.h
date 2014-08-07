#ifndef RESOURCE_MGR_H
#define RESOURCE_MGR_H

#include <map>
#include <vector>
#include "reference.h"
#include "Resource.h"

class ResourceMgr
{
public:	
    bool init();
    bool destroy();

    void reg(std::wstring resType, ResourceLoader* loader);
    Resource* get(std::wstring uri);

protected:
    void parseUri(std::wstring& uri );

    struct RegistryItem
    {
        std::wstring mType;
        ResourceLoader* loader;
    };

protected:
    typedef std::map<std::wstring, RegistryItem> TResourceRegistry; 
    TResourceRegistry mResourceRegistry;
};


/*
void testCode()
{
    ResourceMgr resMgr;
    resMgr.init();

    Configuration* config = static_cast<Configuration>(resMgr.get(L"/config/root"));
    Effect* effect = resMgr.get(L"/visual/base_effect");
    Texture* tex = resMgr.get(L"/tex2d/some_tex");
}*/

#endif