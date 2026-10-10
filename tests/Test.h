#pragma once
#include "../content/contest/Base.h"
mt19937 gen(712367);
int ri(int l,int r) { return uniform_int_distribution<int>(l,r)(gen); }
int checks = 0;
#define CHECK(x) do { checks ++; if (!(x)) { cerr << "FAIL " << __FILE__ << ":" << __LINE__ << " " << #x << "\n"; exit(1); } } while (0)
void done(const char *name) { cout << name << ": " << checks << " assertions; seed 712367\n"; }
