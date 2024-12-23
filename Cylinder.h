#pragma once

#include "Geometry.h"
#include <cmath>

class Cylinder {
public:
	template <class T>
	static Geometry<T> Create(float topR = 1.0f, float baseR = 1.0f, float h = 2.0f, int numC = 36, int numH = 18) {
		std::vector<DirectX::XMFLOAT3> vertices;
		std::vector<unsigned short> indices;

		// 从顶部到底部， numH + 1个环
		for (int i = 0; i <= numH; i++) {
			float z = h / 2.0f - h * i / numH;
			float r = topR + (baseR - topR) * i / numH;
			// 每个环有numC个顶点
			for (int j = 0; j < numC; j++) {
				float angle = 2.0f * DirectX::XM_PI * j / numC;
				float x = r * std::cos(angle);
				float y = r * std::sin(angle);
				vertices.push_back({ x, y, z });
			}

			// 生成 中间 索引
			if (i != 0) {
				for (int j = 0; j < numC; j++) {
					int i0 = (i - 1) * numC + j;
					int i1 = (i - 1) * numC + (j + 1) % numC;
					int i2 = i * numC + (j + 1) % numC;
					int i3 = i * numC + j;
					indices.push_back(i0);
					indices.push_back(i2);
					indices.push_back(i1);
					indices.push_back(i0);
					indices.push_back(i3);
					indices.push_back(i2);
				}
			}
		}

		DirectX::XMFLOAT3 centerTop = { 0.0f, 0.0f, h / 2.0f };
		DirectX::XMFLOAT3 centerBase = { 0.0f, 0.0f, -h / 2.0f };
		vertices.push_back(centerTop);
		vertices.push_back(centerBase);

		unsigned short top = static_cast<unsigned short>(vertices.size() - 2);
		unsigned short base = static_cast<unsigned short>(vertices.size() - 1);
		// 生成顶部索引, 底部索引
		for (int i = 0; i < numC; i++) {
			// 顶部三角形
			indices.push_back(top);
			indices.push_back(i);
			indices.push_back((i + 1) % numC);

			// 底部三角形
			indices.push_back(base);
			indices.push_back(numH * numC + (i + 1) % numC);
			indices.push_back(numH * numC + i);
		}

		std::vector<T> verts(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			verts[i].pos = vertices[i];
		}

		return { verts, indices };
	}

	template <class T>
	static Geometry<T> Create(int numC, int numH) {
		return Create<T>(1.0f, 1.0f, 2.0f, numC, numH);
	}
	template <class T>
	static Geometry<T> Create(float topR, float baseR, float h) {
		return Create<T>(topR, baseR, h, 36, 18);
	}

	Geometry<DefaultVertice> CreateD(float topR = 1.0f, float baseR = 1.0f, float h = 2.0f, int numC = 36, int numH = 18) {
		std::vector<DefaultVertice> vertices;
		std::vector<unsigned short> indices;

		// 从顶部到底部， numH + 1个环
		for (int i = 0; i <= numH; i++) {
			float z = h / 2.0f - h * i / numH;
			float r = topR + (baseR - topR) * i / numH;
			// 每个环有numC个顶点
			for (int j = 0; j < numC; j++) {
				float angle = 2.0f * DirectX::XM_PI * j / numC;
				float x = r * std::cos(angle);
				float y = r * std::sin(angle);
				vertices.push_back({ { x, y, z }, {x, y, 0}, {0.0f, 0.0f}, 2 });
			}

			// 生成 中间 索引
			if (i != 0) {
				for (int j = 0; j < numC; j++) {
					int i0 = (i - 1) * numC + j;
					int i1 = (i - 1) * numC + (j + 1) % numC;
					int i2 = i * numC + (j + 1) % numC;
					int i3 = i * numC + j;
					indices.push_back(i0);
					indices.push_back(i2);
					indices.push_back(i1);
					indices.push_back(i0);
					indices.push_back(i3);
					indices.push_back(i2);
				}
			}
		}

		unsigned short baseTop = static_cast<unsigned short>(vertices.size());

		// 顶部
		for (int i = 0; i < numC; i++) {
			float angle = 2.0f * DirectX::XM_PI * i / numC;
			float x = topR * std::cos(angle);
			float y = topR * std::sin(angle);
			vertices.push_back({ { x, y, h / 2.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, 0 });
		}

		unsigned short baseBot = static_cast<unsigned short>(vertices.size());	

		// 底部
		for (int i = 0; i < numC; i++) {
			float angle = 2.0f * DirectX::XM_PI * i / numC;
			float x = baseR * std::cos(angle);
			float y = baseR * std::sin(angle);
			vertices.push_back({ { x, y, -h / 2.0f }, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, 1 });
		}

		DirectX::XMFLOAT3 centerTop = { 0.0f, 0.0f, h / 2.0f };
		DirectX::XMFLOAT3 centerBase = { 0.0f, 0.0f, -h / 2.0f };
		vertices.push_back({ centerTop, { 0.0f, 0.0f, 1.0f }, {0.0f, 0.0f}, 0 });
		vertices.push_back({ centerBase, { 0.0f, 0.0f, -1.0f }, {0.0f, 0.0f}, 1 });

		unsigned short top = static_cast<unsigned short>(vertices.size() - 2);
		unsigned short base = static_cast<unsigned short>(vertices.size() - 1);
		// 生成顶部索引, 底部索引
		for (int i = 0; i < numC; i++) {
			// 顶部三角形
			indices.push_back(top);
			indices.push_back(baseTop + i);
			if (i == numC - 1) {
				indices.push_back(baseTop);
			}
			else {
				indices.push_back(baseTop + i + 1);
			}
			
			// 底部三角形
			indices.push_back(base);
			if (i == numC - 1) {
				indices.push_back(baseBot);
			}
			else {
				indices.push_back(baseBot + i + 1);
			}
			indices.push_back(baseBot + i);
		}

		return { vertices, indices };
	}
};