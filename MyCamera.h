#pragma once

#include <DirectXMath.h>
#include <vector>

struct CameraData {
    DirectX::XMFLOAT3 position;      // 相机的位置
    DirectX::XMFLOAT3 target;        // 相机朝向目标的点
    DirectX::XMFLOAT3 upVector;      // 用于计算视图矩阵的向上向量
    float fieldOfView;               // 视野角度（FOV）
    float nearPlane;                 // 近裁剪面
    float farPlane;                  // 远裁剪面
    float aspectRatio;               // 视口纵横比
};

typedef struct {
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
    DirectX::XMFLOAT3 position;
    float pad;
} CameraBuffer;

class Camera {
public:
    Camera(const CameraData& data)
        : data(data)
    {
        UpdateMatrices();
    }

    // 更新视图矩阵和投影矩阵
    void UpdateMatrices() {
        using namespace DirectX;
        view = XMMatrixLookAtLH(XMLoadFloat3(&data.position), XMLoadFloat3(&data.target), XMLoadFloat3(&data.upVector));
        projection = XMMatrixPerspectiveFovLH(data.fieldOfView, data.aspectRatio, data.nearPlane, data.farPlane);
    }

    // 获取视图矩阵
    DirectX::XMMATRIX GetViewMatrix() const {
        return view;
    }

    // 获取投影矩阵
    DirectX::XMMATRIX GetProjectionMatrix() const {
        return projection;
    }

    // 获取完整的相机矩阵（视图 + 投影）
    DirectX::XMMATRIX GetCameraMatrix() const {
        return projection * view;  // 先应用视图矩阵，再应用投影矩阵
    }

    // 更新相机位置、朝向等参数
    void SetPosition(const DirectX::XMFLOAT3& newPos) {
        data.position = newPos;
        UpdateMatrices();
    }

    // 返回相机的配置数据
    const CameraData& GetCameraData() const {
        return data;
    }

    CameraBuffer GetCameraBufferData() {
		CameraBuffer buffer;
		buffer.view = view;
		buffer.projection = projection;
		buffer.position = data.position;
		return buffer;
    }

private:
    CameraData data;              // 保存相机的相关数据
    DirectX::XMMATRIX view;       // 视图矩阵
    DirectX::XMMATRIX projection; // 投影矩阵
};
