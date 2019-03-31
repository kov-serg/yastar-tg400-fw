#!/bin/sh

wget https://www.yeastar.eu/downloads/53.18.0.46.bin
unzip -P qazxcvvfrewsd321 53.18.0.46.bin
tar -xf firmware.tar
gcc -o make-ecc make-ecc.c
gcc -o make-2 make-2.c
./make-ecc firmware/firmware.ext2 firmware-with-ecc.bin
./make-2 firmware-with-ecc.bin S34ML01G2-TG400-firmware-53.18.0.46.bin
