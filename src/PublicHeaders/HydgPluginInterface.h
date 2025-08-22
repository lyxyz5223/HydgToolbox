#pragma once
/**
dll插件需要导出一个create函数
如：
extern "C" __declspec(dllexport) void* create()
{
    return new MyPlugin();
}
同时需要导出一个destroy函数
如：
extern "C" __declspec(dllexport) void destroy(void* plugin)
{
    delete static_cast<HydgPlugin*>(plugin);
}
*/
#include "PubSubEventBusInterface.h"

struct PluginContext {
  PubSubEventBusInterface *eventBus = nullptr;
};

/**
 * Hydrogen 插件接口
 * @author lyxyz5223
 */
class HydgPlugin {
public:
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
     * 获取插件作者
     * @return 插件作者
     */
    virtual const char* getAuthor() const = 0;

    /**
     * 获取插件描述
     * @return 插件描述
     */
    virtual const char* getDescription() const = 0;

    /**
     * 插件主入口函数，提供上下文菜单，当插件销毁的时候上下文将自动销毁
     */
    virtual void main(const PluginContext *context) = 0;

    /**
     * 插件关闭函数，将会在插件的destroy之前调用
     */
    virtual void shutdown() = 0;

    /**
     * 插件接收事件函数
     */
    // virtual void receiveEvent() = 0;
};

