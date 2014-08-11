#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

class PropertyNode 
{
public:
    PropertyNode();
    ~PropertyNode();
    void addChild(PropertyNode* other);

    PropertyNode* parent;
    vector<PropertyNode*> children;

    string key;
    string val;
    bool isLeaf;

public:
    // Explicitly convert values, no template!
    bool asInt(const int& output);
    bool asFloat(float& output);
    bool asString(string& output);
};

class PropertyPath
{
public:
    PropertyPath();
    void fromString(string& path);
    void reset();

    void dump()
    {
        for (size_t i = 0; i < mPath.size(); ++i)
        {
            cout << mPath[i] << ".";
        }
        cout << "\n";
    }

protected:
    vector<string> mPath;
};

class PropertyTree
{
public:
    PropertyTree() 
    {
        mRoot = NULL;
    }

    void query(string path)
    {

    }

    PropertyNode* mRoot;
};

class Dumper
{
public:
    Dumper():indent_step(2){}
    void bang(PropertyNode* node)
    {
        _bang(node, 0);
    }

protected:
    int indent_step;
    void _bang(PropertyNode* pNode, int indent)
    {
        string local_indent_string;
        for (int i = 0; i < indent; ++i)
        {
            local_indent_string += " ";
        }
        cout << local_indent_string << pNode->key << ": " << pNode->val <<endl;
        cout << local_indent_string << "{" << endl;
        for (unsigned int i = 0;i < pNode->children.size(); ++i)
        {
            _bang(pNode->children[i], indent + indent_step);
        }
        cout << local_indent_string << "}" << endl;
    }

};

#endif