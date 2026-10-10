/**
 * @title 生成树计数（Matrix-Tree）
 * @brief 统计无向生成树、有向根向／根出树；带权时求边权积之和。
 * @note 0-based，n>=1、素数模；自环忽略。spanning_trees 无向无权；weighted_spanning_trees
 * 的边为(u,v,w)，权可按模归一；directed_spanning_trees(n,root,edges,mod,toward)
 * 为根向树（每个点沿原边到根），toward=false 为从根出发的树。n=1返回1；根删行列。例：0→1、1→2 的单位权边，root=0,toward=false 返回1。
 * @time O(n^3)
 * @depends contest/Base.h, algebra/Gauss.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/Gauss.h"

int weighted_spanning_trees(
    int n, const vector<tuple<int, int, int>> &ed,
    int mod) {
	assert(n >= 1);
	vector<vector<int>> a(n, vector<int>(n));
	auto add = [&](int u, int v, i64 x) {
		a[u][v] = (a[u][v] + x + mod) % mod;
	};
	for (auto [u, v, w] : ed)
		if (u != v) {
			w = (i64(w) % mod + mod) % mod;
			add(u, u, w), add(v, v, w), add(u, v, -w),
			    add(v, u, -w);
		}
	a.pop_back();
	for (auto &r : a)
		r.pop_back();
	return determinant(a, mod);
}

int directed_spanning_trees(
    int n, int root,
    const vector<tuple<int, int, int>> &ed, int mod,
    bool toward = true) {
	assert(n >= 1 && 0 <= root && root < n);
	vector<vector<int>> a(n, vector<int>(n));
	for (auto [u, v, w] : ed)
		if (u != v) {
			if (!toward) swap(u, v);
			w = (i64(w) % mod + mod) % mod;
			a[u][u] = (i64(a[u][u]) + w) % mod;
			a[u][v] = (i64(a[u][v]) - w + mod) % mod;
		}
	a.erase(a.begin() + root);
	for (auto &r : a)
		r.erase(r.begin() + root);
	return determinant(a, mod);
}

int spanning_trees(int n,
                   const vector<pair<int, int>> &ed,
                   int mod) {
	assert(n >= 1);
	vector<vector<int>> a(n, vector<int>(n));
	for (auto [u, v] : ed)
		if (u != v) {
			a[u][u] = (a[u][u] + 1) % mod;
			a[v][v] = (a[v][v] + 1) % mod;
			a[u][v] = (a[u][v] + mod - 1) % mod;
			a[v][u] = (a[v][u] + mod - 1) % mod;
		}
	a.pop_back();
	for (auto &r : a)
		r.pop_back();
	return determinant(a, mod);
}
