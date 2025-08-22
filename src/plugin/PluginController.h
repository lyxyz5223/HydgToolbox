#pragma once

#include <string>
#include <filesystem>
#include <functional>
#include "../PublicHeaders/HydgPluginInterface.h"
#include "../logger/Logger.h"
#ifdef _WIN32
#include <Windows.h>
#define DLL_HANDLE HMODULE
#define DLL_OPEN LoadLibraryA
#define DLL_SYM GetProcAddress
#define DLL_CLOSE FreeLibrary
#else
#include <dlfcn.h>
#define DLL_HANDLE void*
#define DLL_OPEN dlopen
#define DLL_SYM dlsym
#define DLL_CLOSE dlclose
#endif

// 用于C++插件的信息结构体
struct HydgPluginInfo
{
  std::string id;
  std::string name;
  std::string version;
  std::string author;
  std::string description;
  HydgPluginInfo() = default; // 默认构造函数
  // 防止结构体赋值不全
  HydgPluginInfo(const std::string &id, const std::string &name, const std::string &version,
                 const std::string &author, const std::string &description)
      : id(id), name(name), version(version), author(author), description(description) {}
};

// 插件控制器，用于控制一个插件的生命周期，创建与销毁
class PluginController
{
private:
  // 日志
  Logger logger;
  // 插件路径
  std::string pluginPath;
  // 插件信息
  HydgPluginInfo pluginInfo;
  DLL_HANDLE handle = nullptr;

  std::function<HydgPlugin*()> createPluginFunc = nullptr;
  std::function<void(HydgPlugin*)> destroyPluginFunc = nullptr;
  HydgPlugin* pluginObj = nullptr;
  PluginContext context;

public:
  PluginController(const std::string &pluginPathAndName, PluginContext context);
  ~PluginController();
  // 加载插件
  void load();
  // 创建插件
  void create();
  // 运行插件
  void run();
  // 销毁插件
  void destroy();
  // 获取插件信息，create之后才能获取
  HydgPluginInfo getInfo() const;
};
