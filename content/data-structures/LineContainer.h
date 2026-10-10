/**
 * @title 直线最大值查询（动态凸包）
 * @brief 动态插入直线，查询指定横坐标处最大的 kx+b。
 * @note add(k,b) 后 query(x) 求最大 kx+b；求最小值可取负。差值、交点整除和求值均须不溢出 i64；至少插入一条线。
 * @time 均摊 O(log n)
 * @depends contest/Base.h
 * @author Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/data-structures/LineContainer.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Line {
	mutable i64 k, m, p;
	bool operator<(const Line &o) const {
		return k < o.k;
	}
	bool operator<(i64 x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static const i64 inf = LLONG_MAX;
	i64 div(i64 a, i64 b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b);
	}
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k)
			x->p = x->m > y->m ? inf : -inf;
		else
			x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(i64 k, i64 m) {
		auto z = insert({ k, m, 0 }), y = z ++, x = y;
		while (isect(y, z))
			z = erase(z);
		if (x != begin() && isect(--x, y))
			isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	i64 query(i64 x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
