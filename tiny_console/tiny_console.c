#include <stdio.h>
#include "tiny_console.h"
#include "arg_checkers.h"
#include "tiny_console_cmd.h"
#include "mem_mana/mem_mana.h"
#include "tiny_console_conf.h"
#include "value_ops.h"
#include "iterators.h"
#include "linker_tools.h"
#include "strarg.h"
#include "builtin_cmds/builtin_cmds.h"

void console_register_all_cmds(console_t* this)
{
    LINKER_SYMBOL_TYPE(__sconsole_cmd, console_cmd_desc_t);
    LINKER_SYMBOL_TYPE(__econsole_cmd, console_cmd_desc_t);

    console_cmd_desc_t* pcur_desc = __sconsole_cmd;

    while (pcur_desc < __econsole_cmd) {
        console_register_command(this, pcur_desc);
        pcur_desc++;
    }
}

int console_init(console_t* this, uint32_t buffer_size, console_out_t output_fn,
                 const char* prefix)
{
    CHECK_PTR(this, -EINVAL);
    CHECK_PTR(output_fn, -EINVAL);
    RETURN_IF_ZERO(buffer_size, -EINVAL);

    this->buffer_size = buffer_size;
    this->prefix = prefix;
    this->cwd = "~"; // for now, we don't support filesystem
    this->write = output_fn;
    this->rx_idx = 0;
    this->tx_idx = 0;
    this->current_state = console_state_normal;

    // set flags default value
    this->flags.bits.echo = 1;
    this->flags.bits.color = 1;

    console_register_all_cmds(this);

    return 0;
}

console_t* console_create_in_pool(uint32_t buffer_size, console_out_t output_fn,
                                  const char* prefix, uint32_t pool)
{
    RETURN_IF(pool == UINT32_MAX, NULL);

    console_t* this = memAlloc(sizeof(console_t), pool);

    CHECK_PTR(this, NULL);

    this->mem_pool = pool;

    this->command_table =
        map_create_in_pool(CONSOLE_MAX_HASH, CONSOLE_HASH_METHOD, pool);
    CHECK_PTR_GOTO(this->command_table, map_err);

    this->rxbuf = memAlloc(buffer_size, pool);
    CHECK_PTR_GOTO(this->rxbuf, rxbuf_err);

    this->txbuf = memAlloc(buffer_size, pool);
    CHECK_PTR_GOTO(this->txbuf, txbuf_err);

    this->cmdbuf = memAlloc(buffer_size, pool);
    CHECK_PTR_GOTO(this->cmdbuf, cmdbuf_err);

    int retv = console_init(this, buffer_size, output_fn, prefix);

    GOTO_IF(retv < 0, console_err);

    return this;

console_err:
cmdbuf_err:
    memFree(this->txbuf);
txbuf_err:
    memFree(this->rxbuf);
rxbuf_err:
    map_delete(this->command_table);
map_err:
    console_delete(this);
    return NULL;
}

int console_send_char(console_t* this, const char ch)
{
    CHECK_PTR(this, -EINVAL);
    // buffer full
    int retv = 0;

    if (this->tx_idx >= this->buffer_size) {
        retv = console_flush(this);
    }

    this->txbuf[this->tx_idx++] = ch;

    return retv;
}

int console_send_str(console_t* this, const char* str)
{
    CHECK_PTR(this, -EINVAL);
    CHECK_PTR(str, -EINVAL);

    uint32_t len = strlen(str);

    while (0 != len) {
        // buffer full
        if (this->tx_idx >= this->buffer_size) {
            // flush buffer && send all the data inside of it
            int retv = console_flush(this);
            RETURN_IF(retv < 0, retv);
        } else { // buffer is not full, then copy the data into buffer
            uint32_t buffer_free_space = this->buffer_size - this->tx_idx;
            uint32_t copy_len = MIN(buffer_free_space, len);

            memcpy(&this->txbuf[this->tx_idx], str, copy_len);
            this->tx_idx += copy_len;
            len -= copy_len;
            str += copy_len;
        }
    }

    return 0;
}

