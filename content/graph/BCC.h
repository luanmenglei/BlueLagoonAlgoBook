/**
 * @title 割点、桥、点双与边双
 * @brief 枚举割点、桥、点双与边双连通分量，并构建圆方森林。
 * @note 顶点编号 0..n-1，无向图可不连通、可有平行边；自环需在 add 前过滤。
 * 先 BCC b(n)，逐条 b.add(u,v)，再 b.solve()，最后读取结果或调用 b.block_tree()。
 * 每个对象只 solve 一次，修改图后需重建对象。dfs、color 为内部过程；深链注意递归栈。
 * cut[u] 判割点，bridge[id] 判桥，ec[u] 为边双编号，blocks[i] 为第 i 个点双的顶点集。
 * 点双可共享割点；每个顶点只属于一个边双。block_tree() 返回点双的圆方森林。
 * @time DFS O(n+m)，点双去重额外 O(m log m)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct BCC {
	// n：原图顶点数；timer：DFS 时间戳，从 1 开始分配。
	int n, timer = 0;
	// g[u] 存 { 邻点, 无向边编号 }；一条边的两个方向共用同一编号。
	vector<vector<pair<int, int>>> g;
	// ed[id] = { u, v }：编号 id 的边的两个端点，编号按 add 顺序从 0 开始。
	vector<pair<int, int>> ed;
	// dfn[u]：首次访问时间，0 表示未访问。
	// low[u]：从 u 的 DFS 子树经至多一条返祖边可达的最小 dfn。
	// st：尚未归入点双的边编号栈，不是顶点栈。
	// ec[u]：u 所属边双的编号，求解前为 -1，求解后从 0 连续编号。
	// 同一边双内任意两点删除任意一条边后仍连通；孤立点也单独成分量。
	vector<int> dfn, low, st, ec;
	// cut[u]：删除顶点 u 后，图的连通分量数是否增加（割点）。
	// bridge[id]：删除编号 id 的边后，图的连通分量数是否增加（桥）。
	vector<bool> cut, bridge;
	// blocks[i]：第 i 个点双的顶点列表，已排序去重，编号从 0 开始。
	// 割点可出现在多个列表中；桥的两端构成一个二点块，孤立点构成单点块。
	// blocks 的编号与 ec 的编号无关，blocks.size() 是点双数，不是边双数。
	vector<vector<int>> blocks;
	// 构造 n 个顶点的空图。
	BCC(int n)
	    : n(n), g(n), dfn(n), low(n), ec(n, -1),
	      cut(n) {}
	// 加一条无向边，只调用一次，不必再 add(v,u)；要求 u != v。
	void add(int u, int v) {
		assert(u != v);
		int id = ed.size();
		ed.push_back({ u, v });
		g[u].push_back({ v, id });
		g[v].push_back({ u, id });
	}
	// 内部 Tarjan DFS；pe 为父边编号，DFS 根传 -1。
	// 按父边编号跳过反向边，保证平行边仍能作为返祖边更新 low。
	void dfs(int u, int pe) {
		dfn[u] = low[u] = ++timer;
		// child：u 在 DFS 树中的直接孩子数，用于判断根是否为割点。
		int child = 0;
		for (auto [v, id] : g[u])
			if (id != pe) {
				if (!dfn[v]) {
					child ++;
					st.push_back(id);
					dfs(v, id);
					low[u] = min(low[u], low[v]);
					// v 的子树无法绕回 u 或其祖先，则树边 (u,v) 是桥。
					bridge[id] = low[v] > dfn[u];
					if (low[v] >= dfn[u]) {
						// 子树无法绕过 u 回到其祖先：弹出一个点双。
						// 非根 u 是割点；根仅在有至少两个 DFS 孩子时是割点。
						cut[u] = pe != -1 || child > 1;
						// b 收集该点双的端点，x 是从边栈弹出的边编号。
						vector<int> b;
						int x;
						do {
							x = st.back();
							st.pop_back();
							b.push_back(ed[x].first);
							b.push_back(ed[x].second);
						} while (x != id);
						sort(b.begin(), b.end());
						b.erase(
						    unique(b.begin(), b.end()),
						    b.end());
						blocks.push_back(b);
					}
				} else if (dfn[v] < dfn[u]) {
					st.push_back(id);
					low[u] = min(low[u], dfn[v]);
				}
			}
	}
	// 内部染色：忽略所有桥，把 u 所在的连通块标为边双 c。
	void color(int u, int c) {
		ec[u] = c;
		for (auto [v, id] : g[u])
			if (!bridge[id] && ec[v] == -1) color(v, c);
	}
	// 处理所有连通分量，填好 cut、bridge、blocks、ec。
	void solve() {
		bridge.resize(ed.size());
		for (int u = 0; u < n; u ++)
			if (!dfn[u]) {
				dfs(u, -1);
				if (g[u].empty())
					blocks.push_back({ u });
			}
		// c 是已分配的边双数；结束后未存为成员。
		// 外部可用 n ? *max_element(ec.begin(), ec.end()) + 1 : 0 求边双数。
		int c = 0;
		for (int u = 0; u < n; u ++)
			if (ec[u] == -1) color(u, c ++);
	}
	// solve 后调用，返回圆方森林的邻接表，共 n + blocks.size() 个结点。
	// 圆点 0..n-1 对应原图顶点；方点 n+i 对应 blocks[i]。
	// u 属于 blocks[i] 时连边 u <-> n+i；孤立点也会连向自己的单点块。
	// 这是点双圆方森林；若要边双缩点后的桥森林，另枚举 bridge[id]，
	// 将 ed[id] 的两个端点映射到 ec 后连边即可。
	vector<vector<int>> block_tree() {
		vector<vector<int>> t(n + blocks.size());
		for (int i = 0; i < (int)blocks.size(); i ++)
			for (int u : blocks[i]) {
				t[u].push_back(n + i);
				t[n + i].push_back(u);
			}
		return t;
	}
};
