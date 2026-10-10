/**
 * @title 支配树
 * @brief 求从入口出发到达某点时必经的支配关系，返回直接支配点。
 * @note 0-based 有向图；返回直接支配点，根指向自身，不可达为 -1。每个对象求解一次；递归栈按图深度准备。
 * @time O((n+m) log n)
 * @depends contest/Base.h
 * @author koosaga; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/dominator.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Dominator {
	int timer = 0;
	vector<vector<int>> g, rg, bucket;
	vector<int> dfn, seq, par, best, semi, parent, dom;
	Dominator(int n)
	    : g(n), rg(n + 1), bucket(n + 1), dfn(n),
	      seq(n + 1), par(n + 1), best(n + 1),
	      semi(n + 1), parent(n + 1), dom(n + 1) {}
	int find(int x, int depth = 0) {
		if (par[x] == x) return depth ? -1 : x;
		int p = find(par[x], depth + 1);
		if (p == -1) return depth ? par[x] : best[x];
		if (semi[best[x]] > semi[best[par[x]]])
			best[x] = best[par[x]];
		par[x] = p;
		return depth ? p : best[x];
	}
	void dfs(int u) {
		int x = dfn[u] = ++timer;
		seq[x] = u;
		par[x] = best[x] = semi[x] = x;
		for (int v : g[u]) {
			if (!dfn[v]) dfs(v), parent[dfn[v]] = x;
			rg[dfn[v]].push_back(x);
		}
	}
	vector<int> solve(int root) {
		dfs(root);
		for (int i = timer; i; i --) {
			for (int v : rg[i])
				semi[i] = min(semi[i], semi[find(v)]);
			if (i > 1) bucket[semi[i]].push_back(i);
			for (int v : bucket[i]) {
				int p = find(v);
				dom[v] = semi[p] == i ? i : p;
			}
			if (i > 1) par[i] = parent[i];
		}
		vector<int> ans(g.size(), -1);
		ans[root] = root;
		for (int i = 2; i <= timer; i ++) {
			if (semi[i] != dom[i]) dom[i] = dom[dom[i]];
			ans[seq[i]] = seq[dom[i]];
		}
		return ans;
	}
};
