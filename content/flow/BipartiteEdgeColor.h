/**
 * @title 二分图最优边染色
 * @brief 用最大度数 Δ 种颜色给二分图边染色，使同点关联边颜色不同。
 * @note 两侧分别 0-based，允许平行边；输出边颜色 0..Delta-1。使用交替路径换色，内存 O((l+r)Delta)；适合中等规模。
 * @time O(m(n+Delta))，n=l+r
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int>
bipartite_edge_color(int l, int r,
                     const vector<pair<int, int>> &ed) {
	int n = l + r, m = ed.size(), delta = 0;
	vector<int> deg(n);
	for (auto [u, v] : ed)
		delta = max({ delta, ++deg[u], ++deg[l + v] });
	vector<vector<int>> at(n, vector<int>(delta, -1));
	vector<int> col(m, -1);
	auto other = [&](int id, int u) {
		return ed[id].first ^ (l + ed[id].second) ^ u;
	};
	for (int id = 0; id < m; id ++) {
		auto [u, v] = ed[id];
		v += l;
		int a = 0, b = 0;
		while (at[u][a] != -1)
			a ++;
		while (at[v][b] != -1)
			b ++;
		if (a != b) {
			vector<int> path;
			int x = v, c = a;
			while (at[x][c] != -1) {
				int e = at[x][c];
				path.push_back(e);
				x = other(e, x);
				c ^= a ^ b;
			}
			for (int e : path) {
				auto [s, t] = ed[e];
				t += l;
				at[s][col[e]] = at[t][col[e]] = -1;
			}
			for (int e : path) {
				auto [s, t] = ed[e];
				t += l;
				col[e] ^= a ^ b;
				at[s][col[e]] = at[t][col[e]] = e;
			}
		}
		col[id] = a;
		at[u][a] = at[v][a] = id;
	}
	return col;
}
