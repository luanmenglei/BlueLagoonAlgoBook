/**
 * @title 多边形有向面积
 * @brief 使用叉积求多边形两倍有向面积。
 * @note 返回2倍有向面积，CCW为正；输入非空。i64 乘法可能溢出，可将点类型改 i128。
 * @time O(n)
 * @depends contest/Base.h, geometry/Point.h
 * @author Ulf Lundstrom; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/PolygonArea.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

template <class T> T polygonArea2(vector<Point<T>> &v) {
	T a = v.back().cross(v[0]);
	for (int i = 0; i < (int)(v).size() - 1; i ++)
		a += v[i].cross(v[i + 1]);
	return a;
}
