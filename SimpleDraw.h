#pragma once

#include "MyWindow.h"
#include "MyTimer.h"
#include <thread>
#include <atomic>
#include <mutex>

class SimpleDraw {
public:
	SimpleDraw();
	int Draw();
	~SimpleDraw();
private:
	void HandlerEvent();
private:
	Window window;
	Timer timer;
	std::atomic<bool> running;      // 标记线程运行状态
	std::thread eventThread;        // 处理 HandlerEvent 的线程
	std::mutex eventMutex;          // 防止资源竞争的互斥锁
};