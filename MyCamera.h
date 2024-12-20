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
private:
    static int baseHeight;
    static float baseFOV;
public:
    Camera();
    Camera(const CameraData& data);

    static void Init(int baseheight, float baseFOv);
    void Resize(int width, int height) noexcept;
    float GetCurZ(int height) noexcept;   // 获取当前投影屏幕的Z值
    // 更新视图矩阵和投影矩阵
    void UpdateMatrices();
    // 获取视图矩阵
    DirectX::XMMATRIX GetViewMatrix() const;
    // 获取投影矩阵
    DirectX::XMMATRIX GetProjectionMatrix() const;
    // 获取完整的相机矩阵（视图 + 投影）
    DirectX::XMMATRIX GetCameraMatrix() const;
    // 更新相机位置，并重新计算矩阵
    void SetPosition(const DirectX::XMFLOAT3& newPos);
    // 更新相机的目标（朝向）
    void SetTarget(const DirectX::XMFLOAT3& newTarget);
    void SetUpV(const DirectX::XMFLOAT3& newUpV);
    // 根据法向量设置相机的朝向
    void SetOrientationFromNormal(const DirectX::XMFLOAT3& normal);
    // 返回相机的配置数据
    const CameraData& GetCameraData() const;
    // 转换为可以传递给GPU的相机数据
    CameraBuffer GetCameraBufferData() const;
    // 设置视野角度（FOV）
    void SetFieldOfView(float fov);
    void SetFarPlane(float far);
    // 设置纵横比
    void SetAspectRatio(float aspectRatio);
    // 相机的旋转，控制俯仰角、偏航角和滚转角
    void AdjustRotation(float pitch, float yaw, float roll = 0.0f);
    // 相机围绕目标点进行绕行旋转
    void Orbit(float deltaYaw, float deltaPitch);

private:
    CameraData data;              // 保存相机的相关数据
    DirectX::XMMATRIX view;       // 视图矩阵
    DirectX::XMMATRIX projection; // 投影矩阵
};
