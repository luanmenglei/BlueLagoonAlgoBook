/**
 * @title 线性规划最优化（单纯形法）
 * @brief 求非负变量在线性不等式约束下的线性目标最大值。
 * @note 最大化 $c^{\mathsf T}x$，约束 $Ax\le b$、$x\ge0$，要求至少一条约束。solve(x) 返回 -inf 无解、+inf
 * 无界或最优值；数值稳定性依赖建模尺度，需检查约束残差。
 * @time O(nm * pivot数)，最坏指数
 * @depends contest/Base.h
 * @author Stanford; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/numerical/Simplex.h
 * @license MIT
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

using T =
    double; // long double, Rational, double + mod<P>...
using vd = vector<T>;
using vvd = vector<vd>;

const T eps = 1e-8, inf = 1 / .0;
#define MP make_pair
#define ltj(X)                                         \
	if (s == -1 || MP(X[j], N[j]) < MP(X[s], N[s]))    \
	s = j

struct LPSolver {
	int m, n;
	vector<int> N, B;
	vvd D;

	LPSolver(const vvd &A, const vd &b, const vd &c)
	    : m((int)(b).size()), n((int)(c).size()),
	      N(n + 1), B(m), D(m + 2, vd(n + 2)) {
		for (int i = 0; i < m; i ++)
			for (int j = 0; j < n; j ++)
				D[i][j] = A[i][j];
		for (int i = 0; i < m; i ++) {
			B[i] = n + i;
			D[i][n] = -1;
			D[i][n + 1] = b[i];
		}
		for (int j = 0; j < n; j ++) {
			N[j] = j;
			D[m][j] = -c[j];
		}
		N[n] = -1;
		D[m + 1][n] = 1;
	}

	void pivot(int r, int s) {
		T *a = D[r].data(), inv = 1 / a[s];
		for (int i = 0; i < m + 2; i ++)
			if (i != r && abs(D[i][s]) > eps) {
				T *b = D[i].data(), inv2 = b[s] * inv;
				for (int j = 0; j < n + 2; j ++)
					b[j] -= a[j] * inv2;
				b[s] = a[s] * inv2;
			}
		for (int j = 0; j < n + 2; j ++)
			if (j != s) D[r][j] *= inv;
		for (int i = 0; i < m + 2; i ++)
			if (i != r) D[i][s] *= -inv;
		D[r][s] = inv;
		swap(B[r], N[s]);
	}

	bool simplex(int phase) {
		int x = m + phase - 1;
		for (;;) {
			int s = -1;
			for (int j = 0; j < n + 1; j ++)
				if (N[j] != -phase) ltj(D[x]);
			if (D[x][s] >= -eps) return true;
			int r = -1;
			for (int i = 0; i < m; i ++) {
				if (D[i][s] <= eps) continue;
				if (r == -1 ||
				    MP(D[i][n + 1] / D[i][s], B[i]) <
				        MP(D[r][n + 1] / D[r][s], B[r]))
					r = i;
			}
			if (r == -1) return false;
			pivot(r, s);
		}
	}

	T solve(vd &x) {
		int r = 0;
		for (int i = 1; i < m; i ++)
			if (D[i][n + 1] < D[r][n + 1]) r = i;
		if (D[r][n + 1] < -eps) {
			pivot(r, n);
			if (!simplex(2) || D[m + 1][n + 1] < -eps)
				return -inf;
			for (int i = 0; i < m; i ++)
				if (B[i] == -1) {
					int s = 0;
					for (int j = 1; j < n + 1; j ++)
						ltj(D[i]);
					pivot(i, s);
				}
		}
		bool ok = simplex(1);
		x = vd(n);
		for (int i = 0; i < m; i ++)
			if (B[i] < n) x[B[i]] = D[i][n + 1];
		return ok ? D[m][n + 1] : inf;
	}
};

#undef MP
#undef ltj
