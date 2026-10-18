# 来源与版本

外部仓库按 `sources.lock.json` 的 commit 固定；链接指向实际参考版本。直接改写的上游原件存于 `third_party/originals`，用于追溯作者、原注释、接口和许可证。cp-book 用于选题与组织参考，本册没有直接导入其 C++23 泛型实现。

本册不是统一许可证的代码库。BlueLagoon 自写部分标记 CC0-1.0；上游明确给出的 MIT、Boost、Unlicense、CC0 按各文件保留。未标明许可证的上游文件记录为 “Not specified upstream”，不能把它们归入自写 CC0 部分。许可证文本见 `third_party/licenses`。上游测试状态仅作历史记录，本册实际测试状态以 `tests/report.json` 与 `output/coverage.json` 为准。

KACTL 风格的维护要点：单段源码、显式章节目录、作者/来源/约定/复杂度、短码、独立编译、随机对拍与 CI。本册页数没有固定上限。

LaTeX 组织参考 [KACTL 的维护说明](https://github.com/kth-competitive-programming/kactl#customizing-kactl)：简短主文件导入每章 `chapter.tex`，代码通过导入命令和文件头元数据生成排版。BlueLagoon 的章节／条目命令为自写实现，保留 XeLaTeX 中文、双栏与 Pygments 高亮。上游文件历史元数据保存在 `third_party/attribution.json`，不参与章节选择。

| 本册文件 | 作者 / 改编者 | 来源 | 许可证记录 |
|---|---|---|---|
| `advanced/BostanMori.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/Chordal.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/chordal_graph.cpp) | Not specified upstream; retain attribution |
| `advanced/Delaunay.h` | Philippe Legault; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/FastDelaunay.h) | MIT |
| `advanced/DirectedMSTFast.h` | chilli, Takanori MAEHARA, Benq, Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/DirectedMST.h) | CC0 |
| `advanced/DynamicMST.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/offline_dynamic_mst.cpp) | Not specified upstream; retain attribution |
| `advanced/EdgeColoring.h` | Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/EdgeColoring.h) | CC0 |
| `advanced/FPSCompose.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/GeneralWeightedMatching.h` | koosaga / upstream blossom; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/combinatorial_optimization/matching_weighted_dense.cpp) | Not specified upstream; retain attribution |
| `advanced/KShortest.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/KthRoot.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/MatroidIntersection.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/MinCostCirculation.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/NimProduct.h` | koosaga / upstream nimbers; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/math/nim_multiplication.cpp) | Not specified upstream; retain attribution |
| `advanced/OnlineConvolution.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/PRecursive.h` | koosaga / upstream P-recursive; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/math/recurrence/recurrence_precursive.cpp) | Not specified upstream; retain attribution |
| `advanced/Pfaffian.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/RealFFT.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `advanced/ReedsSloane.h` | koosaga / upstream Reeds-Sloane; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/math/recurrence/recurrence_reedssloane.cpp) | Not specified upstream; retain attribution |
| `advanced/Runs.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/string/run_enumerate.cpp) | Not specified upstream; retain attribution |
| `advanced/SuffixTree.h` | Unknown / KACTL; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/strings/SuffixTree.h) | Not specified upstream; retain attribution |
| `advanced/Tetration.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/ArbitraryConvolution.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/ConsecutiveInterpolation.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/FPS.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/FPSSqrt.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/Gauss.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/MatrixTree.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/NTT.h` | BlueLagoon | NTT structure informed by ecnerwala/cp-book src/fft; BlueLagoon C++20 implementation | CC0-1.0 |
| `algebra/Polynomial.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/Recurrence.h` | BlueLagoon | Berlekamp-Massey / Kitamasa; reference cp-book and koosaga; BlueLagoon implementation | CC0-1.0 |
| `algebra/Simplex.h` | Stanford; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/numerical/Simplex.h) | MIT |
| `algebra/Transforms.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `algebra/XorBasis.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `contest/Base.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `contest/Int128.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `contest/OrderedSet.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `contest/RandomHash.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `contest/Template.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/Cartesian.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/Centroid.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/DSU.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/Fenwick.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/HLD.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/LCT.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/LiChao.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/LineContainer.h` | Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/data-structures/LineContainer.h) | CC0 |
| `data-structures/Persistent.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/SegmentTree.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/SparseTable.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/Treap.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `data-structures/UnionFindRollback.h` | Lukas Polacek, Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/data-structures/UnionFindRollback.h) | CC0 |
| `data-structures/VirtualTree.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `flow/BipartiteEdgeColor.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `flow/Blossom.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/combinatorial_optimization/matching_short.cpp) | Not specified upstream; retain attribution |
| `flow/Dinic.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `flow/GomoryHu.h` | chilli, Takanori MAEHARA; BlueLagoon | KACTL GomoryHu.h (Gusfield); BlueLagoon Dinic adaptation | CC0-1.0 |
| `flow/HopcroftKarp.h` | Adam Soltan; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/HopcroftKarp.h) | CC0 |
| `flow/LowerBound.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `flow/MinCostFlow.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `flow/MinimumVertexCover.h` | Johan Sannemo, Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/MinimumVertexCover.h) | CC0 |
| `flow/WeightedMatching.h` | Jeremy Lim, Joshua Andersson; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/WeightedMatching.h) | CC0 |
| `geometry/3dHull.h` | Johan Sannemo; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/3dHull.h) | Not specified upstream; retain attribution |
| `geometry/CircleIntersection.h` | Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleIntersection.h) | CC0 |
| `geometry/CircleLine.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleLine.h) | CC0 |
| `geometry/CirclePolygonIntersection.h` | chilli, Takanori MAEHARA; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CirclePolygonIntersection.h) | CC0 |
| `geometry/CircleTangents.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/CircleTangents.h) | CC0 |
| `geometry/ClosestPair.h` | Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ClosestPair.h) | CC0 |
| `geometry/ConvexHull.h` | Stjepan Glavina, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ConvexHull.h) | Unlicense |
| `geometry/ConvexTangent.h` | zigui; koosaga library; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/geometry/tangent.cpp) | Not specified upstream; retain attribution |
| `geometry/HalfPlane.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `geometry/HullDiameter.h` | Oleksandr Bacherikov, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/HullDiameter.h) | Boost Software License |
| `geometry/InsidePolygon.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/InsidePolygon.h) | CC0 |
| `geometry/LineHullIntersection.h` | Oleksandr Bacherikov, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/LineHullIntersection.h) | Boost Software License |
| `geometry/LineProjectionReflection.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/LineProjectionReflection.h) | CC0 |
| `geometry/MinimumEnclosingCircle.h` | Andrew He, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/MinimumEnclosingCircle.h) | CC0 |
| `geometry/Minkowski.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `geometry/OnSegment.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/OnSegment.h) | CC0 |
| `geometry/Point.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/Point.h) | CC0 |
| `geometry/Point3D.h` | Ulf Lundstrom with inspiration from tinyKACTL; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/Point3D.h) | CC0 |
| `geometry/PointInsideHull.h` | chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/PointInsideHull.h) | CC0 |
| `geometry/PolygonArea.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/PolygonArea.h) | CC0 |
| `geometry/SegmentDistance.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/SegmentDistance.h) | CC0 |
| `geometry/SegmentIntersection.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/SegmentIntersection.h) | CC0 |
| `geometry/circumcircle.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/circumcircle.h) | CC0 |
| `geometry/lineDistance.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/lineDistance.h) | CC0 |
| `geometry/lineIntersection.h` | Victor Lecomte, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/lineIntersection.h) | CC0 |
| `geometry/sideOf.h` | Ulf Lundstrom; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/sideOf.h) | CC0 |
| `graph/BCC.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/DirectedMST.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/Dominator.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/graph/dominator.cpp) | Not specified upstream; retain attribution |
| `graph/DynamicConnectivity.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/EulerWalk.h` | Simon Lindholm; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/graph/EulerWalk.h) | CC0 |
| `graph/GlobalMinCut.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/SCC.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/ShortestPath.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/TreeIsomorphism.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `graph/TwoSat.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/Binomial.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/DiscreteLog.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/DuSieve.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/Factor.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/FloorSum.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/Min25.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/Modular.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/PrimitiveRoot.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `number-theory/Sieve.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/AhoCorasick.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/BitsetLCS.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/Duval.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/Eertree.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/Hash.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/Manacher.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/PrefixZ.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/SAM.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `strings/SuffixArray.h` | 罗穗骞, chilli; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/strings/SuffixArray.h) | Unknown |
| `strings/SuffixLCP.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/BinaryLifting.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/CombinatorialTables.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/FastIO.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/Fenwick2D.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/GaussBinary.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/LIS.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/ManhattanMST.h` | chilli, Takanori MAEHARA; BlueLagoon adaptation | [kactl](https://github.com/kth-competitive-programming/kactl/blob/27faa89f9b47e5fa4578eadea6122b59da544052/content/geometry/ManhattanMST.h) | CC0 |
| `supplement/Matrix.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/Mo.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/PolygonTools.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `supplement/SternBrocot.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `various/DeBruijn.h` | koosaga; BlueLagoon adaptation | [koosaga](https://github.com/koosaga/olympiad/blob/bdc7524704edadfa60082df14925753a95e61360/Library/codes/ad_hoc/debruijn.cpp) | Not specified upstream; retain attribution |
| `various/DivideDP.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `various/KnuthDP.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |
| `various/SMAWK.h` | BlueLagoon | SMAWK; oracle interface inspired by ecnerwala/cp-book/src/smawk.hpp; BlueLagoon recursive implementation | CC0-1.0 |
| `various/Simpson.h` | BlueLagoon | BlueLagoon implementation; standard algorithm | CC0-1.0 |

## 2026-10-17 性能工具

新增代码为 BlueLagoon 按标准算法编写。Barrett 使用向下取整倒数和一次向下修正，与 [ACL 的向上取整倒数实现](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp)不同；ACL 用于核对用途与范围，未直接导入其源码。AVX2 的函数属性与检测方式核对 [GCC x86 built-ins](https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html) 和 [函数属性](https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html)。

| 本册文件 | 作者 / 改编者 | 来源 | 许可证记录 |
|---|---|---|---|
| `performance/Arena.h` | BlueLagoon | BlueLagoon implementation; aligned bump allocation | CC0-1.0 |
| `performance/Barrett.h` | BlueLagoon | BlueLagoon implementation; reciprocal division with floor(2^64/m) | CC0-1.0 |
| `performance/BitTricks.h` | BlueLagoon | BlueLagoon implementation; Gosper's hack | CC0-1.0 |
| `performance/FastDiv.h` | BlueLagoon | BlueLagoon implementation; x86 DIV instruction | CC0-1.0 |
| `performance/FlatHash.h` | BlueLagoon | BlueLagoon implementation; linear probing with SplitMix64 | CC0-1.0 |
| `performance/Montgomery.h` | BlueLagoon | BlueLagoon implementation; Montgomery REDC and Newton inverse | CC0-1.0 |
| `performance/Simd.h` | BlueLagoon | BlueLagoon implementation; GCC x86 function targets and intrinsics | CC0-1.0 |

## 2026-10-18 Beats 与路径工具

三份新增源码为 BlueLagoon 编写；Mandatory 仅将现有支配树拆边封装，其依赖的来源与许可证保持原样。算法与复杂度核对 [kmyk 的 Beats 作者文档](https://kmyk.github.io/competitive-programming-library/data_structure/segment_tree_beats.hpp.html)、[Kare 的正权替换路论文](https://arxiv.org/abs/1511.06905)；[Vassilevska Williams 的 Faster Replacement Paths](https://arxiv.org/abs/1007.2216)说明有向替换路不是不可解问题。没有直接导入上述页面的实现。

| 本册文件 | 作者 / 改编者 | 来源 | 许可证记录 |
|---|---|---|---|
| `data-structures/SegmentTreeBeats.h` | BlueLagoon | 极值／严格次极值与摊还递归 | CC0-1.0 |
| `graph/Mandatory.h` | BlueLagoon | 最短路子图、边拆点和现有 Dominator | 包装代码 CC0-1.0；依赖保留原许可证 |
| `graph/ReplacementPaths.h` | BlueLagoon | 两棵共享选定最短路的 SPT、区间最小值 | CC0-1.0 |

## 2026-10-18 FWT／FMT 公式速查

`algebra/TransformsGuide.md` 按已有实现整理变换定义、逆变换、蝶形与正交补说明；子集卷积的分层 Zeta／Möbius 方法参考 Björklund、Husfeldt、Kaski、Koivisto 的 [Fourier meets Möbius: fast subset convolution](https://arxiv.org/abs/cs/0611101)。说明为自行撰写，没有导入论文源码。
