/**
 * @title 多模式匹配（AC 自动机）
 * @brief 同时匹配多个模式串，统计每个模式串在文本中的出现次数。
 * @note 小写 a..z，非空模式；add 返回终点编号，重复模式共享终点。全部添加后 build 一次；count 返回各终点出现次数，可重复查询。
 * @time O(26*节点数+文本长度)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct AC {
	struct Node {
		array<int, 26> ch{};
		int fail = 0;
	};
	vector<Node> tr = { Node() };
	vector<int> order;
	int add(const string &s) {
		assert(!s.empty());
		int u = 0;
		for (char c : s) {
			int x = c - 'a';
			if (!tr[u].ch[x]) {
				int v = tr.size();
				tr[u].ch[x] = v;
				tr.push_back(Node());
			}
			u = tr[u].ch[x];
		}
		return u;
	}
	void build() {
		order.clear();
		queue<int> q;
		for (int v : tr[0].ch)
			if (v) q.push(v);
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			order.push_back(u);
			for (int c = 0; c < 26; c ++) {
				int &v = tr[u].ch[c];
				if (v)
					tr[v].fail = tr[tr[u].fail].ch[c],
					q.push(v);
				else
					v = tr[tr[u].fail].ch[c];
			}
		}
	}
	vector<i64> count(const string &s) const {
		vector<i64> cnt(tr.size());
		int u = 0;
		for (char c : s)
			u = tr[u].ch[c - 'a'], cnt[u] ++;
		for (int i = (int)order.size() - 1; i >= 0; i --)
			cnt[tr[order[i]].fail] += cnt[order[i]];
		return cnt;
	}
};
