/**
 * @title 一般图 Δ+1 边染色
 * @brief 用不超过 Δ+1 种颜色给一般简单图边染色。
 * @note 简单无向图，0-based，无自环／平行边。输出按输入边顺序的颜色，范围0..Δ。不能用于一般多重图。
 * @usage `edgeColoring(3,{{0,1},{1,2},{2,0}})` 返回三种颜色，顺序对应输入边。
 * @time O(nm)
 * @depends contest/Base.h
 * @author Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/EdgeColoring.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int> edgeColoring(int N,
                         vector<pair<int, int>> eds) {
	vector<int> cc(N + 1), ret(int(eds.size())), fan(N),
	    free(N), loc;
	for (pair<int, int> e : eds)
		++cc[e.first], ++cc[e.second];
	int u, v,
	    ncols = *max_element(cc.begin(), cc.end()) + 1;
	vector<vector<int>> adj(N, vector<int>(ncols, -1));
	for (pair<int, int> e : eds) {
		tie(u, v) = e;
		fan[0] = v;
		loc.assign(ncols, 0);
		int at = u, end = u, d, c = free[u], ind = 0,
		    i = 0;
		while (d = free[v],
		       !loc[d] && (v = adj[u][d]) != -1)
			loc[d] = ++ind, cc[ind] = d, fan[ind] = v;
		cc[loc[d]] = c;
		for (int cd = d; at != -1;
		     cd ^= c ^ d, at = adj[at][cd])
			swap(adj[at][cd],
			     adj[end = at][cd ^ c ^ d]);
		while (adj[fan[i]][d] != -1) {
			int left = fan[i], right = fan[++i],
			    e = cc[i];
			adj[u][e] = left;
			adj[left][e] = u;
			adj[right][e] = -1;
			free[right] = e;
		}
		adj[u][d] = fan[i];
		adj[fan[i]][d] = u;
		for (int y : { fan[0], u, end })
			for (int &z = free[y] = 0; adj[y][z] != -1;
			     z ++)
				;
	}
	for (int i = 0; i < int(eds.size()); i ++)
		for (tie(u, v) = eds[i]; adj[u][ret[i]] != v;)
			++ret[i];
	return ret;
}
