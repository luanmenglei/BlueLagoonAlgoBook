/**
 * @title 两圆交点
 * @brief 求两个圆的交点。
 * @note DP，半径非负；相同圆（无限交点）须调用前单独处理。返回两交点，相切时重复；精度边界按尺度调整。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleIntersection.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using DP = Point<double>;
bool circleInter(DP a, DP b, double r1, double r2,
                 pair<DP, DP> *out) {
	if (a == b) {
		assert(r1 != r2);
		return false;
	}
	DP vec = b - a;
	double d2 = vec.dist2(), sum = r1 + r2,
	       dif = r1 - r2,
	       p = (d2 + r1 * r1 - r2 * r2) / (d2 * 2),
	       h2 = r1 * r1 - p * p * d2;
	if (sum * sum < d2 || dif * dif > d2) return false;
	DP mid = a + vec * p,
	   per = vec.perp() * sqrt(fmax(0, h2) / d2);
	*out = { mid + per, mid - per };
	return true;
}
