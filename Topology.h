#pragma once

#include "Bindable.h"

class Topology : public Bindable {
public:
	Topology(Graphics& gtx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};