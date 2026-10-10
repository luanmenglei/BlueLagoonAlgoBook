/**
 * @title 无向图全局最小割
 * @brief 在所有非平凡顶点划分中求最小割权，恢复割的一侧顶点。
 * @note Stoer-Wagner；无向非负权邻接矩阵、对称、n>=2，平行边先求和。返回割权和一侧顶点；对角线忽略，总权须不溢出。
 * @time O(n^3)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

pair<i64, vector<int>> min_cut(vector<vector<i64>> a) {
	int n = a.size();
	assert(n >= 2);
	vector<int> active(n);
	iota(active.begin(), active.end(), 0);
	vector<vector<int>> co(n);
	for (int i = 0; i < n; i ++)
		co[i] = { i };
	pair<i64, vector<int>> ans = { LLONG_MAX, {} };
	while (active.size() > 1) {
		vector<i64> w(n);
		vector<bool> used(n);
		int pre = -1;
		for (int i = 0; i < (int)active.size(); i ++) {
			int v = -1;
			for (int u : active)
				if (!used[u] &&
				    (v == -1 || w[u] > w[v]))
					v = u;
			if (i + 1 == (int)active.size()) {
				if (w[v] < ans.first)
					ans = { w[v], co[v] };
				for (int u : active)
					if (u != pre && u != v)
						a[pre][u] = a[u][pre] +=
						    a[u][v];
				co[pre].insert(co[pre].end(),
				               co[v].begin(),
				               co[v].end());
				active.erase(find(active.begin(),
				                  active.end(), v));
				break;
			}
			used[v] = true;
			pre = v;
			for (int u : active)
				if (!used[u]) w[u] += a[v][u];
		}
	}
	return ans;
}
