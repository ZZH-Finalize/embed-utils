#ifndef __CONSOLE_COLOR_H__
#define __CONSOLE_COLOR_H__

#define CONCOLE_COLOR_BLACK            "0"
#define CONCOLE_COLOR_RED              "1"
#define CONCOLE_COLOR_GREEN            "2"
#define CONCOLE_COLOR_YELLOW           "3"
#define CONCOLE_COLOR_BLUE             "4"
#define CONCOLE_COLOR_PURPLE           "5"
#define CONCOLE_COLOR_DARK_GREEN       "6"
#define CONCOLE_COLOR_WHITE            "7"

// todo: add macro control

#define CONSOLE_COLOR_SEQ(front, back) "\033[4" back ";3" front "m"
#define CONSOLE_COLOR_SEQ_END          "\033[0m"

#define CONSOLE_COLORED_STR(str, front, back) \
    CONSOLE_COLOR_SEQ(front, back) str CONSOLE_COLOR_SEQ_END

#endif // __CONSOLE_COLOR_H__
