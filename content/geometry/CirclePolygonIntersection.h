/**
 * @title 圆与多边形交面积
 * @brief 求圆与简单多边形相交区域的面积。
 * @note DP；简单CCW多边形、无连续重复点、r>=0。返回有向交面积；CW 输入需取负或反序。
 * @time O(n)
 * @depends contest/Base.h, geometry/Point.h
 * @author chilli, Takanori MAEHARA; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CirclePolygonIntersection.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using DP = Point<double>;
#define arg(p, q) atan2(p.cross(q), p.dot(q))
double circlePoly(DP c, double r, vector<DP> ps) {
	auto tri = [&](DP p, DP q) {
		auto r2 = r * r / 2;
		DP d = q - p;
		auto a = d.dot(p) / d.dist2(),
		     b = (p.dist2() - r * r) / d.dist2();
		auto det = a * a - b;
		if (det <= 0) return arg(p, q) * r2;
		auto s = max(0., -a - sqrt(det)),
		     t = min(1., -a + sqrt(det));
		if (t < 0 || 1 <= s) return arg(p, q) * r2;
		DP u = p + d * s, v = q + d * (t - 1);
		return arg(p, u) * r2 + u.cross(v) / 2 +
		       arg(v, q) * r2;
	};
	auto sum = 0.0;
	for (int i = 0; i < (int)(ps).size(); i ++)
		sum += tri(ps[i] - c,
		           ps[(i + 1) % (int)(ps).size()] - c);
	return sum;
}

#undef arg
