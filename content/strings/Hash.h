/**
 * @title 双模字符串哈希
 * @brief 计算子串双模哈希，用于快速比较子串。
 * @note 0-based 半开区间 [l,r)。比较哈希时同时比较长度；存在碰撞，固定 base 易被针对，必要时在全局随机选同一 base。
 * @time 预处理 O(n)，查询 O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct StringHash {
	static const int p = 1000000007, q = 1000000009;
	static const int base = 911382323;
	vector<array<int, 2>> h, pw;
	StringHash(const string &s)
	    : h(s.size() + 1), pw(s.size() + 1) {
		pw[0] = { 1, 1 };
		for (int i = 0; i < (int)s.size(); i ++)
			for (int j = 0; j < 2; j ++) {
				int mod = j ? q : p;
				pw[i + 1][j] =
				    i64(pw[i][j]) * base % mod;
				h[i + 1][j] =
				    (i64(h[i][j]) * base +
				     (unsigned char)s[i] + 1) %
				    mod;
			}
	}
	array<int, 2> get(int l, int r) const {
		array<int, 2> ans;
		for (int j = 0; j < 2; j ++) {
			int mod = j ? q : p;
			ans[j] =
			    (h[r][j] -
			     i64(h[l][j]) * pw[r - l][j] % mod +
			     mod) %
			    mod;
		}
		return ans;
	}
};
