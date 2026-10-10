/**
 * @title 有理函数第 k 项／快速递推（Bostan-Mori）
 * @brief 求有理生成函数的指定系数，也可加速常系数线性递推取项。
 * @note 输入系数均归一到 [0,998244353)。模998244353，Q[0]非零；返回 [x^k]P/Q，k 为 u64。linear_rec_fast 的 c[i] 表示
 * a[n]=Σc[i]a[n-1-i]；init 长度等于阶数。
 * @usage `poly::bostan_mori({0,1},{1,mod-1,mod-1},10)` 得55；即 $\frac{x}{1-x-x^2}$。
 * @time O(d log d log k)
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
int bostan_mori(Vec p, Vec q, u64 k) {
	assert(!q.empty() && q[0]);
	if (p.empty()) return 0;
	while (k) {
		Vec qm = q;
		for (int i = 1; i < (int)qm.size(); i += 2)
			qm[i] = qm[i] ? mod - qm[i] : 0;
		Vec u = conv(p, qm), v = conv(q, qm), np, nq;
		for (int i = k & 1; i < (int)u.size(); i += 2)
			np.push_back(u[i]);
		for (int i = 0; i < (int)v.size(); i += 2)
			nq.push_back(v[i]);
		p = move(np);
		q = move(nq);
		k >>= 1;
	}
	return p.empty()
	           ? 0
	           : i64(p[0]) * power(q[0], mod - 2) % mod;
}
int linear_rec_fast(const Vec &init, const Vec &c,
                    u64 k) {
	int d = c.size();
	assert((int)init.size() == d);
	if (!d) return 0;
	Vec q(d + 1);
	q[0] = 1;
	for (int i = 0; i < d; i ++)
		q[i + 1] = c[i] ? mod - c[i] : 0;
	Vec p = conv(init, q);
	p.resize(d);
	return bostan_mori(p, q, k);
}
} // namespace poly
