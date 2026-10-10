/**
 * @title 倍增祖先／路径第 k 点
 * @brief 求某点的第 k 级祖先，以及两点路径上指定偏移的顶点。
 * @note 以 HLD 的父亲／深度初始化；kth_path 的 k 为从 u 开始的 0-based 偏移，越界返回 -1。jump 不允许越过根。
 * @usage `BinaryLifting b(h);`；`b.kth_path(u,v,0)` 为u，`b.jump(v,1)` 为父亲（根返回-1）。
 * @time O(n log n) 预处理，O(log n) 查询
 * @depends contest/Base.h, data-structures/HLD.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../data-structures/HLD.h"

struct BinaryLifting {
	vector<vector<int>> up;
	vector<int> dep;
	BinaryLifting(const HLD &h) : dep(h.dep) {
		int n = h.n, lg = 1;
		while ((1LL << lg) <= n)
			lg ++;
		up.assign(lg, vector<int>(n));
		for (int v = 0; v < n; v ++)
			up[0][v] = h.p[v] < 0 ? v : h.p[v];
		for (int j = 1; j < lg; j ++)
			for (int v = 0; v < n; v ++)
				up[j][v] = up[j - 1][up[j - 1][v]];
	}
	int jump(int v, int k) const {
		if (k < 0 || k > dep[v]) return -1;
		for (int j = 0; k; k >>= 1, j ++)
			if (k & 1) v = up[j][v];
		return v;
	}
	int lca(int u, int v) const {
		if (dep[u] < dep[v]) swap(u, v);
		u = jump(u, dep[u] - dep[v]);
		if (u == v) return u;
		for (int j = (int)up.size() - 1; j >= 0; j --)
			if (up[j][u] != up[j][v])
				u = up[j][u], v = up[j][v];
		return up[0][u];
	}
	int kth_path(int u, int v, int k) const {
		int w = lca(u, v), a = dep[u] - dep[w],
		    b = dep[v] - dep[w];
		if (k < 0 || k > a + b) return -1;
		return k <= a ? jump(u, k) : jump(v, a + b - k);
	}
};
