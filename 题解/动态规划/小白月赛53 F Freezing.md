### F. Freezing
链接： https://ac.nowcoder.com/acm/contest/11230/F

### 题意
给出 $n$ 个 $m$ 长度的二进制字符串，问有多少个子序列满足：
+ 对于两个相邻的二进制字符串，相同的位置，值不同。
**其中**：$1 \le n \le 2 \times 10^5, 1 \le m \le 16$ 。 

#### 分析
#DP #拆位
首先看 $m$ 值比较小，想到状态压缩DP。
定义 $dp(st)$ 表示最后选中状态为 $st$ 时的方案数量，总方案数就是累加起来。
有转移方程：
$dp(st) = \sum dp(valid) \ if \ valid \& st = 0$ 。
总方案数：$\sum dp(st)$ 
复杂度为：$n$ 个阶段，每个阶段 $2^m$ 个决策，转移为 $O(1)$ ，因此为 $O(n \cdot 2^m)$ 

这样的复杂度是不够的，需要进行优化，题目提示 $n \cdot 2^{\dfrac m 2} \le 5.12 \times 10^7$ ，所以每个阶段应该只能有 $2^{\dfrac m 2}$ 个决策。
考虑将状态拆分高八位和低八位。
我们称 $hi$ 为 $i$ 状态的高八位，$li$ 为 $i$ 状态的低八位。
定义 $dp(hi, li)$ 为：最后选中状态的高八位为 $hi$ ，且低八位和 $li$ 交集为空的方案数。
转移方程：
$$
\begin{aligned}
z = \sum dp(j, li) \ if \ j \& hi = 0 \\
dp(hi, j) += z \ \ if \ j \& li = 0
\end{aligned}
$$
总方案数就是 $\sum z$ 。

转移方程第一句：
根据dp方程的定义，$dp(hi, li)$ 本身是低八位和li交集为空的方案数，只要枚举所有和 $hi$ 交集为空的，累加起来就是整个状态和 $i$ 的状态交集为空的方案数。

第二句：
$z$ 表示的就是当前求出来的方案数（准确说是以i状态为最后一个的方案数），因此根据dp定义，累加到 $dp(hi, j)$ 。

#### Code
```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 200010, M = 8, P = 998244353;
int n, m, a[N], dp[1 << M][1 << M]; // dp(hi, li) 表示当前队列最右状态高八位为hi，低八位和li没有交集的方案数

int main () {
    cin >> n >> m;
    for (int i = 1; i <= n; i ++ ) {
        for (int j = 1; j <= m; j ++ ) {
            char x; cin >> x;
            a[i] = (a[i] << 1) | (x == 'o');
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i ++ ) {
        int hi = a[i] >> 8, li = a[i] & 255, z = 1;
        for (int j = 0; j < (1 << 8); j ++ )
            if (!(j & hi)) (z += dp[j][li]) %= P;
        (ans += z) %= P;
        for (int j = 0; j < (1 << 8); j ++ )
            if (!(j & li)) (dp[hi][j] += z) %= P;
    }
    cout << ans << endl;
    return 0;
}
```