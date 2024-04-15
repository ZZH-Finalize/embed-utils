#include <stdlib.h>
#include <sys/errno.h>
#include "test_frame.h"
#include "arg_checkers.h"
#include "iterators.h"
#include "mem_mana/mem_mana.h"

LINKER_SYMBOL_TYPE(__stest_cases, __test_case_info_t);
LINKER_SYMBOL_TYPE(__etest_cases, __test_case_info_t);

LINKER_SYMBOL_TYPE(__sdemo, __demo_fn_t);
LINKER_SYMBOL_TYPE(__edemo, __demo_fn_t);

uint32_t get_all_testcases_num(void)
{
    return ((uint32_t) __etest_cases - (uint32_t) __stest_cases)
           / sizeof(*__stest_cases);
}

uint32_t run_all_testcases(test_case_arg_t* arg)
{
    CHECK_PTR(arg, -EINVAL);

#if CONFIG_ENABLE_TEST_CASES == 1
    uint32_t succ_count = 0;
    uint32_t test_case_num = get_all_testcases_num();

    FOR_I (test_case_num) {
        __test_case_info_t* test_case_info = &__stest_cases[i];
        arg->print("Running test case: %s\r\n", test_case_info->name);
        int retv = test_case_info->fn(arg);
        arg->print("Return value: %d\r\n", retv);
#if CONFIG_CHECK_TESTCASE_MEMPOOL == 1
        uint8_t isClean = memIsClean(CONFIG_TEST_CASE_MEMPOOL);
        const char* fmt = isClean ? "clean" : "dirty";

        arg->print("Memory pool is %s!\r\n", fmt);
#endif
        arg->print("\r\n");
        succ_count += retv == 0;
    }

    return succ_count;
#else
    return 0;
#endif
}

void run_all_demo(void)
{
    uint32_t demo_num = ((uint32_t) __edemo - (uint32_t) __sdemo) / 4;

    FOR_I (demo_num) {
        __sdemo[i]();
    }
}
