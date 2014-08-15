#ifndef EFFECT_WRAPPER_H
#define EFFECT_WRAPPER_H

#include "d3dx11effect.h"
#include "typedefs.h"

#include <string>
class Effect
{
public:
    Effect();

    uint32 mIndex;
    std::string mEffectName;

    ID3DX11Effect* mFX;
};

class Technique
{
public:
    std::string mTechName;
    uint32 mTechIndex;
};


#endif