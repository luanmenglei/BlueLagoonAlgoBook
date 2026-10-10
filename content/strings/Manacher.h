/**
 * @title 回文半径／最长回文（Manacher）
 * @brief 求每个奇／偶中心的最长回文半径，可据此求最长回文子串。
 * @note odd[i] 对应长度 2*odd[i]-1；even[i] 对应以 i-1 与 i 之间为中心的长度 2*even[i]。
 * @time O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

pair<vector<int>, vector<int>>
manacher(const string &s) {
	int n = s.size();
	vector<int> odd(n), even(n);
	for (int i = 0, l = 0, r = -1; i < n; i ++) {
		int k =
		    i > r ? 1 : min(odd[l + r - i], r - i + 1);
		while (i - k >= 0 && i + k < n &&
		       s[i - k] == s[i + k])
			k ++;
		odd[i] = k --;
		if (i + k > r) l = i - k, r = i + k;
	}
	for (int i = 0, l = 0, r = -1; i < n; i ++) {
		int k =
		    i > r ? 0
		          : min(even[l + r - i + 1], r - i + 1);
		while (i - k - 1 >= 0 && i + k < n &&
		       s[i - k - 1] == s[i + k])
			k ++;
		even[i] = k --;
		if (i + k > r) l = i - k - 1, r = i + k;
	}
	return { odd, even };
}
