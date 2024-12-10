#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gtx, const Drawable& parent) :
	vcbuf(gtx),
	parent(parent) {
}

void TransformCbuf::Bind(Graphics& gtx) noexcept {
	vcbuf.Update(gtx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gtx.GetProjection()));
	vcbuf.Bind(gtx);
}