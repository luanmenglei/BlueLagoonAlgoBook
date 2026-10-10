/**
 * @title 点到凸多边形的切线
 * @brief 求经过给定点、与凸多边形相切的两侧切点。
 * @note c 为CCW强凸多边形且末尾重复首点，至少2个不同顶点。p 不等于任何顶点；up=1/-1 求两侧切点，严格内部返回-1，共线切线可能任选端点。
 * @time O(log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author zigui; koosaga library; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/geometry/tangent.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using IP = Point<i64>;
int convex_tangent(const vector<IP> &c, IP p,
                   int up = 1) {
	auto sign = [&](i64 x) {
		return x > 0 ? up : x == 0 ? 0 : -up;
	};
	auto local = [&](IP a, IP b, IP d) {
		return sign(p.cross(a, b)) <= 0 &&
		       sign(p.cross(b, d)) >= 0;
	};
	int n = c.size() - 1, l = 0, r = n;
	if (local(c[1], c[0], c[n - 1])) return 0;
	while (l + 1 < r) {
		int m = (l + r) / 2;
		if (local(c[m - 1], c[m], c[m + 1])) return m;
		if (sign(p.cross(c[l], c[l + 1])) < 0) {
			if (sign(p.cross(c[m], c[m + 1])) > 0)
				r = m;
			else if (sign(p.cross(c[m], c[l])) > 0)
				l = m;
			else
				r = m;
		} else {
			if (sign(p.cross(c[m], c[m + 1])) < 0)
				l = m;
			else if (sign(p.cross(c[m], c[l])) < 0)
				l = m;
			else
				r = m;
		}
	}
	if (l && local(c[l - 1], c[l], c[l + 1])) return l;
	if (r != n && local(c[r - 1], c[r], c[r + 1]))
		return r;
	return -1;
}
