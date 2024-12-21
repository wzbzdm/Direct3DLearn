#pragma once
#include <DirectXMath.h>

class Ray {
private:
	DirectX::XMFLOAT3 origin;  // 射线起点
	DirectX::XMFLOAT3 direction;  // 射线方向
public:
	Ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction)
		: origin(origin), direction(direction) {}
    Ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& target, int i)
        : origin(origin)
    {
        DirectX::XMVECTOR originVec = DirectX::XMLoadFloat3(&origin);
        DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetVec, originVec);
        directionVec = DirectX::XMVector3Normalize(directionVec); // Normalize the direction

        DirectX::XMStoreFloat3(&direction, directionVec);
    }

	const DirectX::XMFLOAT3& GetOrigin() const {
		return origin;
	}
	const DirectX::XMFLOAT3& GetDirection() const {
		return direction;
	}

    // 射线与三角形的交点检测实现
    bool RayIntersectsTriangle(const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2,
        float& t, DirectX::XMFLOAT3& barycentricCoords) const
    {
        using namespace DirectX;

        // 将三角形表示为两个边向量
        XMVECTOR edge1 = XMLoadFloat3(&v1) - XMLoadFloat3(&v0);
        XMVECTOR edge2 = XMLoadFloat3(&v2) - XMLoadFloat3(&v0);

        // 计算法向量
        XMVECTOR h = XMVector3Cross(XMLoadFloat3(&direction), edge2);
        float a = XMVectorGetX(XMVector3Dot(edge1, h));

        // 如果平行（接近 0），则不相交。引入一个容差值（epsilon）。
        const float epsilon = 1e-4f;  // 可以根据需求调整容差值
        if (fabs(a) < epsilon) {
            return false;
        }

        float f = 1.0f / a;

        XMVECTOR s = XMLoadFloat3(&origin) - XMLoadFloat3(&v0);
        float u = f * XMVectorGetX(XMVector3Dot(s, h));
        if (u < -epsilon || u > 1.0f + epsilon) {
            return false;
        }

        XMVECTOR q = XMVector3Cross(s, edge1);
        float v = f * XMVectorGetX(XMVector3Dot(XMLoadFloat3(&direction), q));
        if (v < -epsilon || u + v > 1.0f + epsilon) {
            return false;
        }

        // 计算交点参数 t
        t = f * XMVectorGetX(XMVector3Dot(edge2, q));
        if (t > epsilon) {  // 在t值计算时也考虑容差
            // 交点的重心坐标
            barycentricCoords = { 1 - u - v, u, v };
            return true;
        }

        return false;
    }

};