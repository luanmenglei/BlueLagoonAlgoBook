#include "Test.h"
#include "../content/advanced/BostanMori.h"
#include "../content/advanced/OnlineConvolution.h"
#include "../content/advanced/FPSCompose.h"
#include "../content/advanced/KthRoot.h"
#include "../content/advanced/Tetration.h"
#include "../content/advanced/NimProduct.h"
#include "../content/advanced/Pfaffian.h"
#include "../content/advanced/PRecursive.h"
#include "../content/advanced/ReedsSloane.h"
#include "../content/advanced/RealFFT.h"
#include "../content/algebra/MatrixTree.h"

using poly::Vec;
Vec naive_mul(Vec a, Vec b, int n) {
	Vec c(n);
	for (int i = 0; i < (int)a.size(); i ++)
		for (int j = 0; j < (int)b.size() && i + j < n;
		     j ++)
			c[i + j] = (c[i + j] + i64(a[i]) * b[j]) %
			           poly::mod;
	return c;
}
int main() {
	const int mod = poly::mod;
	for (int it = 0; it < 700; it ++) {
		Vec p(ri(0, 15)), q(ri(1, 10));
		for (int &x : p)
			x = ri(0, mod - 1);
		for (int &x : q)
			x = ri(0, mod - 1);
		q[0] = ri(1, mod - 1);
		Vec terms(100);
		int inv = poly::power(q[0], mod - 2);
		for (int k = 0; k < 100; k ++) {
			i64 v = k < (int)p.size() ? p[k] : 0;
			for (int j = 1; j < (int)q.size() && j <= k;
			     j ++)
				v = (v -
				     i64(q[j]) * terms[k - j] % mod +
				     mod) %
				    mod;
			terms[k] = v * inv % mod;
		}
		for (int k : { 0, 1, 2, 7, 30, 99 })
			CHECK(poly::bostan_mori(p, q, k) ==
			      terms[k]);
		int n = ri(1, 100);
		Vec a(ri(0, 100)), b(n);
		for (int &x : a)
			x = ri(0, mod - 1);
		poly::OnlineConvolution oc(a, n);
		for (int k = 0; k < n; k ++) {
			b[k] = ri(0, mod - 1);
			i64 sum = 0;
			for (int j = 0; j < (int)a.size() && j <= k;
			     j ++)
				sum =
				    (sum + i64(a[j]) * b[k - j]) % mod;
			CHECK(oc.push(b[k]) == sum);
		}
	}
	for (int it = 0; it < 300; it ++) {
		int n = ri(0, 30);
		Vec a(ri(0, 40)), g(n + ri(0, 10));
		for (int &x : a)
			x = ri(0, mod - 1);
		for (int i = 1; i < (int)g.size(); i ++)
			g[i] = ri(0, mod - 1);
		Vec expected(n), pw(n);
		if (n) pw[0] = 1;
		for (int x : a) {
			for (int j = 0; j < n; j ++)
				expected[j] =
				    (expected[j] + i64(x) * pw[j]) %
				    mod;
			pw = naive_mul(pw, g, n);
		}
		CHECK(poly::compose(a, g, n) == expected);
		int shift = ri(0, mod - 1);
		auto shifted = poly::taylor_shift(a, shift);
		Vec check(a.size()), power(a.size());
		if (!a.empty()) power[0] = 1;
		for (int x : a) {
			for (int j = 0; j < (int)a.size(); j ++)
				check[j] =
				    (check[j] + i64(x) * power[j]) %
				    mod;
			power = naive_mul(power, { shift, 1 },
			                  a.size());
		}
		CHECK(shifted == check);
		int d = ri(1, 10);
		Vec c(d), s(250);
		for (int &x : c)
			x = ri(0, mod - 1);
		for (int i = 0; i < d; i ++)
			s[i] = ri(0, mod - 1);
		for (int i = d; i < 250; i ++)
			for (int j = 0; j < d; j ++)
				s[i] =
				    (s[i] + i64(c[j]) * s[i - 1 - j]) %
				    mod;
		for (int k : { 0, 1, 10, 50, 249 })
			CHECK(poly::linear_rec_fast(
			          Vec(s.begin(), s.begin() + d), c,
			          k) == s[k]);
	}
	for (u64 p = 2; p <= 100; p ++)
		if (is_prime(p))
			for (int k = 1; k <= 12; k ++)
				for (u64 a = 0; a < p; a ++) {
					bool exists = false;
					for (u64 x = 0; x < p; x ++)
						exists |= pow_mod(x, k, p) == a;
					auto got = kth_root(a, k, p);
					CHECK(got.has_value() == exists);
					if (got)
						CHECK(pow_mod(*got, k, p) == a);
				}
	for (int len = 1; len <= 3; len ++) {
		vector<u64> a(len, 1);
		function<void(int)> dfs = [&](int i) {
			if (i < len) {
				for (int x = 1; x <= 3; x ++)
					a[i] = x, dfs(i + 1);
				return;
			}
			u128 v = 1;
			for (int j = len - 1; j >= 0; j --) {
				u128 b = a[j], x = 1;
				for (u128 k = 0; k < v; k ++)
					x *= b;
				v = x;
			}
			for (int m = 1; m <= 200; m ++)
				CHECK(tower::tetration(a, m) == v % m);
			for (int cap = 1; cap <= 200; cap ++)
				CHECK(tower::cap(a, 0, cap) ==
				      min<u128>(v, cap));
		};
		dfs(0);
	}
	CHECK(tower::tetration(vector<u64>(1000, 1),
	                       1234567) == 1);
	CHECK(
	    tower::tetration({ 2, 1000000000000000000ULL },
	                     1000000007) ==
	    pow_mod(2, 1000000000000000000ULL, 1000000007));
	nimbers::init();
	int tab[16][16]{};
	for (int x = 0; x < 16; x ++)
		for (int y = 0; y < 16; y ++) {
			set<int> s;
			for (int i = 0; i < x; i ++)
				for (int j = 0; j < y; j ++)
					s.insert(tab[i][y] ^ tab[x][j] ^
					         tab[i][j]);
			int v = 0;
			while (s.count(v))
				v ++;
			tab[x][y] = v;
			CHECK(nimbers::product(u64(x), u64(y)) ==
			      (u64)v);
		}
	for (int it = 0; it < 5000; it ++) {
		u64 a = (u64(gen()) << 32) | gen(),
		    b = (u64(gen()) << 32) | gen(),
		    c = (u64(gen()) << 32) | gen();
		CHECK(nimbers::product(a, b) ==
		      nimbers::product(b, a));
		CHECK(nimbers::product(a, b ^ c) ==
		      (nimbers::product(a, b) ^
		       nimbers::product(a, c)));
		CHECK(nimbers::product(a, 1ULL) == a);
		CHECK(nimbers::product(nimbers::product(a, b),
		                       c) ==
		      nimbers::product(a,
		                       nimbers::product(b, c)));
	}
	for (int it = 0; it < 1000; it ++) {
		int n = 2 * ri(0, 5), p = 101;
		vector<vector<int>> a(n, vector<int>(n));
		for (int i = 0; i < n; i ++)
			for (int j = i + 1; j < n; j ++)
				a[i][j] = ri(0, p - 1),
				a[j][i] = a[i][j] ? p - a[i][j] : 0;
		function<int(vector<int>)> dfs =
		    [&](vector<int> s) {
			    if (s.empty()) return 1;
			    int ans = 0;
			    for (int j = 1; j < (int)s.size();
			         j ++) {
				    vector<int> t;
				    for (int k = 1; k < (int)s.size();
				         k ++)
					    if (k != j) t.push_back(s[k]);
				    int z =
				        i64(a[s[0]][s[j]]) * dfs(t) % p;
				    ans =
				        (ans + (j & 1 ? z : p - z)) % p;
			    }
			    return ans;
		    };
		vector<int> s(n);
		iota(s.begin(), s.end(), 0);
		int got = pfaffian(a, p);
		CHECK(got == dfs(s));
		CHECK(i64(got) * got % p == determinant(a, p));
	}
	for (int it = 0; it < 20; it ++) {
		int p = 1000000007, c = ri(1, 20);
		vector<int> s(150, 1);
		for (int i = 1; i < 150; i ++)
			s[i] = i64(s[i - 1]) * (i + c) % p;
		vector<int> train(s.begin(), s.begin() + 40);
		auto rel =
		    p_recursive::find_recurrence_relation(train,
		                                          1, p);
		CHECK(p_recursive::extended(149, rel, train,
		                            p) == s);
	}
	for (int it = 0; it < 400; it ++) {
		i64 p = vector<i64>{
			4, 8, 12, 27, 1000, 1000000000
		}[it % 6];
		int d = ri(1, 8);
		vector<i64> c(d), s(150);
		for (auto &x : c)
			x = ri(0, p - 1);
		for (int i = 0; i < d; i ++)
			s[i] = it % 10 ? ri(0, p - 1) : 0;
		for (int i = d; i < 150; i ++)
			for (int j = 0; j < d; j ++)
				s[i] = (s[i] + c[j] * s[i - 1 - j]) % p;
		composite_rec::LinearRecurrence rec(
		    vector<i64>(s.begin(), s.begin() + 80), p,
		    false);
		for (int k : { 0, 1, 5, 79, 80, 100, 149 }) {
			if (rec.get_nth(k) != s[k]) {
				cerr << "RS it=" << it << " mod=" << p
				     << " d=" << d << " k=" << k
				     << " got=" << rec.get_nth(k)
				     << " expected=" << s[k] << "\n";
				for (auto x : c)
					cerr << x << " ";
				cerr << "\n";
				for (auto x : rec.trans)
					cerr << x << " ";
				cerr << "\n";
			}
			CHECK(rec.get_nth(k) == s[k]);
		}
	}
	for (i64 m : { 4, 8, 12 }) {
		vector<i64> s(30);
		s[0] = m / 2;
		composite_rec::LinearRecurrence r(s, m, false);
		CHECK(r.get_nth(0) == m / 2);
		CHECK(r.get_nth(1) == 0);
		CHECK(r.get_nth(1000000000000ULL) == 0);
	}
	for (int it = 0; it < 80; it ++) {
		i64 p = it % 2 ? 101 : 998244353;
		int d = ri(1, 7);
		vector<i64> c(d), s(120);
		for (auto &x : c) x = ri(0, p - 1);
		for (int i = 0; i < d; i ++) s[i] = ri(0, p - 1);
		s[0] = 0;
		for (int i = d; i < 120; i ++)
			for (int j = 0; j < d; j ++)
				s[i] = (s[i] + c[j] * s[i - 1 - j]) % p;
		composite_rec::LinearRecurrence r(vector<i64>(s.begin(), s.begin() + 60), p, true);
		for (int k : {0, 1, 59, 60, 90, 119}) CHECK(r.get_nth(k) == s[k]);
	}
	for (int it = 0; it < 300; it ++) {
		vector<double> a(ri(0, 80)), b(ri(0, 80));
		for (auto &x : a)
			x = ri(-100, 100) / 7.;
		for (auto &x : b)
			x = ri(-100, 100) / 7.;
		auto got = real_fft::convolution(a, b);
		if (a.empty() || b.empty())
			CHECK(got.empty());
		else {
			vector<double> out(a.size() + b.size() - 1);
			for (int i = 0; i < (int)a.size(); i ++)
				for (int j = 0; j < (int)b.size(); j ++)
					out[i + j] += a[i] * b[j];
			for (int i = 0; i < (int)out.size(); i ++)
				CHECK(abs(out[i] - got[i]) < 1e-7);
		}
	}
	for (int it = 0; it < 300; it ++) {
		int n = ri(1, 4), m = ri(0, 9), p = 101;
		vector<tuple<int, int, int>> ed;
		for (int i = 0; i < m; i ++)
			ed.emplace_back(ri(0, n - 1), ri(0, n - 1),
			                ri(0, 5));
		int expected = 0;
		for (int mask = 0; mask < (1 << m); mask ++)
			if (__builtin_popcount((unsigned)mask) ==
			    n - 1) {
				vector<int> par(n);
				iota(par.begin(), par.end(), 0);
				function<int(int)> f = [&](int x) {
					return par[x] == x
					           ? x
					           : par[x] = f(par[x]);
				};
				bool ok = true;
				int w = 1;
				for (int i = 0; i < m; i ++)
					if (mask >> i & 1) {
						auto [u, v, c] = ed[i];
						if (f(u) == f(v)) ok = false;
						par[f(u)] = f(v);
						w = w * c % p;
					}
				if (ok) expected = (expected + w) % p;
			}
		CHECK(weighted_spanning_trees(n, ed, p) ==
		      expected);
		for (int root = 0; root < n; root ++)
			for (bool toward : { false, true }) {
				int expected = 0;
				vector<vector<int>> options(n);
				for (int i = 0; i < m; i ++) {
					auto [u, v, w] = ed[i];
					if (!toward) swap(u, v);
					if (u != v && u != root)
						options[u].push_back(i);
				}
				vector<int> parent(n, -1);
				function<void(int, int)> dfs =
				    [&](int u, int product) {
					    if (u == n) {
						    for (int v = 0; v < n; v ++)
							    if (v != root) {
								    int x = v;
								    for (int k = 0;
								         k < n &&
								         x != root;
								         k ++)
									    x = parent[x];
								    if (x != root)
									    return;
							    }
						    expected =
						        (expected + product) %
						        p;
						    return;
					    }
					    if (u == root) {
						    dfs(u + 1, product);
						    return;
					    }
					    for (int i : options[u]) {
						    auto [a, b, w] = ed[i];
						    parent[u] = toward ? b : a;
						    dfs(u + 1, product * w % p);
					    }
				    };
				dfs(0, 1);
				CHECK(directed_spanning_trees(
				          n, root, ed, p, toward) ==
				      expected);
			}
	}
	done("advanced mathematics");
}
