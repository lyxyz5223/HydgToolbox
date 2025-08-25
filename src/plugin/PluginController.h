#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include "../PublicHeaders/HydgPluginInterface.h"
#include "../logger/Logger.h"
#include "HydgPluginInfo.h"
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



/**
 * 插件控制器，用于控制一个插件的生命周期，创建与销毁
 * @author lyxyz5223
 */
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
  std::vector<HydgPlugin*> pluginObjs; // 允许多次创建，存在多个插件实例
  std::vector<unsigned long long> runningIdxes;
  PluginContext context;

public:
  PluginController(const std::string &pluginPathAndName, PluginContext context);
  ~PluginController();
  // 加载插件
  bool load();
  // 卸载插件
  void unload();
  // 创建插件实例
  bool create();
  // 运行插件
  bool run();
  // 关闭插件
  bool shutdown();
  // 销毁插件实例
  bool destroy();
  // 获取插件信息，create之后才能获取
  HydgPluginInfo getInfo() const;
};
