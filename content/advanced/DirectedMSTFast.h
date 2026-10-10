/**
 * @title 有向最小生成树／选边恢复（快速版）
 * @brief 求指定根的最小树形图，并恢复父亲和原输入边编号。
 * @note namespace arborescence；0-based，dmst(n,root,edges) 返回 cost,parent,edge 或
 * nullopt。edge[v] 是原输入边编号，根为-1；允许负权／平行边。权值、对偶偏移、总权不得溢出 i64；堆递归注意栈。
 * @usage `arborescence::dmst(3,0,{{0,1,4},{1,2,2},{0,2,9}})` 总权6，edge 为 {-1,0,1}；检查 optional。
 * @time O(m log n)，空间 O(n+m)
 * @depends contest/Base.h, data-structures/UnionFindRollback.h
 * @author chilli, Takanori MAEHARA, Benq, Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/DirectedMST.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../data-structures/UnionFindRollback.h"

namespace arborescence {

struct Edge {
	int a, b;
	i64 w;
	int id = -1;
};
struct Node { /// lazy skew heap node
	Edge key;
	Node *l, *r;
	i64 delta;
	void prop() {
		key.w += delta;
		if (l) l->delta += delta;
		if (r) r->delta += delta;
		delta = 0;
	}
	Edge top() {
		prop();
		return key;
	}
};
Node *merge(Node *a, Node *b) {
	if (!a || !b) return a ? a : b;
	a->prop(), b->prop();
	if (a->key.w > b->key.w) swap(a, b);
	swap(a->l, (a->r = merge(b, a->r)));
	return a;
}
void pop(Node *&a) {
	a->prop();
	a = merge(a->l, a->r);
}

struct Result {
	i64 cost;
	vector<int> parent, edge;
};
optional<Result> dmst(int n, int r, vector<Edge> g) {
	RollbackUF uf(n);
	vector<Node *> heap(n);
	vector<unique_ptr<Node>> pool;
	for (int j = 0; j < int(g.size()); j ++) {
		Edge e = g[j];
		e.id = j;
		pool.emplace_back(
		    new Node{ e, nullptr, nullptr, 0 });
		heap[e.b] = merge(heap[e.b], pool.back().get());
	}
	i64 res = 0;
	vector<int> seen(n, -1), path(n), par(n);
	seen[r] = r;
	vector<Edge> Q(n), in(n, { -1, -1, 0, -1 });
	deque<tuple<int, int, vector<Edge>>> cycs;
	for (int s = 0; s < n; s ++) {
		int u = s, qi = 0, w;
		while (seen[u] < 0) {
			if (!heap[u]) return nullopt;
			Edge e = heap[u]->top();
			heap[u]->delta -= e.w, pop(heap[u]);
			Q[qi] = e, path[qi ++] = u, seen[u] = s;
			res += e.w, u = uf.find(e.a);
			if (seen[u] ==
			    s) { /// found cycle, contract
				Node *cyc = 0;
				int end = qi, time = uf.time();
				do
					cyc = merge(cyc,
					            heap[w = path[--qi]]);
				while (uf.join(u, w));
				u = uf.find(u), heap[u] = cyc,
				seen[u] = -1;
				cycs.push_front(
				    { u, time, { &Q[qi], &Q[end] } });
			}
		}
		for (int i = 0; i < qi; i ++)
			in[uf.find(Q[i].b)] = Q[i];
	}

	for (auto &[u, t, comp] :
	     cycs) { // restore sol (optional)
		uf.rollback(t);
		Edge inEdge = in[u];
		for (auto &e : comp)
			in[uf.find(e.b)] = e;
		in[uf.find(inEdge.b)] = inEdge;
	}
	for (int i = 0; i < n; i ++)
		par[i] = in[i].a;
	vector<int> ids(n);
	for (int i = 0; i < n; i ++)
		ids[i] = in[i].id;
	return Result{ res, par, ids };
}

} // namespace arborescence
