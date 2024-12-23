#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gtx, const Shape3DBase& parent) : parent(parent) {
	if (!vcbuf) {
		vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gtx);
	}
}

void TransformCbuf::Bind(Graphics& gtx, unsigned int start, unsigned int len) noexcept {
	vcbuf->Update(gtx, DirectX::XMMatrixTranspose(parent.GetTransformMatrix()));	// 测试将投影矩阵放在着色器中
	vcbuf->Bind(gtx, start, len);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::vcbuf;