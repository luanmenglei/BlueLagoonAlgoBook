/**
 * @title 有上下界可行流／最大流／最小流
 * @brief 求满足边流量上下界的可行环流及非负源汇流的最大／最小值。
 * @note 0-based；无源汇环流调用 feasible。solve 求非负 s→t 流的最大／最小值；对象只求解一次。原边流量=下界+d.used(u,id)。容量与平衡量须小于
 * INF。
 * @time 一次可行性最大流＋一次残量最大流
 * @depends contest/Base.h, flow/Dinic.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../flow/Dinic.h"

struct LowerBound {
	int n, ss, tt;
	Dinic d;
	vector<i64> bal;
	LowerBound(int n)
	    : n(n), ss(n), tt(n + 1), d(n + 2), bal(n) {}
	int add(int u, int v, i64 l, i64 r) {
		assert(0 <= l && l <= r);
		bal[u] -= l;
		bal[v] += l;
		return d.add(u, v, r - l);
	}
	bool feasible() {
		i64 need = 0;
		for (int i = 0; i < n; i ++) {
			if (bal[i] > 0)
				d.add(ss, i, bal[i]), need += bal[i];
			if (bal[i] < 0) d.add(i, tt, -bal[i]);
		}
		return d.flow(ss, tt) == need;
	}
	optional<i64> solve(int s, int t,
	                    bool maximum = true) {
		int id = d.add(t, s, LLONG_MAX / 4);
		if (!feasible()) return nullopt;
		i64 base = d.used(t, id);
		for (int u = 0; u < n + 2; u ++)
			for (auto &e : d.g[u])
				if (u >= n || e.to >= n) e.cap = 0;
		auto &e = d.g[t][id];
		d.g[s][e.rev].cap = e.cap = 0;
		return maximum ? base + d.flow(s, t)
		               : base - d.flow(t, s, base);
	}
};
