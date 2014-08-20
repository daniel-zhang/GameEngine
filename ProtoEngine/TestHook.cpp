#include "TestHook.h"

#include "OSInterface.h"
#include "ShaderEnum.h"
#include "EffectMgr.h"

#if defined DEBUG || defined _DEBUG
    #define  ENABLE_RUNTIME_TEST
#endif

void test_default()
{
}

void test_game_init()
{
    osi_test();
    named_enum_test();
}

void test_render_core_init()
{

}

void test_phase_2_singletons()
{
    shader_mgr_test();
}

//
// test router
//
void test_hook(EnumTestHookPoint eHookPoint)
{
#if defined ENABLE_RUNTIME_TEST

    switch (eHookPoint)
    {
    case at_game_init:
        test_game_init();
    	break;

    case at_render_core_init:
        test_render_core_init();
        break;

    case after_phase_2_singletons:
        test_phase_2_singletons();
        break;

    default:
        test_default();
    }

#endif
}
