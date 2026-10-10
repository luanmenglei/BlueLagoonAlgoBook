/**
 * @title 二分图最大匹配（Hopcroft-Karp）
 * @brief 求二分图最大基数匹配，返回匹配数和右侧各点的配对。
 * @note 左侧邻接 g，右侧匹配数组 r 初始全 -1；两侧分别 0-based。返回匹配数，r[v] 是匹配左点。
 * @time O(m sqrt(n))
 * @depends contest/Base.h
 * @author Adam Soltan; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/HopcroftKarp.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

int hopcroftKarp(vector<vector<int>> &g,
                 vector<int> &r) {
	int n = (int)(g).size(), res = 0;
	vector<int> l(n, -1), q(n), d(n);
	auto dfs = [&](auto f, int u) -> bool {
		int t = exchange(d[u], 0) + 1;
		for (int v : g[u])
			if (r[v] == -1 ||
			    (d[r[v]] == t && f(f, r[v])))
				return l[u] = v, r[v] = u, 1;
		return 0;
	};
	for (int t = 0, f = 0;; t = f = 0, d.assign(n, 0)) {
		for (int i = 0; i < n; i ++)
			if (l[i] == -1) q[t ++] = i, d[i] = 1;
		for (int i = 0; i < t; i ++)
			for (int v : g[q[i]]) {
				if (r[v] == -1)
					f = 1;
				else if (!d[r[v]])
					d[r[v]] = d[q[i]] + 1,
					q[t ++] = r[v];
			}
		if (!f) return res;
		for (int i = 0; i < n; i ++)
			if (l[i] == -1) res += dfs(dfs, i);
	}
}
