#include "RenderCore.h"
#include <iostream>
#include <windows.h>

/*
int main(int argc, char** argv)
{
    RenderCore rc;
}
*/
int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    RenderCore rc;
    rc.init();
    rc.run();

    return 0;
}