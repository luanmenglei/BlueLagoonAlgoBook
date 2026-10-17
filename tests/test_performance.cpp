#include "Test.h"
#include "../content/performance/Barrett.h"
#include "../content/performance/Montgomery.h"
#include "../content/performance/FastDiv.h"
#include "../content/performance/BitTricks.h"
#include "../content/performance/FlatHash.h"
#include "../content/performance/Arena.h"
#include "../content/performance/Simd.h"

u64 rand64() {
	return (u64(gen()) << 32) | gen();
}
uint32_t refpow(uint32_t a, u64 e, uint32_t m) {
	u64 r = 1 % m, x = a % m;
	while (e) {
		if (e & 1) r = r * x % m;
		x = x * x % m;
		e >>= 1;
	}
	return uint32_t(r);
}
void modular(uint32_t m, int rounds) {
	Barrett32 b(m);
	vector<u64> xs{ 0,         1,
		            m - 1ULL,  m,
		            m + 1ULL,  ~0ULL,
		            ~0ULL - 1, u64(m) * m - 1 };
	for (int j = 0; j < 64; j ++) {
		xs.push_back(1ULL << j);
		xs.push_back((1ULL << j) - 1);
	}
	for (u64 x : xs)
		CHECK(b.reduce(x) == x % m);
	for (int i = 0; i < rounds; i ++) {
		u64 x = rand64();
		uint32_t a = gen(), c = gen();
		CHECK(b.reduce(x) == x % m);
		CHECK(b.mul(a, c) == u64(a) * c % m);
	}
	if (!(m & 1)) return;
	Montgomery32 z(m);
	vector<uint32_t> a{ 0, 1, m - 1, m, UINT32_MAX };
	for (int i = 0; i < rounds; i ++)
		a.push_back(gen());
	uint32_t product = z.encode(1), ref = 1 % m;
	for (uint32_t x : a) {
		uint32_t y = gen(), u = z.encode(x),
		         v = z.encode(y);
		CHECK(u < m && v < m);
		CHECK(u == (u128(x) << 32) % m);
		CHECK(z.decode(u) == x % m);
		CHECK(z.decode(z.mul(u, v)) == u64(x) * y % m);
		CHECK(z.decode(z.add(u, v)) ==
		      (u64(x) + y) % m);
		CHECK(z.decode(z.sub(u, v)) ==
		      (u64(x % m) + m - y % m) % m);
		product = z.mul(product, u);
		ref = u64(ref) * x % m;
		CHECK(z.decode(product) == ref);
		u64 t = rand64() % (u64(m) << 32);
		uint32_t r = z.reduce(t);
		CHECK(r < m && (u128(r) << 32) % m == t % m);
	}
	for (u64 t : { 0ULL, (u64(m) << 32) - 1 }) {
		uint32_t r = z.reduce(t);
		CHECK(r < m && (u128(r) << 32) % m == t % m);
	}
	for (u64 e : { 0ULL, 1ULL, 2ULL, 12345ULL, ~0ULL })
		CHECK(z.pow(UINT32_MAX, e) ==
		      refpow(UINT32_MAX, e, m));
}
int main() {
	rng.seed(712367);
	// Small exhaustive domains plus full-width random
	// values and awkward moduli.
	for (uint32_t m = 1; m <= 64; m ++) {
		Barrett32 b(m);
		for (uint32_t x = 0; x < m; x ++)
			for (uint32_t y = 0; y < m; y ++) {
				CHECK(b.mul(x, y) == u64(x) * y % m);
				if (m & 1) {
					Montgomery32 z(m);
					CHECK(
					    z.decode(z.mul(z.encode(x),
					                   z.encode(y))) ==
					    u64(x) * y % m);
				}
			}
	}
	for (uint32_t m :
	     { 1u, 2u, 3u, 4u, 7u, 65535u, 65536u,
	       998244353u, 1000000007u, 2147483647u,
	       2147483648u, 4294967291u, UINT32_MAX })
		modular(m, 2000);
	for (int i = 0; i < 300; i ++)
		modular(gen() | 1u, 20);
	for (int i = 0; i < 100000; i ++) {
		uint32_t y = gen() | 1u;
		u64 x = rand64() % (u64(y) << 32);
		auto [q, r] = divmod32(x, y);
		CHECK(q == x / y && r == x % y);
	}
	for (uint32_t y : { 1u, 2u, 4u, 65536u, 2147483648u,
	                    UINT32_MAX })
		for (u64 x :
		     { 0ULL, u64(y) - 1, (u64(y) << 32) - 1 }) {
			auto [q, r] = divmod32(x, y);
			CHECK(q == x / y && r == x % y);
		}
	CHECK(lsb_index(0) == -1 && msb_index(0) == -1);
	CHECK(lowbit(0) == 0 && !next_mask(0));
	CHECK(!next_mask(~0ULL) && !next_mask(1ULL << 63));
	CHECK(next_mask(1ULL << 62) ==
	      optional<u64>(1ULL << 63));
	vector<vector<u64>> masks(17);
	for (u64 x = 1; x < (1ULL << 16); x ++) {
		masks[__builtin_popcountll(x)].push_back(x);
		int l = 0, h = 0;
		while (!(x >> l & 1))
			l ++;
		for (u64 y = x; y >>= 1;)
			h ++;
		CHECK(lsb_index(x) == l && msb_index(x) == h);
		CHECK(lowbit(x) == (1ULL << l));
	}
	for (auto &v : masks)
		for (size_t i = 1; i < v.size(); i ++)
			CHECK(next_mask(v[i - 1]) ==
			      optional<u64>(v[i]));
	// Independent next_permutation oracle across the
	// entire 64-bit width.
	for (int it = 0; it < 1000; it ++) {
		u64 x = rand64(), y = 0;
		array<int, 64> a;
		for (int i = 0; i < 64; i ++)
			a[63 - i] = x >> i & 1;
		bool ok = next_permutation(a.begin(), a.end());
		for (int i = 0; i < 64; i ++)
			y = y * 2 + a[i];
		auto z = next_mask(x);
		CHECK(bool(z) == ok);
		if (ok) CHECK(*z == y);
	}
	FlatHash<i64> h;
	unordered_map<u64, i64> ref;
	for (int it = 0; it < 40000; it ++) {
		u64 x = it % 3 ? u64(ri(0, 999)) : rand64();
		if (it % 100 == 0) x = ~0ULL;
		int d = ri(-1000, 1000);
		h[x] += d, ref[x] += d;
		CHECK(h.size() == ref.size());
		CHECK(h.find(x) && *h.find(x) == ref[x]);
		CHECK(
		    static_cast<const FlatHash<i64> &>(h).find(
		        x) != nullptr);
		if (it == 20000) h.clear(), ref.clear();
	}
	for (auto [x, v] : ref)
		CHECK(h.find(x) && *h.find(x) == v);
	h.clear();
	CHECK(!h.find(0) && !h.find(~0ULL) && !h.size());
	CHECK(h[0] == 0 && h[~0ULL] == 0);
	// Force a wrap-around collision cluster; salt is
	// fixed before inserting.
	FlatHash<int> cluster;
	cluster.seed = 0;
	vector<u64> keys;
	for (u64 x = 0; keys.size() < 4; x ++)
		if ((Hash::mix(x) & 7) == 7) keys.push_back(x);
	for (int i = 0; i < 4; i ++)
		cluster[keys[i]] = i;
	for (int i = 0; i < 4; i ++)
		CHECK(*cluster.find(keys[i]) == i);
	CHECK(!cluster.find(keys.back() + 1));
	struct alignas(max_align_t) Node {
		u64 x, y;
	};
	Arena pool(1 << 16);
	vector<Node *> nodes;
	for (int i = 0; i < 100; i ++) {
		*pool.make<char>() = 'x';
		auto p = pool.make<Node>(u64(i), u64(i * i));
		CHECK(uintptr_t(p) % alignof(Node) == 0);
		nodes.push_back(p);
	}
	for (int i = 0; i < 100; i ++)
		CHECK(nodes[i]->x == u64(i) &&
		      nodes[i]->y == u64(i * i));
	pool.reset();
	CHECK(pool.pos == 0 && *pool.make<int>(42) == 42);
	Arena small(sizeof(u64));
	CHECK(*small.make<u64>(~0ULL) == ~0ULL);
	bool threw = false;
	try {
		small.make<char>();
	} catch (const bad_alloc &) {
		threw = true;
	}
	CHECK(threw);
	Arena empty(0);
	threw = false;
	try {
		empty.make<int>();
	} catch (const bad_alloc &) {
		threw = true;
	}
	CHECK(threw);
	struct Throws {
		Throws() { throw 1; }
	};
	pool.reset();
	try {
		pool.make<Throws>();
	} catch (int) {
	}
	CHECK(pool.pos == 0);
	xor_words(nullptr, nullptr, 0);
	for (int n = 0; n <= 513; n ++)
		for (int off = 0; off < 4; off ++) {
			vector<u64> a(n + 8), b(n + 8);
			for (auto &x : a)
				x = rand64();
			for (auto &x : b)
				x = rand64();
			auto want = a, scalar = a, simd = a;
			for (int i = off; i < off + n; i ++)
				want[i] ^= b[i];
			xor_words_scalar(scalar.data() + off,
			                 b.data() + off, n);
			xor_words(simd.data() + off, b.data() + off,
			          n);
			CHECK(want == scalar && want == simd);
			xor_words(simd.data() + off,
			          simd.data() + off, n);
			for (int i = off; i < off + n; i ++)
				want[i] = 0;
			CHECK(want == simd);
		}
#if defined(__GNUC__) &&                               \
    (defined(__i386__) || defined(__x86_64__))
	cout << "CPU AVX2: "
	     << bool(__builtin_cpu_supports("avx2"))
	     << "\n";
#endif
#ifdef BL_PORTABLE
	cout << "Dispatch: forced portable\n";
#endif
	done("performance");
}
