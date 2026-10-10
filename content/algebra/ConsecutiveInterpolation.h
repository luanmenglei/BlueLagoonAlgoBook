/**
 * @title 连续点值插值
 * @brief 由连续整数点上的值，求低次多项式在任意点的值。
 * @note 给定次数<=n的多项式在0..n的点值，求 x 处值；模998244353，n<mod，输入点值归一。
 * @time O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

int interpolate_consecutive(const vector<int> &y,
                            i64 x) {
	const int mod = 998244353;
	auto pw = [&](i64 a, int b) {
		i64 z = 1;
		for (; b; b >>= 1, a = a * a % mod)
			if (b & 1) z = z * a % mod;
		return int(z);
	};
	int n = y.size() - 1;
	assert(n >= 0 && n < mod);
	x = (x % mod + mod) % mod;
	if (x <= n) return y[x];
	vector<i64> pre(n + 2, 1), suf(n + 2, 1),
	    fac(n + 1, 1), inv(n + 1);
	for (int i = 0; i <= n; i ++)
		pre[i + 1] = pre[i] * (x - i + mod) % mod;
	for (int i = n; i >= 0; i --)
		suf[i] = suf[i + 1] * (x - i + mod) % mod;
	for (int i = 1; i <= n; i ++)
		fac[i] = fac[i - 1] * i % mod;
	inv[n] = pw(fac[n], mod - 2);
	for (int i = n; i; i --)
		inv[i - 1] = inv[i] * i % mod;
	i64 ans = 0;
	for (int i = 0; i <= n; i ++) {
		i64 v = i64(y[i]) * pre[i] % mod * suf[i + 1] %
		        mod * inv[i] % mod * inv[n - i] % mod;
		ans = (ans + ((n - i) & 1 ? mod - v : v)) % mod;
	}
	return ans;
}
