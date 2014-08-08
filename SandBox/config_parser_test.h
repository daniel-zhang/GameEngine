#ifndef config_parser_test_h
#define config_parser_test_h

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

class IO
{
public:
    void fileToString(string filename, string& output)
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

    void stringToFile(string filename, string& input)
    {
        ofstream ofs(filename.c_str(), ios::out);
        if (ofs.is_open())
        {
            ofs << input;
            ofs.close();
        }
    }
};

class Token
{
public:
    bool  pure_string;
    string str;
    string file_name;
    unsigned int line_number;

public:
    Token()
    {
        file_name = "";
        str = "";
        line_number = 0;
        pure_string = false;
    }
    Token(string& input, string& filename, unsigned int lineNum, bool pureString = false)
    {
        file_name = filename;
        str = input;
        line_number = lineNum;
        pure_string = pureString;
    }
    void set(string& input, string& filename, unsigned int lineNum, bool pureString = false)
    {
        file_name = filename;
        str = input;
        line_number = lineNum;
        pure_string = pureString;
    }
    bool is(const char* input)
    {
        return (str.compare(input) == 0);
    }

    bool isExpression()
    {
        return true;
    }
    bool isIdentifier()
    {
        if (str.size() == 0)
            return false;
        else if (str.size() == 1)
            return isAZaz_(str[0]);
        else
        {
            if(isAZaz_(str[0]) == false)
                return false;
            for (unsigned int i = 1; i < str.size(); ++i)
                if (isAZaz09_(str[i]) == false)
                    return false;
            return true;
        }
    }

protected:
    bool isAZaz_(char c)
    {
        if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') )
            return true;
        else
            return false;
    }
    bool isAZaz09_(char c)
    {
        if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') )
            return true;
        else
            return false;
    }

};
class Tokenizer
{
public:
    bool process(std::string& input, string& filename = string("NA"))
    {
        reset();

        size_t size = input.size();
        if (input.size() < 0)
        {
            return false;
        }

        string curr_token_str;

        size_t index = 0;
        size_t lineNum = 1;
        bool insideQuotation = false;
        while (index < size)
        {
            char c = input[index];
            //
            // High priority block that process non-nested paired stuff like "..." or /*...*/
            //
            if (insideQuotation || c == '"')
            {
                // enter quotation zone
                if (insideQuotation == false && c == '"')
                {
                    // finish the previous token if any
                    if (!curr_token_str.empty())
                    {
                        tokens.push_back(Token(curr_token_str, filename, lineNum));
                        curr_token_str.clear();
                    }
                    insideQuotation = true;
                    // the beginning '"' is skipped
                }
                // in quotation zone: keep everything
                else if (insideQuotation == true && c != '"')
                {
                    curr_token_str += c;
                }
                // leave quotation zone
                else if (insideQuotation == true && c == '"')
                {
                    // empty() check is not required, as null string is allowed
                    tokens.push_back(Token(curr_token_str, filename, lineNum));
                    curr_token_str.clear();

                    insideQuotation = false;
                    // the ending'"' is skipped
                }
            }
            //
            // Normal tokenize logic
            //
            else
            {
                if (c == ' ' || c == '\n' || c == '\t')
                {
                    // finish the previous token if any
                    // then skip the current token
                    if (!curr_token_str.empty())
                    {
                        tokens.push_back(Token(curr_token_str, filename, lineNum));
                        curr_token_str.clear();
                    }
                    if (c == '\n')
                    {
                        ++lineNum;
                    }
                }
                else if(c == '{' || c == '}' || c == '=')
                {
                    // finish the previous token if any
                    if (!curr_token_str.empty())
                    {
                        tokens.push_back(Token(curr_token_str, filename, lineNum));
                        curr_token_str.clear();
                    }
                    // finish the current token
                    curr_token_str += c;
                    tokens.push_back(Token(curr_token_str, filename, lineNum));
                    curr_token_str.clear();
                }
                else 
                {
                    curr_token_str += c;
                }
            }
            ++index;
        }
        return true;
    }

    void reset()
    {
        tokens.clear();
    }

    vector<Token> tokens;
};

class NodePath
{
public:
    void fromString(string& path)
    {
    }

    void asString(string& output)
    {
    }

    vector<string> internal_path;
};

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

