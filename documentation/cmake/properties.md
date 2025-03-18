# CMake属性
CMake 使用属性来控制构建过程的各个方面，这些属性可以应用于全局范围、目录、目标、测试、源文件、缓存条目以及已安装文件等。以下是 CMake 中各类属性的参考文档，涵盖其说明和用途。

## 全局范围的属性

#### - **ALLOW_DUPLICATE_CUSTOM_TARGETS**：允许在不同目录中定义同名的自定义目标。
#### - **AUTOGEN_SOURCE_GROUP**：指定由自动生成的源文件应放置的源组名称。
#### - **AUTOGEN_TARGETS_FOLDER**：定义自动生成的目标应放置的文件夹。
#### - **AUTOMOC_SOURCE_GROUP**：指定由 AUTOMOC 生成的源文件应放置的源组名称。
#### - **AUTOMOC_TARGETS_FOLDER**：定义 AUTOMOC 生成的目标应放置的文件夹。
#### - **AUTORCC_SOURCE_GROUP**：指定由 AUTORCC 生成的源文件应放置的源组名称。
#### - **AUTOUIC_SOURCE_GROUP**：指定由 AUTOUIC 生成的源文件应放置的源组名称。
#### - **CMAKE_C_KNOWN_FEATURES**：列出 C 语言已知的特性。
#### - **CMAKE_CUDA_KNOWN_FEATURES**：列出 CUDA 语言已知的特性。
#### - **CMAKE_CXX_KNOWN_FEATURES**：列出 C++ 语言已知的特性。
#### - **CMAKE_HIP_KNOWN_FEATURES**：列出 HIP 语言已知的特性。
#### - **CMAKE_ROLE**：指示 CMake 的运行角色，例如构建、测试等。
#### - **DEBUG_CONFIGURATIONS**：指定哪些配置被视为调试配置。
#### - **DISABLED_FEATURES**：列出被禁用的特性。
#### - **ECLIPSE_EXTRA_CPROJECT_CONTENTS**：为 Eclipse 生成器添加额外的 cproject 内容。
#### - **ECLIPSE_EXTRA_NATURES**：为 Eclipse 生成器添加额外的 natures。
#### - **ENABLED_FEATURES**：列出已启用的特性。
#### - **ENABLED_LANGUAGES**：列出已启用的编程语言。
#### - **FIND_LIBRARY_USE_LIB32_PATHS**：在查找库时使用 lib32 路径。
#### - **FIND_LIBRARY_USE_LIB64_PATHS**：在查找库时使用 lib64 路径。
#### - **FIND_LIBRARY_USE_LIBX32_PATHS**：在查找库时使用 libx32 路径。
#### - **FIND_LIBRARY_USE_OPENBSD_VERSIONING**：在 OpenBSD 上查找库时使用版本控制。
#### - **GENERATOR_IS_MULTI_CONFIG**：指示生成器是否支持多配置。
#### - **GLOBAL_DEPENDS_DEBUG_MODE**：启用全局依赖项的调试模式。
#### - **GLOBAL_DEPENDS_NO_CYCLES**：防止全局依赖项循环。
#### - **INSTALL_PARALLEL**：控制安装步骤的并行度。
#### - **IN_TRY_COMPILE**：指示当前是否在 try_compile 调用中。
#### - **JOB_POOLS**：定义可用于构建的作业池。
#### - **PACKAGES_FOUND**：列出已找到的包。
#### - **PACKAGES_NOT_FOUND**：列出未找到的包。
#### - **PREDEFINED_TARGETS_FOLDER**：指定预定义目标应放置的文件夹。
#### - **PROPAGATE_TOP_LEVEL_INCLUDES_TO_TRY_COMPILE**：将顶级包含目录传播到 try_compile。
#### - **REPORT_UNDEFINED_PROPERTIES**：报告未定义的属性。
#### - **RULE_LAUNCH_COMPILE**：设置编译规则的启动器命令。
#### - **RULE_LAUNCH_CUSTOM**：设置自定义规则的启动器命令。
#### - **RULE_LAUNCH_LINK**：设置链接规则的启动器命令。
#### - **RULE_MESSAGES**：控制规则消息的显示。
#### - **TARGET_ARCHIVES_MAY_BE_SHARED_LIBS**：指示目标存档可能是共享库。
#### - **TARGET_MESSAGES**：控制目标消息的显示。
#### - **TARGET_SUPPORTS_SHARED_LIBS**：指示目标是否支持共享库。
#### - **USE_FOLDERS**：在生成器中使用文件夹组织目标。
#### - **XCODE_EMIT_EFFECTIVE_PLATFORM_NAME**：在 Xcode 生成器中发出有效的平台名称。

