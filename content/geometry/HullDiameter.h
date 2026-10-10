/**
 * @title 凸包直径／最远点对（旋转卡壳）
 * @brief 求凸多边形中欧氏距离最远的一对顶点。
 * @note 输入非空CCW凸包，无相邻重复与中间共线点；返回最远点对。平方距离 i64 不得溢出。
 * @time O(n)
 * @depends contest/Base.h, geometry/Point.h
 * @author Oleksandr Bacherikov, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/HullDiameter.h
 * @license Boost Software License
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using IP = Point<i64>;
array<IP, 2> hullDiameter(vector<IP> S) {
	int n = (int)(S).size(), j = n < 2 ? 0 : 1;
	pair<i64, array<IP, 2>> res({ 0, { S[0], S[0] } });
	for (int i = 0; i < j; i ++)
		for (;; j = (j + 1) % n) {
			res = max(res, { (S[i] - S[j]).dist2(),
			                 { S[i], S[j] } });
			if ((S[(j + 1) % n] - S[j])
			        .cross(S[i + 1] - S[i]) >= 0)
				break;
		}
	return res.second;
}
