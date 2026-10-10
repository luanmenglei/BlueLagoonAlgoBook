Burnside：群 $G$ 作用下的轨道数为
$$N=\frac{1}{|G|}\sum_{g\in G}\operatorname{fix}(g).$$
模运算除以 $|G|$ 要求可逆，否则使用整数或更高模数计算。
Pólya：置换 $g$ 的长度 $j$ 循环数为 $c_j(g)$，循环指标为
$$Z_G(x_1,x_2,\ldots)=\frac{1}{|G|}\sum_{g\in G}\prod_{j\ge1}x_j^{c_j(g)}.$$
$m$ 色染色时，各 $x_j$ 代入 $m$。
Catalan：$C_0=1$，
$$\begin{aligned}C_n&=\frac{1}{n+1}\binom{2n}{n}\\&=\binom{2n}{n}-\binom{2n}{n-1}.\end{aligned}$$
第二类 Stirling：$S(0,0)=1$，
$$S(n,k)=S(n-1,k-1)+kS(n-1,k).$$
第一类无符号 Stirling：$c(0,0)=1$，
$$c(n,k)=c(n-1,k-1)+(n-1)c(n-1,k).$$
Bell：$\displaystyle B_n=\sum_{k=0}^{n}S(n,k)$。
错排：$D_0=1$，$D_1=0$，
$$D_n=(n-1)(D_{n-1}+D_{n-2}).$$
分拆数：$P_0=1$，负下标为 $0$。令广义五边形数 $g_k^{\pm}=k(3k\pm1)/2$，则
$$P_n=\sum_{k\ge1}(-1)^{k-1}\bigl(P_{n-g_k^-}+P_{n-g_k^+}\bigr).$$
Cayley：$n\ge2$ 个有标号点的树为 $n^{n-2}$；Prüfer 码长度为 $n-2$。
Pick：简单格点多边形面积 $A=I+B/2-1$；有 $h$ 个孔时 $A=I+B/2-1+h$，边界计数包括所有边界。
格点线段的边界间隔数为 $\gcd(|\Delta x|,|\Delta y|)$；简单格点多边形满足
$$B=\sum_{\text{edges}}\gcd(|\Delta x|,|\Delta y|).$$
Euler：连通平面图 $V-E+F=2$（含外面）；$c$ 个连通块时 $V-E+F=1+c$。
Knuth 充分条件：$a\le b\le c\le d$ 时，
$$\begin{aligned}w(b,c)&\le w(a,d),\\w(a,c)+w(b,d)&\le w(a,d)+w(b,c).\end{aligned}$$
Monge：以下不等式推出全单调与最小决策单调。
$$\begin{aligned}A(i,j)+A(i+1,j+1)\\{}\le A(i,j+1)+A(i+1,j).\end{aligned}$$
常用求和：
$$\sum_{i=1}^{n}i=\frac{n(n+1)}{2},\qquad\sum_{i=1}^{n}i^2=\frac{n(n+1)(2n+1)}{6}.$$
内存：`int[N]` 约 $4N$ 字节，`i64[N]` 约 $8N$；`vector` 有额外容量和对象开销。邻接表双向边算两份。
复杂度预算以考场机器测量为准；大常数、随机算法与哈希不能只看渐进式。
