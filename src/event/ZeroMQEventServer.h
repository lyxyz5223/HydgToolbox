#pragma once

// C++
#include <thread>

// 日志
#include "../logger/Logger.h"

// third-party libraries
#define ZMQ_STATIC
#include "zmq.hpp"

// self definition
#include "EventType.h"

class ZeroMQEventServer
{
private:
	// 日志
	Logger logger{"ZeroMQEventServer"};
	zmq::context_t zmqContext{1};
	zmq::socket_t zmqSocket{zmqContext, zmq::socket_type::router};
	// 工作线程
	std::thread workerThread;
	
public:
	~ZeroMQEventServer();
	ZeroMQEventServer();

	void publishEvent(const PublishEvent event);

private:
	void msgPublishThreadWorker();
	void msgPublishHandler();
};

