/**
 * @title 整数键开放寻址哈希表
 * @brief 用连续数组、线性探测和随机混洗存储 u64 到 T
 * 的映射。
 * @note 支持 find、下标插入和
 * clear，不支持单项删除。所有 u64 键均合法，无哨兵键。
 * 容量为 2 的幂，装载率不超过
 * 1/2；扩容会使指针和引用失效，clear
 * 后也不可使用旧引用。 T
 * 需可默认构造、复制赋值；不存在的键由 operator[] 插入
 * T{}。T 不可为 bool，可用 unsigned char 代替。非抗恶意自适应哈希。
 * @usage FlatHash<int> h(1000); h[42]++; if(auto
 * p=h.find(42)) cout<<*p;
 * @time 查找期望 O(1)，插入期望均摊 O(1)；最坏
 * O(容量)，clear O(容量)
 * @depends contest/Base.h, contest/RandomHash.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; linear probing
 * with SplitMix64
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"
#include "../contest/RandomHash.h"

template <class T> struct FlatHash {
	static_assert(!is_same_v<T, bool>);
	vector<u64> key;
	vector<T> val;
	vector<unsigned char> used;
	// 当前键数；used 区分空位，0 也可作为键。
	size_t cnt = 0;
	// 每个对象固定盐值，已有元素时不可修改。
	u64 seed = rng();
	explicit FlatHash(size_t expected = 0) {
		size_t c = 8;
		while (c / 2 < expected) {
			if (c > numeric_limits<size_t>::max() / 2)
				throw length_error("FlatHash capacity");
			c *= 2;
		}
		key.resize(c), val.resize(c), used.resize(c);
	}
	// 内部：找到 x 的位置或探测链上的第一个空位。
	size_t slot(u64 x) const {
		size_t mask = key.size() - 1;
		size_t p = Hash::mix(x + seed) & mask;
		while (used[p] && key[p] != x)
			p = (p + 1) & mask;
		return p;
	}
	T *find(u64 x) {
		size_t p = slot(x);
		return used[p] ? &val[p] : nullptr;
	}
	const T *find(u64 x) const {
		size_t p = slot(x);
		return used[p] ? &val[p] : nullptr;
	}
	// 内部：容量翻倍后重新散列，原有引用全部失效。
	void grow() {
		FlatHash h(key.size());
		h.seed = seed;
		for (size_t i = 0; i < key.size(); i ++)
			if (used[i]) h[key[i]] = val[i];
		*this = move(h);
	}
	T &operator[](u64 x) {
		size_t p = slot(x);
		if (used[p]) return val[p];
		if (cnt == key.size() / 2) grow(), p = slot(x);
		used[p] = 1, key[p] = x, val[p] = T{};
		cnt ++;
		return val[p];
	}
	size_t size() const { return cnt; }
	void clear() {
		fill(used.begin(), used.end(), 0);
		// 保留容量；T 持有的资源不会在这里全部释放。
		cnt = 0;
	}
};