## 目录上的属性

#### - **ADDITIONAL_CLEAN_FILES**：指定在执行清理时应删除的额外文件。
#### - **BINARY_DIR**：获取当前目录的二进制目录路径。
#### - **BUILDSYSTEM_TARGETS**：列出构建系统中定义的目标。
#### - **CACHE_VARIABLES**：列出缓存中的变量。
#### - **CLEAN_NO_CUSTOM**：在清理时不删除自定义命令生成的文件。
#### - **CMAKE_CONFIGURE_DEPENDS**：指定影响配置的依赖项，若依赖项更改，将触发重新配置。  
#### - **CMAKE_PARENT_LIST_FILE**：获取调用当前目录的 CMake 列表文件的路径。  
#### - **CMAKE_POLICY_DEFAULT_CMP<NNNN>**：设置指定编号的 CMake 策略的默认行为。  
#### - **CMAKE_POLICY_VERSION**：指定 CMake 策略的版本，以控制新特性和行为变更。  
#### - **CMAKE_SUBDIRS**：列出当前目录下的所有子目录。  
#### - **CMAKE_WARN_DEPRECATED**：在使用已弃用的功能时发出警告。  
#### - **EXCLUDE_FROM_ALL**：指定该目录中的目标不包含在默认构建中。  
#### - **IMPORTED_NO_SONAME**：指定导入目标是否忽略 `SONAME`。  
#### - **INTERPROCEDURAL_OPTIMIZATION**：启用或禁用全程序优化。  
#### - **INTERPROCEDURAL_OPTIMIZATION_<CONFIG>**：为特定构建配置启用/禁用全程序优化。  
#### - **INCLUDE_DIRECTORIES**：指定目录的包含路径。  
#### - **INSTALL_NAME_DIR**：指定安装时共享库的 `install_name`。  
#### - **INSTALL_RPATH**：定义安装时 RPATH。  
#### - **INSTALL_RPATH_USE_LINK_PATH**：指示安装时是否使用链接路径作为 RPATH。  
#### - **LINK_DIRECTORIES**：指定链接库的搜索路径。  
#### - **LINK_LIBRARIES**：指定默认链接的库。  
#### - **SOURCE_DIR**：获取当前目录的源代码路径。  

## 目标上的属性  

