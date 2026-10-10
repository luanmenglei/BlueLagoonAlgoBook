/**
 * @title 最小覆盖圆
 * @brief 求覆盖全部给定点、半径最小的圆。
 * @note DP；返回圆心和半径，输入非空。随机增量，允许共线与重复点；浮点误差按尺度调整。
 * @time 期望 O(n)
 * @depends contest/Base.h, geometry/circumcircle.h
 * @author Andrew He, chilli; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/MinimumEnclosingCircle.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/circumcircle.h"

pair<DP, double> mec(vector<DP> ps) {
	shuffle((ps).begin(), (ps).end(), mt19937(time(0)));
	DP o = ps[0];
	double r = 0, EPS = 1 + 1e-8;
	for (int i = 0; i < (int)(ps).size(); i ++)
		if ((o - ps[i]).dist() > r * EPS) {
			o = ps[i], r = 0;
			for (int j = 0; j < i; j ++)
				if ((o - ps[j]).dist() > r * EPS) {
					o = (ps[i] + ps[j]) / 2;
					r = (o - ps[i]).dist();
					for (int k = 0; k < j; k ++)
						if ((o - ps[k]).dist() >
						    r * EPS) {
							o = ccCenter(ps[i], ps[j],
							             ps[k]);
							r = (o - ps[i]).dist();
						}
				}
		}
	return { o, r };
}
