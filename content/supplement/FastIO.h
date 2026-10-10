/**
 * @title fread 快读快写
 * @brief 用 fread／fwrite 缓冲读写整数，支持 128 位整数及 EOF 检测。
 * @note 批处理 IO；不要与 cin/scanf 混用。read 整数要求输入在目标类型范围内，EOF 返回 false；支持有符号最小值和
 * i128。对象含64KiB缓冲；输出析构时 flush，交互题显式 flush。
 * @usage `FastInput in; i64 x;`；`if(in.read(x)) out.write(x);`，out 为 FastOutput；EOF 时 read 返回
 * false。
 * @time O(字符数)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

struct FastInput {
	static const int B = 1 << 16;
	FILE *f;
	char buf[B];
	int p = 0, n = 0;
	FastInput(FILE *f = stdin) : f(f) {}
	int get() {
		if (p == n) n = fread(buf, 1, B, f), p = 0;
		return n ? (unsigned char)buf[p ++] : EOF;
	}
	template <class T> bool read(T &x) {
		int c;
		do {
			c = get();
			if (c == EOF) return false;
		} while (c <= 32);
		bool neg = c == '-';
		if (neg || c == '+') c = get();
		u128 v = 0;
		assert(c >= '0' && c <= '9');
		while (c >= '0' && c <= '9')
			v = v * 10 + c - '0', c = get();
		x = neg ? T(-i128(v - 1)) - 1 : T(v);
		return true;
	}
};
struct FastOutput {
	static const int B = 1 << 16;
	FILE *f;
	char buf[B];
	int n = 0;
	FastOutput(FILE *f = stdout) : f(f) {}
	~FastOutput() { flush(); }
	void flush() {
		if (n) fwrite(buf, 1, n, f), n = 0;
	}
	void put(char c) {
		if (n == B) flush();
		buf[n ++] = c;
	}
	template <class T>
	void write(T x, char end = '\n') {
		u128 v;
		if (x < 0)
			put('-'), v = u128(-(i128(x) + 1)) + 1;
		else
			v = x;
		char s[50];
		int k = 0;
		do {
			s[k ++] = char('0' + v % 10);
			v /= 10;
		} while (v);
		while (k)
			put(s[--k]);
		if (end) put(end);
	}
};
