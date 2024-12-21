#pragma once

#include "Shape3DBase.h"
#include "BindableBase.h"
#include "TextureGenerators.h"
#include "ImageLoader.h"
#include "Geometry.h"
#include "Ray.h"

template<class T>
class Shape3D : public Shape3DBase {
private:
	void BindCustom(Graphics& gfx) noexcept
	{
		// 材质, 插槽2
		AddBind(std::make_unique<MaterialCbuf>(gfx, *this), 2, 1);
		// 颜色，插槽3
		AddBind(std::make_unique<ColorCbuf>(gfx, *this), 3, 1);

		// 世界变换, 顶点插槽1
		AddBind(std::make_unique<TransformCbuf>(gfx, *this), 1, 1);
	}

protected:
	static bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}

	void BindAll(Graphics& gfx) noexcept
	{
		this->BindCustom(gfx);
	}

	static void BindStaticAll(Graphics& gfx, Geometry<DefaultVertice>& model) {
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"LightVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"LightPixelShader.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Normal",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Index",     0, DXGI_FORMAT_R32_UINT,        0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		//TextureData data = ImageLoader::Load2D(TESTIMG);
		//auto pTexture = std::make_unique<Texture2D>(gfx, data);
		//AddStaticBind(std::move(pTexture));
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind, unsigned int start, unsigned int len) noexcept
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		std::unique_ptr<BindableInfo> info = std::make_unique<BindableInfo>();
		info->bind = std::move(bind);
		info->start = start;
		info->num = len;
		staticBinds.push_back(std::move(info));
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
	{
		AddStaticBind(std::move(bind), 0, 1);
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		std::unique_ptr<BindableInfo> info = std::make_unique<BindableInfo>();
		info->bind = std::move(ibuf);
		staticBinds.push_back(std::move(info));
	}
	void SetIndexFromStatic() noexcept
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b->bind.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && pIndexBuffer != nullptr);
	}
private:
	const std::vector<std::unique_ptr<BindableInfo>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<BindableInfo>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<BindableInfo>> Shape3D<T>::staticBinds;
