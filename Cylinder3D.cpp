#include "Cylinder.h"
#include "Cylinder3D.h"
#include "BindableBase.h"

DirectX::XMFLOAT3 Cylinder3D::conf = { 1.0f, 1.0f, 2.0 };

Cylinder3D::Cylinder3D(Graphics& gfx)
{
    if (!IsStaticInitialized()) {
        struct Vertex
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } color;
            DirectX::XMFLOAT3 pos;
        };

		auto model = Cylinder::Create<Vertex>(1, 1, 2, 36, 18);
		model.vertices[36 * 19].color = { 0.0, 1.0, 0, 1.0 };  // 顶部
		model.vertices[36 * 19 + 1].color = { 1.0, 0.0, 0, 1.0 };  // 底部
        // 填充颜色
        for (int i = 0; i < model.vertices.size(); i++) {
            if (i >= 0 && i < 36) {
				model.vertices[i].color = { 0, 1.0, 0, 1.0 };  // 顶部
			}
			else if (i >= 36 && i < 36 * 18) {
				model.vertices[i].color = { 0, 0, 1.0, 1.0 };  // 侧面
			}
            else if (i >= 36 * 18 && i < 36 * 19) {
                model.vertices[i].color = { 1.0, 0, 0, 1.0 };  // 底部
            }
		}

        // 添加静态绑定：顶点缓冲区
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

        // 添加着色器
        auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShaderColor.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));
        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShaderColor.cso"));

        // 添加索引缓冲区
        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

        // 添加颜色常量缓冲区
        struct ConstantBuffer
        {
            struct
            {
                float r, g, b, a;
            } face_colors[3];  // 顶部、底部、侧面
        };

        const ConstantBuffer cb =
        {
            {
                { 0.0f, 1.0f, 0.0f, 1.0f },  // 顶部
                { 1.0f, 0.0f, 0.0f, 1.0f },  // 底部
                { 0.0f, 0.0f, 1.0f, 1.0f }   // 侧面
            }
        };
        AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer>>(gfx, cb));

        // 添加输入布局
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, 0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

void Cylinder3D::SetSize(const DirectX::XMFLOAT3& size)
{
    conf = size;  // 设置顶部半径、底部半径、高度
}

void Cylinder3D::ScaleSize(const DirectX::XMFLOAT3& radio)
{
    conf.x *= radio.x;
    conf.y *= radio.y;
    conf.z *= radio.z;  // 缩放操作
}

DirectX::XMMATRIX Cylinder3D::GetTransformMatrix() const noexcept
{
    // 获取平移矩阵
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

    // 获取旋转矩阵（绕X、Y、Z轴旋转）
    DirectX::XMMATRIX rotationX = DirectX::XMMatrixRotationX(rotation.x);
    DirectX::XMMATRIX rotationY = DirectX::XMMatrixRotationY(rotation.y);
    DirectX::XMMATRIX rotationZ = DirectX::XMMatrixRotationZ(rotation.z);
    DirectX::XMMATRIX rotationMatrix = rotationX * rotationY * rotationZ;

    // 获取缩放矩阵（按 X, Y, Z 三个方向的缩放）
    DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(conf.x, conf.y, conf.z);

    // 返回组合的变换矩阵：缩放 -> 旋转 -> 平移
    return scaling * rotationMatrix * translation;
}