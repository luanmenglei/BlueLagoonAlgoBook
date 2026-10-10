/**
 * @title 后缀 LCP 查询
 * @brief 查询任意两个后缀的最长公共前缀长度。
 * @note 后缀起点 0..n，n 表示空后缀。返回两个后缀的 LCP；rank 包括空后缀。
 * @time 额外预处理 O(n log n)，查询 O(1)
 * @depends contest/Base.h, strings/SuffixArray.h, data-structures/SparseTable.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../strings/SuffixArray.h"
#include "../data-structures/SparseTable.h"

struct SuffixLCP {
	SuffixArray sa;
	ST st;
	vector<int> rank;
	SuffixLCP(const string &s)
	    : sa(s), st(sa.lcp), rank(s.size() + 1) {
		for (int i = 0; i < (int)sa.sa.size(); i ++)
			rank[sa.sa[i]] = i;
	}
	int lcp(int i, int j) const {
		if (i == j) return (int)rank.size() - 1 - i;
		int l = rank[i], r = rank[j];
		if (l > r) swap(l, r);
		return st.query(l + 1, r);
	}
};
