#pragma once

#include <DirectXMath.h>

class Transformable
{
public:
    // 平移操作
    void SetPosition(const DirectX::XMFLOAT3& position);
    void Translate(const DirectX::XMFLOAT3& offset);

    // 旋转操作
    void SetRotation(const DirectX::XMFLOAT3& rotation);
    void Rotate(const DirectX::XMFLOAT3& delta);

    void Update(float dt) noexcept;

    // 获取当前变换矩阵
    virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;
protected:
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 5.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
};
