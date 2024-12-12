#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gtx) noexcept {
	return gtx.context.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& gtx) noexcept {
	return gtx.device.Get();
}