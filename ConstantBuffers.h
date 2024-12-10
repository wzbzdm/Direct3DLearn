#pragma once

#include "Bindable.h"

template<typename T>
class ConstantBuffer : public Bindable {
public:
	void Update(Graphics& gtx, const T& data) {
		INFOMAN(gtx);
		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(GetContext(gtx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
		memcpy(msr.pData, &data, sizeof(data));
		GetContext(gtx)->Unmap(pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& gtx, const T& data) {
		INFOMAN(gtx);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(data);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &data;

		GFX_THROW_INFO(GetDevice(gtx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}
	ConstantBuffer(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}

protected:
	ID3D11Buffer* pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gtx) noexcept override {
		GetContext(gtx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gtx) noexcept override {
		GetContext(gtx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};
