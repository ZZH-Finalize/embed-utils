#ifndef __GNU_ATTRIBUTES_H__
#define __GNU_ATTRIBUTES_H__

#define GNU_ATTR(...)           __attribute__((__VA_ARGS__))
#define GNU_SECTION(sec)        GNU_ATTR(__section__(#sec))
#define GNU_USED_SECTION(sec)   GNU_ATTR(__used__, __section__(#sec))
#define GNU_WEAK_SECTION(sec)   GNU_ATTR(__weak__, __section__(#sec))
#define GNU_WEAK                GNU_ATTR(__weak__)
#define GNU_HOT                 GNU_ATTR(__hot__)
#define GNU_PURE                GNU_ATTR(__pure__)
#define GNU_UNUSED              GNU_ATTR(__unused__)
#define GNU_USED                GNU_ATTR(__used__)
#define GNU_NORETURN            GNU_ATTR(__noreturn__)
#define GNU_FORMAT(fmt, fa, va) GNU_ATTR(__format__(fmt, fa, va))
#define GNU_PRINTF(fa, va)      GNU_FORMAT(__printf__, fa, va)
#define GNU_SCANF(fa, va)       GNU_FORMAT(__scanf__, fa, va)
#define GNU_INTERRUPT(type)     GNU_ATTR(__interrupt__(#type), __unused__)

#endif // __GNU_ATTRIBUTES_H__
