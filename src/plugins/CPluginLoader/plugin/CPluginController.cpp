#include "CPluginController.h"
#include <thread>

CPluginController::CPluginController(const std::string &pluginPathAndName, PluginContext context)
    : pluginPath(pluginPathAndName),
      context(context),
      cPlugin(new CPlugin())
{
  //logger.setLevel(LogLevel::trace);
}

CPluginController::~CPluginController()
{
  delete cPlugin;// 关闭C插件实例
  if (handle)
      DLL_CLOSE(handle);
}

template <typename Func>
Func CPluginController::loadFunction(const std::string &funcName)
{
  if (!handle)
  {
    logger.error("C plugin not loaded");
    return nullptr;
  }
  void* func = DLL_SYM(handle, funcName.c_str());
  if (!func)
  {
    logger.error("Failed to load c plugin function: {}", funcName);
  }
  return static_cast<Func>(func);
}

bool CPluginController::load()
{
  // 加载插件逻辑
  if (std::filesystem::exists(pluginPath))
  {
    // 读取插件文件（dll或so等）
    handle = DLL_OPEN(pluginPath.c_str());
    if (handle)
    {
      CPlugin::MainFunc mainFunc = loadFunction<decltype(mainFunc)>("main");
      cPlugin->setMainFunc(mainFunc);
      CPlugin::ShutdownFunc shutdownFunc = loadFunction<decltype(shutdownFunc)>("shutdown");
      cPlugin->setShutdownFunc(shutdownFunc);
      // 获取 获取插件信息 的函数指针
      CPlugin::GetInfoFunc getIdFunc = loadFunction<decltype(getIdFunc)>("getId");
      cPlugin->setGetIdFunc(getIdFunc);
      CPlugin::GetInfoFunc getNameFunc = loadFunction<decltype(getNameFunc)>("getName");
      cPlugin->setGetNameFunc(getNameFunc);
      CPlugin::GetInfoFunc getVersionFunc = loadFunction<decltype(getVersionFunc)>("getVersion");
      cPlugin->setGetVersionFunc(getVersionFunc);
      CPlugin::GetInfoFunc getAuthorFunc = loadFunction<decltype(getAuthorFunc)>("getAuthor");
      cPlugin->setGetAuthorFunc(getAuthorFunc);
      CPlugin::GetInfoFunc getDescriptionFunc = loadFunction<decltype(getDescriptionFunc)>("getDescription");
      cPlugin->setGetDescriptionFunc(getDescriptionFunc);
      // 获取插件信息
      pluginInfo = {
        cPlugin->getId(),
        cPlugin->getName(),
        cPlugin->getVersion(),
        cPlugin->getAuthor(),
        cPlugin->getDescription()
      };
      return true;
    }
  }
  return false;
}

bool CPluginController::run()
{
  logger.info("Running c plugin: {}", pluginInfo.name);
  // std::thread([this]() {
    cPlugin->main(&context);
  // }).detach();
  return true;
}

bool CPluginController::destroy()
{
  bool result = cPlugin->shutdown();
  if (!result)
  {
    logger.info("C plugin {} cancel shutdown", pluginInfo.name);
    return false; // 取消关闭插件
  }
  logger.info("C plugin destroyed: {}", pluginInfo.name);
  return true; // 正常关闭插件
}

HydgPluginInfo CPluginController::getInfo() const
{
  return pluginInfo;
}