void console_display_prefix(console_t* this)
{
    console_set_color(this, concole_color_green, concole_color_black);
    console_send_str(this, this->prefix);
    console_cancel_color(this);

    console_send_char(this, ':');

    console_set_color(this, concole_color_blue, concole_color_black);
    console_send_str(this, this->cwd);
    console_cancel_color(this);

    console_send_char(this, '$');
    console_send_char(this, ' ');
}

int console_vprintf(console_t* this, const char* fmt, va_list vargs)
{
    CHECK_PTR(this, -EINVAL);
    CHECK_PTR(fmt, -EINVAL);

    console_flush(this);

    uint32_t txbuf_free_size = this->buffer_size - this->tx_idx;
    int len =
        vsnprintf(&this->txbuf[this->tx_idx], txbuf_free_size, fmt, vargs);

    this->tx_idx += len;

    if (len == (int) txbuf_free_size)
        console_flush(this);

    return len;
}

int console_printf(console_t* this, const char* fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    int len = console_vprintf(this, fmt, vargs);

    va_end(vargs);

    return len;
}

static uint32_t parse_arg_num(const char* str)
{
    CHECK_PTR(str, 0);

    uint32_t arg_num = 0;

    while (*str) {
        if ('\n' == *(str + 1) || '\0' == *(str + 1))
            return arg_num;
        else if (' ' == *str && ' ' != *(str + 1))
            arg_num++;

        str++;
    }

    return arg_num;
}

static int parse_arg_desc(const char* arg_desc, uint32_t* min_arg_num,
                          uint32_t* max_arg_num)
{
    CHECK_PTR(min_arg_num, -EINVAL);
    CHECK_PTR(max_arg_num, -EINVAL);

    *min_arg_num = 0;
    *max_arg_num = 0;

    RETURN_IF(NULL == arg_desc, 0);

    uint32_t arg_num = 0;
    uint32_t optional_arg_num = 0;
    uint32_t* counter = &arg_num;

    bool end_flag = 0;

    while (*arg_desc) {
        switch (*arg_desc) {
            case 'd': // unsigned number
            case 'i': // signed number
            case 'f': // floating point number
            case 's': // string
                if (1 == end_flag)
                    goto err;

                (*counter)++;
                break;

            case '[': // switch to optional_arg
                if (1 == end_flag)
                    goto err;
                counter = &optional_arg_num;
                break;

            case ']': // end of optional_arg
                      // goto done;
                end_flag = 1;
                break;

            case '+': // inf arg
                optional_arg_num = UINT32_MAX - arg_num;
                goto done;

            default: return -EINVAL;
        }

        arg_desc++;
    }

done:
    *min_arg_num = arg_num;
    *max_arg_num = arg_num + optional_arg_num;
    return 0;
err:
    *min_arg_num = 0;
    *max_arg_num = 0;
    return -EINVAL;
}

static int console_execute(console_t* this)
{
    console_cmd_arg_t* arg_arr = NULL;
    uint32_t arg_num = 0;

    char* first_arg = strchr(this->cmdbuf, ' ');

    arg_num = parse_arg_num(first_arg);

    if (NULL != first_arg) {
        *first_arg = '\0';
    }

    console_cmd_desc_t* cmd_desc = NULL;
    int search_res =
        map_search(this->command_table, this->cmdbuf, (map_value_t*) &cmd_desc);
    RETURN_IF(search_res < 0, search_res);

    uint32_t min_arg_num = 0, max_arg_num = 0;
    int parse_res =
        parse_arg_desc(cmd_desc->arg_desc, &min_arg_num, &max_arg_num);
    RETURN_IF_NZERO(parse_res, parse_res);

    if (arg_num > max_arg_num || arg_num < min_arg_num) {
        console_println(this, "arg num error: %ld give, needs [%lu - %ld]",
                        arg_num, min_arg_num, max_arg_num);
        return -EINVAL;
    }

    first_arg++;

    if (0 != arg_num) {
        arg_arr = memAlloc(sizeof(char*) * arg_num, this->mem_pool);
        CHECK_PTR(arg_arr, -ENOMEM);

        // replace all spaces to \0
        for (char* ch = first_arg; '\0' != *ch; ch++) {
            if (' ' == *ch)
                *ch = '\0';
        }

        char* cur_arg = first_arg;
        const char* arg_types = cmd_desc->arg_desc;

        FOR_I (arg_num) {
            // skip optional symbol
            if ('[' == *arg_types || ']' == *arg_types)
                arg_types++;

            // convert arg by type
            switch (*arg_types) {
                case 'f':
                case 'i': // todo: should use new method
                case 'd': {
                    int conv_res =
                        getNum(cur_arg, (uint32_t*) &arg_arr[i].unum);

                    if (1 != conv_res) {
                        console_send_strln(this, "arg format error");
                        memFree(arg_arr);
                        return -EINVAL;
                    }

                    break;
                }

                case '+':
                case 's': {
                    arg_arr[i].str = cur_arg;
                    break;
                }

                default: break;
            }

            if ('+' != *arg_types)
                arg_types++;

            // skip next chars
            while ('\0' != *cur_arg) cur_arg++;

            // skip \0
            cur_arg++;
        }
    }

    console_send_strln(this, "");

    int cmd_res = cmd_desc->fn(this, arg_num, arg_arr);

    if (NULL != arg_arr)
        memFree(arg_arr);

    return cmd_res;
}

