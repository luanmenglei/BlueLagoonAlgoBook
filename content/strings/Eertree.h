/**
 * @title 回文树／不同回文与出现次数
 * @brief 在线维护不同回文子串，统计各回文的出现次数。
 * @note 小写 a..z；节点0是奇根，1是偶根，其余对应不同回文。add 返回当前最长回文后缀节点；count 完成后调用一次。
 * @time O(26n) 空间，线性构造
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct Eertree {
	struct Node {
		array<int, 26> ch{};
		int len = 0, link = 0;
		i64 cnt = 0;
	};
	vector<Node> tr = vector<Node>(2);
	vector<int> s = { -1 };
	int last = 1;
	Eertree() { tr[0].len = -1; }
	int get(int u) {
		int p = s.size() - 1;
		while (s[p - tr[u].len - 1] != s[p])
			u = tr[u].link;
		return u;
	}
	int add(char c) {
		int x = c - 'a';
		s.push_back(x);
		int p = get(last);
		if (!tr[p].ch[x]) {
			int u = tr.size();
			tr.push_back(Node());
			tr[u].len = tr[p].len + 2;
			tr[u].link =
			    tr[u].len == 1
			        ? 1
			        : tr[get(tr[p].link)].ch[x];
			tr[p].ch[x] = u;
		}
		last = tr[p].ch[x];
		tr[last].cnt ++;
		return last;
	}
	void count() {
		for (int i = (int)tr.size() - 1; i >= 2; i --)
			tr[tr[i].link].cnt += tr[i].cnt;
	}
};
