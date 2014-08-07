#ifndef VIRTUAL_FILE_SYSTEM_H
#define VIRTUAL_FILE_SYSTEM_H

#include "reference.h"
#include <string>

/*

vfs::physical_path mount_point = L"";
vfs.mount();
vfs.map();

*/


class PathNode
{
};

class Path
{

};

class PhysicalFile
{
    std::wstring mFilename;
    std::wstring mExtension;
};

class VirtualFile
{

};

class VirtualPathNode 
{
public:
    std::wstring mNodeName;
    std::wstring mPhysicalPath;

    VirtualPathNode* mParent;
    std::vector<VirtualPathNode*> mChildren;
};

class VirtualFileSystem 
{
public:
    VirtualPathNode* mRoot;

    void addNode(VirtualPathNode* parent, );

protected:
    std::vector<VirtualPathNode*> mNodes;
};



#endif