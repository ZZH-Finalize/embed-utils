#ifndef __TEST_CASE_CONF_H__
#define __TEST_CASE_CONF_H__

#include "linker_tools.h"

typedef struct
{
    int (*print)(const char *fmt, ...);
    void *args;
} test_case_arg_t;

typedef int (*__test_case_fn_t)(test_case_arg_t *arg);
typedef void (*__demo_fn_t)(test_case_arg_t *arg);

typedef struct
{
    const char *name;
    __test_case_fn_t fn;
} __test_case_info_t;

#ifdef CONFIG_ENABLE_TEST_CASES
#define EXPORT_TEST_CASE_LEVEL(fn, level) \
    EXPORT_FUNC_WITH_NAME_LEVEL(fn, __test_case_fn_t, test_cases, level)
#else
#define EXPORT_TEST_CASE_LEVEL(fn, level) \
    GNU_UNUSED __test_case_fn_t __ptr_to_##fn##_##level = fn
#endif

#ifdef CONFIG_ENABLE_DEMO
#define EXPORT_DEMO_LEVEL(fn, level) \
    EXPORT_FUNC_WITH_LEVEL(fn, __demo_fn_t, demo, level)
#else
#define EXPORT_DEMO_LEVEL(fn, level) \
    GNU_UNUSED __demo_fn_t __ptr_to_##fn##_##level = fn
#endif

#define EXPORT_TEST_CASE(fn) EXPORT_TEST_CASE_LEVEL(fn, 9)
#define EXPORT_DEMO(fn)      EXPORT_DEMO_LEVEL(fn, 9)

#define __DEFINE_EXPORT(ret_t, fn, exp)         \
    static ret_t fn(test_case_arg_t *arg); \
    exp(fn);                             \
    static ret_t fn(test_case_arg_t *arg)

#define DEFINE_TESTCASE(fn) __DEFINE_EXPORT(int, fn, EXPORT_TEST_CASE)
#define DEFINE_DEMO(fn)     __DEFINE_EXPORT(void, fn, EXPORT_DEMO)

#endif // __TEST_CASE_CONF_H__
