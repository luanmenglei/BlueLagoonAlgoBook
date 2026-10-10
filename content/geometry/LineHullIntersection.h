/**
 * @title 凸包极值点与直线交
 * @brief 求凸包在指定方向的极值顶点，以及直线穿过的边。
 * @note 严格CCW凸多边形，至少3点；extrVertex 求方向投影最大点，lineHull 返回被穿过的边编号。具体返回编码见本章速查。
 * @time O(log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author Oleksandr Bacherikov, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/LineHullIntersection.h
 * @license Boost Software License
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

#define cmp(i, j)                                      \
	sgn(dir.perp().cross(poly[(i) % n] - poly[(j) % n]))
#define extr(i)                                        \
	cmp(i + 1, i) >= 0 && cmp(i, i - 1 + n) < 0
template <class P>
int extrVertex(vector<P> &poly, P dir) {
	int n = (int)(poly).size(), lo = 0, hi = n;
	if (extr(0)) return 0;
	while (lo + 1 < hi) {
		int m = (lo + hi) / 2;
		if (extr(m)) return m;
		int ls = cmp(lo + 1, lo), ms = cmp(m + 1, m);
		(ls < ms || (ls == ms && ls == cmp(lo, m))
		     ? hi
		     : lo) = m;
	}
	return lo;
}

#define cmpL(i) sgn(a.cross(poly[i], b))
template <class P>
array<int, 2> lineHull(P a, P b, vector<P> &poly) {
	int endA = extrVertex(poly, (a - b).perp());
	int endB = extrVertex(poly, (b - a).perp());
	if (cmpL(endA) < 0 || cmpL(endB) > 0)
		return { -1, -1 };
	array<int, 2> res;
	for (int i = 0; i < 2; i ++) {
		int lo = endB, hi = endA,
		    n = (int)(poly).size();
		while ((lo + 1) % n != hi) {
			int m =
			    ((lo + hi + (lo < hi ? 0 : n)) / 2) % n;
			(cmpL(m) == cmpL(endB) ? lo : hi) = m;
		}
		res[i] = (lo + !cmpL(hi)) % n;
		swap(endA, endB);
	}
	if (res[0] == res[1]) return { res[0], -1 };
	if (!cmpL(res[0]) && !cmpL(res[1]))
		switch (
		    (res[0] - res[1] + (int)(poly).size() + 1) %
		    (int)(poly).size()) {
			case 0:
				return { res[0], res[0] };
			case 2:
				return { res[1], res[1] };
		}
	return res;
}

#undef cmp
#undef extr
#undef cmpL
