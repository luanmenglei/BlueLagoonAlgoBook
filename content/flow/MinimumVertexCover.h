/**
 * @title 二分图最小点覆盖／最大独立集
 * @brief 求二分图最小点覆盖；其顶点补集是最大独立集。
 * @note 输入左侧邻接、左右点数 n,m；结果编码左侧 0..n-1，右侧 n..n+m-1。补集为最大独立集。
 * @time O(m sqrt(n))
 * @depends contest/Base.h, flow/HopcroftKarp.h
 * @author Johan Sannemo, Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/MinimumVertexCover.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../flow/HopcroftKarp.h"

vector<int> cover(vector<vector<int>> &g, int n,
                  int m) {
	vector<int> match(m, -1);
	int res = hopcroftKarp(g, match);
	vector<bool> lfound(n, true), seen(m);
	for (int it : match)
		if (it != -1) lfound[it] = false;
	vector<int> q, cover;
	for (int i = 0; i < n; i ++)
		if (lfound[i]) q.push_back(i);
	while (!q.empty()) {
		int i = q.back();
		q.pop_back();
		lfound[i] = 1;
		for (int e : g[i])
			if (!seen[e] && match[e] != -1) {
				seen[e] = true;
				q.push_back(match[e]);
			}
	}
	for (int i = 0; i < n; i ++)
		if (!lfound[i]) cover.push_back(i);
	for (int i = 0; i < m; i ++)
		if (seen[i]) cover.push_back(n + i);
	assert((int)(cover).size() == res);
	return cover;
}
