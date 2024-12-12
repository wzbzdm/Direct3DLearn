#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	~IndexBuffer();
	IndexBuffer(Graphics& gtx, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gtx) noexcept override;
	UINT GetCount() const noexcept;
protected:
	UINT count;
	ID3D11Buffer* pIndexBuffer = nullptr;
};