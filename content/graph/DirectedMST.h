/**
 * @title 有向最小生成树／最小树形图
 * @brief 求从指定根可达所有点的有向生成树最小总权值。
 * @note Chu-Liu/Edmonds；0-based，求 root 到所有点的最小树形图权值，失败 nullopt。支持负权和平行边；不恢复边。边权和总和须不溢出。
 * @time O(nm)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct DEdge {
	int u, v;
	i64 w;
};
optional<i64> directed_mst(int n, int root,
                           vector<DEdge> ed) {
	i64 ans = 0;
	const i64 inf = LLONG_MAX / 4;
	while (true) {
		vector<i64> in(n, inf);
		vector<int> pre(n), id(n, -1), vis(n, -1);
		for (auto e : ed)
			if (e.u != e.v && e.w < in[e.v])
				in[e.v] = e.w, pre[e.v] = e.u;
		in[root] = 0;
		for (i64 w : in) {
			if (w == inf) return nullopt;
			ans += w;
		}
		int cnt = 0;
		for (int i = 0; i < n; i ++) {
			int v = i;
			while (v != root && id[v] == -1 &&
			       vis[v] != i)
				vis[v] = i, v = pre[v];
			if (v != root && id[v] == -1) {
				for (int u = pre[v]; u != v; u = pre[u])
					id[u] = cnt;
				id[v] = cnt ++;
			}
		}
		if (!cnt) return ans;
		for (int i = 0; i < n; i ++)
			if (id[i] == -1) id[i] = cnt ++;
		for (auto &e : ed) {
			i64 w = in[e.v];
			e.u = id[e.u];
			e.v = id[e.v];
			if (e.u != e.v) e.w -= w;
		}
		root = id[root];
		n = cnt;
	}
}
