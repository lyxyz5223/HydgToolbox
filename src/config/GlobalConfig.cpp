#include "GlobalConfig.h"

// 设定初始值
std::string GlobalConfig::ZeroMQBindUrl = "tcp://*:1037";
std::string GlobalConfig::PluginDirectory = "./plugins";
#if defined(_WIN32)
const std::string GlobalConfig::PluginExtension = ".dll";
#elif defined(__APPLE__)
const std::string GlobalConfig::PluginExtension = ".dylib";
#else
const std::string GlobalConfig::PluginExtension = ".so";
#endif
