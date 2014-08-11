#include "Configuration.h"

ConfigNode::ConfigNode()
{
    section_name = "__invalid_section_name__";
}

RenderConfig::RenderConfig()
{
    section_name = "render_config";

    // Default values
    screen_width       = 400;
    screen_height      = 300;
    enable_msaa        = true;
    msaa_quality       = 2;
    force_singlethread = true;
}

void RenderConfig::sync( PropertyTree* propertyTree, std::string configPath )
{
    std::string path = configPath + "/" + this->section_name + "/";
    propertyTree->convert(path + "screen_width", screen_width);
    propertyTree->convert(path + "screen_height", screen_height);
    propertyTree->convert(path + "enable_msaa", enable_msaa);
    propertyTree->convert(path + "msaa_quality", msaa_quality);
    propertyTree->convert(path + "force_singlethread", force_singlethread);
}

RootConfig::RootConfig()
{
    section_name = "root";
}

void RootConfig::sync( PropertyTree* propertyTree, std::string configPath /*= ""*/ )
{
    render_config.sync(propertyTree, std::string("/") + this->section_name);
}

bool Configuration::init()
{
    PropertyTree properties;
    ConfigParser cParser;
    std::string errorMsg;
    if ( cParser.process("../ProtoEngine/config.txt", properties, errorMsg) == false)
    {
        // some error handling here: output<<errorMsg
        return false;
    }

    root.sync(&properties);
    return true;
}

bool Configuration::destroy()
{
    return true;
}
