/**
 * @title 奇模数连续模乘（Montgomery）
 * @brief 在 Montgomery
 * 域中连续乘法，避免每次乘法执行整数除法。
 * @note m 为奇数且 1<=m<2^32；R=2^32，域内表示为 xR mod
 * m。 encode 将普通数转入，decode 转回；mul/add/sub
 * 的参数必须是同一对象的域内数且小于 m。
 * 不可混用不同模数；域内的 1 是 encode(1)。reduce
 * 为内部过程，要求 t<mR。
 * @usage Montgomery32 z(998244353); auto
 * a=z.encode(123); auto r=z.decode(z.mul(a,a));
 * @time 构造、转换、单次运算 O(1)；pow O(log e)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; Montgomery REDC
 * and Newton inverse
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

struct Montgomery32 {
	// m；-m^(-1) mod R；R^2 mod m。
	uint32_t m, neg, r2;
	explicit Montgomery32(uint32_t mod) : m(mod) {
		assert(m & 1);
		uint32_t x = 1;
		for (int i = 0; i < 5; i ++)
			x *= 2 - m * x;
		neg = 0u - x;
		r2 = (u128(1) << 64) % m;
	}
	uint32_t reduce(u64 t) const {
		uint32_t k = uint32_t(t) * neg;
		// t+k*m 的低 32 位为 0；分开加高位，避免 64
		// 位溢出。
		u64 v = (t >> 32) + (u64(k) * m >> 32) +
		        (uint32_t(t) != 0);
		return uint32_t(v >= m ? v - m : v);
	}
	uint32_t encode(uint32_t a) const {
		return reduce(u64(a % m) * r2);
	}
	uint32_t decode(uint32_t a) const {
		return reduce(a);
	}
	uint32_t mul(uint32_t a, uint32_t b) const {
		return reduce(u64(a) * b);
	}
	uint32_t add(uint32_t a, uint32_t b) const {
		u64 s = u64(a) + b;
		return uint32_t(s >= m ? s - m : s);
	}
	uint32_t sub(uint32_t a, uint32_t b) const {
		return a >= b ? a - b
		              : uint32_t(u64(a) + m - b);
	}
	// 对外接收普通数 a，返回普通数 a^e mod m。
	uint32_t pow(uint32_t a, u64 e) const {
		uint32_t x = encode(a), r = encode(1);
		for (; e; e >>= 1, x = mul(x, x))
			if (e & 1) r = mul(r, x);
		return decode(r);
	}
};
