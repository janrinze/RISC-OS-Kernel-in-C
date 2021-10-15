#!/bin/bash


ROM_ABSTRACT=./ro_rom.o
if [ -f "$ROM_ABSTRACT" ]; then
    echo "$ROM_EXTRACT exists."
else
    echo "$ROM_EXRACT does not exist. generating from source file."
    mkdir -p rominstall
    cd rominstall
    wget https://www.riscosopen.org/zipfiles/platform/raspberry-pi/BCM2835.5.28.zip
    unzip BCM2835.5.28.zip
    dd if=RISCOS.IMG of=ro_rom.img bs=65536 skip=1
    arm-linux-gnueabihf-objcopy -I binary -O elf32-littlearm -B armv7 ro_rom.img ../ro_rom.o
    cd ..
    rm -Rf rominstall
fi

