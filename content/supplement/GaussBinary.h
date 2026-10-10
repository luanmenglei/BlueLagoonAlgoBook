/**
 * @title 异或方程组／解空间（GF(2)）
 * @brief 求二元域线性方程组的一个特解与全部自由方向。
 * @note m 个变量，下标 0..m-1，右侧为第 m 位；每行长度 ceil((m+1)/64)。返回可行性、一个特解与齐次解空间基，所有解为特解 XOR 基的任意组合。
 * @usage 方程 x0 XOR x1=1 编码为 `{{7}}`；`gauss_binary({{7}},2)` 返回特解与一个自由方向。
 * @time O(nm ceil((m+1)/64))
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct BinarySolution {
	bool ok;
	int rank;
	vector<int> value;
	vector<vector<int>> basis;
};
BinarySolution gauss_binary(vector<vector<u64>> a,
                            int m) {
	int n = a.size(), w = (m + 64) / 64, r = 0;
	for (auto &v : a)
		assert((int)v.size() == w);
	auto bit = [&](int i, int j) {
		return (a[i][j / 64] >> (j % 64)) & 1;
	};
	vector<int> where(m, -1);
	for (int j = 0; j < m; j ++) {
		int p = r;
		while (p < n && !bit(p, j))
			p ++;
		if (p == n) continue;
		swap(a[p], a[r]);
		where[j] = r;
		for (int i = 0; i < n; i ++)
			if (i != r && bit(i, j))
				for (int k = 0; k < w; k ++)
					a[i][k] ^= a[r][k];
		r ++;
	}
	for (int i = r; i < n; i ++)
		if (bit(i, m)) return { false, r, {}, {} };
	BinarySolution out{ true, r, vector<int>(m), {} };
	for (int j = 0; j < m; j ++)
		if (where[j] != -1)
			out.value[j] = bit(where[j], m);
		else {
			vector<int> v(m);
			v[j] = 1;
			for (int k = 0; k < m; k ++)
				if (where[k] != -1)
					v[k] = bit(where[k], j);
			out.basis.push_back(v);
		}
	return out;
}
