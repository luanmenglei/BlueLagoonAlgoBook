提交编译：g++ main.cpp -std=c++20 -O2 -Wall -Wextra -o main
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
