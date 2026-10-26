提交编译：g++ main.cpp -std=c++20 -O2 -Wall -Wextra -o main
开赛速配：在 VS Code 的 Bash 终端输入下面一行，定义 crun（仅当前终端有效；长期使用可加入 ~/.bashrc 后执行 source ~/.bashrc）。
`crun(){ g++ -std=c++20 -O2 "$1.cpp" -o "$1.run" && "./$1.run"; }`
先保存 a.cpp，并切到文件所在目录；参数写 a，不带 .cpp。编译失败时不会运行旧程序。
手动输入：`crun a`；文件输入：`crun a < a.in`；文件输入输出：`crun a < a.in > a.out`。
输出文件会被覆盖；编译错误和 cerr 仍显示在终端。Ctrl+C 中止运行；换终端需重新定义。
本地调试：g++ main.cpp -std=c++20 -O1 -g -D_GLIBCXX_DEBUG -fsanitize=address,undefined -o main
调试选项依考场安装情况使用；最终提交移除 DEBUG 和重定向。
Linux/Bash 对拍：for ((s=1;;s++)); do ./gen "$s" > in; ./main < in > out; ./brute < in > ans; diff -w out ans || break; done
生成器接受种子 s；保存失败输入，先排除暴力程序本身错误。
Vim：set nu ts=4 sw=4 noet autoindent | syntax on
Vim 编译：:w | !g++ % -std=c++20 -O2 -Wall -Wextra -o %:r
Vim 运行：:!./%:r < in
__builtin_ctzll(x)、clzll(x) 要求 x != 0；移位量必须小于位宽。
枚举子集：for (int s = m; s; s = (s - 1) & m)；零子集单独处理。
枚举全集所有子集共 $\mathcal{O}(3^n)$；SOS DP 为 $\mathcal{O}(n2^n)$。
检查乘法是否先转 i64／i128；INF 加边权前确认不会溢出。
浮点输出设精度；比较误差按量纲和规模选择；几何中优先整数谓词。
DFS 深链注意栈；大数组注意内存；比赛开始确认编译器、时间限制和可用调试工具。
