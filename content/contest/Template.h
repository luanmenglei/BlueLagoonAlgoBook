/**
 * @title 单测／多测入口
 * @brief 提供 solve 与 main 的单测／多测程序入口。
 * @note 多测入口；单测改为直接 solve()。数组大小和每组清理范围按题目修改。
 * @time 按题目
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

const int N = 3e5 + 10;
int n, a[N];

void solve() {
	cin >> n;
	for (int i = 1; i <= n; i ++)
		cin >> a[i];
	// Reset the state used by this test case here.
}

int main() {
	ios::sync_with_stdio(false), cin.tie(0),
	    cout.tie(0);
	int tt;
	cin >> tt;
	while (tt --)
		solve();
	return 0;
}
