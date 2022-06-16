#!/bin/bash

git config --global --add safe.directory /soh
make setup -C soh -j$(nproc) OPTFLAGS=-O2 DEBUG=0 CC="gcc" CXX="g++""

