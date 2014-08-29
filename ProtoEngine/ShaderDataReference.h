#ifndef SHADER_DATA_REFERENCE_H
#define SHADER_DATA_REFERENCE_H

#include "AttributeReference.h"

class ShaderDataReference
{
public:
    ShaderDataReference(){}
    ~ShaderDataReference()
    {
        for (uint32 i = 0; i < mRefs.size(); ++i) {
            if (mRefs[i]) { delete mRefs[i]; mRefs[i] = 0; }
        }
        mRefs.clear();
    }
    // Encapsulate TAttrRef<..> to make use of type deduction on typename T
    template <NativeEnum_ShaderVarTag E, typename T >
    void add(T& data) 
    { 
        mRefs.push_back(new TAttrRef<T, E>(data)); 
    }

    std::vector<AttrReferenceInterface*> mRefs;
};

#endif