/**
 * @title 点分治骨架
 * @brief 按重心拆分树，提供逐子树统计路径贡献的回调骨架。
 * @note 无向树，0-based。work 统计经过重心的贡献：逐子树查询已有信息，再加入当前子树；仅访问 !dead 节点。深链注意递归栈。
 * @time 分治框架 O(n log n)，另加 work 开销
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Centroid {
	vector<vector<int>> g;
	vector<int> sz;
	vector<bool> dead;
	Centroid(int n) : g(n), sz(n), dead(n) {}
	int size(int u, int p) {
		sz[u] = 1;
		for (int v : g[u])
			if (v != p && !dead[v]) sz[u] += size(v, u);
		return sz[u];
	}
	int center(int u, int p, int tot) {
		for (int v : g[u])
			if (v != p && !dead[v])
				if (sz[v] > tot / 2)
					return center(v, u, tot);
		return u;
	}
	template <class F> void solve(int u, F &work) {
		u = center(u, -1, size(u, -1));
		work(u, g, dead);
		dead[u] = true;
		for (int v : g[u])
			if (!dead[v]) solve(v, work);
	}
};
