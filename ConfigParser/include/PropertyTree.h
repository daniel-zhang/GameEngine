#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::stringstream;

class PropertyNode 
{
public:
    PropertyNode();
    ~PropertyNode();
    void addChild(PropertyNode* other);
    PropertyNode* getChild(string& key);

    PropertyNode* parent;
    vector<PropertyNode*> children;

    string key;
    string val;
    bool isLeaf;
};

class PropertyPath
{
public:
    PropertyPath();
    PropertyPath(string& path);
    void fromString(string& path);
    void reset();
    void dump();

    vector<string> mPath;
};

/*
usage 1
    PropertyNode* node = PropertyTree::query("/aaa/bbb/ccc")
    int width=0;
    if(propertyTree:convert(node, width)
        ...
    else
        ...

usage 2
    int width=0;
    if(propertyTree:convert("/aaa/bbb/ccc", width)
        ...
    else
        ...
*/
class PropertyTree
{
public:
    PropertyTree();

    PropertyNode* query(string& pathString);

    template<typename VAL_TYPE>
    bool convert(PropertyNode& node, VAL_TYPE& output);

    template<>
    bool convert(PropertyNode& node, bool& output);

    template<typename VAL_TYPE>
    bool convert(string path, VAL_TYPE& output);

public:
    PropertyNode* mRoot;

    stringstream mConverter;
    vector<string> mTrue;
    vector<string> mFalse;

protected:
    // Case-Insensitive compare
    bool iEqual(string& left, string& right);
};

// General conversion for: int, float, string
template<typename VAL_TYPE>
bool PropertyTree::convert( PropertyNode& node, VAL_TYPE& output )
{
    // Non-leaf node does not have concrete value
    if (node.children.size() != 0)
        return false;

    mConverter.clear();
    mConverter.str("");
    mConverter << node.val;

    if(mConverter >> output)
        return true;
    else
        return false;
}

// Specialization for boolean
template<>
bool PropertyTree::convert(PropertyNode& node, bool& output)
{
    // Non-leaf node does not have concrete value
    if (node.children.size() != 0)
        return false;

    // Is node TRUE?
    for (unsigned int i = 0; i < mTrue.size(); ++i)
    {
        if (iEqual(node.val, mTrue[i]))
        {
            output = true;
            return true;
        }
    }
    // Is node FALSE?
    for (unsigned int i = 0; i < mFalse.size(); ++i)
    {
        if (iEqual(node.val, mFalse[i]))
        {
            output = false;
            return true;
        }
    }
    // Failed to identify node 
    return false;
}

template<typename VAL_TYPE>
bool PropertyTree::convert( string path, VAL_TYPE& output )
{
    PropertyNode* node = query(path);
    if (!node)
        return false;

    return convert<VAL_TYPE>(*node, output);
}


class Dumper
{
public:
    Dumper();
    void bang(PropertyNode* node);

protected:
    int indent_step;
    void _bang(PropertyNode* pNode, int indent);

};

#endif