/**
 * ZeroMQEventServer.h
 * @author: lyxyz5223
 * 事件服务器，基于ZeroMQ实现
 * 提供发布和订阅功能
 */

#pragma once
// C++
#include <thread>

// 日志
#include "../logger/Logger.h"

// third-party libraries
#define ZMQ_STATIC
#include "zmq.hpp"

// self definition

class ZeroMQEventServer
{
private:
	// 日志
	Logger logger{"ZeroMQEventServer"};
	zmq::context_t zmqContext{1};
	zmq::socket_t zmqSocket{zmqContext, zmq::socket_type::xpub};
	// 工作线程
	std::thread workerThread;
	bool shouldStop = false; // 控制线程停止的标志
public:
	~ZeroMQEventServer();
	ZeroMQEventServer();

	void publishEvent(std::string pubId, std::string evtName, std::string evtContent);

private:
	void msgPublishThreadWorker();
	void msgPublishHandler();
};

