#pragma once

#include "MyCamera.h"
#include "MyLightManager.h"

class CameraManager {
public:
    CameraManager() : defaultCamera() {
        AddAndActiveCamera(defaultCamera);
    }

    void Resize(int width, int height) noexcept {
        for (auto& camera : cameras) {
            camera.Resize(width, height);
        }
    }

    // 添加一个相机
    void AddCamera(const Camera& camera) {
        cameras.push_back(camera);
    }

    void AddAndActiveCamera(const Camera& camera) {
        currentCameraIndex = cameras.size();
        cameras.push_back(camera);
    }

    // 获取当前相机（通常为主相机）
    Camera& GetCurrentCamera() {
        if (currentCameraIndex >= 0 && currentCameraIndex < cameras.size()) {
            return cameras[currentCameraIndex];
        }
        return defaultCamera;
    }

    // 设置当前使用的相机
    void SwitchCamera(unsigned int index) {
        if (index < cameras.size())
            currentCameraIndex = index;
    }

    // 获取视图矩阵
    DirectX::XMMATRIX GetViewMatrix() const {
        return cameras.at(currentCameraIndex).GetViewMatrix();
    }

    // 获取投影矩阵
    DirectX::XMMATRIX GetProjectionMatrix() const {
        return cameras.at(currentCameraIndex).GetProjectionMatrix();
    }

    // 获取合并后的摄像机矩阵（视图矩阵 * 投影矩阵）
    DirectX::XMMATRIX GetCameraMatrix() const {
        return cameras.at(currentCameraIndex).GetCameraMatrix();
    }

    // 更新灯光
    void UpdateLightsWithCamera(LightManager& lightManager) const {
        // 获取当前相机的视图矩阵
        DirectX::XMMATRIX viewMatrix = GetViewMatrix();
        // 更新灯光位置和方向
        lightManager.TransformLightsWithViewMatrix(viewMatrix);
    }

    void RemoveCamera(unsigned int index) {
        if (index < cameras.size()) {
            cameras.erase(cameras.begin() + index);
            if (currentCameraIndex >= cameras.size()) {
                currentCameraIndex = cameras.size() - 1; // 防止越界
            }
        }
    }

private:
    Camera defaultCamera;
    std::vector<Camera> cameras;  // 管理的所有相机
    unsigned int currentCameraIndex = 0; // 当前激活的相机
};
