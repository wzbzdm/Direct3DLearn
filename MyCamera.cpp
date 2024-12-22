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

DirectX::XMFLOAT3 Camera::GetCurDir(float x, float y) noexcept {
    // 假定输入是 NDC 范围，直接用裁剪坐标
    DirectX::XMVECTOR clipCoords = DirectX::XMVectorSet(x, y, 1.0f, 1.0f);

    // 使用缓存的投影矩阵逆矩阵
    DirectX::XMVECTOR viewCoords = DirectX::XMVector4Transform(clipCoords, inverseProjection);

    // 使用缓存的视图矩阵逆矩阵
    DirectX::XMVECTOR worldCoords = DirectX::XMVector3TransformCoord(viewCoords, inverseView);

    // 计算射线方向
    DirectX::XMVECTOR cameraPosition = DirectX::XMLoadFloat3(&data.position);
    DirectX::XMVECTOR rayDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(worldCoords, cameraPosition));

    // 返回方向向量
    DirectX::XMFLOAT3 direction;
    DirectX::XMStoreFloat3(&direction, rayDirection);

    return direction;
}


DirectX::XMFLOAT3 Camera::GetPos() const noexcept {
    return data.position;
}

void Camera::UpdateMatrices() {
    using namespace DirectX;
    view = XMMatrixLookAtLH(XMLoadFloat3(&data.position), XMLoadFloat3(&data.target), XMLoadFloat3(&data.upVector));
    inverseView = DirectX::XMMatrixInverse(nullptr, view);
    projection = XMMatrixPerspectiveFovLH(data.fieldOfView, data.aspectRatio, data.nearPlane, data.farPlane);
    inverseProjection = DirectX::XMMatrixInverse(nullptr, projection);
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

// 前向矢量
DirectX::XMFLOAT3 Camera::Normalize(const DirectX::XMFLOAT3& vec) {
    DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&vec);
    v = DirectX::XMVector3Normalize(v);
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, v);
    return result;
}

// 右向矢量
DirectX::XMFLOAT3 Camera::CrossProduct(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) {
    DirectX::XMVECTOR va = DirectX::XMLoadFloat3(&a);
    DirectX::XMVECTOR vb = DirectX::XMLoadFloat3(&b);
    DirectX::XMVECTOR cross = DirectX::XMVector3Cross(va, vb);
    DirectX::XMFLOAT3 result;
    DirectX::XMStoreFloat3(&result, cross);
    return result;
}

void Camera::GetCameraAxes(DirectX::XMFLOAT3& forward, DirectX::XMFLOAT3& right, DirectX::XMFLOAT3& up) {
    forward = Normalize(DirectX::XMFLOAT3(
        data.target.x - data.position.x,
        data.target.y - data.position.y,
        data.target.z - data.position.z
    ));
    right = Normalize(CrossProduct(forward, data.upVector));
    up = Normalize(CrossProduct(right, forward));
}

// 上下旋转（俯仰）：pitch
// 左右旋转（偏航）：yaw
// 绕朝向旋转（滚转）：roll
void Camera::AdjustOrientation(float pitch, float yaw, float roll) {
    DirectX::XMFLOAT3 forward, right, up;
    GetCameraAxes(forward, right, up);  // 获取当前相机的方向轴

    // 构造旋转矩阵
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&right), pitch) *   // 俯仰
        DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&up), yaw) *       // 偏航
        DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&forward), roll);  // 滚转

    // 更新方向向量
    DirectX::XMVECTOR newForward = DirectX::XMVector3Transform(
        DirectX::XMLoadFloat3(&forward),
        rotation
    );

    DirectX::XMVECTOR newUp = DirectX::XMVector3Transform(
        DirectX::XMLoadFloat3(&up),
        rotation
    );

    // 将更新的方向存入 forward 和 up 向量
    DirectX::XMStoreFloat3(&forward, newForward);
    DirectX::XMStoreFloat3(&up, newUp);

    // 更新相机目标点
    data.target = {
        data.position.x + forward.x,
        data.position.y + forward.y,
        data.position.z + forward.z
    };

    // 更新相机上向量
    data.upVector = up;

    // 更新视图矩阵
    UpdateMatrices();
}


void Camera::Move(float forwardDelta, float rightDelta, float upDelta) {
    DirectX::XMFLOAT3 forward, right, up;
    GetCameraAxes(forward, right, up);

    // 根据局部轴计算位移
    data.position.x += forward.x * forwardDelta + right.x * rightDelta + up.x * upDelta;
    data.position.y += forward.y * forwardDelta + right.y * rightDelta + up.y * upDelta;
    data.position.z += forward.z * forwardDelta + right.z * rightDelta + up.z * upDelta;

    // 更新目标点（保证朝向不变）
    data.target.x += forward.x * forwardDelta + right.x * rightDelta + up.x * upDelta;
    data.target.y += forward.y * forwardDelta + right.y * rightDelta + up.y * upDelta;
    data.target.z += forward.z * forwardDelta + right.z * rightDelta + up.z * upDelta;

    UpdateMatrices(); // 更新视图矩阵
}
