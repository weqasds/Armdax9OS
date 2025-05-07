#!/bin/bash
#以scripts文件夹为根目录
set -e  # 发生任何错误时退出脚本
cp -v -p .././src/gcc-12.4.0.tar.xz ../../.././home/weqasds
cd ../../.././home/weqasds

tar xf gcc-12.4.0.tar.xz

mkdir gcc-build
cd gcc-build

../gcc-12.4.0/configure \
    --target=arm-none-eabi \
    --prefix=/home/weqasds/gcc-arm-none-eabi-12.4 \
    --enable-languages=c,c++ \
    --disable-nls \
    --disable-shared \
    --disable-werror \
    --without-headers

make -j$(nproc) all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

cd ..
rm -rf gcc-build gcc-12.4.0*
