#pragma once

#include "Geometry.h"

class Plane {
	static std::vector<unsigned short> defaultIndices;
public:

	template <class T>
	static Geometry<T> Create() noexcept {
		return Create<T>(2.0, 2.0, 0.0);
	}

	template <class T>
	static Geometry<T> Create(DirectX::XMFLOAT2 conf) noexcept {
		return Create<T>(conf.x, conf.y, 0.0);
	}

	template <class T>
	static Geometry<T> Create(float width, float height) noexcept {
		return Create<T>(width, height, 0.0);
	}

	template <class T>
	static Geometry<T> Create(float width, float height, float depth) noexcept {
		std::vector<DirectX::XMFLOAT3> vertices;

		vertices.push_back({ -width / 2.0f, -height / 2.0f, depth });
		vertices.push_back({ width / 2.0f, -height / 2.0f, depth });
		vertices.push_back({ -width / 2.0f, height / 2.0f, depth });
		vertices.push_back({ width / 2.0f, height / 2.0f, depth });

		std::vector<T> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			verts[i].pos = vertices[i];
		}

		return { verts, defaultIndices };
	}
};