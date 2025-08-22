#include "PluginController.h"
#include <thread>
#include <qcoreapplication.h>

PluginController::PluginController(const std::string &pluginPathAndName, PluginContext context)
    : logger("PluginController"),
      pluginPath(pluginPathAndName),
      context(context)
{
  //logger.setLevel(LogLevel::trace);
}

PluginController::~PluginController()
{
  //this->destroy();
  if (handle)
    DLL_CLOSE(handle);
}

void PluginController::load()
{
  // 加载插件逻辑
  if (std::filesystem::exists(pluginPath))
  {
    // 读取插件文件（dll或so等）
    handle = DLL_OPEN(pluginPath.c_str());
    if (handle)
    {
      auto createFunc = (HydgPlugin * (*)()) DLL_SYM(handle, "create");
      if (createFunc) {
        createPluginFunc = createFunc;
        logger.trace("Plugin function 'create' loaded successfully.");
      } else {
        logger.error("Plugin function 'create' not found.");
      }
      auto destroyFunc = (void (*)(HydgPlugin *))DLL_SYM(handle, "destroy");
      if (destroyFunc) {
        destroyPluginFunc = destroyFunc;
        logger.trace("Plugin function 'destroy' loaded successfully.");
      } else {
        logger.error("Plugin function 'destroy' not found.");
      }
    }
  }
}


void PluginController::create()
{
  if (createPluginFunc) {
    pluginObj = createPluginFunc();
    if (pluginObj) {
      pluginInfo = {
        pluginObj->getId(),
        pluginObj->getName(),
        pluginObj->getVersion(),
        pluginObj->getAuthor(),
        pluginObj->getDescription()
      };
      logger.info("Plugin created: id: {}, name: {}, version: {}, author: {}, description: {}", pluginInfo.id, pluginInfo.name, pluginInfo.version, pluginInfo.author, pluginInfo.description);
    }
    else {
      logger.error("Failed to create plugin.");
    }
  }
  else {
    logger.error("Create function not set.");
  }
}

void PluginController::run()
{
  if (pluginObj) {
    logger.info("Running plugin: {}", pluginInfo.name);
    // std::thread([this]() {
      pluginObj->main(&context);
    // }).detach();
  }
  else {
    logger.error("Plugin is not created.");
  }
}

void PluginController::destroy()
{
  if (destroyPluginFunc && pluginObj) {
    pluginObj->shutdown();
    // 处理Qt事件，防止因提前卸载dll导致Qt事件机制报错
    QCoreApplication::processEvents(); // 无效
    destroyPluginFunc(pluginObj);
    logger.info("Plugin destroyed: {}", pluginInfo.name);
    pluginObj = nullptr;
  } else {
    logger.error("Destroy function not set or plugin is not created.");
  }
}

HydgPluginInfo PluginController::getInfo() const
{
  return pluginInfo;
}
