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

	Geometry<DefaultVertice> CreateD(float width = 2.0f, float height = 2.0f) noexcept {
		std::vector<DefaultVertice> vertices = {
			// 正面
			{ { -width / 2.0f, -height / 2.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0}, // 左下角
			{ { width / 2.0f, -height / 2.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, 0}, // 右下角
			{ { width / 2.0f, height / 2.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0}, // 右上角
			{ { -width / 2.0f, height / 2.0f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, 0}, // 左上角

			// 反面
			{ { -width / 2.0f, height / 2.0f, 0.0f }, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, 1}, // 左上角
			{ { width / 2.0f, height / 2.0f, 0.0f }, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, 1}, // 右上角
			{ { width / 2.0f, -height / 2.0f, 0.0f }, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, 1}, // 右下角
			{ { -width / 2.0f, -height / 2.0f, 0.0f }, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, 1}, // 左下角
		};

		std::vector<unsigned short> indices = {
			0, 1, 2, 0, 2, 3,   // 正面
			4, 5, 6, 4, 6, 7,    // 反面
		};

		return { vertices, indices };
	}
};