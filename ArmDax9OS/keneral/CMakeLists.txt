cmake_minimum_required(VERSION 3.17)
project(Kernel)
set(kernel_target "kernel.img")



#添加编译的子目录
add_subdirectory(${root_dir}/arch)
add_subdirectory(${root_dir}/libs)
add_subdirectory(device)
add_subdirectory(fs)
add_subdirectory(ipc)
add_subdirectory(irq)
add_subdirectory(process)
add_subdirectory(object)
add_subdirectory(elf)
add_subdirectory(mm)
add_subdirectory(sched)
add_subdirectory(syscall)
add_subdirectory(time)

#include directory
target_include_directories(${kernel_target} PRIVATE ${root_dir}/include)
target_include_directories(${kernel_target} PRIVATE ${root_dir}/arch/arm)


add_executable(${kernel_target})
