#include "Plane.h"
#include "Plane3D.h"
#include "BindableBase.h"

// 平面默认大小
DirectX::XMFLOAT2 Plane3D::defaultSize = { 1.0f, 1.0f };

Plane3D::Plane3D(Graphics& gfx)
{
    if (!IsStaticInitialized()) {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
        };

        auto model = Plane::Create<Vertex>(defaultSize);

        // 添加静态绑定：顶点缓冲区
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // 添加着色器
        auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // 添加颜色常量缓冲区
        struct ConstantBuffer
        {
            struct
            {
                float r, g, b, a;
            } face_colors[2];
        };

        const ConstantBuffer cb =
        {
            {
                { 0.5f, 1.0f, 0.5f, 1.0f },
                { 0.5f, 1.0f, 0.5f, 1.0f }  // 设置平面颜色
            }
        };
        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer>>(gfx, cb));

        // 添加输入布局
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        // 添加拓扑
        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else {
        SetIndexFromStatic();
    }

    // 添加变换常量缓冲区
    AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Plane3D::SetDimensions(const DirectX::XMFLOAT2& size)
{
    this->size = size;  // 设置平面尺寸（长、宽）
}

void Plane3D::ScaleDimensions(const DirectX::XMFLOAT2& factors)
{
    size.x *= factors.x;
    size.y *= factors.y;  // 缩放操作，根据传入的比例因子来调整尺寸
}

DirectX::XMMATRIX Plane3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y 三个方向的缩放）
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(size.x, size.y, 1.0);  // Z轴方向不缩放

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

void Plane3D::InitColor() noexcept {
    SetColors({
		{ 0.5f, 1.0f, 0.5f, 1.0f },
		{ 0.5f, 1.0f, 0.5f, 1.0f }
        });
}