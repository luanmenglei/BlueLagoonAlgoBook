/**
 * @title 虚树
 * @brief 压缩树上关键点及必要 LCA，保留关键点之间的祖先关系。
 * @note 输入关键点；输出祖先→后代边，补入相邻 LCA。单点时无边，节点自身由输入保存；边长为 dep[v]-dep[u]。
 * @time O(k log k + k log n)
 * @depends contest/Base.h, data-structures/HLD.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../data-structures/HLD.h"

vector<pair<int, int>> virtual_tree(vector<int> a,
                                    const HLD &h) {
	if (a.empty()) return {};
	auto cmp = [&](int x, int y) {
		return h.in[x] < h.in[y];
	};
	sort(a.begin(), a.end(), cmp);
	int k = a.size();
	for (int i = 1; i < k; i ++)
		a.push_back(h.lca(a[i - 1], a[i]));
	sort(a.begin(), a.end(), cmp);
	a.erase(unique(a.begin(), a.end()), a.end());
	vector<int> st;
	vector<pair<int, int>> ed;
	for (int u : a) {
		while (!st.empty() &&
		       h.out[st.back()] <= h.in[u])
			st.pop_back();
		if (!st.empty()) ed.push_back({ st.back(), u });
		st.push_back(u);
	}
	return ed;
}
