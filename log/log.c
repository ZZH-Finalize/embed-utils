#include <stdio.h>
#include <stdarg.h>

#include "log.h"

log_level_t __current_level = LOG_LEVEL_INFO;
log_output_t __log_output_low = NULL;

void log_output(log_level_t level, const char *msg, ...)
{
    char buf[256];
    va_list vargs;
    va_start(vargs, msg);

    vsnprintf(buf, sizeof(buf), msg, vargs);

    log_output_raw(level, buf);

    va_end(vargs);
}
