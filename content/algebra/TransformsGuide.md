约定：下标是 $k$ 位集合，$N=2^k$，$|S|$ 表示置位数，$\langle S,T\rangle=|S\cap T|\bmod 2$。Transforms.h 的系数模 $998244353$，输入长度必须为非零的二次幂，系数先归一化到 $[0,998244353)$。这里 FMT 指子集 Zeta／Möbius 变换，不同资料的命名可能不同。

XOR 变换（Walsh–Hadamard）：令 $\widehat a=H(a)$，则
$$\widehat a[S]=\sum_{T\subseteq U}(-1)^{\langle S,T\rangle}a[T],\qquad U=\{0,\ldots,k-1\}.$$
$$a[S]=N^{-1}\sum_{T\subseteq U}(-1)^{\langle S,T\rangle}\widehat a[T].$$
其中 $H^2=NI$。每层把当前位为 0、1 的一对值 $(u,v)$ 变成 $(u+v,u-v)$；逆变换可每层乘 $2^{-1}$，也可像代码一样重复正变换后统一乘 $N^{-1}$，两种归一化不能同时做。接口：`fwt(a,0)`／`fwt(a,0,true)`。XOR 逆变换要求 2 可逆，本模板的奇素数模满足条件。

OR 变换／子集 FMT：正变换是子集 Zeta，逆变换是子集 Möbius。
$$\widehat a[S]=\sum_{T\subseteq S}a[T],\qquad a[S]=\sum_{T\subseteq S}(-1)^{|S|-|T|}\widehat a[T].$$
每层正变换 $(u,v)\mapsto(u,u+v)$，逆变换 $(u,v)\mapsto(u,v-u)$，不需要除以 $N$。接口：`fwt(a,1)` 与 `sos(a)` 等价；`fwt(a,1,true)` 与 `sos(a,true)` 等价。也就是枚举位 $b$，对含 $b$ 的 $S$ 执行 $a[S]\mathrel{+}=a[S\setminus\{b\}]$，反演时改为减法。

AND 变换／超集 FMT：正变换是超集 Zeta，逆变换是超集 Möbius。
$$\widehat a[S]=\sum_{T\supseteq S}a[T],\qquad a[S]=\sum_{T\supseteq S}(-1)^{|T|-|S|}\widehat a[T].$$
每层正变换 $(u,v)\mapsto(u+v,v)$，逆变换 $(u,v)\mapsto(u-v,v)$，不需要归一化。接口：`fwt(a,2)`／`fwt(a,2,true)`。所有蝶形公式中的 $u,v$ 均为更新前的值；减法在模意义下处理。

位运算卷积：取 $\circ\in\{\oplus,\cup,\cap\}$，分别对应 XOR、OR、AND。若 $\mathcal T$ 是对应的变换，则
$$c[S]=\sum_{A\circ B=S}a[A]b[B],\qquad c=\mathcal T^{-1}(\mathcal T(a)\cdot\mathcal T(b)).$$
乘号表示逐项相乘。调用顺序是分别变换 a、b，执行 `a[i]=i64(a[i])*b[i]%mod`，再逆变换 a。每次变换时间 $O(k2^k)$、额外空间 $O(1)$。OR 允许 $A\cap B\ne\varnothing$，不能直接替代下面的不交子集卷积。

不交子集卷积：`convolution(a,b)` 求 $c[S]=\sum_{T\subseteq S}a[T]b[S\setminus T]$。按集合大小分层再做子集 FMT：
$$A_i[S]=\sum_{\substack{T\subseteq S\\|T|=i}}a[T],\qquad B_i[S]=\sum_{\substack{T\subseteq S\\|T|=i}}b[T].$$
$$C_r[S]=\sum_{i=0}^r A_i[S]B_{r-i}[S],\qquad c[S]=(\mu C_{|S|})[S].$$
这里 $\mu$ 是子集 Möbius 逆变换。反演保证并集恰为 $S$，再取层数 $|S|$ 保证两集合不交。时间 $O(k^2 2^k)$，空间 $O(k2^k)$。参考 Björklund 等的 Fourier meets Möbius: fast subset convolution（arXiv:cs/0611101）；链接见 SOURCES.md。

线性基的正交补：在 $\mathbb F_2^k$ 中，把异或值看作位向量，内积为 $\langle x,y\rangle=\operatorname{popcount}(x\mathbin{\&}y)\bmod 2$。给定子空间 $V$，所求通常是
$$V^\perp=\{y:\ \langle x,y\rangle=0\ \text{对所有 }x\in V\},\qquad \dim V^\perp=k-\dim V.$$
XorBasis.h 没有专用的正交补接口，`reduced()` 只做消元化简。但 supplement/GaussBinary.h 已能求齐次解空间：把 V 的生成元逐行作为系数，右侧全填 0，调用 `gauss_binary(rows,k)`，返回的 `basis` 就是正交补的一组基。每行必须有 $\lceil(k+1)/64\rceil$ 个 64 位块；k=64 时应把生成元 x 编为 `{x,0}`，第 64 位是右端项。返回向量的第 j 项对应原掩码的第 j 位。

正交补中的向量均与 V 正交，但这组基的向量之间不保证两两正交。例如在 $\mathbb F_2^2$ 中，$V=\operatorname{span}\{11_2\}$ 满足 $V^\perp=V$，所以不能套用实数空间的正交直和结论。实数向量的 Gram–Schmidt 正交化是另一件事，当前库没有专门实现。

与 FWT 的联系：令 $1_V$ 为子空间 V 的示性函数，则
$$H(1_V)[S]=\begin{cases}|V|,&S\in V^\perp,\\0,&S\notin V^\perp.\end{cases}$$
若 $S\notin V^\perp$，取 $v\in V$ 使 $\langle S,v\rangle=1$，将 x 与 $x\oplus v$ 配对，符号恰好抵消。这能解释为何异或线性空间计数会同时出现 FWT 与正交补；求正交补本身只需消元，无须枚举 $2^k$ 个状态。
