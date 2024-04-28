project('embed-utils', 'c',
    version: '0.1',
    license: 'GPL-3.0',
    license_files: 'LICENSE',
    default_options: {
        'warning_level': 'everything',
        'c_std': 'gnu17',
        'buildtype': 'debug'
    }
)

# Get toolchain
# objcopy_tool = find_program('objcopy')
objdump_tool = find_program('objdump')
size_tool = find_program('size')

# Project informations
proj_name = meson.project_name()
src_dir = meson.current_source_dir()
build_dir = meson.current_build_dir()
exe_sufix = meson.get_external_property('exe_sufix', 'elf')

# Import modules
fs = import('fs')
ssmod = import('sourceset')

# Global informations
utils_src = ssmod.source_set()
testcae_src = ssmod.source_set()
include_directories = include_directories([
        '.',
        'test_cases'
    ]
)

utils_src.add(files(
        'bitmap/bitmap.c',
        'hash/str_hash.c',
        'map/map.c',
        'mem_mana/mem_mana.c',
        'ringbuf/ringbuf.c',
        'tiny_console/tiny_console.c',
        'tiny_console/builtin_cmds/help.c',
        'strarg.c',
        'value_ops.c',
    ),
)

testcae_src.add(
    when: 'embed-utils-tests',
    if_true: files(
        'test_cases/bitmap/bitmap_basic.c',
        'test_cases/bitmap/bitmap_clear.c',
        'test_cases/map/map_basic_test.c',
        'test_cases/map/map_detete_clear_test.c',
        'test_cases/map/map_duplcate_key.c',
        'test_cases/map/map_test_data.c',
        'test_cases/test_frame.c',
    ),
)

ss_configs = {
    'embed-utils-tests': get_option('embed-utils-tests'),
    'native': get_option('native')
}

ld_dep = declare_dependency(
    link_args: [
        '-T' + files('linker.ld')[0].full_path(),
        '-Wl,-Map,memory.map'
    ]
)

utils_src.add(
    when: 'native',
    if_true: ld_dep
)

utils_src.add(
    declare_dependency(
        compile_args: '-Wno-c++-compat'
    )
)

utils_src.add_all(testcae_src)
utils_src = utils_src.apply(ss_configs)

lib = static_library('embed-utils',
    build_by_default: true,
    sources: utils_src.sources(),
    dependencies: utils_src.dependencies(),
    include_directories: include_directories
)

exe = executable(
    'main',
    link_whole: lib,
    # name_suffix: exe_sufix,
    sources: [
        'main.c'
    ],
    native: true,
    # dependencies: utils_src.dependencies(),
)

dis = custom_target(
    'main' + '_dis',
    depends: exe,
    capture: true,
    command: [
        objdump_tool.full_path(),
        '-SD',
        exe.full_path()
    ],
    output: fs.replace_suffix(exe.name(), '.s')
)

run_target(
    'main' + '_size',
    depends: dis,
    command: [
        size_tool.full_path(),
        exe.full_path(),
    ]
)

embed_utils_dep = declare_dependency(
    link_with: lib,
    include_directories: include_directories,
)

embed_utils_whole_dep = declare_dependency(
    link_whole: lib,
    include_directories: include_directories,
)
