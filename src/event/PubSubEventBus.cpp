#include "PubSubEventBus.h"

PubSubEventBus::PubSubEventBus()
    : logger("PubSubEventBus"),
      eServer(new ZeroMQEventServer())
{
}

void PubSubEventBus::publish(const char *publisherId, const char *eventName, const char *eventContent)
{
  // 拷贝，避免使用原始字符串指针
  std::string pubId = publisherId;
  std::string evtName = eventName;
  std::string evtContent = eventContent;
  // 发布事件的时候进行分发
  std::thread([this, pubId, evtName, evtContent]() {
    logger.trace("Publishing event: publisherId={}, eventName={}, eventContent={}", pubId, evtName, evtContent);
    subscriberMap.lock();
    subscriberCallbacks.lock();
    SubscriberIdList subscribers;
    SubscriberCallbackList callbacks;
    // 查找匹配的订阅者
    if (subscriberMap.count(evtName))
    {
      subscribers = subscriberMap[evtName];
      // 获取订阅者的回调函数
      for (const auto& subId : subscribers)
      {
        if (subscriberCallbacks.count(subId))
          callbacks.push_back(subscriberCallbacks[subId]);
      }
    }
    subscriberCallbacks.unlock();
    subscriberMap.unlock();
    // 通过ZeroMQ进行跨语言分发
    eServer->publishEvent(pubId, evtName, evtContent);
    // 遍历订阅者回调函数列表进行分发
    for (const auto& [callback, userData] : callbacks)
    {
      callback(pubId.c_str(), evtName.c_str(), evtContent.c_str(), userData);
    }

  }).detach();
}

void PubSubEventBus::subscribe(const char* subscriberId, const char* eventName, SubscriberCallback callback, void* userData)
{
  std::string subId = subscriberId;
  std::string evtName = eventName;
  std::thread([this, subId, evtName, callback, userData]() {
    subscriberMap.lock();
    subscriberCallbacks.lock();
    if (!subscriberMap.count(evtName))
      subscriberMap[evtName] = SubscriberIdList{};
    if (std::find(subscriberMap[evtName].begin(), subscriberMap[evtName].end(), subId) == subscriberMap[evtName].end())
    {
      // 如果订阅者不存在，则插入
      subscriberMap[evtName].push_back(subId);
      logger.trace("Subscriber {} subscribed to event {}", subId, evtName);
    }
    else
    {
      logger.trace("Subscriber {} already subscribed to event {}, updating callback", subId, evtName);
    }
    subscriberCallbacks[subId] = { callback, userData };
    subscriberCallbacks.unlock();
    subscriberMap.unlock();
  }).join();
}

void PubSubEventBus::unsubscribe(const char* subscriberId, const char* eventName)
{
  std::string subId = subscriberId;
  std::string evtName = eventName;
  std::thread([this, subId, evtName]() {
    subscriberMap.lock();
    subscriberCallbacks.lock();
    if (!subscriberMap.count(evtName))
    { // map中完全不存在需要取消订阅的事件
      logger.warn("Event {} not found for unsubscription", evtName);
    }
    else if (std::find(subscriberMap[evtName].begin(), subscriberMap[evtName].end(), subId) == subscriberMap[evtName].end())
    { // map中存在需要取消订阅的事件，但是订阅者列表中不存在该订阅者
      logger.warn("Subscriber {} not found in event {} for unsubscription", subId, evtName);
    }
    else
    { // 匹配订阅者和所订阅的事件成功，将该订阅者从列表中删除
      auto &vec = subscriberMap[evtName];
      auto it = std::find(vec.begin(), vec.end(), subId);
      if (it != vec.end())
        vec.erase(it); // 将回调函数从回调列表中删除
      if (subscriberCallbacks.count(subId))
        subscriberCallbacks.erase(subId);
      // 如果删除后订阅者列表为空，则删除该事件
      if (subscriberMap[evtName].empty())
        subscriberMap.erase(evtName);
      logger.trace("Subscriber {} unsubscribed from event {}", subId, evtName);
    }
    subscriberCallbacks.unlock();
    subscriberMap.unlock();
  }).join();
}
