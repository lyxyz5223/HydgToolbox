#include "toolbox/HydgToolbox.h"
#include <QtWidgets/QApplication>
#include "logger/Logger.h"
#include "config/GlobalConfig.h"

int main(int argc, char *argv[])
{
    // 日志打印程序启动
    Logger logger("HydgToolbox");
    logger.setLevel(LogLevel::trace);
    std::string args;
    for (int i = 0; i < argc; ++i) {
        args += fmt::format("\t{}\n", argv[i]);
    }
    logger.info("Hydrogen Toolbox started with {} arguments:\n{}", argc, args);
    logger.info() << "ZeroMQConfig bind url: " << GlobalConfig::ZeroMQBindUrl;
    logger.info() << "Plugins path: " << GlobalConfig::PluginDirectory;
    QApplication a(argc, argv);
    HydgToolbox w;
    w.show();
    auto code = a.exec();
    logger.info("Hydrogen Toolbox exited with code {}", code);
    return code;
}
