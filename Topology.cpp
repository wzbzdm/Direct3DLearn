#include "Topology.h"

Topology::Topology(Graphics& gtx, D3D11_PRIMITIVE_TOPOLOGY type) {
	this->type = type;
}

void Topology::Bind(Graphics& gtx) noexcept {
	GetContext(gtx)->IASetPrimitiveTopology(type);
}