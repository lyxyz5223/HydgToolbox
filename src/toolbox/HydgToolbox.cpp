#include "HydgToolbox.h"
#include <QCloseEvent>
#include "../config/GlobalConfig.h"
#include "PluginListView.h"

HydgToolbox::HydgToolbox(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  // 加载插件列表
  //PluginManager pluginManager(GlobalConfig::PluginDirectory);
  pluginManager = new PluginManager(GlobalConfig::PluginDirectory);
  pluginManager->scanPlugins();
  // pluginManager->createAndRunPlugins(context);
  pluginManager->loadPlugins(context);
  pluginManager->createPlugins();
  auto pluginList = pluginManager->getPluginInfoList();
  // 设置插件信息列表
  ui.pluginListView->setPluginInfoList(pluginList);
  
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

void HydgToolbox::handleRunPlugin(const QString &pluginFilePath)
{
  if (pluginManager)
  {
    pluginManager->runPlugin(pluginFilePath.toStdString());
  }
}
