/**
 * @title 三维凸包
 * @brief 求一般位置三维点集的凸包，输出外向三角面。
 * @note 至少4点且任意4点不共面；输出外向三角面。含共面输入先选择适合的退化处理方案，不可直接套用。
 * @time O(n^2)
 * @depends contest/Base.h, geometry/Point3D.h
 * @author Johan Sannemo; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/3dHull.h
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point3D.h"

using P3 = Point3D<double>;

struct PR {
	void ins(int x) { (a == -1 ? a : b) = x; }
	void rem(int x) { (a == x ? a : b) = -1; }
	int cnt() { return (a != -1) + (b != -1); }
	int a, b;
};

struct F {
	P3 q;
	int a, b, c;
};

vector<F> hull3d(const vector<P3> &A) {
	assert((int)(A).size() >= 4);
	vector<vector<PR>> E(
	    (int)(A).size(),
	    vector<PR>((int)(A).size(), { -1, -1 }));
#define E(x, y) E[f.x][f.y]
	vector<F> FS;
	auto mf = [&](int i, int j, int k, int l) {
		P3 q = (A[j] - A[i]).cross((A[k] - A[i]));
		if (q.dot(A[l]) > q.dot(A[i])) q = q * -1;
		F f{ q, i, j, k };
		E(a, b).ins(k);
		E(a, c).ins(j);
		E(b, c).ins(i);
		FS.push_back(f);
	};
	for (int i = 0; i < 4; i ++)
		for (int j = i + 1; j < 4; j ++)
			for (int k = j + 1; k < 4; k ++)
				mf(i, j, k, 6 - i - j - k);

	for (int i = 4; i < (int)(A).size(); i ++) {
		for (int j = 0; j < (int)(FS).size(); j ++) {
			F f = FS[j];
			if (f.q.dot(A[i]) > f.q.dot(A[f.a])) {
				E(a, b).rem(f.c);
				E(a, c).rem(f.b);
				E(b, c).rem(f.a);
				swap(FS[j --], FS.back());
				FS.pop_back();
			}
		}
		int nw = (int)(FS).size();
		for (int j = 0; j < nw; j ++) {
			F f = FS[j];
#define C(a, b, c)                                     \
	if (E(a, b).cnt() != 2) mf(f.a, f.b, i, f.c);
			C(a, b, c);
			C(a, c, b);
			C(b, c, a);
		}
	}
	for (F &it : FS)
		if ((A[it.b] - A[it.a])
		        .cross(A[it.c] - A[it.a])
		        .dot(it.q) <= 0)
			swap(it.c, it.b);
	return FS;
};

#undef E
#undef C
