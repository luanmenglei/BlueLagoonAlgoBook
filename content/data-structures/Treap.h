/**
 * @title 序列分裂／合并／翻转（隐式 Treap）
 * @brief 按位置分裂和合并序列，维护区间翻转与区间和。
 * @note 0 为空节点；split(root,k) 分出前 k 个元素。区间 [l,r) 用两次 split，再 flip／读 sum，最后 merge。
 * @time 期望 O(log n) 每次
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Treap {
	struct Node {
		int l = 0, r = 0, sz = 1;
		u64 pri;
		i64 val, sum;
		bool rev = false;
	};
	vector<Node> tr = { { 0, 0, 0, 0, 0, 0, false } };
	mt19937_64 rng{ 712367 };
	int node(i64 v) {
		tr.push_back({ 0, 0, 1, rng(), v, v, false });
		return tr.size() - 1;
	}
	void flip(int p) {
		if (p) swap(tr[p].l, tr[p].r), tr[p].rev ^= 1;
	}
	void push(int p) {
		if (tr[p].rev) {
			flip(tr[p].l);
			flip(tr[p].r);
			tr[p].rev = false;
		}
	}
	void pull(int p) {
		auto &a = tr[p];
		a.sz = tr[a.l].sz + tr[a.r].sz + 1;
		a.sum = tr[a.l].sum + tr[a.r].sum + a.val;
	}
	pair<int, int> split(int p, int k) {
		if (!p) return { 0, 0 };
		push(p);
		if (tr[tr[p].l].sz >= k) {
			auto [a, b] = split(tr[p].l, k);
			tr[p].l = b;
			pull(p);
			return { a, p };
		}
		auto [a, b] =
		    split(tr[p].r, k - tr[tr[p].l].sz - 1);
		tr[p].r = a;
		pull(p);
		return { p, b };
	}
	int merge(int a, int b) {
		if (!a || !b) return a ? a : b;
		if (tr[a].pri < tr[b].pri) {
			push(a);
			tr[a].r = merge(tr[a].r, b);
			pull(a);
			return a;
		}
		push(b);
		tr[b].l = merge(a, tr[b].l);
		pull(b);
		return b;
	}
};
