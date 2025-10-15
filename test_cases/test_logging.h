#ifndef __TEST_LOGGING_H__
#define __TEST_LOGGING_H__

#include <errno.h>
#include "test_case_conf.h"

#define TEST_PRINT_RAW(fmt, ...) arg->print(fmt, ##__VA_ARGS__)

#define TEST_PRINT(level, fmt, ...) \
    TEST_PRINT_RAW("[TEST][" #level "]: " fmt "\r\n", ##__VA_ARGS__)

#define TEST_INFO(fmt, ...) TEST_PRINT(INFO, fmt, ##__VA_ARGS__)
#define TEST_FAIL(fmt, ...)                   \
    do {                                      \
        TEST_PRINT(FAIL, fmt, ##__VA_ARGS__); \
        return -EINVAL;                       \
    } while (0)

#define TEST_PRINT_TO_RAW(arg, fmt, ...) arg->print(fmt, ##__VA_ARGS__)

#define TEST_PRINT_TO(arg, level, fmt, ...) \
    TEST_PRINT_TO_RAW(arg, "[TEST][" #level "]: " fmt "\r\n", ##__VA_ARGS__)

#define TEST_INFO_TO(arg, fmt, ...) TEST_PRINT_TO(arg, INFO, fmt, ##__VA_ARGS__)
#define TEST_FAIL_TO(arg, fmt, ...)                   \
    do {                                              \
        TEST_PRINT_TO(arg, FAIL, fmt, ##__VA_ARGS__); \
        return -EINVAL;                               \
    } while (0)

#endif // __TEST_LOGGING_H__
