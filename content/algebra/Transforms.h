/**
 * @title 位运算卷积／子集和／子集卷积
 * @brief 计算 XOR／OR／AND 卷积、子集和变换及不交子集卷积。
 * @note 数组长度$2^k$，系数模998244353。type=0/1/2 对应 XOR/OR/AND；SOS 为子集和，inv 为反演；convolution 求
 * $\sum_{T\subseteq S}a[T]b[S\setminus T]$。
 * @time FWT/SOS O(k2^k)，子集卷积 O(k^2 2^k)，空间 O(k2^k)
 * @depends contest/Base.h, algebra/NTT.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/NTT.h"

namespace subset {
const int mod = 998244353;
void fwt(vector<int> &a, int type, bool inv = false) {
	int n = a.size();
	assert(n && (n & (n - 1)) == 0);
	for (int d = 1; d < n; d *= 2)
		for (int l = 0; l < n; l += 2 * d)
			for (int j = 0; j < d; j ++) {
				int &x = a[l + j], &y = a[l + j + d],
				    u = x, v = y;
				if (type == 0)
					x = (u + v) % mod,
					y = (u - v + mod) % mod;
				if (type == 1)
					y = (v + (inv ? mod - u : u)) % mod;
				if (type == 2)
					x = (u + (inv ? mod - v : v)) % mod;
			}
	if (type == 0 && inv) {
		int z = poly::power(n, mod - 2);
		for (int &x : a)
			x = i64(x) * z % mod;
	}
}
void sos(vector<int> &a, bool inv = false) {
	for (int b = 1; b < (int)a.size(); b *= 2)
		for (int s = 0; s < (int)a.size(); s ++)
			if (s & b)
				a[s] = (a[s] + (inv ? mod - a[s ^ b]
				                    : a[s ^ b])) %
				       mod;
}
vector<int> convolution(const vector<int> &a,
                        const vector<int> &b) {
	int n = a.size(), k = __lg(n);
	assert(n && (n & (n - 1)) == 0 &&
	       b.size() == a.size());
	vector<vector<int>> x(k + 1, vector<int>(n)), y = x,
	                                              z = x;
	for (int s = 0; s < n; s ++)
		x[__builtin_popcount(s)][s] = a[s],
		y[__builtin_popcount(s)][s] = b[s];
	for (int i = 0; i <= k; i ++)
		sos(x[i]), sos(y[i]);
	for (int i = 0; i <= k; i ++)
		for (int j = 0; j <= i; j ++)
			for (int s = 0; s < n; s ++)
				z[i][s] = (z[i][s] +
				           i64(x[j][s]) * y[i - j][s]) %
				          mod;
	for (int i = 0; i <= k; i ++)
		sos(z[i], true);
	vector<int> c(n);
	for (int s = 0; s < n; s ++)
		c[s] = z[__builtin_popcount(s)][s];
	return c;
}
} // namespace subset
