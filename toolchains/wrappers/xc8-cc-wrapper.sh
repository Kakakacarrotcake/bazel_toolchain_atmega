$!/usr/bin/env bash

echo ------------------------------------------
echo ------------------------------------------
echo $@
echo ------------------------------------------
echo ------------------------------------------

# export PATH="/home/nedzad/Desktop/bazel_experiments/gtest_atmega_328/mplab_xc8_compiler/bin/avr/bin:$PATH" 

# echo $PATH
./external/mplab_xc8_compiler+/v3.00/bin/xc8-cc $@