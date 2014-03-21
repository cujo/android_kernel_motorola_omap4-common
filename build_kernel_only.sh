#!/bin/bash
######################################
#	   KERNEL BUILDER	     #
######################################
#
#(c)2014 dtrail <mnl.manz@gmail.com>

###################################
# Start preparing the environment #
###################################

#
# Declare functions
#

alias cls='echo -en "\x0c"'
function clear {
	local CLEAR=''

	type -p clear >/dev/null && \
		CLEAR=$(exec clear)
	[[ -z $CLEAR ]] && type -p tput >/dev/null && \
		CLEAR=$(exec tput clear)
	[[ -z $CLEAR ]] && \
		CLEAR=$'\e[H\e[2J'

	echo -en "$CLEAR"

	eval "function clear { echo -en '$CLEAR'; }"
}

#
# Make sure we have permission access
#

chmod +x spyder
chmod +x edison
chmod +x targa

# 
# Prepare compiler
#	
set -m
echo \n
echo "Welcome to the JBX-KERNEL-BUILDER!\n"

export ARCH=arm
export SUBARCH=arm
export PATH=/data/4.4/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin:$PATH
export CROSS_COMPILE=arm-eabi-
##########################################
# preparation done! Continue with script #
##########################################

######################################
#	     Main Menu		     #
######################################
main:
# clear the screen here
clear
# Choose option
while true; do
    read -p "TEST BUILDER: [1]Compiling Test [2]Build full package [3]Clean build dirs [4]Exit : " 1234
    case $yn in
        [1]* ) goto testb;;
        [2]* ) goto full;;
	[3]* ) make mrproper; make clean; return;;
	[4]* ) exit;;
        * ) echo "Enter an number from 1-4 ";;
    esac
done

######################################
#	     TEST - BUILD	     #
######################################
testb:
# clear the screen here
clear
# Select device
while true; do
    read -p "Select device: [1]Spyder [2]Edison [3]Targa [4]Abort : " 1234
    case $yn in
        [1]* ) make mapphone_OCE_defconfig; make -j4; echo "Test Kernel built!\n"; sleep 1; goto main;;
        [2]* ) make mapphone_OCEdison_defconfig; make -j4; echo "Test Kernel built!\n"; sleep 1; goto main;;
	[3]* ) make mapphone_OCTarga_defconfig; make -j4; echo "Test Kernel built!\n"; sleep 1; goto main;;
	[4]* ) break;;
        * ) echo "Enter an number from 1-4 ";;
    esac
done


######################################
#	     FULL - BUILD	     #
######################################
full:
# clear the screen here
clear
# Select device
while true; do
    read -p "Select device: [1]Spyder [2]Edison [3]Targa [4]Abort : " 1234
    case $yn in
        [1]* ) sh spyder; exit;;
        [2]* ) sh edison; exit;;
	[3]* ) sh targa; exit;;
	[4]* ) break;;
        * ) echo "Enter an number from 1-4 ";;
    esac
done
