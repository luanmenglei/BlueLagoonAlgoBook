/**
 * @title 64 位判素／质因数分解
 * @brief 用 Miller-Rabin 判素，用 Pollard-Rho 分解 64 位无符号整数。
 * @note 支持完整 u64 范围，factor 要求 n>=1，输出未排序的质因子列表；判素基底对 64 位确定正确，Rho 耗时随机。
 * @time MR O(log n)；Rho 期望约 O(n^(1/4))
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

bool is_prime(u64 n) {
	if (n < 2) return false;
	for (u64 p :
	     { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 })
		if (n % p == 0) return n == p;
	u64 d = n - 1;
	int s = __builtin_ctzll(d);
	d >>= s;
	for (u64 a :
	     { 2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
	       9780504ULL, 1795265022ULL }) {
		if (a % n == 0) continue;
		u64 x = pow_mod(a % n, d, n);
		if (x == 1 || x == n - 1) continue;
		bool ok = false;
		for (int i = 1; i < s; i ++) {
			x = mul_mod(x, x, n);
			if (x == n - 1) {
				ok = true;
				break;
			}
		}
		if (!ok) return false;
	}
	return true;
}
u64 rho(u64 n) {
	if (n % 2 == 0) return 2;
	static mt19937_64 rng(712367);
	while (true) {
		u64 c = rng() % (n - 1) + 1,
		    x = rng() % (n - 2) + 2, y = x, d = 1;
		auto f = [&](u64 v) {
			return u64((u128(mul_mod(v, v, n)) + c) %
			           n);
		};
		while (d == 1) {
			x = f(x);
			y = f(f(y));
			d = gcd(x > y ? x - y : y - x, n);
		}
		if (d != n) return d;
	}
}
void factor(u64 n, vector<u64> &a) {
	if (n == 1) return;
	if (is_prime(n)) {
		a.push_back(n);
		return;
	}
	u64 p = rho(n);
	factor(p, a);
	factor(n / p, a);
}
