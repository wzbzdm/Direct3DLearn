#pragma once

#include "Geometry.h"
#include <cmath>

class Sphere {
public:
    template <class T>
    static Geometry<T> Create(float r, int numC, int numH) {
        std::vector<DirectX::XMFLOAT3> vertices;
        std::vector<unsigned short> indices;

        // 断言输入有效性
        assert(numC >= 3 && numH >= 3);

        // 创建中间顶点
        for (int i = 1; i < numH; i++) { // 从第一个环到最后一个环
            float theta = DirectX::XM_PI * i / numH; // 纬度角
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int j = 0; j < numC; j++) { // 每个环上的顶点
                float phi = 2.0f * DirectX::XM_PI * j / numC; // 经度角
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);
                vertices.push_back({
                    r * cosPhi * sinTheta,
                    r * sinPhi * sinTheta,
                    r * cosTheta
                    });
            }
        }

        // 添加北极点和南极点
        int northPoleIdx = (int)vertices.size();
        vertices.push_back({ 0.0f, 0.0f, r }); // 北极点
        int southPoleIdx = (int)vertices.size();
        vertices.push_back({ 0.0f, 0.0f, -r }); // 南极点

        // 生成中间部分的索引
        auto calcIdx = [numC](int iH, int iC) { return iH * numC + iC; };
        for (int i = 0; i < numH - 2; i++) {
            for (int j = 0; j < numC; j++) {
                int nextJ = (j + 1) % numC;

                // 两个三角形构成一个矩形
                indices.push_back(calcIdx(i, j));
                indices.push_back(calcIdx(i + 1, j));
                indices.push_back(calcIdx(i, nextJ));

                indices.push_back(calcIdx(i, nextJ));
                indices.push_back(calcIdx(i + 1, j));
                indices.push_back(calcIdx(i + 1, nextJ));
            }
        }

        // 为北极和南极点生成索引
        for (int j = 0; j < numC; j++) {
            int nextJ = (j + 1) % numC;

            // 北极点扇形
            indices.push_back(northPoleIdx);
            indices.push_back(calcIdx(0, j));
            indices.push_back(calcIdx(0, nextJ));

            // 南极点扇形
            indices.push_back(southPoleIdx);
            indices.push_back(calcIdx(numH - 2, nextJ));
            indices.push_back(calcIdx(numH - 2, j));
        }

        // 将顶点数据转为模板类型 T
        std::vector<T> verts(vertices.size());
        for (size_t i = 0; i < vertices.size(); i++) {
            verts[i].pos = vertices[i];
        }

        return { std::move(verts), std::move(indices) };
    }


	template <class T>
	static Geometry<T> Create(float r) {
		return Create<T>(r, 24, 24);
	}

	template <class T>
	static Geometry<T> Create(int numC, int numH) {
		return Create<T>(1.0f, numC, numH);
	}
};