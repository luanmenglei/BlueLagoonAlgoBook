/**
 * @title 线段交点
 * @brief 求两条线段的交点或重叠区间端点。
 * @note 返回0／1／2个点，2个表示重叠区间端点。整数点仅适用于交点也为整数的场景；一般构造用浮点并调整 eps。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h, geometry/OnSegment.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/SegmentIntersection.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"
#include "../geometry/OnSegment.h"

template <class P>
vector<P> segInter(P a, P b, P c, P d) {
	auto oa = c.cross(d, a), ob = c.cross(d, b),
	     oc = a.cross(b, c), od = a.cross(b, d);
	// Checks if intersection is single non-endpoint
	// point.
	if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
		return { (a * ob - b * oa) / (ob - oa) };
	set<P> s;
	if (onSegment(c, d, a)) s.insert(a);
	if (onSegment(c, d, b)) s.insert(b);
	if (onSegment(a, b, c)) s.insert(c);
	if (onSegment(a, b, d)) s.insert(d);
	return { (s).begin(), (s).end() };
}
