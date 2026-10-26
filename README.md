# BlueLagoon 算法模板书

[![Verify and print](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml/badge.svg)](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml)

BlueLagoon 的 ICPC 队伍模板：GNU C++20 代码、中文用法说明、回归与随机对拍，以及 A4 双栏打印版。

[查看打印版](output/BlueLagoon.pdf) · [下载最新构建](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml) · [维护指南](CONTRIBUTING.md) · [来源与许可证](SOURCES.md)

## 比赛时怎么用

- **纸质查阅**：下载 PDF，按实际大小、长边翻转双面打印。每个代码条目注明用途、前提、复杂度和依赖；抄代码时要一并收齐依赖。
- **本地使用**：从 `content/` 选取需要的头文件。代码使用 GNU 扩展，建议在比赛对应的 GCC 环境下编译。
- **生成单文件**：运行下面的命令展开依赖，再补上题目自己的 `solve()` / `main()`。不同章节可能有同名符号，不支持把全书一次性合并成一个程序。

```bash
python3 scripts/bundle.py algebra/FPS.h --output build/fps.cpp
```

仓库中的 PDF 是已提交的打印快照。最新源码对应的 PDF 请从成功的 Actions 运行中下载 `BlueLagoon-print`；`BlueLagoon-portable` 是包含源码、测试和 PDF 的便携包。下载 Actions 附件需要登录 GitHub。

### 开场配置 crun

在 Ubuntu / VS Code 的 **Bash 终端**输入一行：

```bash
crun(){ g++ -std=c++20 -O2 "$1.cpp" -o "$1.run" && "./$1.run" < "${2:-/dev/stdin}" > "${3:-/dev/stdout}"; }
```

先保存 `a.cpp`，并切换到它所在的目录：

```bash
crun a               # 手动输入，输出到终端
crun a a.in          # 文件输入，输出到终端
crun a a.in a.out    # 文件输入，输出到 a.out
```

编译失败不会运行旧程序；输出文件会被覆盖，编译错误和 `cerr` 仍留在终端。函数只在当前终端生效；长期使用可写入 `~/.bashrc` 后执行 `source ~/.bashrc`。更多调试与对拍命令见[比赛环境速查](content/contest/Environment.md)。

## 书里有什么

| 章节 | 内容 |
| --- | --- |
| 比赛环境与基础工具 | 主程序骨架、整数工具、随机哈希、编译与对拍 |
| 数据结构 | 线段树、树链剖分、持久化、动态树等 |
| 图论 | 连通性、最短路、支配树、路径与删边问题 |
| 网络流与匹配 | 最大流、费用流、二分图与一般图匹配 |
| 字符串 | 匹配、后缀数组、自动机、回文结构 |
| 数论 | 素数、同余、乘法函数与反演 |
| 代数 | 多项式、变换、线性代数 |
| 几何 | 二维与三维几何、凸包、半平面交 |
| 杂项 | DP 优化、博弈与常用公式 |
| 常用补充 | 常见算法和数据结构的补充实现 |
| 进阶附录 | 使用条件较强的进阶算法 |
| 性能工具与卡常技巧 | 模运算、哈希表、内存池、SIMD 等 |

完整条目和顺序以 PDF 目录及各章的 `chapter.tex` 为准。覆盖范围与尚缺内容见 [COVERAGE_AUDIT.md](COVERAGE_AUDIT.md)。

## 本地验证与构建

需要 **Python 3.10+** 和支持 C++20 的 **GNU g++**。生成 PDF 还需要 XeLaTeX 和中文字体；安装方式见[维护指南](CONTRIBUTING.md#构建环境)。

```bash
git clone https://github.com/luanmenglei/BlueLagoonAlgoBook.git
cd BlueLagoonAlgoBook
python3 -m pip install -r requirements.txt

python3 scripts/verify.py       # 覆盖检查、独立编译、全部测试
python3 scripts/build.py        # 验证报告合格后生成 output/BlueLagoon.pdf
python3 scripts/pack.py         # 打包到 ../BlueLagoon.zip
```

如果编译器不叫 `g++`，用 `python3 scripts/verify.py --compiler g++-16` 指定。也可使用 `make check pdf`；Windows 提供 `build.ps1`。

## 代码入书的要求

**每个 `content/**/*.h` 都必须有对应的运行测试，包括暂未印入书中的代码。** 新增或修改模板时：

1. 在 `tests/test_*.cpp` 中直接包含该头文件，调用它的接口，并用 `CHECK(...)` 核对结果；最后调用 `done(...)` 输出断言统计。
2. 覆盖接口约定内的边界情况。算法应尽量与独立暴力、已知结果或另一种实现对拍，保留固定随机种子。
3. 运行完整验证，再构建 PDF。代码、测试或验证工具变更后，旧报告失效。

验证会拒绝没有对应测试的头文件、编译失败、运行失败，以及没有执行断言的测试组。构建与打包还会核对报告是否包含全部头文件和全部测试组，拒绝缺项、重复项、失败、过期或局部验证的报告。GitHub Actions 使用相同入口。

```bash
python3 scripts/verification.py                  # 快速检查头文件与测试的对应关系
python3 scripts/verify.py --suite strings --skip-headers  # 开发时只测字符串
```

局部结果写入 `build/tests/report.partial.json`，不能代替完整验证。完整报告在 `tests/report.json`；构建后的 `output/coverage.json` 可查询每个收录条目对应哪些测试组。

这些检查保证的是**有明确测试、测试确实运行且通过**，不是所有输入上的正确性证明。直接 `include` 也不能自动证明断言充分覆盖了该模块；审阅时仍要检查测试内容，并确认复杂度、整型范围、递归栈、浮点误差和算法使用前提。

## 修改这本书

- 修改模板：编辑 `content/<章节>/*.h`，补上对应测试。
- 调整收录：编辑 `content/<章节>/chapter.tex`；章节顺序在 `BlueLagoon.tex`。
- 修改中文速查：编辑 `content/<章节>/*.md`。
- 修改版式：编辑 `tex/`；不要直接修改自动生成的 `build/`。

文件头格式、出版流程和维护命令见 [CONTRIBUTING.md](CONTRIBUTING.md)。第三方代码保留各自的来源与许可证，详见 [SOURCES.md](SOURCES.md)。
