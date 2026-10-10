# BlueLagoon 模板覆盖审计

审计日期：2026-10-09。对象：当前 `content/` 中的 100 份 C++20 代码和 5 份说明，以及它们生成的打印册。

结论：当前版本覆盖了常用算法，但没有完整覆盖参考资料。缺项不仅是冷门算法，也包括一般图带权匹配、负费用环循环流、Delaunay、runs、二维 BIT、GF(2) 消元等实用板子。先前的编译、对拍和排版检查验证的是已收录内容，不能证明参考资料的覆盖完整性。

## 范围与判断标准

- 本地 PDF：`../teamnote.pdf`（24 页）、`../teambook.pdf`（22 页）、`../reference.pdf`（24 页）。下文页码为 PDF 页码，从 1 开始。
- [cp-book 固定版本](https://github.com/ecnerwala/cp-book/tree/248fdab618caec560fe7b4e3e2a2babe97a3aad1/src)：检查 `src/` 算法模块及相关接口。
- [koosaga Library 固定版本](https://github.com/koosaga/olympiad/tree/bdc7524704edadfa60082df14925753a95e61360/Library)：主要检查 `Library/codes`，同时检查 `Library/icpc-book-master/content` 的独有内容。没有把 USACO 题解、测试、历史重复版本当成独立板子。
- [KACTL 固定版本](https://github.com/kth-competitive-programming/kactl/tree/27faa89f9b47e5fa4578eadea6122b59da544052/content)：检查算法头文件及辅助内容。仓库版本见 `sources.lock.json`；本报告不声称覆盖后续提交。
- **缺失**：没有现成实现，也没有功能相同的替代。
- **部分覆盖**：只有公式、特殊情形、较慢版本，或缺少参考实现的重要接口。
- **替代实现**：实现方式不同，但对应的基本任务已有现成代码；不按缺失计。

按功能合并重复板子，不以文件名是否相同判定，也不计算容易误导的覆盖百分比。表中“缺失”指可直接使用的模板缺失；部分短代码可以临场写出，但仍是资料差异。

## 相比 teamnote.pdf

| 页码 / 原项目 | 当前状态 | 差异 |
|---|---|---|
| 2–3 Minimum Cost Circulation | 部分覆盖 | 有无费用上下界流与 SSP 费用流；没有 Goldberg–Tarjan / cost scaling，以及允许残量负费用环的通用最小费用循环流。 |
| 6 General Weighted Matching | **缺失** | 当前 `WeightedMatching.h` 是 Hungarian 二分图指派；`Blossom.h` 是一般图无权匹配。两者都不覆盖一般图带权匹配。 |
| 7 Bipolar Orientation | **缺失** | 无 st-numbering / 双极定向。 |
| 8 Directed MST | 部分覆盖 | 有 O(nm) 缩环求权值版本；无快速堆实现、选边恢复。 |
| 9 Dynamic MST Offline | **缺失** | 回滚 DSU 离线连通性不能替代动态最小生成树。 |
| 10 General Edge Coloring | **缺失** | 只有二分图最优边染色；无一般图 Vizing 的 Δ+1 染色。 |
| 10 k Shortest Paths | **缺失** | 只有单源最短路，未收录 K 短路。 |
| 11 Perfect Elimination Ordering | **缺失** | 无弦图识别 / 完美消除序。 |
| 11 Tree Decomposition, tw ≤ 2 | **缺失** | 无树宽不超过 2 的树分解。 |
| 12 LCS: Bitset, Hirschberg | 部分覆盖 | Bitset 只求长度；无 Hirschberg 恢复 LCS。 |
| 12 LCS: Circular | **缺失** | 无循环 LCS。 |
| 13 Run Enumerate | **缺失** | 无 runs（极大周期段）枚举。 |
| 14–16 Polynomial / Algorithms on Polynomial | 部分覆盖 | 常用乘除、求逆、ln/exp、幂、平方根、多点求值和插值已有；无一般 k 次 FPS 开根接口，泛型域与多项式类接口也较少。 |
| 17 P-recursive | **缺失** | BM 只处理常系数线性递推，不能替代多项式系数递推猜测。 |
| 19 Discrete Kth Root | 部分覆盖 | 只有模平方根；无一般离散 k 次根。 |
| 20 Nim Multiplication | **缺失** | 博弈说明里的 Nim 异或不能替代 nimber 乘法。 |
| 20 Partition Number | 部分覆盖 | 有五边形数递推公式，没有现成生成函数 / 求表代码。 |
| 20 Prime Counting | 替代实现 | `Min25(n).cnt[0]` 已给出 π(n)；未照搬参考文件的专用实现，不算功能缺失。 |
| 20 Stern–Brocot Tree | **缺失** | 无有理数区间内最简分数搜索。 |
| 20–21 Tetration | **缺失** | 无扩展欧拉降幂处理幂塔。 |
| 21 Xudyh Sieve | 部分覆盖 | 杜教筛已有 φ/μ 前缀和；无参考中的通用卷积关系 f*g 前缀和框架。 |
| 21–22 3D Convex Hull | 部分覆盖 | 当前要求一般位置；无完整退化处理，性能接口也与参考快速版本不同。 |
| 22–23 Delaunay | **缺失** | 无平面 Delaunay 三角剖分。 |
| 23 Smallest Enclosing Circle | 部分覆盖 | 二维最小圆已收录；同节三维近似最小包围球没有。 |
| 23 Mathematics | 部分覆盖 | 有普通 Cayley、Matrix Tree 与面积算法；无 Tutte 矩阵说明、指定度数树计数公式、完整 Green 定理与圆弧线积分速查。 |
| 24 Fast LL Division / Modulo | **缺失** | 无预处理除数的快速除法 / 取模。 |
| 24 Bit Twiddling Hack | 部分覆盖 | 有子集枚举、位内建函数注意事项；未覆盖整套位技巧。 |
| 24 Fast Integer IO | **缺失** | 当前是关闭同步的 iostream；没有 fread 缓冲快读快写。 |
| 24 Nasty Stack Hacks | **缺失** | 没有汇编换栈；仅有深递归风险提示。属于平台技巧，优先级低。 |

其余主要算法已有同等基本任务的实现：Dinic / 可行循环流、Gomory–Hu、二分图匹配、一般图无权匹配、Simplex、BCC、SCC / 2-SAT、支配树、LCT 基本动态森林、全局最小割、二分图边染色、AC、Duval、SA、Manacher、回文树、BM、组合数、De Bruijn、扩展 GCD、离散对数、因数分解、动态直线凸包、半平面交、二维最小包围圆、凸多边形切线。这里的“已有”不意味着所有泛型、性能与扩展接口均相同。

## teambook.pdf 额外暴露的缺项

| 页码 / 原项目 | 当前状态 | 差异 |
|---|---|---|
| 2 fastIO / hashTable / optimizations | 缺失 / 部分覆盖 | 无缓冲 IO、手写开放寻址哈希、汇编 / SIMD 优化集；现有 RandomHash 只是哈希函数。 |
| 2 useful | 部分覆盖 | PBDS 排序集合已有；没有完整的 bitset find_first / find_next 等工具集。 |
| 3 bitset | **缺失** | 无通用运行时长度动态 bitset；BitsetLCS 的内部存储不是公开动态 bitset 模板。 |
| 4 fft | **缺失** | 无实数 / 复数 FFT。整数及模卷积由 NTT 和三模 CRT 替代。 |
| 5 blackbox | **缺失** | 实际为一侧固定、另一侧逐项输入的在线卷积；不是黑盒矩阵求解。 |
| 9–10 polygonArcCut | **缺失** | 无直线段与圆弧混合边界区域的半平面裁剪；圆与多边形交面积不等价。 |
| 10 checkPlaneInt | 部分覆盖 | 是二维半平面可行点的随机增量算法，包含无界区域情形；当前半平面交依赖外加边界框，没有这一接口。 |
| 14–15 minCostNegCycle | **缺失** | 无最小平均费用环 / 消负环费用优化。普通负环检测不是费用优化器。 |
| 15 retro | **缺失** | 无有环游戏图的胜 / 负 / 和逆向分析。 |
| 15–16 smith | **缺失** | 无 Smith 值 / 有环无偏组合游戏处理。 |
| 16–17 twoChineseFast | 部分覆盖 | 当前最小树形图版本较慢且只求权值。 |
| 18 chordaltree | **缺失** | 无弦图团树构造。 |
| 18 minimization | **缺失** | 无 DFA / 带输出状态系统最小化。 |
| 18–22 matroidIntersection | **缺失** | 无拟阵交模板。 |

`Template.java` 未收录，属于选定 GNU C++20 的语言范围。Vim、比赛骨架、CRT、模乘、逆元、Pollard、常用多项式、Simplex、类欧几里得求和、BM、积分、圆公切线、最小圆、切线、凸包直径、CHT、PAM、SAM、SA、Duval、点分治骨架、支配树、Blossom、HLD、Hungarian、全局最小割、LCT 的基本功能已有。点分治骨架不等于每类点分治题的现成查询实现。

## reference.pdf 额外暴露的缺项

| 页码 / 原项目 | 当前状态 | 差异 |
|---|---|---|
| 4–6 备注、约数表、公式 | 部分覆盖 | 无历法速查、高合成数 / 最大约数个数表、球面距离与球冠公式、常见积分表、Runge–Kutta、Bell / 分拆数数值表、Fibonacci 恒等式、Gambler's ruin 概率速查。部分组合数公式已有。 |
| 7 Fourier transform | 部分覆盖 | 模 / 整数卷积已有替代，实数 FFT 没有。 |
| 14–15 Delaunay | **缺失** | 同上。 |
| 18 Suffix Tree | **缺失** | 无 Ukkonen 等显式后缀树；SA 和 SAM 不等于该实现。 |
| 21–22 Two Chinese | 部分覆盖 | 同上：快速最小树形图接口缺失。 |
| 23–24 Hexagonal / Triangular paper | 未收录附件 | 无六角 / 三角绘图纸。这是打印附件，不是算法板子。 |

该 PDF 的 Edmonds 是一般图无权匹配，当前 Blossom 已覆盖，不能误判为缺失。GNU C++20 骨架、调试 / 对拍命令、支配树、Hungarian、Dinic / Gomory–Hu、基本 LCT、半平面交、AC、PAM、SAM、KMP / Duval、Simplex、隐式 Treap 均有对应。

## 三个代码仓库的其他差异

下表合并重复功能。来源缩写：C = cp-book，K = KACTL，O = koosaga Library/codes，O旧 = Library/icpc-book-master/content。

| 类别 | 尚无现成模板的功能 | 来源 / 核对入口 |
|---|---|---|
| 常用数据结构 | 二维 Fenwick、Mo 排序、二维前缀和 / 子矩阵查询、通用矩阵运算与矩阵快速幂 | K `FenwickTree2d.h`, `MoQueries.h`, `SubMatrix.h`, `Matrix.h`；C `num/mat.hpp` |
| 树工具 | 第 k 级祖先 / Binary Lifting 专用接口、换根 DP 通用框架 | K `BinaryLifting.h`；C `tree/level_ancestor.hpp`；O `all_direction_tree_dp.cpp`。当前 HLD 有 LCA，但没有祖先跳跃接口。 |
| 进阶动态树 | Top Tree、HDLT 在线动态连通性 | C `tree/top_tree.hpp`；O `top_tree.cpp`, `HDLT.cpp` |
| 进阶序列结构 | 排列树 / Busan tree、Kinetic Segment Tree、在线区间众数、重量平衡树、聚合双端队列 | C `seq/perm_tree.hpp`；O 对应同名文件及 `busan_tree.cpp`, `deque.cpp` |
| 泛型结构 | 泛型 monoid 线段树 / lazy 框架、最小值及次数聚合器 | C `ds/seg_tree.hpp`, `cnt_min.hpp`；O `atcoder_segtree.cpp`。当前线段树只有区间赋值 / 加法 / 求和，算部分覆盖。 |
| 基础图工具 | 拓扑排序、Floyd、Bellman–Ford 单源最短路及负环影响传播 | K `TopoSort.h`, `FloydWarshall.h`, `BellmanFord.h`。当前仅 Dijkstra 和全图负环检测。 |
| 图搜索 | 极大团枚举、最大团、最大独立集 | K `MaximalCliques.h`, `MaximumClique.h`, `MaximumIndependentSet.h` |
| 图结构 | SPQR 树、三边连通、四环计数、边不交生成树、extreme vertex sets | C `graph/spqr_tree.hpp`；O `triedgeconnectivity.cpp`, `count_4_cycles.cpp`, `disjoint_spantree.cpp`, `extreme_vertex_sets.cpp` |
| 图定向与分解 | st-numbering / make_st_dag、弦图 / 团树、树宽 2 分解、动态 MST、K 短路、一般图边染色 | C / O；对应前述 PDF 缺项 |
| 组合优化 | 一般图带权匹配、skew-symmetric flow、精确整数 / 大整数线性规划 | O `matching_weighted_dense.cpp`, `matching_weighted_sparse.cpp`, `skew_symmetric_flow.cpp`, `lp_linear_bigint.cpp` |
| 字符串 | 后缀树、循环 LCS、semi-local LCS / seaweed、双端回文树、runs | K `SuffixTree.h`；O `lcs_circular.cpp`, `lcs_seaweed.cpp`, `palindrome_tree_deque.cpp`, `run_enumerate.cpp` |
| 多项式 | 实数 FFT、在线 / relaxed 卷积、FPS 复合、Bostan–Mori 有理函数取项、快速等差采样点批量平移 / 求值、Taylor shift 专用接口 | C `fft/engines/real.hpp`, `online.hpp`, `series.hpp`, `ap_sampled_poly.hpp`；K FFT；当前连续点插值只求单点。 |
| 特殊卷积 | 自然溢出模 2^64 卷积、多维卷积、下标乘积模 2^n 的卷积 | O `convolution_2^64.cpp`, `convolution_kdim.cpp`, `convolution_2^n.cpp`。后者不是 XOR / AND / OR 卷积。 |
| 递推 | P-recursive、合数模 Reeds–Sloane、快速 NTT 递推取项 | O `recurrence_precursive.cpp`, `recurrence_reedssloane.cpp`；C FPS 有理取项。当前 BM + Kitamasa 为 O(d² log k)，不是快速版本。 |
| 线性代数 | GF(2) 方程组消元、实数 / 模矩阵求逆、三对角系统、精确整数行列式、实数行列式接口 | K `SolveLinearBinary.h`, `MatrixInverse*.h`, `Tridiagonal.h`, `IntDeterminant.h`, `Determinant.h`。当前实数消元、素数模消元 / 行列式及 XorBasis 只是部分覆盖。 |
| 代数与计数 | 特征多项式、伴随矩阵、Pfaffian、图色多项式 | C `linalg/char_poly.hpp`；O `characteristic_polynomial.cpp`, `adjugate.cpp`, `pfaffian.cpp`, `chromatic_polynomial.cpp` |
| 大数与矩阵性能 | 大整数、Karatsuba、Strassen | O `bigint.cpp`, `karatsuba.cpp`, `strassen.cpp`；O旧 Karatsuba |
| 数值算法 | 实多项式根隔离 / 求根、黄金分割、三分、Hill Climbing | K `PolyRoots.h`, `GoldenSectionSearch.h`, `TernarySearch.h`, `HillClimbing.h`；O `find_roots.cpp` |
| 数论 | Jacobi 符号、Stern–Brocot / 连分数 / 分数二分、一般离散 k 次根、幂塔、四平方表示 | C `nt/jacobi.hpp`；K `ContinuedFractions.h`, `FracBinarySearch.h`；O `fractions.cpp`, `kth_root.cpp`, `tetration.cpp`, `four_squares_theorem.cpp` |
| 数论框架 | 通用 Dirichlet 级数、格点 / 模区间计数专用接口 | C `nt/dirichlet_series.hpp`, `lattice_cnt.hpp`。现有 FloorSum 能构造部分计数，但没有封装接口。 |
| 代数类型 | 有理数、Hurwitz 四元数、pairnum 等代数数类型、通用截断级数与仿射函数组合 | C `num/fraction.hpp`, `quaternion_hurwitz.hpp`, `pairnum.hpp`, `trunc_series.hpp`, `linear_fn.hpp`。普通模运算 / FPS 已有，但这些泛型类型没有。 |
| 组合算法 | Garsia–Wachs / alphabetic Huffman、Hu–Shing、PQ tree、RSK、稳定婚姻 / 稳定室友 | C `seq/alphabetic_huffman_code.hpp`；O `garsia_wachs.cpp`, `hu_shing.cpp`, `pq_tree.cpp`, `rsk.cpp`, `stable_marriage.cpp`, `stable_roommate.cpp` |
| 排列与置换群 | 排列排名 / 反排名、multinomial 现成函数、Schreier–Sims 群成员判定 | K `IntPerm.h`, `multinomial.h`；O旧 `schreier-sims.cpp` |
| 组合求表 | 分拆数、Stirling 数快速计算代码、nimber 乘法 | O `partition_number.cpp`, `stirling.cpp`, `nim_multiplication.cpp`；C `num/nim_prod.hpp`。前两项仅有递推公式。 |
| 博弈 | 有环图逆向分析、Smith 值、cold partisan games | teambook；C `combo_games/cold_games.hpp`。当前 SG / Nim / Wythoff 说明没有覆盖。 |
| 几何常用补充 | Delaunay、Manhattan MST、KD-tree、多边形重心 / 直线裁剪 / 并面积、多面体体积、球面距离、线性变换、角度排序工具 | K 对应同名文件；O `delaunay.cpp`, `polygon_union.cpp` |
| 几何进阶 | 圆盘凸包、删除点凸包、三角形内点计数、圆弧区域裁剪、三维最小包围球 | O `convex_hull_circle.cpp`, `convex_hull_decremental.cpp`, `count_points_in_triangle.cpp`；前述 PDF |
| 常用杂项 | LIS、区间最少覆盖、区间集合、分段常值区间、Pisinger 快速背包、无限棋盘马距离 | K `LIS.h`, `IntervalCover.h`, `IntervalContainer.h`, `ConstantIntervals.h`, `FastKnapsack.h`；O `pisinger.cpp`, `knight_distance.cpp` |
| 速度与内存工具 | 缓冲 IO、FastMod / fastlldiv、开放寻址哈希、arena 分配器 / STL allocator、小指针、SIMD / 循环展开、crope | K `FastInput.h`, `FastMod.h`, `HashMap.h`, `BumpAllocator*.h`, `SmallPtr.h`, `SIMD.h`, `Unrolling.h`；C `hash_map.hpp`；O tools |

### 已有替代，或只是另一种实现

- KACTL 的 `WeightedMatching.h` 同样是二分图带权匹配，已由 Hungarian 覆盖；需要补的是 koosaga / teamnote 的一般图带权匹配。
- `CompressTree` 已由 VirtualTree 覆盖；LCA 已由 HLD 覆盖；DFS 二分图增广匹配已由 Hopcroft–Karp 覆盖；s-t MinCut 可用 Dinic 的源侧集合。
- `Prime Counting / prime_prefix_sums` 已由 Min25 的 cnt / sum 覆盖；Mertens 求和已由杜教筛覆盖。专用更快实现属于性能补充。
- 模逆、CRT、模乘、模幂、因数分解、模平方根、类欧几里得求和、Z 函数、最小表示、常用圆 / 线相交都已有。模数类型类、哈希变体等不能仅因文件名不同就算算法缺失。
- Push–Relabel、Edmonds–Karp、固定步长积分、分段 / 位压缩筛、固定模数离散对数、线性时间 RMQ / SA 等是目前没有的实现或性能变体；对应基本任务已有 Dinic、自适应 Simpson、线性筛、exBSGS、SparseTable、倍增 SA。超大范围分段筛接口仍需单独补。
- 上游 `tensor`、`yc`、比较器、构建聚合文件等库基础设施不逐个计为算法缺项；C++20 标准库也覆盖了部分位操作工具。Java 与平台汇编技巧不影响 GNU C++20 算法范围。

## 已有但值得补强的接口

| 当前模块 | 缺少的接口 / 限制 |
|---|---|
| MinCostFlow / LowerBound | 通用带费用上下界循环流、负环费用优化；当前 SSP 的适用前提不能省略。 |
| DirectedMST | 选边恢复与快速实现。 |
| BitsetLCS | 实际子序列恢复、循环 / semi-local 版本。 |
| HLD | 祖先跳跃、路径第 k 点、非交换路径的方向处理。 |
| LCT | 当前为路径异或；没有泛型路径聚合 / lazy，以及虚子树维护模板。 |
| SegmentTree / Persistent / Treap | 目前都是特定任务的实现；无泛型懒标记、持久化 lazy / 序列等扩展。不是仅换一个聚合类型就能覆盖所有情况。 |
| MatrixTree | 当前封装为无向无权多重图计数；无带权 / 有向 wrapper、BEST 欧拉回路计数。 |
| Simplex | 当前为浮点版本；没有精确有理数 / 大整数求解器。 |
| DuSieve / Min25 | 固定函数或受限素数取值形式，不能称作任意积性函数的通用黑盒。 |
| 3D Hull / HalfPlane | 三维退化情形、无界半平面交的可靠分类 / 可行点接口。 |
| Polynomial / FPS | 一般 k 次根、复合、Taylor shift、在线卷积、域泛型。 |
| Formulas / Games | 有公式的地方不自动等于有经过测试、可直接抄用的代码。 |

## 建议补充顺序

1. **先补参考主册的实质缺项**：一般图带权匹配、通用最小费用循环流、Delaunay、runs、K 短路、离线动态 MST、一般图边染色、弦图 / 团树、st-numbering、树宽 2 分解、P-recursive、离散 k 次根、幂塔、nimber 乘法、Stern–Brocot、LCS 恢复 / 循环版。
2. **补小而常用的接口**：二维 BIT、Mo、祖先跳跃 / 路径第 k 点、矩阵快速幂 / 求逆、GF(2) 消元、LIS、分拆 / Stirling 求表、缓冲 IO、多边形裁剪 / 重心、Manhattan MST、KD-tree、后缀树。
3. **补多项式与组合优化扩展**：实数 FFT、在线卷积、Bostan–Mori、FPS 复合 / Taylor shift、合数模递推、拟阵交、稳定婚姻、特征多项式、Pfaffian、Garsia–Wachs。
4. **最后评估高维护成本内容**：Top Tree、HDLT、SPQR、PQ tree、精确 LP、Hu–Shing、复杂动态几何与平台汇编。没有页数上限也仍需考虑队伍能否使用和维护。

这份审计只记录差异，没有向打印册加入未经适配和验证的新代码。每个新算法应继续按 C++20、个人代码风格、来源 / 许可、功能测试、渲染检查的现有流程加入。
