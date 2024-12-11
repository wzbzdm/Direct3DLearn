#include "SimpleDraw.h"
#include "Hexahedron3D.h"

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	boxes.push_back(std::make_unique<Hexahedron3D>(window.Gfx()));
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f));
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
	window.Gfx().ClearBuffer(1.0f, 1.0f, 1.0f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(window.Gfx());
	}
	window.Gfx().EndFrame();
}