/**
 * @title 离线动态连通性
 * @brief 离线处理边的加入／删除时段，查询各时刻的连通关系。
 * @note t>=1，时刻 0..t-1；边有效期 [x,y) 加入根 (1,0,t)。删除时配对同一条边的加入时刻；重复边需计数或分别编号；叶回调查询 DSU。
 * @time O(边有效期数 log t log n + 查询数 log n)
 * @depends contest/Base.h, data-structures/UnionFindRollback.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../data-structures/UnionFindRollback.h"

struct DynamicConnectivity {
	int t;
	RollbackUF dsu;
	vector<vector<pair<int, int>>> ed;
	DynamicConnectivity(int n, int t)
	    : t(t), dsu(n), ed(4 * t + 4) {}
	void add(int p, int l, int r, int x, int y,
	         pair<int, int> e) {
		if (x >= r || y <= l) return;
		if (x <= l && r <= y) {
			ed[p].push_back(e);
			return;
		}
		int m = (l + r) / 2;
		add(p * 2, l, m, x, y, e);
		add(p * 2 + 1, m, r, x, y, e);
	}
	template <class F>
	void solve(int p, int l, int r, F &f) {
		int tm = dsu.time();
		for (auto [u, v] : ed[p])
			dsu.join(u, v);
		if (r - l == 1)
			f(l, dsu);
		else {
			int m = (l + r) / 2;
			solve(p * 2, l, m, f);
			solve(p * 2 + 1, m, r, f);
		}
		dsu.rollback(tm);
	}
};
