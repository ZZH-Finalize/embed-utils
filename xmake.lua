-- project
set_project('embed-utils')
set_version('0.1')
set_license('GPL-3.0')
add_rules('mode.debug', 'mode.release')

includes('options.lua')

if has_config('toolchain') then
    set_toolchains('$(toolchain)')
else
    set_toolchains('gcc')
end

-- global settings
set_languages('gnu23')
set_warnings('everything')

-- targets
target('embed-utils')
    set_kind('static')

    -- includes
    add_includedirs(
        '.',
        'test_cases',
        {public = true}
    )

    -- add_options(
    --     'ENABLE_TEST_CASES',
    --     'TESTCASE_POOL_SIZE',
    --     'CHECK_TESTCASE_MEMPOOL',
    --     'ENABLE_DEMO',
    --     'DEFAULT_POOL_SIZE',
    --     'BUILTIN_CMD_ENABLE'
    -- )

    if has_config('ENABLE_TEST_CASES') then
        add_defines('CONFIG_ENABLE_TEST_CASES', {public = true})
    end

    if has_config('CHECK_TESTCASE_MEMPOOL') then
        add_defines('CONFIG_CHECK_TESTCASE_MEMPOOL', {public = true})
    end

    if has_config('ENABLE_DEMO') then
        add_defines('CONFIG_ENABLE_DEMO', {public = true})
    end

    add_defines(
        'CONFIG_DEFAULT_POOL_SIZE=$(DEFAULT_POOL_SIZE)',
        'CONFIG_TESTCASE_POOL_SIZE=$(TESTCASE_POOL_SIZE)',
        {public = true}
    )
    
    if has_config('BUILTIN_CMD_ENABLE') then
        add_defines('CONFIG_CONSOLE_BUILTIN_CMD_ENABLE', {public = true})
    end

    -- add source files
    add_files('**.c|test_cases/**.c|main.c')
    
    -- add test files when enabled
    if has_config('ENABLE_TEST_CASES') then
        add_files('test_cases/**.c')
    end

target('main')
    set_kind('binary')
    add_deps('embed-utils')
    set_extension('.exe')
    add_linkgroups('embed-utils', {whole = true})
    set_enabled(get_config('native'))

    add_files(
        'main.c',
        'linker.ld'
    )

    after_load(function (target)
        -- add map file
        target:add('ldflags', '-Wl,-Map,' .. path.join(target:targetdir(), target:name() .. '.map'))
    end)
    
    -- custom rules for objdump and size
    after_build(function (target)
        os.cd(target:targetdir())

        os.runv('objdump', 
            {'-Sd', '--visualize-jumps', target:targetfile()}, 
            {stdout = target:name() .. '.s'}
        )

        os.execv('size', {target:targetfile()})
    end)
