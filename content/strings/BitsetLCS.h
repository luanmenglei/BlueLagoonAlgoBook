/**
 * @title 最长公共子序列长度／恢复（LCS）
 * @brief 用位运算求最长公共子序列长度，用 Hirschberg 恢复一条子序列。
 * @note 任意字节串；bitset_lcs 只求长度，lcs_restore 另作恢复。可将较短串作为 b 以减少空间；unsigned 计算跨字借位。
 * lcs_restore(a,b) 返回一条 LCS；使用 string_view、不限制字节字母表。例：lcs_restore("abc","ac") 得 "ac"。
 * @time O(|a| ceil(|b|/64))，空间 O(256 ceil(|b|/64))；恢复 O(nm) 时间、O(m+log n) 额外空间（另计结果字符串）
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

string lcs_restore(string_view a, string_view b) {
	if (a.empty() || b.empty()) return "";
	if (a.size() == 1)
		return b.find(a[0]) == string_view::npos
		           ? ""
		           : string(1, a[0]);
	int mid = a.size() / 2, m = b.size(), k = 0;
	{
		auto row = [&](string_view x, bool rev) {
			vector<int> d(m + 1);
			for (int i = 0; i < (int)x.size(); i ++) {
				int pre = 0;
				for (int j = 1; j <= m; j ++) {
					int old = d[j];
					char u =
					    x[rev ? x.size() - 1 - i : i];
					char v = b[rev ? m - j : j - 1];
					d[j] = u == v ? pre + 1
					              : max(d[j], d[j - 1]);
					pre = old;
				}
			}
			return d;
		};
		auto l = row(a.substr(0, mid), false);
		auto r = row(a.substr(mid), true);
		for (int j = 1; j <= m; j ++)
			if (l[j] + r[m - j] > l[k] + r[m - k])
				k = j;
	}
	return lcs_restore(a.substr(0, mid),
	                   b.substr(0, k)) +
	       lcs_restore(a.substr(mid), b.substr(k));
}

int bitset_lcs(const string &a, const string &b) {
	int n = b.size(), m = (n + 63) / 64;
	vector<vector<u64>> mask(256, vector<u64>(m));
	for (int i = 0; i < n; i ++)
		mask[(unsigned char)b[i]][i / 64] |=
		    1ULL << (i % 64);
	vector<u64> s(m);
	for (unsigned char c : a) {
		u64 carry = 1, borrow = 0;
		for (int j = 0; j < m; j ++) {
			u64 x = s[j] | mask[c][j];
			u64 y = (s[j] << 1) | carry;
			carry = s[j] >> 63;
			u64 z = x - y - borrow;
			borrow = x < y || (borrow && x == y);
			s[j] = x & ~z;
		}
	}
	int ans = 0;
	for (u64 x : s)
		ans += __builtin_popcountll(x);
	return ans;
}
