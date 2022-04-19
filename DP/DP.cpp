#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1010, M = 1010, INF = 0x3f3f3f3f;

/* 状态空间
一个实际问题的各种可能情况构成的集合
由小及大：当状态空间位于边界上或某个小范围内等特殊情形，该状态空间的解往往是已知的。
若能将此解的应用场景扩大到原问题的状态空间，并且扩展过程的每个步骤具有相似性，就可以考虑使用递推或递归求解。
换句话说，程序在每个步骤上应该面对相同种类的问题，这些问题都是原问题的一个「子问题」，可能仅在规模或者某些限制条件上有所区别，并且能够使用「求解原问题的程序」进行求解。
*/

/* dp

视频讲解：
https://www.bilibili.com/video/av93356551  状压dp

NOTE 遇到求最值问题, 是否能将搜索性问题转化为判定性问题?
NOTE 状态不好确定时, 尝试转化问题模型、逆序思考、增加维度等
*/

/* 线性dp
① 前缀和后缀的转移，例如从 dp[i-1] 转移到 dp[i]，或者从 dp[j] 转移到 dp[i] (j < i), dp[i]可以表示为一个状态或者一组状态等
② 双序列问题，一般定义 dp[i][j] 表示对子问题 (s1[:i], s2[:j]) 的最优解结果，例如LCS

一些题目
二进制枚举状态 https://codeforces.com/problemset/problem/1391/D
数组划分(好题) 将搜索性问题转化为判定性问题  http://oj.daimayuan.top/problem/665  证明: https://zhuanlan.zhihu.com/p/489360066
智乃买瓜 消去影响 https://ac.nowcoder.com/acm/contest/view-submission?submissionId=50913018
*/


// 最长上升子序列(LIS)
// 把一个序列 A 变为非严格单调递增: 修改次数最小为 n - LIS (非降)
// 把一个序列 A 变为 严格单调递增: 修改次数最小为 n - LIS (非降) 其中LIS 指 B[i] = A[i] - i 的LIS

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
// 转换 求所有合法括号序列数量  http://oj.daimayuan.top/course/10/problem/707
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
// 1. 最大子段和 以 i 结尾, 则 ans = sum[i] - min(sum[j]) j < i
// 2. 令 f(i) 为以 i 结尾的最大子段和, 则 f(i) = max(f(i-1) + a[i], 0)
// 转换  https://codeforces.com/problemset/problem/1370/E
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

// 最大两段子段和(两段至少相距 gap) 要求每一段都是非空
// 令 f(i) 为以 a[i] 结尾的最大子段和, g(i) 以 a[i] 开头的最大子段和(倒着求一遍)
int maxTwoSubarraySum (int n, int a[], int gap = 1)
{
    int f[N], g[N];
    for (int i = 1; i <= n; i ++ ) {
        f[i] = max(f[i-1], 0) + a[i];
    }
    for (int i = n; i >= 1; i -- ) {
        g[i] = max(g[i+1], 0) + a[i];
    }
    for (int i = 2; i <= n; i ++ ) f[i] = max(f[i-1], f[i]);
    for (int i = n-1; i >= 1; i -- ) g[i] = max(g[i+1], g[i]);
    int ans = -2e9; // 可以为空: ans = 0
    for (int i = 1; i + gap + 1 <= n; i ++ ) {
        ans = max(ans, f[i] + g[i + gap + 1]);
    }
    return ans;
}

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
// NOTE 注意判断全为负数的情况
// 模板题  https://www.luogu.com.cn/problem/P1121
int maxTwoCircleSubarraySum (int n, int a[], int gap = 1)
{
    int ans = maxTwoSubarraySum(n, a, gap);
    // 特判全为负数的情况, 这个时候如果取反，会导致取完, 也就是 ans = tot, 这样取环形就会导致一个不取
    if (none_of(a + 1, a + n + 1, [&](int x){ return x >= 0; })) {
        return ans;
    }
    int tot = accumulate(a + 1, a + n + 1, 0);
    for (int i = 1; i <= n; i ++ ) a[i] = -a[i];
    ans = max(ans, tot + maxTwoSubarraySum(n, a, gap));
    return ans;
}


