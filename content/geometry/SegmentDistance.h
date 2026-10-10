/**
 * @title 点到线段距离
 * @brief 求点到线段的最短距离。
 * @note DP=Point<double>；零长度线段可处理。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/SegmentDistance.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using DP = Point<double>;
double segDist(DP s, DP e, DP p) {
	if (s == e) return (p - s).dist();
	auto d = (e - s).dist2(),
	     t = min(d, max(.0, (p - s).dot(e - s)));
	return ((p - s) * d - (e - s) * t).dist() / d;
}
