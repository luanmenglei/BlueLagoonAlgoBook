/**
 * @title 形式幂级数复合／多项式平移
 * @brief 计算 f(g(x)) 的截断系数，以及多项式 f(x+c) 的系数。
 * @note 输入系数均归一到 [0,998244353)。模998244353，compose 返回 f(g) $\bmod x^n$，要求
 * g[0]=0；当前为分块版本，不冒充准线性实现。taylor_shift 返回 f(x+c)，长度小于模数，卷积满足 NTT 上限。
 * @usage `poly::compose({1,2,3},{0,1,1},4)` 得 {1,2,5,6}；`taylor_shift({1,2,3},1)` 得 {6,8,3}。
 * @time 复合 O(n²+n sqrt(n) log n)，空间 O(n sqrt(n))；平移 O(n log n)
 * @depends contest/Base.h, algebra/FPS.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/FPS.h"

namespace poly {
Vec compose(const Vec &f, const Vec &g, int n) {
	assert(n >= 0 && (g.empty() || g[0] == 0));
	if (!n) return {};
	Vec base(g.begin(),
	         g.begin() + min(n, (int)g.size()));
	int b = max(1, (int)sqrt(n));
	vector<Vec> pw(b + 1, Vec(n));
	pw[0][0] = 1;
	for (int j = 1; j <= b; j ++)
		pw[j] = cut(conv(pw[j - 1], base), n);
	Vec ans(n);
	int blocks = (min(n, (int)f.size()) + b - 1) / b;
	for (int t = blocks - 1; t >= 0; t --) {
		ans = cut(conv(ans, pw[b]), n);
		for (int j = 0;
		     j < b && t * b + j < min(n, (int)f.size());
		     j ++)
			for (int i = 0; i < n; i ++)
				ans[i] = (ans[i] + i64(f[t * b + j]) *
				                       pw[j][i]) %
				         mod;
	}
	return ans;
}
Vec taylor_shift(const Vec &a, int c) {
	int n = a.size();
	if (!n) return {};
	assert(n < mod);
	Vec fac(n, 1), inv(n, 1), u(n), v(n);
	for (int i = 1; i < n; i ++)
		fac[i] = i64(fac[i - 1]) * i % mod;
	inv[n - 1] = power(fac[n - 1], mod - 2);
	for (int i = n - 1; i; i --)
		inv[i - 1] = i64(inv[i]) * i % mod;
	int p = 1;
	for (int i = 0; i < n; i ++)
		u[n - 1 - i] = i64(a[i]) * fac[i] % mod,
		          v[i] = i64(p) * inv[i] % mod,
		          p = i64(p) * c % mod;
	Vec w = conv(u, v), out(n);
	for (int i = 0; i < n; i ++)
		out[i] = i64(w[n - 1 - i]) * inv[i] % mod;
	return out;
}
} // namespace poly
