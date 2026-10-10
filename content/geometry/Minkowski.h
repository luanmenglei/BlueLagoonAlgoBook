/**
 * @title 凸多边形闵可夫斯基和
 * @brief 求两个凸多边形点向量之和形成的凸多边形。
 * @note 输入CCW严格凸包且不重复首点；允许空、点、线段退化，退化时用凸包收尾。坐标和与叉积须不溢出。
 * @time 非退化 O(n+m)，退化 O((n+m) log(n+m))
 * @depends contest/Base.h, geometry/ConvexHull.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/ConvexHull.h"

using IP = Point<i64>;
vector<IP> minkowski(vector<IP> a, vector<IP> b) {
	if (a.empty() || b.empty()) return {};
	if (a.size() < 3 || b.size() < 3) {
		vector<IP> c;
		for (IP x : a)
			for (IP y : b)
				c.push_back(x + y);
		return convexHull(c);
	}
	auto rotate_min = [](vector<IP> &p) {
		auto it = min_element(
		    p.begin(), p.end(), [](IP x, IP y) {
			    return tie(x.y, x.x) < tie(y.y, y.x);
		    });
		rotate(p.begin(), it, p.end());
	};
	rotate_min(a);
	rotate_min(b);
	int n = a.size(), m = b.size();
	a.push_back(a[0]);
	b.push_back(b[0]);
	vector<IP> c;
	int i = 0, j = 0;
	IP p = a[0] + b[0];
	c.push_back(p);
	while (i < n || j < m) {
		IP x = i < n ? a[i + 1] - a[i] : IP(),
		   y = j < m ? b[j + 1] - b[j] : IP();
		i64 z = x.cross(y);
		bool ai = i < n && (j == m || z >= 0),
		     bj = j < m && (i == n || z <= 0);
		if (ai) p = p + x, i ++;
		if (bj) p = p + y, j ++;
		c.push_back(p);
	}
	c.pop_back();
	return c;
}
