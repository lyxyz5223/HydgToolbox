#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "../logger/Logger.h"
#include "PluginController.h"

class PluginManager
{
private:
  // 日志打印器
  Logger logger;
  std::string pluginPath;
  std::vector<std::string> pluginFiles;
  std::unordered_map<std::string, std::unique_ptr<PluginController>> pluginsMap;

  public:
  PluginManager(const std::string &path);
  ~PluginManager();
  void scanPlugins();
  void createAndRunPlugins(const PluginContext& context);
  /**
   * 销毁所有插件
   * @return true表示成功销毁，false表示有插件取消shutdown
   */
  bool destroyPlugins();
  
private:
};

