/**
 * @title 区间 DP 四边形不等式优化（Knuth）
 * @brief 用相邻区间最优决策界，缩小区间 DP 的分割点枚举范围。
 * @note 半开区间 dp[l][r]=min(l<k<r) dp[l][k]+dp[k][r]+cost(l,r)，单点代价0。充分条件：cost
 * 区间包含单调且满足四边形不等式，保证 opt[l][r-1]<=opt[l][r]<=opt[l+1][r]；和不得溢出。
 * @time O(n^2) 时间／空间
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class F> i64 knuth_dp(int n, F cost) {
	vector<vector<i64>> dp(n + 1, vector<i64>(n + 1));
	vector<vector<int>> opt(n + 1, vector<int>(n + 1));
	for (int i = 0; i < n; i ++)
		opt[i][i + 1] = i + 1;
	for (int len = 2; len <= n; len ++)
		for (int l = 0; l + len <= n; l ++) {
			int r = l + len;
			dp[l][r] = LLONG_MAX;
			int lo = max(l + 1, opt[l][r - 1]),
			    hi = min(r - 1, opt[l + 1][r]);
			for (int k = lo; k <= hi; k ++) {
				i64 v =
				    dp[l][k] + dp[k][r] + cost(l, r);
				if (v < dp[l][r])
					dp[l][r] = v, opt[l][r] = k;
			}
		}
	return dp[0][n];
}
