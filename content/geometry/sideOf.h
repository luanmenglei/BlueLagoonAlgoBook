/**
 * @title 有向直线侧别
 * @brief 判断点在有向直线左侧、右侧还是线上。
 * @note 返回左1／线上0／右-1；eps 版本使用到直线的距离，要求起终点不同。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/sideOf.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P> int sideOf(P s, P e, P p) {
	return sgn(s.cross(e, p));
}

template <class P>
int sideOf(P s, P e, P p, double eps) {
	auto a = (e - s).cross(p - s);
	double l = (e - s).dist() * eps;
	return (a > l) - (a < -l);
}
