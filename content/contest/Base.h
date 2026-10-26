/**
 * @title 公共前置声明
 * @brief 提供整型别名、标准库和模板共用的基础声明。
 * @note 所有片段默认依赖本段；GNU 扩展 __int128、PBDS 均需 GCC 兼容环境。
 * @time O(1)
 * @depends -
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once

#include<bits/stdc++.h>
#include<cassert>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