static inline void console_update_normal(console_t* this, char ch)
{
    switch (ch) {
        case '\b':
        case '\177':
            if (0 == this->cur_idx)
                break;

            console_send_char(this, '\b');
            this->cur_idx -= 1;

            if ('\177' == ch) {
                console_send_str(this, "\033[J");
                this->cmdbuf[this->cur_idx] = '\0';
            }
            break;

        case '\r':
            this->cmdbuf[this->cur_idx] = '\0';

            if (this->cur_idx > 1) {
                console_send_strln(this, "");
                this->last_ret_v = console_execute(this);
                if (this->last_ret_v == -EEXIST) {
                    // this->rxbuf[this->rx_idx - 1] = '\0';
                    console_send_str(this, this->cmdbuf);
                    console_send_str(this, ": no such command");
                }
            }

            this->cur_idx = 0;
            this->cmdbuf[0] = '\0';
            console_send_strln(this, "");
            console_display_prefix(this);
            break;

        /* ignore list */
        case '\n':
        case '\t':
        case '\026': break;

        // up arrow is \033A down \033B right \033C left \033D
        case '\033': this->current_state = console_state_033; break;

        default:
            if (this->cur_idx >= this->buffer_size) {
                console_send_strln(this,
                                   "console buffer full, drop previous data");
                this->cur_idx = 0;
                this->cmdbuf[0] = '\0';
                console_display_prefix(this);
                console_flush(this);
                return;
            } else if (1 == this->flags.bits.echo) {
                console_send_char(this, ch);
            }

            this->cmdbuf[this->cur_idx++] = ch;
            break;
    }
}

static inline void console_update_033(console_t* this, char ch)
{
    bool exit_033 = true;

    switch (ch) {
        case 'A': // up arrow
            // console_send_strln(this, "up arrow");
            break;

        case 'B': // down arrow
            // console_send_strln(this, "down arrow");
            break;

        case 'C': // right arrow
            // console_send_strln(this, "right arrow");
            break;

        case 'D': // left arrow
            // console_send_strln(this, "left arrow");
            break;

        case '[': exit_033 = false; break;

        default: // ignore other
            break;
    }

    // back to normal state
    if (true == exit_033)
        this->current_state = console_state_normal;

    this->cur_idx--;
}

void console_update(console_t* this)
{
    CHECK_PTR(this, );

    // should be the recived len
    uint32_t recived_len = this->rx_idx;
    RETURN_IF_ZERO(recived_len, );

    FOR_I (recived_len) {
        char ch = this->rxbuf[i];

        switch (this->current_state) {
            case console_state_normal: console_update_normal(this, ch); break;

            case console_state_033: console_update_033(this, ch); break;

            default: break;
        }
    }

    this->rx_idx = 0;
    this->rxbuf[0] = '\0';

    console_flush(this);
}
