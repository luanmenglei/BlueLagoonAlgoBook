/**
 * @title 数值积分（自适应 Simpson）
 * @brief 用递归分段的 Simpson 公式近似计算有限区间定积分。
 * @note 平滑函数有限区间积分，eps>0。depth 是递归上限，达到上限时误差目标不保证；不适合未经处理的奇点、剧烈振荡或狭窄尖峰。
 * @time 依赖函数与目标误差，最坏 O(2^depth)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class F>
double integrate(F f, double l, double r,
                 double eps = 1e-8, int depth = 25) {
	auto area = [](double l, double r, double a,
	               double b, double c) {
		return (r - l) * (a + 4 * b + c) / 6;
	};
	auto rec = [&](auto self, double l, double r,
	               double a, double b, double c,
	               double s, double eps,
	               int dep) -> double {
		double m = (l + r) / 2, d = f((l + m) / 2),
		       e = f((m + r) / 2);
		double x = area(l, m, a, d, b),
		       y = area(m, r, b, e, c), z = x + y - s;
		if (!dep || abs(z) <= 15 * eps)
			return x + y + z / 15;
		return self(self, l, m, a, d, b, x, eps / 2,
		            dep - 1) +
		       self(self, m, r, b, e, c, y, eps / 2,
		            dep - 1);
	};
	double a = f(l), b = f((l + r) / 2), c = f(r);
	return rec(rec, l, r, a, b, c, area(l, r, a, b, c),
	           eps, depth);
}
