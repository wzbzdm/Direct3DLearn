#include "Sphere.h"
#include "Sphere3D.h"
#include "BindableBase.h"

Sphere3D::Sphere3D(Graphics& gfx)
{
    if (!IsStaticInitialized()) {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
            int colorIndex; // 用于索引颜色
        };

		auto model = Sphere::Create<Vertex>(1.0f, numSegments, numRings);

        // 添加颜色索引
        for (int i = 0; i < model.vertices.size(); i++) {
            model.vertices[i].colorIndex = (i / numSegments) % 3;
        }

        // 添加静态绑定：顶点缓冲区
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // 添加着色器
        auto pvs = std::make_unique<VertexShader>(gfx, L"SphereVertex.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"SphereColor.cso"));

        // 添加索引缓冲区
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // 添加颜色常量缓冲区
        struct ConstantBuffer
        {
            struct
            {
                float r, g, b, a;
            } face_colors[3];
        };

        const ConstantBuffer cb =
        {
            {
                { 1.0f, 5.0f, 5.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f, 0.0f }
            }
        };

        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer>>(gfx, cb));

        // 添加输入布局
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "ColorIndex", 0, DXGI_FORMAT_R32_UINT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

void Sphere3D::SetRadius(float r)
{
    radius = r;
}

void Sphere3D::ScaleRadius(float factor)
{
    radius *= factor;
}

DirectX::XMMATRIX Sphere3D::GetTransformMatrix() const noexcept
{
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(radius, radius, radius);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    return scaling * rotationMatrix * translation;
}