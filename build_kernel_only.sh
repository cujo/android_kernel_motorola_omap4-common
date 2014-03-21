export ARCH=arm
export SUBARCH=arm
export PATH=/data/4.4/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin:$PATH
export CROSS_COMPILE=arm-eabi-
make mapphone_OCE_defconfig
echo "Now type make -jX (X stands for your amount of CPU cores)\n"
