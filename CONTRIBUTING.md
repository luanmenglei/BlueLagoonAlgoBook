# 维护指南

## 新增或修改模板

1. 在 `content/<章节>/` 新建或修改 `.h`，注明接口前提、复杂度、来源和许可证。
2. 在 `tests/test_*.cpp` 中直接 `#include "../content/<章节>/<文件>.h"`，编写运行断言。可以扩展现有测试组，不要求一个头文件一个测试文件。
3. 在对应 `chapter.tex` 中加入 `\blimport{文件.h}`。暂不打印可注释掉该行，但仍须通过测试覆盖检查和独立编译。
4. 执行完整验证与 PDF 构建，查看更新后的页面，再提交源码和需要更新的打印快照。

测试不应只有 `include` 或 `CHECK(true)`。应调用被测接口，用固定答案、独立暴力或另一种实现核对结果，并覆盖接口约定内的空集、最小规模、重复值、边界值和多次调用等适用情况。随机种子统一为 `712367`。性能规模与适用条件需要单独审阅，不能由小规模对拍推出。

基础声明、程序入口等不适合算法对拍的模块也须有针对性检查，例如类型宽度、输入读取和多测行为，不设免测名单。

### 文件头

```cpp
/**
 * @title 功能名称（常用算法名）
 * @brief 求解对象、接口效果和返回结果。
 * @note 下标、数据范围、特殊约束和使用前提。
 * @usage 调用示例（可选）
 * @time O(n log n)
 * @depends contest/Base.h
 * @author 作者及改编者
 * @source 原始链接或实现说明
 * @license 原始许可证
 */
```

除 `@usage` 外，上面的字段均必填；可另写 `@status`。`@depends` 列出全部本地 include 路径，以 `content/` 为根，用逗号分隔；无依赖写 `-`。缺字段、重复字段、未知标签或依赖不一致会阻止构建。

标题、用途、约定、复杂度、来源与依赖直接从文件头生成，不在目录中重复维护。保持已有 C++ 风格：tab 缩进、`i64`、同行大括号、`i ++`；算法正文尽量适应双栏行宽。

### 测试与出版校验

`python3 scripts/verify.py` 顺序执行：

- 检查章节、元数据、依赖和每个头文件的直接测试引用。
- 运行维护工具检查及 `tests/test_*.py` 回归测试。
- 独立编译全部头文件，运行全部 `tests/test_*.cpp`。
- 要求每个测试组成功退出，并通过 `done(...)` 报告至少一次实际执行的 `CHECK(...)`。
- 保存完整结果、测试对应关系和输入 SHA-256 到 `tests/report.json`。

直接测试引用是“由哪个测试组负责”的约定，不是语义覆盖率或正确性证明。仅通过其他头文件间接包含不计入对应关系；测试仍要实际调用该模块并验证结果。

`build.py` 和 `pack.py` 共用出版校验：报告必须来自完整验证，代码、测试及验证工具指纹一致，全部头文件与全部测试组各出现一次且通过，测试对应关系也必须匹配。打包还核对 PDF 与排版输入是否一致。

筛选测试只产生 `build/tests/report.partial.json`，不刷新完整报告；如果局部测试失败，会撤销已有完整报告。原始文件变更会使旧完整报告自然失效。

```bash
python3 scripts/verification.py
python3 scripts/verify.py --suite number --skip-headers
python3 -m unittest discover -s tests -p 'test_*.py'
python3 scripts/verify.py
python3 scripts/build.py
```

## 构建环境

需要 Python 3.10+、GNU g++（C++20）和 `requirements.txt` 中的 Pygments。排版需要 XeLaTeX。Ubuntu 24.04 可使用与 CI 一致的依赖：

```bash
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
  texlive-xetex texlive-latex-extra texlive-lang-chinese texlive-fonts-recommended \
  fonts-noto-cjk lmodern tex-gyre fonts-texgyre fonts-texgyre-math
python3 -m pip install -r requirements.txt
```

Windows 使用 SimSun；其他系统使用 Noto Serif CJK SC / Noto Sans Mono CJK SC。数学字体为 TeX Gyre Termes Math。格式化额外需要 clang-format 21。

```bash
python3 scripts/verify.py --compiler g++ --jobs 4
python3 scripts/build.py
python3 scripts/pack.py
```

