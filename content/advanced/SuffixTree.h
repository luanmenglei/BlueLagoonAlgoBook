/**
 * @title 后缀树／最长公共子串（Ukkonen）
 * @brief 构建压缩后缀树，并提供两串最长公共子串查询。
 * @note 输入 a..z 及两个保留符号 {、|（编码26/27）；完整后缀树须追加唯一终止符。边区间 [l,r)，根0，节点1是辅助节点。SuffixTree::LCS(s,t)
 * 的原串必须仅小写，返回(最长公共子串长度,在拼接串中的一个起点)。lcs 递归注意深链栈。
 * @usage `SuffixTree st("aba{");` 是完整树；`SuffixTree::LCS("abac","bac")` 的 first 为3。
 * @time O(28n) 时间／空间
 * @depends contest/Base.h
 * @author Unknown / KACTL; BlueLagoon adaptation
 * @source
 * https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/strings/SuffixTree.h
 * @license Not specified upstream; retain attribution
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct SuffixTree {
	enum { ALPHA = 28 }; // N ~ 2*maxlen+10
	int toi(char c) { return c - 'a'; }
	string a; // v = cur node, q = cur position
	vector<array<int, ALPHA>> t;
	vector<int> l, r, p, s;
	int v = 0, q = 0, m = 2;

	void ukkadd(int i, int c) {
	suff:
		if (r[v] <= q) {
			if (t[v][c] == -1) {
				t[v][c] = m;
				l[m] = i;
				p[m ++] = v;
				v = s[v];
				q = r[v];
				goto suff;
			}
			v = t[v][c];
			q = l[v];
		}
		if (q == -1 || c == toi(a[q]))
			q ++;
		else {
			l[m + 1] = i;
			p[m + 1] = m;
			l[m] = l[v];
			r[m] = q;
			p[m] = p[v];
			t[m][c] = m + 1;
			t[m][toi(a[q])] = v;
			l[v] = q;
			p[v] = m;
			t[p[m]][toi(a[l[m]])] = m;
			v = s[p[m]];
			q = l[m];
			while (q < r[m]) {
				v = t[v][toi(a[q])];
				q += r[v] - l[v];
			}
			if (q == r[m])
				s[m] = v;
			else
				s[m] = m + 2;
			q = r[v] - (q - r[m]);
			m += 2;
			goto suff;
		}
	}

	SuffixTree(string a)
	    : a(a), t(2 * a.size() + 5), l(t.size()),
	      r(t.size(), a.size()), p(t.size()),
	      s(t.size()) {
		for (char c : a)
			assert(0 <= toi(c) && toi(c) < ALPHA);

		for (auto &v : t)
			v.fill(-1);
		t[1].fill(0);
		s[0] = 1;
		l[0] = l[1] = -1;
		r[0] = r[1] = p[0] = p[1] = 0;
		for (int i = 0; i < int(a.size()); i ++)
			ukkadd(i, toi(a[i]));
	}

	// example: find longest common substring (uses
	// ALPHA = 28)
	pair<int, int> best{};
	int lcs(int node, int i1, int i2, int olen) {
		if (l[node] <= i1 && i1 < r[node]) return 1;
		if (l[node] <= i2 && i2 < r[node]) return 2;
		int mask = 0,
		    len = node ? olen + (r[node] - l[node]) : 0;
		for (int c = 0; c < ALPHA; c ++)
			if (t[node][c] != -1)
				mask |= lcs(t[node][c], i1, i2, len);
		if (mask == 3)
			best = max(best, { len, r[node] - len });
		return mask;
	}
	static pair<int, int> LCS(string s, string t) {
		SuffixTree st(s + (char)('z' + 1) + t +
		              (char)('z' + 2));
		st.lcs(0, int(s.size()),
		       int(s.size()) + 1 + int(t.size()), 0);
		return st.best;
	}
};
