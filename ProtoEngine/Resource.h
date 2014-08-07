#ifndef RESOURCE_H
#define RESOURCE_H

class Resource
{
public:
    Resource()
    {

    }
    ~Resource();

    enum EResourceState
    {
        E_Success = 0,
        E_Failed,
        E_Loading,
        E_Uninitialized
    };

    bool init();

protected:
    static std::wstring mType;
    EResourceState mState;
};

class ResourceLoader
{
public:
    virtual void load(Resource* pResource);
};


class Config : public Resource
{
public:
    uint32 mConfigSectionNum;
};

class ConfigLoader : public ResourceLoader
{
public:
    void load(Resource* pResource)
    {
        Config* pConfig = new Config();
        //pConfig->mConfigSectionNum = some_how_get();

        pResource = pConfig;
    }
};


#endif