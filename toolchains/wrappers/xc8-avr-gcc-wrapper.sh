#!/usr/bin/env bash


echo "-------------------------------------------------"
echo "-------------------------------------------------"
echo $@
echo "-------------------------------------------------"
echo "-------------------------------------------------"


# ./external/mplab_xc8_compiler+/v3.00/avr/bin/avr-gcc $@ 
# "-specs=./external/mplab_xc8_compiler+/v3.00/avr/lib/gcc/avr/5.4.0/device-specs/specs-atmega328p" "-mmcu=atmega328p"

./external/toolchains++_repo_rules+xc8-compiler/avr8-gnu-toolchain-linux_x86_64/bin/avr-gcc $@ 
