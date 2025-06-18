
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",    
    "flag_group", 
    "flag_set",   
    "tool_path",
)
load("//:tools/paths_helper.bzl", "MPLAB_XC8_COMPILER_REPO_NAME")

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
        tool_path(name = "gcc", path = "wrappers/xc8-cc-wrapper.sh"),
        tool_path(name = "ar", path = "wrappers/xc8-ar-wrapper.sh"),
        tool_path(name = "objcopy", path = "wrappers/xc8-objcopy-wrapper.sh"),
        tool_path(name = "objdump", path = "wrappers/xc8-objdump-wrapper.sh"),
        tool_path(name = "nm", path = "wrappers/xc8-nm-wrapper.sh"),
        tool_path(name = "strip", path = "wrappers/xc8-strip-wrapper.sh"),
        tool_path(name = "ld", path = "wrappers/xc8-ld-wrapper.sh"),
        tool_path(name = "cpp", path = "wrappers/xc8-cpp-wrapper.sh"),
        tool_path(name = "avr-gcc", path = "wrappers/xc8-avr-gcc-wrapper.sh"),
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
                                "-mcpu=atmega328",
                                "-v",
                                "-Iexternal/mplab_xc8_compiler+/v3.00/avr/avr/include/avr",
                                "-Iexternal/mplab_xc8_compiler+/v3.00/avr/avr/include/",
                                "-Iexternal/mplab_xc8_compiler+/v3.00/dfp/xc8/avr/include/",
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
                                "-mcpu=atmega328",
                            ]
                        )
                    ]
                )
            ]
        ),
    ]

    features = default_compile_flags_features + default_linking_flags_features

    cxx_builtin_include_directories = [
        "%package(@@" + MPLAB_XC8_COMPILER_REPO_NAME + "//)%/v3.00/avr/avr/include" 
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = cxx_builtin_include_directories,
        # I think this value is not used but the documentation does not state it is depreceated
        compiler = "xc8",
        tool_paths = tool_paths,
        toolchain_identifier = "bare_metal_atmega_328_toolchain"
    )


bare_metal_atmega_328_toolchain_config_info = rule(
    implementation = _bare_metal_atmega_328_toolchain_config_info_impl,
    attrs = {},
    provides = [CcToolchainConfigInfo]
)