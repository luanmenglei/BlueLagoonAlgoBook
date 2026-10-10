/**
 * @title 圆与直线相交
 * @brief 求圆与直线的交点。
 * @note Point<double>，半径非负，a!=b；返回0/1/2个交点。h2 接近零时应按题目尺度设 eps。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleLine.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P>
vector<P> circleLine(P c, double r, P a, P b) {
	P ab = b - a,
	  p = a + ab * (c - a).dot(ab) / ab.dist2();
	double s = a.cross(b, c),
	       h2 = r * r - s * s / ab.dist2();
	if (h2 < 0) return {};
	if (h2 == 0) return { p };
	P h = ab.unit() * sqrt(h2);
	return { p - h, p + h };
}
