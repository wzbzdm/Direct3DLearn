#include "Drawable.h"
#include "Transformable.h"

class Cylinder3D : public Drawable, public Transformable
{
public:
    Cylinder3D() = default;

    void SetPosition(const DirectX::XMFLOAT3& position) noexcept override;
    void Translate(const DirectX::XMFLOAT3& offset) noexcept override;
    void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept override;
    void Rotate(const DirectX::XMFLOAT3& delta) noexcept override;
    void SetDimensions(float baseRadius, float height);
    void ScaleDimensions(const DirectX::XMFLOAT2& factors); // 单独缩放半径和高度
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
    DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    float baseRadius = 1.0f;
    float height = 1.0f;
};
