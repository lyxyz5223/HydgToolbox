cmake_minimum_required(VERSION 3.21)
project(DesktopTimer)

# target_include_directories(DesktopTimer PRIVATE ../../plugin)

# 查找 Qt
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
qt_standard_project_setup()


# Qt 宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 处理头文件
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 处理源文件
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 处理 Qt 文件
# qt6_wrap_ui(UI_HEADERS ${UIS})
# qt6_add_resources(RESOURCE_SOURCES ${RESOURCES})
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
# add_library(DesktopTimer SHARED dllmain.cpp)
add_library(DesktopTimer SHARED
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ../../logger/Logger.h
    ../../logger/Logger.cpp
)
set_target_properties(DesktopTimer PROPERTIES
    OUTPUT_NAME "DesktopTimer"
    PREFIX ""
    SUFFIX ".dll" # Windows下生成dll，Linux/macOS可改为.so
)


# 链接 Qt 库
target_link_libraries(DesktopTimer PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
)

# 日志打印
include_directories(../../../libs/spdlog/include)

