/**
 * @title 点到直线距离
 * @brief 求点到有向直线的带符号距离。
 * @note 带符号距离，左正右负；a!=b。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/lineDistance.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P> double lineDist(P a, P b, P p) {
	return (double)(b - a).cross(p - a) /
	       (b - a).dist();
}
