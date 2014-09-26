#include "TextureMgr.h"
#include "RenderInterface.h"
#include "GameResourcePath.h"

/*
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
*/

#include <algorithm>

TextureMgr::TextureMgr()
{
    mRIRef = NULL;
    mTexPath = gTexturePath;
}

TextureMgr::~TextureMgr()
{
}

bool TextureMgr::init(RenderInterface* ri)
{
    mRIRef = ri;
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    return true;
}

bool TextureMgr::destroy()
{
    std::map<std::wstring, Texture*>::iterator iter;
    for (iter = mTexMap.begin(); iter != mTexMap.end(); ++iter)
    {
        if (iter->second)
        {
            delete (iter->second);
            iter->second = NULL;
        }
    }
    mTexMap.clear();
    return true;
}

Texture* TextureMgr::getTexture(std::wstring& texName)
{
    std::map<std::wstring, Texture*>::const_iterator citer = mTexMap.find(texName);
    if (citer != mTexMap.end())
    {
        return (citer->second);
    }
    else
    {
        Texture* pTex = new Texture;
        if (loadFromDisk(texName, pTex))
        {
            mTexMap.insert(std::pair<std::wstring, Texture*>(texName, pTex));
            return pTex;
        }
        else
        {
            return NULL;
        }
    }
}

Texture* TextureMgr::getTexture(const wchar_t* texName)
{
    return getTexture(std::wstring(texName));
}

bool TextureMgr::loadFromDisk(std::wstring& filename, Texture* pTex)
{
    if (pTex == NULL)
        return false;

    pTex->mFilename = filename;

    std::wstring ext = L"";
    getExtAsLowerCase(filename, ext);

    /*
    HRESULT hr = -1;
    if (ext.compare(L".dds") == 0)
    {
        // DDSTextureLoader
        hr = DirectX::CreateDDSTextureFromFile(mRIRef->mDevice, filename.c_str(), &pTex->mResource, &pTex->mSRV);
        if (FAILED(hr)) return false;
    }
    else if(ext.compare(L".tga") == 0)
    {
        // runtime converting from tga to dds not supported
    }
    else
    {
        // WICTextureLoader
        hr = DirectX::CreateWICTextureFromFile(mRIRef->mDevice, mRIRef->mCtx, filename.c_str(), &pTex->mResource, &pTex->mSRV);
    }

    if (FAILED(hr)) return false;
    */
    return true;
}

void TextureMgr::getExtAsLowerCase(std::wstring& filename, std::wstring& ext)
{
    std::size_t extPos = filename.rfind(L".");
    if (extPos == std::wstring::npos) return;
    ext = filename.substr(extPos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
}

void texture_mgr_test()
{
    Texture* pTree = Singleton<TextureMgr>::getInstance().getTexture(L"tree0.dds");
    Texture* pFace = Singleton<TextureMgr>::getInstance().getTexture(L"HeadHires.bmp");

    if (pTree)
    {

    }
    if (pFace)
    {

    }
}
