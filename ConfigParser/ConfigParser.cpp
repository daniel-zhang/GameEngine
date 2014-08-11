#include "ConfigParser.h"
#include <fstream>
using std::ifstream;
using std::ios;

ConfigParser::ConfigParser()
{

}

ConfigParser::~ConfigParser()
{

}

bool ConfigParser::process( string filename, PropertyTree& proTree, string& errmsg )
{
    string buffer;
    fileToString(filename, buffer);

    mTokenizer.process(buffer, filename);
    proTree.mRoot = NULL;

    try
    {
        proTree.mRoot = mParser.parse(mTokenizer.tokens);
    }
    catch (std::exception& e)
    {
        errmsg = e.what();
        return false;
    }
    return true;
}

void ConfigParser::fileToString( string& filename, string& output )
{
    ifstream fin(filename.c_str(), ios::in);
    if (fin.is_open())
    {
        stringstream ss;
        ss << fin.rdbuf();
        output = ss.str();
        fin.close();
    }
}
