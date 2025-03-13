# 基础镜像选择Ubuntu Jammy（LTS版本）
FROM ubuntu:jammy AS builder
ARG DEBIAN_FRONTEND=noninteractive

# 安装基础工具链
RUN apt-get update && apt-get install -y \
    qemu-system-arm gdb-multiarch cmake ninja-build git wget \
    flex bison dejagnu texinfo python3-dev libgmp-dev libmpfr-dev \
    libmpc-dev libisl-dev zlib1g-dev libexpat1-dev libboost-all-dev \
    qemu-user-static binfmt-support sudo && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# 配置多架构支持
RUN docker-apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# 创建开发者用户
RUN useradd -m -u 1000 -s /bin/bash weqasds && \
    echo "weqasds ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# 下载并编译ARM交叉编译器
USER weqasds
WORKDIR /home/weqasds
RUN git clone --depth 1 --branch releases/gcc-12.4.0 https://gcc.gnu.org/git/gcc.git gcc-src && \
    cd gcc-src && \
    ./contrib/download_prerequisites && \
    mkdir ../gcc-build && cd ../gcc-build && \
    ../gcc-src/configure --target=arm-none-eabi \
        --prefix=/home/weqasds/cross-gcc \
        --enable-languages=c,c++ \
        --disable-nls \
        --disable-libssp \
        --with-arch=armv8-a \
        --with-fpu=neon \
        --with-float=hard && \
    make -j$(nproc) && \
    make install

# 下载文档资料
RUN mkdir -p documentation && \
    wget -P documentation https://developer.arm.com/documentation/ddi0487/latest/ARM-Architecture-Reference-Manual-ARMv8-A.pdf && \
    wget -P documentation https://raw.githubusercontent.com/raspberrypi/documentation/master/configuration/boot_config.md && \
    wget -P documentation https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc.pdf && \
    wget -P documentation https://github.com/raspberrypi/firmware/raw/master/boot/overlays/README

# 环境变量配置
ENV PATH="/home/weqasds/cross-gcc/bin:${PATH}"
ENV ARM_ARCH=armv8-a
ENV RPI4_SYSROOT=/home/weqasds/rpi4-sysroot

# 创建工作目录并设置权限
RUN mkdir -p /workspace/Armdax9OS ${RPI4_SYSROOT} && \
    sudo chown -R weqasds:weqasds /workspace

# 最终工作目录设置
WORKDIR /workspace/Armdax9OS