/**
 * @title 多边形直线裁剪与重心
 * @brief 按直线裁剪凸多边形，并计算简单多边形的面积重心。
 * @note cut 输入凸多边形；center 输入简单多边形，均按边界顺序给出。cut 保留有向直线 a→b 左侧，含边界；凸多边形输入时输出单个凸多边形。center
 * 返回面积重心，零面积返回 nullopt。
 * @usage `polygon_cut(p,a,b)` 保留 a→b 左边；`polygon_center(p)` 为面积重心，非顶点均值。
 * @time O(n)
 * @depends contest/Base.h, geometry/Point.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

vector<Point<double>>
polygon_cut(const vector<Point<double>> &p,
            Point<double> a, Point<double> b,
            double eps = 1e-9) {
	vector<Point<double>> out;
	int n = p.size();
	for (int i = 0; i < n; i ++) {
		auto u = p[i], v = p[(i + 1) % n];
		double x = (b - a).cross(u - a),
		       y = (b - a).cross(v - a);
		if (x >= -eps) out.push_back(u);
		if ((x > eps && y < -eps) ||
		    (x < -eps && y > eps))
			out.push_back(u + (v - u) * (x / (x - y)));
	}
	return out;
}
optional<Point<double>>
polygon_center(const vector<Point<double>> &p,
               double eps = 1e-9) {
	Point<double> ans;
	double area = 0;
	int n = p.size();
	for (int i = 0; i < n; i ++) {
		auto a = p[i], b = p[(i + 1) % n];
		double w = a.cross(b);
		area += w;
		ans = ans + (a + b) * w;
	}
	if (abs(area) <= eps) return nullopt;
	return ans / (3 * area);
}
