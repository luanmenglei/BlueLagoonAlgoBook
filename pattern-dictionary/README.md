# BlueLagooon常见题目模式字典+图灵奖速查表

独立的比赛参考册，现有 **157 条**，按 **M → D → N → C → Q → I** 排列。正文用问题形态描述，离开原题和网页也能读懂。

[单列打印源](dictionary.tex) · [可编辑正文](dictionary.md) · [离线检索](index.html) · [公式推导](research/derivations.md) · [来源记录](research/sources.md)

每条先给常见说法作为检索入口，再写精确对象、操作、限制与可用做法；加粗突出必要条件和容易混淆的区别。题面索引按模糊线索找编号，完整目录列出全部条目，内部编号保持稳定。

计数部分分两层：先看 **“先判断能不能数”** 与模型／结构／复杂度速览，再看 **“组合计数与多项式模式”**。本版补充经过指定点的格路、少量禁点格路、反射与投票、卡特兰模型、同上下限球盒、嵌套集合单射分配六类组合模型，并保留十二重计数表、斯特林行列计算表与有向链递推。

本目录独立维护、构建和打印，不需要加入主书的章节清单或构建入口。排版参考主书的 A4 竖版双栏、TeX Gyre Termes 字体和功能名称目录；打印使用实际大小，双面打印选长边翻转。

## 离线使用

直接用浏览器打开 `index.html`。搜索覆盖关键词、标题与正文，支持空格分隔的多词查询、常用同义词和板块筛选；点击条目前的关键词也能检索。没有直接结果时才尝试严格近似匹配，并提示核对模型条件。

正文、公式和计算表均可离线阅读，请保留同目录的 `assets/`。来源默认折叠，不参与正文检索；展开后可核对资料，访问外部论文或原题链接需要联网。

## 维护与重建

正文的唯一维护源为 `source/entries.json`：

- `keywords`：宽泛检索词；`text`：精确正文；`highlights`：正文中加粗的原样短语。
- `derivation`：较长公式与推导；`references`：来源记录。
- `shape_index`：题面索引；`counting_parts`：计数两层分组；`counting_overview`：可行性速览。
- `print_tables`：两张计算表及链递推。**`source/tables.tex`、`source/tables.md` 由此自动生成，不单独修改。**

```sh
cd pattern-dictionary
python3 scripts/build.py --pdf
node scripts/build-html.cjs
```

Python 仅用标准库。PDF 需要 XeLaTeX 或 Tectonic；优先使用宋体或 Noto Serif CJK SC，另提供 macOS Songti 与 Fandol 回退。HTML 生成需要 Node.js，KaTeX 与字体已随附，无需安装依赖或联网。具体写作和核验要求见 [MAINTENANCE.md](MAINTENANCE.md)。

## 文件

- `dictionary.pdf`：打印版，含题面索引和完整目录。
- `dictionary.md` / `dictionary.tex`：生成的可读正文和独立排版源。
- `index.html` / `assets/`：离线检索页、公式样式及字体。
- `source/entries.json`：条目、关键词、来源、索引与计算表的维护源。
- `source/tables.md` / `source/tables.tex`：生成的计算表。
- `scripts/`：本册独立生成工具。
- `research/`：来源、长公式、修订说明及核验记录。

复杂度按相应条件下的赛场可用实现估算。新增公式和关键转化的核验范围见研究记录，题库提交记录不能代替正确性说明。
