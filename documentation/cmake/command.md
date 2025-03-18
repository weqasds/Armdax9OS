# CMake 命令参考文档

## 1. 基本命令

### 1.1 `cmake_minimum_required`
- **语法**: `cmake_minimum_required(VERSION <min>[...<max>] [FATAL_ERROR])`
- **参数**:
  - `VERSION <min>`: 指定 CMake 的最低版本（必选）。
  - `[...<max>]`: 可选，指定 CMake 的最高版本。
  - `FATAL_ERROR`: 可选，如果版本不满足要求，则报错并停止。
- **示例**:
  ```cmake
  cmake_minimum_required(VERSION 3.10)
  ```

### 1.2 `project`
- **语法**: `project(<PROJECT-NAME> [<language>...])`
- **参数**:
  - `<PROJECT-NAME>`: 项目名称（必选）。
  - `<language>`: 可选，指定项目使用的编程语言（如 `C`、`CXX`）。
- **示例**:
  ```cmake
  project(MyProject C CXX)
  ```

### 1.3 `add_executable`
- **语法**: `add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL] [source1] [source2 ...])`
- **参数**:
  - `<name>`: 可执行文件名称（必选）。
  - `WIN32`: 可选，指定为 Windows 应用程序。
  - `MACOSX_BUNDLE`: 可选，指定为 macOS 应用程序包。
  - `EXCLUDE_FROM_ALL`: 可选，排除从默认构建目标。
  - `source1, source2, ...`: 源文件列表（必选）。
- **示例**:
  ```cmake
  add_executable(MyApp main.cpp utils.cpp)
  ```

### 1.4 `add_library`
- **语法**: `add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] [source1] [source2 ...])`
- **参数**:
  - `<name>`: 库名称（必选）。
  - `STATIC | SHARED | MODULE`: 可选，指定库类型（静态库、动态库或模块）。
  - `EXCLUDE_FROM_ALL`: 可选，排除从默认构建目标。
  - `source1, source2, ...`: 源文件列表（必选）。
- **示例**:
  ```cmake
  add_library(MyLib STATIC lib.cpp)
  ```

---

## 2. 变量与属性

### 2.1 `set`
- **语法**: `set(<variable> <value>... [PARENT_SCOPE])`
- **参数**:
  - `<variable>`: 变量名称（必选）。
  - `<value>`: 变量值（必选）。
  - `PARENT_SCOPE`: 可选，将变量设置到父作用域。
- **示例**:
  ```cmake
  set(MY_VAR "Hello World")
  ```

### 2.2 `unset`
- **语法**: `unset(<variable> [CACHE | PARENT_SCOPE])`
- **参数**:
  - `<variable>`: 变量名称（必选）。
  - `CACHE`: 可选，从缓存中移除变量。
  - `PARENT_SCOPE`: 可选，从父作用域中移除变量。
- **示例**:
  ```cmake
  unset(MY_VAR)
  ```

### 2.3 `configure_file`
- **语法**: `configure_file(<input> <output> [COPYONLY] [ESCAPE_QUOTES] [@ONLY])`
- **参数**:
  - `<input>`: 输入文件路径（必选）。
  - `<output>`: 输出文件路径（必选）。
  - `COPYONLY`: 可选，仅复制文件，不替换变量。
  - `ESCAPE_QUOTES`: 可选，转义引号。
  - `@ONLY`: 可选，仅替换 `@VAR@` 形式的变量。
- **示例**:
  ```cmake
  configure_file(config.h.in config.h)
  ```

---

## 3. 控制流

### 3.1 `if`
- **语法**: `if(<condition>) ... endif()`
- **参数**:
  - `<condition>`: 条件表达式（必选）。
- **示例**:
  ```cmake
  if(WIN32)
    message("Running on Windows")
  endif()
  ```

### 3.2 `foreach`
- **语法**: `foreach(<loop_var> <items>) ... endforeach()`
- **参数**:
  - `<loop_var>`: 循环变量（必选）。
  - `<items>`: 迭代项列表（必选）。
- **示例**:
  ```cmake
  foreach(item IN ITEMS a b c)
    message("Item: ${item}")
  endforeach()
  ```

---

## 4. 文件与目录

### 4.1 `file`
- **语法**: `file(<command> [<args>...])`
- **常用命令**:
  - `READ`: 读取文件内容。
  - `WRITE`: 写入文件内容。
  - `GLOB`: 匹配文件列表。
- **示例**:
  ```cmake
  file(GLOB SOURCES "src/*.cpp")
  ```

### 4.2 `add_subdirectory`
- **语法**: `add_subdirectory(<source_dir> [<binary_dir>] [EXCLUDE_FROM_ALL])`
- **参数**:
  - `<source_dir>`: 子目录路径（必选）。
  - `<binary_dir>`: 可选，指定构建目录。
  - `EXCLUDE_FROM_ALL`: 可选，排除从默认构建目标。
- **示例**:
  ```cmake
  add_subdirectory(src)
  ```

---

## 5. 链接与编译

### 5.1 `target_link_libraries`
- **语法**: `target_link_libraries(<target> <PRIVATE|PUBLIC|INTERFACE> <item>...)`
- **参数**:
  - `<target>`: 目标名称（必选）。
  - `<PRIVATE|PUBLIC|INTERFACE>`: 指定链接范围（必选）。
  - `<item>`: 链接的库或目标（必选）。
- **示例**:
  ```cmake
  target_link_libraries(MyApp PRIVATE MyLib)
  ```

### 5.2 `target_include_directories`
- **语法**: `target_include_directories(<target> <INTERFACE|PUBLIC|PRIVATE> <items>...)`
- **参数**:
  - `<target>`: 目标名称（必选）。
  - `<INTERFACE|PUBLIC|PRIVATE>`: 指定包含范围（必选）。
  - `<items>`: 包含目录路径（必选）。
- **示例**:
  ```cmake
  target_include_directories(MyApp PUBLIC include)
  ```

---

## 6. 安装与测试

### 6.1 `install`
- **语法**: `install(TARGETS <targets>... [DESTINATION <dir>])`
- **参数**:
  - `<targets>`: 目标名称（必选）。
  - `DESTINATION <dir>`: 安装目录（必选）。
- **示例**:
  ```cmake
  install(TARGETS MyApp DESTINATION bin)
  ```

### 6.2 `add_test`
- **语法**: `add_test(NAME <name> COMMAND <command> [<arg>...])`
- **参数**:
  - `NAME <name>`: 测试名称（必选）。
  - `COMMAND <command>`: 测试命令（必选）。
  - `<arg>`: 命令参数（可选）。
- **示例**:
  ```cmake
  add_test(NAME MyTest COMMAND MyApp --test)
  ```

---

以上是 CMake 常用命令的参考文档，更多详细内容可参考 [CMake 官方文档](https://cmake.com.cn/cmake/help/latest/manual/cmake-commands.7.html)。