/**
 * @title 曼哈顿距离最小生成树候选边
 * @brief 生成线性数量的候选边，配合 Kruskal 求点集曼哈顿距离 MST。
 * @note 0-based 整数点，可重复；返回 (距离,u,v)，至多4n条。对结果跑 Kruskal 得完整图 Manhattan MST。坐标变换、距离、比较中间值不得溢出
 * i64。
 * @usage `auto ed=manhattanMST(points);`；ed 元素为 (距离,u,v)，再按距离排序跑 Kruskal。
 * @time O(n log n)
 * @depends contest/Base.h, geometry/Point.h
 * @author chilli, Takanori MAEHARA; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ManhattanMST.h
 * @license CC0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../geometry/Point.h"

typedef Point<i64> P;
vector<tuple<i64, int, int>>
manhattanMST(vector<P> ps) {
	vector<int> id(int(ps.size()));
	iota(id.begin(), id.end(), 0);
	vector<tuple<i64, int, int>> edges;
	for (int k = 0; k < 4; k ++) {
		sort(id.begin(), id.end(), [&](int i, int j) {
			return (ps[i] - ps[j]).x <
			       (ps[j] - ps[i]).y;
		});
		map<i64, int> sweep;
		for (int i : id) {
			for (auto it = sweep.lower_bound(-ps[i].y);
			     it != sweep.end(); sweep.erase(it ++)) {
				int j = it->second;
				P d = ps[i] - ps[j];
				if (d.y > d.x) break;
				edges.push_back({ d.y + d.x, i, j });
			}
			sweep[-ps[i].y] = i;
		}
		for (P &p : ps)
			if (k & 1)
				p.x = -p.x;
			else
				swap(p.x, p.y);
	}
	return edges;
}
