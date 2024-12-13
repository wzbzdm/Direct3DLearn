#include "SimpleDraw.h"
#include "DrawUnitBase.h"

constexpr float PI = 3.14159265f;

SimpleDraw::SimpleDraw() : window(800, 600, L"Test") {
	// boxes.push_back(std::make_unique<Cylinder3D>(window.Gfx()));
	// boxes.push_back(std::make_unique<Hexahedron3D>(window.Gfx()));
	// boxes.push_back(std::make_unique<Plane3D>(window.Gfx()));
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{ 2.0f,PI * 2.0f };
	std::uniform_real_distribution<float> ddist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> odist{ 1.0f,PI * 0.5f };
	std::uniform_real_distribution<float> rdist{ 3.0f,6.0f };
	boxes.push_back(std::make_unique<Box>(window.Gfx(), rng, adist, ddist, odist, rdist));
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
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(window.Gfx());
	}
	window.Gfx().EndFrame();
}