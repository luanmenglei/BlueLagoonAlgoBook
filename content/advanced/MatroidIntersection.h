/**
 * @title 无权拟阵交
 * @brief 在两个无权拟阵中求元素个数最多的公共独立集。
 * @note 地集 0..n-1；oracle 接收元素下标列表，返回是否独立。两个 oracle 必须确实对应拟阵；输出最大公共独立集。不支持带权；oracle 不得修改外部状态。
 * @usage 均匀拟阵例：`auto a=[](auto s){return s.size()<=2;};`；`matroid_intersection(5,a,a)`
 * 返回2个元素。实题替换独立性 oracle。
 * @time 每次增广 O(n²) 次 oracle，至多 n 次
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class A, class B>
vector<int> matroid_intersection(int n, A a, B b) {
	vector<int> in(n);
	for (;;) {
		vector<int> cur;
		for (int i = 0; i < n; i ++)
			if (in[i]) cur.push_back(i);
		vector<int> pre(n, -1);
		queue<int> q;
		int end = -1;
		auto trial = [&](int add, int del) {
			vector<int> s;
			for (int x : cur)
				if (x != del) s.push_back(x);
			if (add >= 0) s.push_back(add);
			return s;
		};
		for (int i = 0; i < n; i ++)
			if (!in[i] && a(trial(i, -1)))
				pre[i] = -2, q.push(i);
		while (!q.empty() && end < 0) {
			int x = q.front();
			q.pop();
			if (!in[x]) {
				if (b(trial(x, -1))) {
					end = x;
					break;
				}
				for (int y : cur)
					if (pre[y] == -1 && b(trial(x, y)))
						pre[y] = x, q.push(y);
			} else
				for (int y = 0; y < n; y ++)
					if (!in[y] && pre[y] == -1 &&
					    a(trial(y, x)))
						pre[y] = x, q.push(y);
		}
		if (end < 0) return cur;
		for (int x = end; x >= 0; x = pre[x])
			in[x] ^= 1;
	}
}
