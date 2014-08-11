#ifndef PARSER_TEST_2_H
#define PARSER_TEST_2_H

#include "ConfigParser.h"

void parser_test2()
{
    cout << "--------------------------------------" << endl;
    cout << "void parser_test2()" << endl;
    cout << "--------------------------------------" << endl;

    ConfigParser cpaser;
    PropertyTree proTree;
    string errorMsg;
    if ( cpaser.process("../SandBox/sample_config.txt", proTree, errorMsg) == false)
    {
        cout << errorMsg << endl;
        return;
    }

    // Sample usage
    string stringVal = "";
    float floatVal = 0.f;
    int intVal = 0;
    bool boolVal1 = false;
    bool boolVal2 = false;

    proTree.convert("/AI/path", stringVal);
    proTree.convert("/AI/effect_model/minimal", floatVal);
    proTree.convert("/AI/effect_model/maximal", intVal);
    proTree.convert("/AI/AI/effect_model/strong_ai", boolVal1);
    proTree.convert("/AI/AI/effect_model2/strong_ai", boolVal2);

    cout << stringVal << endl << floatVal << endl << intVal << endl << (boolVal1? "true":"false") << endl << (boolVal2 ? "true" : "false") << endl;
}


#endif