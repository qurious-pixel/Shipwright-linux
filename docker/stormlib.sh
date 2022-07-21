#!/bin/bash

export PATH=$PATH:/opt/devkitpro/portlibs/switch/bin/

/opt/devkitpro/portlibs/switch/bin/aarch64-none-elf-cmake -B StormLib/build-switch -S StormLib -DCMAKE_INSTALL_PREFIX=/opt/devkitpro/portlibs/switch/
make -C StormLib/build-switch -j$(nproc)
make install -C StormLib/build-switch

make -f Makefile.switch -j$(nproc) OPTFLAGS=-O2 DEBUG=0
