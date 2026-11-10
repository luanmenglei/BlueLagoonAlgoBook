/**
 * @title 有序多重集／排名／前驱后继（Treap）
 * @brief 按值维护可重复元素，支持插入、删除一个、查找、排名、第 k 小及严格前驱后继。
 * @note 0 为空节点；每个副本独占节点。search／kth／pre／nxt 返回节点编号，失败返回 0，用 tr[p].val 取值。
 *   rk(p,v) 为小于 v 的元素数加一，v 可以不存在；kth 从 1 开始。pre／nxt 严格小于／大于 v。
 *   split(p,v) 分为 <=v 和 >v；merge(a,b) 要求 a 中所有值 <= b 中所有值。删除不存在的值无效果。
 *   节点不回收，空间按累计插入次数计；修改根后须接收返回值。
 * @usage OrderedTreap t; int root=0; root=t.insert(root,5); int p=t.search(root,5); int r=t.rk(root,5); root=t.erase(root,5);
 * @time 期望 O(log n) 每次
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct OrderedTreap {
	struct Node {
		int l = 0, r = 0, sz = 1;
		u64 pri;
		i64 val;
	};
	vector<Node> tr = { { 0, 0, 0, 0, 0 } };
	mt19937_64 rng{ 712367 };
	int node(i64 v) {
		tr.push_back({ 0, 0, 1, rng(), v });
		return tr.size() - 1;
	}
	void pull(int p) {
		tr[p].sz = tr[tr[p].l].sz + tr[tr[p].r].sz + 1;
	}
	pair<int, int> split(int p, i64 v) {
		if (!p) return { 0, 0 };
		if (tr[p].val <= v) {
			auto [a, b] = split(tr[p].r, v);
			tr[p].r = a;
			pull(p);
			return { p, b };
		}
		auto [a, b] = split(tr[p].l, v);
		tr[p].l = b;
		pull(p);
		return { a, p };
	}
	int merge(int a, int b) {
		if (!a || !b) return a ? a : b;
		if (tr[a].pri < tr[b].pri) {
			tr[a].r = merge(tr[a].r, b);
			pull(a);
			return a;
		}
		tr[b].l = merge(a, tr[b].l);
		pull(b);
		return b;
	}
	int insert(int p, i64 v) {
		auto [a, b] = split(p, v);
		int q = node(v);
		return merge(merge(a, q), b);
	}
	int erase(int p, i64 v) {
		if (!p) return 0;
		if (tr[p].val == v)
			return merge(tr[p].l, tr[p].r);
		if (v < tr[p].val)
			tr[p].l = erase(tr[p].l, v);
		else
			tr[p].r = erase(tr[p].r, v);
		pull(p);
		return p;
	}
	int search(int p, i64 v) const {
		while (p && tr[p].val != v)
			p = v < tr[p].val ? tr[p].l : tr[p].r;
		return p;
	}
	int rk(int p, i64 v) const {
		int ans = 1;
		while (p) {
			if (tr[p].val < v) {
				ans += tr[tr[p].l].sz + 1;
				p = tr[p].r;
			} else p = tr[p].l;
		}
		return ans;
	}
	int kth(int p, int k) const {
		if (k < 1 || k > tr[p].sz) return 0;
		while (p) {
			int s = tr[tr[p].l].sz;
			if (k == s + 1) return p;
			if (k <= s) p = tr[p].l;
			else {
				k -= s + 1;
				p = tr[p].r;
			}
		}
		return 0;
	}
	int pre(int p, i64 v) const {
		int ans = 0;
		while (p) {
			if (tr[p].val < v) {
				ans = p;
				p = tr[p].r;
			} else p = tr[p].l;
		}
		return ans;
	}
	int nxt(int p, i64 v) const {
		int ans = 0;
		while (p) {
			if (tr[p].val > v) {
				ans = p;
				p = tr[p].l;
			} else p = tr[p].r;
		}
		return ans;
	}
};
