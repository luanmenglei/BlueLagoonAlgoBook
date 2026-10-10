/**
 * @title 有序集合排名／第 k 小（PBDS）
 * @brief 维护有序集合，查询元素排名和第 k 小元素。
 * @note 比较器必须严格弱序，不能用 less_equal 模拟重复值；不存在的第 k 项返回 end()。
 * @time O(log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T>
using OrderedSet =
    tree<T, null_type, less<T>, rb_tree_tag,
         tree_order_statistics_node_update>;
// s.order_of_key(x): number of keys < x.
// s.find_by_order(k): iterator to 0-based kth key.
// Duplicates: store pair<value, unique_id>.
