/**
 * @title 反对称矩阵 Pfaffian
 * @brief 计算偶数阶反对称矩阵的 Pfaffian，其平方等于行列式。
 * @note 偶数阶反对称矩阵，系数归一到[0,p)，p 为奇素数。返回 pf(A)，满足
 * $\operatorname{pf}(A)^2=\det(A)$；空矩阵返回1。不是直接的整数匹配计数器。
 * @usage `pfaffian({{0,3},{98,0}},101)` 为3；平方等于该矩阵行列式。
 * @time O(n³)
 * @depends contest/Base.h, number-theory/Modular.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../number-theory/Modular.h"

int pfaffian(vector<vector<int>> a, int p) {
	int n = a.size();
	assert(n % 2 == 0 && p > 2);
	i64 ans = 1;
	for (int i = 0; i < n; i += 2) {
		int j = i + 1;
		while (j < n && !a[i][j])
			j ++;
		if (j == n) return 0;
		if (j != i + 1) {
			swap(a[j], a[i + 1]);
			for (auto &r : a)
				swap(r[j], r[i + 1]);
			ans = (p - ans) % p;
		}
		int v = a[i][i + 1], inv = pow_mod(v, p - 2, p);
		ans = ans * v % p;
		for (int x = i + 2; x < n; x ++)
			for (int y = x + 1; y < n; y ++) {
				i64 z = (i64(a[i][y]) * a[i + 1][x] -
				         i64(a[i][x]) * a[i + 1][y]) %
				        p * inv % p;
				a[x][y] = (a[x][y] + z + p) % p;
				a[y][x] = a[x][y] ? p - a[x][y] : 0;
			}
	}
	return ans;
}
