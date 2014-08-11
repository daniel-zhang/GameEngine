#include "PropertyTree.h"
#include <ctype.h>

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

PropertyNode* PropertyNode::getChild( string& key )
{
    for (unsigned int i = 0; i < children.size(); ++i)
    {
        if (key.compare(children[i]->key) == 0)
        {
            return children[i];
        }
    }
    return NULL;
}

PropertyPath::PropertyPath()
{
    reset();
}

PropertyPath::PropertyPath( string& path )
{
    reset();
    fromString(path);
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

void PropertyPath::dump()
{
    for (size_t i = 0; i < mPath.size(); ++i)
    {
        cout << mPath[i] << ".";
    }
    cout << "\n";
}

PropertyTree::PropertyTree()
{
    mRoot = NULL;

    mTrue.push_back("true");
    mTrue.push_back("yes");
    mFalse.push_back("false");
    mFalse.push_back("no");
}


PropertyNode* PropertyTree::query( string& pathString )
{
    PropertyNode* node = mRoot;
    if (!node)
    {
        return NULL;
    }

    PropertyPath path(pathString);
    for(unsigned int i = 0; i < path.mPath.size(); ++i)
    {
        string& key = path.mPath[i];
        node = node->getChild(key);
        if (!node)
        {
            return NULL;
        }
    }
    return node;
}

bool PropertyTree::iEqual( string& left, string& right )
{
    if (left.size() != right.size())
        return false;

    for (unsigned int i = 0; i < left.size(); ++i)
    {
        if (tolower(left[i]) != tolower(right[i]))
            return false;
    }
    return true;
}

Dumper::Dumper() :indent_step(2)
{

}

void Dumper::bang( PropertyNode* node )
{
    _bang(node, 0);
}

void Dumper::_bang( PropertyNode* pNode, int indent )
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
