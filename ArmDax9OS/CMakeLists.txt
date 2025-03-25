cmake_minimum_required(VERSION 3.17)
project(ArmDax9OS)

#配置编译参数
set(PLATFORM "arm64" CACHE STRING "Target platform: [arm64|raspberry|qemu_arm64]")
# 根据用户选择的平台设置 CONFIG_LOAD_OFFSET 的值
if(PLATFORM STREQUAL "arm64")
    set(CONFIG_LOAD_OFFSET 1)
elseif(PLATFORM STREQUAL "raspberry")
    set(CONFIG_LOAD_OFFSET 2)
elseif(PLATFORM STREQUAL "qemu_arm64")
    set(CONFIG_LOAD_OFFSET 3)
else()
    message(FATAL_ERROR "Unsupported platform: ${PLATFORM}")
endif()

#设置工具链前缀
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
#设置构建目录
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(${CMAKE_INSTALL_PREFIX} "${CMAKE_SOURCE_DIR}/../out/prefix")
set(${CMAKE_BINARY_DIR} "${CMAKE_SOURCE_DIR}/../out/build")
# 更新包含目录（匹配项目结构）
# # 后边子目录其他头文件使用时不需要再mm/xx.h 
# include_directories(
#     include
#     include/fs
#     include/mm
#     include/process
#     include/syscall
#     include/time
#     arch/arm/include
#     arch/generic/include
#     keneral
#     libs
#     user
# )

# #添加编译的子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/arch)
add_subdirectory(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/keneral)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs)


install(TARGETS ${PROJECT_NAME} DESTINATION bin)