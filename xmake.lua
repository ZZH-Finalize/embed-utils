-- project
set_project('embed-utils')
set_version('0.1')
set_license('GPL-3.0')

-- project options
option('embed-utils-tests')
    set_default(false)
    set_showmenu(true)
    set_description('Enable embed-utils tests')
option_end()

option('native')
    set_default(true)
    set_showmenu(true)
    set_description('Enable native build')
option_end()

-- global settings
set_languages('gnu23')
set_warnings('everything')

-- includes
add_includedirs('.')
add_includedirs('test_cases')

-- targets
target('embed-utils')
    set_kind('static')
    
    -- add source files
    add_files('**.c|test_cases/**.c')
    
    -- add test files when enabled
    if has_config('embed-utils-tests') then
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
