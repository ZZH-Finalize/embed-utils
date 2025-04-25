#include <stdint.h>
#include <stdio.h>
#include "test_frame.h"

void memInit(void);

int main(const int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    memInit();

    test_case_arg_t arg = {
        .print = printf,
    };

    run_all_demo(&arg);
    run_all_testcases(&arg);

    return 0;
}
