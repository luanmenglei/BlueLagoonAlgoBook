/**
 * @title 动态模数快速取模（Barrett）
 * @brief 预处理模数的倒数，用乘法高位和一次修正求余数。
 * @note 1<=m<2^32；reduce 接受任意 u64，mul 接受任意
 * uint32_t。
 * 模数固定时复用对象；不要每次乘法都构造。编译期常量模数优先直接用
 * % 并实测。
 * @usage Barrett32 b(998244353); auto r=b.mul(123,456);
 * @time 构造 O(1)（一次除法）；reduce、mul O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; reciprocal
 * division with floor(2^64/m)
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

struct Barrett32 {
	uint32_t m; // 模数；构造后不要修改。
	u64 inv;    // floor(2^64/m)，m=1 时单独处理。
	explicit Barrett32(uint32_t mod) : m(mod) {
		assert(m);
		inv = m == 1 ? 0 : u64((u128(1) << 64) / m);
	}
	uint32_t reduce(u64 x) const {
		if (m == 1) return 0;
		u64 q = u128(x) * inv >> 64;
		// q 比真实商至多小 1，q*m<=x，故减法不会下溢。
		u64 r = x - q * m;
		return uint32_t(r >= m ? r - m : r);
	}
	uint32_t mul(uint32_t a, uint32_t b) const {
		return reduce(u64(a) * b);
	}
};
