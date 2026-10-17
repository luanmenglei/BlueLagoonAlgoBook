/**
 * @title 固定容量内存池（Arena）
 * @brief
 * 一次分配堆内存，按对齐要求顺序创建轻量结点，整池重置。
 * @note 不扩容、不单独释放；reset
 * 使所有旧指针失效。仅支持可平凡析构的 T，
 * alignof(T)<=alignof(max_align_t)；容量不足抛
 * bad_alloc。对象不可复制或移动。
 * @usage Arena a(1<<20); auto
 * p=a.make<pair<int,int>>(3,4); a.reset();
 * @time 分配 O(1) 加构造开销；reset
 * O(1)；空间为预设容量
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; aligned bump
 * allocation
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"

struct Arena {
	void *buf;
	size_t cap,
	    pos = 0; // 字节容量、已消耗字节（含对齐填充）。
	explicit Arena(size_t bytes)
	    : buf(::operator new(bytes)), cap(bytes) {}
	~Arena() { ::operator delete(buf); }
	Arena(const Arena &) = delete;
	Arena &operator=(const Arena &) = delete;
	void reset() { pos = 0; }
	template <class T, class... Args>
	T *make(Args &&...args) {
		static_assert(is_trivially_destructible_v<T>);
		static_assert(alignof(T) <=
		              alignof(max_align_t));
		void *p = static_cast<char *>(buf) + pos;
		size_t space = cap - pos;
		if (!align(alignof(T), sizeof(T), p, space))
			throw bad_alloc();
		T *out = ::new (p) T(forward<Args>(args)...);
		pos = cap - space + sizeof(T);
		return out;
	}
};
