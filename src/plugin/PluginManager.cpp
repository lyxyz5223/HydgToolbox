#include "PluginManager.h"
#include <filesystem>
#include "../config/GlobalConfig.h"

PluginManager::PluginManager(const std::string &path)
 : logger("PluginManager"), pluginPath(path)
{

}

PluginManager::~PluginManager()
{
  // destroyPlugins();
}

void PluginManager::scanPlugins()
{
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
}

void PluginManager::loadPlugins(const PluginContext& context)
{
  // 注释原因：在scanPlugins中调用
  // 清空pluginsMap和runningPluginsMap
  //pluginsMap.clear();
  //runningPluginsMap.clear();

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
}

void PluginManager::createPlugins()
{
  for (const auto &pair : pluginsMap)
  {
    if (pair.second->create())
      logger.info("Plugin {} created successfully", pair.first);
    else
      logger.error("Failed to create plugin {}", pair.first);
  }
}

void PluginManager::runPlugins()
{
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
}

void PluginManager::createPlugin(const std::string &pluginFile)
{
  if (pluginsMap.count(pluginFile)) {
    auto &pluginController = pluginsMap[pluginFile];
    if (pluginController->create()) {
      logger.info("Plugin {} successfully created", pluginFile);
    } else {
      logger.error("Failed to create plugin {}", pluginFile);
    }
  }
}

void PluginManager::runPlugin(const std::string &pluginFile)
{
  if (pluginsMap.count(pluginFile)) {
    auto &pluginController = pluginsMap[pluginFile];
    if (pluginController->create()) {
      if (pluginController->run()) {
        // 运行成功，将该插件添加进运行中的插件列表
        runningPluginsMap[pluginFile] = pluginController;
        logger.info("Plugin {} successfully created and run", pluginFile);
      } else {
        logger.error("Failed to run plugin {}", pluginFile);
      }
    } else {
      logger.error("Failed to create plugin {}", pluginFile);
    }
  }
}



bool PluginManager::destroyPlugins()
{
  // 销毁所有插件
  for (auto &pair : runningPluginsMap)
  {
    bool result = pair.second->destroy();
    if (!result)
    {
      logger.info("Plugin {} cancel shutdown, toolbox will not be closed.", pair.first);
      return false; // 如果有插件取消关闭，返回false
    }
    pair.second.reset();
  }
  runningPluginsMap.clear();
  pluginsMap.clear();
  logger.info("All plugins destroyed");
  return true; // 成功销毁所有插件
}


std::vector<HydgPluginInfo> PluginManager::getPluginInfoList() const
{
  std::vector<HydgPluginInfo> infoList;
  for (const auto &pair : pluginsMap)
  {
    HydgPluginInfo info = pair.second->getInfo();
    infoList.push_back(info);
  }
  return infoList;
}
