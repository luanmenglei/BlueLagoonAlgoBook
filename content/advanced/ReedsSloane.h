/**
 * @title 合数模线性递推／第 k 项（Reeds-Sloane）
 * @brief 在合数模数下由样本猜测常系数递推，再求第 k 项。
 * @note namespace composite_rec，LinearRecurrence(samples,mod) 默认合数分支；仅在模数确为素数时可传
 * true。1<mod<=10^9，样本非空且归一，建议至少2d项并留独立验证；get_nth 返回 u64
 * 下标项。试除分解模数，零序列返回0；拟合不证明未知序列的无限规律。修正负余数更新、前缀瞬态与素数分支前导零；CRT 用 i128。
 * @usage Fibonacci 的充分样本、mod=8：`composite_rec::LinearRecurrence
 * r(samples,8,false);`；`r.get_nth(10)` 为7。
 * @time 猜测约 O(N² log mod) 加试除；取项 O(d² log k)
 * @depends contest/Base.h
 * @author koosaga / upstream Reeds-Sloane; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/math/recurrence/recurrence_reedssloane.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

namespace composite_rec {

using lint = i64;

struct LinearRecurrence {
	using ll = i64;
	using vec = vector<ll>;

	static void extand(vec &a, ll d, ll value = 0) {
		if (d <= a.size()) return;
		a.resize(d, value);
	}

	static vec BerlekampMassey(const vec &s, ll mod) {
		vec c{ 1 }, b{ 1 };
		int len = 0, gap = 1;
		ll old = 1;
		auto inv = [&](ll x) {
			ll ans = 1;
			for (ll k = mod - 2; k;
			     k >>= 1, x = x * x % mod)
				if (k & 1) ans = ans * x % mod;
			return ans;
		};
		for (int i = 0; i < (int)s.size(); i ++) {
			ll d = s[i];
			for (int j = 1; j < (int)c.size() && j <= i;
			     j ++)
				d = (d + c[j] * s[i - j]) % mod;
			if (!d) {
				gap ++;
				continue;
			}
			vec t = c;
			ll coef = d * inv(old) % mod;
			c.resize(max(c.size(), b.size() + gap));
			for (int j = 0; j < (int)b.size(); j ++)
				c[j + gap] = (c[j + gap] -
				              coef * b[j] % mod + mod) %
				             mod;
			if (2 * len <= i)
				len = i + 1 - len, b = move(t), old = d,
				gap = 1;
			else
				gap ++;
		}
		c.resize(len + 1);
		return c;
	}
	static void exgcd(ll a, ll b, ll &g, ll &x, ll &y) {
		if (!b)
			x = 1, y = 0, g = a;
		else {
			exgcd(b, a % b, g, y, x);
			y -= x * (a / b);
		}
	}
	static ll crt(const vec &c, const vec &m) {
		ll n = c.size();
		ll M = 1, ans = 0;
		for (ll i = 0; i < n; ++i)
			M *= m[i];
		for (ll i = 0; i < n; ++i) {
			ll x, y, g, tm = M / m[i];
			exgcd(tm, m[i], g, x, y);
			ans = (ans + i128(tm) * x * c[i] % M) % M;
		}
		return (ans + M) % M;
	}
	static vec ReedsSloane(const vec &s, ll mod) {
		assert(mod > 1 && mod <= 1000000000LL &&
		       !s.empty());
		auto inverse = [](ll a, ll m) {
			ll d, x, y;
			exgcd(a, m, d, x, y);
			return d == 1 ? (x % m + m) % m : -1;
		};
		auto L = [](const vec &a, const vec &b) {
			ll da = (a.size() > 1 ||
			         (a.size() == 1 && a[0]))
			            ? (ll)a.size() - 1
			            : -1000;
			ll db = (b.size() > 1 ||
			         (b.size() == 1 && b[0]))
			            ? (ll)b.size() - 1
			            : -1000;
			return max(da, db + 1);
		};
		auto prime_power = [&](const vec &s, ll mod,
		                       ll p, ll e) {
			// linear feedback shift register mod p^e, p
			// is prime
			vector<vec> a(e), b(e), an(e), bn(e), ao(e),
			    bo(e);
			vec t(e), u(e), r(e), to(e, 1), uo(e),
			    pw(e + 1);
			;

			pw[0] = 1;
			for (ll i = pw[0] = 1; i <= e; ++i)
				pw[i] = pw[i - 1] * p;
			for (ll i = 0; i < e; ++i) {
				a[i] = { pw[i] }, an[i] = { pw[i] };
				b[i] = { 0 },
				bn[i] = { s[0] * pw[i] % mod };
				t[i] = s[0] * pw[i] % mod;
				if (t[i] == 0) {
					t[i] = 1, u[i] = e;
				} else {
					for (u[i] = 0; t[i] % p == 0;
					     t[i] /= p, ++u[i])
						;
				}
			}
			for (ll k = 1; k < s.size(); ++k) {
				for (ll g = 0; g < e; ++g) {
					if (L(an[g], bn[g]) >
					    L(a[g], b[g])) {
						ao[g] = a[e - 1 - u[g]];
						bo[g] = b[e - 1 - u[g]];
						to[g] = t[e - 1 - u[g]];
						uo[g] = u[e - 1 - u[g]];
						r[g] = k - 1;
					}
				}
				a = an, b = bn;
				for (ll o = 0; o < e; ++o) {
					ll d = 0;
					for (ll i = 0;
					     i < a[o].size() && i <= k;
					     ++i) {
						d = (d + a[o][i] * s[k - i]) %
						    mod;
					}
					if (d == 0) {
						t[o] = 1, u[o] = e;
					} else {
						for (u[o] = 0, t[o] = d;
						     t[o] % p == 0;
						     t[o] /= p, ++u[o])
							;
						ll g = e - 1 - u[o];
						if (L(a[g], b[g]) == 0) {
							extand(bn[o], k + 1);
							bn[o][k] =
							    (bn[o][k] + d) % mod;
						} else {
							ll coef =
							    t[o] *
							    inverse(to[g], mod) %
							    mod * pw[u[o] - uo[g]] %
							    mod;
							ll m = k - r[g];
							extand(an[o],
							       ao[g].size() + m);
							extand(bn[o],
							       bo[g].size() + m);
							for (ll i = 0;
							     i < ao[g].size();
							     ++i) {
								an[o][i + m] -=
								    coef * ao[g][i] %
								    mod;
								if (an[o][i + m] < 0)
									an[o][i + m] += mod;
							}
							while (an[o].size() &&
							       an[o].back() == 0)
								an[o].pop_back();
							for (ll i = 0;
							     i < bo[g].size();
							     ++i) {
								bn[o][i + m] -=
								    coef * bo[g][i] %
								    mod;
								if (bn[o][i + m] < 0)
									bn[o][i + m] += mod;
							}
							while (bn[o].size() &&
							       bn[o].back() == 0)
								bn[o].pop_back();
						}
					}
				}
			}
			return make_pair(an[0], bn[0]);
		};

		vector<tuple<ll, ll, ll>> fac;
		for (ll i = 2; i * i <= mod; ++i)
			if (mod % i == 0) {
				ll cnt = 0, pw = 1;
				while (mod % i == 0)
					mod /= i, ++cnt, pw *= i;
				fac.emplace_back(pw, i, cnt);
			}
		if (mod > 1) fac.emplace_back(mod, mod, 1);
		vector<vec> as;
		ll n = 0;
		for (auto &&x : fac) {
			ll mod, p, e;
			vec a, b;
			tie(mod, p, e) = x;
			auto ss = s;
			for (auto &&x : ss)
				x %= mod;
			tie(a, b) = prime_power(ss, mod, p, e);
			as.emplace_back(a);
			n = max(
			    n, max((ll)a.size(), (ll)b.size() + 1));
		}
		vec a(n), c(as.size()), m(as.size());

		for (ll i = 0; i < n; ++i) {
			for (ll j = 0; j < as.size(); ++j) {
				m[j] = get<0>(fac[j]);
				c[j] = i < as[j].size() ? as[j][i] : 0;
			}
			a[i] = crt(c, m);
		}
		return a;
	}

	LinearRecurrence(const vec &s, const vec &c, ll mod)
	    : init(s), trans(c), mod(mod), m(s.size()) {}

	LinearRecurrence(const vec &s, ll mod,
	                 bool is_prime = false)
	    : mod(mod) {
		vec A;
		if (is_prime)
			A = BerlekampMassey(s, mod);
		else
			A = ReedsSloane(s, mod);
		if (A.empty()) A = { 0 };
		m = A.size() - 1;

		trans.resize(m);
		for (ll i = 0; i < m; ++i) {
			trans[i] = (mod - A[i + 1]) % mod;
		}
		init = { s.begin(), s.begin() + m };
	}
	int get_nth(unsigned long long n) {
		int m = trans.size();
		if (!m) return 0;
		vector<int> s(m), t(m);
		s[0] = 1;
		if (m != 1)
			t[1] = 1;
		else
			t[0] = trans[0];
		auto mul = [&](vector<int> v, vector<int> w) {
			int m = v.size();
			vector<int> t(2 * m);
			for (int j = 0; j < m; j ++) {
				for (int k = 0; k < m; k ++) {
					t[j + k] += 1ll * v[j] * w[k] % mod;
					if (t[j + k] >= mod)
						t[j + k] -= mod;
				}
			}
			for (int j = 2 * m - 1; j >= m; j --) {
				for (int k = 1; k <= m; k ++) {
					t[j - k] +=
					    1ll * t[j] * trans[k - 1] % mod;
					if (t[j - k] >= mod)
						t[j - k] -= mod;
				}
			}
			t.resize(m);
			return t;
		};
		while (n) {
			if (n & 1) s = mul(s, t);
			t = mul(t, t);
			n >>= 1;
		}
		lint ret = 0;
		for (int i = 0; i < m; i ++)
			ret += 1ll * s[i] * init[i] % mod;
		return ret % mod;
	}
	vec init, trans;
	ll mod;
	ll m;
};

} // namespace composite_rec
