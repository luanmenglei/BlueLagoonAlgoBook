/**
 * @title 可持久化权值线段树／区间第 k 小
 * @brief 维护各版本权值频次，通过前缀版本之差查询区间第 k 小。
 * @note 权值离散化到闭区间 1..M；0 为全零根。前缀根差 kth(a,b) 要求差值计数非负且 k 有效。
 * @time 每次 O(log M)，空间 O(更新数 log M)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Persistent {
	struct Node {
		int l = 0, r = 0, cnt = 0;
	};
	vector<Node> tr = { Node() };
	int add(int p, int l, int r, int x, int v = 1) {
		int q = tr.size();
		tr.push_back(tr[p]);
		tr[q].cnt += v;
		if (l != r) {
			int m = (l + r) / 2;
			if (x <= m)
				tr[q].l = add(tr[p].l, l, m, x, v);
			else
				tr[q].r = add(tr[p].r, m + 1, r, x, v);
		}
		return q;
	}
	int kth(int a, int b, int l, int r, int k) {
		if (l == r) return l;
		int cnt = tr[tr[b].l].cnt - tr[tr[a].l].cnt;
		int m = (l + r) / 2;
		if (k <= cnt)
			return kth(tr[a].l, tr[b].l, l, m, k);
		return kth(tr[a].r, tr[b].r, m + 1, r, k - cnt);
	}
};
