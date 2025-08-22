#pragma once
/**
 * 发布/订阅事件总线接口
 * @author lyxyz5223
 */
class PubSubEventBusInterface
{
public:
  typedef void (*SubscriberCallback)(const char *publisherId, const char *eventName, const char *eventContent, void* userData);
  /**
   * 发布消息
   * @param publisherId string 发布者id
   * @param eventName string 发布的事件名称
   * @param eventContent string 发布的事件
   */
  virtual void publish(const char* publisherId, const char* eventName, const char* eventContent) = 0;

  /**
   * 订阅消息
   * @param subscriberId string 订阅者id
   * @param eventName string 订阅的事件名称
   */
  virtual void subscribe(const char *subscriberId,
                         const char *eventName, SubscriberCallback callback, void* userData) = 0;

  /**
   * 取消订阅
   * @param subscriberId string 订阅者id
   * @param eventName string 订阅的事件名称
   */
  virtual void unsubscribe(const char *subscriberId, const char *eventName) = 0;
};
