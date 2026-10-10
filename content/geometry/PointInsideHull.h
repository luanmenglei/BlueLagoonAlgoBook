/**
 * @title 点在凸多边形内的查询
 * @brief 对给定凸包判断点是否在内部，可选择是否包含边界。
 * @note IP；非空CCW严格凸包；strict 控制含边界，小于3点也能处理。
 * @time O(log n)
 * @depends contest/Base.h, geometry/Point.h, geometry/sideOf.h, geometry/OnSegment.h
 * @author chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/PointInsideHull.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"
#include "../geometry/sideOf.h"
#include "../geometry/OnSegment.h"

using IP = Point<i64>;

bool inHull(const vector<IP> &l, IP p,
            bool strict = true) {
	int a = 1, b = (int)(l).size() - 1, r = !strict;
	if ((int)(l).size() < 3)
		return r && onSegment(l[0], l.back(), p);
	if (sideOf(l[0], l[a], l[b]) > 0) swap(a, b);
	if (sideOf(l[0], l[a], p) >= r ||
	    sideOf(l[0], l[b], p) <= -r)
		return false;
	while (abs(a - b) > 1) {
		int c = (a + b) / 2;
		(sideOf(l[0], l[c], p) > 0 ? b : a) = c;
	}
	return sgn(l[a].cross(l[b], p)) < r;
}
