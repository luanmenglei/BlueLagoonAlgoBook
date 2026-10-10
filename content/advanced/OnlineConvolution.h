/**
 * @title 一侧固定的在线卷积
 * @brief 固定一侧系数，另一侧逐项输入时立即返回当前卷积系数。
 * @note 输入系数均归一到 [0,998244353)。模998244353。构造时固定核 a，声明输入长度 n；push(x) 依次输入 b[k]，立即返回
 * $\sum_{i=0}^k a_i b_{k-i}$。无未来项访问；k<n。不是双侧均在线的 relaxed 卷积。
 * @usage `poly::OnlineConvolution c({1,2},3);`；依次 push(3),push(4),push(5) 得3,10,13。
 * @time 总 O(n log² n)，空间 O(n)
 * @depends contest/Base.h, algebra/NTT.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../algebra/NTT.h"

namespace poly {
struct OnlineConvolution {
	Vec a, b, c;
	int n;
	OnlineConvolution(Vec a, int n)
	    : a(move(a)), c(n), n(n) {
		if ((int)this->a.size() > n) this->a.resize(n);
	}
	int push(int x) {
		int k = b.size();
		assert(k < n);
		b.push_back(x);
		if (!a.empty())
			c[k] = (c[k] + i64(a[0]) * x) % mod;
		for (int z = 1; ((k + 1) & (z - 1)) == 0 &&
		                z < (int)a.size() && k + 1 < n;
		     z <<= 1) {
			int r = min(2 * z, (int)a.size());
			Vec u(a.begin() + z, a.begin() + r),
			    v(b.end() - z, b.end());
			Vec w = conv(u, v);
			for (int j = 0;
			     j < (int)w.size() && k + 1 + j < n;
			     j ++)
				c[k + 1 + j] =
				    (c[k + 1 + j] + i64(w[j])) % mod;
		}
		return c[k];
	}
};
} // namespace poly
