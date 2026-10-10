/**
 * @title 前缀函数／KMP／Z 函数
 * @brief 计算前缀函数和 Z 数组，使用 KMP 枚举模式串出现位置。
 * @note 0-based，z[0]=0；KMP 返回每个匹配的起始下标，模式串非空。最短周期候选 n-p[n-1]，整周期需整除 n。
 * @time O(n+m)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int> prefix_function(const string &s) {
	vector<int> p(s.size());
	for (int i = 1; i < (int)s.size(); i ++) {
		int j = p[i - 1];
		while (j && s[i] != s[j])
			j = p[j - 1];
		p[i] = j + (s[i] == s[j]);
	}
	return p;
}
vector<int> kmp(const string &s, const string &t) {
	assert(!t.empty());
	auto p = prefix_function(t);
	vector<int> ans;
	for (int i = 0, j = 0; i < (int)s.size(); i ++) {
		while (j && s[i] != t[j])
			j = p[j - 1];
		j += s[i] == t[j];
		if (j == (int)t.size())
			ans.push_back(i - j + 1), j = p[j - 1];
	}
	return ans;
}
vector<int> z_function(const string &s) {
	int n = s.size();
	vector<int> z(n);
	for (int i = 1, l = 0, r = 0; i < n; i ++) {
		if (i <= r) z[i] = min(r - i + 1, z[i - l]);
		while (i + z[i] < n && s[z[i]] == s[i + z[i]])
			z[i] ++;
		if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
	return z;
}
