/**
 * @title 形式幂级数平方根
 * @brief 计算模素数形式幂级数的一支平方根，并判断是否存在。
 * @note 模998244353，支持前导零；不存在平方根返回 nullopt，返回任意一支模 $x^n$ 根。依赖
 * Tonelli-Shanks；除移位后的可确定部分外，高次项按零延拓计算。
 * @time O(n log n)
 * @depends contest/Base.h, algebra/FPS.h, number-theory/PrimitiveRoot.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/FPS.h"
#include "../number-theory/PrimitiveRoot.h"

namespace poly {
optional<Vec> fps_sqrt(const Vec &a, int n) {
	assert(n >= 1);
	int l = 0;
	while (l < (int)a.size() && !a[l])
		l ++;
	if (l == (int)a.size() || l >= n) return Vec(n);
	if (l & 1) return nullopt;
	auto z = mod_sqrt(a[l], mod);
	if (!z) return nullopt;
	int shift = l / 2, k = n - shift;
	Vec c(a.begin() + l, a.end()), b = { int(*z) };
	for (int m = 2; m / 2 < k; m *= 2) {
		int t = min(m, k);
		Vec d = cut(conv(cut(c, t), inverse(b, t)), t);
		b.resize(t);
		for (int i = 0; i < t; i ++)
			b[i] = i64((b[i] + d[i]) % mod) *
			       ((mod + 1) / 2) % mod;
	}
	b = cut(b, k);
	b.insert(b.begin(), shift, 0);
	return b;
}
} // namespace poly
