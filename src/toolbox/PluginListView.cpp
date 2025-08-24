#include "PluginListView.h"


PluginListView::PluginListView(QWidget* parent)
 : QListView(parent),
   itemDelegate(new PluginCardDelegate(this)),
   model(new QStandardItemModel(this))
{
  setModel(model);
  setItemDelegate(itemDelegate);
  // 禁用编辑
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(this, &QListView::clicked, this, &PluginListView::onItemClicked);
  connect(this, &QListView::doubleClicked, this, &PluginListView::onItemDoubleClicked);
}

PluginListView::~PluginListView()
{
  // delete itemDelegate;
  // delete model;
}


void PluginListView::setPluginInfoList(const std::vector<HydgPluginInfo> &pluginInfoList)
{
  // 清空当前的插件信息
  model->clear();
  // 添加新的插件信息
  for (const auto &info : pluginInfoList)
  {
    QStandardItem* stdItem = new QStandardItem();
    stdItem->setData(QString::fromStdString(info.filePath), Qt::DisplayRole);
    stdItem->setData(QString::fromStdString(info.id), Qt::UserRole);
    stdItem->setData(QString::fromStdString(info.name), Qt::UserRole + 1);
    stdItem->setData(QString::fromStdString(info.version), Qt::UserRole + 2);
    stdItem->setData(QString::fromStdString(info.author), Qt::UserRole + 3);
    stdItem->setData(QString::fromStdString(info.description), Qt::UserRole + 4);
    stdItem->setData(QIcon(), Qt::DecorationRole);
    model->appendRow(stdItem);
  }
}


void PluginListView::onItemClicked(const QModelIndex &index)
{
  // 处理单击事件
  // QString pluginName = index.data(Qt::DisplayRole).toString();
}

void PluginListView::onItemDoubleClicked(const QModelIndex &index)
{
  // 处理双击事件
  QString pluginFilePath = index.data(Qt::DisplayRole).toString();
  emit runPlugin(pluginFilePath);
}


