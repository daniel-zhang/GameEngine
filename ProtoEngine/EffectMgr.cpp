#include "EffectMgr.h"
#include "Effect.h"
#include "GameResourcePath.h"

EffectMgr::EffectMgr()
{
    mDir = std::string(gShaderPath);
    mDir = gShaderPath;
    mExtension = "";
}

EffectMgr::~EffectMgr()
{

}

bool EffectMgr::init()
{


    return true;
}

bool EffectMgr::clear()
{

    return true;
}

Effect* EffectMgr::getByName( const char* effectName )
{

    return NULL;
}

Effect* EffectMgr::getByIndex()
{
    return NULL;

}
