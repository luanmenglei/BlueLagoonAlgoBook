/**
 * @title 原根与模素数平方根
 * @brief 求素数模的原根，以及模素数二次剩余的一个平方根。
 * @note p 为素数；Tonelli-Shanks 支持 p<=LLONG_MAX。返回任一根或 nullopt，另一个根为 (p-x)%p；原根先分解 p-1。
 * @time 平方根 O(log^2 p)，原根依赖分解
 * @depends contest/Base.h, number-theory/Factor.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Factor.h"

u64 primitive_root(u64 p) {
	assert(is_prime(p));
	if (p == 2) return 1;
	vector<u64> a;
	factor(p - 1, a);
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());
	for (u64 g = 2;; g ++) {
		bool ok = true;
		for (u64 q : a)
			if (pow_mod(g, (p - 1) / q, p) == 1) {
				ok = false;
				break;
			}
		if (ok) return g;
	}
}
optional<u64> mod_sqrt(u64 a, u64 p) {
	assert(p <= LLONG_MAX && is_prime(p));
	a %= p;
	if (!a || p == 2) return a;
	if (pow_mod(a, (p - 1) / 2, p) != 1) return nullopt;
	if (p % 4 == 3) return pow_mod(a, (p + 1) / 4, p);
	u64 q = p - 1, z = 2;
	int s = __builtin_ctzll(q);
	q >>= s;
	while (pow_mod(z, (p - 1) / 2, p) == 1)
		z ++;
	u64 c = pow_mod(z, q, p),
	    x = pow_mod(a, (q + 1) / 2, p),
	    t = pow_mod(a, q, p);
	while (t != 1) {
		int i = 0;
		u64 v = t;
		while (v != 1)
			v = mul_mod(v, v, p), i ++;
		u64 b = pow_mod(c, 1ULL << (s - i - 1), p);
		x = mul_mod(x, b, p);
		c = mul_mod(b, b, p);
		t = mul_mod(t, c, p);
		s = i;
	}
	return x;
}
