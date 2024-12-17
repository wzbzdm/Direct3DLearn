#include "MyEnv.h"

Env::Env()
    : lightManager(std::make_unique<LightManager>()),
    cameraManager(std::make_unique<CameraManager>()) {
}

void Env::RefreshBind() {
	// 绑定到 插槽0
	Env::cameraCbuf->Bind(*gfx, 0, 1);
	// 绑定到 插槽1
	Env::lightCbuf->Bind(*gfx, 1, 1);
}

void Env::Initialize(Graphics* gfx) {
	if (Env::gfx == nullptr) {
		Env::gfx = gfx;
		Env::cameraCbuf = std::make_unique<CameraCbuf>(*gfx);
		Env::lightCbuf = std::make_unique<LightCbuf>(*gfx);
	}
}

void Env::AddShape(std::unique_ptr<Shape3DBase> shape) noexcept {
	shapes.push_back(std::move(shape));
}

void Env::UpdateAll(float dt) {
	for (auto& b : GetShapes())
	{
		b->Update(dt);
	}
}

void Env::DrawAll() {
	// 刷新全局绑定
	RefreshBind();
	for (auto& s : shapes) {
		s->Draw(*gfx);
	}
}

std::vector<std::unique_ptr<Shape3DBase>>& Env::GetShapes() noexcept {
	return shapes;
}

std::unique_ptr<LightManager>& Env::Lights() noexcept {
	return lightManager;
}

Camera& Env::Camera() noexcept {
	return cameraManager->GetCurrentCamera();
}

// 在 Env.cpp 中定义静态成员
Graphics* Env::gfx = nullptr; // 在运行时指向有效对象
std::unique_ptr<CameraCbuf> Env::cameraCbuf = nullptr;
std::unique_ptr<LightCbuf> Env::lightCbuf = nullptr;
