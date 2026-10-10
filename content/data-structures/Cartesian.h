/**
 * @title 笛卡尔树
 * @brief 将数组构成下标中序、权值满足最小堆性质的二叉树。
 * @note 0-based；最小堆、下标中序。相等值取左侧为祖先；空输入根为 -1。
 * @time O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

pair<int, vector<array<int, 2>>>
cartesian(const vector<int> &a) {
	int n = a.size();
	vector<array<int, 2>> ch(n, { -1, -1 });
	vector<int> st;
	for (int i = 0; i < n; i ++) {
		int last = -1;
		while (!st.empty() && a[st.back()] > a[i]) {
			last = st.back();
			st.pop_back();
		}
		if (!st.empty()) ch[st.back()][1] = i;
		ch[i][0] = last;
		st.push_back(i);
	}
	return { st.empty() ? -1 : st[0], ch };
}
