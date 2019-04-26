#!/bin/bash

if [ -z "$1" ]; then
	echo "Please provide a file name"
fi

FULL_NAME=$1
NAME="${FULL_NAME%.*}"
OFILE="$NAME.o"

# Compiling an .asm Source File
nasm -felf64 $1 -o $OFILE
ld -o $NAME $OFILE
