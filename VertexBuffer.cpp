#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx, unsigned int start, unsigned int len) noexcept {
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(start, len, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
