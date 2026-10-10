/**
 * @title K 短路／K 最短游走（Eppstein）
 * @brief 求非负权有向图前 k 条最短游走费用，允许重复顶点和边。
 * @note 非负 i64 边权；允许重复点、重复边，平行边视为不同选择。返回按费用排序的至多 k 条游走；s=t 包含空游走；零环可产生无限多条同费用结果。距离与返回费用必须
 * <LLONG_MAX/4。
 * @usage `KShortest g(2);`；add(0,1,2)、add(1,1,1) 后 `g.solve(0,1,3)` 得 {2,3,4}。
 * @time O((n+m) log n + m log n + k log k)，持久堆空间 O(m log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct KShortest {
	struct Edge {
		int u, v;
		i64 w;
	};
	struct Node {
		int l = 0, r = 0, rank = 1, to = 0;
		i64 w = 0;
	};
	int n;
	vector<Edge> ed;
	vector<Node> tr;
	KShortest(int n) : n(n) {}
	void add(int u, int v, i64 w) {
		assert(w >= 0);
		ed.push_back({ u, v, w });
	}
	int merge(int a, int b) {
		if (!a || !b) return a ? a : b;
		if (tr[a].w > tr[b].w) swap(a, b);
		int c = tr.size();
		tr.push_back(tr[a]);
		int r = merge(tr[c].r, b);
		tr[c].r = r;
		if (tr[tr[c].l].rank < tr[r].rank)
			swap(tr[c].l, tr[c].r);
		tr[c].rank = tr[tr[c].r].rank + 1;
		return c;
	}
	vector<i64> solve(int s, int t, int k) {
		const i64 inf = LLONG_MAX / 4;
		vector<vector<int>> rev(n), out(n);
		for (int i = 0; i < (int)ed.size(); i ++)
			rev[ed[i].v].push_back(i),
			    out[ed[i].u].push_back(i);
		vector<i64> d(n, inf);
		vector<int> parent(n, -1), order, h(n);
		priority_queue<pair<i64, int>,
		               vector<pair<i64, int>>,
		               greater<pair<i64, int>>>
		    q;
		d[t] = 0;
		q.push({ 0, t });
		while (!q.empty()) {
			auto [x, u] = q.top();
			q.pop();
			if (x != d[u]) continue;
			order.push_back(u);
			for (int j : rev[u]) {
				auto e = ed[j];
				if (e.w < inf - x && d[e.u] > x + e.w)
					d[e.u] = x + e.w, parent[e.u] = j,
					q.push({ d[e.u], e.u });
			}
		}
		if (k <= 0 || d[s] == inf) return {};
		tr.assign(1, {});
		tr[0].rank = 0;
		for (int u : order) {
			if (parent[u] >= 0)
				h[u] = h[ed[parent[u]].v];
			for (int j : out[u])
				if (j != parent[u] &&
				    d[ed[j].v] < inf) {
					auto e = ed[j];
					i128 delta =
					    i128(e.w) + d[e.v] - d[u];
					assert(0 <= delta && delta < inf);
					int a = tr.size();
					tr.push_back(
					    { 0, 0, 1, e.v, (i64)delta });
					h[u] = merge(h[u], a);
				}
		}
		vector<i64> ans{ d[s] };
		priority_queue<pair<i64, int>,
		               vector<pair<i64, int>>,
		               greater<pair<i64, int>>>
		    pq;
		auto push = [&](i128 x, int v) {
			if (v) {
				assert(x < inf);
				pq.push({ (i64)x, v });
			}
		};
		push(i128(d[s]) + (h[s] ? tr[h[s]].w : 0),
		     h[s]);
		while ((int)ans.size() < k && !pq.empty()) {
			auto [x, u] = pq.top();
			pq.pop();
			ans.push_back(x);
			for (int v : { tr[u].l, tr[u].r })
				if (v)
					push(i128(x) - tr[u].w + tr[v].w,
					     v);
			int v = h[tr[u].to];
			if (v) push(i128(x) + tr[v].w, v);
		}
		return ans;
	}
};
