#!/bin/bash
apt-get update && \
apt-get install -y --no-install-recommends \
    qemu-system-arm gdb-multiarch cmake ninja-build git wget \
    flex bison dejagnu texinfo python3-dev libgmp-dev libmpfr-dev \
    libmpc-dev libisl-dev zlib1g-dev libexpat1-dev libboost-all-dev \
    qemu-user-static binfmt-support sudo