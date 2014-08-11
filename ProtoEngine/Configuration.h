#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "typedefs.h"
#include <string>
#include "PropertyTree.h"
#include "ConfigParser.h"
#include "Singleton.h"

class ConfigNode
{
public:
    ConfigNode();

    virtual void sync(PropertyTree* tree, std::string configPath) = 0;

protected:
    std::string section_name;
};

class RenderConfig : public ConfigNode
{
public:
    RenderConfig();

    void sync(PropertyTree* propertyTree, std::string configPath);

public:
    uint32 screen_width;
    uint32 screen_height;
    bool enable_msaa;
    uint32 msaa_quality;
    bool force_singlethread;
};

class RootConfig : public ConfigNode
{
public:
    RootConfig();
    void sync(PropertyTree* propertyTree, std::string configPath = "");

public:
    RenderConfig render_config;
};

class Configuration : public Singleton<Configuration>
{
public:
    RootConfig root;
    bool init();

    bool destroy();
};

#endif
