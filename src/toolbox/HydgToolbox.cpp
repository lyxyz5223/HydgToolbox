#include "HydgToolbox.h"
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
    pluginManager->destroyPlugins(); // 通知所有插件关闭和销毁
  }
  QMainWindow::closeEvent(event);
}
