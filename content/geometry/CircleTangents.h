/**
 * @title 圆的公切线
 * @brief 求两圆公切线的切点对，也可求过给定点的圆切线。
 * @note Point<double>；r2 取负求内公切线，r2=0 求过点切线；返回切点对，相切时两点相同需另用垂直圆心连线方向表示切线。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleTangents.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P>
vector<pair<P, P>> tangents(P c1, double r1, P c2,
                            double r2) {
	P d = c2 - c1;
	double dr = r1 - r2, d2 = d.dist2(),
	       h2 = d2 - dr * dr;
	if (d2 == 0 || h2 < 0) return {};
	vector<pair<P, P>> out;
	for (double sign : { -1, 1 }) {
		P v =
		    (d * dr + d.perp() * sqrt(h2) * sign) / d2;
		out.push_back({ c1 + v * r1, c2 + v * r2 });
	}
	if (h2 == 0) out.pop_back();
	return out;
}
