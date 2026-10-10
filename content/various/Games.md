无环正常玩法：
$$\operatorname{SG}(x)=\operatorname{mex}\{\operatorname{SG}(y):x\to y\}.$$
无合法操作时 $\operatorname{SG}(x)=0$；独立子游戏的和对应 SG 异或。
SG定理要求无环／终止且是无偏游戏；循环游戏可能存在平局，需反向拓扑判 win/lose/draw。
普通 Nim：所有堆大小异或非零则先手胜；取最后一个获胜。
Misère Nim（取最后一个输）：若全为1，堆数偶数先手胜；否则按普通Nim。
Wythoff：排序 $a\le b$，令 $k=b-a$；败态满足
$$a=\lfloor k\phi\rfloor,\qquad\phi=\frac{1+\sqrt5}{2}.$$
大整数须防浮点取整误差。
减法游戏先计算足够多SG；观察周期只是猜测，需要证明或适用的周期定理。
有向无环图SG：先求拓扑序，逆序计算，mex 使用时间戳清理。
