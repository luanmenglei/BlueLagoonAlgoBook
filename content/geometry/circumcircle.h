/**
 * @title 三角形外接圆
 * @brief 求非共线三点确定的外接圆圆心和半径。
 * @note DP，三个点不共线。ccCenter 返回圆心，ccRadius 返回半径。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/circumcircle.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using DP = Point<double>;
double ccRadius(DP A, DP B, DP C) {
	return (B - A).dist() * (C - B).dist() *
	       (A - C).dist() / abs((B - A).cross(C - A)) /
	       2;
}
DP ccCenter(DP A, DP B, DP C) {
	DP b = C - A, c = B - A;
	return A + (b * c.dist2() - c * b.dist2()).perp() /
	               b.cross(c) / 2;
}
