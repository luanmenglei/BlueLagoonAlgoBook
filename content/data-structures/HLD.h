/**
 * @title 树链剖分／LCA／路径区间
 * @brief 将树上路径和子树转为数组区间，求 LCA，支持按路径方向聚合。
 * @note 0-based，无向树；子树为 [in[u],out[u])。path 回调半开区间，适合可交换运算；edge=true 表示边权存在较深端点。深链改迭代 DFS。
 * path_ordered(u,v,edge) 按 u→v 顺序返回 {l,r,rev}；rev=true 时反向读取 seq[l..r)，适用于非交换聚合；边模式排除
 * LCA。例：拼接各段字符得到整条有向路径字符串。
 * @time 预处理 O(n)，LCA O(log n)，路径 O(log n) 段
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct HLD {
	int n, timer = 0;
	vector<vector<int>> g;
	vector<int> p, dep, sz, son, top, in, out, seq;
	HLD(int n)
	    : n(n), g(n), p(n, -1), dep(n), sz(n),
	      son(n, -1), top(n), in(n), out(n), seq(n) {}
	void dfs(int u) {
		sz[u] = 1;
		for (int v : g[u])
			if (v != p[u]) {
				p[v] = u;
				dep[v] = dep[u] + 1;
				dfs(v);
				sz[u] += sz[v];
				if (son[u] == -1 || sz[v] > sz[son[u]])
					son[u] = v;
			}
	}
	void build(int u, int t) {
		top[u] = t;
		in[u] = timer;
		seq[timer ++] = u;
		if (son[u] != -1) build(son[u], t);
		for (int v : g[u])
			if (v != p[u] && v != son[u]) build(v, v);
		out[u] = timer;
	}
	void init(int r = 0) {
		timer = 0;
		p[r] = -1;
		dep[r] = 0;
		fill(son.begin(), son.end(), -1);
		dfs(r);
		build(r, r);
	}
	int lca(int u, int v) const {
		while (top[u] != top[v]) {
			if (dep[top[u]] < dep[top[v]]) swap(u, v);
			u = p[top[u]];
		}
		return dep[u] < dep[v] ? u : v;
	}
	struct Segment {
		int l, r;
		bool rev;
	};
	vector<Segment>
	path_ordered(int u, int v,
	             bool edge = false) const {
		vector<Segment> a, b;
		while (top[u] != top[v]) {
			if (dep[top[u]] > dep[top[v]]) {
				a.push_back(
				    { in[top[u]], in[u] + 1, true });
				u = p[top[u]];
			} else {
				b.push_back(
				    { in[top[v]], in[v] + 1, false });
				v = p[top[v]];
			}
		}
		if (dep[u] >= dep[v]) {
			if (in[v] + edge < in[u] + 1)
				a.push_back(
				    { in[v] + edge, in[u] + 1, true });
		} else if (in[u] + edge < in[v] + 1)
			b.push_back(
			    { in[u] + edge, in[v] + 1, false });
		reverse(b.begin(), b.end());
		a.insert(a.end(), b.begin(), b.end());
		return a;
	}
	template <class F>
	void path(int u, int v, F f,
	          bool edge = false) const {
		while (top[u] != top[v]) {
			if (dep[top[u]] < dep[top[v]]) swap(u, v);
			f(in[top[u]], in[u] + 1);
			u = p[top[u]];
		}
		if (dep[u] > dep[v]) swap(u, v);
		if (in[u] + edge <= in[v])
			f(in[u] + edge, in[v] + 1);
	}
};
