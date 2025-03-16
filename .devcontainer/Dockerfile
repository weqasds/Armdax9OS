# 使用 Ubuntu Jammy 作为基础镜像
FROM ubuntu:jammy

# 设置非交互式安装
ENV DEBIAN_FRONTEND=noninteractive
# 设置环境变量
# ENV PATH="/home/weqasds/gcc-arm-none-eabi-12.4/bin:\

# ${PATH}"
# 设置gcc-none-eabi安装目录
ENV PREFIX=/home/weqasds/gcc-arm-none-eabi-12.4
ENV PATH="/home/weqasds/gcc-arm-none-eabi-12.4/bin:\
/home/weqasds/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin:$PATH"
# 安装基础工具链和依赖
RUN apt-get update && apt-get install -y \
    qemu-system-arm \
    qemu-user-static \
    gdb \
    cmake \
    git \
    wget \
    build-essential \
    flex \
    bison \
    texinfo \
    libgmp-dev \
    libmpfr-dev \
    libmpc-dev \
    libisl-dev \
    zlib1g-dev \
    libexpat-dev \
    dejagnu \
    python3 \
    sudo \
    && rm -rf /var/lib/apt/lists/*
RUN groupadd developer
# 创建开发用户
RUN useradd -u 1000 -g developer -m weqasds && \
    echo "weqasds ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers && \
    chown -R weqasds:developer /home/ && \
    mkdir /workspaces && \
    chown -R weqasds:developer /workspaces
# 切换用户
USER weqasds
#设置编译器下载目录
WORKDIR /home/weqasds
COPY ./src/src/gcc-12.4.0.tar.xz /home/weqasds/
COPY ./src/binutils-2.40.tar.xz /home/weqasds/
# 下载并编译 Binutils
RUN if [ -f ./binutils-2.40.tar.xz ];then \
    echo "存在二进制工具跳过下载。"\
    else \
        wget https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz 
RUN tar xf binutils-2.40.tar.xz && \
    mkdir binutils-build && cd binutils-build && \
    ../binutils-2.40/configure \
        --target=arm-none-eabi \
        --prefix=/home/weqasds/gcc-arm-none-eabi-12.4 \
        --disable-nls \
        --enable-lto \
        --with-sysroot \
        --with-system-zlib \
        --disable-werror && \
    make -j$(nproc) && \
    make install && \
    cd .. && rm -rf binutils-build binutils-2.40*

# 下载并编译 arm-none-eabi-gcc
RUN if [ -f ./src/gcc-12.4.0.tar.xz ];then \
    echo "存在编译器代码跳过下载。"\
    else \
    wget https://ftp.gnu.org/gnu/gcc/gcc-12.4.0/gcc-12.4.0.tar.xz
# RUN tar xf gcc-12.4.0.tar.xz && \
#     mkdir gcc-build && cd gcc-build && \
#     ../gcc-12.4.0/configure \
#         --target=arm-none-eabi \  
#         --prefix=/home/weqasds/gcc-arm-none-eabi-12.4 \
#         --enable-languages=c,c++ \
#         --disable-nls \
#         --disable-shared \
#         --disable-werror \
#         --without-headers && \
#     make -j$(nproc) all-gcc && \
#     make all-target-libgcc &&\
#     make install-gcc && \
#     make install-target-libgcc \
#     cd .. && rm -rf gcc-build gcc-12.4.0*

# # 安装树莓派工具链
# RUN git clone https://github.com/raspberrypi/tools rpi-tools

# 安装 arm-linux-eabi 工具链
RUN sudo apt-get update && sudo apt-get install -y \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf

# 创建文档目录并下载参考资料
# 在项目中已经存在不再需要
# RUN mkdir -p documentation && cd documentation && \
#     mkdir arm gcc raspberrypi && \
#     # ARM 文档
#     wget -O arm/ARMv8-A-Architecture-Reference-Manual.pdf \
#     https://developer.arm.com/documentation/ddi0487/latest && \
#     # GCC 文档
#     wget -O gcc/GCC-Linking-Scripts.pdf \
#     https://sourceware.org/binutils/docs-2.40/ld.pdf && \
#     # 树莓派文档
#     git clone --depth 1 https://github.com/raspberrypi/documentation raspberrypi/pi4-docs




# 设置工作目录
WORKDIR /workspaces