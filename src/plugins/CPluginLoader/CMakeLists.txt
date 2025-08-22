cmake_minimum_required(VERSION 3.21)
project(CPluginLoader)

# 使用C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 为MSVC编译器添加/utf-8选项
if(MSVC)
  add_compile_options(/utf-8)
endif()

# 处理头文件
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
# 处理所有文件夹中的头文件
file(GLOB_RECURSE ALL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h")

# 处理源文件
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
# 处理所有文件夹中的源文件
file(GLOB_RECURSE ALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp")

add_library(CPluginLoader SHARED
    ${HEADERS}
    ${SOURCES}
    ${ALL_SOURCES}
    ${ALL_HEADERS}
    ../../logger/Logger.h
    ../../logger/Logger.cpp
    # ../../config/GlobalConfig.h
    ../../config/GlobalConfig.cpp
)
set_target_properties(CPluginLoader PROPERTIES
    OUTPUT_NAME "CPluginLoader"
    PREFIX ""
    SUFFIX ".dll" # Windows下生成dll，Linux/macOS可改为.so
)

# 日志打印
include_directories(../../../libs/spdlog/include)

# 包含日志头文件目录
include_directories(../../logger)

# 包含插件接口头文件目录
include_directories(../../PublicHeaders)

# 包含配置头文件目录
include_directories(../../config)
