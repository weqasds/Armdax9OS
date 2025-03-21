
# 直接使用根目录定义的 CONFIG_LOAD_OFFSET
target_compile_definitions(${PROJECT_NAME} PRIVATE CONFIG_LOAD_OFFSET=${CONFIG_LOAD_OFFSET})
# Add architecture-specific include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/generic/include
)