#pragma once

#include "Bindable.h"

template<typename T>
class ConstantBuffer : public Bindable {
public:
	void Update(Graphics& gtx, const T& data) {
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gtx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, &data, sizeof(data));
		GetContext(gtx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	void Update(Graphics& gtx, const std::vector<T>& data) {
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gtx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, data.data(), sizeof(T) * data.size());
		GetContext(gtx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	ConstantBuffer(Graphics& gtx, const T& data) {
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(data);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &data;

		GetDevice(gtx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}

	ConstantBuffer(Graphics& gtx, const std::vector<T>& data) {
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T) * data.size();
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = data.data();

		GetDevice(gtx)->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	}

	ConstantBuffer(Graphics& gfx)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(T);
		cbd.StructureByteStride = 0u;
		GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override {
		GetContext(gtx)->VSSetConstantBuffers(start, len, pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	/*PixelConstantBuffer(Graphics& gtx, const std::vector<C>& data) {
		this->Initialize(gtx, data.data(), sizeof(C) * data.size());
	}*/
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override {
		GetContext(gtx)->PSSetConstantBuffers(start, len, pConstantBuffer.GetAddressOf());
	}
};

// 同时绑定到顶点着色器和像素着色器
template<typename C>
class SharedConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override {
		GetContext(gtx)->VSSetConstantBuffers(start, len, pConstantBuffer.GetAddressOf());
		GetContext(gtx)->PSSetConstantBuffers(start, len, pConstantBuffer.GetAddressOf());
	}
};
