/**
 * @title 离线动态最小生成树（边权修改）
 * @brief 对固定连通图的离线边权修改，求每次修改后的 MST 总权。
 * @note 固定的连通无向图，0-based，允许自环／平行边。updates 为(边编号,新权)，返回每次修改后的 MST 总权；不支持直接加删边。构造保存初始权，solve
 * 可重复调用。i64 总权不得溢出。
 * @usage `DynamicMST d(3,{{0,1,1},{1,2,2},{0,2,5}});`；`d.solve({{2,0}})` 得 {1}。
 * @time 典型 O((m+q) log m log q)，回滚与排序常数较大
 * @depends contest/Base.h, data-structures/UnionFindRollback.h
 * @author koosaga; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/offline_dynamic_mst.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../data-structures/UnionFindRollback.h"

struct DynamicMST {
	struct Edge {
		int u, v;
		i64 w;
	};
	int n;
	vector<Edge> ed;
	vector<pair<int, i64>> qr;
	vector<int> cnt;
	vector<i64> w, ans;
	RollbackUF uf;
	DynamicMST(int n, vector<Edge> ed)
	    : n(n), ed(move(ed)), uf(n) {}
	void contract(int l, int r, vector<int> v,
	              vector<int> &must,
	              vector<int> &maybe) {
		sort(v.begin(), v.end(), [&](int a, int b) {
			return tie(w[a], a) < tie(w[b], b);
		});
		int t = uf.time();
		for (int i = l; i <= r; i ++) {
			auto e = ed[qr[i].first];
			uf.join(e.u, e.v);
		}
		for (int i : v)
			if (uf.join(ed[i].u, ed[i].v))
				must.push_back(i);
		uf.rollback(t);
		for (int i : must)
			uf.join(ed[i].u, ed[i].v);
		for (int i : v)
			if (uf.join(ed[i].u, ed[i].v))
				maybe.push_back(i);
		uf.rollback(t);
	}
	void rec(int l, int r, vector<int> v, i64 base) {
		if (l == r) {
			int x = qr[l].first;
			w[x] = qr[l].second;
			if (uf.find(ed[x].u) == uf.find(ed[x].v))
				ans[l] = base;
			else {
				i64 best = w[x];
				for (int i : v)
					best = min(best, w[i]);
				ans[l] = base + best;
			}
			return;
		}
		int mid = (l + r) / 2;
		vector<int> a = v, b = v, must, maybe;
		for (int i = mid + 1; i <= r; i ++)
			if (!--cnt[qr[i].first])
				a.push_back(qr[i].first);
		contract(l, mid, a, must, maybe);
		int t = uf.time();
		i64 cost = base;
		for (int i : must)
			cost += w[i], uf.join(ed[i].u, ed[i].v);
		rec(l, mid, maybe, cost);
		uf.rollback(t);
		for (int i = mid + 1; i <= r; i ++)
			cnt[qr[i].first] ++;
		for (int i = l; i <= mid; i ++)
			if (!--cnt[qr[i].first])
				b.push_back(qr[i].first);
		must.clear();
		maybe.clear();
		contract(mid + 1, r, b, must, maybe);
		cost = base;
		for (int i : must)
			cost += w[i], uf.join(ed[i].u, ed[i].v);
		rec(mid + 1, r, maybe, cost);
		uf.rollback(t);
		for (int i = l; i <= mid; i ++)
			cnt[qr[i].first] ++;
	}
	vector<i64> solve(vector<pair<int, i64>> updates) {
		qr = move(updates);
		ans.assign(qr.size(), 0);
		if (qr.empty()) return ans;
		uf = RollbackUF(n);
		for (auto e : ed)
			uf.join(e.u, e.v);
		assert(n >= 1 && uf.size(0) == n);
		uf.rollback(0);
		w.clear();
		for (auto e : ed)
			w.push_back(e.w);
		cnt.assign(ed.size(), 0);
		for (auto [i, x] : qr) {
			assert(0 <= i && i < (int)ed.size());
			cnt[i] ++;
		}
		vector<int> v;
		for (int i = 0; i < (int)ed.size(); i ++)
			if (!cnt[i]) v.push_back(i);
		rec(0, qr.size() - 1, v, 0);
		return ans;
	}
};
