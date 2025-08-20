#pragma once

// Hydrogen 插件接口
class HydgPlugin {
public:
    virtual ~HydgPlugin() = default;

    /**
     * 获取插件id(唯一)
     * @return 插件id
     */
    virtual const char* getId() const = 0;

    /**
     * 获取插件名称
     * @return 插件名称
     */
    virtual const char* getName() const = 0;

    /**
     * 获取插件版本
     * @return 插件版本
     */
    virtual const char* getVersion() const = 0;

    /**
     * 获取插件描述
     * @return 插件描述
     */
    virtual const char* getDescription() const = 0;

    /**
     * 插件主入口函数
     */
    virtual void main() = 0;

    /**
     * 插件关闭函数
     */
    virtual void shutdown() = 0;

    /**
     * 插件接收事件函数
     */
    virtual void receiveEvent() = 0;
};