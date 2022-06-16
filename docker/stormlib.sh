#!/bin/bash

/opt/devkitpro/portlibs/switch/bin/aarch64-none-elf-cmake -B StormLib/build -S StormLib -- -D_POSIX_C_SOURCE=200809L
make -C StormLib/build -j$(nproc)
make install -C StormLib/build
cp /usr/local/lib/libstorm.a /opt/devkitpro/portlibs/switch/lib/
cp /usr/local/include/Storm* /opt/devkitpro/portlibs/switch/include/

cp -av /usr/local/lib/libSDL2* /lib/x86_64-linux-gnu/