class Parser 
{
public:
    stringstream error_info;

    Parser(){reset();}
    Node* parse(vector<Token>& input)
    {
        for (size_t i = 0; i < input.size(); ++i)
        {
            _tokens.push_back(input[i]);
        }

        Node* root = new Node();

        _parse(root);

        return root;
    }

protected:
    void reset()
    {
        pos = 0;
    }
    void _parse(Node* root)
    {
        if( curr().is("{") )
        {
            pos++;
            while(curr().is("}") == false)
            {
                if (beyondEnd())
                {
                    // throw: invalid syntax
                    error_info.str(""); error_info.clear();
                    error_info << "[parser@" << peek(-1).file_name << "#" << peek(-1).line_number <<  "] unexpected file end";
                    throw runtime_error(error_info.str().c_str()); 
                }
                _parse(root);
            }
            pos++; // skip the '}'
            return;
        }
        else if (curr().isIdentifier())
        {
            Node* node = new Node();
            node->key = curr().str;
            if (peek(1).is("{"))
            {
                ++pos;
                _parse(node);
                root->addChild(node);
                return;
            }
            else if (peek(1).is("="))
            {
                pos += 2;
                if (beyondEnd())
                {
                    // throw '=' is incomplete
                    error_info.str(""); error_info.clear();
                    error_info << "[parser@" << peek(-1).file_name << "#" << peek(-1).line_number <<  "] right side of '=' is invalid";
                    throw runtime_error(error_info.str().c_str()); 
                }
                else if (curr().isExpression() == false)
                {
                    //throw: '=' is not followed by a valid expression
                    error_info.str(""); error_info.clear();
                    error_info << "[parser@" << peek(-1).file_name << "#" << peek(-1).line_number <<  "] right side of '=' is invalid";
                    throw runtime_error(error_info.str().c_str()); 
                }
                else
                {
                    node->val = curr().str;
                    ++pos;
                    root->addChild(node);
                    return;
                }
            }
            else
            {
                //throw: identifier is not followed by '=' or '{'
                error_info.str(""); error_info.clear();
                error_info << "[parser@" << curr().file_name << "#" << curr().line_number <<  "] right side of identifier {" 
                    << curr().str
                    << "} is not valid";
                throw runtime_error(error_info.str().c_str()); 
            }
        }
        else
        {
            // throw: un-recognized token: curr()
            error_info.str(""); error_info.clear();
            error_info << "[parser@" << curr().file_name << "#" << curr().line_number <<  "] identifier {" 
                << curr().str
                << "} is not recognized";
            throw runtime_error(error_info.str().c_str()); 
        }
    }

    bool atEnd()
    {
        return pos == _tokens.size();
    }
    bool beyondEnd()
    {
        return pos >= _tokens.size();
    }

    Token& curr()
    {
        if((unsigned int )pos >= _tokens.size())
        {
            error_info.str(""); error_info.clear();
            error_info << "[parser@" << peek(-1).file_name << "#" << peek(-1).line_number <<  "] curr() broke the right boundary";
            throw runtime_error(error_info.str().c_str()); 
        }
        return _tokens[pos];
    }

    Token& peek(int offset = 1)
    {
        if (pos + offset >= static_cast<int>( _tokens.size()))
        {
            error_info.str(""); error_info.clear();
            error_info << "[parser@" << curr().file_name << "#" << curr().line_number <<  "] peek() broke the right boundary";
            throw runtime_error(error_info.str().c_str()); 
        }
        else if (pos + offset < 0)
        {
            error_info.str(""); error_info.clear();
            error_info << "[parser@" << curr().file_name << "#" << curr().line_number <<  "] peek() broke the left boundary";
            throw runtime_error(error_info.str().c_str()); 
        }
        else
            return  _tokens[pos + offset];
    }
    
protected:
    int pos;
    vector<Token> _tokens;
};

///
///
///
void config_parser_test()
{
    string config_file_name = "../SandBox/sample_config.txt";
    IO io;
    string buffer;
    io.fileToString(config_file_name, buffer);

    Tokenizer tokenizer;
    tokenizer.process(buffer, config_file_name);

    Parser parser;
    Node* node = NULL;
    try
    {
        node = parser.parse(tokenizer.tokens);
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }
    Dumper dumper;
    dumper.bang(node);
}

#endif