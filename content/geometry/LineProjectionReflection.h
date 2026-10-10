/**
 * @title 点到直线的投影／轴对称
 * @brief 求点在直线上的投影，或关于该直线的对称点。
 * @note a!=b，Point<double>；refl=false 投影，true 对称。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/LineProjectionReflection.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P>
P lineProj(P a, P b, P p, bool refl = false) {
	P v = b - a;
	return p - v.perp() * (1 + refl) * v.cross(p - a) /
	               v.dist2();
}
