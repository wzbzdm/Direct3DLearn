#pragma once
#include <DirectXMath.h>
#include <optional>
#include <cmath>

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

class PlaneC {
public:
	DirectX::XMFLOAT3 point;    // 平面上的一点
	DirectX::XMFLOAT3 normal;   // 平面的法向量（需归一化）

	// 构造函数
	PlaneC(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& normal)
		: point(point), normal(normal) {}

	// 平面与射线求交点
	// 参数为射线的起点和方向
	std::optional<DirectX::XMFLOAT3> Intersect(const DirectX::XMFLOAT3& rayOrigin, const DirectX::XMFLOAT3& rayDir) const {
		DirectX::XMVECTOR n = DirectX::XMLoadFloat3(&normal);
		DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&point);
		DirectX::XMVECTOR r0 = DirectX::XMLoadFloat3(&rayOrigin);
		DirectX::XMVECTOR rd = DirectX::XMLoadFloat3(&rayDir);

		// 计算平面法向量和射线方向的点积
		float denominator = DirectX::XMVectorGetX(DirectX::XMVector3Dot(n, rd));

		// 若点积接近 0，则射线平行于平面，无交点
		if (std::fabs(denominator) < 1e-6) {
			return std::nullopt;
		}

		// 计算射线原点到平面点的向量
		DirectX::XMVECTOR r0_to_p0 = DirectX::XMVectorSubtract(p0, r0);

		// 计算射线参数 t
		float t = DirectX::XMVectorGetX(DirectX::XMVector3Dot(n, r0_to_p0)) / denominator;

		// 如果 t < 0，说明射线从平面背面开始，没有交点
		if (t < 0) {
			return std::nullopt;
		}

		// 计算交点
		DirectX::XMVECTOR intersection = DirectX::XMVectorAdd(r0, DirectX::XMVectorScale(rd, t));
		DirectX::XMFLOAT3 intersectPoint;
		DirectX::XMStoreFloat3(&intersectPoint, intersection);

		return intersectPoint;
	}

	// 平面与另一个平面求交线（辅助用法，视需求扩展）
	std::optional<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> Intersect(const PlaneC& other) const {
		DirectX::XMVECTOR n1 = DirectX::XMLoadFloat3(&normal);
		DirectX::XMVECTOR n2 = DirectX::XMLoadFloat3(&other.normal);
		DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&point);
		DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&other.point);

		// 计算两个法向量的叉积，得到交线方向
		DirectX::XMVECTOR lineDir = DirectX::XMVector3Cross(n1, n2);

		// 若叉积接近零向量，说明两平面平行或重合
		float lenSquared = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(lineDir));
		if (lenSquared < 1e-6) {
			return std::nullopt;
		}

		// 求交线上一点（解方程组）
		DirectX::XMVECTOR linePoint = DirectX::XMVectorZero(); // 初始点暂为空，需通过解方程得到

		// 暂时假设点 linePoint = origin，用户可扩展此功能计算具体点

		DirectX::XMFLOAT3 dir, point;
		DirectX::XMStoreFloat3(&dir, lineDir);
		DirectX::XMStoreFloat3(&point, linePoint);

		return std::make_pair(point, dir);
	}
};
