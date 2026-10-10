/**
 * @title 素数模离散 k 次根
 * @brief 求模素数幂方程的一个 k 次根，或判断无解。
 * @note p 为素数且 <=LLONG_MAX，k>=1，返回任意一个 x 满足 $x^k\equiv a\pmod p$；无解 nullopt。BSGS 的 sqrt(p)
 * 内存是实际限制；不是合数模开根。
 * @usage `kth_root(4,2,7)` 返回2或5；`kth_root(3,2,7)` 无解。
 * @time 期望 O(sqrt(p))，另含 p-1 分解
 * @depends contest/Base.h, number-theory/PrimitiveRoot.h, number-theory/DiscreteLog.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/PrimitiveRoot.h"
#include "../number-theory/DiscreteLog.h"

optional<u64> kth_root(u64 a, u64 k, u64 p) {
	assert(k >= 1 && p <= LLONG_MAX && is_prime(p));
	a %= p;
	if (a == 0 || p == 2) return a;
	u64 g = primitive_root(p), m = p - 1;
	auto z = bsgs(g, a, p);
	if (!z) return nullopt;
	u64 d = gcd(k, m);
	if (*z % d) return nullopt;
	m /= d;
	u64 t = m == 1
	            ? 0
	            : mul_mod(*z / d,
	                      inverse((k / d) % m, m), m);
	return pow_mod(g, t, p);
}
