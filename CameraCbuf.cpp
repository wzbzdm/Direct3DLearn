#include "CameraCbuf.h"

CameraCbuf::CameraCbuf(Graphics& gfx) noexcept {
	if (!cameraBuffer) {
		cameraBuffer = std::make_unique<SharedConstantBuffer<CameraBuffer>>(gfx);
	}
}

void CameraCbuf::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	CameraBuffer cb = gfx.GetCameraBuffer();
	cameraBuffer->Update(gfx, cb);
	cameraBuffer->Bind(gfx, start, len);
}

std::unique_ptr<SharedConstantBuffer<CameraBuffer>> CameraCbuf::cameraBuffer;