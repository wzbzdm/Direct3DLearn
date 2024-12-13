#pragma once

#include "MyCamera.h"
#include "Bindable.h"
#include "ConstantBuffers.h"

class CameraCbuf : public Bindable {
public:
	CameraCbuf(Graphics& gfx) noexcept;
	void Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept override;
private:
	static std::unique_ptr<PixelConstantBuffer<CameraBuffer>> cameraBuffer;
};