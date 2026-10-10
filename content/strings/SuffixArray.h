/**
 * @title 后缀数组＋Height
 * @brief 按字典序排序全部后缀，计算相邻后缀最长公共前缀。
 * @note 输入字节串不得含零字节；0-based。sa 大小 n+1，sa[0]=n 是空后缀；lcp[i]=LCP(sa[i-1],sa[i])。LCP 查询对 rank 之间的
 * lcp 做 RMQ。
 * @time O(n log n)
 * @depends contest/Base.h
 * @author 罗穗骞, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/strings/SuffixArray.h
 * @license Unknown
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct SuffixArray {
	vector<int> sa, lcp;
	SuffixArray(string s,
	            int lim = 256) { // or vector<int>
		s.push_back(0);
		int n = (int)(s).size(), k = 0, a, b;
		vector<int> x(n), y(n), ws(max(n, lim));
		for (int i = 0; i < n; i ++)
			x[i] = (unsigned char)s[i];
		sa = lcp = y, iota((sa).begin(), (sa).end(), 0);
		for (int j = 0, p = 0; p < n;
		     j = max(1, j * 2), lim = p) {
			p = j, iota((y).begin(), (y).end(), n - j);
			for (int i = 0; i < n; i ++)
				if (sa[i] >= j) y[p ++] = sa[i] - j;
			fill((ws).begin(), (ws).end(), 0);
			for (int i = 0; i < n; i ++)
				ws[x[i]] ++;
			for (int i = 1; i < lim; i ++)
				ws[i] += ws[i - 1];
			for (int i = n; i --;)
				sa[--ws[x[y[i]]]] = y[i];
			swap(x, y), p = 1, x[sa[0]] = 0;
			for (int i = 1; i < n; i ++)
				a = sa[i - 1], b = sa[i],
				x[b] = (y[a] == y[b] &&
				        y[a + j] == y[b + j])
				           ? p - 1
				           : p ++;
		}
		for (int i = 0, j; i < n - 1; lcp[x[i ++]] = k)
			for (k &&k --, j = sa[x[i] - 1];
			     s[i + k] == s[j + k]; k ++)
				;
	}
};
