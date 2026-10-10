/**
 * @title 积性函数求和（Min_25 筛）
 * @brief 统计素数个数／和，并求指定形式积性函数的前缀和。
 * @note n>=1；cnt[0] 与 sum[0] 是素数个数／和。multiplicative_sum 要求 $f(1)=1$、积性、$f(p)=ap+b$，并传入
 * `f(p,e)` $=f(p^e)$。仅对构造时同一个 n 的整除分块值调用 id；i128 结果不溢出。
 * @time 典型 O(n^(3/4)/log n) 筛阶段，递归依赖 f；空间 O(sqrt n)
 * @depends contest/Base.h, number-theory/Sieve.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Sieve.h"

struct Min25 {
	i64 n, sq;
	vector<i64> w, cnt;
	vector<i128> sum, pref;
	vector<int> lo, hi, pr;
	Min25(i64 n)
	    : n(n), sq(sqrt((long double)n)), lo(sq + 2),
	      hi(sq + 2) {
		assert(n >= 1);
		while ((sq + 1) <= n / (sq + 1))
			sq ++;
		while (sq > n / sq)
			sq --;
		lo.resize(sq + 2);
		hi.resize(sq + 2);
		for (i64 l = 1, r; l <= n; l = r + 1) {
			r = n / (n / l);
			i64 x = n / l;
			int id = w.size();
			w.push_back(x);
			cnt.push_back(x - 1);
			sum.push_back(i128(x) * (x + 1) / 2 - 1);
			if (x <= sq)
				lo[x] = id;
			else
				hi[n / x] = id;
		}
		Sieve s(sq);
		pr = s.primes;
		pref = { 0 };
		for (int i = 0; i < (int)pr.size(); i ++) {
			i64 p = pr[i];
			pref.push_back(pref.back() + p);
			for (int j = 0;
			     j < (int)w.size() && p <= w[j] / p;
			     j ++) {
				int k = id(w[j] / p);
				cnt[j] -= cnt[k] - i;
				sum[j] -= i128(p) * (sum[k] - pref[i]);
			}
		}
	}
	int id(i64 x) const {
		return x <= sq ? lo[x] : hi[n / x];
	}
	template <class F>
	i128 rec(i64 x, int k, i64 a, i64 b, F &f) const {
		if (x < 2 || (k && x <= pr[k - 1])) return 0;
		int z = id(x);
		i128 ans = a * (sum[z] - pref[k]) +
		           i128(b) * (cnt[z] - k);
		for (int i = k;
		     i < (int)pr.size() && pr[i] <= x / pr[i];
		     i ++) {
			i64 p = pr[i], pe = p;
			for (int e = 1; pe <= x / p; e ++, pe *= p)
				ans += f(p, e) *
				           rec(x / pe, i + 1, a, b, f) +
				       f(p, e + 1);
		}
		return ans;
	}
	template <class F>
	i128 multiplicative_sum(i64 a, i64 b, F f) const {
		return 1 + rec(n, 0, a, b, f);
	}
};
