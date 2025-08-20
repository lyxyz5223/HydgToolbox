#pragma once

#include <string>
#include <functional>
#include <vector>

struct PublishedEvent
{
    std::string publisherId;  // 发布者id
    std::string eventName;    // 事件名称
    std::string eventContent; // 事件内容
};



// struct Subscriber
// {
//     std::string subscriberId;
//     SubscribeType type = SubscribeType::BOTH;
//     std::string publisherId;
//     std::string eventName;
//     Subscriber(const std::string& subscriberId, SubscribeType type, const std::string& publisherId, const std::string& eventName)
//         : subscriberId(subscriberId), type(type), publisherId(publisherId), eventName(eventName) {};
//     Subscriber(const Subscriber &e)
//     {
//         this->subscriberId = e.subscriberId;
//         this->type = e.type;
//         this->publisherId = e.publisherId;
//         this->eventName = e.eventName;
//     }
//     bool operator==(const Subscriber &event)
//     {
//         if (subscriberId != event.subscriberId)
//             return false;
//         if (type != event.type)
//             return false;
//         switch (type)
//         {
//         case SubscribeType::PUBLISHER_ID:
//             if (publisherId != event.publisherId)
//                 return false;
//             break;
//         case SubscribeType::EVENT_NAME:
//             if (eventName != event.eventName)
//                 return false;
//             break;
//         case SubscribeType::BOTH: // 如果前面两项不返回false，那么一定都相等了，直接返回true
//             break;
//         default:
//             break;
//         }
//         return true;
//     }
//     static size_t hash(const Subscriber &event) noexcept
//     {
//         // 使用 std::hash 对 SubscribeType 和字符串进行哈希
//         // size_t h1 = std::hash<SubscribeType>{}(event.type);
//         // size_t h2 = std::hash<std::string>{}(event.subscriberId);
//         // size_t h3 = std::hash<std::string>{}(event.eventName);
//         // return h1 ^ (h2 << 1) ^ (h3 << 2);
//         return std::hash<std::string>{}(
//             event.subscriberId + std::to_string(static_cast<int>(event.type)) + event.publisherId + event.eventName);
//     }
//     size_t hash() const noexcept
//     {
//         return hash(*this);
//     }
// };
// namespace std
// {
//     template <>
//     struct hash<Subscriber>
//     {
//         size_t operator()(const Subscriber &event) const
//         {
//             return Subscriber::hash(event);
//         }
//     };
// }

// 订阅的事件
using SubscriptionTopic = std::string;
using SubscriberId = std::string;
using SubscriberIdList = std::vector<SubscriberId>;