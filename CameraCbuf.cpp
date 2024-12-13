#include "CameraCbuf.h"

CameraCbuf::CameraCbuf(Graphics& gfx) noexcept {
	if (!cameraBuffer) {
		cameraBuffer = std::make_unique<PixelConstantBuffer<CameraBuffer>>(gfx);
	}
}

void CameraCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	CameraBuffer cb = gfx.GetCameraBuffer();
	cameraBuffer->Update(gfx, cb);
	cameraBuffer->Bind(gfx, start, len);
}

std::unique_ptr<PixelConstantBuffer<CameraBuffer>> CameraCbuf::cameraBuffer;