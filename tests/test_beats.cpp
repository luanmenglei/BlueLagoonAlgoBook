#include "Test.h"
#include "../content/data-structures/SegmentTreeBeats.h"

void check(SegBeats &t, const vector<i64> &a, int l,
           int r) {
	auto q = t.query(l, r);
	i64 sum = 0, mn = SegBeats::INF,
	    mx = -SegBeats::INF;
	for (int i = l; i < r; i ++)
		sum += a[i], mn = min(mn, a[i]),
		             mx = max(mx, a[i]);
	CHECK(q.sum == sum && q.mn == mn && q.mx == mx);
}
void change(SegBeats &t, vector<i64> &a, int l, int r,
            i64 v, int op) {
	if (op == 0) t.add(l, r, v);
	if (op == 1) t.chmin(l, r, v);
	if (op == 2) t.chmax(l, r, v);
	for (int i = l; i < r; i ++) {
		if (op == 0) a[i] += v;
		if (op == 1) a[i] = min(a[i], v);
		if (op == 2) a[i] = max(a[i], v);
	}
}
int main() {
	// Exhaust all small states and one-step operations,
	// including equal-to-second-extreme.
	for (int n = 0; n <= 5; n ++) {
		int states = 1;
		for (int i = 0; i < n; i ++)
			states *= 3;
		for (int mask = 0; mask < states; mask ++) {
			vector<i64> base(n);
			int z = mask;
			for (auto &x : base)
				x = z % 3 - 1, z /= 3;
			for (int l = 0; l <= n; l ++)
				for (int r = l; r <= n; r ++)
					for (int op = 0; op < 3; op ++)
						for (int v = -2; v <= 2; v ++) {
							auto a = base;
							SegBeats t(a);
							change(t, a, l, r, v, op);
							for (int x = 0; x <= n; x ++)
								for (int y = x; y <= n;
								     y ++)
									check(t, a, x, y);
						}
		}
	}
	for (int it = 0; it < 300; it ++) {
		int n = ri(1, 80);
		vector<i64> a(n);
		for (auto &x : a)
			x = ri(-100, 100);
		SegBeats t(a);
		for (int j = 0; j < 1000; j ++) {
			int l = ri(0, n), r = ri(l, n);
			if (j % 3 == 0) l = 0, r = n;
			i64 v = ri(-200, 200);
			int op = ri(0, 2);
			change(t, a, l, r, v, op);
			check(t, a, 0, n);
			l = ri(0, n), r = ri(l, n);
			check(t, a, l, r);
		}
	}
	vector<i64> a{ -100000000000000000LL,
		           100000000000000000LL, 0 };
	SegBeats t(a);
	for (int it = 0; it < 100; it ++) {
		change(t, a, 0, 3, 50000000000000000LL, 1);
		change(t, a, 0, 3, -50000000000000000LL, 2);
		change(t, a, 0, 3,
		       it % 2 ? -10000000000000000LL
		              : 10000000000000000LL,
		       0);
		for (int l = 0; l < 3; l ++)
			for (int r = l + 1; r <= 3; r ++)
				check(t, a, l, r);
	}
	// Larger nested ranges exercise amortized update
	// paths and lazy composition.
	int n = 100000;
	a.resize(n);
	iota(a.begin(), a.end(), -50000LL);
	SegBeats big(a);
	for (int i = 0; i < 400; i ++) {
		int l = i * 17, r = n - i * 13;
		change(big, a, l, r,
		       i % 3 == 0 ? 3 : ri(-10000, 10000),
		       i % 3);
	}
	for (int i = 0; i < 100; i ++) {
		int l = ri(0, n), r = ri(l, n);
		check(big, a, l, r);
	}
	done("Segment Tree Beats");
}
