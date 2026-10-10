/**
 * @title 全覆盖循环序列（De Bruijn）
 * @brief 构造循环序列，使每种长度 n 的字母串恰好出现一次。
 * @note 返回字母表0..k-1、阶数n的循环序列，长度$k^n$；线性展开时重复开头n-1个字符。输出规模须可存储，k=1 特判。
 * @time O(k^n)
 * @depends contest/Base.h
 * @author koosaga; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/ad_hoc/debruijn.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int> de_bruijn(int k, int n) {
	assert(k >= 1 && n >= 1);
	if (k == 1) return { 0 };
	vector<int> a(n + 1), ans;
	auto dfs = [&](auto self, int t, int p) -> void {
		if (t > n) {
			if (n % p == 0)
				for (int i = 1; i <= p; i ++)
					ans.push_back(a[i]);
			return;
		}
		a[t] = a[t - p];
		self(self, t + 1, p);
		for (int i = a[t - p] + 1; i < k; i ++)
			a[t] = i, self(self, t + 1, t);
	};
	dfs(dfs, 1, 1);
	return ans;
}
