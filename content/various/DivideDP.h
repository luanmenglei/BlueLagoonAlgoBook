/**
 * @title 决策单调 DP 的分治优化
 * @brief 在最优决策单调的前提下，加速一层 DP 的转移。
 * @note dp[i]=min(0<=j<i) get(i,j)；get 通常含上一层 dp[j]。必须证明最左最优决策随 i 不减，且 get 不依赖当前尚未求出的 dp。返回
 * dp[0]=0；不可达用小于 LLONG_MAX 的有限 INF。
 * @time 每层 O(n log n) oracle调用
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class F> vector<i64> divide_dp(int n, F get) {
	vector<i64> dp(n + 1);
	auto rec = [&](auto self, int l, int r, int lo,
	               int hi) -> void {
		if (l > r) return;
		int m = (l + r) / 2, k = -1;
		i64 best = LLONG_MAX;
		for (int j = lo; j <= min(hi, m - 1); j ++) {
			i64 v = get(m, j);
			if (v < best) best = v, k = j;
		}
		assert(k != -1);
		dp[m] = best;
		self(self, l, m - 1, lo, k);
		self(self, m + 1, r, k, hi);
	};
	if (n) rec(rec, 1, n, 0, n - 1);
	return dp;
}
