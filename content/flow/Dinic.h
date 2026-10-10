/**
 * @title 最大流／最小割（Dinic）
 * @brief 求 s 到 t 最大流，恢复原边流量，并定位最小割的源侧。
 * @note 0-based，非负容量，返回新增流量；默认 lim=LLONG_MAX/4。add 返回邻接边编号。终止后另跑 bfs 得最小割可达侧；反向初始容量非零时 used
 * 是净流。
 * @time O(n^2 m)，二分图单位网络 O(m sqrt(n))
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Dinic {
	struct Edge {
		int to, rev;
		i64 cap, init;
	};
	vector<vector<Edge>> g;
	vector<int> dep, cur;
	Dinic(int n) : g(n), dep(n), cur(n) {}
	int add(int u, int v, i64 c, i64 rc = 0) {
		int a = g[u].size(), b = g[v].size();
		g[u].push_back({ v, b + (u == v), c, c });
		g[v].push_back({ u, a, rc, rc });
		return a;
	}
	bool bfs(int s, int t) {
		fill(dep.begin(), dep.end(), -1);
		queue<int> q;
		dep[s] = 0;
		q.push(s);
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (auto e : g[u])
				if (e.cap && dep[e.to] == -1)
					dep[e.to] = dep[u] + 1,
					q.push(e.to);
		}
		return dep[t] != -1;
	}
	i64 dfs(int u, int t, i64 f) {
		if (u == t) return f;
		for (int &i = cur[u]; i < (int)g[u].size();
		     i ++) {
			auto &e = g[u][i];
			if (!e.cap || dep[e.to] != dep[u] + 1)
				continue;
			i64 v = dfs(e.to, t, min(f, e.cap));
			if (v) {
				e.cap -= v;
				g[e.to][e.rev].cap += v;
				return v;
			}
		}
		return 0;
	}
	i64 flow(int s, int t, i64 lim = LLONG_MAX / 4) {
		assert(s != t);
		i64 ans = 0;
		while (ans < lim && bfs(s, t)) {
			fill(cur.begin(), cur.end(), 0);
			while (i64 f = dfs(s, t, lim - ans))
				ans += f;
		}
		return ans;
	}
	i64 used(int u, int id) const {
		return g[u][id].init - g[u][id].cap;
	}
};
