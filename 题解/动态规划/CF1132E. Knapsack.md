https://codeforces.com/contest/1132/problem/E

**题意**
有数字 $1 \sim 8$ ，给出每种数字的个数。
选出若干个数字，要求这些数字的和最大且不超过 $W$ ，求最大值。
$0 \le cnt \le 10^{16}, 0 \le W \le 10^{18}$ 。

**分析**
#DP #背包
因为数字权值只有 $1 \sim 8$ ，而个数很大，所以有很多组合，**贡献相同但是组合数特别多**。
我们考虑把这种贡献相同的组合剥离出来。
$LCM([1, \cdots , 8]) = 840$ ，那么 $840$ 个 $1$ 的组合，与 $420$ 个 $2$ 的组合，与 $120$ 个 $8$ 的组合是一样的。
我们把每种数字产生的 $840$ 的个数剥离出来，这样每种数字最多只会产生 $840-i$ 的贡献。
**我们设 $dp(i, j)$ 表示前 $i$ 种数字，产生了 $j$ 的贡献时，每种数字产生的 $840$ 的个数和。**
那么，对于每一个数字，我们只需要枚举它用来产生贡献的个数即可，这个数量为 $LCM / i$ 。
假设对于当前数字，选择了 $k$ 个作为贡献($0 \le k \lt LCM/i$)，那么会产生 $(cnt_i - k) / LCM$ 个 $LCM$ 。
也就是状态转移方程为：
$dp(i, j+k\times i) = \max {(dp(i-1, j) + (cnt_i - k) / LCM}) \quad (0 \le j \lt LCM \times 8)$    
那么在枚举答案时，我们去枚举产生的贡献，和对应贡献下，产生的 $LCM$ 的个数，相加即可，注意答案要小于 $W$ 。

**Code** 
```c++
#include <bits/stdc++.h>
using namespace std;

using LL = long long;

const LL LCM_8 = 840, N = LCM_8 * 10 + 20;
LL W, cnt[9]; // 有十种数字
LL dp[9][N<<1]; // dp(i, j) 前i个种类，不考虑LCM情况下，构成总数为j时，产生的lcm数量
// LCM([1, ... , 10]) = 2520
// 注意这里的LCM只考虑单个的情况，即 2 * 1260 = 2520, 1260个2构成一个lcm
// 这样，每个数字最多会对总数产生 2519 个贡献

int main () {
    // int LCM = 1;
    // for (int i = 2; i <= 8; i ++ ) LCM = lcm(LCM, i);
    // cout << LCM << endl;
    memset(dp, -0x3f, sizeof dp); dp[0][0] = 0;
    cin >> W;
    for (int i = 1; i <= 8; i ++ ) cin >> cnt[i];
    for (int i = 1; i <= 8; i ++ ) {
        for (int j = 0; j < N; j ++ ) {
            // 枚举当前使用了几个贡献到总数中
            for (int k = min(cnt[i], LCM_8 / i - 1); k >= 0; k -- ) {
                dp[i][j+k*i] = max(dp[i][j+k*i], dp[i-1][j] + (cnt[i] - k) / (LCM_8 / i));
            }
        }
    }
    LL ans = 0;
    for (int i = 0; i < N; i ++ ) 
        ans = max(ans, i + (LL)LCM_8 * min(dp[8][i], W >= i ? (W - i) / LCM_8 : -N));
    cout << ans << endl;
    return 0;
}
```