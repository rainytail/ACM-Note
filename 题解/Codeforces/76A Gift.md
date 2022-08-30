## Gift 
### 题意
给定 $n$ 个点 $m$ 条边的图，每条边有权值 $(g_i, s_i)$ 。
给定 $G, S$ ，求一棵图的生成树 $T$ ，使得 $G \times \max(g_i) + S \times \max(s_i)$ 最小。

其中 $2 \le N \le 200, 1 \le M \le 50000$ 。
https://codeforces.com/problemset/problem/76/A

### 分析
#最小生成树 #排序 
由于两个权值 $g_i, s_i$ 都对答案产生影响，我们可以固定其中一个，然后求另一个值，使答案最小。
比如，我们可以按照 $g_i$ 把边排序，枚举 $g_i$ 的值，对所有 $g_j \le g_i$ 的边，求出他们的最小生成树，使得 $max(s_i)$ 最小。
这样的复杂度是 $O(nmlogm)$ ，由于我们边是一个一个加上去的，可以维护前面的最小生成树，使得生成树的那个边序列是升序，然后每次暴力把当前边插入到有序序列中。
还有个结论：一条边如果没有被加入到最小生成树中，那么在后面也不会被加入到最小生成树中。所以我们可以把生成树的备用边缩小到 $n - 1$ 范围内。 
这样复杂度就是 $O(nm)$ 。

### Code
```c++
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Edge { int u, v, g, s; };

const int N = 210, M = 50010 << 1;
int n, m, G, S, fa[N];
ll ans = 2e18 + 10;
Edge E[M], T[M]; // E是原图，T是生成树
int tot; // tot维护生成树边

int find (int x) {
    return x == fa[x] ? x : fa[x] = find(fa[x]);
}

int main () {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> G >> S;
    for (int i = 1; i <= m; i ++ ) {
        int x, y, gi, si; cin >> x >> y >> gi >> si;
        E[i] = { x, y, gi, si };
    }
    sort(E + 1, E + m + 1, [&](Edge a, Edge b){ return a.g < b.g; });
    for (int i = 1; i <= m; i ++ ) {
        int p = ++ tot; // 在生成树上加一条边
        while(p >= 2 && T[p-1].s > E[i].s) {
            T[p] = T[p - 1];
            -- p;
        }
        T[p] = E[i]; // 暴力把第i条边插入到生成树有序边序列中
        // 作一边Kruskal
        int cnt = 0, ms = -1;
        iota(fa + 1, fa + n + 1, 1);
        for (int j = 1; j <= tot; j ++ ) {
            int u = find(T[j].u), v = find(T[j].v);
            if (u == v) continue;
            T[++ cnt] = T[j]; // 注意，如果不被加入到生成树中，在以后也不会被考虑到
            fa[u] = v; 
            ms = max(ms, T[j].s);
        }
        if (cnt == n - 1) ans = min(ans, (ll)G * E[i].g + (ll)S * ms);
        tot = cnt;
    }
    if (ans == 2e18 + 10) ans = -1;
    cout << ans << endl;
    return 0;
}
```
