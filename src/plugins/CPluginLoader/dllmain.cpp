#include "dllmain.h"
#include <string>
#include "Logger.h"
#include "plugin/CPluginManager.h"
#include "GlobalConfig.h"

class CPluginLoader : public HydgPlugin
{
private:
  Logger logger{"CPluginLoader"};
  const PluginContext *pluginContext = nullptr;
  // C插件管理器
  CPluginManager *pluginManager = nullptr;

public:
  CPluginLoader() {}
  ~CPluginLoader() {}

  void main(const PluginContext* context) override
  {
    this->pluginContext = context;
    // 插件主入口函数逻辑
    logger.info("C Plugin Loader started with context: {}", context->eventBus ? "EventBus available" : "No EventBus");
    // 创建CPluginManager实例
    std::string pluginDir = "CPlugins";
    if (GlobalConfig::PluginDirectory.size() && (GlobalConfig::PluginDirectory.back() != '/' && GlobalConfig::PluginDirectory.back() != '\\'))
      pluginDir = GlobalConfig::PluginDirectory + "/" + pluginDir;
    else
      pluginDir = GlobalConfig::PluginDirectory + pluginDir;
    pluginManager = new CPluginManager(pluginDir);
    pluginManager->scanPlugins(); // 扫描所有C插件
    pluginManager->createAndRunPlugins(*pluginContext); // 创建并运行所有C插件
  }

  bool shutdown() override
  {
    // 插件关闭逻辑
    if (pluginManager)
      return pluginManager->destroyPlugins(); // 销毁所有C插件
    return true;
    // if (!result)
    //   return false; // 取消关闭
    // return true; // 正常关闭
  }

  const char* getName() override {
    return "C Plugin Loader";
  }

  const char* getId() override {
    return "c_plugin_loader";
  }

  const char* getVersion() override {
    return "1.0.0";
  }

  const char* getAuthor() override {
    return "lyxyz5223";
  }

  const char* getDescription() override {
    return "A plugin to load C plugins.";
  }
};


HydgPlugin* create()
{
  return new CPluginLoader();
}

void destroy(HydgPlugin* plugin)
{
  delete plugin;
}
