/**
 * @title Lyndon 分解／字符串最小表示
 * @brief 求 Lyndon 因子分解，以及字典序最小循环位移的起点。
 * @note duval 返回各 Lyndon 因子的半开区间；min_rotation 返回字典序最小循环位移的起点，空串返回0。按 char 比较，建议 ASCII 字符集。
 * @time O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<pair<int, int>> duval(const string &s) {
	int n = s.size();
	vector<pair<int, int>> ans;
	for (int i = 0; i < n;) {
		int j = i + 1, k = i;
		while (j < n && s[k] <= s[j]) {
			k = s[k] < s[j] ? i : k + 1;
			j ++;
		}
		while (i <= k)
			ans.push_back({ i, i + j - k }), i += j - k;
	}
	return ans;
}
int min_rotation(const string &s) {
	int n = s.size();
	if (!n) return 0;
	string t = s + s;
	int i = 0, j = 1, k = 0;
	while (i < n && j < n && k < n) {
		if (t[i + k] == t[j + k]) {
			k ++;
			continue;
		}
		if (t[i + k] > t[j + k]) {
			i += k + 1;
			if (i <= j) i = j + 1;
		} else {
			j += k + 1;
			if (j <= i) j = i + 1;
		}
		k = 0;
	}
	return min(i, j);
}
