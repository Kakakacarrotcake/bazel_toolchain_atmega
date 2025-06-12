
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

def _linux_x86_64_toolchain_config_info_impl(ctx):

    tool_paths = [
        tool_path(name = "gcc", path = "/usr/bin/clang"),
        tool_path(name = "ar", path = "/usr/bin/ar"),
        tool_path(name = "cpp", path = "/usr/bin/cpp"),
        tool_path(name = "dwp", path = "/usr/bin/dwp"),
        tool_path(name = "gcov", path = "/usr/bin/gcov"),
        tool_path(name = "ld", path = "/usr/bin/ld"),
        tool_path(name = "nm", path = "/usr/bin/nm"),
        tool_path(name = "objcopy", path = "/usr/bin/objcopy"),
        tool_path(name = "objdump", path = "/usr/bin/objdump"),
        tool_path(name = "strip", path = "/usr/bin/strip"),
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
                                "-lm"
                            ]
                        )
                    ]
                )
            ]
        ),
    ]

    features = default_compile_flags_features + default_linking_flags_features

    cxx_builtin_include_directories = [
        "/usr/include/",
        "/usr/lib/llvm-14/lib/clang/"
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = cxx_builtin_include_directories,
        # I believe this value is not used but the documentation does not state it is depreceated
        compiler = "clang",
        tool_paths = tool_paths,
        toolchain_identifier = "linux_x86_64_cc_toolchain"
    )


linux_x86_64_toolchain_config_info = rule(
    implementation = _linux_x86_64_toolchain_config_info_impl,
    attrs = {},
    provides = [CcToolchainConfigInfo]
)