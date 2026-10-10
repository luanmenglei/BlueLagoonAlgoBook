/**
 * @title 树状数组／前缀和／第 k 项
 * @brief 支持单点加、前缀和，以及按累计权值查找第 k 项。
 * @note 1-based；add 要求 1 <= x <= n。kth 要求每个位置的权值非负、1 <= k <= sum(n)。
 * @time O(log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Fenwick {
	int n;
	vector<i64> tr;
	Fenwick(int n) : n(n), tr(n + 1) {}
	void add(int x, i64 v) {
		for (; x <= n; x += x & -x)
			tr[x] += v;
	}
	i64 sum(int x) const {
		i64 ans = 0;
		for (; x; x -= x & -x)
			ans += tr[x];
		return ans;
	}
	int kth(i64 k) const {
		int x = 0;
		for (int d = 1 << __lg(max(1, n)); d; d >>= 1)
			if (x + d <= n && tr[x + d] < k)
				k -= tr[x += d];
		return x + 1;
	}
};
