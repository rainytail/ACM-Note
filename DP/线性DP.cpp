#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 10010;

/* 线性dp
① 前缀和后缀的转移，例如从 dp[i-1] 转移到 dp[i]，或者从 dp[j] 转移到 dp[i] (j < i), dp[i]可以表示为一个状态或者一组状态等
② 双序列问题，一般定义 dp[i][j] 表示对子问题 (s1[:i], s2[:j]) 的最优解结果，例如LCS
*/

// 最长上升子序列(LIS)
// O(n^2) 写法，适用于比较复杂的变形
// 定义 dp[i] 为以 a[i] 为末尾的 LIS 的长度
// 转换  https://www.luogu.com.cn/problem/P4310
int lisSlow(int n, int a[])
{
    int dp[N];
    for (int i = 1; i <= n; i++) {
        dp[i] = 1;
        for (int j = 1; j < i; j++) {
            if (a[j] < a[i]) // 非降： <=
                dp[i] = max(dp[i], dp[j] + 1);
        }
    }
    return *max_element(dp + 1, dp + n + 1);
}

// 最长上升子序列(LIS)
// O(nlogn) 写法，定义 dp[i] 为长度为 i 的 LIS 的末尾元素的最小值
// 显然对于每个位置尽量让他更小，使后面的元素有更多选择
// 求每个前缀的LIS: 注意我们遍历到 i 时，其实就是只考虑了前 i 个元素构成的 LIS ，所以直接开个数组记录
//
// 求下降  http://oj.daimayuan.top/course/10/problem/699
int lisFast(int n, int a[])
{
    int dp[N];
    int len = 1;
    dp[len] = a[1];
    for (int i = 2; i <= n; i++) {
        if (a[i] < dp[len]) dp[++len] = a[i]; // 非降：<=
        else *lower_bound(dp + 1, dp + len + 1, a[i], greater<int>()) = a[i];
    }
    return len;
}

// LIS 求方案数
// LC 673 https://leetcode-cn.com/problems/number-of-longest-increasing-subsequence/solution/zui-chang-di-zeng-zi-xu-lie-de-ge-shu-by-w12f/
// 第一个是 O(n^2), 第二个是 O(nlogn)
// 第一种方法类似最短路计数，第二种方法是二分+贪心
int waysOfLisSlow (int n, int a[])
{
    int dp[N], cnt[N]; // cnt[i] 为以 i 为结尾的 LIS 的数量，判断这个是不是整个的 LIS 即可
    int maxLis = 0, ans = 0;
    for (int i = 1; i <= n; i ++ ) {
        dp[i] = cnt[i] = 1;
        for (int j = 1; j < i; j ++ ) {
            if (a[j] >= a[i]) continue; // 非降：>
            if (dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                cnt[i] = cnt[j];
            } else if (dp[j] + 1 == dp[i]) {
                cnt[i] += cnt[j];
            }
        }
        if (maxLis < dp[i]) {
            maxLis = dp[i];
            ans = cnt[i];
        } else if (maxLis == dp[i]) {
            ans += cnt[i];
        }
    }
    return ans;
}

// LIS 求方案数
// O(nlogn)
// 具体原理: LC 673 https://leetcode-cn.com/problems/number-of-longest-increasing-subsequence/solution/zui-chang-di-zeng-zi-xu-lie-de-ge-shu-by-w12f/
// 由于不会用复杂一点的lb和ub，就直接手写二分了
int binarySearch (int n, function<bool(int)> f) {
    // 寻找第一个满足条件的位置
    int l = 0, r = n;
    while (l < r) {
        int mid = l + r >> 1;
        if (f(mid)) r = mid;
        else l = mid + 1;
    }
    return r;
}
int waysToLisFast (int n, int a[])
{
    vector<vector<int>> d, cnt; // d 记录历史信息，cnt记录个数的前缀和
    for (int i = 1; i <= n; i ++ ) {
        // 找到第一个大于等于它的
        int p = binarySearch(d.size(), [&](int v){return d[v].back() >= a[i];});
        int c = 1;
        if (p > 0) {
            int k = binarySearch(d[p-1].size(), [&](int k){return d[p-1][k] < a[i];});
            c = cnt[p-1].back() - cnt[p-1][k];
        }
        if (p == d.size()) {
            d.push_back({a[i]});
            cnt.push_back({0, c});
        } else {
            d[p].push_back(a[i]);
            cnt[p].push_back(cnt[p].back() + c);
        }
    }
    return cnt.back().back();
}

