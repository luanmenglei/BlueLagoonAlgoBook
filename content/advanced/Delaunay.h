/**
 * @title Delaunay 三角剖分
 * @brief 对二维整数点集求 Delaunay 三角剖分，输出逆时针三角形。
 * @note namespace delaunay，triangulate(vector<Point<i64>>)
 * 返回按每3点分组的逆时针三角形。点不可重复，坐标绝对值<=10^9；共线返回空，共圆选择任意合法对角线。谓词使用 i128；结果为点坐标，不是点编号。工作区单线程。
 * @usage 三点 (0,0),(2,0),(0,2) 调用 `delaunay::triangulate(p)` 返回一个逆时针三角形，共3个点。
 * @time O(n log n) 时间，O(n) 空间
 * @depends contest/Base.h, geometry/Point.h
 * @author Philippe Legault; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/FastDelaunay.h
 * @license MIT
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

namespace delaunay {

typedef Point<i64> P;
typedef struct Quad *Q;
typedef __int128_t
    lll; // (can be i64 if coords are < 2e4)
P arb(LLONG_MAX,
      LLONG_MAX); // not equal to any other point

struct Quad {
	Q rot, o;
	P p = arb;
	bool mark;
	P &F() { return r()->p; }
	Q &r() { return rot->rot; }
	Q prev() { return rot->o->rot; }
	Q next() { return r()->prev(); }
};
Q H = nullptr;
vector<unique_ptr<Quad>> pool;

bool circ(P p, P a, P b,
          P c) { // is p in the circumcircle?
	lll p2 = p.dist2(), A = a.dist2() - p2,
	    B = b.dist2() - p2, C = c.dist2() - p2;
	return p.cross(a, b) * C + p.cross(b, c) * A +
	           p.cross(c, a) * B >
	       0;
}
Q makeEdge(P orig, P dest) {
	Q r = H;
	if (!r) {
		Q v[4];
		for (int i = 0; i < 4; i ++) {
			pool.emplace_back(new Quad{});
			v[i] = pool.back().get();
		}
		for (int i = 0; i < 4; i ++)
			v[i]->rot = v[(i + 1) % 4];
		r = v[0];
	}
	H = r->o;
	r->r()->r() = r;
	for (int i = 0; i < 4; i ++)
		r = r->rot, r->mark = false, r->p = arb,
		r->o = i & 1 ? r : r->r();
	r->p = orig;
	r->F() = dest;
	return r;
}
void splice(Q a, Q b) {
	swap(a->o->rot->o, b->o->rot->o);
	swap(a->o, b->o);
}
Q connect(Q a, Q b) {
	Q q = makeEdge(a->F(), b->p);
	splice(q, a->next());
	splice(q->r(), b);
	return q;
}

pair<Q, Q> rec(const vector<P> &s) {
	if (int(s.size()) <= 3) {
		Q a = makeEdge(s[0], s[1]),
		  b = makeEdge(s[1], s.back());
		if (int(s.size()) == 2) return { a, a->r() };
		splice(a->r(), b);
		auto side = s[0].cross(s[1], s[2]);
		Q c = side ? connect(b, a) : 0;
		return { side < 0 ? c->r() : a,
			     side < 0 ? c : b->r() };
	}

#define H(e) e->F(), e->p
#define valid(e) (e->F().cross(H(base)) > 0)
	Q A, B, ra, rb;
	int half = int(s.size()) / 2;
	tie(ra, A) =
	    rec(vector<P>(s.begin(), s.end() - half));
	tie(B, rb) = rec(vector<P>(
	    s.begin() + int(s.size()) - half, s.end()));
	while ((B->p.cross(H(A)) < 0 && (A = A->next())) ||
	       (A->p.cross(H(B)) > 0 && (B = B->r()->o)))
		;
	Q base = connect(B->r(), A);
	if (A->p == ra->p) ra = base->r();
	if (B->p == rb->p) rb = base;

#define DEL(e, init, dir)                              \
	Q e = init->dir;                                   \
	if (valid(e))                                      \
		while (circ(e->dir->F(), H(base), e->F())) {   \
			Q t = e->dir;                              \
			splice(e, e->prev());                      \
			splice(e->r(), e->r()->prev());            \
			e->o = H;                                  \
			H = e;                                     \
			e = t;                                     \
		}
	for (;;) {
		DEL(LC, base->r(), o);
		DEL(RC, base, prev());
		if (!valid(LC) && !valid(RC)) break;
		if (!valid(LC) ||
		    (valid(RC) && circ(H(RC), H(LC))))
			base = connect(RC, base->r());
		else
			base = connect(base->r(), LC->r());
	}
	return { ra, rb };
}

vector<P> triangulate(vector<P> pts) {
	pool.clear();
	H = nullptr;
	for (auto p : pts)
		assert(abs(p.x) <= 1000000000LL &&
		       abs(p.y) <= 1000000000LL);
	sort(pts.begin(), pts.end());
	assert(unique(pts.begin(), pts.end()) == pts.end());
	if (int(pts.size()) < 2) return {};
	Q e = rec(pts).first;
	vector<Q> q = { e };
	int qi = 0;
	while (e->o->F().cross(e->F(), e->p) < 0)
		e = e->o;
#define ADD                                            \
	{                                                  \
		Q c = e;                                       \
		do {                                           \
			c->mark = 1;                               \
			pts.push_back(c->p);                       \
			q.push_back(c->r());                       \
			c = c->next();                             \
		} while (c != e);                              \
	}
	ADD;
	pts.clear();
	while (qi < int(q.size()))
		if (!(e = q[qi ++])->mark) ADD;
	pool.clear();
	H = nullptr;
	return pts;
}

#undef H
#undef valid
#undef DEL
#undef ADD

} // namespace delaunay
