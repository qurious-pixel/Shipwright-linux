#Change "FROM ubuntu:21.04 as build" to "FROM ubuntu:20.04 as build" or lower to build on your system
#By doing so it will require you to edits things in code mainly "/libultraship/libultraship/SDLController.cpp"
#Check comment about to uncomment stuff for Ubuntu 20.04 and probably lower.
#I advice to find a way to update your Ubuntu but if you do not want I can not force so I can confirm that 20.04 is working if you
#disable some things in SDLController.cpp.
#If you test lower version nor have a better way to support a lower version feel free to tell me :)
#Good luck ~
FROM ubuntu:21.04 as build

ENV LANG C.UTF-8
ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
        binutils:i386 \
        gcc-10:i386 \
        g++-10:i386 \
        python3 \
        python \
        make \
        cmake \
        git \
        lld \
        libsdl2-dev:i386 \
        zlib1g-dev:i386 \
        libbz2-dev:i386 \
        libpng-dev:i386 \
        libgles2-mesa-dev && \
    ln -s /usr/bin/gcc-10 /usr/bin/gcc && \
    ln -s /usr/bin/gcc-10 /usr/bin/cc && \
    ln -s /usr/bin/g++-10 /usr/bin/g++ && \
    ln -s /usr/bin/g++-10 /usr/bin/c++

RUN git clone https://github.com/nigels-com/glew.git && \
    make -C /glew extensions -j$(nproc) && \
    make -C /glew install ARCH64=false

RUN mkdir /soh
WORKDIR /soh