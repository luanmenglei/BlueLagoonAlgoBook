/**
 * @title 多项式除法／取模／多点求值／插值
 * @brief 提供多项式除法／余数、批量点值求值及点值插值。
 * @note 系数归一，低次在前；除数非零。ProductTree 支持空点集，求值可重复点，插值要求点两两不同。所有中间NTT长度满足上限。
 * @time 除法 O(n log n)；多点操作 O(n log^2 n)
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
void trim(Vec &a) {
	while (!a.empty() && !a.back())
		a.pop_back();
}
pair<Vec, Vec> divmod(Vec a, Vec b) {
	trim(a);
	trim(b);
	assert(!b.empty());
	if (a.size() < b.size()) return { {}, a };
	int k = a.size() - b.size() + 1;
	Vec x = a, y = b;
	reverse(x.begin(), x.end());
	reverse(y.begin(), y.end());
	Vec q = cut(conv(cut(x, k), inverse(y, k)), k);
	reverse(q.begin(), q.end());
	Vec t = conv(b, q);
	a.resize(b.size() - 1);
	for (int i = 0; i < (int)a.size(); i ++)
		a[i] = (a[i] - t[i] + mod) % mod;
	trim(a);
	return { q, a };
}
struct ProductTree {
	int n;
	vector<Vec> tr;
	Vec xs;
	ProductTree(const Vec &x)
	    : n(x.size()), tr(4 * n + 4), xs(x) {
		if (n) build(1, 0, n);
	}
	void build(int p, int l, int r) {
		if (r - l == 1) {
			tr[p] = { (mod - xs[l]) % mod, 1 };
			return;
		}
		int m = (l + r) / 2;
		build(p * 2, l, m);
		build(p * 2 + 1, m, r);
		tr[p] = conv(tr[p * 2], tr[p * 2 + 1]);
	}
	void eval(int p, int l, int r, const Vec &a,
	          Vec &ans) const {
		Vec b = divmod(a, tr[p]).second;
		if (r - l == 1) {
			ans[l] = b.empty() ? 0 : b[0];
			return;
		}
		int m = (l + r) / 2;
		eval(p * 2, l, m, b, ans);
		eval(p * 2 + 1, m, r, b, ans);
	}
	Vec evaluate(const Vec &a) const {
		Vec ans(n);
		if (n) eval(1, 0, n, a, ans);
		return ans;
	}
	Vec combine(int p, int l, int r,
	            const Vec &v) const {
		if (r - l == 1) return { v[l] };
		int m = (l + r) / 2;
		Vec a = conv(combine(p * 2, l, m, v),
		             tr[p * 2 + 1]);
		Vec b = conv(combine(p * 2 + 1, m, r, v),
		             tr[p * 2]);
		a.resize(max(a.size(), b.size()));
		for (int i = 0; i < (int)b.size(); i ++)
			a[i] = (a[i] + b[i]) % mod;
		return a;
	}
	Vec interpolate(const Vec &ys) const {
		assert(ys.size() == xs.size());
		if (!n) return {};
		Vec d = evaluate(derivative(tr[1])), v(n);
		for (int i = 0; i < n; i ++) {
			assert(d[i]);
			v[i] =
			    i64(ys[i]) * power(d[i], mod - 2) % mod;
		}
		return cut(combine(1, 0, n, v), n);
	}
};
} // namespace poly
