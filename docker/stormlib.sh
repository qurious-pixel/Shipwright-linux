#!/bin/bash

rm -r StormLib/build
/opt/devkitpro/portlibs/switch/bin/aarch64-none-elf-cmake -B StormLib/build -S StormLib
make -C StormLib/build -j$(nproc)
make install -C StormLib/build
cp -v /usr/local/lib/libstorm.a /opt/devkitpro/portlibs/switch/lib/
cp -v /usr/local/include/Storm* /opt/devkitpro/portlibs/switch/include/

make -j$(nproc) OPTFLAGS=-O2 DEBUG=0
