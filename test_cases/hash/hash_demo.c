#include "hash_all_test.h"
#include "iterators.h"

static hash_func_table_t hash_funcs[] = {
    {"bkdr_hash", bkdr_hash},
    {"sdbm_hash", sdbm_hash},
    {"rs_hash",   rs_hash  },
    {"js_hash",   js_hash  },
    {"pjw_hash",  pjw_hash },
    {"elf_hash",  elf_hash },
    {"djb_hash",  djb_hash },
    {"ap_hash",   ap_hash  },
};

static void str_hash_demo(test_case_arg_t *arg)
{
    const char *test_str = "[]{};',./:\"<>?"
                           "widfjonclnfdoewn;lal"
                           "4521367279892980"
                           "a629f6d6-1172-47d8-acbd-0d120511c371";

    ITER_ARRAY (hash_info, hash_funcs) {
        arg->print("%s: %ld\r\n", hash_info->name, hash_info->func(test_str));
    }
}

EXPORT_DEMO(str_hash_demo);
