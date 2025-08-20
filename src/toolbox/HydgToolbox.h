#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HydgToolbox.h"

class HydgToolbox : public QMainWindow
{
    Q_OBJECT

public:
    HydgToolbox(QWidget *parent = nullptr);
    ~HydgToolbox();

private:
    Ui::HydgToolboxClass ui;
};
