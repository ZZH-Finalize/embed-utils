#include <stdio.h>
#include <string.h>
#include "iterators.h"
#include "tiny_console/tiny_console.h"
#include "tiny_console/tiny_console_cmd.h"

void memInit(void);

int console_write(console_t *this, const char *buf, uint32_t len)
{
    (void) this;
    FOR_I (len) {
        putchar(buf[i]);
    }

    return 0;
}

CONSOLE_CMD_DEF(__printf)
{
    CONSOLE_CMD_UNUSE_ARGS;

    console_println(this, "argc: %d", argc);

    return 0;
}

const char *test_cases[] = {
    // single command and multiple slices
    "printf 46 5\r", "prin", "tf 4", "6 5 1\b134\r",

    // multiple commands and multiple slices
    "abc\rdef\r", "eh", "c\rdef\r",

    // \b test
    "printf 4\b6 5\r",
    "\b\b\b\b\b\b\b",
    "printf\r",
};

int main(const int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    memInit();

    console_cmd_desc_t desc = {
        .cmd = "printf",
        .desc = "just like c printf",
        .arg_desc = "d[fis]+",
        .fn = __printf,
    };

    console_t *this = console_create(32, console_write, "zzh@ZZH-WORK");
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

    return 0;
}
