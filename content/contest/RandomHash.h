/**
 * @title 随机数与哈希
 * @brief 生成随机数，为哈希表的整数键提供混洗哈希。
 * @note SplitMix64 混洗；哈希碰撞仍有可能。对拍使用固定随机种子以复现。
 * @time 平均 O(1) 哈希
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

mt19937_64 rng(chrono::steady_clock::now()
                   .time_since_epoch()
                   .count());
i64 rnd(i64 l, i64 r) {
	return uniform_int_distribution<i64>(l, r)(rng);
}
struct Hash {
	static u64 mix(u64 x) {
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(u64 x) const {
		static const u64 seed = rng();
		return mix(x + seed);
	}
};
// unordered_map<i64, int, Hash> mp;
