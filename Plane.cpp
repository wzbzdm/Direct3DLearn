#include "Plane.h"

std::vector<unsigned short> Plane::defaultIndices = {
		0, 1, 2, 2, 1, 3,   // 正面
		3, 1, 2, 2, 1, 0    // 反面
};