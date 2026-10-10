/**
 * @title 直线最小值查询（离散李超树）
 * @brief 插入直线，在预先登记的横坐标处查询所有直线的最小值。
 * @note 所有查询横坐标提前收集；query 的 x 是 xs 中的下标，根参数 (1,0,xs.size()-1)。返回最小值 i128；至少加一条线后查询。
 * @time O(log n) 每次
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct LiChao {
	struct Line {
		i64 k = 0, b = 0;
		i128 get(i64 x) const {
			return i128(k) * x + b;
		}
	};
	vector<i64> xs;
	vector<Line> tr;
	vector<bool> has;
	LiChao(vector<i64> a) : xs(a) {
		sort(xs.begin(), xs.end());
		xs.erase(unique(xs.begin(), xs.end()),
		         xs.end());
		assert(!xs.empty());
		tr.resize(xs.size() * 4);
		has.resize(tr.size());
	}
	void add(Line v, int p, int l, int r) {
		if (!has[p]) {
			tr[p] = v;
			has[p] = true;
			return;
		}
		int m = (l + r) / 2;
		bool left = v.get(xs[l]) < tr[p].get(xs[l]);
		bool mid = v.get(xs[m]) < tr[p].get(xs[m]);
		if (mid) swap(v, tr[p]);
		if (l == r) return;
		if (left != mid)
			add(v, p * 2, l, m);
		else
			add(v, p * 2 + 1, m + 1, r);
	}
	void add(i64 k, i64 b) {
		add({ k, b }, 1, 0, xs.size() - 1);
	}
	i128 query(int x, int p, int l, int r) const {
		i128 ans =
		    has[p] ? tr[p].get(xs[x]) : i128(1) << 126;
		if (l == r) return ans;
		int m = (l + r) / 2;
		return min(ans, x <= m ? query(x, p * 2, l, m)
		                       : query(x, p * 2 + 1,
		                               m + 1, r));
	}
};