// 长度为 m 的 LIS 数量
// 定义状态: dp(i, j) 以 a[i] 结尾, 且长度为 j 的 LIS 数量
// 转移: dp(i, j) = Σdp(k, j-1)  k < i && a[k] < a[i] (严格上升)
// 枚举长度j作为第一层, i为第二层, 注意每次只会多出一个i作为策略之一，可以使用树状数组维护
struct fenwick {
    ll c[N];
    void clear (int n) { fill(c + 1, c + n + 1, 0); }
    void add (int x, int v) {
        if (x) for (; x < N; x += x & -x) c[x] += v;
    }
    int query (int x) {
        int res = 0;
        for (; x; x -= x & -x) res += c[x];
        return res;
    }
} F;
int getMLengthLis (int n, int m, int a[])
{
    int dp[N][N];
    vector<int> alls; // 用来离散化
    for (int i = 1; i <= n; i ++ ) {
        alls.push_back(a[i]);
    }

    sort(alls.begin(), alls.end());
    alls.erase(unique(alls.begin(), alls.end()), alls.end());
    for (int i = 1; i <= n; i ++ ) {
        a[i] = lower_bound(alls.begin(), alls.end(), a[i]) - alls.begin() + 1;
    }

    for (int i = 1; i <= n; i ++ ) {
        dp[i][1] = 1;
    }

    for (int j = 2; j <= m; j ++ ) {
        F.clear(alls.size()); // 0的位置不清空
        for (int i = 1; i <= n; i ++ ) {
            dp[i][j] = F.query(a[i] - 1);
            F.add(a[i], dp[i][j - 1]);
        }
    }

    ll ans = 0;
    for (int i = 1; i <= n; i ++ ) ans += dp[i][m];

    return ans;
}

// 最长回文子串(LPS)
// https://codeforces.com/problemset/problem/5/C
// dp解释：https://codeforces.com/blog/entry/213
// 状态定义 对于 i 为 ')' 的情况：
// - d[i] 与第i个位置匹配的'('位置
// - c[i] 能够匹配成回文串的最左的'('位置
// 使用栈来模拟
// 状态转移 c[i] = c[d[i]-1] if d[i]-1 is ')' and c[d[i]-1] != -1
int getLPS (int n, char s[])
{
    int d[N], c[N];
    vector<int> stk; // 模拟栈
    for (int i = 1; i <= n; i ++ ) {
        if (s[i] == '(') stk.push_back(i);
        else {
            if (stk.empty()) d[i] = c[i] = -1;
            else {
                d[i] = c[i] = stk.back();
                stk.pop_back();
                if (s[d[i]-1] == ')' && c[d[i]-1] != -1) c[i] = c[d[i]-1];
            }
        }
    }
    int Ans = 0, cnt = 1; // Ans为LPS, cnt为方案数
    for (int i = 1; i <= n; i ++ ) {
        if (s[i] == '(' || c[i] == -1) continue;
        if (Ans == i - c[i] + 1) ++ cnt;
        else if (Ans < i  - c[i] + 1) {
            Ans = i - c[i] + 1;
            cnt = 1;
        }
    }
    return Ans;
}

// 最大子段和相关
// https://www.luogu.com.cn/problem/solution/P1121
// 最大子段和 以 i 结尾, 则 ans = sum[i] - min(sum[j]) j < i
int maxSubarraySum (int n, int a[])
{
    int ans = 0, last = 0; // last维护前缀最小 非空: ans = -2e9
    for (int i = 1; i <= n; i ++ ) {
        last += a[i];
        ans = max(ans, last);
        if (last < 0) last = 0;
    }
    return ans;
}
// 子段长度不大于 m
// 同样动态维护 min(sum[k]), i-m <= k < i, 使用单调队列维护

// 子段长度不小于 m
// 维护 min(sum[k]), 1 <= k <= i-m 即可

// 最大两段子段和
// 令 f(i) 为以 a[i] 结尾的最大子段和, g(i) 以 a[i] 开头的最大子段和(倒着求一遍)

// 最大 m 段子段和
// 令 f(i, j, 0/1) 为前 i 个元素, 开了 j 段, 第 i 个元素选或不选时的最大 j 段和
// - f(i, j, 1) = max(f(i-1, j, 1), f(i-1, j-1, 0)) + a[i]
// - f(i, j, 0) = max(f(i-1, j, 0), f(i-1, j, 1))

// 环状最大子段和
// 1. 将序列倍长, 做一遍长度不大于 m 的最大子段和
// 2. 考虑子段是否过端点, 过端点取反, 求最小子段和(对序列取反求最大), 用总和减去即可

// 环状最大两段子段和
// 对于所有答案可能的情况, 一种是一段跨过了端点, 另一种是没有跨过端点
// 那么求一遍两段最大子段和, 再求一遍最小两段子段和, 用总和减去就是跨过端点的两段最大子段和


// 线性DP与其他算法结合
// 图论 + DP
// NOTE 最短路DP一般需要做多次最短路，因此数据范围较小
// 最短路 + DP  https://www.luogu.com.cn/problem/P1772


int main()
{
    return 0;
}