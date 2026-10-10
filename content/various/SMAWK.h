/**
 * @title 全单调矩阵行最小值（SMAWK）
 * @brief 通过矩阵元素查询接口，求全单调矩阵每行最左最小值的位置。
 * @note 离线 n×m 全单调矩阵，每行求最左最小列。全单调要求每个子矩阵行最优列也单调；只有原矩阵行最优列单调不足以使用。get 为 O(1) oracle。
 * @time O(n+m) oracle调用
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source SMAWK; oracle interface inspired by ecnerwala/cp-book/src/smawk.hpp; BlueLagoon
 * recursive implementation
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class F>
vector<int> smawk(int n, int m, F get) {
	assert(n >= 0 && (n == 0 || m >= 1));
	vector<int> rows(n), cols(m), ans(n, -1);
	iota(rows.begin(), rows.end(), 0);
	iota(cols.begin(), cols.end(), 0);
	auto rec = [&](auto self, const vector<int> &r,
	               const vector<int> &c) -> void {
		if (r.empty()) return;
		vector<int> st;
		for (int x : c) {
			while (!st.empty() &&
			       get(r[st.size() - 1], x) <
			           get(r[st.size() - 1], st.back()))
				st.pop_back();
			if (st.size() < r.size()) st.push_back(x);
		}
		vector<int> odd;
		for (int i = 1; i < (int)r.size(); i += 2)
			odd.push_back(r[i]);
		self(self, odd, st);
		int p = 0;
		for (int i = 0; i < (int)r.size(); i += 2) {
			int stop = i + 1 < (int)r.size()
			               ? ans[r[i + 1]]
			               : st.back();
			int best = st[p];
			while (true) {
				if (get(r[i], st[p]) < get(r[i], best))
					best = st[p];
				if (st[p] == stop) break;
				p ++;
			}
			ans[r[i]] = best;
		}
	};
	rec(rec, rows, cols);
	return ans;
}
