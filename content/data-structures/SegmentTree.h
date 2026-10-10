/**
 * @title 区间加／赋值／求和线段树
 * @brief 维护区间加、区间赋值与区间和。
 * @note 初始全零，闭区间 1..n；从根参数 (1,1,n) 调用，要求 1 <= x <= y <= n。普通线段树可删懒标记。
 * @time O(log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct SegTree {
	struct Node {
		i64 sum = 0, add = 0, set = 0;
		bool has = false;
	};
	int n;
	vector<Node> tr;
	SegTree(int n) : n(n), tr(4 * n + 4) {}
	void apply(int p, int l, int r, i64 v, bool set) {
		if (set) {
			tr[p].sum = (r - l + 1) * v;
			tr[p].set = v;
			tr[p].has = true;
			tr[p].add = 0;
		} else {
			tr[p].sum += (r - l + 1) * v;
			tr[p].add += v;
		}
	}
	void push(int p, int l, int r) {
		int m = (l + r) / 2;
		if (tr[p].has) {
			apply(p * 2, l, m, tr[p].set, true);
			apply(p * 2 + 1, m + 1, r, tr[p].set, true);
			tr[p].has = false;
		}
		if (tr[p].add) {
			apply(p * 2, l, m, tr[p].add, false);
			apply(p * 2 + 1, m + 1, r, tr[p].add,
			      false);
			tr[p].add = 0;
		}
	}
	void update(int p, int l, int r, int x, int y,
	            i64 v, bool set = false) {
		if (x <= l && r <= y)
			return apply(p, l, r, v, set);
		push(p, l, r);
		int m = (l + r) / 2;
		if (x <= m) update(p * 2, l, m, x, y, v, set);
		if (y > m)
			update(p * 2 + 1, m + 1, r, x, y, v, set);
		tr[p].sum = tr[p * 2].sum + tr[p * 2 + 1].sum;
	}
	i64 query(int p, int l, int r, int x, int y) {
		if (x <= l && r <= y) return tr[p].sum;
		push(p, l, r);
		int m = (l + r) / 2;
		i64 ans = 0;
		if (x <= m) ans += query(p * 2, l, m, x, y);
		if (y > m)
			ans += query(p * 2 + 1, m + 1, r, x, y);
		return ans;
	}
};
