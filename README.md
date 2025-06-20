# BAZEL TOOLCHAINS EXPERIMENT

There are two toolchains set. One is for regular linux x86-64 target and execution, other is for avr compiler with the linux x86-64 set as execution and arduino nano (atmega328) set as target platform, this toolchain is hermetic as well.


Build the code in hex format directly with: <br />
    *"bazelisk build @code_repo//:main_hex --platforms=@toolchains//:bare_metal_atmega_328"* <br />
Then the code is uploaded to the board with the help of avrdude: <br />
    *"avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:bazel-bin/external/code_repo+/main.hex:i"*


Build the code in ELF format with: <br />
    *"bazelisk build @code_repo//:main --platforms=@toolchains//:bare_metal_atmega_328"* <br />
The generated object code is not in .hex format, in order to convert we run: <br />
    *"./avr-objcopy -O ihex bazel-bin/external/code_repo+/main main.hex"* <br />
Then the code is uploaded to the board with the help of avrdude: <br />
    *"avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:main.hex:i"*






