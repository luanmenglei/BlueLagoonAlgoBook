/**
 * @title 静态区间最值／幂等查询（ST 表）
 * @brief 预处理静态数组，常数时间查询区间 min、max 等幂等运算。
 * @note 0-based 闭区间；仅适合幂等运算，如 min/max/gcd，不能直接改成 sum。
 * @time 预处理 O(n log n)，查询 O(1)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct ST {
	vector<vector<int>> st;
	ST(const vector<int> &a) {
		int n = a.size();
		assert(n);
		st.push_back(a);
		for (int k = 1; (1 << k) <= n; k ++) {
			st.emplace_back(n - (1 << k) + 1);
			for (int i = 0; i + (1 << k) <= n; i ++)
				st[k][i] =
				    min(st[k - 1][i],
				        st[k - 1][i + (1 << (k - 1))]);
		}
	}
	int query(int l, int r) const {
		int k = __lg(r - l + 1);
		return min(st[k][l], st[k][r - (1 << k) + 1]);
	}
};
