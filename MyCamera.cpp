#include "MyCamera.h"

int Camera::baseHeight = 600;
float Camera::baseFOV = 114.0f / 360 * DirectX::XM_PI;

void Camera::Init(int baseheight, float baseFOv) {
    Camera::baseHeight = baseheight;
    Camera::baseFOV = baseFOv / 360 * DirectX::XM_PI;
}

Camera::Camera() {
    // 初始化CameraData并设置默认参数
    data = {
        DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),   // 默认位置 (在Z轴负方向上)
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),    // 默认目标 (指向原点)
        DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),    // 默认上向量 (指向Y轴正方向)
        Camera::baseFOV,                        // FOV
        0.1f,                                   // 默认近裁剪面距离 
        100.0f,                                  // 默认远裁剪面距离
        4.0 / 3.0                               // 默认纵横比 (4:3)
    };
    UpdateMatrices();
}

Camera::Camera(const CameraData& data) : data(data) {
    UpdateMatrices();
}

void Camera::Resize(int width, int height) noexcept {
    // 根据上一次的数据，修改FOV，使投影矩阵不前后移动
    // height / tan(FOV) = newheight / tan(newFOV)
    data.fieldOfView = 2 * atanf(height * tanf(Camera::baseFOV / 2) / Camera::baseHeight);
    data.aspectRatio = static_cast<float>(width) / height;

    UpdateMatrices();
}

// 根据 fieldOfView 和 height 计算 Z
float Camera::GetCurZ(int height) noexcept {
    // height / tan(FOV/2)
    return static_cast<float>(height) / tanf(data.fieldOfView / 2);
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept {
    return data.position;
}

void Camera::UpdateMatrices() {
    using namespace DirectX;
    view = XMMatrixLookAtLH(XMLoadFloat3(&data.position), XMLoadFloat3(&data.target), XMLoadFloat3(&data.upVector));
    projection = XMMatrixPerspectiveFovLH(data.fieldOfView, data.aspectRatio, data.nearPlane, data.farPlane);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const {
    return view;
}

// 获取投影矩阵
DirectX::XMMATRIX Camera::GetProjectionMatrix() const {
    return projection;
}

// 获取完整的相机矩阵（视图 + 投影）
DirectX::XMMATRIX Camera::GetCameraMatrix() const {
    return XMMatrixMultiply(view, projection);  // 先应用视图矩阵，再应用投影矩阵
}

// 更新相机位置，并重新计算矩阵
void Camera::SetPosition(const DirectX::XMFLOAT3& newPos) {
    if (data.target.x == newPos.x && data.target.y == newPos.y && data.target.z == newPos.z) return;
    data.position = newPos;
    UpdateMatrices();
}

// 更新相机的目标（朝向）
void Camera::SetTarget(const DirectX::XMFLOAT3& newTarget) {
    // 防止数据相同
    if (data.position.x == newTarget.x && data.position.y == newTarget.y && data.position.z == newTarget.z) return;
    data.target = newTarget;
    UpdateMatrices();
}

void Camera::SetUpV(const DirectX::XMFLOAT3& newUpV) {
    data.upVector = newUpV;
    UpdateMatrices();
}

// 根据法向量设置相机的朝向
void Camera::SetOrientationFromNormal(const DirectX::XMFLOAT3& normal) {
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
const CameraData& Camera::GetCameraData() const {
    return data;
}

// 转换为可以传递给GPU的相机数据
CameraBuffer Camera::GetCameraBufferData() const {
    CameraBuffer buffer;
    buffer.view = DirectX::XMMatrixTranspose(view);
    buffer.projection = DirectX::XMMatrixTranspose(projection);
    buffer.position = data.position;
    buffer.pad = 0.0f;  // 保持4字节对齐
    return buffer;
}

// 设置视野角度（FOV）
void Camera::SetFieldOfView(float fov) {
    data.fieldOfView = fov;
    UpdateMatrices();
}

void Camera::SetFarPlane(float far) {
    data.farPlane = far;
    UpdateMatrices();
}

// 设置纵横比
void Camera::SetAspectRatio(float aspectRatio) {
    data.aspectRatio = aspectRatio;
    UpdateMatrices();
}

// 相机的旋转，控制俯仰角、偏航角和滚转角
void Camera::AdjustRotation(float pitch, float yaw, float roll) {
    using namespace DirectX;
    XMVECTOR direction = XMLoadFloat3(&data.target) - XMLoadFloat3(&data.position); // 当前朝向
    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);             // 旋转矩阵
    direction = XMVector3TransformNormal(direction, rotation);                      // 旋转方向
    XMStoreFloat3(&data.target, XMLoadFloat3(&data.position) + direction);          // 更新目标
    UpdateMatrices();
}

// 相机围绕目标点进行绕行旋转
void Camera::Orbit(float deltaYaw, float deltaPitch) {
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