// 线性DP与其他算法结合
// 图论 + DP
// NOTE 最短路DP一般需要做多次最短路，因此数据范围较小
// 最短路 + DP  https://www.luogu.com.cn/problem/P1772


/* 区间dp
区间DP一般是转移是合并区间或者分解区间。
总结套路：https://www.luogu.com.cn/blog/BreakPlus/ou-jian-dp-zong-jie-ti-xie

① 将序列分为 K 个连续区间，求解这些区间的某个最优性质。
一般定义 dp[i][k] 表示将 a[:i] 分为 K 个连续区间得到的最优解
枚举最后一个区间的左端点 j，即最后区间为 [j, i] ，从 dp[j-1][k-1] 转移到 dp[i][k] ，转移时考虑 a[j:i] 对最优解的影响。
体育节  http://oj.daimayuan.top/course/10/problem/668  证明: https://zhuanlan.zhihu.com/p/490707329

② 求解关于某个序列的最优性质，要求大区间的最优解可以依赖于小区间的最优解。
一般定义 dp[i][j] 表示 a[i:j] 的最优解
此时可以枚举区间大小 [l, r] ，从小区间转移到大区间
一般是从 [l+1, r], [l, r-1] 转移到 [l, r] ，或者枚举中间断点 k ，从 [l, k], [k+1, r] 转移到 [l, r] 。
*/


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

// 求第 k 个满足条件的数字
// 求第 k 个数位含有 666 的数字  https://www.acwing.com/problem/content/312/
// 一种做法是二分答案, 因为[1, mid]中满足条件的数字数量一定是单调的
ll dfs (int pos, bool limit, bool last1, bool last2, bool ok)
{
    if (pos == cnt) return ok;
    ll &ans = dp[pos][limit][last1][last2][ok];
    if (ans != -1) return ans; else ans = 0;
    for (int v = 0; v <= (limit ? A[pos] : 9); v ++ ) {
        ans += dfs(pos + 1, limit && v == A[pos], last2, v == 6, \
                   ok || (last1 && last2 && v == 6));
    }
    return ans;
}
ll getKthNumber (int k)
{
    ll l = 0, r = 1e18;
    while(l < r) {
        ll mid = (l + r) >> 1ll;
        if (f(mid) >= x) r = mid;
        else l = mid + 1;
    }
    return r;
}

// 对数字的数位映射到某个函数，求区间函数和
// 定义数字x价值为数字段数值乘上区间长度平方, 求[l, r]所有数字的价值和
// https://www.luogu.com.cn/problem/P3754
// dp(i, 0/1, j, k) 表示前 i 个数字固定, 且当前无/有限制, 这个状态下最后数字为 j , 且连续长度为 k 时的数字数量
// sum(i, 0/1, j, k) 为这个状态的价值和
// 1. v == j  dp += dfs.dp sum += dfs.sum
// 2. v != j  dp += dfs.dp sum += dfs.sum + dfs.dp * j * k * k
const int SIZE = 17;
int A[SIZE], cnt;
ll dp[SIZE][2][SIZE][SIZE], sum[SIZE][2][SIZE][SIZE];
pair<int, int> dfs (int pos, bool limit, int last, int k)
{
    if (pos == cnt) return { 1, last * k * k };
    ll &ans1 = dp[pos][limit][last][k];
    ll &ans2 = sum[pos][limit][last][k];
    if (ans1 != -1) return { ans1, ans2 }; else ans1 = ans2 = 0;
    for (int v = 0; v <= (limit ? A[pos] : 9); v ++ )
        if (v == last) {
            auto [c1, c2] = dfs(pos+1, limit && v == A[pos], v, k+1);
            ans1 += c1;
            ans2 += c2;
        } else {
            auto [c1, c2] = dfs(pos+1, limit && v == A[pos], v, 1);
            ans1 += c1;
            ans2 += c2 + c1 * k * k * last;
        }
    return { ans1, ans2 };
}

/* 背包问题
从物品的选择次序的无后效性着手
*/

