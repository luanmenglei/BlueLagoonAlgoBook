/**
 * @title 半平面交
 * @brief 求若干有向直线左侧半平面的交，并与指定边界框求交。
 * @note 保留每条有向直线的左侧；起终点不同。返回与 $[-\mathrm{bound},\mathrm{bound}]^2$ 的交，不自动判无界；bound
 * 必须覆盖题目全部合法解。空交返回空；低维交可能为空或含重复顶点，需另判面积与维度；eps 是叉积量纲，需按方向长度调整。
 * @time O(n log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

using DP = Point<double>;
struct HalfPlane {
	DP p, v;
	double angle;
	HalfPlane(DP a, DP b)
	    : p(a), v(b - a), angle(atan2(v.y, v.x)) {}
};
vector<DP> halfplane(vector<HalfPlane> a, double bound,
                     double eps = 1e-9) {
	assert(bound > 0);
	vector<DP> box = { DP(-bound, -bound),
		               DP(bound, -bound),
		               DP(bound, bound),
		               DP(-bound, bound) };
	for (int i = 0; i < 4; i ++)
		a.emplace_back(box[i], box[(i + 1) % 4]);
	auto out = [&](const HalfPlane &h, DP p) {
		return h.v.cross(p - h.p) < -eps;
	};
	auto inter = [](const HalfPlane &a,
	                const HalfPlane &b) {
		return a.p + a.v * (b.v.cross(a.p - b.p) /
		                    a.v.cross(b.v));
	};
	sort(a.begin(), a.end(),
	     [](const auto &x, const auto &y) {
		     return x.angle < y.angle;
	     });
	deque<HalfPlane> q;
	for (auto h : a) {
		while (q.size() > 1 &&
		       out(h, inter(q[q.size() - 2], q.back())))
			q.pop_back();
		while (q.size() > 1 &&
		       out(h, inter(q[0], q[1])))
			q.pop_front();
		if (!q.empty() &&
		    abs(h.v.cross(q.back().v)) < eps) {
			if (h.v.dot(q.back().v) < 0) return {};
			if (out(h, q.back().p))
				q.pop_back();
			else
				continue;
		}
		q.push_back(h);
	}
	while (q.size() > 2 &&
	       out(q.front(),
	           inter(q[q.size() - 2], q.back())))
		q.pop_back();
	while (q.size() > 2 &&
	       out(q.back(), inter(q[0], q[1])))
		q.pop_front();
	if (q.size() < 3) return {};
	vector<DP> ans;
	for (int i = 0; i < (int)q.size(); i ++)
		ans.push_back(
		    inter(q[i], q[(i + 1) % q.size()]));
	return ans;
}
