#!/bin/bash

/opt/devkitpro/portlibs/switch/bin/aarch64-none-elf-cmake -B /soh/StormLib/build -S /soh/StormLib
make -C /soh/StormLib/build -j$(nproc)
make install -C /soh/StormLib/build
cp /usr/local/lib/libstorm.a /opt/devkitpro/portlibs/switch/lib/
cp /usr/local/include/Storm* /opt/devkitpro/portlibs/switch/include/

cp -av /usr/local/lib/libSDL2* /lib/x86_64-linux-gnu/
