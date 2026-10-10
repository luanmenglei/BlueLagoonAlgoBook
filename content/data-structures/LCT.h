/**
 * @title 动态树路径异或（LCT）
 * @brief 动态连边、断边、修改点权，并查询森林中两点路径异或。
 * @note 1-based，0 是空节点。query 要求两点连通；初始点权用 set 设置。支持森林，link 防止成环，cut 仅删除已有直接边。
 * @time 均摊 O(log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct LCT {
	struct Node {
		int ch[2] = {}, p = 0;
		bool rev = false;
		i64 val = 0, sum = 0;
	};
	vector<Node> tr;
	LCT(int n) : tr(n + 1) {}
	bool root(int x) {
		int p = tr[x].p;
		return !p ||
		       (tr[p].ch[0] != x && tr[p].ch[1] != x);
	}
	void pull(int x) {
		tr[x].sum = tr[tr[x].ch[0]].sum ^ tr[x].val ^
		            tr[tr[x].ch[1]].sum;
	}
	void flip(int x) {
		if (x)
			swap(tr[x].ch[0], tr[x].ch[1]),
			    tr[x].rev ^= 1;
	}
	void push(int x) {
		if (tr[x].rev) {
			flip(tr[x].ch[0]);
			flip(tr[x].ch[1]);
			tr[x].rev = false;
		}
	}
	void rotate(int x) {
		int y = tr[x].p, z = tr[y].p;
		int k = tr[y].ch[1] == x, w = tr[x].ch[k ^ 1];
		if (!root(y)) tr[z].ch[tr[z].ch[1] == y] = x;
		tr[x].p = z;
		tr[x].ch[k ^ 1] = y;
		tr[y].p = x;
		tr[y].ch[k] = w;
		if (w) tr[w].p = y;
		pull(y);
		pull(x);
	}
	void splay(int x) {
		vector<int> st = { x };
		for (int y = x; !root(y);)
			st.push_back(y = tr[y].p);
		while (!st.empty())
			push(st.back()), st.pop_back();
		while (!root(x)) {
			int y = tr[x].p, z = tr[y].p;
			if (!root(y))
				rotate((tr[y].ch[1] == x) ==
				               (tr[z].ch[1] == y)
				           ? y
				           : x);
			rotate(x);
		}
	}
	void access(int x) {
		for (int y = 0; x; y = x, x = tr[x].p) {
			splay(x);
			tr[x].ch[1] = y;
			pull(x);
		}
	}
	void makeroot(int x) {
		access(x);
		splay(x);
		flip(x);
	}
	int findroot(int x) {
		access(x);
		splay(x);
		push(x);
		while (tr[x].ch[0])
			x = tr[x].ch[0], push(x);
		splay(x);
		return x;
	}
	bool link(int x, int y) {
		makeroot(x);
		if (findroot(y) == x) return false;
		tr[x].p = y;
		return true;
	}
	bool cut(int x, int y) {
		makeroot(x);
		access(y);
		splay(y);
		if (tr[y].ch[0] != x || tr[x].ch[1])
			return false;
		tr[y].ch[0] = tr[x].p = 0;
		pull(y);
		return true;
	}
	void set(int x, i64 v) {
		access(x);
		splay(x);
		tr[x].val = v;
		pull(x);
	}
	i64 query(int x, int y) {
		makeroot(x);
		access(y);
		splay(y);
		return tr[y].sum;
	}
};
