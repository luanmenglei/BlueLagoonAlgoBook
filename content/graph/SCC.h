/**
 * @title 强连通分量
 * @brief 分解有向图的强连通分量，提供逆拓扑序分量编号。
 * @note 0-based。comp 编号为逆拓扑序；solve 一次，修改图后重建对象。深链注意栈。
 * @time O(n+m)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct SCC {
	int timer = 0, cnt = 0;
	vector<vector<int>> g;
	vector<int> dfn, low, comp, st;
	SCC(int n) : g(n), dfn(n), low(n), comp(n, -1) {}
	void dfs(int u) {
		dfn[u] = low[u] = ++timer;
		st.push_back(u);
		for (int v : g[u]) {
			if (!dfn[v])
				dfs(v), low[u] = min(low[u], low[v]);
			else if (comp[v] == -1)
				low[u] = min(low[u], dfn[v]);
		}
		if (low[u] == dfn[u]) {
			int v;
			do {
				v = st.back();
				st.pop_back();
				comp[v] = cnt;
			} while (v != u);
			cnt ++;
		}
	}
	void solve() {
		for (int i = 0; i < (int)g.size(); i ++)
			if (!dfn[i]) dfs(i);
	}
};
