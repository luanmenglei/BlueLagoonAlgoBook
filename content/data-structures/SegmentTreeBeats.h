/**
 * @title 区间最值修改／加／求和（Segment Tree Beats）
 * @brief 维护区间
 * chmin、chmax、加，以及区间和、最小值、最大值。
 * @note 从 vector<i64> 建树；0-based 半开区间
 * [l,r)。chmin 令 a[i]=min(a[i],v)，chmax 同理。
 * 实际及内部临时极值、修改参数严格在 (-INF,INF)
 * 内；中间区间和、累计加法标记须能装入 i64。
 * 空区间修改无效，查询返回
 * {0,INF,-INF}。完整版本为摊还界，不保证单次最坏 O(log
 * n)。
 * @usage SegBeats t(vector<i64>{3,1,4});
 * t.chmin(0,3,2); t.add(1,3,5); auto q=t.query(0,3);
 * @time 建树 O(n)，q 次操作总计 O((n+q) log^2 n)
 * 摊还，空间 O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard Segment
 * Tree Beats
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

struct SegBeats {
	static constexpr i64 INF = LLONG_MAX / 4;
	struct Node {
		i64 sum = 0, add = 0;
		// 最大、严格次大；最小、严格次小。无次极值用哨兵。
		i64 mx = -INF, sx = -INF, mn = INF, sn = INF;
		int cx = 0, cn = 0,
		    len = 0; // 两种极值个数、长度。
	};
	struct Result {
		i64 sum = 0, mn = INF, mx = -INF;
	};
	int n;
	vector<Node> tr;
	explicit SegBeats(const vector<i64> &a)
	    : n(a.size()), tr(4 * n + 4) {
		if (n) build(1, 0, n, a);
	}
	void pull(int p) {
		auto &a = tr[p * 2], &b = tr[p * 2 + 1];
		Node c;
		c.len = a.len + b.len;
		c.sum = i128(a.sum) + b.sum;
		c.mx = max(a.mx, b.mx), c.mn = min(a.mn, b.mn);
		c.cx = (a.mx == c.mx ? a.cx : 0) +
		       (b.mx == c.mx ? b.cx : 0);
		c.cn = (a.mn == c.mn ? a.cn : 0) +
		       (b.mn == c.mn ? b.cn : 0);
		c.sx = max(a.mx == c.mx ? a.sx : a.mx,
		           b.mx == c.mx ? b.sx : b.mx);
		c.sn = min(a.mn == c.mn ? a.sn : a.mn,
		           b.mn == c.mn ? b.sn : b.mn);
		tr[p] = c;
	}
	void build(int p, int l, int r,
	           const vector<i64> &a) {
		if (r - l == 1) {
			assert(-INF < a[l] && a[l] < INF);
			auto &x = tr[p];
			x.sum = x.mx = x.mn = a[l];
			x.cx = x.cn = x.len = 1;
			return;
		}
		int m = (l + r) / 2;
		build(p * 2, l, m, a),
		    build(p * 2 + 1, m, r, a);
		pull(p);
	}
	void apply_add(int p, i64 v) {
		auto &x = tr[p];
		x.sum = i128(x.sum) + i128(v) * x.len;
		x.mx += v, x.mn += v, x.add += v;
		if (x.sx != -INF) x.sx += v;
		if (x.sn != INF) x.sn += v;
	}
	// 内部捷径：仅最大值受影响，必须 sx<v<mx。
	void apply_min(int p, i64 v) {
		auto &x = tr[p];
		if (x.mx <= v) return;
		assert(x.sx < v);
		x.sum = i128(x.sum) + i128(v - x.mx) * x.cx;
		if (x.mn == x.mx)
			x.mn = v;
		else if (x.sn == x.mx)
			x.sn = v;
		x.mx = v;
	}
	// 内部捷径：仅最小值受影响，必须 mn<v<sn。
	void apply_max(int p, i64 v) {
		auto &x = tr[p];
		if (x.mn >= v) return;
		assert(v < x.sn);
		x.sum = i128(x.sum) + i128(v - x.mn) * x.cn;
		if (x.mx == x.mn)
			x.mx = v;
		else if (x.sx == x.mn)
			x.sx = v;
		x.mn = v;
	}
	void push(int p) {
		// 先下传加法，再按父结点现有极值限制孩子。
		for (int c : { p * 2, p * 2 + 1 }) {
			apply_add(c, tr[p].add);
			apply_min(c, tr[p].mx);
			apply_max(c, tr[p].mn);
		}
		tr[p].add = 0;
	}
	// op=0 加，op=1 chmin，op=2
	// chmax；其余函数为公开入口。
	void update(int p, int l, int r, int ql, int qr,
	            i64 v, int op) {
		if (qr <= l || r <= ql) return;
		auto &x = tr[p];
		if ((op == 1 && x.mx <= v) ||
		    (op == 2 && x.mn >= v))
			return;
		if (ql <= l && r <= qr) {
			if (op == 0) return apply_add(p, v);
			if (op == 1 && x.sx < v)
				return apply_min(p, v);
			if (op == 2 && v < x.sn)
				return apply_max(p, v);
		}
		push(p);
		int m = (l + r) / 2;
		update(p * 2, l, m, ql, qr, v, op);
		update(p * 2 + 1, m, r, ql, qr, v, op);
		pull(p);
	}
	void change(int l, int r, i64 v, int op) {
		assert(0 <= l && l <= r && r <= n);
		assert(-INF < v && v < INF);
		if (l < r) update(1, 0, n, l, r, v, op);
	}
	void add(int l, int r, i64 v) {
		change(l, r, v, 0);
	}
	void chmin(int l, int r, i64 v) {
		change(l, r, v, 1);
	}
	void chmax(int l, int r, i64 v) {
		change(l, r, v, 2);
	}
	Result get(int p, int l, int r, int ql, int qr) {
		if (qr <= l || r <= ql) return {};
		if (ql <= l && r <= qr)
			return { tr[p].sum, tr[p].mn, tr[p].mx };
		push(p);
		int m = (l + r) / 2;
		auto a = get(p * 2, l, m, ql, qr);
		auto b = get(p * 2 + 1, m, r, ql, qr);
		return { i64(i128(a.sum) + b.sum),
			     min(a.mn, b.mn), max(a.mx, b.mx) };
	}
	Result query(int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		return l == r ? Result{} : get(1, 0, n, l, r);
	}
};
