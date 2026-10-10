/**
 * @title 无向图两点最小割树（Gomory-Hu）
 * @brief 构造一棵树，用树路径最小边权表示原图任意两点的最小割。
 * @note 无向非负容量图，0-based；任意两点最小割等于树路径最小边权。总容量小于 Dinic 默认流量上限。
 * @time n-1 次最大流
 * @depends contest/Base.h, flow/Dinic.h
 * @author chilli, Takanori MAEHARA; BlueLagoon
 * @source KACTL GomoryHu.h (Gusfield); BlueLagoon Dinic adaptation
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../flow/Dinic.h"

vector<tuple<int, int, i64>>
gomory_hu(int n,
          const vector<tuple<int, int, i64>> &ed) {
	vector<int> p(n);
	vector<tuple<int, int, i64>> ans;
	for (int i = 1; i < n; i ++) {
		Dinic d(n);
		for (auto [u, v, w] : ed)
			if (u != v) d.add(u, v, w, w);
		i64 f = d.flow(i, p[i]);
		d.bfs(i, p[i]);
		ans.push_back({ i, p[i], f });
		for (int j = i + 1; j < n; j ++)
			if (p[j] == p[i] && d.dep[j] != -1)
				p[j] = i;
	}
	return ans;
}
