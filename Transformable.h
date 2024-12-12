#pragma once

#include <DirectXMath.h>

class Transformable
{
public:
    // 平移操作
    virtual void SetPosition(const DirectX::XMFLOAT3& position) = 0;
    virtual void Translate(const DirectX::XMFLOAT3& offset) = 0;

    // 旋转操作
    virtual void SetRotation(const DirectX::XMFLOAT3& rotation) = 0;
    virtual void Rotate(const DirectX::XMFLOAT3& delta) = 0;

    // 获取当前变换矩阵
    virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;
};
