#include "Hexahedron.h"
#include "BoxTest.h"
#include "BindableBase.h"
#include "TextureGenerators.h"
#include "ImageLoader.h"

Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if (!IsStaticInitialized()) {
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 tex;
			UINT index;
		};

		// auto model = Hexahedron().CreateT<Vertex>();

		auto model = Hexahedron().CreateD();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"LightVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"LightPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		/*struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};

		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));*/

		//const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		//{
		//	{ "Position",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "TexCoord",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "Index",     0, DXGI_FORMAT_R32_UINT,        0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		TextureData data = ImageLoader::Load2D(TESTIMG);
		auto pTexture = std::make_unique<Texture2D>(gfx, data);
		AddStaticBind(std::move(pTexture));

		// 纹理生成与绑定
		//TextureGenerator3D textureGen3D(256, 256, 256, [](size_t x, size_t y, size_t z, uint8_t* pOut)
		//	{
		//		// 根据坐标生成纹理数据，简单处理为单一颜色值（可根据需求修改）
		//		pOut[0] = static_cast<uint8_t>((x + y + z) % 256);
		//		pOut[1] = static_cast<uint8_t>((x * y + z) % 256);
		//		pOut[2] = static_cast<uint8_t>((y + z) % 256);
		//		pOut[3] = 255; // alpha值设为255（不透明）
		//	});

		//TextureData textureData = textureGen3D.Generate();
		//auto pTexture = std::make_unique<Texture3D>(gfx, textureData);
		//AddStaticBind(std::move(pTexture));
		//for (int i = 0; i < 6; i++) {
		//	TextureData data = ImageLoader::Load2D(TESTIMG);
		//	auto pTexture = std::make_unique<Texture2D>(gfx, data);
		//	AddStaticBind(std::move(pTexture), i, 1);
		//}
	}
	else {
		SetIndexFromStatic();
	}
	// AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	// 动态绑定数据包括，灯光，相机
	BindDefault(gfx, *this);

	// 材质, 插槽2
	AddBind(std::make_unique<MaterialCbuf>(gfx, *this), 2, 1);

	// 世界变换, 顶点插槽1
	AddBind(std::make_unique<TransformCbuf>(gfx, *this), 1, 1);
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformMatrix() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

void Box::SetSize(const DirectX::XMFLOAT3& size) {

}

void Box::Zoom(const DirectX::XMFLOAT3& scale) {

}
