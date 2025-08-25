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
  const PluginContext *&pluginContext;

public:
  // 这里采用指针的引用，保证与dllmain.cpp中的context指针一致
  DesktopTimer(QWidget *parent, const PluginContext *&context);
  ~DesktopTimer();
protected:
  void closeEvent(QCloseEvent *event) override;
private slots:
  void updateTime();
};

