/**
 * @title 固定端点必经点／边
 * @brief 用拆边和支配树精确求有向图 s 到 t
 * 的必经点、边，也支持非负权最短路。
 * @note
 * 0-based，允许平行边、自环；边按输入顺序编号。reachable=false
 * 时所有标记为 false。 可达时端点也算必经点；s=t
 * 允许空路径，只有 s 必经。每条边拆成新点，可能有
 * O(n+m) 递归深度。 mandatory_shortest 要求
 * 0<=w<LLONG_MAX/4，所有需表示的有限最短距离小于该上界；支持零权环。
 * @usage auto a=mandatory_st(3,{{0,1},{1,2}},0,2); //
 * a.vertex[1]、a.edge[0] 为 true。
 * @time O((n+m) log(n+m))，空间 O(n+m)
 * @depends contest/Base.h, graph/Dominator.h,
 * graph/ShortestPath.h
 * @author BlueLagoon
 * @source BlueLagoon wrapper; edge subdivision and
 * dominator tree
 * @license CC0-1.0 (wrapper only; dependencies retain
 * their licenses)
 */
#pragma once
#include "../contest/Base.h"
#include "Dominator.h"
#include "ShortestPath.h"

struct MandatoryResult {
	bool reachable;
	vector<bool> vertex,
	    edge; // 原顶点／原边是否出现在每条 s-t 路径上。
};

MandatoryResult
mandatory_st(int n, const vector<pair<int, int>> &ed,
             int s, int t) {
	assert(0 <= s && s < n && 0 <= t && t < n);
	int m = ed.size();
	MandatoryResult ans{ false, vector<bool>(n),
		                 vector<bool>(m) };
	Dominator d(n + m);
	for (int i = 0; i < m; i ++) {
		auto [u, v] = ed[i];
		assert(0 <= u && u < n && 0 <= v && v < n);
		// 每条边各有一个新点，平行边也不能共用。
		d.g[u].push_back(n + i);
		d.g[n + i].push_back(v);
	}
	auto p = d.solve(s);
	if (p[t] == -1) return ans;
	ans.reachable = true;
	// 支配树上 t 到根的祖先恰好是所有必经点。
	for (int x = t;; x = p[x]) {
		if (x < n)
			ans.vertex[x] = true;
		else
			ans.edge[x - n] = true;
		if (x == s) break;
	}
	return ans;
}

MandatoryResult mandatory_shortest(
    int n, const vector<tuple<int, int, i64>> &ed,
    int s, int t) {
	assert(0 <= s && s < n && 0 <= t && t < n);
	const i64 inf = LLONG_MAX / 4;
	vector<vector<pair<int, i64>>> g(n), rg(n);
	for (auto [u, v, w] : ed) {
		assert(0 <= u && u < n && 0 <= v && v < n &&
		       0 <= w && w < inf);
		g[u].push_back({ v, w });
		rg[v].push_back({ u, w });
	}
	auto a = dijkstra(g, s), b = dijkstra(rg, t);
	MandatoryResult ans{ false, vector<bool>(n),
		                 vector<bool>(ed.size()) };
	if (a[t] == inf) return ans;
	vector<pair<int, int>> tight;
	vector<int> id;
	for (int i = 0; i < (int)ed.size(); i ++) {
		auto [u, v, w] = ed[i];
		if (a[u] != inf && b[v] != inf &&
		    a[u] + w + b[v] == a[t]) {
			tight.push_back({ u, v });
			id.push_back(i);
		}
	}
	auto z = mandatory_st(n, tight, s, t);
	ans.reachable = true;
	ans.vertex = move(z.vertex);
	for (int i = 0; i < (int)id.size(); i ++)
		ans.edge[id[i]] = z.edge[i];
	return ans;
}
