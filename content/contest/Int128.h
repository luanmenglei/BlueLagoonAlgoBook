/**
 * @title 128 位读写与有符号整除
 * @brief 读写 128 位整数，并计算带符号整数的向下／向上整除。
 * @note 读入值必须在 i128 范围内。floor_div／ceil_div 要求 b 非零且商可表示。
 * @time O(位数)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

istream &operator>>(istream &in, i128 &x) {
	string s;
	in >> s;
	x = 0;
	bool neg = !s.empty() && s[0] == '-';
	for (int i = neg || (!s.empty() && s[0] == '+');
	     i < (int)s.size(); i ++)
		x = x * 10 - (s[i] - '0');
	if (!neg) x = -x;
	return in;
}
ostream &operator<<(ostream &out, i128 x) {
	if (!x) return out << '0';
	u128 y = x < 0 ? -u128(x) : u128(x);
	string s;
	while (y)
		s += char('0' + y % 10), y /= 10;
	if (x < 0) out << '-';
	reverse(s.begin(), s.end());
	return out << s;
}
i64 floor_div(i64 a, i64 b) {
	assert(b && !(a == LLONG_MIN && b == -1));
	i64 q = a / b, r = a % b;
	return q - (r && ((r < 0) != (b < 0)));
}
i64 ceil_div(i64 a, i64 b) {
	assert(b && !(a == LLONG_MIN && b == -1));
	i64 q = a / b, r = a % b;
	return q + (r && ((r < 0) == (b < 0)));
}
