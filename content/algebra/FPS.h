/**
 * @title 形式幂级数求逆／ln／exp／幂
 * @brief 提供求导、积分、乘法逆、对数、指数与非负整数次幂。
 * @note 模998244353、n>=1；inv 要求常数项非零；ln 常数项1，exp 常数项0，pow 指数非负。全部返回模 $x^n$
 * 结果；精度必须小于模数且所有中间卷积满足NTT长度界。
 * @time O(n log n)（当前 ln/exp Newton 实现常数较大）
 * @depends contest/Base.h, algebra/NTT.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/NTT.h"

namespace poly {
Vec cut(Vec a, int n) {
	a.resize(n);
	return a;
}
Vec derivative(const Vec &a) {
	Vec b(max(0, (int)a.size() - 1));
	for (int i = 1; i < (int)a.size(); i ++)
		b[i - 1] = i64(a[i]) * i % mod;
	return b;
}
Vec integral(const Vec &a) {
	Vec b(a.size() + 1), inv(a.size() + 1);
	if (a.size()) inv[1] = 1;
	for (int i = 2; i <= (int)a.size(); i ++)
		inv[i] =
		    mod - i64(mod / i) * inv[mod % i] % mod;
	for (int i = 0; i < (int)a.size(); i ++)
		b[i + 1] = i64(a[i]) * inv[i + 1] % mod;
	return b;
}
Vec inverse(const Vec &a, int n) {
	assert(n >= 1 && !a.empty() && a[0]);
	Vec b = { power(a[0], mod - 2) };
	for (int m = 2; m / 2 < n; m *= 2) {
		int k = min(m, n);
		Vec c = conv(cut(a, k), b);
		c.resize(k);
		for (int &x : c)
			x = x ? mod - x : 0;
		c[0] = (c[0] + 2) % mod;
		b = cut(conv(b, c), k);
	}
	return cut(b, n);
}
Vec logarithm(const Vec &a, int n) {
	assert(!a.empty() && a[0] == 1 && n >= 1);
	if (n == 1) return { 0 };
	return cut(
	    integral(cut(conv(derivative(a), inverse(a, n)),
	                 n - 1)),
	    n);
}
Vec exponential(const Vec &a, int n) {
	assert(n >= 1 && (a.empty() || a[0] == 0));
	Vec b = { 1 };
	for (int m = 2; m / 2 < n; m *= 2) {
		int k = min(m, n);
		Vec c = cut(a, k), d = logarithm(b, k);
		for (int i = 0; i < k; i ++)
			c[i] = (c[i] - d[i] + mod) % mod;
		c[0] ++;
		b = cut(conv(b, c), k);
	}
	return cut(b, n);
}
Vec fps_power(const Vec &a, u64 e, int n) {
	assert(n >= 1);
	if (!e) {
		Vec b(n);
		b[0] = 1;
		return b;
	}
	int l = 0;
	while (l < (int)a.size() && !a[l])
		l ++;
	if (l == (int)a.size() || u128(l) * e >= (u64)n)
		return Vec(n);
	int shift = u128(l) * e, k = n - shift;
	Vec b(a.begin() + l, a.end());
	int iv = power(b[0], mod - 2),
	    c = power(b[0], e % (mod - 1));
	for (int &x : b)
		x = i64(x) * iv % mod;
	b = logarithm(b, k);
	for (int &x : b)
		x = i64(x) * (e % mod) % mod;
	b = exponential(b, k);
	for (int &x : b)
		x = i64(x) * c % mod;
	b.insert(b.begin(), shift, 0);
	return b;
}
} // namespace poly
