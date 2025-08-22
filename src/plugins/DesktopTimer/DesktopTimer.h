#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include "../../PublicHeaders/HydgPluginInterface.h"

class DesktopTimer : public QMainWindow
{
  Q_OBJECT
private:
  QLabel *timeLabel;
  QTimer *timer;
  const PluginContext *pluginContext;

public:
  DesktopTimer(QWidget *parent = nullptr, const PluginContext *context = nullptr);
  ~DesktopTimer();
private slots:
  void updateTime();
};

