#ifndef __TEST_FRAME_H__
#define __TEST_FRAME_H__

#include <stddef.h>
#include "test_case_conf.h"
#include "test_logging.h"

#define TEST_ASSERT(cond, fmt, ...)                 \
    do {                                            \
        if (0 == (cond)) {                          \
            TEST_PRINT(ASSERT, fmt, ##__VA_ARGS__); \
            return -EINVAL;                         \
        }                                           \
    } while (0)

#define TEST_ASSERT_TO(arg, cond, fmt, ...)                 \
    do {                                                    \
        if (0 == (cond)) {                                  \
            TEST_PRINT_TO(arg, ASSERT, fmt, ##__VA_ARGS__); \
            return -EINVAL;                                 \
        }                                                   \
    } while (0)

size_t run_all_testcases(test_case_arg_t* arg);
size_t get_all_testcases_num(void);
void run_all_demo(test_case_arg_t* arg);

#endif // __TEST_FRAME_H__
