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

    template <class T>
    Geometry<T> CreateT(float l, float w, float h) {
        std::vector<T> vertices = {
            // 前面 (z = 1.0f)
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f, 0.0f }, 0 }, // 左下角
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, { 1.0f, 0.0f }, 0 }, // 右下角
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, { 1.0f, 1.0f }, 0 }, // 右上角
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f, 1.0f }, 0 }, // 左上角

            // 后面 (z = -1.0f)
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { 1.0f, 0.0f }, 1 }, // 左下角
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, 0.0f }, 1 }, // 右下角
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f, 1.0f }, 1 }, // 右上角
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { 1.0f, 1.0f }, 1 }, // 左上角

            // 上面 (y = 1.0f)
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f, 0.0f }, 2 },
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, { 1.0f, 0.0f }, 2 },
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, { 1.0f, 1.0f }, 2 },
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f, 1.0f }, 2 },

            // 下面 (y = -1.0f)
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, 0.0f }, 3 },
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, { 1.0f, 0.0f }, 3 },
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, { 1.0f, 1.0f }, 3 },
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f, 1.0f }, 3 },

            // 左面 (x = -1.0f)
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { 1.0f, 0.0f }, 4 },
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, 0.0f }, 4 },
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f, 1.0f }, 4 },
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { 1.0f, 1.0f }, 4 },

            // 右面 (x = 1.0f)
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, 0.0f }, 5 },
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, { 1.0f, 0.0f }, 5 },
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, { 1.0f, 1.0f }, 5 },
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f, 1.0f }, 5 }
        };

        // 生成索引，确保每个面被分成两个三角形
        std::vector<unsigned short> indices = {
            // Front face
            0, 1, 2, 0, 2, 3,
            // Back face
            6, 5, 4, 7, 6, 4,
            // Top face
            8, 9, 10, 8, 10, 11,
            // Bottom face
            12, 13, 14, 12, 14, 15,
            // Left face
            18, 17, 16, 19, 18, 16,
            // Right face
            22, 21, 20, 23, 22, 20
        };

        return Geometry<T>(vertices, indices);
    }

	template <class T>
	Geometry<T> CreateT(DirectX::XMFLOAT3 conf) {
		return CreateT<T>(conf.x, conf.y, conf.z);
	}

	template <class T>
	Geometry<T> CreateT() {
		return CreateT<T>(2, 2, 2);
	}

    // 生成默认结构
    Geometry<DefaultVertice> CreateD(float l = 1.0f, float w = 1.0f, float h = 1.0f) {
        // 默认颜色 (白色)
        DirectX::XMFLOAT4 defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };

        std::vector<DefaultVertice> vertices = {
            // Front face
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, defaultColor },
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f }, defaultColor },
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f }, defaultColor },

            // Back face
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f }, defaultColor },
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f }, defaultColor },
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f }, defaultColor },

            // Top face
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f }, defaultColor },
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f }, defaultColor },

            // Bottom face
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, defaultColor },
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f }, defaultColor },
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f }, defaultColor },
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f }, defaultColor },

            // Left face
            { { -l / 2.0f, -h / 2.0f,  w / 2.0f }, { -1.0f, 0.0f,  0.0f }, { 1.0f, 0.0f }, defaultColor },
            { { -l / 2.0f, -h / 2.0f, -w / 2.0f }, { -1.0f, 0.0f,  0.0f }, { 0.0f, 0.0f }, defaultColor },
            { { -l / 2.0f,  h / 2.0f, -w / 2.0f }, { -1.0f, 0.0f,  0.0f }, { 0.0f, 1.0f }, defaultColor },
            { { -l / 2.0f,  h / 2.0f,  w / 2.0f }, { -1.0f, 0.0f,  0.0f }, { 1.0f, 1.0f }, defaultColor },

            // Right face
            { {  l / 2.0f, -h / 2.0f, -w / 2.0f }, {  1.0f, 0.0f,  0.0f }, { 0.0f, 0.0f }, defaultColor },
            { {  l / 2.0f, -h / 2.0f,  w / 2.0f }, {  1.0f, 0.0f,  0.0f }, { 1.0f, 0.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f,  w / 2.0f }, {  1.0f, 0.0f,  0.0f }, { 1.0f, 1.0f }, defaultColor },
            { {  l / 2.0f,  h / 2.0f, -w / 2.0f }, {  1.0f, 0.0f,  0.0f }, { 0.0f, 1.0f }, defaultColor }
        };

        std::vector<unsigned short> indices = {
            // Front face
            0, 1, 2, 0, 2, 3,
            // Back face
            6, 5, 4, 7, 6, 4,
            // Top face
            8, 9, 10, 8, 10, 11,
            // Bottom face
            12, 13, 14, 12, 14, 15,
            // Left face
            18, 17, 16, 19, 18, 16,
            // Right face
            22, 21, 20, 23, 22, 20
        };

        return Geometry<DefaultVertice>(std::move(vertices), std::move(indices));
    }
};