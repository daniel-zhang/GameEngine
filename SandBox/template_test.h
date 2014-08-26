#ifndef TEMPLATE_TEST
#define TEMPLATE_TEST
#include <vector>
#include <iostream>

using namespace std;

void template_test();

enum ERandomTypes
{
    e_type_0 = 0,
    e_type_1, 
    e_type_2, 
    e_type_3, 
    e_type_4 
};

class TypeRoot
{
public:
    ~TypeRoot(){}
};

template<ERandomTypes n>
class GType : public TypeRoot
{ 
public:
    GType()
    {
        mType = n;
    }
    ~GType()
    {
        for (unsigned int i = 0; i < mObjs.size(); ++i)
        {
            if (mObjs[i] != NULL)
            {
                delete mObjs[i];
                mObjs[i] = NULL;
            }
        }
        mObjs.clear();
    }

    void sayType()
    {
        cout <<"My Type Is: " << mType << endl;
    }

    GType<n>* create()
    {
        GType<n>* p = new GType<n>;
        mObjs.push_back(p);
        return p;
    }
    vector<GType<n>*> mObjs;
    ERandomTypes mType;
};

class TypeContainer
{
public:
    TypeContainer()
    {
        GType<e_type_0>* p0 = new GType<e_type_0>();
        mTypes.push_back(p0);

        GType<e_type_1>* p1 = new GType<e_type_1>();
        mTypes.push_back(p1);

        GType<e_type_2>* p2 = new GType<e_type_2>();
        mTypes.push_back(p2);
        
        GType<e_type_3>* p3 = new GType<e_type_3>();
        mTypes.push_back(p3);

        GType<e_type_4>* p4 = new GType<e_type_4>();
        mTypes.push_back(p4);
    }
    ~TypeContainer()
    {
        for (unsigned int i = 0; i < mTypes.size(); ++i)
        {
            if (mTypes[i])
            {
                delete mTypes[i];
                mTypes[i] = NULL;
            }
        }
        mTypes.clear();
    }
    vector<TypeRoot*> mTypes;

    template<ERandomTypes n>
    GType<n>* get()
    {
        return static_cast<GType<n>*>(mTypes[n]);
    }

    TypeRoot* createByTypeEnum(ERandomTypes e)
    {
        return mTypes[e]->create();
    }
};

#endif