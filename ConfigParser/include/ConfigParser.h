#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "PropertyTree.h"
#include "Parser.h"

// A Wrapper class
class ConfigParser
{
public:
    ConfigParser();
    ~ConfigParser();

    bool process(string filename, PropertyTree& proTree, string& errmsg);

public:
    Tokenizer mTokenizer;
    Parser mParser;

protected:
    void fileToString(string& filename, string& output);
};
#endif