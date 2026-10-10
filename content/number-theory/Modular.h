/**
 * @title 模运算／扩欧／扩展 CRT
 * @brief 提供模运算、扩展欧几里得、模逆与非互质同余方程合并。
 * @note modulus 正数；CRT 合并 x=a (mod m)、x=b (mod n)，失败 nullopt，返回最小非负解与 lcm；要求
 * lcm<=LLONG_MAX。inverse 不存在返回-1。
 * @time O(log modulus)；乘法 O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

u64 mul_mod(u64 a, u64 b, u64 m) {
	return u128(a) * b % m;
}
u64 pow_mod(u64 a, u64 b, u64 m) {
	u64 ans = 1 % m;
	for (; b; b >>= 1, a = mul_mod(a, a, m))
		if (b & 1) ans = mul_mod(ans, a, m);
	return ans;
}
i128 exgcd(i128 a, i128 b, i128 &x, i128 &y) {
	if (!b) {
		x = 1;
		y = 0;
		return a;
	}
	i128 g = exgcd(b, a % b, y, x);
	y -= a / b * x;
	return g;
}
i64 inverse(i64 a, i64 m) {
	i128 x, y;
	i128 g = exgcd((i128(a) % m + m) % m, m, x, y);
	if (g != 1) return -1;
	return (x % m + m) % m;
}
optional<pair<i64, i64>> crt(i64 a, i64 m, i64 b,
                             i64 n) {
	assert(m > 0 && n > 0);
	a = (i128(a) % m + m) % m;
	b = (i128(b) % n + n) % n;
	i128 x, y, g = exgcd(m, n, x, y), d = i128(b) - a;
	if (d % g) return nullopt;
	i128 mod = i128(m) / g * n;
	assert(mod <= LLONG_MAX);
	i128 k = (d / g * x % (n / g) + n / g) % (n / g);
	i128 v = (a + i128(m) * k) % mod;
	return pair<i64, i64>{ (i64)v, (i64)mod };
}
