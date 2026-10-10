/**
 * @title 树同构
 * @brief 确定性判断两棵无向树是否同构。
 * @note 输入无向简单树；使用同一个字典分配子树类型，确定性判同构，无哈希碰撞。深链注意栈。
 * @time 保守 O(n log^2 n)，空间 O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int> tree_centers(const vector<vector<int>> &g) {
	int n = g.size();
	if (n <= 2) {
		vector<int> a(n);
		iota(a.begin(), a.end(), 0);
		return a;
	}
	vector<int> d(n), q;
	for (int i = 0; i < n; i ++)
		if ((d[i] = g[i].size()) == 1) q.push_back(i);
	int head = 0, left = n;
	while (left > 2) {
		int tail = q.size();
		left -= tail - head;
		while (head < tail)
			for (int v : g[q[head ++]])
				if (--d[v] == 1) q.push_back(v);
	}
	return vector<int>(q.begin() + head, q.end());
}
int rooted_id(int u, int p,
              const vector<vector<int>> &g,
              map<vector<int>, int> &mp) {
	vector<int> a;
	for (int v : g[u])
		if (v != p) a.push_back(rooted_id(v, u, g, mp));
	sort(a.begin(), a.end());
	if (!mp.count(a)) mp[a] = mp.size();
	return mp[a];
}
bool tree_iso(const vector<vector<int>> &a,
              const vector<vector<int>> &b) {
	if (a.size() != b.size()) return false;
	if (a.empty()) return true;
	map<vector<int>, int> mp;
	int x = rooted_id(tree_centers(a)[0], -1, a, mp);
	for (int r : tree_centers(b))
		if (rooted_id(r, -1, b, mp) == x) return true;
	return false;
}
