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
  //this->unload();
}

bool PluginController::load()
{
  bool result = true;
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
        result = false;
      }
      auto destroyFunc = (void (*)(HydgPlugin *))DLL_SYM(handle, "destroy");
      if (destroyFunc) {
        destroyPluginFunc = destroyFunc;
        logger.trace("Plugin function 'destroy' loaded successfully.");
      } else {
        logger.error("Plugin function 'destroy' not found.");
        result = false;
      }
    }
  }
  return result;
}

void PluginController::unload()
{
  if (handle)
    DLL_CLOSE(handle);
  handle = 0;
}

bool PluginController::create()
{
  if (createPluginFunc) {
    pluginObjs.push_back(createPluginFunc());
    if (pluginInfo.filePath.empty() && pluginObjs.size()) {
      //auto curIdx = pluginObjs.size() - 1;
      auto curObj = pluginObjs.back();
      pluginInfo = {
        pluginPath,
        curObj->getId(),
        curObj->getName(),
        curObj->getVersion(),
        curObj->getAuthor(),
        curObj->getDescription()
      };
      logger.info("Plugin created: id: {}, name: {}, version: {}, author: {}, description: {}", pluginInfo.id, pluginInfo.name, pluginInfo.version, pluginInfo.author, pluginInfo.description);
    }
    else {
      logger.error("Failed to create plugin.");
      return false;
    }
  }
  else {
    logger.error("Create function not set.");
    return false;
  }
  return true;
}

bool PluginController::run()
{
  if (pluginObjs.size()) {
    logger.info("Running plugin: {}", pluginInfo.name);
    // std::thread([this]() {
    // 防止多次运行同一个对象
    if (std::find(runningIdxes.begin(), runningIdxes.end(), pluginObjs.size() - 1) != runningIdxes.end())
      return true; // 如果已经运行，则直接返回成功
    pluginObjs.back()->main(&context);
    runningIdxes.push_back(pluginObjs.size() - 1);
    // }).detach();
    return true;
  }
  else {
    logger.error("Plugin is not created.");
    return false;
  }
}

bool PluginController::shutdown()
{
  if (runningIdxes.size())
  {
    for (auto& idx : runningIdxes)
    {
      bool result = pluginObjs[idx]->shutdown();
      if (!result)
      {
        logger.info("Plugin cancel shutdown.");
        return false; // 如果关闭失败（取消关闭），返回false
      }
      else
      {
        logger.info("Plugin shutdown successfully.");
      }
    }
  }
  return true;
}
bool PluginController::destroy()
{
  if (destroyPluginFunc && pluginObjs.size())
  {
    // 处理Qt事件，防止因提前卸载dll导致Qt事件机制报错
    //QCoreApplication::processEvents(); // 无效
    for (auto& obj : pluginObjs)
      destroyPluginFunc(obj);
    logger.info("Plugin destroyed: {}", pluginInfo.name);
    pluginObjs.clear();
  } else {
    logger.error("Destroy function not set or plugin is not created.");
    return false; // 销毁失败
  }
  return true; // 成功销毁插件
}

HydgPluginInfo PluginController::getInfo() const
{
  return pluginInfo;
}
