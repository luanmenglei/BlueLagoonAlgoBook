/**
 * @title 最长递增子序列长度／恢复（LIS）
 * @brief 恢复一条最长严格递增或非降子序列的原下标。
 * @note 返回一条子序列的原下标；strict=true 严格递增，false 非降。空输入返回空。
 * @usage `lis(vector<int>{3,1,2})` 返回下标 {1,2}；非降版本另传 false。
 * @time O(n log n)
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; standard algorithm
 * @license CC0-1.0
 * @status See tests/report.json; upstream claims are separate.
 */
#pragma once
#include "../contest/Base.h"

template <class T>
vector<int> lis(const vector<T> &a,
                bool strict = true) {
	vector<T> val;
	vector<int> last, pre(a.size(), -1);
	for (int i = 0; i < (int)a.size(); i ++) {
		int k =
		    (strict ? lower_bound(val.begin(),
		                          val.end(), a[i])
		            : upper_bound(val.begin(),
		                          val.end(), a[i])) -
		    val.begin();
		if (k) pre[i] = last[k - 1];
		if (k == (int)val.size())
			val.push_back(a[i]), last.push_back(i);
		else
			val[k] = a[i], last[k] = i;
	}
	vector<int> ans;
	if (!last.empty())
		for (int i = last.back(); i != -1; i = pre[i])
			ans.push_back(i);
	reverse(ans.begin(), ans.end());
	return ans;
}
