# E. Cars
## 题意
在 $X$ 轴上有 $n$ 辆车，每辆车有位置和朝向。
给出 $m$ 个制约关系 $u、v$，表示 $u$ 和 $v$ 是相向还是相背。
构造出这 $n$ 辆车的朝向与位置，满足上述制约关系。

## 分析
#二分图 #拓扑排序 #偏序

相向：u ->     <- v
相背：<- u     v ->
**对于任意制约关系， $u$ 和 $v$ 的朝向都是相反的。**
定义 $dir_u = 0$ 表示 $u$ 朝向左边，$dir_u = 1$ 表示 $u$ 朝向右边，则**由制约关系形成的图一定是二分图**。
所以如果图不是二分图，就一定不能满足制约关系，反之可以。

考虑如何构造出车的朝向与位置。
对于制约关系 $u、v$ ，假设 $u$ 朝向左边，$v$ 朝向右边。
如果是相向的关系，则 $u$ 的位置在 $v$ 左边，即 $x_u < x_v$ ，否则 $x_v > x_u$ 。
对于 $x_u < x_v$ ，建立边 $u -> v$ 。
则对于新图，做拓扑排序得到 $x$ 的偏序集。
注意图可能不为拓扑图，这种情况也是不满足制约关系。

## Code1
```c++
#include <bits/stdc++.h>  
#define rep(x, a, b) for (int x = a; x <= b; x ++ )  
#define per(x, a, b) for (int x = a; x >= b; x -- )  
#define all(x) begin(x), end(x)  
#define pb(x) push_back(x)  
#define forr(x, a) for (auto &x: a)  
using namespace std;  
using ll = long long;  
  
// 有 n 辆车在x轴上，m个关系(u, v)，表示相向或者相背，求合法的，每辆车所在位置和朝向  
// 相向：u -> <- v type = 0  
// 背向：<- u v -> type = 1  
// 无论相向还是相背，方向都是相反的，只有位置不同  
// 用 0 表示 u 节点朝向左，1表示朝向右  
// 所以图为二分图，用染色法判断  
// 对于位置的选择，假设当前 u 朝向左，v朝向右  
// 那么可以根据关系判断位置的大小，所以最后所有点的位置就是偏序  
// 重建图，用拓扑排序得到偏序集  
  
struct Edge { int t, u, v; };  
const int N = 200010;  
int n, m;  
int dir[N];  
vector<int> g[N];  
Edge e[N];  
int deg[N];  
int q[N], hh, tt, tp[N];  
  
bool dfs (int u, int d) {  
    dir[u] = d;  
    for (int v: g[u]) {  
        if (dir[v] == -1 && !dfs(v, d^1)) return false;  
        else if (dir[v] == d) return false;  
    }  
    return true;  
}  
  
bool isBit() {  
    // 注意可能不是连通图  
    fill(dir + 1, dir + n + 1, -1);  
    for (int i = 1; i <= n; i ++ )  
        if (dir[i] == -1 && !dfs(i, 0)) return false;  
    return true;  
}  
  
bool isTopo () {  
    rep(i, 1, n) forr(v, g[i]) ++ deg[v];  
    hh = 0, tt = -1;  
    rep(i, 1, n) if (!deg[i]) q[++ tt] = i;  
    int cnt = 0;  
    while(hh <= tt) {  
        int u = q[hh ++ ];  
        tp[u] = ++ cnt;  
        forr(v, g[u])  
            if (-- deg[v] == 0) q[++ tt] = v;  
    }  
    return tt == n - 1;  
}  
  
void solve() {  
    cin >> n >> m;  
    rep(i, 1, m) {  
        int t, u, v; cin >> t >> u >> v;  
        e[i] = { t, u, v };  
        g[u].pb(v), g[v].pb(u);  
    }  
    if (!isBit()) return cout << "NO\n", void();  
    rep(i, 1, n) g[i].clear();  
    rep(i, 1, m) {  
        auto [t, u, v] = e[i];  
        if (dir[u] == 1) swap(u, v);  
        if (t == 1) g[u].pb(v);  
        else g[v].pb(u);  
    }  
    if (!isTopo()) return cout << "NO\n", void();  
    cout << "YES\n";  
    rep(i, 1, n) cout << (dir[i]==0 ? "L ": "R ") << tp[i] << endl;  
}  
  
int main () {  
#ifdef LOCAL  
    freopen(R"(C:\Users\wanglong\clion\input.txt)", "r", stdin);  
    freopen(R"(C:\Users\wanglong\clion\output.txt)", "w", stdout);  
#endif  
    cin.tie(nullptr); ios::sync_with_stdio(false);  
//    int _; cin >> _; while( _ -- )  
        solve();  
    return 0;  
}
```