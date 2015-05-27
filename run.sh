#!/bin/bash
cd Toolchain
make all
cd ..
make all
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 
