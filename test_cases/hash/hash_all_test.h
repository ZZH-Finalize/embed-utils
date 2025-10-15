#ifndef __HASH_ALL_TEST_H__
#define __HASH_ALL_TEST_H__

#include "hash/str_hash.h"

typedef struct
{
    const char* name;
    str_hash_t func;
} hash_func_table_t;

#define HASH_FUNC_ITEM(fn) {.name = #fn, .func = fn}

#endif // __HASH_ALL_TEST_H__
