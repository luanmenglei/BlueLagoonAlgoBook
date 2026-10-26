#include "Test.h"
#include "../content/data-structures/HLD.h"
#include "../content/supplement/Fenwick2D.h"
#include "../content/supplement/Mo.h"
#include "../content/supplement/BinaryLifting.h"
#include "../content/supplement/Matrix.h"
#include "../content/supplement/GaussBinary.h"
#include "../content/supplement/LIS.h"
#include "../content/supplement/CombinatorialTables.h"
#include "../content/supplement/FastIO.h"
#include "../content/supplement/SternBrocot.h"
#include "../content/supplement/PolygonTools.h"
#include "../content/supplement/ManhattanMST.h"
#include "../content/strings/BitsetLCS.h"

i64 mst(int n, vector<tuple<i64, int, int>> e) {
	sort(e.begin(), e.end());
	vector<int> p(n);
	iota(p.begin(), p.end(), 0);
	function<int(int)> find = [&](int x) {
		return p[x] == x ? x : p[x] = find(p[x]);
	};
	i64 ans = 0;
	for (auto [w, u, v] : e)
		if (find(u) != find(v))
			ans += w, p[find(u)] = find(v);
	return ans;
}
int main() {
	for (int it = 0; it < 200; it ++) {
		Fenwick2D f;
		vector<pair<i64, i64>> p;
		vector<i64> a(30);
		for (int i = 0; i < 30; i ++)
			p.emplace_back(ri(-9, 9), ri(-9, 9)),
			    f.reserve(p.back().first,
			              p.back().second);
		f.build();
		for (int j = 0; j < 100; j ++) {
			int i = ri(0, 29), v = ri(-20, 20);
			a[i] += v;
			f.add(p[i].first, p[i].second, v);
			int x = ri(-10, 10), y = ri(-10, 10);
			i64 sum = 0;
			for (int k = 0; k < 30; k ++)
				if (p[k].first < x && p[k].second < y)
					sum += a[k];
			CHECK(f.sum(x, y) == sum);
		}
	}
	for (int it = 0; it < 200; it ++) {
		int n = ri(0, 100), q = ri(1, 100);
		vector<int> a(n), ans(q), freq(20);
		for (int &x : a)
			x = ri(0, 19);
		vector<MoQuery> qs;
		int distinct = 0;
		for (int i = 0; i < q; i ++) {
			int l = ri(0, n), r = ri(l, n);
			qs.push_back({ l, r, i });
			set<int> s(a.begin() + l, a.begin() + r);
			ans[i] = s.size();
		}
		mo(
		    n, qs,
		    [&](int i) { distinct += !freq[a[i]] ++; },
		    [&](int i) { distinct -= !--freq[a[i]]; },
		    [&](int id) {
			    CHECK(distinct == ans[id]);
		    });
	}
	for (int it = 0; it < 120; it ++) {
		int n = ri(1, 50);
		HLD h(n);
		for (int v = 1; v < n; v ++) {
			int u = ri(0, v - 1);
			h.g[u].push_back(v);
			h.g[v].push_back(u);
		}
		h.init(ri(0, n - 1));
		BinaryLifting b(h);
		for (int t = 0; t < 100; t ++) {
			int u = ri(0, n - 1), v = ri(0, n - 1),
			    w = h.lca(u, v);
			vector<int> path, right;
			for (int x = u; x != w; x = h.p[x])
				path.push_back(x);
			path.push_back(w);
			for (int x = v; x != w; x = h.p[x])
				right.push_back(x);
			reverse(right.begin(), right.end());
			path.insert(path.end(), right.begin(),
			            right.end());
			CHECK(b.lca(u, v) == w);
			for (int k = 0; k < (int)path.size(); k ++)
				CHECK(b.kth_path(u, v, k) == path[k]);
			CHECK(b.kth_path(u, v, path.size()) == -1);
			for (bool edge : { false, true }) {
				vector<int> got;
				for (auto s :
				     h.path_ordered(u, v, edge)) {
					vector<int> a;
					for (int k = s.l; k < s.r; k ++)
						a.push_back(h.seq[k]);
					if (s.rev)
						reverse(a.begin(), a.end());
					got.insert(got.end(), a.begin(),
					           a.end());
				}
				vector<int> expected = path;
				if (edge)
					expected.erase(
					    find(expected.begin(),
					         expected.end(), w));
				CHECK(got == expected);
			}
		}
	}
	const int mod = 101;
	for (int it = 0; it < 400; it ++) {
		int n = ri(0, 6);
		matrix::Mat a(n, vector<int>(n));
		for (auto &r : a)
			for (int &x : r)
				x = ri(0, 100);
		auto inv = matrix::inverse_mod(a, mod);
		if (inv)
			CHECK(matrix::multiply(a, *inv, mod) ==
			      matrix::identity(n, mod));
		int k = ri(0, 15);
		auto naive = matrix::identity(n, mod);
		for (int j = 0; j < k; j ++)
			naive = matrix::multiply(naive, a, mod);
		CHECK(matrix::power(a, k, mod) == naive);
		vector<vector<double>> r(n, vector<double>(n));
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < n; j ++)
				r[i][j] =
				    ri(-10, 10) + (i == j ? 100 : 0);
		auto x = matrix::inverse_real(r);
		CHECK(x.has_value());
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < n; j ++) {
				double v = 0;
				for (int t = 0; t < n; t ++)
					v += r[i][t] * (*x)[t][j];
				CHECK(abs(v - (i == j)) < 1e-9);
			}
	}
	CHECK(!matrix::inverse_mod({ { 1, 1 }, { 1, 1 } },
	                           mod));
	CHECK(
	    !matrix::inverse_real({ { 0, 0 }, { 0, 0 } }));
	for (int it = 0; it < 1500; it ++) {
		int m = ri(0, 8), n = ri(0, 12);
		vector<vector<u64>> a(n, vector<u64>(1));
		for (auto &r : a)
			r[0] = ri(0, (1 << (m + 1)) - 1);
		set<int> expected;
		for (int mask = 0; mask < (1 << m); mask ++) {
			bool ok = true;
			for (auto &r : a)
				ok &= ((__builtin_popcount(
				            (unsigned)(r[0] & mask)) &
				        1) == int(r[0] >> m & 1));
			if (ok) expected.insert(mask);
		}
		auto got = gauss_binary(a, m);
		CHECK(got.ok == !expected.empty());
		if (got.ok) {
			set<int> s;
			int start = 0;
			for (int i = 0; i < m; i ++)
				start |= got.value[i] << i;
			for (int mask = 0;
			     mask < (1 << (int)got.basis.size());
			     mask ++) {
				int v = start;
				for (int j = 0;
				     j < (int)got.basis.size(); j ++)
					if (mask >> j & 1)
						for (int k = 0; k < m; k ++)
							v ^= got.basis[j][k] << k;
				s.insert(v);
			}
			CHECK(s == expected);
			CHECK(got.rank + (int)got.basis.size() ==
			      m);
		}
	}
	for (int it = 0; it < 500; it ++) {
		int n = ri(0, 14);
		vector<int> a(n);
		for (int &x : a)
			x = ri(-5, 5);
		for (bool strict : { false, true }) {
			int best = 0;
			for (int mask = 0; mask < (1 << n);
			     mask ++) {
				int old = 0, len = 0;
				bool first = true, ok = true;
				for (int i = 0; i < n; i ++)
					if (mask >> i & 1) {
						if (!first)
							ok &= strict ? old < a[i]
							             : old <= a[i];
						old = a[i];
						first = false;
						len ++;
					}
				if (ok) best = max(best, len);
			}
			auto s = lis(a, strict);
			CHECK((int)s.size() == best);
			for (int i = 1; i < (int)s.size(); i ++)
				CHECK(s[i] > s[i - 1] &&
				      (strict
				           ? a[s[i]] > a[s[i - 1]]
				           : a[s[i]] >= a[s[i - 1]]));
		}
	}
	{
		int m = 130;
		vector<vector<u64>> a(m, vector<u64>(3));
		vector<int> x(m);
		for (int i = 0; i < m; i ++)
			x[i] = i % 3 == 0;
		for (int i = 0; i < m; i ++) {
			int rhs = 0;
			for (int j = i; j < m; j ++)
				if (j == i || (i + j) % 7 == 0)
					a[i][j / 64] |= 1ULL << (j % 64),
					    rhs ^= x[j];
			if (rhs) a[i][m / 64] |= 1ULL << (m % 64);
		}
		auto s = gauss_binary(a, m);
		CHECK(s.ok && s.rank == m && s.value == x &&
		      s.basis.empty());
		a.push_back(a.back());
		a.back()[m / 64] ^= 1ULL << (m % 64);
		CHECK(!gauss_binary(a, m).ok);
	}
	for (int mod : { 1, 7, 1000 }) {
		vector<int> p(101);
		p[0] = 1 % mod;
		for (int coin = 1; coin <= 100; coin ++)
			for (int x = coin; x <= 100; x ++)
				p[x] = (p[x] + p[x - coin]) % mod;
		CHECK(partition_table(100, mod) == p);
	}
	for (int n = 0; n <= 7; n ++) {
		vector<int> a(n), count(n + 1);
		iota(a.begin(), a.end(), 0);
		do {
			int c = 0;
			vector<int> seen(n);
			for (int v = 0; v < n; v ++)
				if (!seen[v]) {
					c ++;
					for (int x = v; !seen[x]; x = a[x])
						seen[x] = 1;
				}
			count[c] ++;
		} while (next_permutation(a.begin(), a.end()));
		auto s = stirling_table(n, 1000000007, true);
		CHECK(s[n] == count);
		vector<int> parts(n + 1), label(n);
		function<void(int, int)> dfs = [&](int i,
		                                   int hi) {
			if (i == n) {
				parts[hi + 1] ++;
				return;
			}
			for (int c = 0; c <= hi + 1; c ++)
				label[i] = c, dfs(i + 1, max(hi, c));
		};
		dfs(0, -1);
		CHECK(stirling_table(n, 1000000007)[n] ==
		      parts);
	}
	FILE *f = tmpfile();
	CHECK(f);
	vector<i128> numbers = { 0,
		                     -1,
		                     LLONG_MIN,
		                     LLONG_MAX,
		                     i128(1) << 100,
		                     -(i128(1) << 100) };
	for (int i = 0; i < 1000; i ++)
		numbers.push_back(i64(gen()) - INT_MAX);
	{
		FastOutput out(f);
		for (auto x : numbers)
			out.write(x);
		out.flush();
	}
	rewind(f);
	{
		FastInput in(f);
		for (auto x : numbers) {
			i128 y;
			CHECK(in.read(y) && x == y);
		}
		i128 y;
		CHECK(!in.read(y));
	}
	fclose(f);
	for (int it = 0; it < 3000; it ++) {
		int a = ri(0, 40), b = ri(1, 15), c = ri(0, 40),
		    d = ri(1, 15);
		if (i64(a) * d >= i64(c) * b) continue;
		auto x = simplest_fraction(a, b, c, d);
		CHECK(x.has_value());
		int bp = -1, bq = -1;
		for (int q = 1; q <= 1000 && bq < 0; q ++) {
			int p = i64(a) * q / b + 1;
			if (i64(p) * d < i64(c) * q) bp = p, bq = q;
		}
		CHECK(x->first == bp && x->second == bq);
	}
	for (int it = 0; it < 400; it ++) {
		int n = ri(1, 30);
		vector<Point<i64>> p;
		for (int i = 0; i < n; i ++)
			p.emplace_back(ri(-20, 20), ri(-20, 20));
		vector<tuple<i64, int, int>> all;
		for (int i = 0; i < n; i ++)
			for (int j = 0; j < i; j ++)
				all.emplace_back(
				    abs(p[i].x - p[j].x) +
				        abs(p[i].y - p[j].y),
				    i, j);
		CHECK(mst(n, manhattanMST(p)) == mst(n, all));
	}
	for (int it = 0; it < 200; it ++) {
		double x = ri(0, 10);
		vector<Point<double>> p = {
			Point<double>(0, 0), Point<double>(10, 0),
			Point<double>(10, 10), Point<double>(0, 10)
		};
		auto cut = polygon_cut(p, Point<double>(x, -1),
		                       Point<double>(x, 11));
		double area = 0;
		for (int i = 0; i < (int)cut.size(); i ++)
			area +=
			    cut[i].cross(cut[(i + 1) % cut.size()]);
		CHECK(abs(area / 2 - 10 * x) < 1e-8);
		auto c = polygon_center(p);
		CHECK(c && abs(c->x - 5) < 1e-9 &&
		      abs(c->y - 5) < 1e-9);
	}
	for (int it = 0; it < 1000; it ++) {
		string a, b;
		for (int n = ri(0, 20); n --;)
			a += char('a' + ri(0, 3));
		for (int n = ri(0, 20); n --;)
			b += char('a' + ri(0, 3));
		auto c = lcs_restore(a, b);
		vector<vector<int>> d(
		    a.size() + 1, vector<int>(b.size() + 1));
		for (int i = 1; i <= (int)a.size(); i ++)
			for (int j = 1; j <= (int)b.size(); j ++)
				d[i][j] =
				    a[i - 1] == b[j - 1]
				        ? d[i - 1][j - 1] + 1
				        : max(d[i - 1][j], d[i][j - 1]);
		CHECK((int)c.size() == d.back().back());
		for (auto s : { a, b }) {
			int j = 0;
			for (char x : s)
				if (j < (int)c.size() && x == c[j]) j ++;
			CHECK(j == (int)c.size());
		}
	}
	done("common supplement");
}
