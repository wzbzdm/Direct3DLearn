#pragma once

#include <vector>
#include <DirectXMath.h>

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