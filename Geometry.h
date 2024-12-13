#pragma once

#include <vector>
#include <DirectXMath.h>

typedef struct {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	DirectX::XMFLOAT4 color;
} DefaultVertice;

template <class T>
class Geometry {
public:
	Geometry() = default;
	Geometry(const Geometry&) = delete;
	Geometry(std::vector<T> vertices, std::vector<unsigned short> indices) : vertices(std::move(vertices)), indices(std::move(indices)) {}
public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};