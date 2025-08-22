#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "Logger.h"
#include "CPluginController.h"

class CPluginManager
{
private:
  // 日志打印器
  Logger logger{"CPluginLoader CPluginManager"};
  std::string pluginPath;
  std::vector<std::string> pluginFiles;
  std::unordered_map<std::string, std::unique_ptr<CPluginController>> pluginsMap;

  public:
  CPluginManager(const std::string &path);
  ~CPluginManager();
  void scanPlugins();
  void createAndRunPlugins(const PluginContext& context);
  bool destroyPlugins();
  
private:
};

