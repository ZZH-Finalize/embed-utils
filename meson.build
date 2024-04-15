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

# Project informations
proj_name = meson.project_name()
src_dir = meson.current_source_dir()
build_dir = meson.current_build_dir()
exe_sufix = meson.get_external_property('exe_sufix', 'elf')

# Import modules
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
    'embed-utils-tests': get_option('embed-utils-tests')
}

utils_src.add_all(testcae_src)
utils_src = utils_src.apply(ss_configs)

# exe = executable(
#     exe_name,
#     link_depends: lds,
#     name_suffix: exe_sufix,
#     sources: target_srcs.sources(),
#     include_directories: target_incs,
#     dependencies: target_srcs.dependencies(),
#     c_args: target_c_args,
#     link_args: target_link_args
# )

lib = static_library('embed-utils',
    build_by_default: true,
    sources: utils_src.sources(),
    dependencies: utils_src.dependencies(),
    include_directories: include_directories
)

embed_utils_dep = declare_dependency(
    link_with: lib,
    include_directories: include_directories,
)

embed_utils_whole_dep = declare_dependency(
    link_whole: lib,
    include_directories: include_directories,
)
