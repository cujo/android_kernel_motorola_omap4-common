#!/bin/bash

# Ramdisk needs addition to fix 10% battery

rm ramdisk.img
cd ramdisk
gzip -dc ../ramdisk.img | cpio -i

echo " "
echo "Edit files: add ':/system/framework/telephony-msim.jar' to BOOTCLASSPATH "
echo " "

sleep

find . | cpio -o -H newc | gzip > ../ramdisk.img

# end ramdisk


