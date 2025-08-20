cmake_minimum_required(VERSION 3.21)
project(HydgToolbox VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
qt_standard_project_setup()


# Qt 宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 包含头文件目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    libs/jsoncpp-1.9.6/include
    libs/ZeroMQ/include
)

# 源文件列表
set(SOURCES
    src/logger/Logger.cpp
    src/config/GlobalConfig.cpp
    src/utils/StringProcess.cpp
    src/event/ZeroMQEventServer.cpp
    src/event/PubSubEventBus.cpp
    src/plugin/PluginManager.cpp
    src/toolbox/HydgToolbox.cpp
    src/main.cpp
)

# 头文件
set(HEADERS
    src/logger/Logger.h
    src/config/GlobalConfig.h
    src/utils/StringProcess.h
    src/utils/ThreadSafeUnorderedMap.h
    src/event/ZeroMQEventServer.h
    src/event/PubSubEventBus.h
    src/plugin/PluginManager.h
    src/plugin/HydgPluginInterface.h
    src/toolbox/HydgToolbox.h
)

# Qt 资源和 UI 文件
# set(RESOURCES HydgToolbox.qrc)
# set(UIS HydgToolbox.ui)

# 处理 Qt 文件
# qt6_wrap_ui(UI_HEADERS ${UIS})
# qt6_add_resources(RESOURCE_SOURCES ${RESOURCES})
file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")
add_executable(HydgToolbox
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)


# 链接 Qt 库
# target_link_libraries(HydgToolbox PRIVATE Qt6::Core Qt6::Gui Qt6::Quick)
# 链接 ZeroMQ 和 jsoncpp（假设你用的是静态库或 DLL）
target_link_libraries(HydgToolbox PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
    iphlpapi
    ws2_32
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/ZeroMQ/lib/Debug/libzmq-v143-mt-sgd-4_3_5.lib
)

# 如果 jsoncpp 是源码编译，可以直接添加 src 目录
include_directories(libs/jsoncpp-1.9.6/include)
# add_subdirectory(libs/jsoncpp-1.9.6)
# target_link_libraries(HydgToolbox PRIVATE jsoncpp_lib)

# spdlog
# set(spdlog_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/spdlog)
# find_package(spdlog REQUIRED)
# target_link_libraries(HydgToolbox PRIVATE spdlog::spdlog)
include_directories(libs/spdlog/include)

# 资源文件（如图标）
# 可用 install 命令安装到输出目录
install(FILES src/HydgToolbox.ico DESTINATION .)