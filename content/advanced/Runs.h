/**
 * @title 字符串极大周期段（runs）
 * @brief 枚举无法向左右延伸、长度至少两倍最小周期的周期段。
 * @note 输入小写字母；返回 (最小周期,l,r)，区间 [l,r)，长度>=2p，左右不能在该周期下延伸。基于两种字典序 Lyndon 根及前后 LCP；结果去重。
 * @usage `enumerate_runs("ababab")` 含 {2,0,6}；输出最小周期与半开区间。
 * @time O(n log n) 时间／空间
 * @depends contest/Base.h, strings/SuffixLCP.h
 * @author koosaga; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/string/run_enumerate.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../strings/SuffixLCP.h"

vector<array<int, 3>> enumerate_runs(string s) {
	int n = s.size();
	if (!n) return {};
	SuffixLCP fw(s);
	string rev = s;
	reverse(rev.begin(), rev.end());
	SuffixLCP bw(rev);
	vector<pair<int, int>> intervals;
	for (int pass = 0; pass < 2; pass ++) {
		SuffixArray sa(s);
		vector<int> rank(n + 1);
		for (int i = 0; i <= n; i ++)
			rank[sa.sa[i]] = i;
		vector<int> st{ n };
		for (int i = n - 1; i >= 0; i --) {
			while (rank[st.back()] > rank[i])
				st.pop_back();
			intervals.emplace_back(i, st.back());
			st.push_back(i);
		}
		for (char &c : s)
			c = 'a' + 'z' - c;
	}
	vector<array<int, 3>> ans;
	for (auto [x, y] : intervals) {
		int p = y - x, l = x - bw.lcp(n - x, n - y),
		    r = y + fw.lcp(x, y);
		if (r - l >= 2 * p) ans.push_back({ p, l, r });
	}
	sort(ans.begin(), ans.end());
	ans.erase(unique(ans.begin(), ans.end()),
	          ans.end());
	return ans;
}
