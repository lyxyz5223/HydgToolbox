#include "dllmain.h"
#include <string>
#include <QtWidgets/QApplication>
#include "DesktopTimer.h"
#include "../../logger/Logger.h"

class DesktopTimerPlugin : public HydgPlugin
{
private:
  DesktopTimer *pluginWindow = nullptr;
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
    pluginWindow = new DesktopTimer(nullptr, pluginContext);
    // QMetaObject::invokeMethod(pluginWindow, "show", Qt::QueuedConnection);
    pluginWindow->show();
  }

  void shutdown() override
  {
    if (pluginWindow) {
      pluginWindow->close();
      // pluginWindow->deleteLater(); // 可能导致报错
      delete pluginWindow; // 立即销毁
      pluginWindow = nullptr;
    }
  }

  const char* getName() const override {
    return "Desktop Timer Plugin";
  }

  const char* getId() const override {
    return "desktop_timer_plugin";
  }

  const char* getVersion() const override {
    return "1.0.0";
  }

  const char* getAuthor() const override {
    return "lyxyz5223";
  }

  const char* getDescription() const override {
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
