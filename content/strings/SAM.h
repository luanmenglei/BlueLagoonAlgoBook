/**
 * @title 后缀自动机／子串出现次数
 * @brief 构建表示所有子串的状态图，统计各状态子串的出现次数。
 * @note 小写 a..z；逐字符 add。count 在构造完成后仅调用一次；状态代表长度 (len[link],len] 的子串，共享 endpos 与出现次数。
 * @time 构造／计数 O(26n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct SAM {
	struct Node {
		array<int, 26> ch;
		int len = 0, link = -1;
		i64 cnt = 0;
		Node() { ch.fill(-1); }
	};
	vector<Node> tr = { Node() };
	int last = 0;
	void add(char c) {
		int x = c - 'a', u = tr.size(), p = last;
		tr.push_back(Node());
		tr[u].len = tr[last].len + 1;
		tr[u].cnt = 1;
		while (p != -1 && tr[p].ch[x] == -1)
			tr[p].ch[x] = u, p = tr[p].link;
		if (p == -1)
			tr[u].link = 0;
		else {
			int q = tr[p].ch[x];
			if (tr[q].len == tr[p].len + 1)
				tr[u].link = q;
			else {
				int v = tr.size();
				tr.push_back(tr[q]);
				tr[v].len = tr[p].len + 1;
				tr[v].cnt = 0;
				while (p != -1 && tr[p].ch[x] == q)
					tr[p].ch[x] = v, p = tr[p].link;
				tr[q].link = tr[u].link = v;
			}
		}
		last = u;
	}
	void count() {
		int n = tr[last].len;
		vector<int> cnt(n + 1), a(tr.size());
		for (auto u : tr)
			cnt[u.len] ++;
		for (int i = 1; i <= n; i ++)
			cnt[i] += cnt[i - 1];
		for (int i = 0; i < (int)tr.size(); i ++)
			a[--cnt[tr[i].len]] = i;
		for (int i = (int)a.size() - 1; i > 0; i --)
			tr[tr[a[i]].link].cnt += tr[a[i]].cnt;
	}
};
