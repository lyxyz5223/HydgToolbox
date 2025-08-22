#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include "ZeroMQEventServer.h"
#include "../logger/Logger.h"
#include "../utils/ThreadSafeUnorderedMap.h"
#include "../PublicHeaders/PubSubEventBusInterface.h"

class PubSubEventBus : public PubSubEventBusInterface
{
public:
  using SubscriptionTopic = std::string;
  using SubscriberId = std::string;
  using SubscriberIdList = std::vector<SubscriberId>;
  using SubscriberCallback = PubSubEventBusInterface::SubscriberCallback;
  struct SubscriberCallbackWithData {
    SubscriberCallback callback;
    void* userData;
  };
  using SubscriberCallbackList = std::vector<SubscriberCallbackWithData>;
private:
	// 日志
	Logger logger;
	// ZeroMQ服务
	ZeroMQEventServer *eServer;
	ThreadSafeUnorderedMap<SubscriptionTopic, SubscriberIdList> subscriberMap; // 存储id作为键值的map
  // 订阅者id-回调函数列表
  ThreadSafeUnorderedMap<SubscriberId, SubscriberCallbackWithData> subscriberCallbacks;

public:
	~PubSubEventBus() {}
	PubSubEventBus();

	/**
	 * 发布消息
	 * @param publisherId string 发布者id
	 * @param eventName string 发布的事件名称
	 * @param eventContent string 发布的事件
	 */
	void publish(const char* publisherId, const char* eventName, const char* eventContent) override;

	/**
	 * 订阅消息
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	void subscribe(const char *subscriberId,
    const char *eventName, SubscriberCallback callback, void* userData) override;

	/**
	 * 取消订阅
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	void unsubscribe(const char *subscriberId, const char *eventName) override;
};

