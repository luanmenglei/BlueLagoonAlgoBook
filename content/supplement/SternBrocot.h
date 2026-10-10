/**
 * @title 区间内分母最小的分数
 * @brief 求严格位于给定非负有理数区间内、分母最小的分数。
 * @note 输入非负有理数 L<R，分母为正，均用 i64；求严格位于 (L,R) 内、分母最小（并列分子最小）的分数。分子／分母越过 i64 时返回 nullopt；内部用
 * i128。
 * @usage `simplest_fraction(1,3,1,2)` 得 (2,5)，端点均不允许。
 * @time O(log M) 次整除，M 为结果量级
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

optional<pair<i64, i64>>
simplest_fraction(i64 a, i64 b, i64 c, i64 d) {
	assert(a >= 0 && c >= 0 && b > 0 && d > 0 &&
	       i128(a) * d < i128(c) * b);
	i128 l = 0, q = 1, r = 1, s = 0;
	while (true) {
		i128 x = l + r, y = q + s;
		if (x > LLONG_MAX || y > LLONG_MAX)
			return nullopt;
		if (x * b <= i128(a) * y) {
			i128 k = (i128(a) * q - i128(b) * l) /
			         (i128(b) * r - i128(a) * s);
			l += k * r;
			q += k * s;
		} else if (x * d >= i128(c) * y) {
			i128 k = (i128(d) * r - i128(c) * s) /
			         (i128(c) * q - i128(d) * l);
			r += k * l;
			s += k * q;
		} else
			return pair<i64, i64>{ (i64)x, (i64)y };
	}
}
