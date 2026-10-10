/**
 * @title 杜教筛 phi／mu 前缀和
 * @brief 求大范围欧拉函数之和与莫比乌斯函数之和。
 * @note n>=0 且 n<LLONG_MAX；建议 lim 约 $n^{2/3}$，根据内存调整。相同对象复用缓存；phi 前缀返回 i128，mu 前缀返回 i64。
 * @time 标准 cutoff 下约 O(n^(2/3))，map 另有对数因子
 * @depends contest/Base.h, number-theory/Sieve.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Sieve.h"

struct DuSieve {
	int lim;
	vector<i64> phi, mu;
	map<i64, i128> mp;
	map<i64, i64> mm;
	DuSieve(int lim)
	    : lim(lim), phi(lim + 1), mu(lim + 1) {
		Sieve s(lim);
		for (int i = 1; i <= lim; i ++)
			phi[i] = phi[i - 1] + s.phi[i],
			mu[i] = mu[i - 1] + s.mu[i];
	}
	i128 sum_phi(i64 n) {
		if (n <= lim) return phi[n];
		if (mp.count(n)) return mp[n];
		i128 ans = i128(n) * (n + 1) / 2;
		for (i64 l = 2, r; l <= n; l = r + 1) {
			r = n / (n / l);
			ans -= i128(r - l + 1) * sum_phi(n / l);
		}
		return mp[n] = ans;
	}
	i64 sum_mu(i64 n) {
		if (n <= lim) return mu[n];
		if (mm.count(n)) return mm[n];
		i64 ans = 1;
		for (i64 l = 2, r; l <= n; l = r + 1) {
			r = n / (n / l);
			ans -= (r - l + 1) * sum_mu(n / l);
		}
		return mm[n] = ans;
	}
};
