#include "PubSubEventBus.h"

PubSubEventBus::PubSubEventBus()
    : logger("PubSubEventBus"),
      eServer(new ZeroMQEventServer())
{
}

void PubSubEventBus::publish(std::string publisherId, std::string eventName, std::string eventContent)
{
    // 发布事件的时候进行分发
    std::thread([this, publisherId, eventName, eventContent]() {
        logger.trace("Publishing event: publisherId={}, eventName={}, eventContent={}",
                    publisherId, eventName, eventContent);
        PublishedEvent event{publisherId, eventName, eventContent};
        subscriberMap.lock();
        SubscriberIdList subscribers;
        // 查找匹配的订阅者
        if (subscriberMap.count(eventName))
            subscribers = subscriberMap[eventName];
        subscriberMap.unlock();
        // 通过ZeroMQ进行跨语言分发
        eServer->publishEvent(event);
        // 遍历订阅者列表进行分发

    }).detach();
}

void PubSubEventBus::subscribe(std::string subscriberId, std::string eventName)
{
    std::thread([this, subscriberId, eventName]() {
        subscriberMap.lock();
        if (!subscriberMap.count(eventName))
            subscriberMap[eventName] = SubscriberIdList{};
        if (subscriberMap[eventName].find(subscriberId) == std::string::npos)
        {
            // 如果订阅者不存在，则插入
            subscriberMap[eventName].push_back(subscriberId);
            logger.trace("Subscriber {} subscribed to event {}", subscriberId, eventName);
        }
        else
        {
            logger.trace("Subscriber {} already subscribed to event {}", subscriberId, eventName);
        }
        subscriberMap.unlock();
    }).join();
}

void PubSubEventBus::unsubscribe(std::string subscriberId, std::string eventName)
{
    std::thread([this, subscriberId, eventName]() {
        subscriberMap.lock();
        if (!subscriberMap.count(eventName))
        { // map中完全不存在需要取消订阅的事件
            logger.warn("Event {} not found for unsubscription", eventName);
        }
        else if (subscriberMap[eventName].find(subscriberId) == std::string::npos)
        { // map中存在需要取消订阅的事件，但是订阅者列表中不存在该订阅者
            logger.warn("Subscriber {} not found in event {} for unsubscription", subscriberId, eventName);
        }
        else
        { // 匹配订阅者和所订阅的事件成功，将该订阅者从列表中删除
            subscriberMap[eventName].erase(subscriberId);
            // 如果删除后订阅者列表为空，则删除该事件
            if (subscriberMap[eventName].empty())
                subscriberMap.erase(eventName);
            logger.trace("Subscriber {} unsubscribed from event {}", subscriberId, eventName);
        }
        subscriberMap.unlock();
    }).join();
}