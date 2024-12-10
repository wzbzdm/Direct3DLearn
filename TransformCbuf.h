#pragma once

#include "Drawable.h"
#include "ConstantBuffers.h"

class TransformCbuf : public Bindable {
public:
	TransformCbuf(Graphics& gtx, const Drawable& parent);
	void Bind(Graphics& gtx) noexcept override;
protected:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};