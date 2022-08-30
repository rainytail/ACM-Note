## 题意
一维数轴上给定 N 个带权值的点，要求从一个位置开始，只朝着一个方向跳跃。
每次跳跃距离不能比之前小，且必须跳到带权值的点上，问最大跳到的权值和。

其中 $1 \le N \le 1000$ 。

## 分析
#DP #单调队列

只能朝一个方向跳，那么分两次计算，一次向右边跳，一次向左边跳。

首先可以看出来状态：f(i, j) 表示跳到第 i 个点，且跳跃为 j->i 时获得的最大权值和。

定义状态转移方程（向右边跳）：
$$
f(i, j) = min(f(j, k)) + s(i) \ \
(j < i, k \le j \ and \ x(j) - x(k) \le x(i) - x(j))
$$
复杂度为 $O(n^3)$

发现决策过程只与 j, k 有关，所以可以考虑从 i-1 转移到 i 。
首先是 i-1 可以作为 i 决策的方案了，所以初始化 $f(i,j) = f(i-1,j)-s(i-1)$
然后因为 $x(i) \gt x(i-1)$ ，所以 $k$ **可以取更小的值了**，使用单调队列来枚举 k 的值就行了。

## Code
```c++
#include <bits/stdc++.h>  
using namespace std;  
using T = pair<int, int>;  
  
const int N = 1010;  
  
#define x(i) p[i].first  
#define s(i) p[i].second  

int n, f[N][N];  
T p[N];  
int ans;  
  
int main () {  
    cin >> n;  
    for (int i = 1; i <= n; i ++ ) {  
        int x, y; cin >> x >> y;  
        p[i] = { x, y };  
    }  
    sort(p + 1, p + n + 1, [&](T a, T b) { return a.first < b.first; });  
    // 正向  
    for (int j = 1; j <= n; j ++ ) {  
        f[j][j] = s(j); ans = max(ans, f[j][j]);  
        for (int i = j + 1, k = j; i <= n; i ++ ) {  
            f[i][j] = f[i-1][j] - s(i-1);  
            while(k > 0 && x(j) - x(k) <= x(i) - x(j)) {  
                f[i][j] = max(f[i][j], f[j][k -- ]);  
            }  
            f[i][j] += s(i);  
            ans = max(ans, f[i][j]);  
        }  
    }    
    // 反向  
    for (int j = n; j >= 1; j -- ) {
        for (int i = j - 1, k = j; i >= 1; i -- ) {  
            f[i][j] = f[i+1][j] - s(i+1);  
            while(k <= n && x(k) - x(j) <= x(j) - x(i)) {  
                f[i][j] = max(f[i][j], f[j][k ++ ]);  
            }  
            f[i][j] += s(i);  
            ans = max(ans, f[i][j]);  
        }  
    }    cout << ans << endl;  
    return 0;  
}
```