-- project options
option('toolchain')
    set_default('gcc')
    set_description('select compile toolchain')

option('native')
    set_default(false)
    set_showmenu(true)
    set_description('build native test code')

option('ENABLE_TEST_CASES')
    set_default(false)
    -- add_defines('CONFIG_ENABLE_TEST_CASES', {public = true})

option('TESTCASE_POOL_SIZE')
    set_default('0')
    -- add_defines('CONFIG_TESTCASE_POOL_SIZE=$(TESTCASE_POOL_SIZE)', {public = true})

option('CHECK_TESTCASE_MEMPOOL')
    set_default(false)
    -- add_defines('CONFIG_CHECK_TESTCASE_MEMPOOL', {public = true})

option('ENABLE_DEMO')
    set_default(false)
    -- add_defines('CONFIG_ENABLE_DEMO', {public = true})

option('DEFAULT_POOL_SIZE')
    set_default('4')
    -- add_defines('CONFIG_DEFAULT_POOL_SIZE=$(DEFAULT_POOL_SIZE)', {public = true})

option('BUILTIN_CMD_ENABLE')
    set_default(true)
    -- add_defines('CONFIG_CONSOLE_BUILTIN_CMD_ENABLE', {public = true})
