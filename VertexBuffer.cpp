#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept {
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
}

VertexBuffer::~VertexBuffer() {
	if (pVertexBuffer != nullptr) {
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
}