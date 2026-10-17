/**
 * @title 数组异或（展开循环／AVX2）
 * @brief 对 n 个 u64 执行 a[i]^=b[i]；自动选择 AVX2
 * 或普通展开循环。
 * @note 两段内存须不相交或完全相同；允许非 32
 * 字节对齐，尾部逐项处理。 仅 GNU x86 编译 AVX2
 * 分支；xor_words 检测 CPU 后调用，不要直接调用 avx2
 * 内部函数。 编译整个程序时不要加
 * -mavx2/-march=native，否则回退路径也可能含专用指令。
 * @usage xor_words(a.data(),b.data(),a.size());
 * @time O(n)，每次 AVX2 操作处理 4 个
 * u64；不保证小数组加速
 * @depends contest/Base.h
 * @author BlueLagoon
 * @source BlueLagoon implementation; GCC x86 function
 * targets and intrinsics
 * @license CC0-1.0
 */
#pragma once
#include "../contest/Base.h"
#if !defined(BL_PORTABLE) && defined(__GNUC__) &&      \
    (defined(__i386__) || defined(__x86_64__))
#include <immintrin.h>
#endif

inline void xor_words_scalar(u64 *a, const u64 *b,
                             size_t n) {
	size_t i = 0;
	for (; n - i >= 4; i += 4) {
		a[i] ^= b[i];
		a[i + 1] ^= b[i + 1];
		a[i + 2] ^= b[i + 2];
		a[i + 3] ^= b[i + 3];
	}
	for (; i < n; i ++)
		a[i] ^= b[i];
}
#if !defined(BL_PORTABLE) && defined(__GNUC__) &&      \
    (defined(__i386__) || defined(__x86_64__))
__attribute__((target("avx2"))) inline void
xor_words_avx2(u64 *a, const u64 *b, size_t n) {
	size_t i = 0;
	for (; n - i >= 4; i += 4) {
		auto x = _mm256_loadu_si256(
		    reinterpret_cast<const __m256i *>(a + i));
		auto y = _mm256_loadu_si256(
		    reinterpret_cast<const __m256i *>(b + i));
		_mm256_storeu_si256(
		    reinterpret_cast<__m256i *>(a + i),
		    _mm256_xor_si256(x, y));
	}
	for (; i < n; i ++)
		a[i] ^= b[i];
}
#endif
inline void xor_words(u64 *a, const u64 *b, size_t n) {
#if !defined(BL_PORTABLE) && defined(__GNUC__) &&      \
    (defined(__i386__) || defined(__x86_64__))
	if (__builtin_cpu_supports("avx2")) {
		xor_words_avx2(a, b, n);
		return;
	}
#endif
	xor_words_scalar(a, b, n);
}
