#pragma once

#include "Shape3D.h"
#include "ConstantBuffers.h"

class TransformCbuf : public Bindable {
public:
	TransformCbuf(Graphics& gtx, const Shape3DBase& parent);
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override;
protected:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Shape3DBase& parent;
};