编译器也可通过 `CXX` 指定。Windows 使用 `./build.ps1` 完成验证和构建，`./build.ps1 check` 只验证，支持 `PYTHON` 环境变量指定解释器。

## 目录和排版

| 路径 | 用途 |
| --- | --- |
| `BlueLagoon.tex` | 章节顺序和手工分页 |
| `book.json` | 队名、语言标准、版本日期 |
| `content/<章节>/chapter.tex` | 本章收录条目及顺序 |
| `content/<章节>/*.h` | 算法实现与文件头说明 |
| `content/<章节>/*.md` | 中文速查文档 |
| `tests/` | 算法、工具测试和完整报告 |
| `scripts/` | 验证、排版、依赖展开和打包工具 |
| `tex/` | 字体、版式、条目命令和封面 |
| `build/` | 可重新生成的中间文件 |
| `output/` | PDF、目录、短码与测试对应关系 |
| `third_party/` | 上游原件、归属信息和许可证 |

章节收录示例：

```tex
\section{网络流与匹配}
\blimport{Dinic.h}
\blimport{HopcroftKarp.h}
% \blimport{Blossom.h}
```

速查文档使用 `\blnote{文档.md}{标题}`。导入命令须使用字面量路径，不要放进条件分支或自定义宏。取消某个依赖的收录但仍收录其使用者时，构建会报错。

速查文档及文件头说明支持反引号代码、`$行内公式$` 和 `$$独立公式$$`。当前排版器按行转换，不是完整 Markdown 渲染器；复杂公式可用 `aligned` 分行，避免直接加入 Markdown 表格或代码围栏。普通文字会自动转义。

构建脚本生成 `build/entries/`、`build/print/` 后，调用 XeLaTeX 三遍生成目录、书签和总页数。它不会覆盖主文件、章节清单或版式。

## 其他维护命令

```bash
python3 scripts/build.py --tex-only               # 完整验证后只生成排版片段
python3 scripts/hash.py content/algebra/NTT.h     # 查询代码短码
python3 scripts/bundle.py algebra/FPS.h --output build/fps.cpp
python3 scripts/sync_metadata.py                  # 导出 output/catalog.json
python3 scripts/style.py                          # 格式化代码
python3 scripts/pack.py                           # 生成 ../BlueLagoon.zip
```

短码忽略注释和字符串外空白，保留 token 边界，只用于抄写核对；不验证依赖，也不证明算法等价。`output/coverage.json` 记录本次构建中各代码条目对应的本地测试组。

## 发布与来源

GitHub Actions 在 main 分支 push、pull request 和手动触发时运行；开发分支 push 由 PR 事件验证，避免同一提交重复构建。尚未开 PR 的分支可以手动运行。

流程分为两个任务：`C++20 verification` 完成全部代码验证并上传 `BlueLagoon-verification` 报告；成功后，`Printable PDF and portable package` 下载同一次运行的报告，先核对代码指纹与出版条件，再安装 TeX、构建 PDF 和打包。排版失败时可以使用 GitHub 的 **Re-run failed jobs**，无需重复已经通过的代码测试。

测试任务总时限为 10 分钟，其中完整验证步骤上限 8 分钟；排版任务总时限为 15 分钟，其中依赖安装上限 8 分钟、PDF 构建上限 3 分钟。TeX 安装使用 Ubuntu 官方主镜像替换 runner 中的 Azure 镜像，APT 网络等待设为 30 秒、最多重试 2 次。网络或软件源异常仍可能失败，但不会一直等到原来的 30 分钟总上限；失败后检查步骤日志和诊断附件。

`BlueLagoon-print` 附件包括打印文件和验证报告，`BlueLagoon-portable` 包括便携源码包；排版诊断附件还保留 `apt-install.log`，方便区分依赖下载、字体和排版问题。

`output/BlueLagoon.pdf` 是提交的打印快照；需要更新它时，先完整验证、构建并检查页面。报告、生成的 JSON 和排版中间文件不进入 Git。只改速查文档或排版时，若已有与当前代码一致的完整报告，可以直接重建；新 clone 仍须先验证。

第三方代码按各自许可证保留归属，不给整个仓库统一替换许可证。来源见 [SOURCES.md](SOURCES.md)、`sources.lock.json` 和 `third_party/`；功能覆盖审计见 [COVERAGE_AUDIT.md](COVERAGE_AUDIT.md)。
