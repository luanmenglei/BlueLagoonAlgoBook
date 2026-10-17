/**
 * @title 位扫描与等大小子集枚举
 * @brief 提供零值安全的最高／最低位查询、lowbit
 * 和下一同位数掩码。
 * @note 位编号 0..63；lsb_index、msb_index 对 0 返回
 * -1。next_mask 按数值递增， 保持 1 的个数；0 或已是 64
 * 位中最后一个时返回 nullopt。均用无符号数。
 * @usage auto s=next_mask(7); // 得 11，即 00111
 * 的下一个是 01011。
 * @time 每次操作 O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; Gosper's hack
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

inline int lsb_index(u64 x) {
	return x ? __builtin_ctzll(x) : -1;
}
inline int msb_index(u64 x) {
	return x ? 63 - __builtin_clzll(x) : -1;
}
inline u64 lowbit(u64 x) {
	return x & (0 - x);
}
inline optional<u64> next_mask(u64 x) {
	if (!x) return nullopt;
	u64 a = lowbit(x), b = x + a;
	if (!b) return nullopt;
	// 分两次右移，避免 ctz(a)+2=64 时出现未定义移位。
	return b | (((b ^ x) >> 2) >> lsb_index(a));
}
