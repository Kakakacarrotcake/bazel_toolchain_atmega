
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",    
    "flag_group", 
    "flag_set",   
    "tool_path",
    "action_config",
    "tool",
)
load("@bazel_skylib//lib:paths.bzl", "paths")
load("//:tools/paths_helper.bzl", "AVR_COMPILER")

# AVR is a c compiler so cpp actions are not used.
compile_actions = [
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
    ACTION_NAMES.c_compile,
    # ACTION_NAMES.cpp_compile,
    ACTION_NAMES.linkstamp_compile,
    # ACTION_NAMES.cpp_header_parsing,
    # ACTION_NAMES.cpp_module_compile,
    # ACTION_NAMES.cpp_module_codegen,
    # ACTION_NAMES.lot_backend,
    ACTION_NAMES.clif_match
]

link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    # ACTION_NAMES.cpp_link_static_library,
]

def _bare_metal_atmega_328_toolchain_config_info_impl(ctx):

    tool_paths = [
        tool_path(name = "ar", path = "wrappers/avr-ar-wrapper.sh"),
        tool_path(name = "objcopy", path = "wrappers/avr-objcopy-wrapper.sh"),
        tool_path(name = "objdump", path = "wrappers/avr-objdump-wrapper.sh"),
        tool_path(name = "nm", path = "wrappers/avr-nm-wrapper.sh"),
        tool_path(name = "strip", path = "wrappers/avr-strip-wrapper.sh"),
        tool_path(name = "ld", path = "wrappers/avr-ld-wrapper.sh"),
        tool_path(name = "cpp", path = "wrappers/avr-cpp-wrapper.sh"),
        tool_path(name = "gcc", path = "wrappers/avr-gcc-wrapper.sh"),

    ]

    all_include_dirs = set([])
    # For some reason for some files like in "include/avr" both directories need to be separately included.
    # Once ending in "/include/avr" and another as "/include".
    for path in ctx.files.include_directories:
        path_str = path.path
        # rstrip was not working so I decided to go with this option.
        number_of_chars_in_include = 9 
        include_dir = path_str[0:path_str.find("/include/") + number_of_chars_in_include]
        all_include_dirs.add("-I" + include_dir)
        all_include_dirs.add("-I" + paths.dirname(path_str))

    default_compile_flags_features = [
        feature(
            name = "default_compile_features",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Wall",
                                "-v",
                                "-mmcu=atmega328p",
                                "-nostdlib",
                                "-nostdinc",
                                "-nodefaultlibs",
                                "-nostartfiles",
                                "-Os",
                            ]
                        ),
                        flag_group(
                            flags = list(all_include_dirs)
                        )
                    ]
                )
            ]
        ),
        feature(
            name = "treat_warnings_as_errors",
            flag_sets = [
                flag_set(
                    actions = compile_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-Werror"
                            ]
                        )
                    ]
                )
            ]
        )
    ]

    default_linking_flags_features = [
        feature(
            name = "default_linking_features",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = link_actions,
                    flag_groups = [
                        flag_group(
                            flags = [
                                "-mmcu=atmega328p",
                                "-mrelax",
                                "-mshort-calls",
                                "-fwhole-program"
                            ]
                        )
                    ]
                )
            ]
        ),
    ]

    features = default_compile_flags_features + default_linking_flags_features

    cxx_builtin_include_directories = [
        "%package(@@" + AVR_COMPILER + "//)%/v3.00/avr/avr/include" 
    ]

    objcopy_cfg = action_config(
        action_name = "objcopy_action",
        tools = [tool(path = "wrappers/avr-objcopy-wrapper.sh")],
    )

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = cxx_builtin_include_directories,
        # I think this value is not used but the documentation does not state it is depreceated
        compiler = "avr",
        tool_paths = tool_paths,
        action_configs = [objcopy_cfg],
        toolchain_identifier = "bare_metal_atmega_328_toolchain"
    )


bare_metal_atmega_328_toolchain_config_info = rule(
    implementation = _bare_metal_atmega_328_toolchain_config_info_impl,
    attrs = {
        "include_directories": attr.label_list(
            allow_files = True,
            mandatory = True,
            doc = "Directories to be included in the compilation process."
        ),
    },
    provides = [CcToolchainConfigInfo]
)