// 01 背包
// 模板：n个物品，有体积v和价值w，把它放进V体积的背包的最大价值
// 状态：从前i个物品中选择若干个，容量限制为j时能够获得的最大价值 i [0, n] j [w, W]
// 初始：f(0, j) = 0, 选0个物品，对于任何限制，最大价值都是0
// 决策：对于当前物品，每个状态都可以由两个来源，取max
// - 不选择第 i 个物品，f(i-1, j) -> f(i, j)
// - 选择第 i 个物品，f(i-1, j-w) + v -> f(i, j) j>=w
// 最优解为 f(W)
// 模板题：https://www.luogu.com.cn/problem/P1048
int zeroOneKnapsack (int n, int vals[], int weights[], int W)
{
    int dp[N]; // dp(i) 背包为i时能装的最大价值
    for (int i = 1; i <= n; i ++ )
        for (int j = W; j >= weights[i]; i -- ) {
            int v = vals[i], w = weights[i];
            dp[j] = max(dp[j], dp[j - w] + v);
        }
    return dp[W];
}

// EXTRA: n个物品，装入体积至少为W，求价值和的最小值
// NOTE 装入体积大于W, 就看做W来处理即可, 因为所有大于的和等于的都是同一种状态
// 01分数规划 + 01背包  https://www.luogu.com.cn/problem/P4377
// 转化  https://www.luogu.com.cn/problem/P1156
int zeroOneLeastKnapsack (int n, int vals[], int weights[], int W)
{
    int dp[N];
    fill(dp + 1, dp + n + 1, 1e9); // 初始化dp, 1e9 or 1e18
    dp[0] = 0;

    for (int i = 1; i <= n; i ++ )
        for (int j = W; j >= 0; j -- ) {
            int v = vals[i], w = weights[i];
            int ws = min(W, j + w);
            dp[ws] = min(dp[ws], dp[j] + v);
        }


    /* 另一种写法
    for (int i = 1; i <= n; i++)
        for (int j = W; j >= 0; j--) {
            int v = vals[i], w = weights[i];
            dp[j] = min(dp[j], dp[max(0, j - w)] + v); // max(0, j-w)蕴含了“至少”,有点"堆积"的感觉
        }
    */


    return dp[W];
}

