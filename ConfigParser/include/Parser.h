/*
Parser Pseudo:
global: tokens, pos
void parse(Node* root)
{
    curr == '{':
        pos++
        while(curr != '}' )
        {
            if end of file 
            throw error
            parse(root)
        }
        return 
    curr == 'a-z':
        node = new Node
        node.key = curr_token
        if next == '{'
            pos++
            parse(node)
            root.add(node)
            return 
        else if next == '='
            pos += 2
            if curr != 'a-z'
                throw error 
            else
                node.val = curr
                pos++
                root.add(node)
                return 
        else
            throw error 
    curr == 'anything else'
        throw error 
}

*/
#ifndef PARSER_H
#define PARSER_H

#include "PropertyTree.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

using std::stringstream;
using std::runtime_error;

class Token
{
public:
    bool  pure_string;
    string str;
    string file_name;
    unsigned int line_number;

public:
    Token();
    Token(string& input, string& filename, unsigned int lineNum, bool pureString = false);
    void set(string& input, string& filename, unsigned int lineNum, bool pureString = false);
    bool is(const char* input);

    bool isExpression();
    bool isIdentifier();

protected:
    bool isAZaz_(char c);
    bool isAZaz09_(char c);

};

class Tokenizer
{
public:
    bool process(std::string& input, string& filename = string("NA"));

    void reset();

    vector<Token> tokens;
};

class Parser 
{
public:
    stringstream error_info;

    Parser(){reset();}

    PropertyNode* parse(vector<Token>& input);

protected:
    void reset();
    void _parse(PropertyNode* root);

    bool atEnd();
    bool beyondEnd();

    Token& curr();

    Token& peek(int offset = 1);

protected:
    int pos;
    vector<Token> _tokens;
};

#endif