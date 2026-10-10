/**
 * @title 凸包
 * @brief 求二维点集的逆时针凸包，剔除边上中间共线点。
 * @note IP=Point<i64>，返回CCW凸包，去掉边上中间点，允许空集／重复点／全共线。整数叉积不得溢出。
 * @time O(n log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author Stjepan Glavina, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ConvexHull.h
 * @license Unlicense
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using IP = Point<i64>;
vector<IP> convexHull(vector<IP> pts) {
	if ((int)(pts).size() <= 1) return pts;
	sort((pts).begin(), (pts).end());
	vector<IP> h((int)(pts).size() + 1);
	int s = 0, t = 0;
	for (int it = 2; it --;
	     s = --t, reverse((pts).begin(), (pts).end()))
		for (IP p : pts) {
			while (t >= s + 2 &&
			       h[t - 2].cross(h[t - 1], p) <= 0)
				t --;
			h[t ++] = p;
		}
	return { h.begin(),
		     h.begin() + t - (t == 2 && h[0] == h[1]) };
}
