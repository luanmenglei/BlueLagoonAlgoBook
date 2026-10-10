/**
 * @title 线性递推猜测／第 k 项（BM）
 * @brief 由已知项猜测常系数线性递推，再求递推的第 k 项。
 * @note 模998244353；递推 $s_i=\sum_{j=1}^{d}c_{j-1}s_{i-j}$。BM 至少给2d项才能保证恢复真实d阶递推，有限样本拟合不等于证明。k
 * 从0起，空递推视后续为零。
 * @time BM O(L^2)，第k项 O(d^2 log k)
 * @depends contest/Base.h, algebra/NTT.h
 * @author BlueLagoon
 * @source Berlekamp-Massey / Kitamasa; reference cp-book and koosaga; BlueLagoon
 * implementation
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/NTT.h"

namespace poly {
Vec berlekamp_massey(const Vec &s) {
	Vec c = { 1 }, b = { 1 };
	int l = 0, m = 1, last = 1;
	for (int i = 0; i < (int)s.size(); i ++) {
		int d = s[i];
		for (int j = 1; j <= l; j ++)
			d = (d + i64(c[j]) * s[i - j]) % mod;
		if (!d) {
			m ++;
			continue;
		}
		Vec old = c;
		int coef = i64(d) * power(last, mod - 2) % mod;
		c.resize(max(c.size(), b.size() + m));
		for (int j = 0; j < (int)b.size(); j ++)
			c[j + m] = (c[j + m] -
			            i64(coef) * b[j] % mod + mod) %
			           mod;
		if (2 * l <= i)
			l = i + 1 - l, b = old, last = d, m = 1;
		else
			m ++;
	}
	c.resize(l + 1);
	c.erase(c.begin());
	for (int &x : c)
		x = x ? mod - x : 0;
	return c;
}
int linear_rec(Vec init, const Vec &c, u64 k) {
	if (k < init.size()) return init[k];
	int n = c.size();
	if (!n) return 0;
	assert(init.size() >= (size_t)n);
	auto mul = [&](Vec a, Vec b) {
		Vec d(2 * n - 1);
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < n; j ++)
				d[i + j] =
				    (d[i + j] + i64(a[i]) * b[j]) % mod;
		for (int i = 2 * n - 2; i >= n; i --)
			for (int j = 1; j <= n; j ++)
				d[i - j] =
				    (d[i - j] + i64(d[i]) * c[j - 1]) %
				    mod;
		d.resize(n);
		return d;
	};
	Vec a(n), b(n);
	a[0] = 1;
	if (n == 1)
		b[0] = c[0];
	else
		b[1] = 1;
	for (; k; k >>= 1, b = mul(b, b))
		if (k & 1) a = mul(a, b);
	int ans = 0;
	for (int i = 0; i < n; i ++)
		ans = (ans + i64(a[i]) * init[i]) % mod;
	return ans;
}
} // namespace poly
