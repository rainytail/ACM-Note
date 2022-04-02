#include <bits/stdc++.h>
using namespace std;
const int N = 10010;

/* 线性dp
① 前缀和后缀的转移，例如从 dp[i-1] 转移到 dp[i]，或者从 dp[j] 转移到 dp[i] (j < i), dp[i]可以表示为一个状态或者一组状态等
② 双序列问题，一般定义 dp[i][j] 表示对子问题 (s1[:i], s2[:j]) 的最优解结果，例如LCS
*/

// 最长上升子序列(LIS)
// O(n^2) 写法，适用于比较复杂的变形
// 定义 dp[i] 为以 a[i] 为末尾的 LIS 的长度
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


int main()
{
    return 0;
}