/**
 * @title 实系数多项式乘法／卷积（FFT）
 * @brief 作复数快速傅里叶变换，近似计算实系数多项式卷积。
 * @note fft 长度为正2幂；inverse=true 逆变换并除以 n。convolution 支持实系数，浮点误差依数值规模而变；整数精确计算优先 NTT／CRT。
 * @usage `real_fft::convolution({1,2},{3,4})` 约为 {3,10,8}；不要直接强制转整数，应按精度条件选择 llround 或 NTT。
 * @time O(n log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

namespace real_fft {
using C = complex<double>;
void fft(vector<C> &a, bool inverse = false) {
	int n = a.size();
	assert(n > 0 && (n & (n - 1)) == 0);
	for (int i = 1, j = 0; i < n; i ++) {
		int b = n >> 1;
		for (; j & b; b >>= 1)
			j ^= b;
		j ^= b;
		if (i < j) swap(a[i], a[j]);
	}
	for (int k = 2; k <= n; k <<= 1) {
		C z = polar(1.0,
		            (inverse ? -2 : 2) * acos(-1) / k);
		for (int i = 0; i < n; i += k) {
			C w = 1;
			for (int j = 0; j < k / 2; j ++, w *= z) {
				C u = a[i + j],
				  v = w * a[i + j + k / 2];
				a[i + j] = u + v;
				a[i + j + k / 2] = u - v;
			}
		}
	}
	if (inverse)
		for (C &x : a)
			x /= n;
}
vector<double> convolution(const vector<double> &a,
                           const vector<double> &b) {
	if (a.empty() || b.empty()) return {};
	int m = a.size() + b.size() - 1, n = 1;
	while (n < m)
		n <<= 1;
	vector<C> x(n), y(n);
	copy(a.begin(), a.end(), x.begin());
	copy(b.begin(), b.end(), y.begin());
	fft(x);
	fft(y);
	for (int i = 0; i < n; i ++)
		x[i] *= y[i];
	fft(x, true);
	vector<double> out(m);
	for (int i = 0; i < m; i ++)
		out[i] = x[i].real();
	return out;
}
} // namespace real_fft
