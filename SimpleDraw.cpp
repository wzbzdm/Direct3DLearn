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
		Update();
	}
}

SimpleDraw::~SimpleDraw() {

}

void SimpleDraw::Update() {
	auto dt = timer.Mark();
	window.Gfx().ClearBuffer(1.0f, 0.5f, 0.5f);
	window.RefreshGlobal();
	for (auto& b : window.ActiveEnv()->GetShapes())
	{
		b->Update( window.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		b->Draw(window.Gfx());
	}
	window.Gfx().EndFrame();
}