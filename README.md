# BlueLagoon ICPC Team Reference

[![Verify and print](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml/badge.svg)](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml)

[查看／下载已提交的打印版](output/BlueLagoon.pdf) · [自动构建与下载](https://github.com/luanmenglei/BlueLagoonAlgoBook/actions/workflows/verify.yml) · [来源与许可证](SOURCES.md)

GNU C++20；A4 竖版双栏；代码 8.5pt，行距 10.2pt，tab 按 4 字符展开，与源码对齐。版面参考原始 `teamnote.pdf`，代码按队员习惯使用 tab、`i64`、同行大括号和 `i ++`。142 个代码条目、8 个速查条目，分为 12 章。打印文件是 **`output/BlueLagoon.pdf`**，建议按实际大小打印，长边翻转；行折叠箭头表示同一行的续行。关键词及函数名使用深色加粗，注释使用中灰色斜体；数字、字符串用深色区分，灰度打印仍靠字重和字形区分。

第10章为“常用补充”，第11章为“进阶附录”，第12章为“性能工具与卡常技巧”。各章用 `chapter.tex` 中的导入行选择板子，注释掉对应行即可不印入书中。新增板子带调用例；第11章包含重型算法，各段给出的规模和适用前提仍是使用条件。

第12章提供 Barrett／Montgomery、受限范围的 x86 除法、位运算、开放寻址哈希、内存池，以及带 CPU 检测的 AVX2 数组异或和展开回退；选用前提与测试方法见 `content/performance/Guide.md`。这些工具按需使用，未替换原有数论与 NTT 实现。

第7章的 `content/algebra/TransformsGuide.md` 给出 FWT／FMT 的定义、逆变换、蝶形公式与子集卷积分层公式，并说明如何用已有的二元高斯消元求异或线性空间的正交补。

2026-10-18 增补 Segment Tree Beats（chmin/chmax/add 与和、最值查询）、固定端点必经点／边及最短路必经关系、无向严格正权图独立删边最短路。图论选型说明见 `content/graph/PathGuide.md`；必经关系使用支配树精确求解，不依赖哈希。

2026-10-10 起，目录使用功能名称或常用名称，必要时括注算法名；全部代码条目印有“功能”说明。标题、功能、约定和复杂度直接读取 `.h` 文件开头的注解，目录与 PDF 书签使用同一标题。

## 构建

```sh
git clone https://github.com/luanmenglei/BlueLagoonAlgoBook.git
cd BlueLagoonAlgoBook
```

需要 Python 3.10+、GNU g++（支持 C++20）、XeLaTeX（TeX Live，含 xeCJK、fvextra、titlesec、unicode-math 等常见包）。Windows 使用宋体 SimSun；其他系统使用 Noto Serif CJK SC / Noto Sans Mono CJK SC。数学字体为 TeX Gyre Termes Math。格式化另外需要 clang-format 21。算法测试脚本只用 Python 标准库；打印使用 Pygments 进行 C++ 词法高亮，不需要 shell-escape。

Ubuntu 24.04 的排版依赖与 CI 一致：

```sh
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
  texlive-xetex texlive-latex-extra texlive-lang-chinese texlive-fonts-recommended \
  fonts-noto-cjk lmodern tex-gyre fonts-texgyre fonts-texgyre-math
```

在项目根目录执行：

```sh
python -m pip install -r requirements.txt
python scripts/verify.py
python scripts/build.py
```

Windows 可用 `./build.ps1` 一次完成验证与 PDF 构建，或 `./build.ps1 check` 仅验证。脚本支持 `PYTHON` 环境变量指定解释器；编译器用 `CXX` 指定。Linux 可用 `make check pdf`。首次构建运行 XeLaTeX 三遍，生成完整目录、交叉引用和总页数。

构建先把文件头、速查文档和高亮代码转换为 `build/entries/` 与 `build/print/` 中的片段，再编译手工维护的 `BlueLagoon.tex`。主文件、章节清单、版式和封面不会被构建脚本覆盖。`--tex-only` 只准备中间片段，随后也可以直接运行 `xelatex -output-directory=build BlueLagoon.tex`。

```sh
python scripts/verify.py --suite number --skip-headers  # 开发时只跑数论
python scripts/verify.py                               # 出版前必须完整验证
python scripts/build.py --tex-only                     # 只生成 LaTeX 与打印片段
python scripts/hash.py content/algebra/NTT.h            # 对照书上 6 位短码
python scripts/bundle.py algebra/FPS.h --output build/fps.cpp
python scripts/pack.py                                 # 生成 ../BlueLagoon.zip
```

`bundle.py` 按源码中的本地 include 递归展开依赖并去重，保留作者及来源注释。选取算法后追加自己的 `solve()`/`main()`；也可加入 `contest/Template.h`。只展开实际需要的模块：书中不同章节的全局函数名可能相同，全部模块合成一个翻译单元不是支持的用法。

## 维护

- **主文件 `BlueLagoon.tex`** 只维护章节顺序和可选的手工分页。队名、语言版本与版本日期在 `book.json`。
- **章节清单 `content/<章>/chapter.tex`** 用 `\section{章名}`、`\blimport{文件.h}`、`\blnote{文档.md}{标题}` 维护顺序与收录。注释、取消注释或移动导入行即可；可直接插入 LaTeX 正文、公式、`\columnbreak` 和 `\newpage`。导入命令使用字面量路径，不要放进条件分支或自定义宏。
- **算法正文与条目信息直接修改 `content/**/*.h`**；打印与测试读取同一份文件。文件开头的 `/** ... */` 注解是代码条目信息的唯一维护来源。
- **版式 `tex/preamble.tex`** 管字体、纸张、双栏、高亮和行距；**导入／条目命令 `tex/commands.tex`** 管统一条目格式；**封面与目录 `tex/frontmatter.tex`** 管首页。自动生成的片段只在 `build/` 中，不直接修改。
- 速查文档及条目说明支持 `$行内公式$`、`$$独立公式$$` 与反引号代码；独立公式写在单独一行，过长公式用 `aligned` 分行。普通文字仍自动转义。复杂度中的 `O(...)` 自动转成数学排版。
- `python scripts/sync_metadata.py` 读取文件头，导出 `output/catalog.json` 供检索；它不会覆盖文件头。`python scripts/style.py` 负责代码格式化，保护字符串、注释和 include 文件名。
- 格式化行宽为 56 字符，适配双栏 8.5pt 代码；源码和打印都按 4 字符 tab 对齐，尽量避免标识符被自动换行拆开。
- 新模块在 `tests/test_*.cpp` 中增加边界用例，以及与独立暴力/其他算法比较的随机测试。`scripts/validate.py` 检查章节导入、重复条目、依赖声明与环；所有 `.h` 都参加独立编译，包括暂未印入书中的代码。
- 修改算法、文件头或验证工具后完整执行 `verify.py`，再 `build.py`。报告保存代码与测试输入的 SHA-256；这些输入变化后，旧报告不能用于出版。只调整版式、章节顺序或收录选择时可直接重建 PDF，构建会重新检查导入和可打印依赖。筛选测试适合开发，不能刷新完整出版凭证。
- `output/hashes.json` 是本版短码；`output/coverage.json` 列出直接引用该模块的本地测试组。短码忽略注释和字符串外的空白，保留 token 边界；不用于验证依赖或判断两个算法等价。PDF 中依赖需递归收齐。
- GitHub Actions 自动运行目录检查、独立编译、15 组回归，以及 PDF 构建，并上传打印文件。执行时间较长时可减少 `verify.py --jobs`，默认 4。

文件头使用以下注解，`@usage` 可选，其他核心字段必填；长说明可续行。缺字段、重复字段、拼错标签或依赖与 include 不一致会阻止构建。标题写功能／常用名称，功能说明写清求解对象和返回结果。

```cpp
/**
 * @title 二分图最小权匹配（KM／Hungarian）
 * @brief 求覆盖左侧所有点的最小权匹配；两侧等大时为最小权完美匹配。
 * @note n<=m，0-based；允许负权，费用及中间运算不得溢出。
 * @usage auto [cost,match] = weightedMatching(a);
 * @time O(n^2 m)
 * @depends contest/Base.h
 * @author 原作者及改编者
 * @source 原始链接或实现说明
 * @license 原始许可证
 */
```

`@depends` 列出全部本地 include 路径（相对 content），包括公共声明；无依赖写 `-`。`@brief` 印为“功能”，`@note` 印为“约定”，`@usage` 印为“调用”，`@time` 印为“复杂度”；来源、许可证和依赖也由文件头自动读取。`@status` 可选，用于上游／本地验证状态说明。

来源记录见 [SOURCES.md](SOURCES.md)、`sources.lock.json` 与 `third_party/originals`；混合许可证按原文件保留，不给全部第三方代码统一改许可证。发布包仅包含维护需要的工具，初次导入用的 `prepare*.py` / `catalog.py` / `record_sources.py` 不参与重建。

这套组织方式参考 [KACTL 的主文件、chapter.tex 与 kactlimport 工作流](https://github.com/kth-competitive-programming/kactl#customizing-kactl)。例如网络流与匹配章可以这样维护：

```tex
\section{网络流与匹配}
\blimport{Dinic.h}
\blimport{HopcroftKarp.h}
\blimport{WeightedMatching.h}
% \blimport{Blossom.h} % 暂不打印，但仍参加独立编译
```

代码标题与说明由文件头生成，无需在章节清单再次填写。取消收录被其他模板依赖的文件时，构建会报出缺失的可打印依赖。上游历史记录集中在 `third_party/attribution.json`；它只保存追溯信息，不控制排版或收录。

## GitHub 工作流

每次 push、pull request 或手动运行 Actions，都会完整验证 142 个头文件和 15 组测试，再构建 PDF 与便携源码包。打开上面的自动构建链接，选取对应提交的成功运行，在 **Artifacts** 中下载：`BlueLagoon-print` 包含 PDF、目录、短码、覆盖信息和测试报告；`BlueLagoon-portable` 包含可重新构建的 `BlueLagoon.zip`。下载 Actions 产物需要登录 GitHub，产物保留 90 天；构建失败时另外保存诊断日志。

仓库里的 `output/BlueLagoon.pdf` 是提交时的打印快照，Actions 的 PDF 对应各次提交。准备更新仓库快照时，在本地完整验证、构建后一起提交源码和 PDF。测试报告、排版中间文件与生成的 JSON 不进入 Git；从新 clone 的仓库构建时先运行 `verify.py`。`.gitattributes` 统一源码为 LF，`.editorconfig` 保留 C++ 的四字符 tab；GitHub Actions 的依赖更新由 Dependabot 每月检查。

## 验证范围

与三份原始 PDF、cp-book、koosaga Library 和 KACTL 的功能差异见 [COVERAGE_AUDIT.md](COVERAGE_AUDIT.md)。当前是选集，编译与对拍通过不代表参考资料已经完整收录；审计区分缺失、部分覆盖和替代实现，并列出建议补充顺序。

本机以 g++ 10.3.0、`-std=c++20 -O2 -Wall -Wextra` 完成 142 个头文件的独立编译，随机种子固定为 712367。15 组测试覆盖数据结构、图、流与匹配、字符串、数论、代数、几何、杂项；断言数量与结果见 `tests/report.json`。测试包括小规模穷举和随机对拍，例如一般图匹配的子集 DP、支配树的删点可达性、最小割的点集枚举、组合数的 Pascal 表、半平面交的逐边裁剪、DP 优化的朴素转移。

测试规模有限；复杂度上界、递归栈、整型乘法和浮点误差需要依题目确认。每段正文给出的前提是接口的一部分，例如三维凸包要求任意 4 点不共面，最小费用流要求无可达负费用环，半平面交使用显式包围盒。基础声明与主程序骨架只有独立编译标记；继承的上游测试记录不等同于本地对拍。

## 文件

```text
BlueLagoon.tex              手工维护的简短入口与章节顺序
book.json                   队名、标准、日期
content/<章>/chapter.tex    手工维护的章节正文、导入顺序与收录选择
content/<章>/*.h            算法正文与文件头注解（唯一维护来源）
content/<章>/*.md           环境、公式、约定速查
tests/                     独立暴力与随机测试、报告
scripts/                   验证、排版、短码、依赖展开、打包
tex/preamble.tex            字体、纸张、页眉、双栏、代码排版
tex/commands.tex            章节导入与统一条目排版命令
tex/frontmatter.tex         封面、说明与目录
output/                    可打印 PDF、短码、覆盖列表
build/                     自动生成的中间文件，可删除后重建
third_party/               对应版本原件与许可证文本
```
