/**
 * @title 组合数取模（Lucas／扩展 Lucas）
 * @brief 计算大 n 的组合数模素数或合数，可复用质数幂预处理。
 * @note 扩展 Lucas：分解模数到质数幂、剔除 p 因子阶乘，再 CRT。每个 $p^e$ 都须能建立 $\mathcal{O}(p^e)$ 表，建议
 * <=2e6；n<=LLONG_MAX。素数情形即 Lucas，可复用预处理对象。
 * @time 预处理 O(sum p^e)，每次 O(log n log mod)，另加分解
 * @depends contest/Base.h, number-theory/Factor.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Factor.h"

struct PrimePowerBinomial {
	i64 p, mod;
	vector<i64> fac;
	PrimePowerBinomial(i64 p, i64 pk)
	    : p(p), mod(pk), fac(pk + 1, 1) {
		for (i64 i = 1; i <= mod; i ++)
			fac[i] = i % p ? mul_mod(fac[i - 1], i, mod)
			               : fac[i - 1];
	}
	i64 exponent(u64 n) const {
		i64 ans = 0;
		while (n)
			n /= p, ans += n;
		return ans;
	}
	i64 fact(u64 n) const {
		if (!n) return 1;
		return mul_mod(
		    mul_mod(pow_mod(fac[mod], n / mod, mod),
		            fac[n % mod], mod),
		    fact(n / p), mod);
	}
	i64 choose(u64 n, u64 k) const {
		if (k > n) return 0;
		i64 e =
		    exponent(n) - exponent(k) - exponent(n - k);
		u64 v = mul_mod(fact(n), inverse(fact(k), mod),
		                mod);
		v = mul_mod(v, inverse(fact(n - k), mod), mod);
		return mul_mod(v, pow_mod(p, e, mod), mod);
	}
};
i64 binomial_mod(u64 n, u64 k, i64 mod) {
	assert(mod > 0);
	if (mod == 1 || k > n) return 0;
	vector<u64> a;
	factor(mod, a);
	sort(a.begin(), a.end());
	i64 ans = 0, m = 1;
	for (int i = 0; i < (int)a.size();) {
		i64 p = a[i], pk = 1;
		while (i < (int)a.size() && a[i] == (u64)p)
			pk *= p, i ++;
		i64 v = PrimePowerBinomial(p, pk).choose(n, k);
		auto z = crt(ans, m, v, pk);
		tie(ans, m) = *z;
	}
	return ans;
}
