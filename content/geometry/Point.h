/**
 * @title 二维点与向量
 * @brief 提供二维点向量的算术、点积、叉积、长度与方向操作。
 * @note Point<i64> 用于整数谓词，Point<double> 用于构造与距离；dot/cross/dist2 均以 T 计算，先估计溢出。unit/normal
 * 要求非零向量。
 * @time O(1)
 * @depends contest/Base.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/Point.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class T> int sgn(T x) {
	return (x > 0) - (x < 0);
}
template <class T> struct Point {
	using P = Point;
	T x, y;
	explicit Point(T x = 0, T y = 0) : x(x), y(y) {}
	bool operator<(P p) const {
		return tie(x, y) < tie(p.x, p.y);
	}
	bool operator==(P p) const {
		return tie(x, y) == tie(p.x, p.y);
	}
	P operator+(P p) const {
		return P(x + p.x, y + p.y);
	}
	P operator-(P p) const {
		return P(x - p.x, y - p.y);
	}
	P operator*(T d) const { return P(x * d, y * d); }
	P operator/(T d) const { return P(x / d, y / d); }
	T dot(P p) const { return x * p.x + y * p.y; }
	T cross(P p) const { return x * p.y - y * p.x; }
	T cross(P a, P b) const {
		return (a - *this).cross(b - *this);
	}
	T dist2() const { return x * x + y * y; }
	double dist() const {
		return sqrt((double)dist2());
	}
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const {
		return *this / dist();
	} // makes dist()=1
	P perp() const {
		return P(-y, x);
	} // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the
	// origin
	P rotate(double a) const {
		return P(x * cos(a) - y * sin(a),
		         x * sin(a) + y * cos(a));
	}
	friend ostream &operator<<(ostream &os, P p) {
		return os << "(" << p.x << "," << p.y << ")";
	}
};
