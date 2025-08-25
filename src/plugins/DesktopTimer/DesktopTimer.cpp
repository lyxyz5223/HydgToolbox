#include "DesktopTimer.h"

DesktopTimer::DesktopTimer(QWidget *parent, const PluginContext *&context)
    : QMainWindow(parent), pluginContext(context)
{
  setWindowTitle("Desktop Timer");
  setFixedSize(300, 200);

  timeLabel = new QLabel(this);
  timeLabel->setAlignment(Qt::AlignCenter);
  timeLabel->setGeometry(50, 80, 200, 40);
  timeLabel->setStyleSheet("font-size: 24px;");

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &DesktopTimer::updateTime);
  timer->start(1000); // 每秒刷新

  updateTime(); // 初始化显示
}

DesktopTimer::~DesktopTimer()
{
  // Qt会自动释放子控件
}

void DesktopTimer::closeEvent(QCloseEvent *event)
{
  if (timer && timer->isActive()) {
    timer->stop();
  }
  QMainWindow::closeEvent(event);
}

void DesktopTimer::updateTime()
{
  QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
  timeLabel->setText(currentTime);
  // 可以通过 pluginContext 访问事件总线等功能
  // 测试发布时钟信号
  if (pluginContext)
    pluginContext->eventBus->publish("desktop_timer_event", "update_time", currentTime.toUtf8().constData());
}
