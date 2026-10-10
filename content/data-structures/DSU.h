/**
 * @title 普通／带权并查集
 * @brief 维护集合连通关系，以及合并时给出的两点数值差约束。
 * @note 0-based；d[x] 表示 value[x]-value[parent[x]]；find 后为到根差值。join 返回约束是否一致，普通合并用 w=0。
 * @time 均摊 O(alpha(n))
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct DSU {
	vector<int> p, sz;
	vector<i64> d;
	DSU(int n) : p(n), sz(n, 1), d(n) {
		iota(p.begin(), p.end(), 0);
	}
	int find(int x) {
		if (p[x] == x) return x;
		int y = p[x];
		p[x] = find(y);
		d[x] += d[y];
		return p[x];
	}
	// Enforce value[y] - value[x] = w.
	bool join(int x, int y, i64 w = 0) {
		int a = find(x), b = find(y);
		w += d[x] - d[y];
		if (a == b) return w == 0;
		if (sz[a] < sz[b]) swap(a, b), w = -w;
		p[b] = a;
		d[b] = w;
		sz[a] += sz[b];
		return true;
	}
};
