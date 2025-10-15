#include "mem_mana/mem_pools.h"
#include "test_case_conf.h"
#include "test_frame.h"
#include "test_logging.h"
#include "tiny_console/tiny_console.h"
#include "tiny_console/tiny_console_cmd.h"
#include "iterators.h"

int console_write(console_t *this, const char *buf, uint32_t len)
{
    test_case_arg_t *arg = console_get_data(this);

    FOR_I (len) {
        TEST_PRINT_RAW("%c", buf[i]);
    }

    return 0;
}

CONSOLE_CMD_DEF(__printf)
{
    CONSOLE_CMD_UNUSE_ARGS;

    console_println(this, "argc: %d", argc);

    // d[fus]

    console_println(this, "argv[0]: %d", argv[0].num);
    if (argc > 1)
        console_println(this, "argv[1]: %f", argv[1].fnum);
    if (argc > 2)
        console_println(this, "argv[2]: %u", argv[2].unum);
    if (argc > 3)
        console_println(this, "argv[3]: %s", argv[3].str);

    if (argc > 4) {
        for (int i = 0; i < argc - 4; i++) {
            console_println(this, "argv[%d]: %s", i + 4, argv[i + 4].str);
        }
    }

    return 0;
}

const char *test_cases[] = {
    // single command and multiple slices
    "printf 15 32.7232 31 string asdads ][]] 9\r",
    "printf -15 -32.7232 -31 string asdads ][]] -9\r",
    "printf asds\r",
    "printf\r",
    "printf 46 5\r",
    "prin",
    "tf 4",
    "6 5 1\b134\r",

    // multiple commands and multiple slices
    "abc\rdef\r",
    "eh",
    "c\rdef\r",

    // \b test
    "printf 4\b6 5\r",
    "\b\b\b\b\b\b\b",
    "printf\r",
    "printf 5 5 5 abc \"ab cde fg\" 456 \"123\"\"456\"\r",
    "printf      1    2 3  \"sadad asdsad\"   \r",
};

DEFINE_TESTCASE(console_basic)
{
    console_cmd_desc_t desc = {
        .cmd = "printf",
        .desc = "just like c printf",
        .arg_desc = "d[fus]+",
        .fn = __printf,
    };

    // create console in test case memory pool
    console_t *this = console_create_in_pool(128, console_write, "zzh@ZZH-WORK",
                                             MEMPOOL_TESTCASE);

    // bind environment data
    console_bind_data(this, arg);

    console_register_command(this, &desc);
    console_display_prefix(this);
    console_flush(this);

    // this->flags.bits.echo = 0;

    ITER_ARRAY (iter, test_cases) {
        size_t len = strlen(*iter);
        FOR_I (len) {
            console_input_char(this, (*iter)[i]);
        }

        console_update(this);
    }

    console_delete(this);

    return 0;
}
