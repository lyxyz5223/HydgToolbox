#include "CPluginManager.h"
#include <filesystem>
#include "GlobalConfig.h"

CPluginManager::CPluginManager(const std::string &path)
 : pluginPath(path)
{

}

CPluginManager::~CPluginManager()
{
  destroyPlugins();
}

void CPluginManager::scanPlugins()
{
  // 遍历插件目录，识别插件文件
  if (!std::filesystem::exists(pluginPath))
  {
    std::filesystem::create_directories(pluginPath);
    logger.warn("C plugin directory does not exist, created: {}", pluginPath);
  }
  for (const auto &entry : std::filesystem::directory_iterator(pluginPath))
  {
    if (entry.is_regular_file() && entry.path().extension() == GlobalConfig::PluginExtension)
    {
      pluginFiles.push_back(entry.path().string());
    }
  }
}

void CPluginManager::createAndRunPlugins(const PluginContext& context)
{
  unsigned long long pluginCount = 0;
  unsigned long long successCount = 0;
  // 创建并运行插件
  for (const auto &pluginFile : pluginFiles)
  {
    auto cPluginController = std::make_unique<CPluginController>(pluginFile, context);
    if (cPluginController->load())
      if (cPluginController->run())
        successCount++;
    pluginsMap[pluginFile] = std::move(cPluginController);
    pluginCount++;
  }
  logger.info("All {} c plugins with {} successfully loaded and running", pluginCount, successCount);
}

bool CPluginManager::destroyPlugins()
{
  // 销毁所有插件
  for (auto &pair : pluginsMap)
  {
    bool result = pair.second->destroy();
    if (!result)
    {
      logger.info("C plugin {} cancel shutdown", pair.first);
      return false;
    }
    pair.second.reset();
  }
  pluginsMap.clear();
  logger.info("All c plugins destroyed");
  return true;
}
