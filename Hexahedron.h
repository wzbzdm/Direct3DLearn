#pragma once

#include "Geometry.h"

class Hexahedron {
	static std::vector<unsigned short> defaultIndices;
public:
	template <class T>
	Geometry<T> Create(float l, float w, float h) {
		l /= 2;
		w /= 2;
		h /= 2;
        const std::vector<DirectX::XMFLOAT3> vertices =
        {
            { -l,-w,-h },
			{ l,-w,-h },
			{ -l,w,-h },
			{ l,w,-h },
			{ -l,-w,h },
			{ l,-w,h },
			{ -l,w,h },
			{ l,w,h }
        };

		std::vector<T> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			verts[i].pos = vertices[i];
		}

		return { verts, defaultIndices };
	}

	template <class T>
	Geometry<T> Create(DirectX::XMFLOAT3 conf) {
		return Create<T>(conf.x, conf.y, conf.z);
	}

	template <class T>
	Geometry<T> Create() {
		return Create<T>(2, 2, 2);
	}
};