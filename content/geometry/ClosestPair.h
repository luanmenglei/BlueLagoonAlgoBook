/**
 * @title 最近点对
 * @brief 求二维整数点集中欧氏距离最近的一对点。
 * @note IP整数点，至少2点；平方距离及坐标差不溢出，重复点允许。
 * @time O(n log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ClosestPair.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using IP = Point<i64>;
pair<IP, IP> closest(vector<IP> v) {
	assert((int)(v).size() > 1);
	set<IP> S;
	sort((v).begin(), (v).end(),
	     [](IP a, IP b) { return a.y < b.y; });
	pair<i64, pair<IP, IP>> ret{ LLONG_MAX,
		                         { IP(), IP() } };
	int j = 0;
	for (IP p : v) {
		IP d{ 1 + (i64)sqrt(ret.first), 0 };
		while (v[j].y <= p.y - d.x)
			S.erase(v[j ++]);
		auto lo = S.lower_bound(p - d),
		     hi = S.upper_bound(p + d);
		for (; lo != hi; ++lo)
			ret = min(
			    ret, { (*lo - p).dist2(), { *lo, p } });
		S.insert(p);
	}
	return ret.second;
}
