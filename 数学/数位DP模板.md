数位DP基础模板：
```c++
/* 数位dp
数位dp用来求计数问题, 通常给出区间[l,r], 问在区间内满足某种性质的数的数量有多少个。
一般设 dp(i, 0/1, j) 表示前 i 个数字固定，且此时没有限制时, 得到的满足性质的数字数量, 后面的维度是其他要求的限制
NOTE 限制状态能用boo类型表示就使用, 这样可以减小dp容量, 大大减小memset的时间, 比如魔鬼数这一题
*/
// 模板 [l, r] 中不包含4和62的数字的数量  https://www.acwing.com/problem/content/1087/
int A[N], cnt, dp[N][2][N]; // N设置为数字的最大位数
int dfs (int pos, bool limit, int last)
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

    while (x) A[cnt++] = x % 10, x /= 10;
    reverse(A, A + cnt);
    return dfs(0, 1, 0);
}
// 然后就是直接 f(r) - f(l-1) 就是 [l, r] 范围内满足条件的数字了
```

这种做法在每次计算 $f(x)$ 函数时，都需要初始化 $dp$ 数组并且重新计算关于 $x$ 的 $dp$ 函数。
这是因为受到了 $limit$ 的影响，不同的 $x$ ，$limit$ 可能不同，同理还有 $lead$ (前导零)。

我们可以只对 $pos$ 进行记忆化，因为对于每一个位置， $limit$ 在整个递归过程只会出现一次，比如对于 $234$ ，如果 $pos = 2$ ，如果不受到 $limit$ 的限制，那么前面可能有 $11, 12 \cdots 23$ ，而在 $limit = true$ 时，只有 $23$ 这一种可能。

```c++
int A[10], cnt;
int dp[10];

int dfs (vector<string>& digits, int pos, int limit, bool lead) {
    if (pos == cnt) return !lead;
    // 只对pos记忆化，dp的含义是不被 n 限制的所有合法方案，那么也不会被lead限制，因为不同的n可能会导致lead的取值不同
    if (!limit && !lead && dp[pos] >= 0) return dp[pos];
    int ans = 0;
    if (lead) ans += dfs(digits, pos + 1, false, true);
    for (int i = 0; i < digits.size(); i ++ ) {
        if (limit && digits[i][0]-'0' > A[pos]) break;
        ans += dfs(digits, pos + 1, limit && digits[i][0]-'0' == A[pos], 0);
    }
    // 注意，只对pos记忆化，对于limit上界和lead是不确定的
    if (!limit && !lead) dp[pos] = ans;
    return ans;
}

int f (vector<string> &digits, int x) {
    memset(dp, -1, sizeof dp);
    memset(A, 0, sizeof A);
    cnt = 0;
    
    while (x) {
        A[cnt ++ ] = x % 10;
        x /= 10;
    }
    reverse(A, A + cnt);

    return dfs(digits, 0, 1, 1);    
}
```