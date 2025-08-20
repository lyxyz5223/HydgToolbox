#include "ZeroMQEventServer.h"

#include <iostream>
#include "../utils/StringProcess.h"
#include <chrono>
#include "../config/GlobalConfig.h"
#include <json/json.h>

ZeroMQEventServer::~ZeroMQEventServer()
{

}

ZeroMQEventServer::ZeroMQEventServer()
    // : zmqContext(1), zmqSocket(zmqContext, zmq::socket_type::router)
{
    zmqSocket.bind(GlobalConfig::ZeroMQBindUrl);
    // 启动工作线程
    workerThread = std::thread(&ZeroMQEventServer::msgPublishThreadWorker, this);
}

void ZeroMQEventServer::msgPublishThreadWorker()
{
    while (true)
    {
        msgPublishHandler();
        // 休眠一段时间，让其他线程有机会
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
    }
}

void ZeroMQEventServer::msgPublishHandler()
{
    try {
        zmq::message_t identity; // 身份标识（发布者id）
        zmq::message_t msgContent;
        // 接收 DEALER 身份标识（ROUTER 必需）
        std::optional retPart0 = zmqSocket.recv(identity, zmq::recv_flags::dontwait);// 接收信息不要等待，立刻返回
        if (!retPart0)//如果没有接收到消息
            return;
        auto timeStart = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - timeStart < std::chrono::seconds(5)) // 5秒超时
        {
            std::optional retPart1 = zmqSocket.recv(msgContent, zmq::recv_flags::dontwait); // 接收信息不要等待，立刻返回
            if (!retPart1)//如果没有接收到消息
            {
                //std::cout << "[Info][ZeroMQ] No message received now.\n";
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }
            else
            {
                // Received message
                std::string msgContentStr = msgContent.to_string();
                logger.trace("Received message: length: {}, contents: {}", msgContentStr.size(), msgContentStr);
            }
        }
    }
    catch (const zmq::error_t e)
    {
        logger.error("[Error][ZeroMQ] Error occurred: " + std::string(e.what()));
    }

}


void ZeroMQEventServer::publishEvent(const PublishEvent event)
{
    logger.trace("Publishing event: publisherId={}, eventName={}, eventContent={}",
                event.publisherId, event.eventName, event.eventContent);
    
    // 将PublishEvent转换为JSON字符串
    using namespace Json;
    Value jsonEvent;
    jsonEvent["publisherId"] = event.publisherId;
    jsonEvent["eventName"] = event.eventName;
    jsonEvent["eventContent"] = event.eventContent;
    Json::StreamWriterBuilder builder;
    std::string jsonStr = Json::writeString(builder, jsonEvent);
    // 创建zmq消息
    zmq::message_t msg(jsonStr);
    
    // 发送消息前先发送认证标识（发布者id）
    zmqSocket.send(zmq::message_t(event.publisherId), zmq::send_flags::none);
    // 发送消息
    zmqSocket.send(msg, zmq::send_flags::none);
}