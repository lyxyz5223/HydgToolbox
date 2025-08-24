#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "../logger/Logger.h"
#include "PluginController.h"
#include "../utils/ThreadSafeUnorderedMap.h"

class PluginManager
{
private:
  // 日志打印器
  Logger logger;
  std::string pluginPath;
  std::vector<std::string> pluginFiles;
  ThreadSafeUnorderedMap<std::string, std::shared_ptr<PluginController>> pluginsMap;
  ThreadSafeUnorderedMap<std::string, std::shared_ptr<PluginController>> runningPluginsMap;

  public:
  PluginManager(const std::string &path);
  ~PluginManager();
  void scanPlugins();

  /**
   * 加载所有插件
   * @param context 插件上下文
   */
  void loadPlugins(const PluginContext& context);

  /**
   * 创建所有插件
   */
  void createPlugins();

  /**
   * 运行所有插件
   */
  void runPlugins();

  /**
   * 通过插件名称创建单个插件
   * @param pluginFile 插件文件
   */
  void createPlugin(const std::string &pluginFile);

  /**
   * 通过插件名称运行单个插件
   * @param pluginFile 插件文件
   */
  void runPlugin(const std::string &pluginFile);

  /**
   * 销毁所有插件
   * @return true表示成功销毁，false表示有插件取消shutdown
   */
  bool destroyPlugins();
  /**
   * 获取插件信息列表
   * @return 插件信息列表的vector
   */
  std::vector<HydgPluginInfo> getPluginInfoList() const;

private:
};

