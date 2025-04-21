-- project
set_project('embed-utils')
set_version('0.1')
set_license('GPL-3.0')

includes('options.lua')

-- global settings
set_languages('gnu23')
set_warnings('everything')

-- includes
add_includedirs(
    '.',
    'test_cases',
    {public = true}
)

-- targets
target('embed-utils')
    set_kind('static')

    add_options(
        'ENABLE_TEST_CASES',
        'TESTCASE_POOL_SIZE',
        'CHECK_TESTCASE_MEMPOOL',
        'ENABLE_DEMO',
        'DEFAULT_POOL_SIZE',
        'BUILTIN_CMD_ENABLE'
    )
    
    -- add source files
    add_files('**.c|test_cases/**.c')
    
    -- add test files when enabled
    if has_config('ENABLE_TEST_CASES') then
        add_files('test_cases/**.c')
    end

target('main')
    set_kind('binary')
    add_files('main.c')
    add_deps('embed-utils')
    add_linkgroups('embed-utils', {whole = true})
    set_enabled(get_config('native'))

    after_load(function (target)
        -- add map file
        if has_config('native') then
            target:add('ldflags', '-T $(projectdir)/linker.ld')
            target:add('ldflags', '-Wl,-Map,' .. path.join(target:targetdir(), target:name() .. '.map'))
        end
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
