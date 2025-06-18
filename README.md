# BAZEL TOOLCHAINS EXPERIMENT

There are two toolchains set. One is for regular linux x86-64 target and execution, other is for xc8 compiler with the linux x86-64 set as execution and arduino nano (atmega328) set as target platform, this toolchain is meant to be hermetic as well.
XC8 Compiler data is not included in the git repository.

<p>Generated object code is not in .hex format which is used to upload to the board, in order to convert we run: <br>
    *"./mplab_xc8_compiler/v3.00/bin/avr-objcopy -O ihex bazel-bin/external/code_repo+/main ../main.hex"* 

Then the code is uploaded to the board with the help of avrdude: <br>
    *"avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:../main.hex:i"* </p>
