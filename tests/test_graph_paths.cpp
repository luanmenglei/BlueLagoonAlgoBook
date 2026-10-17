#include "Test.h"
#include "../content/graph/Mandatory.h"
#include "../content/graph/ReplacementPaths.h"

using Edges = vector<tuple<int, int, i64>>;
constexpr i64 INF = LLONG_MAX / 4;
// Independent Floyd oracle: delete by original edge ID
// or vertex.
i64 distance(int n, const Edges &ed, int s, int t,
             bool undirected, int ban = -1,
             int vertex = -1) {
	if (s == vertex || t == vertex) return INF;
	vector<vector<i64>> d(n, vector<i64>(n, INF));
	for (int i = 0; i < n; i ++)
		d[i][i] = 0;
	for (int i = 0; i < (int)ed.size(); i ++) {
		auto [u, v, w] = ed[i];
		if (i == ban || u == vertex || v == vertex)
			continue;
		d[u][v] = min(d[u][v], w);
		if (undirected) d[v][u] = min(d[v][u], w);
	}
	for (int k = 0; k < n; k ++)
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < n; j ++)
				d[i][j] =
				    min(d[i][j], d[i][k] + d[k][j]);
	return d[s][t];
}
void directed(int n, const Edges &ed, int s, int t) {
	vector<pair<int, int>> plain;
	Edges unit;
	for (auto [u, v, w] : ed)
		plain.push_back({ u, v }),
		    unit.push_back({ u, v, 1 });
	auto a = mandatory_st(n, plain, s, t);
	auto b = mandatory_shortest(n, ed, s, t);
	i64 d = distance(n, ed, s, t, false);
	CHECK(a.reachable == (d < INF) &&
	      b.reachable == (d < INF));
	for (int i = 0; i < n; i ++) {
		CHECK(a.vertex[i] ==
		      (d < INF && distance(n, unit, s, t, false,
		                           -1, i) == INF));
		CHECK(b.vertex[i] ==
		      (d < INF && distance(n, ed, s, t, false,
		                           -1, i) > d));
	}
	for (int i = 0; i < (int)ed.size(); i ++) {
		CHECK(a.edge[i] ==
		      (d < INF && distance(n, unit, s, t, false,
		                           i) == INF));
		CHECK(b.edge[i] ==
		      (d < INF &&
		       distance(n, ed, s, t, false, i) > d));
	}
}
void undirected(int n, const Edges &ed, int s, int t) {
	ReplacementPaths g(n);
	for (auto [u, v, w] : ed)
		g.add(u, v, w);
	auto a = g.solve(s, t);
	CHECK(a.size() == ed.size());
	for (int i = 0; i < (int)ed.size(); i ++)
		CHECK(a[i] == distance(n, ed, s, t, true, i));
	CHECK(g.solve(t, s) == a);
}
int main() {
	// Every loop-free directed graph on 3 vertices,
	// including all s,t choices.
	for (int mask = 0; mask < 64; mask ++) {
		Edges e;
		int bit = 0;
		for (int u = 0; u < 3; u ++)
			for (int v = 0; v < 3; v ++)
				if (u != v) {
					if (mask >> bit & 1)
						e.push_back({ u, v, bit % 2 });
					bit ++;
				}
		for (int s = 0; s < 3; s ++)
			for (int t = 0; t < 3; t ++)
				directed(3, e, s, t);
	}
	// Every undirected 4-vertex graph with absent /
	// weight 1 / weight 2 edges.
	for (int mask = 0; mask < 729; mask ++) {
		int z = mask;
		Edges e;
		for (int u = 0; u < 4; u ++)
			for (int v = u + 1; v < 4; v ++) {
				if (z % 3) e.push_back({ u, v, z % 3 });
				z /= 3;
			}
		for (int s = 0; s < 4; s ++)
			for (int t = s; t < 4; t ++)
				undirected(4, e, s, t);
	}
	for (int it = 0; it < 800; it ++) {
		int n = ri(1, 9), m = ri(0, 30),
		    s = ri(0, n - 1), t = ri(0, n - 1);
		Edges e;
		for (int i = 0; i < m; i ++)
			e.push_back({ ri(0, n - 1), ri(0, n - 1),
			              ri(0, 8) });
		directed(n, e, s, t);
		for (auto &[u, v, w] : e)
			w ++;
		undirected(n, e, s, t);
	}
	undirected(3,
	           { { 0, 1, 1 },
	             { 0, 1, 1 },
	             { 1, 2, 2 },
	             { 0, 2, 9 },
	             { 1, 1, 1 } },
	           0, 2);
	undirected(3,
	           { { 0, 1, 100000000000000000LL },
	             { 1, 2, 100000000000000000LL },
	             { 0, 2, 300000000000000000LL } },
	           0, 2);
	directed(4,
	         { { 0, 1, 0 },
	           { 1, 2, 0 },
	           { 2, 1, 0 },
	           { 2, 3, 1 },
	           { 0, 3, 1 },
	           { 1, 1, 0 } },
	         0, 3);
	// Long positive chain: all deletions disconnect;
	// labels are iterative.
	ReplacementPaths chain(10000);
	for (int i = 1; i < 10000; i ++)
		chain.add(i - 1, i, 1);
	for (i64 d : chain.solve(0, 9999))
		CHECK(d == INF);
	done("mandatory and replacement paths");
}
