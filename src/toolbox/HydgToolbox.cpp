#include "HydgToolbox.h"
#include <QCloseEvent>
#include "../config/GlobalConfig.h"

HydgToolbox::HydgToolbox(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 加载插件列表
    //PluginManager pluginManager(GlobalConfig::PluginDirectory);
    pluginManager = new PluginManager(GlobalConfig::PluginDirectory);
    pluginManager->scanPlugins();
    pluginManager->createAndRunPlugins(context);
}

HydgToolbox::~HydgToolbox()
{
  delete pluginManager;
}

void HydgToolbox::closeEvent(QCloseEvent *event)
{
  if (pluginManager)
  {
    bool result = pluginManager->destroyPlugins(); // 通知所有插件关闭和销毁
    if (!result) {
      event->ignore(); // 如果有插件取消关闭，忽略关闭事件
    }
  }
  QMainWindow::closeEvent(event);
}
