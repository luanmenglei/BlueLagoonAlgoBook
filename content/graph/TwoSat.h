/**
 * @title 布尔约束可行解（2-SAT）
 * @brief 求解由两文字析取组成的布尔约束，返回一组可行赋值。
 * @note 变量 0..n-1；文字 2*x+v 表示 x=v，取反用 ^1。either 添加析取，imply 同时添加逆否命题；求解一次。
 * @time O(n+m)
 * @depends contest/Base.h, graph/SCC.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"
#include "../graph/SCC.h"

struct TwoSat {
	int n;
	SCC s;
	vector<bool> ans;
	TwoSat(int n) : n(n), s(2 * n), ans(n) {}
	// Literal 2*x+v means variable x takes value v.
	void imply(int a, int b) {
		s.g[a].push_back(b);
		s.g[b ^ 1].push_back(a ^ 1);
	}
	void either(int a, int b) { imply(a ^ 1, b); }
	void force(int a) { either(a, a); }
	bool solve() {
		s.solve();
		for (int i = 0; i < n; i ++) {
			if (s.comp[2 * i] == s.comp[2 * i + 1])
				return false;
			ans[i] = s.comp[2 * i + 1] < s.comp[2 * i];
		}
		return true;
	}
};
