#ifndef __TEST_TYPEDEF_H__
#define __TEST_TYPEDEF_H__

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

#endif // __TEST_TYPEDEF_H__
