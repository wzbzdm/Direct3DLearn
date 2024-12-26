#pragma once

#include "Geometry.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

class Diamond {
public:
    // Diamond-Square 地形生成方法
    static Geometry<DefaultVertice> CreateD(int size, float roughness) {
        std::vector<DefaultVertice> vertices;
        std::vector<unsigned int> indices;
        if (size < 2 || (size & (size - 1)) != 0) {
            throw std::invalid_argument("Size must be a power of 2 plus 1.");
        }

        int gridSize = size;
        float halfStep = (gridSize - 1) / 2.0f;

        // 初始化高度图
        std::vector<std::vector<float>> heightMap(gridSize, std::vector<float>(gridSize, 0.0f));

        // 随机数生成器
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(-roughness, roughness);

        // 初始化角点
        heightMap[0][0] = distribution(generator);
        heightMap[0][gridSize - 1] = distribution(generator);
        heightMap[gridSize - 1][0] = distribution(generator);
        heightMap[gridSize - 1][gridSize - 1] = distribution(generator);

        // Diamond-Square 算法
        for (int stepSize = gridSize - 1; stepSize > 1; stepSize /= 2) {
            int halfSize = stepSize / 2;

            // Diamond step
            for (int y = halfSize; y < gridSize; y += stepSize) {
                for (int x = halfSize; x < gridSize; x += stepSize) {
                    float avg = (heightMap[y - halfSize][x - halfSize] +
                        heightMap[y - halfSize][x + halfSize] +
                        heightMap[y + halfSize][x - halfSize] +
                        heightMap[y + halfSize][x + halfSize]) / 4.0f;
                    heightMap[y][x] = avg + distribution(generator);
                }
            }

            // Square step
            for (int y = 0; y < gridSize; y += halfSize) {
                for (int x = (y + halfSize) % stepSize; x < gridSize; x += stepSize) {
                    float avg = 0.0f;
                    int count = 0;
                    if (x - halfSize >= 0) { avg += heightMap[y][x - halfSize]; count++; }
                    if (x + halfSize < gridSize) { avg += heightMap[y][x + halfSize]; count++; }
                    if (y - halfSize >= 0) { avg += heightMap[y - halfSize][x]; count++; }
                    if (y + halfSize < gridSize) { avg += heightMap[y + halfSize][x]; count++; }

                    heightMap[y][x] = avg / count + distribution(generator);
                }
            }

            roughness *= 0.5f; // 缩小扰动范围
        }

        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                DefaultVertice vert;
                vert.pos = DirectX::XMFLOAT3(static_cast<float>(x), heightMap[y][x], static_cast<float>(y));
                vert.normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // 初始为 Y 轴朝上
                vert.tex = DirectX::XMFLOAT2(x / static_cast<float>(gridSize - 1),
                    y / static_cast<float>(gridSize - 1));
                vert.index = y * gridSize + x;
                vertices.push_back(vert);
            }
        }

        // 构建索引数据
        for (int y = 0; y < gridSize - 1; ++y) {
            for (int x = 0; x < gridSize - 1; ++x) {
                unsigned int topLeft = y * gridSize + x;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (y + 1) * gridSize + x;
                unsigned int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        return { vertices, indices };
    }
};