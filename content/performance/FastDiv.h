/**
 * @title 64 除 32 位商余数（x86 divl）
 * @brief 同时返回无符号除法的商和余数，x86 使用单条
 * divl。
 * @note y>0 且 x<y*2^32，保证商能装入
 * uint32_t；违反前提可能触发硬件异常。 GNU x86
 * 使用汇编，其余架构使用 / 和 %。这不是
 * Barrett；速度需实测。
 * @usage auto [q,r]=divmod32(10000000000ULL,7);
 * @time O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; x86 DIV
 * instruction
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

inline pair<uint32_t, uint32_t> divmod32(u64 x,
                                         uint32_t y) {
	assert(y && (x >> 32) < y);
#if !defined(BL_PORTABLE) && defined(__GNUC__) &&      \
    (defined(__i386__) || defined(__x86_64__))
	uint32_t q = uint32_t(x), r = x >> 32;
	asm("divl %2" : "+a"(q), "+d"(r) : "rm"(y) : "cc");
	return { q, r };
#else
	return { uint32_t(x / y), uint32_t(x % y) };
#endif
}