// EXTRA: 打印字典序最小的方案
// 倒序遍历物品，同时使用fa数组记录转移来源
vector<int> GetLexicographicSmallestResult (int n, int vals[], int weights[], int W)
{
    // use zeroOneKnapsack
    int dp[N], fa[N][M]; // fa(i, j)表示对于前i个数字，j容量背包通过哪个背包转移过来
    for (int i = 1; i <= n; i ++ ) {
        for (int j = 0; j <= W; j ++ ) fa[i][j] = j; // 假设不选
        for (int j = W; j >= weights[i]; j--) {
            int v = vals[i], w = weights[i];
            if (dp[j - w] + v > dp[j]) { // 注意不要取等于，保证字典序最小
                dp[j] = dp[j - w] + v;
                fa[i][j] = j - w;
            }
        }
    }
    // 倒序遍历物品
    vector<int> ans;
    for (int i = n, j = W; i >= 1; i -- ) {
        if (fa[i][j] == j) -- i;
        else {
            ans.push_back(i);
            j = fa[i][j];
            i -- ; // 一个物品只能装一次，如果是完全背包，去掉这一句
        }
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

// EXTRA: 从序列 a 中选择若干个数，使其总和为 sum 的方案数
// NOTE: 判断能否达到 sum ,使用bitset或者开bool型dp
// 转换: https://ac.nowcoder.com/acm/contest/23106/A
// 转换: https://codeforces.com/contest/1637/problem/D
int zeroOneWaysToSum (int n, int vals[], int S)
{
    int dp[N];
    dp[0] = 1; // 全部不选到达0，初始条件
    for (int i = 1; i <= n; i ++ )
        for (int j = S; j >= vals[i]; j -- ) {
            int v = vals[i];
            dp[j] += dp[j - v]; // % mod
        }
    return dp[S];

    // 判定性(使用bitset): 能否到达sum
    bitset<N> f;
    f[0] = 1;
    for (int i = 1; i <= n; i ++ )
        f |= f << vals[i];
    return f[S];
}

// 多重背包
// 模板: n个物品, 有体积和价值, 每个物品有若干数量, 问容量为W的背包能放最大多少价值的物品
// 每个数字都能转化为若干个二进制数相加，把每个物品的数量转化为若干个二进制数，然后就是01背包了
int boundedKnapsackBinary (int n, int vals[], int weights[], int numbers[], int W)
{
    int dp[N];

    auto calc = [&](int v, int w) {
        for (int j = W; j >= w; j -- )
            dp[j] = max(dp[j], dp[j - w] + v);
    };

    for (int i = 1; i <= n; i ++ ) {
        int v = vals[i], w = weights[i], num = numbers[i], cnt = 1; // cnt为当前二进制数
        while(cnt <= num) {
            calc(v * cnt, w * cnt); // 进行一次01背包
            num -= cnt; // 剩余数量
            cnt <<= 1;
        }
        if (num) { // 剩余的直接再做一次01背包
            calc(v * num, w * num);
        }
    }

    return dp[W];
}

// EXTRA 多重背包计数(前缀和优化)
// https://www.luogu.com.cn/problem/P1077

// 分组背包
// 分组背包，就是对物品分组，要求每一组只能选一个物品，求出选择的物品的最大价值。
// 转换  https://www.luogu.com.cn/problem/P5322


// 树上背包/依赖背包
// 模板题  https://www.acwing.com/problem/content/description/10/
// 多棵树的情况 金明的预算方案  https://www.acwing.com/problem/content/489/
// 树上背包+01分数规划  https://www.luogu.com.cn/problem/P4322
int n, root, v[N], w[N], V;
int dp[N][M]; // 用j容量的背包，选择以i为根的子树，且i被选择
vector<int> g[N];
void dfs (int u)
{
    // 选择u
    for (int j = V; j >= v[u]; j -- ) dp[u][j] = w[u];
    // 选择子节点
    for (int s: g[u]) {
        dfs(s);
        for (int j = V; j >= v[u]; j -- ) // 给以u为根的这棵树分配多少容量
            for (int k = j - v[u]; k >= 0; k -- ) // 给s儿子分配多少容量
                dp[u][j] = max(dp[u][j], dp[u][j - k] + dp[s][k]);
    }
}

// 树形dp
// 一般以子树的根节点和根节点对应状态作为dp维度

// 树上长度为 k 的路径数量
// 设 dp[i][j] 表示以 i 为根, 长度为 j 的路径
// 注意根节点可能连接两头，即不是作为路径端点, 所以我们可以开一个全局变量 ans 来维护答案
// dp 数组只需要维护根节点为路径端点的情况
// https://codeforces.com/problemset/problem/161/D
void getWaysOfKLengthRoadInTree (int u, int p)
{
    dp[u][0] = 1;
    for (int v: g[u]) {
        if (v == p) continue;
        dfs(v, u);
        for (int i = 0; i < k; i ++ ) ans += dp[v][i] * dp[u][k-1-i];
        for (int i = 1; i <= k; i ++ ) dp[u][i] += dp[v][i-1];
    }
}

// 状压dp
// 题单  https://www.luogu.com.cn/training/215#problems
// 浅谈状压dp  https://www.luogu.com.cn/blog/yijan/zhuang-ya-dp
// 算法竞赛专题解析  https://blog.csdn.net/weixin_43914593/article/details/106432695
// todo 题单  https://ac.nowcoder.com/acm/problem/collection/808
//      题单  https://ac.nowcoder.com/acm/problem/collection/810
// 

/* 概率dp/期望dp
   https://oi-wiki.org/dp/probability/
   https://en.wikipedia.org/wiki/Probability

*/

// 概率
// 求概率，要考虑在当前状态下的各个可能的选择
// 抓老鼠  https://codeforces.com/problemset/problem/148/D

// 期望
// 重复操作型，会产生无限递归型公式，化简可以形成dp公式
// 收集邮票  https://www.luogu.com.cn/problem/P4550
// 2022昆明B(状压+期望dp):  https://ac.nowcoder.com/acm/contest/32708/B


int main()
{
    return 0;
}