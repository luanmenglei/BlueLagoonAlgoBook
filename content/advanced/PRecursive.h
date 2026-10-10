/**
 * @title 多项式系数递推猜测与延拓
 * @brief 由样本猜测多项式系数线性递推，并据此延拓后续项。
 * @note namespace p_recursive。find_recurrence_relation(terms,deg,prime,verify) 猜测 $\sum_i
 * c_i(n-i)a_{n-i}=0$；extended 延拓。样本必须足够、首项多项式在延拓点可逆，否则 assert。拟合成功不证明无限序列规律；务必留独立验证项。
 * @usage 阶乘样本 {1,1,2,6,24,120}、deg=1 可猜测；`extended` 需留足初项。约定为 $\sum_i c_i(n-i)a_{n-i}=0$。
 * @time 消元约 O(N³)，延拓 O(Nrd)
 * @depends contest/Base.h
 * @author koosaga / upstream P-recursive; BlueLagoon adaptation
 * @source
 * https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/math/recurrence/recurrence_precursive.cpp
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

namespace p_recursive {

int mod_inv(int a, int mod) {
	int b = mod, s = 1, t = 0, old_a = a;
	while (b) {
		int q = a / b;
		swap(a %= b, b);
		swap(s -= t * q, t);
	}
	if (a != 1) {
		fprintf(
		    stderr,
		    "Error: %d^{-1} mod %d does not exist.\n\n",
		    old_a, mod);
		assert(0);
	}
	return s < 0 ? s + mod : s;
}

vector<int> extended(int n,
                     const vector<vector<int>> &coeffs,
                     const vector<int> &terms,
                     int mod) {

	vector<int> ret(max<int>(n + 1, terms.size()));
	copy(terms.begin(), terms.end(), ret.begin());
	const int order = coeffs.size() - 1;
	const int deg = coeffs[0].size() - 1;
	assert((int)terms.size() >= order);
	for (int m = terms.size(); m <= n; ++m) {
		int s = 0;
		for (int i = 1; i <= order; ++i) {
			int k = m - i, t = ret[k];
			for (int d = 0; d <= deg; ++d) {
				s = (s + i64(t) * coeffs[i][d]) % mod;
				t = i64(t) * k % mod;
			}
		}
		int denom = 0, mpow = 1;
		for (int d = 0; d <= deg; ++d) {
			denom = (denom + i64(mpow) * coeffs[0][d]) %
			        mod;
			mpow = i64(mpow) * m % mod;
		}
		ret[m] =
		    i64(mod - s) * mod_inv(denom, mod) % mod;
	}
	return ret;
}

vector<vector<int>>
find_recurrence_relation(const vector<int> &terms,
                         const int deg, const int mod,
                         bool verify = true) {

	const int n = terms.size();
	const int B =
	    (n + 2) / (deg + 2);     // number of blocks
	const int C = B * (deg + 1); // number of columns
	const int R = n - (B - 1);   // number of rows
	assert(B >= 2);
	assert(R >= C - 1);

	auto mul = [mod](int a, int b) {
		return i64(a) * b % mod;
	};
	auto fixed = [mod](int a) {
		return (a %= mod) < 0 ? a + mod : a;
	};
	auto error = [](int order, int deg) {
		fprintf(stderr,
		        "Error: Could not find a recurrence "
		        "relation "
		        "of order <= %d and degree <= %d.\n\n",
		        order, deg);
		assert(0);
	};

	vector<vector<int>> mat(R, vector<int>(C));
	for (int y = 0; y < R; ++y) {
		for (int b = 0; b < B; ++b) {
			for (int d = 0, v = fixed(terms[y + b]);
			     d <= deg; ++d) {
				mat[y][b * (deg + 1) + d] = v;
				v = mul(v, y + b);
			}
		}
	}

	int rank = 0;
	for (int x = 0; x < C; ++x) {
		int pivot = -1;
		for (int y = rank; y < R; ++y)
			if (mat[y][x] != 0) {
				pivot = y;
				break;
			}
		if (pivot < 0) break;
		if (pivot != rank) swap(mat[rank], mat[pivot]);
		int inv = mod_inv(mat[rank][x], mod);
		for (int x2 = x; x2 < C; ++x2)
			mat[rank][x2] = mul(mat[rank][x2], inv);
		for (int y = rank + 1; y < R; ++y)
			if (mat[y][x]) {
				int c = mod - mat[y][x];
				for (int x2 = x; x2 < C; ++x2) {
					mat[y][x2] =
					    (i64(mat[y][x2]) +
					     i64(c) * mat[rank][x2]) %
					    mod;
				}
			}
		++rank;
	}

	if (rank == C) error(B - 1, deg);

	for (int y = rank - 1; y >= 0; --y)
		if (mat[y][rank]) {
			assert(mat[y][y] == 1);
			int c = mod - mat[y][rank];
			for (int y2 = 0; y2 < y; ++y2) {
				mat[y2][rank] = (mat[y2][rank] +
				                 i64(c) * mat[y2][y]) %
				                mod;
			}
		}

	int order = rank / (deg + 1);
	vector<vector<int>> ret(order + 1,
	                        vector<int>(deg + 1));
	ret[0][rank % (deg + 1)] = 1;
	for (int y = rank - 1; y >= 0; --y) {
		int k = order - y / (deg + 1),
		    d = y % (deg + 1);
		ret[k][d] = (mod - mat[y][rank]) % mod;
	}

	if (verify) {
		auto extended_terms =
		    extended(n - 1, ret,
		             vector<int>(terms.begin(),
		                         terms.begin() + order),
		             mod);
		for (int i = 0; i < (int)terms.size(); ++i) {
			if (fixed(terms[i] - extended_terms[i]) !=
			    0)
				error(B - 1, deg);
		}
	}

	return ret;
}

} // namespace p_recursive
