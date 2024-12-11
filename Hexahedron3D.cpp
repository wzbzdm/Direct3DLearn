#include "Hexahedron3D.h"
#include "BindableBase.h"

Hexahedron3D::Hexahedron3D(Graphics& gfx)
{
    if (!IsStaticInitialized()) {
        struct Vertex
        {
            DirectX::XMFLOAT3 pos;
        };

        // 根据初始大小生成顶点
        const std::vector<Vertex> vertices =
        {
            { {-size.x, -size.y, -size.z} },
            { { size.x, -size.y, -size.z} },
            { {-size.x,  size.y, -size.z} },
            { { size.x,  size.y, -size.z} },
            { {-size.x, -size.y,  size.z} },
            { { size.x, -size.y,  size.z} },
            { {-size.x,  size.y,  size.z} },
            { { size.x,  size.y,  size.z} },
        };

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        // 添加着色器
        auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        // 添加索引缓冲区
        const std::vector<unsigned short> indices =
        {
            0,2,1, 2,3,1,
            1,3,5, 3,7,5,
            2,6,3, 3,6,7,
            4,5,7, 4,7,6,
            0,4,2, 2,4,6,
            0,1,4, 1,5,4
        };
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        // 添加颜色常量缓冲区
        struct ConstantBuffer2
        {
            struct
            {
                float r, g, b, a;
            } face_colors[6];
        };
        const ConstantBuffer2 cb2 =
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
        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

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


void Hexahedron3D::SetPosition(const DirectX::XMFLOAT3& position) noexcept
{
    pos = position;  // 设置位置
}

void Hexahedron3D::Translate(const DirectX::XMFLOAT3& offset) noexcept
{
    pos.x += offset.x;
    pos.y += offset.y;
    pos.z += offset.z;
}

void Hexahedron3D::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
    this->rotation = rotation;  // 设置旋转（欧拉角）
}

void Hexahedron3D::Rotate(const DirectX::XMFLOAT3& delta) noexcept
{
    rotation.x += delta.x;
    rotation.y += delta.y;
    rotation.z += delta.z;
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
	// 什么也不做
}