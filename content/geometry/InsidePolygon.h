/**
 * @title 点在多边形内
 * @brief 判断点是否位于简单多边形内部，可选择是否包含边界。
 * @note 整数谓词，简单多边形；strict 控制是否含边界。浮点使用时需补误差判断；空多边形返回false。
 * @time O(n)
 * @depends contest/Base.h, geometry/Point.h, geometry/OnSegment.h, geometry/SegmentDistance.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/InsidePolygon.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"
#include "../geometry/OnSegment.h"
#include "../geometry/SegmentDistance.h"

template <class P>
bool inPolygon(vector<P> &p, P a, bool strict = true) {
	int cnt = 0, n = (int)(p).size();
	for (int i = 0; i < n; i ++) {
		P q = p[(i + 1) % n];
		if (onSegment(p[i], q, a)) return !strict;
		// or: if (segDist(p[i], q, a) <= eps) return
		// !strict;
		cnt ^= ((a.y < p[i].y) - (a.y < q.y)) *
		           a.cross(p[i], q) >
		       0;
	}
	return cnt;
}
