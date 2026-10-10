/**
 * @title 线性筛／质因子／欧拉与莫比乌斯函数
 * @brief 线性预处理质数、最小质因子、欧拉函数及莫比乌斯函数。
 * @note n>=0；lp 最小质因子，phi 欧拉函数，mu 莫比乌斯函数。反复除 lp 可分解 n 以内整数。
 * @time O(n)，空间 O(n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Sieve {
	vector<int> primes, lp, phi, mu;
	Sieve(int n) : lp(n + 1), phi(n + 1), mu(n + 1) {
		if (n >= 1) phi[1] = mu[1] = 1;
		for (int i = 2; i <= n; i ++) {
			if (!lp[i])
				lp[i] = i, primes.push_back(i),
				phi[i] = i - 1, mu[i] = -1;
			for (int p : primes) {
				if (p > n / i) break;
				lp[i * p] = p;
				if (i % p == 0) {
					phi[i * p] = phi[i] * p;
					break;
				}
				phi[i * p] = phi[i] * (p - 1);
				mu[i * p] = -mu[i];
			}
		}
	}
};
