#pragma once

#include "MyGraphics.h"

class Bindable {
public:
	virtual void Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gtx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gtx) noexcept;
};