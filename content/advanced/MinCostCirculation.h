/**
 * @title 有上下界最小费用环流（允许负环）
 * @brief 求满足流量上下界的最低费用环流，允许有限容量负费用环。
 * @note 0-based，有限整数容量 0<=lo<=hi。先求可行流，再消残量负环；solve 返回 i128 费用或 nullopt，flow
 * 为各原边流量。允许自环和平行边。必须检查总容量／余额不超过 i64。消环版适合中小规模，不替代大规模 cost scaling。
 * @usage `MinCostCirculation f(1);`；`f.add(0,0,0,3,-2);`；`f.solve()` 为-6，`f.flow[0]` 为3。
 * @time 可行性 Dinic；优化 O(C nm)，C 为消环次数，整数费用时伪多项式
 * @depends contest/Base.h, flow/Dinic.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../flow/Dinic.h"

struct MinCostCirculation {
	struct Edge {
		int u, v;
		i64 lo, hi, cost;
	};
	struct Arc {
		int u, v;
		i64 cap, cost;
	};
	int n;
	vector<Edge> ed;
	vector<i64> flow;
	MinCostCirculation(int n) : n(n) {}
	void add(int u, int v, i64 lo, i64 hi, i64 cost) {
		assert(0 <= lo && lo <= hi &&
		       cost != LLONG_MIN);
		ed.push_back({ u, v, lo, hi, cost });
	}
	optional<i128> solve() {
		Dinic d(n + 2);
		vector<i64> bal(n);
		vector<int> id;
		for (auto e : ed) {
			id.push_back(d.add(e.u, e.v, e.hi - e.lo));
			bal[e.u] -= e.lo;
			bal[e.v] += e.lo;
		}
		i64 need = 0;
		for (int v = 0; v < n; v ++)
			if (bal[v] > 0)
				d.add(n, v, bal[v]), need += bal[v];
			else if (bal[v] < 0)
				d.add(v, n + 1, -bal[v]);
		if (d.flow(n, n + 1, need) != need)
			return nullopt;
		vector<Arc> a;
		flow.resize(ed.size());
		for (int i = 0; i < (int)ed.size(); i ++) {
			auto e = ed[i];
			i64 f = d.used(e.u, id[i]);
			a.push_back(
			    { e.u, e.v, e.hi - e.lo - f, e.cost });
			a.push_back({ e.v, e.u, f, -e.cost });
		}
		for (;;) {
			vector<i128> dist(n);
			vector<int> pre(n, -1);
			int x = -1;
			for (int k = 0; k < n; k ++) {
				x = -1;
				for (int j = 0; j < (int)a.size();
				     j ++) {
					auto e = a[j];
					if (e.cap &&
					    dist[e.v] > dist[e.u] + e.cost)
						dist[e.v] = dist[e.u] + e.cost,
						pre[e.v] = j, x = e.v;
				}
				if (x < 0) break;
			}
			if (x < 0) break;
			for (int k = 0; k < n; k ++)
				x = a[pre[x]].u;
			int v = x;
			vector<int> cyc;
			i64 f = LLONG_MAX;
			do {
				int j = pre[v];
				cyc.push_back(j);
				f = min(f, a[j].cap);
				v = a[j].u;
			} while (v != x);
			for (int j : cyc)
				a[j].cap -= f, a[j ^ 1].cap += f;
		}
		i128 cost = 0;
		for (int i = 0; i < (int)ed.size(); i ++)
			flow[i] = ed[i].lo + a[2 * i + 1].cap,
			cost += i128(flow[i]) * ed[i].cost;
		return cost;
	}
};
