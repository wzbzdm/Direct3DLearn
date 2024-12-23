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

void Env::DeleteCurShape() noexcept {
	if (activeShape < 0 || activeShape > shapes.size()) return;
	shapes.erase(shapes.begin() + activeShape);
	activeShape = -1;
}

void Env::UpdateAll(float dt) {
	for (int i = 0; i < shapes.size(); i++) {
		if (i == activeShape) continue;
		shapes[i]->Update(dt);
	}
}

int Env::GetClickIndex(Ray& ray) noexcept {
	int index = -1;
	DirectX::XMFLOAT3 calc(0,0, FLT_MAX);
	for (int i = 0; i < shapes.size(); i++) {
		DirectX::XMFLOAT3 mid;

		// 测试
		//DirectX::XMFLOAT3 basepos = DirectX::XMFLOAT3(0.0, 0.0, 0.0);
		//DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&basepos);
		//// 设置 w = 1
		//DirectX::XMVECTOR center = DirectX::XMVectorSetW(pos, 1.0f);

		//DirectX::XMMATRIX trans = shapes[i]->GetTransformMatrix(); // 物体的变换矩阵
		//DirectX::XMMATRIX view = Camera().GetViewMatrix();    // 摄像机的视图投影矩阵
		//DirectX::XMMATRIX proj = Camera().GetProjectionMatrix();

		//// 计算完整的变换矩阵：世界矩阵 -> 视图矩阵 -> 投影矩阵
		//center = DirectX::XMVector4Transform(center, trans);
		//center = DirectX::XMVector4Transform(center, view);
		//center = DirectX::XMVector4Transform(center, proj);
		//// 使用齐次坐标变换点的位置信息
		//DirectX::XMVECTOR ndc = DirectX::XMVectorDivide(center, DirectX::XMVectorSplatW(center));

		if (shapes[i]->RayIntersect(ray, mid)) {
			if (mid.z > calc.z) continue;
			calc = mid;
			index = i;
		}
	}

	return index;
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

LightManager* Env::Lights() noexcept {
	return lightManager.get();
}

Camera& Env::Camera() noexcept {
	return cameraManager->GetCurrentCamera();
}

void Env::SetActiveShape(int index) noexcept {
	this->activeShape = index;
}

int Env::GetActiveShapeIndex() noexcept {
	return this->activeShape;
}

bool Env::HasSelect() noexcept {
	return this->activeShape >= 0 && this->activeShape < shapes.size();
}

std::optional<Shape3DBase*> Env::GetSelectedShape() noexcept {
	if (HasSelect()) {
		return this->shapes[activeShape].get();
	}
	return {};
}

// 在 Env.cpp 中定义静态成员
Graphics* Env::gfx = nullptr; // 在运行时指向有效对象
std::unique_ptr<CameraCbuf> Env::cameraCbuf = nullptr;
std::unique_ptr<LightCbuf> Env::lightCbuf = nullptr;
