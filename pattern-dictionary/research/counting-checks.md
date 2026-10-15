# 计数条目核验记录

核验对象：模式字典 C01–C42；数值复核重点为逆序数、三类背包计数、反射公式与同上下限球盒计数。只使用 Python 3 标准库。

- 原有 **1106 组**：450 组随机背包（无限件、有界件、可区分 0/1 各 150 组），与逐类枚举件数的 DP 对照；656 组逆序数（`1 ≤ n ≤ 16`、`1 ≤ k ≤ 41`），核对每组的全部前缀系数。随机种子固定为 `6016`。
- 追加 **1266 组**：486 组弱／严格边界格路，与逐格 DP 对照；780 组同上下限球盒，与逐盒枚举装量的 DP 对照。覆盖空路径、不可达终点、上下限相等、空装量等边界。
- 所有检查通过。小数据检查用于发现索引、符号、计数口径错误，不替代恒等式证明或渐近复杂度分析；没有测试 NTT 性能，也未据此宣称压位算法普遍更快。

人工审阅确认：C37 的必经点须按坐标偏序成链；C38 只从禁点扣“首次撞点”贡献；C40 的二叉树是左右有别的满二叉树；C42 必须是真正嵌套的允许集合，大小有序不够。斯特林数按一行／固定列区分；固定列的近线性实现使用 Log／Exp 求幂，而非逐轮卷积。

复现：将下面代码保存为 `check_counting.py`，运行 `python3 check_counting.py`。预期依次输出 `original checks: 1106`、`additional checks: 1266`、`all checks: 2372`。

```python
import math
import random

MOD = 998244353
rng = random.Random(6016)

def choose(n, k):
    return math.comb(n, k) if 0 <= k <= n else 0

def fps_exp(h):
    f = [1] + [0] * (len(h) - 1)
    for i in range(1, len(h)):
        f[i] = sum(j * h[j] * f[i-j] for j in range(1, i+1))
        f[i] = f[i] * pow(i, MOD-2, MOD) % MOD
    return f

def knapsack_by_log(items, limit, mode):
    g = [0] * (limit + 1)
    for v, cap in items:
        for j in range(v, limit+1, v):
            g[j] += v
        stop = (cap+1)*v if mode == "bounded" else 2*v
        if mode != "unlimited":
            for j in range(stop, limit+1, stop):
                g[j] -= stop
    return fps_exp([0] + [g[t]*pow(t, MOD-2, MOD) % MOD
                          for t in range(1, limit+1)])

def direct_knapsack(items, limit, mode):
    f = [1] + [0] * limit
    for v, cap in items:
        cap = limit//v if mode == "unlimited" else (1 if mode == "01" else cap)
        nxt = [0] * (limit+1)
        for old, ways in enumerate(f):
            for take in range(min(cap, (limit-old)//v)+1):
                nxt[old+take*v] = (nxt[old+take*v] + ways) % MOD
        f = nxt
    return f

checked_backpack = 0
for mode in ("unlimited", "bounded", "01"):
    for _ in range(150):
        limit = rng.randrange(1, 21)
        items = [(rng.randrange(1, 9), rng.randrange(0, 6))
                 for _ in range(rng.randrange(0, 9))]
        assert knapsack_by_log(items, limit, mode) == direct_knapsack(items, limit, mode)
        checked_backpack += 1

def inversion_layers(n, limit):
    a = [choose(n+j-1, j) % MOD for j in range(limit+1)]
    h = [0] * (limit+1)
    for layer in range(limit//(n+1)+1):
        if layer:
            for j in range(layer, limit+1):
                a[j] = (a[j] + a[j-layer]) % MOD
        shift = layer*(n+1)
        for j in range(limit-shift+1):
            h[j+shift] = (h[j+shift] + a[j]) % MOD
    f = h[:]
    for r in range(1, limit+1):
        sign = -1 if r % 2 else 1
        for shift in (r*(3*r-1)//2, r*(3*r+1)//2):
            for j in range(shift, limit+1):
                f[j] = (f[j] + sign*h[j-shift]) % MOD
    return f

checked_inversions = 0
for n in range(1, 17):
    for limit in range(1, 42):
        # 插入第 i 个最大元素可新增 0..i-1 个逆序对，直接 DP 作独立基准。
        reference = direct_knapsack([(1, i-1) for i in range(1, n+1)], limit, "bounded")
        assert inversion_layers(n, limit) == reference
        checked_inversions += 1
assert checked_backpack == 450 and checked_inversions == 656
print("original checks:", checked_backpack + checked_inversions)

def grid_dp(a, b, height=0, strict=False):
    dp = [[0]*(b+1) for _ in range(a+1)]
    dp[0][0] = 1
    for x in range(a+1):
        for y in range(b+1):
            if x == y == 0:
                continue
            if y > x+height or (strict and y >= x):
                continue
            dp[x][y] = (dp[x-1][y] if x else 0) + (dp[x][y-1] if y else 0)
    return dp[a][b]

checked_reflection = 0
for a in range(9):
    for b in range(9):
        for h in range(5):
            answer = 0 if b > a+h else choose(a+b, a)-choose(a+b, a+h+1)
            assert answer == grid_dp(a, b, h)
            checked_reflection += 1
        strict_answer = 1 if a == b == 0 else (grid_dp(a-1, b) if a > b else 0)
        assert strict_answer == grid_dp(a, b, strict=True)
        checked_reflection += 1

checked_boxes = 0
for balls in range(13):
    for boxes in range(1, 6):
        for low in range(3):
            for high in range(low, 5):
                remain, cap = balls-boxes*low, high-low
                answer = 0
                if 0 <= remain <= boxes*cap:
                    answer = sum((-1)**j * choose(boxes, j) *
                                 choose(remain-j*(cap+1)+boxes-1, boxes-1)
                                 for j in range(boxes+1))
                if remain < 0:
                    reference = 0
                else:
                    reference = direct_knapsack([(1, cap)]*boxes, remain, "bounded")[remain]
                assert answer % MOD == reference
                checked_boxes += 1
assert checked_reflection == 486 and checked_boxes == 780
print("additional checks:", checked_reflection + checked_boxes)
print("all checks:", checked_backpack + checked_inversions + checked_reflection + checked_boxes)
```
