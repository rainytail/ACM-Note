# 数位DP

数位DP用来求计数相关的问题，通常给出区间 $[l, r]$ ，问在这个区间内满足某种性质的数的数量有多少个。

这类问题的代码比较板，放个代码先。

其中 $dp[i][0/1][j]$ 这类 $dp$ 数组是根据题目要求的性质的限制，通常前两个表示前 $i$ 个位置被固定了，且此时有/没有限制时，得到的满足性质的数字数量。

____

典中典题目：[不要62](https://www.acwing.com/problem/content/1087/)

**题意**：问 $[l, r]$ 区间内不含有 $4$ 和 $62$ 的数字的数量。

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 15;
int A[N], cnt, dp[N][2][N];

// 当前到了pos位置，开始填pos位置上的数
int dfs (int pos, bool limit, int last) // limit表示是否被限制上限，last表示上一个数字
{
    if (pos == cnt) return 1;
    int &ans = dp[pos][limit][last];
    if (~ans) return ans; else ans = 0;
    for (int v = 0; v <= (limit ? A[pos] : 9); v ++ )
        if (v == 4 || (last == 6 && v == 2)) continue;
        else ans += dfs(pos+1, limit && v == A[pos], v);
    return ans;
}

int f (int x)
{
    memset(A, 0, sizeof A);
    memset(dp, -1, sizeof dp);
    cnt = 0;
    
    while(x) A[cnt++] = x % 10, x /= 10;
    reverse(A, A + cnt);
    return dfs(0, 1, 0); // 从0号位置开始dp
}

int main ()
{
    int l, r; while(cin >> l >> r, l || r)
    cout << f(r) - f(l-1) << endl;
    return 0;
}
```

