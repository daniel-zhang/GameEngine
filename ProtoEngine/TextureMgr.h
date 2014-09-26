#ifndef TEXTURE_MGR_H
#define TEXTURE_MGR_H

#include "Singleton.h"
#include "reference.h"
#include <string>
#include <map>

class Texture
{
public:
    Texture()
    {
        mFilename = L"";
        mSRV = NULL;
        mResource = NULL;
    }
    ~Texture()
    {
        safe_release(&mSRV);
        safe_release(&mResource);
    }

public:
    std::wstring mFilename;
    ID3D11ShaderResourceView* mSRV;
    ID3D11Resource* mResource;
};

class RenderInterface;
class TextureMgr : public Singleton<TextureMgr>
{
// Make singleton -- start
protected:
    TextureMgr();
    ~TextureMgr();
public:
    friend class Singleton<TextureMgr>;
    bool init(RenderInterface* ri);
    bool destroy();
// Make singleton -- end 

public:
    Texture* getTexture(std::wstring& texName);
    Texture* getTexture(const wchar_t* texName);

protected:
    bool loadFromDisk(std::wstring& filename, Texture* pTex);
    void getExtAsLowerCase(std::wstring& filename, std::wstring& ext);

protected:
    std::wstring mTexPath;
    RenderInterface* mRIRef;
    std::map<std::wstring, Texture*> mTexMap;
};


void texture_mgr_test();
#endif