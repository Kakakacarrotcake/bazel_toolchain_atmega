def _convert_to_hex_impl(ctx):
    
    output_file = ctx.outputs.out
    # Grab the toolchain provider chosen for this target
    cc_toolchain = ctx.toolchains["@bazel_tools//tools/cpp:toolchain_type"]
    objcopy_bin = cc_toolchain.cc.objcopy_executable

    args = ctx.actions.args()
    args.add("-O")
    args.add("ihex")
    args.add(ctx.file.src)
    args.add(output_file)

    ctx.actions.run(
        executable = objcopy_bin,
        inputs = depset(
            direct = [ctx.file.src],
            transitive = [cc_toolchain.cc.all_files],
        ),
        outputs = [output_file],
        arguments = [args],
        mnemonic = "ConvertingELF2hex"
    )

    return DefaultInfo(files = depset([output_file]))


convert_to_hex = rule(
    doc = "This rule uses objcopy and converts a ELF file to a .hex file.",
    implementation = _convert_to_hex_impl,
    attrs = {
        "src" : attr.label(
            mandatory = True,
            allow_single_file = True,
            doc = "ELF file to convert."
        ),
    },
    outputs = {
        "out": "%{src}.hex"
    },
    toolchains = ["@bazel_tools//tools/cpp:toolchain_type"],
)