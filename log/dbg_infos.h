#ifndef __DBG_INFOS_H__
#define __DBG_INFOS_H__

#include <stdint.h>
#include "log.h"

// Use semihost redirect printf output to openocd console
#define DBG_INFO_OUTPUT(fmt, ...) __LOG_OUTPUT(DEBUG, fmt, ##__VA_ARGS__)

#define DBG_PRINT_VAR_RAW(var)                  \
    _Generic(var,                               \
        char: DBG_INFO_OUTPUT("%c", var),       \
        uint8_t: DBG_INFO_OUTPUT("0x%X", var),  \
        uint32_t: DBG_INFO_OUTPUT("0x%X", var), \
        int32_t: DBG_INFO_OUTPUT("%d", var),    \
        uint16_t: DBG_INFO_OUTPUT("0x%X", var), \
        int16_t: DBG_INFO_OUTPUT("%d", var),    \
        void *: DBG_INFO_OUTPUT("%p", var),     \
        default: DBG_INFO_OUTPUT("%u", (u32) var));

#define DBG_PRINT_VAR(var)      \
    DBG_INFO_OUTPUT(#var ": "); \
    DBG_PRINT_VAR_RAW(var)      \
    DBG_INFO_OUTPUT("\r\n")

#define DBG(x)                               \
    ({                                       \
        const typeof(x) val = x;             \
        DBG_INFO_OUTPUT(#x " = "); \
        DBG_PRINT_VAR_RAW(val);              \
        val;                                 \
    })

#endif // __DBG_INFOS_H__
