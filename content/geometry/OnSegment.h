/**
 * @title 点在线段上
 * @brief 判断一个点是否在线段上，包含端点。
 * @note 整数点精确判断；浮点点在线段上用 segDist<=eps。
 * @time O(1)
 * @depends contest/Base.h, geometry/Point.h
 * @author Victor Lecomte, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/OnSegment.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class P> bool onSegment(P s, P e, P p) {
	return p.cross(s, e) == 0 &&
	       (s - p).dot(e - p) <= 0;
}
