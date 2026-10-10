/**
 * @title 异或线性基
 * @brief 维护可表示的异或值，求最大异或和及去重后的第 k 小异或值。
 * @note unsigned 64位；kth 是去重后所有可表示异或值的0-based第k小，包含空集得到的0。若题目要求非空子集，需另记线性相关性。
 * @time 插入／最大值 O(64)，kth O(64^2)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct XorBasis {
	array<u64, 64> b{};
	bool insert(u64 x) {
		for (int i = 63; i >= 0; i --)
			if (x >> i & 1) {
				if (!b[i]) {
					b[i] = x;
					return true;
				}
				x ^= b[i];
			}
		return false;
	}
	bool contains(u64 x) const {
		for (int i = 63; i >= 0; i --)
			if (x >> i & 1) x ^= b[i];
		return x == 0;
	}
	u64 maximum(u64 x = 0) const {
		for (int i = 63; i >= 0; i --)
			x = max(x, x ^ b[i]);
		return x;
	}
	vector<u64> reduced() const {
		auto a = b;
		for (int i = 0; i < 64; i ++)
			if (a[i])
				for (int j = i + 1; j < 64; j ++)
					if (a[j] >> i & 1) a[j] ^= a[i];
		vector<u64> v;
		for (u64 x : a)
			if (x) v.push_back(x);
		return v;
	}
	u64 kth(u64 k) const {
		auto v = reduced();
		assert(v.size() == 64 ||
		       k < (1ULL << v.size()));
		u64 ans = 0;
		for (int i = 0; i < (int)v.size(); i ++)
			if (k >> i & 1) ans ^= v[i];
		return ans;
	}
};
