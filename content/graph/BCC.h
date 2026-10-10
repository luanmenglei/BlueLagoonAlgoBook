/**
 * @title 割点、桥、点双与边双
 * @brief 枚举割点、桥、点双与边双连通分量，并构建圆方森林。
 * @note 0-based 无向多重图；忽略自环后 add，允许平行边。solve 一次；bridge 按边编号，ec 为边双编号，blocks 为点双顶点集；block_tree
 * 是圆方森林。
 * @time DFS O(n+m)，点双去重额外 O(m log m)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct BCC {
	int n, timer = 0;
	vector<vector<pair<int, int>>> g;
	vector<pair<int, int>> ed;
	vector<int> dfn, low, st, ec;
	vector<bool> cut, bridge;
	vector<vector<int>> blocks;
	BCC(int n)
	    : n(n), g(n), dfn(n), low(n), ec(n, -1),
	      cut(n) {}
	void add(int u, int v) {
		assert(u != v);
		int id = ed.size();
		ed.push_back({ u, v });
		g[u].push_back({ v, id });
		g[v].push_back({ u, id });
	}
	void dfs(int u, int pe) {
		dfn[u] = low[u] = ++timer;
		int child = 0;
		for (auto [v, id] : g[u])
			if (id != pe) {
				if (!dfn[v]) {
					child ++;
					st.push_back(id);
					dfs(v, id);
					low[u] = min(low[u], low[v]);
					bridge[id] = low[v] > dfn[u];
					if (low[v] >= dfn[u]) {
						cut[u] = pe != -1 || child > 1;
						vector<int> b;
						int x;
						do {
							x = st.back();
							st.pop_back();
							b.push_back(ed[x].first);
							b.push_back(ed[x].second);
						} while (x != id);
						sort(b.begin(), b.end());
						b.erase(
						    unique(b.begin(), b.end()),
						    b.end());
						blocks.push_back(b);
					}
				} else if (dfn[v] < dfn[u]) {
					st.push_back(id);
					low[u] = min(low[u], dfn[v]);
				}
			}
	}
	void color(int u, int c) {
		ec[u] = c;
		for (auto [v, id] : g[u])
			if (!bridge[id] && ec[v] == -1) color(v, c);
	}
	void solve() {
		bridge.resize(ed.size());
		for (int u = 0; u < n; u ++)
			if (!dfn[u]) {
				dfs(u, -1);
				if (g[u].empty())
					blocks.push_back({ u });
			}
		int c = 0;
		for (int u = 0; u < n; u ++)
			if (ec[u] == -1) color(u, c ++);
	}
	vector<vector<int>> block_tree() {
		vector<vector<int>> t(n + blocks.size());
		for (int i = 0; i < (int)blocks.size(); i ++)
			for (int u : blocks[i]) {
				t[u].push_back(n + i);
				t[n + i].push_back(u);
			}
		return t;
	}
};
