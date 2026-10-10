整除分块：从 $l=1$ 开始，令 $r=\lfloor n/\lfloor n/l\rfloor\rfloor$，则 $[l,r]$ 内 $\lfloor n/i\rfloor$ 不变；下一段从 $r+1$ 开始。
双变量：令 $a=\lfloor n/l\rfloor$、$b=\lfloor m/l\rfloor$，区间右端点为
$$r=\min\!\left(\left\lfloor\frac{n}{a}\right\rfloor,\left\lfloor\frac{m}{b}\right\rfloor\right),$$
循环到 $\min(n,m)$。
常用恒等式：
$$\sum_{d\mid n}\varphi(d)=n,\qquad\sum_{d\mid n}\mu(d)=[n=1],$$
$$\sum_{d\mid n}\mu(d)\frac{n}{d}=\varphi(n).$$
Möbius 反演：
$$F(n)=\sum_{d\mid n}f(d)\quad\Longleftrightarrow\quad f(n)=\sum_{d\mid n}\mu(d)F\!\left(\frac{n}{d}\right).$$
互质对：$1\le x\le n$、$1\le y\le m$ 且 $\gcd(x,y)=1$ 的数量为
$$\sum_{d=1}^{\min(n,m)}\mu(d)\left\lfloor\frac{n}{d}\right\rfloor\left\lfloor\frac{m}{d}\right\rfloor.$$
GCD 权值：$*$ 表示 Dirichlet 卷积，
$$h(\gcd(x,y))=\sum_{\substack{d\mid x\\d\mid y}}(h*\mu)(d).$$
阶乘质因子指数：
$$v_p(n!)=\sum_{k\ge1}\left\lfloor\frac{n}{p^k}\right\rfloor.$$
欧拉降幂：仅在 $\gcd(a,m)=1$ 时直接使用
$$a^b\equiv a^{\,b\bmod\varphi(m)}\pmod m.$$
不互质时不能无条件套用。
