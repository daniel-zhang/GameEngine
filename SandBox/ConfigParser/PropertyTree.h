#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Node 
{
public:
    Node()
    {
        parent = NULL;
        key = "__default_key__";
        val = "__default_val__";
        isLeaf = false;
    }
    ~Node()
    {
        for (unsigned int i = 0; i < children.size(); ++i)
        {
            delete children[i];
            children[i] = NULL;
        }
    }

    Node* parent;
    vector<Node*> children;

    string key;
    string val;
    bool isLeaf;

    void addChild(Node* other)
    {
        other->parent = this;
        children.push_back(other);
    }
};

/*
A general structure
*/
class PropertyPath
{
public:
    PropertyPath()
    {
        reset();
    }
    void fromString(string& path)
    {
        if (path.empty())
            return ;
        // split the string into substrings
        string curr_item = "";
        unsigned int i = 0;
        unsigned int size = path.size();
        while(i < size)
        {
            char c = path[i];

            // Skip the beginning '/' 
            // Handles this pattern: 'xxx/'
            if (c == '/' && !curr_item.empty())
            {
                mPath.push_back(curr_item);
                curr_item.clear();
            }
            // We are at the last char 
            // Either 'xxx/xxx'
            // Or 'xxx/xxx/'
            else if (i == size -1)
            {
                // the path is ended with '/'
                if (c == '/')
                {
                    if (!curr_item.empty())
                    {
                        mPath.push_back(curr_item);
                        curr_item.clear();
                    }
                }
                // the path does not have a trailing '/'
                else
                {
                    curr_item += c;
                    mPath.push_back(curr_item);
                }
            }
            else if (c != '/')
            {
                curr_item += c;
            }
            else
            {
                // something like 'aaa/bbb//ccc' is encountered
                // just skip the extra '//'
            }
            i++;
        }
    }
    void reset()
    {
        mPath.clear();
    }

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

    Node* mRoot;
};

class Dumper
{
public:
    Dumper():indent_step(2){}
    void bang(Node* node)
    {
        _bang(node, 0);
    }

protected:
    int indent_step;
    void _bang(Node* pNode, int indent)
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