#ifndef __LOG_H__
#define __LOG_H__

#include <stdint.h>
#include "arg_checkers.h"
#include "gnu_attributes.h"

typedef int (*log_output_t)(const char *);
extern log_output_t __log_output_low;

typedef enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_LOG,
    LOG_LEVEL_DEBUG,
} log_level_t;

extern log_level_t __current_level;

#define __LOG_OUTPUT(level, msg, ...) log_output(LOG_LEVEL_##level, msg, ##__VA_ARGS__)

// LOG_API
#define LOG_ERROR(msg, ...)    \
    __LOG_OUTPUT(ERROR,        \
                 "["__FILE__   \
                 ":%d]: " msg, \
                 __LINE__, ##__VA_ARGS__)

#define LOG_WARNING(msg, ...)  \
    __LOG_OUTPUT(WARNING,      \
                 "["__FILE__   \
                 ":%d]: " msg, \
                 __LINE__, ##__VA_ARGS__)

#define LOG_INFO(msg, ...) \
    __LOG_OUTPUT(INFO, "[%s]: " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_DBG(msg, ...)         \
    __LOG_OUTPUT(DEBUG,           \
                 "["__FILE__      \
                 "@%s:%d]: " msg, \
                 __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOG_LOG(msg, ...) \
    __LOG_OUTPUT(LOG, "[%s]: " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

// clang-format off
/** LOG API doc
ERROR代表不可恢复的错误, 也就是程序无法纠正的错误, 例如输入的参数类型有误, 必须为真的条件实际为假
WARNING代表警告, 也可以看做是程序可以纠正的错误, 例如输入的参数超出其最大值, 这种情况下程序可以重设其值
INFO代表向使用者展示的信息, 通常是一些阶段性的结果, 应当简洁概要, 例如设备初始化完成/失败
DBG(DEBUG)代表调试信息, 通常是程序中的关键节点信息, 例如一些判断条件的真假, 关键变量的数值等等
LOG代表记录信息, 通常是某个过程中的信息, 此类信息会非常详细, 且数量众多, 例如一个数据包内全部的字节

上述五个级别中, 只有ERROR信息是无法被级别控制机制屏蔽的, 其余信息可以按照等级控制
*/
// clang-format on

static inline log_level_t log_get_cur_level(void)
{
    return __current_level;
}

static inline void log_set_cur_level(log_level_t new_level)
{
    RETURN_IF(new_level > LOG_LEVEL_DEBUG, );

    __current_level = new_level;
}

static inline void log_init(log_output_t output_if)
{
    __log_output_low = output_if;
}

static inline void log_output_raw(log_level_t level, const char *msg)
{
    if ((NULL != __log_output_low) && (__current_level >= level)) {
        __log_output_low(msg);
    }
}

void log_output(log_level_t level, const char *msg, ...) GNU_PRINTF(2, 3);

#endif // __LOG_H__
