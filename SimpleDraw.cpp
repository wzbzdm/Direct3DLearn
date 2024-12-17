#include "SimpleDraw.h"
#include "DrawUnitBase.h"

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	window.TestInit();
}

int SimpleDraw::Draw() {
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		// 事件处理
		HandlerEvent();
	}
}

SimpleDraw::~SimpleDraw() {

}

void SimpleDraw::HandlerEvent() {
	std::optional<Mouse::Event> mevent = window.ReadMouseEvent();
	if (mevent.has_value()) {
		Mouse::Event meven = mevent.value();
		switch (meven.GetType()) {
		case Mouse::Event::Type::LClick:
			break;
		case Mouse::Event::Type::LDClick:
			break;
		case Mouse::Event::Type::RClick:
			break;
		case Mouse::Event::Type::RDClick:
			break;
		case Mouse::Event::Type::LPMove:
			break;
		case Mouse::Event::Type::RPMove:
			break;
		case Mouse::Event::Type::MPMove:
			break;
		case Mouse::Event::Type::WheelDown:
			break;
		case Mouse::Event::Type::WheelUp:
			break;
		}
	}
}