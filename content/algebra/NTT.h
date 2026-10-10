/**
 * @title 多项式乘法／卷积（NTT）
 * @brief 在模 998244353 下作数论变换，快速计算多项式乘法。
 * @note 模数998244353、原根3；系数从常数项开始，归一到 [0,mod)。NTT 长度为2幂且$\le2^{23}$；结果卷积长度也需满足变换上限。
 * @time O(n log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source NTT structure informed by ecnerwala/cp-book src/fft; BlueLagoon C++20 implementation
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

namespace poly {
const int mod = 998244353;
using Vec = vector<int>;
int power(int a, i64 b) {
	int ans = 1;
	for (; b; b >>= 1, a = i64(a) * a % mod)
		if (b & 1) ans = i64(ans) * a % mod;
	return ans;
}
void ntt(Vec &a, bool inv) {
	int n = a.size();
	assert(n && (n & (n - 1)) == 0 && n <= (1 << 23));
	for (int i = 1, j = 0; i < n; i ++) {
		int b = n >> 1;
		for (; j & b; b >>= 1)
			j ^= b;
		j ^= b;
		if (i < j) swap(a[i], a[j]);
	}
	for (int len = 2; len <= n; len <<= 1) {
		int w0 = power(3, (mod - 1) / len);
		if (inv) w0 = power(w0, mod - 2);
		for (int l = 0; l < n; l += len) {
			int w = 1;
			for (int j = 0; j < len / 2; j ++) {
				int u = a[l + j],
				    v = i64(a[l + j + len / 2]) * w %
				        mod;
				a[l + j] = (u + v) % mod;
				a[l + j + len / 2] =
				    (u - v + mod) % mod;
				w = i64(w) * w0 % mod;
			}
		}
	}
	if (inv) {
		int z = power(n, mod - 2);
		for (int &x : a)
			x = i64(x) * z % mod;
	}
}
Vec conv(Vec a, Vec b) {
	if (a.empty() || b.empty()) return {};
	int m = a.size() + b.size() - 1, n = 1;
	while (n < m)
		n <<= 1;
	a.resize(n);
	b.resize(n);
	ntt(a, false);
	ntt(b, false);
	for (int i = 0; i < n; i ++)
		a[i] = i64(a[i]) * b[i] % mod;
	ntt(a, true);
	a.resize(m);
	return a;
}
} // namespace poly
