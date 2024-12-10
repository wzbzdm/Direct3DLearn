#include "VertexBuffer.h"

template<class V>
VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<V>& vertices) : stride(sizeof(V)) {
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(V) * vertices.size());
	bd.StructureByteStride = sizeof(V);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}

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