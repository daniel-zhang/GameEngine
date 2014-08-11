#ifndef parser_test_h
#define parser_test_h

#include <fstream>
#include <string>

#include "Parser.h"

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

void parser_test()
{
    cout << "--------------------------------------" << endl;
    cout << "void parser_test()" << endl;
    cout << "--------------------------------------" << endl;

    string config_file_name = "../SandBox/sample_config.txt";
    IO io;
    string buffer;
    io.fileToString(config_file_name, buffer);

    Tokenizer tokenizer;
    tokenizer.process(buffer, config_file_name);

    Parser parser;
    PropertyNode* node = NULL;
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