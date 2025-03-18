cmake_minimum_required(VERSION 3.17)
project(ArmDax9OS)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
#设置工具链前缀
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)

# 更新包含目录（匹配项目结构）
include_directories(
    include
    include/fs
    include/mm
    include/process
    include/syscall
    include/time
    arch/arm/include
    arch/generic/include
    keneral
    libs
    user
)

# 更新源代码收集模式（支持子目录递归）
file(GLOB_RECURSE SOURCES
    keneral/**/*.c      # 递归keneral所有子目录
    libs/**/*.c        # 递归libs所有子目录
    arch/arm/*.c       # ARM架构C代码
    arch/arm/*.S       # ARM架构汇编
    arch/*.S           # 顶层start.S汇编文件
    arch/**/*.ld       # arch下的所有链接脚本
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib -ffreestanding -Wl,--gc-sections"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)