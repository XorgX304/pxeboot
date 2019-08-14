#!/bin/bash

if [[ $1 == "make" ]]; then
    make image
    cd bootloader_pxe/
    make bootloader
elif [[ $1 == "clean" ]]; then
    make clean
fi
