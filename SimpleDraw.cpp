#include "SimpleDraw.h"
#include "Hexahedron3D.h"
#include "BoxTest.h"

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	//for (auto i = 0; i < 10; i++) {
	//	boxes.push_back(std::make_unique<Hexahedron3D>(window.Gfx()));
	//}
	//window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	
	boxes.push_back(std::make_unique<Box>(window.Gfx(), rng, adist, ddist, odist, rdist));
	
	window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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
	window.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(window.Gfx());
	}
	window.Gfx().EndFrame();
}