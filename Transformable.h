#pragma once

#include <DirectXMath.h>
#include "Ray.h"

class Transformable
{
public:
    // 平移操作
    void SetPosition(const DirectX::XMFLOAT3& position);
    const DirectX::XMFLOAT3& GetPosition() const noexcept;
    void Translate(const DirectX::XMFLOAT3& offset);

    // 旋转操作
    void SetRotation(const DirectX::XMFLOAT3& rotation);
    const DirectX::XMFLOAT3& GetRotation() const noexcept;
    void Rotate(const DirectX::XMFLOAT3& delta);

    // 缩放
    float GetRadius()const noexcept;
    void SetRadius(float r);
    void Zoom(float r);

    virtual void Update(float dt) noexcept;

    // 获取当前变换矩阵
    virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;
    virtual bool RayIntersect(const Ray& ray, DirectX::XMFLOAT3& intersectionPoint) const noexcept = 0;
protected:
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    float radius = 1.0f;
};
