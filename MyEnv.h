#pragma once

#include "MyCameraManager.h"
#include "MyLightManager.h"
#include "CameraCbuf.h"
#include "LightCbuf.h"
#include "Shape3DBase.h"
#include <optional>

class Env {
public:
	Env();
	void RefreshBind();
	static void Initialize(Graphics* gfx); // 静态初始化函数
	void AddShape(std::unique_ptr<Shape3DBase> shape) noexcept;
	void DeleteCurShape() noexcept;
	void DrawAll();
	void UpdateAll(float dt);
	int GetClickIndex(Ray& ray) noexcept;
	std::vector<std::unique_ptr<Shape3DBase>>& GetShapes() noexcept;
	LightManager* Lights() noexcept;
	Camera& Camera() noexcept;
	void SetActiveShape(int index) noexcept;
	int GetActiveShapeIndex() noexcept;
	bool HasSelect() noexcept;
	std::optional<Shape3DBase*> GetSelectedShape() noexcept;

public:
	std::unique_ptr<LightManager> lightManager;
	std::unique_ptr<CameraManager> cameraManager;
private:
	int activeShape = -1;
	static Graphics* gfx;
	static std::unique_ptr<CameraCbuf> cameraCbuf;
	static std::unique_ptr<LightCbuf> lightCbuf;

	// 管理环境中的全局变量
	std::vector<std::unique_ptr<Shape3DBase>> shapes;
};