#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gtx, const Shape3DBase& parent) : parent(parent) {
	if (!vcbuf) {
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gtx);
	}
}

void TransformCbuf::Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept {
	// 当前图形所需的变换 及 所有图形都有的变换
	vcbuf->Update(gtx, DirectX::XMMatrixTranspose(parent.GetTransformMatrix() * gtx.GetCameraMatrix()));
	vcbuf->Bind(gtx, start, len);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::vcbuf;