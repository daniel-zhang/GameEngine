#ifndef TEST_HOOK_H
#define TEST_HOOK_H

enum EnumTestHookPoint
{
    at_default = 0,
    at_game_init,
    at_render_core_init,
    after_phase_2_singletons,
    total_hooks
};
void test_hook(EnumTestHookPoint eHookPoint = at_default);

#endif