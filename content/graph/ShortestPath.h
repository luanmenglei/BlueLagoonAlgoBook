/**
 * @title 非负权最短路／负环检测
 * @brief 求单源非负权最短路，并提供任意位置负环的检测与恢复。
 * @note 0-based。Dijkstra 要求非负边和最短路小于 INF；负环检测相当于接超级源，返回环顶点（不重复首点），支持任意位置负环。
 * @time Dijkstra O((n+m) log n)，负环 O(nm)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<i64>
dijkstra(const vector<vector<pair<int, i64>>> &g,
         int s) {
	const i64 inf = LLONG_MAX / 4;
	vector<i64> d(g.size(), inf);
	priority_queue<pair<i64, int>,
	               vector<pair<i64, int>>,
	               greater<pair<i64, int>>>
	    q;
	d[s] = 0;
	q.push({ 0, s });
	while (!q.empty()) {
		auto [dis, u] = q.top();
		q.pop();
		if (dis != d[u]) continue;
		for (auto [v, w] : g[u])
			if (d[v] > dis + w) {
				d[v] = dis + w;
				q.push({ d[v], v });
			}
	}
	return d;
}
vector<int>
negative_cycle(int n,
               const vector<tuple<int, int, i64>> &ed) {
	vector<i128> d(n);
	vector<int> p(n, -1);
	int x = -1;
	for (int i = 0; i < n; i ++) {
		x = -1;
		for (auto [u, v, w] : ed)
			if (d[v] > d[u] + w)
				d[v] = d[u] + w, p[v] = u, x = v;
	}
	if (x == -1) return {};
	for (int i = 0; i < n; i ++)
		x = p[x];
	vector<int> a = { x };
	for (int v = p[x]; v != x; v = p[v])
		a.push_back(v);
	reverse(a.begin(), a.end());
	return a;
}
