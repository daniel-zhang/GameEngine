#ifndef D3D_UTILS_H
#define D3D_UTILS_H

#include <dxerr.h>

// A helper function to release a COM component
template <typename T> 
void safe_release(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        (*ppT) = NULL;
    }
}

// A debug macro for any Direct3D function that returns an HRESULT value.
#if defined(DEBUG) || defined(_DEBUG)
    #ifndef d3d_check 
    #define d3d_check(x)												\
    {															\
        HRESULT hr = (x);										\
        if(FAILED(hr))											\
            DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);		\
    }
    #endif
#else
    #ifndef d3d_check 
    #define d3d_check(x) (x)
    #endif
#endif


#endif