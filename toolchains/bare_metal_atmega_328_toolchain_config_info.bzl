
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",    
    "flag_group", 
    "flag_set",   
    "tool_path",
)

compile_actions = [
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.cpp_module_codegen,
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

    print(ctx.files.tool_paths[0].path)
    tool_paths = [
        # tool_path(name = "gcc", path = "external/mplab_xc8_compiler/bin/xc8-clangd"),
        tool_path(name = "gcc", path = ctx.files.tool_paths[0].path),

                                        
        tool_path(name = "ar", path = "/mplab_xc8_compiler/bin/xc8-ar"),
        # tool_path(name = "ccov", path = "bin/xc8-ar"),
        # tool_path(name = "lm", path = "bin/xc8-ar"),
        tool_path(name = "objcopy", path = "external/mplab_xc8_compiler/bin/avr-objcopy"),
        tool_path(name = "objdump", path = "external/mplab_xc8_compiler/bin/avr-objdump"),
        # tool_path(name = "gcov", path = tool_paths_list[5]),
        tool_path(name = "nm", path = "external/mplab_xc8_compiler/bin/xc8-ar"),
        tool_path(name = "strip", path = "external/mplab_xc8_compiler/bin/avr/bin/avr-strip"),
        tool_path(name = "ld", path = "external/mplab_xc8_compiler/bin/avr/bin/avr-ld"),
        tool_path(name = "cpp", path = "external/mplab_xc8_compiler/bin/avr/bin/avr-cpp"),


    ]

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
                                "-std=c++17"
                            ]
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
                    # with_features = [""]
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
                                "-lstdc++",
                                # "-lm"
                            ]
                        )
                    ]
                )
            ]
        ),
    ]

    features = default_compile_flags_features + default_linking_flags_features

    cxx_builtin_include_directories = [
        # "/usr/include/",
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = cxx_builtin_include_directories,
        # I believe this value is not used but the documentation does not state it is depreceated
        compiler = "xc8",
        tool_paths = tool_paths,
        toolchain_identifier = "bare_metal_atmega_328_toolchain"
    )


bare_metal_atmega_328_toolchain_config_info = rule(
    implementation = _bare_metal_atmega_328_toolchain_config_info_impl,
    attrs = {
        "tool_paths": attr.label_list(mandatory = True, allow_files = True),
    },
    provides = [CcToolchainConfigInfo]
)