#pragma once

#include <QListView>
#include <QStyledItemDelegate>
#include "PluginCardDelegate.h"
#include <QStandardItemModel>
#include "../plugin/HydgPluginInfo.h"

class PluginListView : public QListView
{
  Q_OBJECT
private:
  QStandardItemModel *model;
  PluginCardDelegate *itemDelegate;

public:
  PluginListView(QWidget* parent = nullptr);
  ~PluginListView();

  /**
   * 设置插件信息列表
   * @param pluginInfoList 插件信息列表
   */
  void setPluginInfoList(const std::vector<HydgPluginInfo> &pluginInfoList);
  
signals:
  void runPlugin(const QString &pluginFilePath);

public slots:
  void onItemClicked(const QModelIndex &index);
  void onItemDoubleClicked(const QModelIndex &index);
};

