#pragma once

#include <string>
#include <filesystem>
#include <functional>
#include "Logger.h"
// #include "HydgPluginInterface.h"
#include "CPlugin.h"
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

/**
 * 插件控制器，用于控制一个插件的生命周期，创建与销毁
 * @author lyxyz5223
 */
class CPluginController
{
private:
  // 日志
  Logger logger{"CPluginLoader CPluginController"};
  // 插件路径
  std::string pluginPath;
  // 插件信息
  HydgPluginInfo pluginInfo;
  DLL_HANDLE handle = nullptr;
  PluginContext context;
  // 插件实例
  CPlugin *cPlugin = nullptr;
  // 加载函数的辅助函数，增强可复用性
  template <typename Func>
  Func loadFunction(const std::string &funcName);

public:
  CPluginController(const std::string &pluginPathAndName, PluginContext context);
  ~CPluginController();
  // 加载插件
  bool load();
  // 运行插件
  bool run();
  // 销毁插件
  bool destroy();
  // 获取插件信息，create之后才能获取
  HydgPluginInfo getInfo() const;
};
