/**
 * @title 最小费用流／最小费用最大流
 * @brief 在流量上限内尽量增广并最小化费用，可用于最小费用最大匹配。
 * @note 非负整数容量，支持负费用但初始正容量残量图不得有负环。返回新增流量／费用，lim 控制需求；费用、势能、距离与乘积需不溢出。
 * @time O(nm + 增广次数 m log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct MCF {
	struct Edge {
		int to, rev;
		i64 cap, cost;
	};
	int n;
	vector<vector<Edge>> g;
	MCF(int n) : n(n), g(n) {}
	void add(int u, int v, i64 cap, i64 cost) {
		int a = g[u].size(), b = g[v].size();
		g[u].push_back({ v, b + (u == v), cap, cost });
		g[v].push_back({ u, a, 0, -cost });
	}
	pair<i64, i64> flow(int s, int t,
	                    i64 lim = LLONG_MAX / 4) {
		assert(s != t);
		const i64 inf = LLONG_MAX / 4;
		vector<i64> h(n), dis(n);
		vector<int> pv(n), pe(n);
		// Virtual source reaches every vertex. No
		// negative cycle.
		for (int it = 0; it < n; it ++) {
			bool changed = false;
			for (int u = 0; u < n; u ++)
				for (auto e : g[u])
					if (e.cap &&
					    h[e.to] > h[u] + e.cost)
						h[e.to] = h[u] + e.cost,
						changed = true;
			if (!changed) break;
			assert(it + 1 < n);
		}
		i64 f = 0, cost = 0;
		while (f < lim) {
			fill(dis.begin(), dis.end(), inf);
			dis[s] = 0;
			priority_queue<pair<i64, int>,
			               vector<pair<i64, int>>,
			               greater<pair<i64, int>>>
			    q;
			q.push({ 0, s });
			while (!q.empty()) {
				auto [du, u] = q.top();
				q.pop();
				if (du != dis[u]) continue;
				for (int i = 0; i < (int)g[u].size();
				     i ++) {
					auto e = g[u][i];
					i64 nd =
					    du + e.cost + h[u] - h[e.to];
					if (e.cap && nd < dis[e.to]) {
						dis[e.to] = nd;
						pv[e.to] = u;
						pe[e.to] = i;
						q.push({ nd, e.to });
					}
				}
			}
			if (dis[t] == inf) break;
			for (int u = 0; u < n; u ++)
				h[u] += min(dis[u], dis[t]);
			i64 z = lim - f;
			for (int v = t; v != s; v = pv[v])
				z = min(z, g[pv[v]][pe[v]].cap);
			for (int v = t; v != s; v = pv[v]) {
				auto &e = g[pv[v]][pe[v]];
				e.cap -= z;
				g[v][e.rev].cap += z;
				cost += z * e.cost;
			}
			f += z;
		}
		return { f, cost };
	}
};
