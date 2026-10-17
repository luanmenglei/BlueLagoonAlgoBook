/**
 * @title 无向正权图单边删除最短路
 * @brief 固定 s,t，预处理每条边独立删除后的 s-t
 * 最短距离。
 * @note
 * 0-based，无向严格正权图，可有平行边和自环；不是累计删边，也不是删点。
 * solve(s,t) 返回按边 ID 排列的答案，断开返回 INF；s=t
 * 时均为 0，可重复 solve。 要求
 * 0<w<INF，所有需表示的有限最短距离小于
 * INF。只返回距离，不恢复替换路径。
 * @usage ReplacementPaths g(3); g.add(0,1,2);
 * g.add(1,2,3); g.add(0,2,9); auto a=g.solve(0,2);
 * @time O((n+m) log(n+m)) 预处理，查表 O(1)，空间
 * O(n+m)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; two shortest path
 * trees and interval minima
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

struct ReplacementPaths {
	static constexpr i64 INF = LLONG_MAX / 4;
	struct Edge {
		int u, v;
		i64 w;
	};
	int n;
	vector<Edge> ed;
	vector<vector<pair<int, int>>>
	    g; // {邻点, 边编号}。
	explicit ReplacementPaths(int n) : n(n), g(n) {}
	int add(int u, int v, i64 w) {
		assert(0 <= u && u < n && 0 <= v && v < n &&
		       0 < w && w < INF);
		int id = ed.size();
		ed.push_back({ u, v, w });
		g[u].push_back({ v, id }),
		    g[v].push_back({ u, id });
		return id;
	}
	struct Tree {
		vector<i64> d;
		vector<int> par,
		    pe; // 父顶点、父边；根与不可达点均为 -1。
	};
	Tree shortest(int root) const {
		Tree a{ vector<i64>(n, INF), vector<int>(n, -1),
			    vector<int>(n, -1) };
		priority_queue<pair<i64, int>,
		               vector<pair<i64, int>>,
		               greater<pair<i64, int>>>
		    q;
		a.d[root] = 0;
		q.push({ 0, root });
		while (!q.empty()) {
			auto [d, u] = q.top();
			q.pop();
			if (d != a.d[u]) continue;
			for (auto [v, id] : g[u])
				if (d + ed[id].w < a.d[v]) {
					a.d[v] = d + ed[id].w;
					a.par[v] = u, a.pe[v] = id;
					q.push({ a.d[v], v });
				}
		}
		return a;
	}
	// 删除树上的 P 边后，每个点所属分支挂在 P
	// 的哪个顶点。
	vector<int> labels(const Tree &a, int root,
	                   const vector<int> &pos) const {
		vector<vector<int>> ch(n);
		for (int v = 0; v < n; v ++)
			if (a.par[v] != -1)
				ch[a.par[v]].push_back(v);
		vector<int> lab(n, -1), st{ root };
		lab[root] = pos[root];
		while (!st.empty()) {
			int u = st.back();
			st.pop_back();
			for (int v : ch[u]) {
				lab[v] = pos[v] == -1 ? lab[u] : pos[v];
				st.push_back(v);
			}
		}
		return lab;
	}
	vector<i64> solve(int s, int t) const {
		assert(0 <= s && s < n && 0 <= t && t < n);
		auto a = shortest(s);
		vector<i64> ans(ed.size(), a.d[t]);
		if (s == t || a.d[t] == INF) return ans;
		auto b = shortest(t);
		vector<int> path, pos(n, -1);
		for (int v = t; v != -1; v = a.par[v])
			path.push_back(v);
		reverse(path.begin(), path.end());
		int k = path.size() - 1;
		vector<bool> on(ed.size());
		for (int i = 0; i <= k; i ++) {
			int u = path[i];
			pos[u] = i;
			if (i < k) {
				int v = path[i + 1];
				// 强制 t 树包含同一条
				// P；正权保证不会形成父指针环。
				b.par[u] = v, b.pe[u] = a.pe[v];
				on[a.pe[v]] = true;
			}
		}
		auto l = labels(a, s, pos),
		     r = labels(b, t, pos);
		vector<i64> tag(4 * k, INF);
		auto update = [&](auto self, int p, int x,
		                  int y, int ql, int qr,
		                  i64 v) -> void {
			if (qr <= x || y <= ql) return;
			if (ql <= x && y <= qr) {
				tag[p] = min(tag[p], v);
				return;
			}
			int m = (x + y) / 2;
			self(self, p * 2, x, m, ql, qr, v);
			self(self, p * 2 + 1, m, y, ql, qr, v);
		};
		for (int id = 0; id < (int)ed.size(); id ++) {
			if (on[id])
				continue; // 只排除这条原边
				          // ID，不能排除平行边。
			auto [u, v, w] = ed[id];
			for (int z = 0; z < 2; z ++, swap(u, v))
				if (l[u] != -1 && r[v] != -1 &&
				    l[u] < r[v]) {
					// s 树前缀 + (u,v) + t 树后缀，绕过
					// P 的 [l[u],r[v]) 边。
					i64 d = a.d[u] + w + b.d[v];
					update(update, 1, 0, k, l[u], r[v],
					       d);
				}
		}
		auto collect = [&](auto self, int p, int x,
		                   int y, i64 v) -> void {
			v = min(v, tag[p]);
			if (y - x == 1) {
				ans[a.pe[path[y]]] = v;
				return;
			}
			int m = (x + y) / 2;
			self(self, p * 2, x, m, v);
			self(self, p * 2 + 1, m, y, v);
		};
		collect(collect, 1, 0, k, INF);
		return ans;
	}
};
