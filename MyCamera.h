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
    Camera() {
        // 初始化CameraData并设置默认参数
        data = {
            DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),   // 默认位置 (在Z轴负方向上)
            DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),    // 默认目标 (指向原点)
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),    // 默认上向量 (指向Y轴正方向)
            114.0f / 360 * DirectX::XM_PI,                                 
            0.1f,                                   // 默认近裁剪面距离 
            100.0f,                                  // 默认远裁剪面距离
            4.0 / 3.0                               // 默认纵横比 (4:3)
        };
        UpdateMatrices();
    }

    Camera(const CameraData& data)
        : data(data)
    {
        UpdateMatrices();
    }

    void Resize(int width, int height) noexcept {
		data.aspectRatio = static_cast<float>(width) / height;
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
        return XMMatrixMultiply(view, projection);  // 先应用视图矩阵，再应用投影矩阵
    }

    // 更新相机位置，并重新计算矩阵
    void SetPosition(const DirectX::XMFLOAT3& newPos) {
        if (data.target.x == newPos.x && data.target.y == newPos.y && data.target.z == newPos.z) return;
        data.position = newPos;
        UpdateMatrices();
    }

    // 更新相机的目标（朝向）
    void SetTarget(const DirectX::XMFLOAT3& newTarget) {
        // 防止数据相同
        if (data.position.x == newTarget.x && data.position.y == newTarget.y && data.position.z == newTarget.z) return;
        data.target = newTarget;
        UpdateMatrices();
    }

    void SetUpV(const DirectX::XMFLOAT3& newUpV) {
        data.upVector = newUpV;
        UpdateMatrices();
    }

    // 根据法向量设置相机的朝向
    void SetOrientationFromNormal(const DirectX::XMFLOAT3& normal) {
        using namespace DirectX;

        // 确保法向量是单位向量
        XMVECTOR normalizedNormal = XMVector3Normalize(XMLoadFloat3(&normal));

        // 更新目标
        XMVECTOR targetPos = XMLoadFloat3(&data.position) + normalizedNormal;

        // 将目标转换为XMFLOAT3
        XMStoreFloat3(&data.target, targetPos);

        // 更新矩阵
        UpdateMatrices();
    }

    // 返回相机的配置数据
    const CameraData& GetCameraData() const {
        return data;
    }

    // 转换为可以传递给GPU的相机数据
    CameraBuffer GetCameraBufferData() const {
        CameraBuffer buffer;
        buffer.view = DirectX::XMMatrixTranspose(view);
        buffer.projection = DirectX::XMMatrixTranspose(projection);
        buffer.position = data.position;
        buffer.pad = 0.0f;  // 保持4字节对齐
        return buffer;
    }

    // 设置视野角度（FOV）
    void SetFieldOfView(float fov) {
        data.fieldOfView = fov;
        UpdateMatrices();
    }

    void SetFarPlane(float far) {
        data.farPlane = far;
        UpdateMatrices();
    }

    // 设置纵横比
    void SetAspectRatio(float aspectRatio) {
        data.aspectRatio = aspectRatio;
        UpdateMatrices();
    }

    // 相机的旋转，控制俯仰角、偏航角和滚转角
    void AdjustRotation(float pitch, float yaw, float roll = 0.0f) {
        using namespace DirectX;
        XMVECTOR direction = XMLoadFloat3(&data.target) - XMLoadFloat3(&data.position); // 当前朝向
        XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);             // 旋转矩阵
        direction = XMVector3TransformNormal(direction, rotation);                      // 旋转方向
        XMStoreFloat3(&data.target, XMLoadFloat3(&data.position) + direction);          // 更新目标
        UpdateMatrices();
    }

    // 相机围绕目标点进行绕行旋转
    void Orbit(float deltaYaw, float deltaPitch) {
        using namespace DirectX;
        XMVECTOR targetPos = XMLoadFloat3(&data.target);
        XMVECTOR cameraPos = XMLoadFloat3(&data.position);

        XMVECTOR offset = cameraPos - targetPos;
        XMMATRIX yawRotation = XMMatrixRotationY(deltaYaw);
        XMMATRIX pitchRotation = XMMatrixRotationAxis(XMLoadFloat3(&data.upVector), deltaPitch);
        offset = XMVector3Transform(offset, yawRotation * pitchRotation);

        cameraPos = targetPos + offset;
        XMStoreFloat3(&data.position, cameraPos);
        UpdateMatrices();
    }

private:
    CameraData data;              // 保存相机的相关数据
    DirectX::XMMATRIX view;       // 视图矩阵
    DirectX::XMMATRIX projection; // 投影矩阵
};
