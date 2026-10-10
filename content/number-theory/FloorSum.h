/**
 * @title 整除取整求和（类欧几里得）
 * @brief 求线性函数除以模数后向下取整的前 n 项之和。
 * @note 求 $\sum_{i=0}^{n-1}\lfloor(ai+b)/m\rfloor$，n>=0、m>0；支持负 a,b，结果 i128，要求最终结果在范围内。
 * @time O(log m)
 * @depends contest/Base.h, contest/Int128.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../contest/Int128.h"

i128 floor_sum(i64 n, i64 m, i64 a, i64 b) {
	assert(n >= 0 && m > 0);
	i64 qa = floor_div(a, m), qb = floor_div(b, m);
	i128 ans =
	    i128(n) * (n - 1) / 2 * qa + i128(n) * qb;
	a = i128(a) - i128(qa) * m;
	b = i128(b) - i128(qb) * m;
	while (true) {
		if (a >= m)
			ans += i128(n) * (n - 1) / 2 * (a / m),
			    a %= m;
		if (b >= m) ans += i128(n) * (b / m), b %= m;
		i128 y = i128(a) * n + b;
		if (y < m) return ans;
		n = y / m;
		b = y % m;
		swap(m, a);
	}
}
