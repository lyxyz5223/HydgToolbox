#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HydgToolbox.h"
#include "../plugin/PluginManager.h"
#include "../event/PubSubEventBus.h"
#include "../PublicHeaders/HydgPluginInterface.h"

class HydgToolbox : public QMainWindow
{
    Q_OBJECT

public:
    HydgToolbox(QWidget *parent = nullptr);
    ~HydgToolbox();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::HydgToolboxClass ui;
    // 启动插件通信
    PubSubEventBus eventBus;
    PluginContext context{&eventBus};
    PluginManager* pluginManager;
};
