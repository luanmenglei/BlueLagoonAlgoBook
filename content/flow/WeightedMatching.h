/**
 * @title 二分图最小权匹配（KM／Hungarian）
 * @brief 求覆盖左侧所有点的最小权匹配；两侧等大时为最小权完美匹配。
 * @note 输入 `cost` 为 n×m 费用矩阵，要求 n<=m、左侧每点都匹配到不同右点；返回最小总费用与左侧对应的右侧下标（0-based）。n=m
 * 为完美匹配；支持负权，最大权可把费用取负。不负责先最大化匹配数量；稀疏图或不存在边的情形优先使用最小费用流。费用差与总费用须可表示；空矩阵合法。
 * @usage `auto [cost,match]=weightedMatching(a);`，例如 a={{3,1},{2,4}} 得费用3，match={1,0}。
 * @time O(n^2 m)
 * @depends contest/Base.h
 * @author Jeremy Lim, Joshua Andersson; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/WeightedMatching.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class T>
pair<T, vector<int>>
weightedMatching(vector<vector<T>> &C) {
	int i = (int)(C).size(),
	    m = i ? (int)(C[0]).size() : 0, c, s, r;
	vector<T> dist(m), pot(m);
	vector<int> match(i), rev(m, -1), cols(m), prev(m);
	T d = 0, nd, cost = 0;
	while (i --) {
		for (int c = 0; c < m; c ++)
			dist[c] = C[i][c], cols[c] = c, prev[c] = i;
		for (s = 0;;) {
			for (int j = s; j < m; j ++) {
				c = cols[j], nd = dist[c] - pot[c];
				if (j == s || d > nd)
					d = nd, swap(cols[s], cols[j]);
			}
			if ((r = rev[c = cols[s ++]]) == -1) break;
			for (int j = 0; j < m; j ++)
				if (dist[j] >
				    (nd = C[r][j] - C[r][c] + dist[c]))
					dist[j] = nd, prev[j] = r;
		}
		cost += dist[c];
		while (s --)
			pot[cols[s]] = dist[cols[s]] - d;
		for (; r != i; swap(c, match[r]))
			r = rev[c] = prev[c];
	}
	return { cost, match };
}
