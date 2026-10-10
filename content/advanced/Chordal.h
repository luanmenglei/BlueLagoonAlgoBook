/**
 * @title 弦图识别与完美消除序
 * @brief 判断简单图是否为弦图，是则返回完美消除序。
 * @note 非空简单无向图；两端邻接均存在。getPEO 返回消除序，无弦图返回空；后继邻点必须成团。
 * @usage `getPEO({{1,2},{0,2},{0,1}})` 返回三角形的某个消除序；无弦四环返回空。
 * @time O(m log n + m log n)（邻接排序／检查）
 * @depends contest/Base.h
 * @author koosaga; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/chordal_graph.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

// given simple, nonempty graph
// in O(m log n), returns permutation of vertices
// (positive) or empty vector (negative) such that for
// p[i], all j > i such that p[j] is adjacent to p[i],
// constitutes cliques. for each undir edge (s, e), both
// s \in gph[e], e \in gph[s] should hold
vector<int> getPEO(vector<vector<int>> gph) {
	int n = int(gph.size());
	vector<int> cnt(n), idx(n);
	for (int i = 0; i < n; i ++)
		sort(gph[i].begin(), gph[i].end());
	priority_queue<pair<int, int>> pq;
	for (int i = 0; i < n; i ++)
		pq.emplace(cnt[i], i);
	vector<int> ord;
	while (!pq.empty()) {
		int x = pq.top().second, y = pq.top().first;
		pq.pop();
		if (cnt[x] != y || idx[x]) continue;
		ord.push_back(x);
		idx[x] = n + 1 - ord.size();
		for (auto &i : gph[x]) {
			if (!idx[i]) {
				cnt[i] ++;
				pq.emplace(cnt[i], i);
			}
		}
	}
	reverse(ord.begin(), ord.end());
	for (auto &i : ord) {
		int minBef = 1e9;
		for (auto &j : gph[i]) {
			if (idx[j] > idx[i])
				minBef = min(minBef, idx[j]);
		}
		minBef --;
		if (minBef < n) {
			minBef = ord[minBef];
			for (auto &j : gph[i]) {
				if (idx[j] > idx[minBef] &&
				    !binary_search(gph[minBef].begin(),
				                   gph[minBef].end(),
				                   j)) {
					return vector<int>();
				}
			}
		}
	}
	return ord;
}
