/**
 * @title 正整数幂塔取模
 * @brief 求右结合正整数幂塔模任意正模数的值，支持不互质底数。
 * @note a 非空且每个底数>=1，mod 正且 <=LLONG_MAX。返回 a[0]^(a[1]^(...)) mod
 * mod；支持不互质，用扩展欧拉及精确截断判断。零底数未定义于此接口。
 * @usage `tower::tetration({2,3,2},1000)` 为512；括号为右结合：$2^{3^2}$。
 * @time 欧拉链 O(log mod) 层；每层含整数分解
 * @depends contest/Base.h, number-theory/Factor.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Factor.h"

namespace tower {
u64 phi(u64 m) {
	vector<u64> ps;
	factor(m, ps);
	sort(ps.begin(), ps.end());
	ps.erase(unique(ps.begin(), ps.end()), ps.end());
	for (u64 p : ps)
		m = m / p * (p - 1);
	return m;
}
u64 cap(const vector<u64> &a, int i, u64 lim) {
	if (lim <= 1) return lim;
	if (i == (int)a.size() - 1) return min(a[i], lim);
	if (a[i] == 1) return 1;
	u64 v = 1, need = 0;
	while (v < lim) {
		need ++;
		v = u128(v) * a[i] >= lim ? lim : v * a[i];
	}
	u64 e = cap(a, i + 1, need);
	v = 1;
	while (e -- && v < lim)
		v = u128(v) * a[i] >= lim ? lim : v * a[i];
	return v;
}
u64 calc(const vector<u64> &a, int i, u64 mod) {
	if (mod == 1) return 0;
	if (i == (int)a.size() - 1) return a[i] % mod;
	u64 p = phi(mod), e = calc(a, i + 1, p);
	if (cap(a, i + 1, p) >= p) e += p;
	return pow_mod(a[i] % mod, e, mod);
}
u64 tetration(const vector<u64> &a, u64 mod) {
	assert(!a.empty() && mod >= 1 && mod <= LLONG_MAX);
	for (u64 x : a)
		assert(x >= 1);
	return calc(a, 0, mod);
}
} // namespace tower
