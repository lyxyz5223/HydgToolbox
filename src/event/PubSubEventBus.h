#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include "ZeroMQEventServer.h"
#include "../logger/Logger.h"
#include "../utils/ThreadSafeUnorderedMap.h"
#include "EventType.h"

class PubSubEventBusInterface {
public:
	/**
	 * 发布消息
	 * @param publisherId string 发布者id
	 * @param eventName string 发布的事件名称
	 * @param eventContent string 发布的事件
	 */
	virtual void publish(std::string publisherId, std::string eventName, std::string eventContent) = 0;

	/**
	 * 订阅消息
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	virtual void subscribe(std::string subscriberId, std::string eventName) = 0;

	/**
	 * 取消订阅
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	virtual void unsubscribe(std::string subscriberId, std::string eventName) = 0;
};

class PubSubEventBus : public PubSubEventBusInterface
{
private:
	// 日志
	Logger logger;
	// ZeroMQ服务
	ZeroMQEventServer *eServer;
	ThreadSafeUnorderedMap<SubscriptionTopic, SubscriberIdList> subscriberMap; // 存储id作为键值的map

public:
	~PubSubEventBus() {}
	PubSubEventBus();

	/**
	 * 发布消息
	 * @param publisherId string 发布者id
	 * @param eventName string 发布的事件名称
	 * @param eventContent string 发布的事件
	 */
	void publish(std::string publisherId, std::string eventName, std::string eventContent) override;

	/**
	 * 订阅消息
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	void subscribe(std::string subscriberId, std::string eventName) override;

	/**
	 * 取消订阅
	 * @param subscriberId string 订阅者id
	 * @param eventName string 订阅的事件名称
	 */
	void unsubscribe(std::string subscriberId, std::string eventName) override;
};

