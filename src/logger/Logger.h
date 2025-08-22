#pragma once
#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <functional>
#include <iostream>

// 用于支持 << 流式传递打印日志
// LoggerStream.h
#include <sstream>
#include <string>
#include <functional>
// 日志打印等级
enum class LogLevel: int{
  trace = SPDLOG_LEVEL_TRACE,
  debug = SPDLOG_LEVEL_DEBUG,
  info = SPDLOG_LEVEL_INFO,
  warn = SPDLOG_LEVEL_WARN,
  err = SPDLOG_LEVEL_ERROR,
  critical = SPDLOG_LEVEL_CRITICAL,
  off = SPDLOG_LEVEL_OFF,
  n_levels
};

class LoggerInterface; // 前置声明
class Logger;
// 辅助类：流式日志打印
class LoggerStream
{
public:
    LoggerStream(LoggerInterface *logger, std::function<void(const std::string &)> log_func)
        : logger_(logger), log_func_(log_func) {}

    // 允许移动
    LoggerStream(LoggerStream &&) noexcept = default;
    LoggerStream &operator=(LoggerStream &&) noexcept = default;

    // 禁止拷贝
    LoggerStream(const LoggerStream &) = delete;
    LoggerStream &operator=(const LoggerStream &) = delete;

    template <typename T>
    LoggerStream &operator<<(const T &value)
    {
        ss_ << value;
        return *this;
    }

    ~LoggerStream()
    {
        log_func_(ss_.str());
    }

private:
    LoggerInterface *logger_;
    std::function<void(const std::string &)> log_func_;
    std::stringstream ss_;
};

// 日志打印器接口
class LoggerInterface
{
public:
    virtual ~LoggerInterface() = default;
    virtual void setLevel(LogLevel level) = 0;
    virtual LoggerStream trace() = 0;
    virtual LoggerStream info() = 0;
    virtual LoggerStream warn() = 0;
    virtual LoggerStream error() = 0;
    virtual LoggerStream critical() = 0;
    virtual void trace(const std::string &msg) = 0;
    virtual void info(const std::string &msg) = 0;
    virtual void warn(const std::string &msg) = 0;
    virtual void error(const std::string &msg) = 0;
    virtual void critical(const std::string &msg) = 0;
    // fmt + args
    template <typename... Args>
    void trace(const std::string &fmt, Args &&...args) noexcept {
        logger_helper(&LoggerInterface::trace, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void info(const std::string &fmt, Args &&...args) noexcept {
        logger_helper(&LoggerInterface::info, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void warn(const std::string &fmt, Args &&...args) noexcept {
        logger_helper(&LoggerInterface::warn, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void error(const std::string &fmt, Args &&...args) noexcept {
        logger_helper(&LoggerInterface::error, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void critical(const std::string &fmt, Args &&...args) noexcept {
        logger_helper(&LoggerInterface::critical, fmt, std::forward<Args>(args)...);
    }

private:
    template <typename... Args>
    void logger_helper(void (LoggerInterface::*func)(const std::string &), const std::string &fmt, Args &&...args)
    {
        std::string formatted_msg = fmt::format(fmt, std::forward<Args>(args)...);
        // 处理后交给具体的日志实现
        (this->*func)(formatted_msg);
    }
};

// 日志打印器
class Logger : public LoggerInterface
{
private:
    std::shared_ptr<spdlog::logger> logger;
public:
    Logger(const std::string &name);
    ~Logger();

    void setLevel(LogLevel level) override {
      spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
    }
    // 流式日志打印
    LoggerStream trace() override;
    LoggerStream info() override;
    LoggerStream warn() override;
    LoggerStream error() override;
    LoggerStream critical() override;
    void trace(const std::string &msg) override;
    void info(const std::string &msg) override;
    void warn(const std::string &msg) override;
    void error(const std::string &msg) override;
    void critical(const std::string &msg) override;
    // fmt + args
    template <typename... Args>
    void trace(const std::string &fmt, Args &&...args) noexcept
    {
        logger_helper(&LoggerInterface::trace, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void info(const std::string &fmt, Args &&...args) noexcept
    {
        logger_helper(&LoggerInterface::info, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void warn(const std::string &fmt, Args &&...args) noexcept
    {
        logger_helper(&LoggerInterface::warn, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void error(const std::string &fmt, Args &&...args) noexcept
    {
        logger_helper(&LoggerInterface::error, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void critical(const std::string &fmt, Args &&...args) noexcept
    {
        logger_helper(&LoggerInterface::critical, fmt, std::forward<Args>(args)...);
    }

private:
    template <typename... Args>
    void logger_helper(void (LoggerInterface::*func)(const std::string &), const std::string &fmt, Args &&...args)
    {
        std::string formatted_msg = fmt::format(fmt, std::forward<Args>(args)...);
        // 处理后交给具体的日志实现
        (this->*func)(formatted_msg);
    }
};
