/**
 * @title 整数分拆数／两类斯特林数
 * @brief 计算整数无序分拆数，以及两类斯特林数的三角表。
 * @note 任意正 int 模数；partition 返回 p(0)..p(n)。stirling 的 first=true 表示第一类无符号，否则第二类；返回三角表，空位为0。
 * @usage `partition_table(5,mod)[5]` 为7；`stirling_table(3,mod)[3][2]` 为3。
 * @time 分拆 O(n sqrt(n))；Stirling O(n²)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

vector<int> partition_table(int n, int mod) {
	vector<int> p(n + 1);
	p[0] = 1 % mod;
	for (int i = 1; i <= n; i ++)
		for (i64 k = 1; k * (3 * k - 1) / 2 <= i; k ++) {
			int sign = k & 1 ? 1 : -1;
			for (i64 g : { k * (3 * k - 1) / 2,
			               k * (3 * k + 1) / 2 })
				if (g <= i)
					p[i] =
					    (i64(p[i]) +
					     sign * i64(p[i - g]) + mod) %
					    mod;
		}
	return p;
}
vector<vector<int>> stirling_table(int n, int mod,
                                   bool first = false) {
	vector<vector<int>> s(n + 1, vector<int>(n + 1));
	s[0][0] = 1 % mod;
	for (int i = 1; i <= n; i ++)
		for (int j = 1; j <= i; j ++)
			s[i][j] =
			    (s[i - 1][j - 1] +
			     i64(first ? i - 1 : j) * s[i - 1][j]) %
			    mod;
	return s;
}
