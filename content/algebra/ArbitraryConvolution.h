/**
 * @title 任意模数卷积
 * @brief 使用三模 NTT 与 CRT，计算指定正整数模数下的卷积。
 * @note mod 为正 int；输入系数在 [0,mod)。三模 CRT 恢复整数后取模，要求原始每项卷积系数 <
 * 三模乘积；变换长度$\le2^{24}$。常见长度与32位模数可满足此界，仍需检查。
 * @time O(n log n)
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

template <int P, int G>
vector<int> ntt_conv(vector<int> a, vector<int> b) {
	auto pw = [](i64 x, int e) {
		i64 z = 1;
		for (; e; e >>= 1, x = x * x % P)
			if (e & 1) z = z * x % P;
		return int(z);
	};
	int m = a.size() + b.size() - 1, n = 1;
	while (n < m)
		n <<= 1;
	assert((P - 1) % n == 0);
	a.resize(n);
	b.resize(n);
	auto transform = [&](vector<int> &v, bool inv) {
		for (int i = 1, j = 0; i < n; i ++) {
			int k = n >> 1;
			for (; j & k; k >>= 1)
				j ^= k;
			j ^= k;
			if (i < j) swap(v[i], v[j]);
		}
		for (int k = 2; k <= n; k <<= 1) {
			int z = pw(G, (P - 1) / k);
			if (inv) z = pw(z, P - 2);
			for (int l = 0; l < n; l += k)
				for (int j = 0, w = 1; j < k / 2;
				     j ++, w = i64(w) * z % P) {
					int x = v[l + j],
					    y = i64(v[l + j + k / 2]) * w %
					        P;
					v[l + j] = (i64(x) + y) % P;
					v[l + j + k / 2] =
					    (i64(x) - y + P) % P;
				}
		}
		if (inv) {
			int z = pw(n, P - 2);
			for (int &x : v)
				x = i64(x) * z % P;
		}
	};
	for (int &x : a)
		x %= P;
	for (int &x : b)
		x %= P;
	transform(a, false);
	transform(b, false);
	for (int i = 0; i < n; i ++)
		a[i] = i64(a[i]) * b[i] % P;
	transform(a, true);
	a.resize(m);
	return a;
}
vector<int> conv_mod(const vector<int> &a,
                     const vector<int> &b, int mod) {
	if (a.empty() || b.empty()) return {};
	const i64 p = 167772161, q = 469762049,
	          r = 1224736769;
	auto x = ntt_conv<p, 3>(a, b),
	     y = ntt_conv<q, 3>(a, b),
	     z = ntt_conv<r, 3>(a, b);
	i64 ip = inverse(p, q), ipq = inverse(p * q % r, r);
	vector<int> ans(x.size());
	for (int i = 0; i < (int)x.size(); i ++) {
		i64 t = (y[i] - x[i] + q) % q * ip % q;
		i128 v = x[i] + i128(p) * t;
		i64 u = (z[i] - (i64)(v % r) + r) % r * ipq % r;
		ans[i] = (v + i128(p) * q * u) % mod;
	}
	return ans;
}