#### - **ARCHIVE_OUTPUT_DIRECTORY**：指定目标的归档文件输出目录。  
#### - **ARCHIVE_OUTPUT_DIRECTORY_<CONFIG>**：指定某个配置下归档文件的输出目录。  
#### - **ARCHIVE_OUTPUT_NAME**：指定目标归档文件的名称。  
#### - **ARCHIVE_OUTPUT_NAME_<CONFIG>**：指定特定配置下目标归档文件的名称。  
#### - **AUTOGEN_TARGET_DEPENDS**：指定自动生成目标的依赖项。  
#### - **AUTOGEN_TARGETS_FOLDER**：指定自动生成目标的文件夹。  
#### - **AUTOMOC**：启用/禁用 Qt 的 `moc` 自动处理。  
#### - **AUTORCC**：启用/禁用 Qt 的 `rcc` 自动处理。  
#### - **AUTOUIC**：启用/禁用 Qt 的 `uic` 自动处理。  
#### - **C_EXTENSIONS**：控制 C 语言是否启用编译器扩展。  
#### - **C_STANDARD**：设置 C 语言标准版本。  
#### - **C_STANDARD_REQUIRED**：强制使用指定的 C 语言标准版本。  
#### - **CUDA_STANDARD**：设置 CUDA 语言标准版本。  
#### - **CUDA_STANDARD_REQUIRED**：强制使用指定的 CUDA 语言标准版本。  
#### - **CXX_EXTENSIONS**：控制 C++ 是否启用编译器扩展。  
#### - **CXX_STANDARD**：设置 C++ 语言标准版本。  
#### - **CXX_STANDARD_REQUIRED**：强制使用指定的 C++ 语言标准版本。  
#### - **DEBUG_POSTFIX**：在调试配置下为目标添加后缀。  
#### - **EXCLUDE_FROM_ALL**：指定目标不参与默认构建。  
#### - **EXPORT_NAME**：指定目标的导出名称。  
#### - **FRAMEWORK**：指定目标作为 macOS 框架。  
#### - **GENERATOR_FILE_NAME**：指定目标在生成器中的文件名。  
#### - **IMPORTED**：标记该目标为导入目标。  
#### - **IMPORTED_GLOBAL**：将导入目标标记为全局目标。  
#### - **INCLUDE_DIRECTORIES**：指定目标的包含路径。  
#### - **INTERFACE_COMPILE_DEFINITIONS**：指定接口编译定义。  
#### - **INTERFACE_COMPILE_FEATURES**：指定接口编译特性。  
#### - **INTERFACE_COMPILE_OPTIONS**：指定接口编译选项。  
#### - **INTERFACE_INCLUDE_DIRECTORIES**：指定接口包含目录。  
#### - **INTERFACE_LINK_DIRECTORIES**：指定接口链接目录。  
#### - **INTERFACE_LINK_LIBRARIES**：指定接口链接的库。  
#### - **INTERFACE_POSITION_INDEPENDENT_CODE**：指定接口是否支持位置无关代码。  
#### - **LIBRARY_OUTPUT_DIRECTORY**：指定目标的库文件输出目录。  
#### - **LIBRARY_OUTPUT_DIRECTORY_<CONFIG>**：指定特定配置下目标库文件的输出目录。  
#### - **LIBRARY_OUTPUT_NAME**：指定目标库文件的名称。  
#### - **LIBRARY_OUTPUT_NAME_<CONFIG>**：指定特定配置下目标库文件的名称。  
#### - **LINK_DEPENDS**：指定目标的链接依赖项。  
#### - **LINK_INTERFACE_LIBRARIES**：指定链接接口库。  
#### - **POSITION_INDEPENDENT_CODE**：指定目标是否编译为位置无关代码。  
#### - **RUNTIME_OUTPUT_DIRECTORY**：指定目标的可执行文件输出目录。  
#### - **RUNTIME_OUTPUT_DIRECTORY_<CONFIG>**：指定特定配置下目标的可执行文件输出目录。  
#### - **RUNTIME_OUTPUT_NAME**：指定目标的可执行文件名称。  
#### - **RUNTIME_OUTPUT_NAME_<CONFIG>**：指定特定配置下目标的可执行文件名称。  
#### - **SOURCES**：列出目标的源文件。  
#### - **VERSION**：指定目标的版本。  
#### - **WINDOWS_EXPORT_ALL_SYMBOLS**：在 Windows 平台上导出目标的所有符号。  
## 测试上的属性  

