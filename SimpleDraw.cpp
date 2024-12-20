#include "SimpleDraw.h"
#include "DrawUnitBase.h"

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	window.TestInit();

	// 创建处理 HandlerEvent 的线程
	eventThread = std::thread([this]() {
		while (running) {
			HandlerEvent();
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 防止空转占用 CPU
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
		POINT pt = { meven.GetPosX(), meven.GetPosY() };
		switch (meven.GetType()) {
		case Mouse::Event::Type::LClick:
			window.LClick(pt);
			break;
		case Mouse::Event::Type::LDClick:
			window.LDClick(pt);
			break;
		case Mouse::Event::Type::RClick:
			window.RClick(pt);
			break;
		case Mouse::Event::Type::RDClick:
			window.RDClick(pt);
			break;
		case Mouse::Event::Type::LPMove:
			window.LPMove(pt);
			break;
		case Mouse::Event::Type::RPMove:
			window.RPMove(pt);
			break;
		case Mouse::Event::Type::MPMove:
			window.MPMove(pt);
			break;
		case Mouse::Event::Type::WheelDown:
			window.WheelDown();
			break;
		case Mouse::Event::Type::WheelUp:
			window.WheelUp();
			break;
		default:
			break;
		}
	}
}