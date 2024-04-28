#include <stdio.h>
#include "tiny_console/tiny_console.h"
#include "iterators.h"
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

    console_t *this = console_create(128, console_write, "zzh@ZZH-WORK");
    console_register_command(this, &desc);
    console_display_prefix(this);
    console_flush(this);

    while (1) {
        console_input_char(this, (char) getchar());
        console_update(this);
    }

    return 0;
}
