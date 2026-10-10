/**
 * @title 三维向量
 * @brief 提供三维向量点积、叉积、距离、归一化与绕轴旋转。
 * @note Point3D<i64/double>；dot、cross、距离、绕轴旋转；归一化要求非零向量。
 * @time O(1)
 * @depends contest/Base.h
 * @author Ulf Lundstrom with inspiration from tinyKACTL; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/Point3D.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class T> struct Point3D {
	using P = Point3D;
	T x, y, z;
	explicit Point3D(T x = 0, T y = 0, T z = 0)
	    : x(x), y(y), z(z) {}
	bool operator<(P p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z);
	}
	bool operator==(P p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z);
	}
	P operator+(P p) const {
		return P(x + p.x, y + p.y, z + p.z);
	}
	P operator-(P p) const {
		return P(x - p.x, y - p.y, z - p.z);
	}
	P operator*(T d) const {
		return P(x * d, y * d, z * d);
	}
	P operator/(T d) const {
		return P(x / d, y / d, z / d);
	}
	T dot(P p) const {
		return x * p.x + y * p.y + z * p.z;
	}
	P cross(P p) const {
		return P(y * p.z - z * p.y, z * p.x - x * p.z,
		         x * p.y - y * p.x);
	}
	T dist2() const { return x * x + y * y + z * z; }
	double dist() const {
		return sqrt((double)dist2());
	}
	// Azimuthal angle (longitude) to x-axis in interval
	// [-pi, pi]
	double phi() const { return atan2(y, x); }
	// Zenith angle (latitude) to the z-axis in interval
	// [0, pi]
	double theta() const {
		return atan2(sqrt(x * x + y * y), z);
	}
	P unit() const {
		return *this / (T)dist();
	} // makes dist()=1
	// returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	// returns point rotated 'angle' radians ccw around
	// axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle);
		P u = axis.unit();
		return u * dot(u) * (1 - c) + (*this) * c -
		       cross(u) * s;
	}
};
