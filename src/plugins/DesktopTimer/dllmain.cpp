#include "dllmain.h"
#include <string>
#include <vector>
#include <QtWidgets/QApplication>
#include "DesktopTimer.h"
#include "../../logger/Logger.h"

/** 
 * 此插件演示Qt窗口的正确创建方式
 * 整个主程序生命周期内只会存在一个相同插件的实例
 * 但是可能会多次调用main函数，
 * 所以千万不要在这个插件类中多次对同一对象多次赋值
 */
class DesktopTimerPlugin : public HydgPlugin
{
private:
  std::vector<DesktopTimer*> pluginWindows;
  Logger logger{"DesktopTimerPlugin"};
  const PluginContext *pluginContext;

public:
  DesktopTimerPlugin() {}
  ~DesktopTimerPlugin() {}

  void main(const PluginContext* context) override
  {
    this->pluginContext = context;
    // 测试事件总线
    context->eventBus->subscribe("desktop_timer_event", "update_time", [](const char* publisherId, const char* eventName, const char* eventContent, void* userData) {
      auto* plugin = static_cast<DesktopTimerPlugin*>(userData);
      if (plugin) {
        // 处理事件
        plugin->logger.info("Received event: publisherId={}, eventName={}, eventContent={}", publisherId, eventName, eventContent);
        plugin->logger.info("Updating timer display...");
      }
    }, this);
    // 插件主入口函数逻辑
    // 创建计时器主窗口
    auto* pluginWindow = new DesktopTimer(nullptr, pluginContext);
    pluginWindows.push_back(pluginWindow);
    pluginWindow->setParent(nullptr);
    // QMetaObject::invokeMethod(pluginWindow, "show", Qt::QueuedConnection);
    pluginWindow->show();
  }

  bool shutdown() override
  {
    for (auto& pluginWindow : pluginWindows)
      if (pluginWindow) {
        pluginWindow->close();
        // pluginWindow->deleteLater(); // 可能导致报错
        delete pluginWindow; // 立即销毁
        pluginWindow = nullptr;
      }
    return true;
  }

  const char* getName() override {
    return "Desktop Timer Plugin";
  }

  const char* getId() override {
    return "desktop_timer_plugin";
  }

  const char* getVersion() override {
    return "1.0.0";
  }

  const char* getAuthor() override {
    return "lyxyz5223";
  }

  const char* getDescription() override {
    return "A plugin to show desktop timer.";
  }
};


HydgPlugin* create()
{
  return new DesktopTimerPlugin();
}

void destroy(HydgPlugin* plugin)
{
  delete plugin;
}
