#include "Test.h"
#include "../content/advanced/GeneralWeightedMatching.h"
#include "../content/advanced/MinCostCirculation.h"
#include "../content/advanced/DirectedMSTFast.h"
#include "../content/advanced/DynamicMST.h"
#include "../content/advanced/KShortest.h"
#include "../content/advanced/Chordal.h"
#include "../content/advanced/EdgeColoring.h"
#include "../content/advanced/MatroidIntersection.h"

bool peo(const vector<vector<int>> &g,
         const vector<int> &p) {
	int n = g.size();
	vector<int> pos(n);
	for (int i = 0; i < n; i ++)
		pos[p[i]] = i;
	for (int u = 0; u < n; u ++)
		for (int v : g[u])
			for (int w : g[u])
				if (pos[v] > pos[u] &&
				    pos[w] > pos[u] && v != w &&
				    find(g[v].begin(), g[v].end(), w) ==
				        g[v].end())
					return false;
	return true;
}
int main() {
	for (int it = 0; it < 1200; it ++) {
		int n = ri(0, 10);
		vector<vector<i64>> w(n, vector<i64>(n));
		weighted_blossom::init(n);
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < i; j ++)
				if (ri(0, 2)) {
					i64 x = ri(1, 100);
					if (it % 20 == 0)
						x *= 10000000000LL;
					w[i][j] = w[j][i] = x;
					weighted_blossom::add_edge(
					    i + 1, j + 1, x);
				}
		vector<i64> d(1 << n, -1);
		function<i64(int)> dp = [&](int mask) {
			if (!mask) return 0LL;
			i64 &v = d[mask];
			if (v >= 0) return v;
			int u = __builtin_ctz((unsigned)mask);
			v = dp(mask ^ (1 << u));
			for (int j = u + 1; j < n; j ++)
				if (mask >> j & 1)
					v = max(v, w[u][j] + dp(mask ^
					                        (1 << u) ^
					                        (1 << j)));
			return v;
		};
		auto [value, count] = weighted_blossom::solve();
		CHECK(value == dp((1 << n) - 1));
		i64 total = 0;
		int c = 0;
		for (int i = 1; i <= n; i ++) {
			int j = weighted_blossom::match[i];
			if (j) {
				CHECK(j <= n &&
				      weighted_blossom::match[j] == i &&
				      w[i - 1][j - 1] > 0);
				if (j < i)
					c ++, total += w[i - 1][j - 1];
			}
		}
		CHECK(total == value && c == count);
	}
	for (int it = 0; it < 1000; it ++) {
		int n = ri(1, 4), m = ri(0, 7);
		MinCostCirculation f(n);
		for (int i = 0; i < m; i ++) {
			int hi = ri(0, 2), lo = ri(0, hi);
			f.add(ri(0, n - 1), ri(0, n - 1), lo, hi,
			      ri(-5, 5));
		}
		optional<i128> best;
		vector<i64> bal(n);
		function<void(int, i128)> dfs = [&](int i,
		                                    i128 cost) {
			if (i == m) {
				if (all_of(bal.begin(), bal.end(),
				           [](i64 x) { return !x; }) &&
				    (!best || cost < *best))
					best = cost;
				return;
			}
			auto e = f.ed[i];
			for (i64 x = e.lo; x <= e.hi; x ++) {
				bal[e.u] -= x;
				bal[e.v] += x;
				dfs(i + 1, cost + i128(x) * e.cost);
				bal[e.u] += x;
				bal[e.v] -= x;
			}
		};
		dfs(0, 0);
		auto got = f.solve();
		CHECK(got == best);
		if (got) {
			i128 cost = 0;
			fill(bal.begin(), bal.end(), 0);
			for (int i = 0; i < m; i ++) {
				auto e = f.ed[i];
				i64 x = f.flow[i];
				CHECK(e.lo <= x && x <= e.hi);
				bal[e.u] -= x;
				bal[e.v] += x;
				cost += i128(x) * e.cost;
			}
			CHECK(cost == *got);
			for (auto x : bal)
				CHECK(x == 0);
		}
	}
	for (int it = 0; it < 1000; it ++) {
		int n = ri(1, 5), root = ri(0, n - 1),
		    m = ri(0, 12);
		vector<arborescence::Edge> ed;
		vector<vector<int>> incoming(n);
		for (int i = 0; i < m; i ++) {
			int u = ri(0, n - 1), v = ri(0, n - 1);
			ed.push_back({ u, v, ri(-10, 20) });
			if (u != v && v != root)
				incoming[v].push_back(i);
		}
		optional<i64> best;
		vector<int> chosen(n, -1);
		function<void(int)> dfs = [&](int v) {
			if (v == n) {
				i64 cost = 0;
				for (int x = 0; x < n; x ++)
					if (x != root) {
						vector<int> seen(n);
						int y = x;
						while (y != root && !seen[y])
							seen[y] = 1,
							y = ed[chosen[y]].a;
						if (y != root) return;
						cost += ed[chosen[x]].w;
					}
				if (!best || cost < *best) best = cost;
				return;
			}
			if (v == root) {
				dfs(v + 1);
				return;
			}
			for (int i : incoming[v])
				chosen[v] = i, dfs(v + 1);
		};
		dfs(0);
		auto got = arborescence::dmst(n, root, ed);
		CHECK(got.has_value() == best.has_value());
		if (got) {
			CHECK(got->cost == *best);
			i64 cost = 0;
			for (int v = 0; v < n; v ++)
				if (v != root) {
					int id = got->edge[v];
					CHECK(id >= 0 && id < m &&
					      ed[id].b == v &&
					      ed[id].a == got->parent[v]);
					cost += ed[id].w;
					int x = v;
					for (int step = 0;
					     step < n && x != root; step ++)
						x = got->parent[x];
					CHECK(x == root);
				}
			CHECK(got->edge[root] == -1 &&
			      cost == got->cost);
		}
	}
	for (int it = 0; it < 300; it ++) {
		int n = ri(1, 10);
		vector<DynamicMST::Edge> ed;
		for (int v = 1; v < n; v ++)
			ed.push_back(
			    { v, ri(0, v - 1), ri(-10, 10) });
		for (int j = 0; j < 15; j ++)
			ed.push_back({ ri(0, n - 1), ri(0, n - 1),
			               ri(-10, 10) });
		DynamicMST d(n, ed);
		vector<pair<int, i64>> updates;
		for (int j = 0; j < 30; j ++)
			updates.emplace_back(ri(0, ed.size() - 1),
			                     ri(-20, 20));
		auto got = d.solve(updates);
		CHECK(d.solve(updates) == got);
		for (int t = 0; t < 30; t ++) {
			ed[updates[t].first].w = updates[t].second;
			vector<int> p(n), id(ed.size());
			iota(p.begin(), p.end(), 0);
			iota(id.begin(), id.end(), 0);
			function<int(int)> f = [&](int x) {
				return p[x] == x ? x : p[x] = f(p[x]);
			};
			sort(id.begin(), id.end(),
			     [&](int a, int b) {
				     return ed[a].w < ed[b].w;
			     });
			i64 cost = 0;
			for (int i : id)
				if (f(ed[i].u) != f(ed[i].v))
					cost += ed[i].w,
					    p[f(ed[i].u)] = f(ed[i].v);
			CHECK(got[t] == cost);
		}
	}
	for (int it = 0; it < 1000; it ++) {
		int n = ri(1, 7), s = ri(0, n - 1),
		    t = ri(0, n - 1), k = ri(0, 20);
		KShortest ks(n);
		vector<vector<pair<int, int>>> g(n), rev(n);
		for (int j = 0; j < ri(0, 20); j ++) {
			int u = ri(0, n - 1), v = ri(0, n - 1),
			    w = ri(0, 9);
			ks.add(u, v, w);
			g[u].emplace_back(v, w);
			rev[v].emplace_back(u, w);
		}
		vector<int> reach(n);
		queue<int> q;
		q.push(t);
		reach[t] = 1;
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (auto [v, w] : rev[u])
				if (!reach[v]) reach[v] = 1, q.push(v);
		}
		vector<i64> expected;
		vector<int> count(n);
		priority_queue<pair<i64, int>,
		               vector<pair<i64, int>>,
		               greater<pair<i64, int>>>
		    pq;
		if (reach[s]) pq.push({ 0, s });
		while ((int)expected.size() < k &&
		       !pq.empty()) {
			auto [x, u] = pq.top();
			pq.pop();
			if (count[u] ++ >= k) continue;
			if (u == t) expected.push_back(x);
			for (auto [v, w] : g[u])
				if (reach[v]) pq.push({ x + w, v });
		}
		CHECK(ks.solve(s, t, k) == expected);
		CHECK(ks.solve(s, t, k) == expected);
	}
	for (int it = 0; it < 250; it ++) {
		int n = ri(1, 6);
		vector<vector<int>> g(n);
		vector<int> p(n);
		iota(p.begin(), p.end(), 0);
		for (int u = 0; u < n; u ++)
			for (int v = 0; v < u; v ++)
				if (ri(0, 1))
					g[u].push_back(v),
					    g[v].push_back(u);
		bool expected = false;
		do {
			if (peo(g, p)) {
				expected = true;
				break;
			}
		} while (next_permutation(p.begin(), p.end()));
		auto got = getPEO(g);
		CHECK(!got.empty() == expected);
		if (!got.empty()) CHECK(peo(g, got));
	}
	for (int it = 0; it < 500; it ++) {
		int n = ri(1, 30);
		vector<pair<int, int>> ed;
		vector<int> deg(n);
		for (int u = 0; u < n; u ++)
			for (int v = 0; v < u; v ++)
				if (ri(0, 4) == 0)
					ed.emplace_back(u, v), deg[u] ++,
					    deg[v] ++;
		int delta =
		    *max_element(deg.begin(), deg.end());
		auto colors = edgeColoring(n, ed);
		vector<set<int>> used(n);
		for (int j = 0; j < (int)ed.size(); j ++) {
			auto [u, v] = ed[j];
			CHECK(0 <= colors[j] && colors[j] <= delta);
			CHECK(used[u].insert(colors[j]).second &&
			      used[v].insert(colors[j]).second);
		}
	}
	for (int it = 0; it < 300; it ++) {
		int n = ri(0, 11), vertices = ri(1, 6);
		vector<int> color(n);
		vector<pair<int, int>> ed;
		for (int i = 0; i < n; i ++)
			color[i] = ri(0, 3),
			ed.emplace_back(ri(0, vertices - 1),
			                ri(0, vertices - 1));
		auto a = [&](vector<int> s) {
			set<int> c;
			for (int i : s)
				if (!c.insert(color[i]).second)
					return false;
			return true;
		};
		auto b = [&](vector<int> s) {
			vector<int> p(vertices);
			iota(p.begin(), p.end(), 0);
			function<int(int)> f = [&](int x) {
				return p[x] == x ? x : p[x] = f(p[x]);
			};
			for (int i : s) {
				auto [u, v] = ed[i];
				if (f(u) == f(v)) return false;
				p[f(u)] = f(v);
			}
			return true;
		};
		int best = 0;
		for (int mask = 0; mask < (1 << n); mask ++) {
			vector<int> s;
			for (int i = 0; i < n; i ++)
				if (mask >> i & 1) s.push_back(i);
			if (a(s) && b(s))
				best = max(best, (int)s.size());
		}
		auto got = matroid_intersection(n, a, b);
		CHECK((int)got.size() == best && a(got) &&
		      b(got));
	}
	done("advanced graph");
}
