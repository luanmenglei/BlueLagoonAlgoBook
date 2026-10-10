/**
 * @title 莫队／静态区间查询
 * @brief 重排离线区间查询，通过两端加入／删除元素维护答案。
 * @note 查询半开区间 [l,r)，id 必须为 0..q-1 的排列。初始统计为空；add/del 接收下标，read(id) 保存当前答案。仅适用于两端插删均可维护的状态。
 * @usage `qs={{0,3,0},{1,4,1}};`；`mo(n,qs,add,del,read);`；三个回调分别加入下标、删除下标、保存答案。
 * @time O((n+q) sqrt(n)) 次状态操作
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct MoQuery {
	int l, r, id;
};
template <class Add, class Del, class Read>
void mo(int n, vector<MoQuery> qs, Add add, Del del,
        Read read) {
	int b = max(1, (int)sqrt(max(n, 1)));
	sort(qs.begin(), qs.end(), [&](auto a, auto c) {
		int x = a.l / b, y = c.l / b;
		if (x != y) return x < y;
		return x & 1 ? a.r > c.r : a.r < c.r;
	});
	int l = 0, r = 0;
	for (auto q : qs) {
		assert(0 <= q.l && q.l <= q.r && q.r <= n);
		while (l > q.l)
			add(--l);
		while (r < q.r)
			add(r ++);
		while (l < q.l)
			del(l ++);
		while (r > q.r)
			del(--r);
		read(q.id);
	}
}
