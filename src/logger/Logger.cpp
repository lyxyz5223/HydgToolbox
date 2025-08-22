#include "Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger::Logger(const std::string &name)
{
    logger = spdlog::get(name);
    if (!logger)
    {
        logger = spdlog::stdout_color_mt(name);
    }
}

Logger::~Logger()
{
    spdlog::drop(logger->name());
}

// 流传递无参日志打印
LoggerStream Logger::trace()
{
    return LoggerStream(this, [this](const std::string &msg) { this->trace(msg); });
}

LoggerStream Logger::info()
{
    return LoggerStream(this, [this](const std::string &msg) { this->info(msg); });
}

LoggerStream Logger::warn()
{
    return LoggerStream(this, [this](const std::string &msg) { this->warn(msg); });
}

LoggerStream Logger::error()
{
    return LoggerStream(this, [this](const std::string &msg) { this->error(msg); });
}

LoggerStream Logger::critical()
{
    return LoggerStream(this, [this](const std::string &msg) { this->critical(msg); });
}

// 带参数日志打印
void Logger::trace(const std::string &msg)
{
    if (logger)
    {
        logger->trace(msg);
    }
}

void Logger::info(const std::string &msg)
{
    if (logger)
    {
        logger->info(msg);
    }
}

void Logger::warn(const std::string &msg)
{
    if (logger)
    {
        logger->warn(msg);
    }
}

void Logger::error(const std::string &msg)
{
    if (logger)
    {
        logger->error(msg);
    }
}

void Logger::critical(const std::string &msg)
{
    if (logger)
    {
        logger->critical(msg);
    }
}
