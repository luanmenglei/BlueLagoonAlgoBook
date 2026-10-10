/**
 * @title 可回滚并查集
 * @brief 维护连通关系，并回退到此前保存的合并状态。
 * @note 0-based；保存 time()，用 rollback(t) 回退。find 不做路径压缩；join 返回是否合并。
 * @time O(log n) 查找，O(回退数) 回退
 * @depends contest/Base.h
 * @author Lukas Polacek, Simon Lindholm; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/data-structures/UnionFindRollback.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct RollbackUF {
	vector<int> e;
	vector<pair<int, int>> st;
	RollbackUF(int n) : e(n, -1) {}
	int size(int x) { return -e[find(x)]; }
	int find(int x) {
		return e[x] < 0 ? x : find(e[x]);
	}
	int time() { return (int)(st).size(); }
	void rollback(int t) {
		for (int i = time(); i -- > t;)
			e[st[i].first] = st[i].second;
		st.resize(t);
	}
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.push_back({ a, e[a] });
		st.push_back({ b, e[b] });
		e[a] += e[b];
		e[b] = a;
		return true;
	}
};
