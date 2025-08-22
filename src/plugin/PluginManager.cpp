#include "PluginManager.h"
#include <filesystem>
#include "../config/GlobalConfig.h"

PluginManager::PluginManager(const std::string &path)
 : logger("PluginManager"), pluginPath(path)
{

}

PluginManager::~PluginManager()
{
  destroyPlugins();
}

void PluginManager::scanPlugins()
{
  // 遍历插件目录，识别插件文件
  if (!std::filesystem::exists(pluginPath))
  {
    std::filesystem::create_directories(pluginPath);
    logger.warn("Plugin directory does not exist, created: {}", pluginPath);
  }
  for (const auto &entry : std::filesystem::directory_iterator(pluginPath))
  {
    if (entry.is_regular_file() && entry.path().extension() == GlobalConfig::PluginExtension)
    {
      pluginFiles.push_back(entry.path().string());
    }
  }
}

void PluginManager::createAndRunPlugins(const PluginContext& context)
{
  unsigned long long pluginCount = 0;
  // 创建并运行插件
  for (const auto &pluginFile : pluginFiles)
  {
    auto pluginController = std::make_unique<PluginController>(pluginFile, context);
    pluginController->load();
    pluginController->create();
    pluginController->run();
    pluginsMap[pluginFile] = std::move(pluginController);
    pluginCount++;
  }
  logger.info("All {} plugins created and running", pluginCount);
}

void PluginManager::destroyPlugins()
{
  // 销毁所有插件
  for (auto &pair : pluginsMap)
  {
    pair.second->destroy();
    pair.second.reset();
  }
  pluginsMap.clear();
  logger.info("All plugins destroyed");
}
