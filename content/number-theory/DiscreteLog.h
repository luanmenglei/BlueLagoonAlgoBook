/**
 * @title 离散对数（BSGS／扩展 BSGS）
 * @brief 求模指数方程的最小非负指数，支持底数与模数不互质。
 * @note 求最小 $x\ge0$ 使 $a^x\equiv b\pmod m$，m<=LLONG_MAX；支持不互质，$0^0$ 按1处理。$\sqrt m$ 内存是实际限制。
 * @time 期望 O(sqrt(m)) 时间／空间
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

optional<u64> bsgs(u64 a, u64 b, u64 m) {
	assert(m >= 1 && m <= LLONG_MAX);
	a %= m;
	b %= m;
	if (m == 1 || b == 1) return 0;
	u64 cnt = 0, k = 1, g;
	while ((g = gcd(a, m)) > 1) {
		if (b == k) return cnt;
		if (b % g) return nullopt;
		b /= g;
		m /= g;
		cnt ++;
		k = mul_mod(k, a / g, m);
		if (m == 1) return cnt;
	}
	b = mul_mod(b, inverse(k, m), m);
	u64 s = sqrt((long double)m);
	while (u128(s) * s < m)
		s ++;
	unordered_map<u64, u64> mp;
	u64 v = 1;
	for (u64 j = 0; j < s; j ++) {
		mp.emplace(v, j);
		v = mul_mod(v, a, m);
	}
	u64 step = inverse(v, m);
	for (u64 i = 0; i <= s; i ++) {
		if (mp.count(b)) return i * s + mp[b] + cnt;
		b = mul_mod(b, step, m);
	}
	return nullopt;
}
