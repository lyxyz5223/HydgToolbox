#include "PluginManager.h"
#include <filesystem>
#include "../config/GlobalConfig.h"

PluginManager::PluginManager(const std::string &path)
 : logger("PluginManager"), pluginPath(path)
{

}

PluginManager::~PluginManager()
{
  //destroyPlugins();
}

void PluginManager::scanPlugins()
{
  pluginsMap.lock();
  runningPluginsMap.lock();
  // 清空pluginsMap和runningPluginsMap
  pluginsMap.clear();
  runningPluginsMap.clear();

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
  runningPluginsMap.unlock();
  pluginsMap.unlock();
}

void PluginManager::loadPlugins(const PluginContext& context)
{
  pluginsMap.lock();
  // 加载所有插件
  for (const auto &pluginFile : pluginFiles)
  {
    auto pluginController = std::make_unique<PluginController>(pluginFile, context);
    if (pluginController->load())
    {
      logger.info("Plugin {} loaded successfully", pluginFile);
      pluginsMap[pluginFile] = std::move(pluginController);
    }
    else
      logger.error("Failed to load plugin {}", pluginFile);
  }
  pluginsMap.unlock();
}

void PluginManager::createPlugins()
{
  pluginsMap.lock();
  for (const auto &pair : pluginsMap)
  {
    if (pair.second->create())
      logger.info("Plugin {} created successfully", pair.first);
    else
      logger.error("Failed to create plugin {}", pair.first);
  }
  pluginsMap.unlock();
}

void PluginManager::runPlugins()
{
  pluginsMap.lock();
  runningPluginsMap.lock();
  unsigned long long pluginCount = 0;
  unsigned long long successCount = 0;
  // 创建并运行插件
  for (const auto &pair : pluginsMap)
  {
    if (pair.second->run())
    {
      // 运行成功，将该插件添加进运行中的插件列表
      runningPluginsMap[pair.first] = pair.second;
      successCount++;
    }
    pluginCount++;
  }
  logger.info("All {} plugins with {} successfully loaded, created and run", pluginCount, successCount);
  runningPluginsMap.unlock();
  pluginsMap.unlock();
}

void PluginManager::createPlugin(const std::string &pluginFile)
{
  pluginsMap.lock();
  if (pluginsMap.count(pluginFile)) {
    auto &pluginController = pluginsMap[pluginFile];
    if (pluginController->create()) {
      logger.info("Plugin {} successfully created", pluginFile);
    } else {
      logger.error("Failed to create plugin {}", pluginFile);
    }
  }
  pluginsMap.unlock();
}

void PluginManager::runPlugin(const std::string &pluginFile)
{
  pluginsMap.lock();
  runningPluginsMap.lock();
  if (pluginsMap.count(pluginFile)) {
    auto &pluginController = pluginsMap[pluginFile];
    if (pluginController->run()) {
      // 运行成功，将该插件添加进运行中的插件列表
      runningPluginsMap[pluginFile] = pluginController;
      logger.info("Plugin {} successfully created and run", pluginFile);
    } else {
      logger.error("Failed to run plugin {}", pluginFile);
    }
  }
  runningPluginsMap.unlock();
  pluginsMap.unlock();
}



bool PluginManager::shutdownAndDestroyRunningPlugins()
{
  pluginsMap.lock();
  runningPluginsMap.lock();
  // 销毁所有插件
  for (auto &pair : runningPluginsMap)
  {
    bool result = pair.second->shutdown();
    if (!result)
    {
      logger.info("Plugin {} cancel shutdown, toolbox will not be closed.", pair.first);
      runningPluginsMap.unlock();
      pluginsMap.unlock();
      return false; // 如果有插件取消关闭，返回false
    }
    else {
      result = pair.second->destroy();
    }
  }
  runningPluginsMap.clear();
  logger.info("All running plugins have been shut down and destroyed.");
  runningPluginsMap.unlock();
  pluginsMap.unlock();
  return true; // 成功销毁所有插件
}

void PluginManager::destroyAllPlugins()
{
  pluginsMap.lock();
  unsigned long long successCount = 0;
  for (auto& pair : pluginsMap)
  {
    bool result = pair.second->destroy();
    if (result)
      successCount++;
  }
  logger.info("Destroy {} plugins with {} success.", pluginsMap.size(), successCount);
  pluginsMap.unlock();
}

std::vector<HydgPluginInfo> PluginManager::getPluginInfoList()
{
  pluginsMap.lock();
  std::vector<HydgPluginInfo> infoList;
  for (const auto &pair : pluginsMap)
  {
    HydgPluginInfo info = pair.second->getInfo();
    infoList.push_back(info);
  }
  pluginsMap.unlock();
  return infoList;
}
