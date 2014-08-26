#include "template_test.h"
#include <windows.h>

void template_test()
{
    TypeContainer tc;
    GType<e_type_0>* p0 = tc.get<e_type_0>();
    GType<e_type_1>* p1 = tc.get<e_type_1>();
    GType<e_type_2>* p2 = tc.get<e_type_2>();
    GType<e_type_3>* p3 = tc.get<e_type_3>();
    GType<e_type_4>* p4 = tc.get<e_type_4>();

    p0->sayType();
    p1->sayType();
    p2->sayType();
    p3->sayType();
    p4->sayType();
}
