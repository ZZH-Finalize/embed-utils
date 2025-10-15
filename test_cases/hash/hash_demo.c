#include "hash_all_test.h"
#include "iterators.h"
#include "test_frame.h"

static hash_func_table_t hash_funcs[] = {
    HASH_FUNC_ITEM(bkdr_hash),
    HASH_FUNC_ITEM(sdbm_hash),
    HASH_FUNC_ITEM(rs_hash),
    HASH_FUNC_ITEM(js_hash),
    HASH_FUNC_ITEM(pjw_hash),
    HASH_FUNC_ITEM(elf_hash),
    HASH_FUNC_ITEM(djb_hash),
    HASH_FUNC_ITEM(ap_hash),
};

DEFINE_DEMO(str_hash_demo)
{
    const char *test_str = "[]{};',./:\"<>?"
                           "widfjonclnfdoewn;lal"
                           "4521367279892980"
                           "a629f6d6-1172-47d8-acbd-0d120511c371";

    ITER_ARRAY (hash_info, hash_funcs) {
        TEST_INFO("%s: %ld", hash_info->name, hash_info->func(test_str));
    }
}
