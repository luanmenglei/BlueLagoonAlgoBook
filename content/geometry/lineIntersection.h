/**
 * @title 直线交点
 * @brief 求两条直线交点，并区分平行与重合。
 * @note 返回1唯一／0平行／-1重合；用 Point<double> 构造非整数交点。方向向量非零；精确零比较需按题目调整。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/lineIntersection.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (d == 0) // if parallel
		return { -(s1.cross(e1, s2) == 0), P(0, 0) };
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return { 1, (s1 * p + e1 * q) / d };
}
