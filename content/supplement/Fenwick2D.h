/**
 * @title 二维树状数组／矩形求和
 * @brief 预登记更新坐标后，动态加点权并查询二维前缀和／矩形和。
 * @note 0-based 点坐标任意 i64；先 reserve 所有可能更新的点，再 build。sum(x,y) 统计横纵坐标均严格小于 x,y 的点。重复 reserve
 * 允许；不能更新未登记点。
 * @usage 登记点 (1,2) 后 build；`f.add(1,2,5);`；`f.rect(0,0,2,3)` 得5。
 * @time O(log² n) 更新／查询，O(n log n) 空间
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Fenwick2D {
	vector<pair<i64, i64>> pts;
	vector<i64> xs;
	vector<vector<i64>> ys, tr;
	void reserve(i64 x, i64 y) {
		pts.emplace_back(x, y);
	}
	void build() {
		xs.clear();
		for (auto [x, y] : pts)
			xs.push_back(x);
		sort(xs.begin(), xs.end());
		xs.erase(unique(xs.begin(), xs.end()),
		         xs.end());
		int n = xs.size();
		ys.assign(n + 1, {});
		tr.assign(n + 1, {});
		for (auto [x, y] : pts)
			for (int i = lower_bound(xs.begin(),
			                         xs.end(), x) -
			             xs.begin() + 1;
			     i <= n; i += i & -i)
				ys[i].push_back(y);
		for (int i = 1; i <= n; i ++) {
			sort(ys[i].begin(), ys[i].end());
			ys[i].erase(
			    unique(ys[i].begin(), ys[i].end()),
			    ys[i].end());
			tr[i].assign(ys[i].size() + 1, 0);
		}
	}
	void add(i64 x, i64 y, i64 v) {
		int k = lower_bound(xs.begin(), xs.end(), x) -
		        xs.begin();
		assert(k < (int)xs.size() && xs[k] == x);
		for (int i = k + 1; i <= (int)xs.size();
		     i += i & -i) {
			int j = lower_bound(ys[i].begin(),
			                    ys[i].end(), y) -
			        ys[i].begin();
			assert(j < (int)ys[i].size() &&
			       ys[i][j] == y);
			for (j ++; j < (int)tr[i].size();
			     j += j & -j)
				tr[i][j] += v;
		}
	}
	i64 sum(i64 x, i64 y) const {
		i64 ans = 0;
		for (int i =
		         lower_bound(xs.begin(), xs.end(), x) -
		         xs.begin();
		     i; i -= i & -i)
			for (int j = lower_bound(ys[i].begin(),
			                         ys[i].end(), y) -
			             ys[i].begin();
			     j; j -= j & -j)
				ans += tr[i][j];
		return ans;
	}
	i64 rect(i64 l, i64 d, i64 r, i64 u) const {
		return sum(r, u) - sum(l, u) - sum(r, d) +
		       sum(l, d);
	}
};
