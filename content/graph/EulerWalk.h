/**
 * @title 欧拉路径
 * @brief 求有向或无向图中恰好经过每条边一次的路径。
 * @note 0-based；邻接项 (终点,边编号)，边编号 0..m-1。无向边两端使用同一编号。有向路径从出入度差为1的点起；无向从奇度点起；无边时返回起点。
 * @time O(n+m)
 * @depends contest/Base.h
 * @author Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/EulerWalk.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int>
eulerWalk(vector<vector<pair<int, int>>> &gr,
          int nedges, int src = 0) {
	int n = (int)(gr).size();
	vector<int> D(n), its(n), eu(nedges), ret,
	    s = { src };
	D[src] ++; // to allow Euler paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), y, e, &it = its[x],
		    end = (int)(gr[x]).size();
		if (it == end) {
			ret.push_back(x);
			s.pop_back();
			continue;
		}
		tie(y, e) = gr[x][it ++];
		if (!eu[e]) {
			D[x] --, D[y] ++;
			eu[e] = 1;
			s.push_back(y);
		}
	}
	for (int x : D)
		if (x < 0 || (int)(ret).size() != nedges + 1)
			return {};
	return { ret.rbegin(), ret.rend() };
}