#### - **COST**：指定测试的执行成本。  
#### - **DEPENDS**：定义测试的依赖项。  
#### - **DISABLED**：禁用该测试。  
#### - **ENVIRONMENT**：指定测试环境变量。  
#### - **FAIL_REGULAR_EXPRESSION**：指定若输出匹配该正则表达式则测试失败。  
#### - **FIXTURES_CLEANUP**：指定测试清理的夹具（Fixture）。  
#### - **FIXTURES_REQUIRED**：指定测试所需的夹具（Fixture）。  
#### - **FIXTURES_SETUP**：指定测试的夹具（Fixture）设置。  
#### - **LABELS**：为测试添加标签。  
#### - **MEASUREMENT**：指定测试的测量项。  
#### - **PASS_REGULAR_EXPRESSION**：指定若输出匹配该正则表达式则测试通过。  
#### - **PROCESSORS**：定义测试可使用的处理器数量。  
#### - **REQUIRED_FILES**：指定测试前需存在的文件。  
#### - **RESOURCE_LOCK**：指定测试执行所需的资源锁。  
#### - **RUN_SERIAL**：指示测试应按序执行。  
#### - **TIMEOUT**：指定测试超时时间。  
#### - **WILL_FAIL**：指定测试应当失败。  

---

## 源文件上的属性  

#### - **ABSTRACT**：标记源文件为抽象文件，不会实际编译。  
#### - **AUTOGEN**：指示该文件由 CMake 自动生成。  
#### - **AUTOMOC**：指定该源文件是否应由 `moc` 处理。  
#### - **AUTORCC**：指定该源文件是否应由 `rcc` 处理。  
#### - **AUTOUIC**：指定该源文件是否应由 `uic` 处理。  
#### - **COMPILE_DEFINITIONS**：为该源文件指定编译定义。  
#### - **COMPILE_FEATURES**：为该源文件指定编译特性。  
#### - **COMPILE_FLAGS**：为该源文件指定额外的编译标志。  
#### - **COMPILE_OPTIONS**：为该源文件指定额外的编译选项。  
#### - **DEPENDS**：指定该源文件的依赖项。  
#### - **EXTERNAL_OBJECT**：指示该源文件已由外部工具编译。  
#### - **GENERATED**：标记该源文件为自动生成的文件。  
#### - **HEADER_FILE_ONLY**：指示该源文件仅为头文件，不编译。  
#### - **INCLUDE_DIRECTORIES**：为该源文件指定包含目录。  
#### - **LABELS**：为该源文件指定标签。  
#### - **LANGUAGE**：指定该源文件的编程语言。  
#### - **LOCATION**：指定该源文件的位置。  
#### - **OBJECT_OUTPUTS**：指定该源文件的输出对象。  
#### - **OBJECT_DEPENDS**：指定该源文件的输出依赖项。  
#### - **SOURCE_GROUP**：指定该源文件的分组。  
#### - **SYMBOLIC**：指示该源文件为符号链接。  

---

## 安装文件上的属性  

#### - **COMPONENT**：指定该文件的安装组件。  
#### - **EXCLUDE_FROM_DEFAULT_PACKAGE**：指示该文件不应包含在默认包中。  
#### - **INCLUDE_REGULAR_EXPRESSION**：指定包含在安装中的文件的正则表达式。  
#### - **INSTALL_RPATH**：指定该文件的安装 RPATH。  
#### - **INSTALL_RPATH_USE_LINK_PATH**：指定安装时是否使用链接路径作为 RPATH。  
#### - **MACOSX_BUNDLE_INFO_PLIST**：指定 macOS 捆绑包的 `Info.plist` 文件。  
#### - **MACOSX_FRAMEWORK_INFO_PLIST**：指定 macOS 框架的 `Info.plist` 文件。  
#### - **VERSION**：指定该文件的版本。  

---

## 缓存变量上的属性  

#### - **HELPSTRING**：为缓存变量指定帮助字符串。  
#### - **ADVANCED**：指示该缓存变量仅在高级模式下显示。  
#### - **STRINGS**：为缓存变量指定可能的字符串值。  
#### - **TYPE**：指定缓存变量的类型（如 BOOL、PATH、STRING、FILEPATH 等）。  
#### - **VALUE**：指定缓存变量的默认值。  

---