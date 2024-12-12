#pragma once

#include "Geometry.h"

class Triangle {
public:
	template <class T>
	static Geometry<T> Create() noexcept {
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<unsigned short> indices;

		vertices.push_back({ 0.0f, 0.5f, 0.0f });
		vertices.push_back({ 0.5f, -0.5f, 0.0f });
		vertices.push_back({ -0.5f, -0.5f, 0.0f });

		std::vector<T> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			verts[i].pos = vertices[i];
		}

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		return { verts, indices };
	}
};