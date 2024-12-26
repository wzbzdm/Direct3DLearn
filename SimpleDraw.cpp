#include "SimpleDraw.h"
#include "DrawUnitBase.h"

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	window.TestInit();

	// 创建处理 HandlerEvent 的线程
	eventThread = std::thread([this]() {
		while (running) {
			HandlerEvent();
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 防止空转占用 CPU
		}
		});
	running = true;
}

int SimpleDraw::Draw() {
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		// 测试使用
		//HandlerEvent();
	}
}

SimpleDraw::~SimpleDraw() {
	// 停止线程并等待退出
	running = false;
	if (eventThread.joinable()) {
		eventThread.join();
	}
}

void SimpleDraw::HandlerEvent() {
	std::optional<Mouse::Event> mevent = window.ReadMouseEvent();
	if (mevent.has_value()) {
		Mouse::Event meven = mevent.value();
		switch (meven.GetType()) {
		case Mouse::Event::Type::LClick:
			window.LClick(meven);
			break;
		case Mouse::Event::Type::LDClick:
			window.LDClick(meven);
			break;
		case Mouse::Event::Type::RClick:
			window.RClick(meven);
			break;
		case Mouse::Event::Type::RDClick:
			window.RDClick(meven);
			break;
		case Mouse::Event::Type::LPMove:
			window.LPMove(meven);
			break;
		case Mouse::Event::Type::RPMove:
			window.RPMove(meven);
			break;
		case Mouse::Event::Type::MPMove:
			window.MPMove(meven);
			break;
		case Mouse::Event::Type::WheelDown:
			window.WheelDown(meven);
			break;
		case Mouse::Event::Type::WheelUp:
			window.WheelUp(meven);
			break;
		default:
			break;
		}
	}
}