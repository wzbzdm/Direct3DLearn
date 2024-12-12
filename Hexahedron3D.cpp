#include "Hexahedron.h"
#include "Hexahedron3D.h"
#include "BindableBase.h"

DirectX::XMFLOAT3 Hexahedron3D::defaultSize = { 1.0f, 0.8f, 0.7f };

Hexahedron3D::Hexahedron3D(Graphics& gfx)
{
    if (!IsStaticInitialized()) {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
        };

		auto model = Hexahedron().Create<Vertex>();

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
            } face_colors[6];
        };

        const ConstantBuffer cb =
        {
            {
                { 1.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f, 0.0f, 1.0f, 1.0f },
                { 1.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f, 1.0f },
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

void Hexahedron3D::SetSize(const DirectX::XMFLOAT3& size)
{
    this->size = size;  // 设置大小（长宽高）
}

void Hexahedron3D::Zoom(const DirectX::XMFLOAT3& scale)
{
    size.x *= scale.x;
    size.y *= scale.y;
    size.z *= scale.z;  // 缩放操作，根据传入的比例因子来调整尺寸
}

DirectX::XMMATRIX Hexahedron3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y, Z 三个方向的缩放）
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(size.x, size.y, size.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}

void Hexahedron3D::Update(float dt) noexcept
{
    // 自定义 Update
	rotation.x += 2.0f * dt;
    rotation.y += 1.6f * dt;
    rotation.z += 1.2f * dt;
}