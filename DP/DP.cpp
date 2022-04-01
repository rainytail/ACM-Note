#include <bits/stdc++.h>
using namespace std;

const int N = 1010, M = 1010;

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

// 01背包 EXTRA: n个物品，装入体积至少为W，求价值和的最小值
// 01分数规划 + 01背包 https://www.luogu.com.cn/problem/P4377
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

// 01背包 EXTRA: 打印字典序最小的方案
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


int main () {
    return 0;
}