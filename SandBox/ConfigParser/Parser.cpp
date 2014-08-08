#include <Parser.h>

Token::Token()
{
    file_name = "";
    str = "";
    line_number = 0;
    pure_string = false;
}

Token::Token( string& input, string& filename, unsigned int lineNum, bool pureString /*= false*/ )
{
    file_name = filename;
    str = input;
    line_number = lineNum;
    pure_string = pureString;
}

void Token::set( string& input, string& filename, unsigned int lineNum, bool pureString /*= false*/ )
{
    file_name = filename;
    str = input;
    line_number = lineNum;
    pure_string = pureString;
}

bool Token::is( const char* input )
{
    return (str.compare(input) == 0);
}

bool Token::isExpression()
{
    return true;
}

bool Token::isIdentifier()
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

bool Token::isAZaz_( char c )
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') )
        return true;
    else
        return false;
}

bool Token::isAZaz09_( char c )
{
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') )
        return true;
    else
        return false;
}

bool Tokenizer::process( std::string& input, string& filename /*= string("NA")*/ )
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

void Tokenizer::reset()
{
    tokens.clear();
}

Node* Parser::parse( vector<Token>& input )
{
    for (size_t i = 0; i < input.size(); ++i)
    {
        _tokens.push_back(input[i]);
    }

    Node* root = new Node();

    _parse(root);

    return root;
}

void Parser::reset()
{
    pos = 0;
}

void Parser::_parse( Node* root )
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

bool Parser::atEnd()
{
    return pos == _tokens.size();
}

bool Parser::beyondEnd()
{
    return (unsigned int)pos >= _tokens.size();
}

Token& Parser::curr()
{
    if((unsigned int )pos >= _tokens.size())
    {
        error_info.str(""); error_info.clear();
        error_info << "[parser@" << peek(-1).file_name << "#" << peek(-1).line_number <<  "] curr() broke the right boundary";
        throw runtime_error(error_info.str().c_str()); 
    }
    return _tokens[pos];
}

Token& Parser::peek( int offset /*= 1*/ )
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
