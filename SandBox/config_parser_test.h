#ifndef config_parser_test_h
#define config_parser_test_h

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

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

class ProtoParser
{
public:
    void parse(std::string& input)
    {

    }

    void tokenize()
    {

    }

};

void config_parser_test()
{
    IO io;
    string buffer;
    io.fileToString("../SandBox/config.txt", buffer);
    io.stringToFile("../SandBox/config2.txt", buffer);
    cout << buffer ;

    ProtoParser pparser;
    pparser.parse(buffer);
}

#endif