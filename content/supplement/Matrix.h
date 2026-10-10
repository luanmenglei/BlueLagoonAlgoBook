/**
 * @title 模矩阵乘幂与实数／模矩阵求逆
 * @brief 计算模矩阵乘法／快速幂，以及实数／素数模矩阵逆。
 * @note 模运算要求 1<mod<=INT_MAX，系数归一。inverse_mod 要求素数；奇异返回 nullopt。实数版 eps 按量纲调整；空矩阵逆为空。乘幂只接受方阵。
 * @usage `matrix::power({{1,1},{1,0}},10,mod)` 的[0][1]为F(10)；`matrix::inverse_mod(a,mod)` 先检查
 * optional。
 * @time 乘法／逆 O(n³)，幂 O(n³ log k)
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

namespace matrix {
using Mat = vector<vector<int>>;
Mat identity(int n, int mod) {
	Mat a(n, vector<int>(n));
	for (int i = 0; i < n; i ++)
		a[i][i] = 1 % mod;
	return a;
}
Mat multiply(const Mat &a, const Mat &b, int mod) {
	int n = a.size(), k = b.size(),
	    m = k ? b[0].size() : 0;
	assert(!n || (int)a[0].size() == k);
	Mat c(n, vector<int>(m));
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < k; j ++)
			if (a[i][j])
				for (int t = 0; t < m; t ++)
					c[i][t] = (c[i][t] +
					           i64(a[i][j]) * b[j][t]) %
					          mod;
	return c;
}
Mat power(Mat a, u64 k, int mod) {
	int n = a.size();
	for (auto &r : a)
		assert((int)r.size() == n);
	Mat b = identity(n, mod);
	for (; k; k >>= 1, a = multiply(a, a, mod))
		if (k & 1) b = multiply(b, a, mod);
	return b;
}
optional<Mat> inverse_mod(Mat a, int mod) {
	int n = a.size();
	Mat b = identity(n, mod);
	for (int i = 0; i < n; i ++) {
		int p = i;
		while (p < n && !a[p][i])
			p ++;
		if (p == n) return nullopt;
		swap(a[p], a[i]);
		swap(b[p], b[i]);
		int x = pow_mod(a[i][i], mod - 2, mod);
		for (int j = 0; j < n; j ++)
			a[i][j] = i64(a[i][j]) * x % mod,
			b[i][j] = i64(b[i][j]) * x % mod;
		for (int k = 0; k < n; k ++)
			if (k != i) {
				int x = a[k][i];
				for (int j = 0; j < n; j ++) {
					a[k][j] =
					    (a[k][j] -
					     i64(x) * a[i][j] % mod + mod) %
					    mod;
					b[k][j] =
					    (b[k][j] -
					     i64(x) * b[i][j] % mod + mod) %
					    mod;
				}
			}
	}
	return b;
}
optional<vector<vector<double>>>
inverse_real(vector<vector<double>> a,
             double eps = 1e-9) {
	int n = a.size();
	vector<vector<double>> b(n, vector<double>(n));
	for (int i = 0; i < n; i ++)
		b[i][i] = 1;
	for (int i = 0; i < n; i ++) {
		int p = i;
		for (int k = i; k < n; k ++)
			if (abs(a[k][i]) > abs(a[p][i])) p = k;
		if (abs(a[p][i]) <= eps) return nullopt;
		swap(a[p], a[i]);
		swap(b[p], b[i]);
		double x = a[i][i];
		for (int j = 0; j < n; j ++)
			a[i][j] /= x, b[i][j] /= x;
		for (int k = 0; k < n; k ++)
			if (k != i) {
				double x = a[k][i];
				for (int j = 0; j < n; j ++)
					a[k][j] -= x * a[i][j],
					    b[k][j] -= x * b[i][j];
			}
	}
	return b;
}
} // namespace matrix
