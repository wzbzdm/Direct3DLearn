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
	
}