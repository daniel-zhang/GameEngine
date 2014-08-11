#include "PropertyTree.h"

PropertyNode::PropertyNode()
{
    parent = NULL;
    key = "__default_key__";
    val = "__default_val__";
    isLeaf = false;
}

PropertyNode::~PropertyNode()
{
    for (unsigned int i = 0; i < children.size(); ++i)
    {
        delete children[i];
        children[i] = NULL;
    }
}

void PropertyNode::addChild( PropertyNode* other )
{
    other->parent = this;
    children.push_back(other);
}

PropertyPath::PropertyPath()
{
    reset();
}

void PropertyPath::fromString( string& path )
{
    if (path.empty())
        return ;

    reset();
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

void PropertyPath::reset()
{
    mPath.clear();
}
