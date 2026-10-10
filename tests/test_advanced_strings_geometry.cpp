#include "Test.h"
#include "../content/advanced/Runs.h"
#include "../content/advanced/SuffixTree.h"
#include "../content/advanced/Delaunay.h"
#include "../content/geometry/ConvexHull.h"

vector<array<int, 3>> brute_runs(const string &s) {
	int n = s.size();
	vector<array<int, 3>> out;
	for (int l = 0; l < n; l ++)
		for (int r = l + 2; r <= n; r ++) {
			int p = 1;
			for (; p < r - l; p ++) {
				bool ok = true;
				for (int i = l + p; i < r; i ++)
					if (s[i] != s[i - p]) ok = false;
				if (ok) break;
			}
			if (r - l < 2 * p) continue;
			if (l > 0 && s[l - 1] == s[l - 1 + p])
				continue;
			if (r < n && s[r] == s[r - p]) continue;
			out.push_back({ p, l, r });
		}
	sort(out.begin(), out.end());
	return out;
}
i128 circle(Point<i64> a, Point<i64> b, Point<i64> c,
            Point<i64> p) {
	i128 x = a.x - p.x, y = a.y - p.y, u = b.x - p.x,
	     v = b.y - p.y, s = c.x - p.x, t = c.y - p.y;
	return (x * x + y * y) * (u * t - v * s) -
	       (u * u + v * v) * (x * t - y * s) +
	       (s * s + t * t) * (x * v - y * u);
}
int main() {
	for (int n = 0; n <= 11; n ++)
		for (int mask = 0; mask < (1 << n); mask ++) {
			string s;
			for (int i = 0; i < n; i ++)
				s += char('a' + (mask >> i & 1));
			CHECK(enumerate_runs(s) == brute_runs(s));
		}
	for (int it = 0; it < 1200; it ++) {
		string s;
		for (int n = ri(1, 35); n --;)
			s += char('a' + ri(0, 3));
		CHECK(enumerate_runs(s) == brute_runs(s));
	}
	for (int it = 0; it < 1500; it ++) {
		string a, b;
		for (int n = ri(0, 20); n --;)
			a += char('a' + ri(0, 3));
		for (int n = ri(0, 20); n --;)
			b += char('a' + ri(0, 3));
		int best = 0;
		for (int i = 0; i < (int)a.size(); i ++)
			for (int j = 0; j < (int)b.size(); j ++) {
				int k = 0;
				while (i + k < (int)a.size() &&
				       j + k < (int)b.size() &&
				       a[i + k] == b[j + k])
					k ++;
				best = max(best, k);
			}
		auto got = SuffixTree::LCS(a, b);
		CHECK(got.first == best);
		SuffixTree st(a + "{");
		set<string> leaves;
		function<void(int, string)> walk =
		    [&](int u, string path) {
			    if (u)
				    path += st.a.substr(
				        st.l[u], st.r[u] - st.l[u]);
			    bool leaf = true;
			    for (int v : st.t[u])
				    if (v != -1)
					    leaf = false, walk(v, path);
			    if (leaf) leaves.insert(path);
		    };
		walk(0, "");
		set<string> expected;
		for (int i = 0; i < (int)st.a.size(); i ++)
			expected.insert(st.a.substr(i));
		CHECK(leaves == expected);
	}
	for (int it = 0; it < 700; it ++) {
		int n = ri(0, 30);
		set<pair<i64, i64>> unique;
		while ((int)unique.size() < n)
			unique.emplace(ri(-20, 20), ri(-20, 20));
		vector<Point<i64>> points;
		for (auto [x, y] : unique)
			points.emplace_back(x, y);
		if (it % 25 == 0) {
			points.clear();
			for (int x = -10; x <= 10; x ++)
				points.emplace_back(x, 3 * x + 5);
		}
		if (it == 1)
			points = { Point<i64>(-1, -1),
				       Point<i64>(1, -1),
				       Point<i64>(1, 1),
				       Point<i64>(-1, 1) };
		if (it % 20 == 0)
			for (auto &p : points)
				p.x *= 20000000LL, p.y *= 20000000LL;
		auto triangles = delaunay::triangulate(points);
		CHECK(triangles.size() % 3 == 0);
		i128 area = 0;
		map<pair<pair<i64, i64>, pair<i64, i64>>, int>
		    edges;
		for (int i = 0; i < (int)triangles.size();
		     i += 3) {
			auto a = triangles[i], b = triangles[i + 1],
			     c = triangles[i + 2];
			CHECK(a.cross(b, c) > 0);
			area += a.cross(b, c);
			for (auto p : points)
				CHECK(circle(a, b, c, p) <= 0);
			for (auto [u, v] :
			     vector<pair<Point<i64>, Point<i64>>>{
			         { a, b }, { b, c }, { c, a } }) {
				pair<i64, i64> x{ u.x, u.y },
				    y{ v.x, v.y };
				if (x > y) swap(x, y);
				CHECK((++edges[{ x, y }] <= 2));
			}
		}
		auto hull = convexHull(points);
		i128 expected = 0;
		for (int i = 0; i < (int)hull.size(); i ++)
			expected += hull[i].cross(
			    hull[(i + 1) % hull.size()]);
		CHECK(area == expected);
	}
	done("advanced strings and geometry");
}
