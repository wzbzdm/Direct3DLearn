#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	~VertexBuffer();
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices);
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	ID3D11Buffer* pVertexBuffer;
};