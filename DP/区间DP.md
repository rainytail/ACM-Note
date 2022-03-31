# 区间DP

区间DP一般是转移是合并区间或者分解区间。

总结套路：https://www.luogu.com.cn/blog/BreakPlus/ou-jian-dp-zong-jie-ti-xie

① 将序列分为 K 个连续区间，求解这些区间的某个最优性质。

一般定义 $dp[i][k]$ 表示将 $a[:i]$ 分为 K 个连续区间得到的最优解

枚举最后一个区间的左端点 $j$，即最后区间为 $[j, i]$ ，从 $dp[j-1][k-1]$ 转移到 $dp[i][k]$ ，转移时考虑 $a[j:i]$ 对最优解的影响。

② 求解关于某个序列的最优性质，要求大区间的最优解可以依赖于小区间的最优解。

一般定义 $dp[i][j]$ 表示 $a[i:j]$ 的最优解

此时可以枚举区间大小 $[l, r]$ ，从小区间转移到大区间

一般是从 $[l+1, r], [l, r-1]$ 转移到 $[l, r]$ ，或者枚举中间断点 $k$ ，从 $[l, k], [k+1, r]$ 转移到 $[l, r]$ 。

____

## 一些题目

体育节  http://oj.daimayuan.top/course/10/problem/668

前缀最大值减去前缀最小值，设 $dp[i][j]$ 表示 $[i, j]$ 的最优解，把最值插入最后的位置，从 $[l+1, r], [l, r-1]$ 转移。

证明：https://zhuanlan.zhihu.com/p/490707329

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 2010;
int n, a[N];
long long dp[N][N];

int main ()
{
    cin >> n;
    for (int i = 1; i <= n; i ++ ) cin >> a[i];
    sort(a + 1, a + n + 1);
    for (int len = 2; len <= n; len ++ )
        for (int i = 1; i + len - 1 <= n; i ++ ) {
            int j = i + len - 1;
            dp[i][j] = a[j] - a[i] + min(dp[i+1][j], dp[i][j-1]);
        }
    cout << dp[1][n] << endl;
    return 0;
}
```



数组划分  http://oj.daimayuan.top/problem/665

将 $n$ 长度序列划分为 $k$ 个子数组，每个子数组求和按位与，求最大值。

**将搜索性问题转化为判定性问题**，即求出能否找出合法划分使得 “大于等于” 某个值。

$dp[i][len]$ 表示前 $i$ 个元素划分 $len$ 组是否合法。

证明：https://zhuanlan.zhihu.com/p/489360066

```c++
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 105;
ll n, k, a[N], pre[N], ans;
bool dp[N][N]; // 前i个元素分割为len块是否可行

ll get (int l, int r) {
    return pre[r] - pre[l - 1];
}

bool check (ll x)
{
    memset(dp, 0, sizeof dp);
    dp[0][0] = 1;
    for (int i = 1; i <= n; i ++ )
        for (int j = 1; j <= i; j ++ )
            if ((get(j, i) & x) == x) // sum[j, i] 不会使结果更差
                for (int len = 1; len <= k; len ++ )
                    dp[i][len] |= dp[j-1][len-1];
    return dp[n][k];
}

int main ()
{
    cin >> n >> k;
    for (int i = 1; i <= n; i ++ ) cin >> a[i];
    partial_sum(a + 1, a + n + 1, pre + 1);
    // 将搜索性问题转化为判定性问题
    for (int i = 63; i >= 0; i -- ) {
        ans |= 1ll << i;
        if (!check(ans)) ans ^= 1ll << i;
    }
    cout << ans << endl;
    return 0;
}
```

