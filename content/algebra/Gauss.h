/**
 * @title 实数／模素数高斯消元与行列式
 * @brief 求实数／素数模线性方程组的解，并计算方阵行列式。
 * @note 增广矩阵 n×(m+1)，返回0无解／1唯一／2多解，自由变量取0。模版要求 p 为素数、系数归一；determinant 输入方阵。实数阈值须按规模调整。
 * @time O(nm min(n,m))；行列式 O(n^3)
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

int gauss_real(vector<vector<double>> a,
               vector<double> &ans, double eps = 1e-9) {
	int n = a.size(), m = n ? a[0].size() - 1 : 0,
	    r = 0;
	vector<int> pos(m, -1);
	for (int c = 0; c < m && r < n; c ++) {
		int p = r;
		for (int i = r; i < n; i ++)
			if (abs(a[i][c]) > abs(a[p][c])) p = i;
		if (abs(a[p][c]) < eps) continue;
		swap(a[p], a[r]);
		pos[c] = r;
		double v = a[r][c];
		for (int j = c; j <= m; j ++)
			a[r][j] /= v;
		for (int i = 0; i < n; i ++)
			if (i != r) {
				v = a[i][c];
				for (int j = c; j <= m; j ++)
					a[i][j] -= v * a[r][j];
			}
		r ++;
	}
	ans.assign(m, 0);
	for (int i = 0; i < m; i ++)
		if (pos[i] != -1) ans[i] = a[pos[i]][m];
	for (int i = r; i < n; i ++)
		if (abs(a[i][m]) > eps) return 0;
	return r == m ? 1 : 2;
}
int gauss_mod(vector<vector<int>> a, vector<int> &ans,
              int p) {
	int n = a.size(), m = n ? a[0].size() - 1 : 0,
	    r = 0;
	vector<int> pos(m, -1);
	for (int c = 0; c < m && r < n; c ++) {
		int k = r;
		while (k < n && !a[k][c])
			k ++;
		if (k == n) continue;
		swap(a[k], a[r]);
		pos[c] = r;
		i64 v = pow_mod(a[r][c], p - 2, p);
		for (int j = c; j <= m; j ++)
			a[r][j] = a[r][j] * v % p;
		for (int i = 0; i < n; i ++)
			if (i != r) {
				v = a[i][c];
				for (int j = c; j <= m; j ++)
					a[i][j] = (a[i][j] -
					           v * a[r][j] % p + p) %
					          p;
			}
		r ++;
	}
	ans.assign(m, 0);
	for (int i = 0; i < m; i ++)
		if (pos[i] != -1) ans[i] = a[pos[i]][m];
	for (int i = r; i < n; i ++)
		if (a[i][m]) return 0;
	return r == m ? 1 : 2;
}
int determinant(vector<vector<int>> a, int p) {
	int n = a.size();
	i64 ans = 1;
	for (int c = 0; c < n; c ++) {
		int k = c;
		while (k < n && !a[k][c])
			k ++;
		if (k == n) return 0;
		if (k != c)
			swap(a[k], a[c]), ans = (p - ans) % p;
		ans = ans * a[c][c] % p;
		i64 inv = pow_mod(a[c][c], p - 2, p);
		for (int i = c + 1; i < n; i ++) {
			i64 v = a[i][c] * inv % p;
			for (int j = c; j < n; j ++)
				a[i][j] =
				    (a[i][j] - v * a[c][j] % p + p) % p;
		}
	}
	return ans;
}
