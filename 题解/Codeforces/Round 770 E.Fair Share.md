# E. Fair Share
## 题意
给定 $m$ 个长度为 $n_i$ 的序列和两个空的可重集合 $L、R$ 。
对于每个序列，选择 $\dfrac n 2$ 个放入 $L$ ，另一半放入 $R$ 。
问：能否存在一种放置方式，使得 $L = R$ ？如果有，输出该方案。
其中 $1 \le m \le 10^5, 2 \le n \le 2 \times 10^5$ 。且 $\sum {n_i} \le 2 \times 10^5$ 。

## 分析
#二分图 #欧拉回路
首先每个序列长度都为偶数，且数字出现次数也是偶数，否则一定无解。
我们可以将**序列编号与数字**构成二分图，如果 $i$ 数组存在数字 $j$ ，则连接 $i、j$ 。

这样，每个序列编号所在节点，度数都为偶数；且每个数字节点的度数也是偶数。
那么这样的图就构成了若干个**欧拉回路**。

跑完欧拉回路后，对于每个点，都有 $deg/2$ 个出边，且有 $deg/2$ 个入边。
对于 $u->v$ ($v$ 为数字节点)，则看作放入 $L$ 集合；$u<-v$ ，则看作放入 $R$ 集合，这样对于每个序列编号，都有一半节点放到 $L$ ，一半放到 $R$ ；对于每个数字，都有一半数量放入 $L$ ，一半放入 $R$ ，满足要求。

数字值域很大，需要先离散化。

遍历欧拉回路，需要删边，对于每个邻接边，可以选择从后往前dfs，这样就可以用 
 vector 的 pop_back 实现 $O(1)$ 删边。无向图记得判重。

## Code
```c++
#include <bits/stdc++.h>  
#define rep(x, a, b) for (int x = a; x <= b; x ++ )  
#define per(x, a, b) for (int x = a; x >= b; x -- )  
#define all(x) begin(x), end(x)  
#define pb(x) push_back(x)  
#define forr(x, a) for (auto &x: a)  
//#define int long long  
using namespace std; using ll = long long;  
template<typename T> void debug(T x) { cout << x << endl; }  
template<typename T, typename... Ts> void debug(T x, Ts... args)  
{  cout << x << endl; debug(args...);  }  
  
struct Node { int v, i, j; };  
  
const int N = 200100;  
int m, n;  
vector<int> a[N], type[N], vis[N]; // 每个数组所有数字的归类  
vector<Node> g[N << 1]; // 建图，每条边要记录：当前边位于哪个数组的哪个位置  
vector<int> alls;  
int cnt[N<<1];  
  
int find (int x) { return lower_bound(all(alls), x) - alls.begin() + 1; }  

// 非标准跑欧拉路径，适用于图上有多个欧拉路径
void dfs (int u) {  
    while(!g[u].empty()) {  
        // 遍历欧拉路径，从最后的边开始跑，则这样可以O(1)删边  
        auto [v, i, j] = g[u].back();  
        g[u].pop_back();  
        if (vis[i][j]) continue;  
        vis[i][j] = 1;  
        if (u > N) type[i][j] = 1;  
        else type[i][j] = 0;   
        dfs(v);  
    }  
}  
  
void solve() {  
     cin >> m;  
     for (int i = 0; i < m; i ++ ) {  
         cin >> n;  
         if (n & 1) return cout << "NO\n", void();  
         type[i].resize(n);  
         a[i].resize(n);  
         vis[i].resize(n);  
         for (int j = 0; j < n; j ++ ) {  
             cin >> a[i][j];  
             alls.pb(a[i][j]);  
         }  
     }  
    std::sort(alls.begin(), alls.end());  
    alls.erase(unique(alls.begin(), alls.end()), alls.end());  
    for (int i = 0; i < m; i ++ ) {  
        for (int j = 0; j < type[i].size(); j ++ ) {  
            a[i][j] = find(a[i][j]) + N;  
            ++ cnt[a[i][j] - N];  
            g[i].push_back({a[i][j], i, j});  
            g[a[i][j]].push_back({i, i, j});  
        }  
    }  
    for (int i = 1; i < N; i ++ ) if (cnt[i] & 1) return cout << "NO\n", void();  
    for (int i = 0; i < m; i ++ ) {  
        dfs(i);  
    }  
    cout << "YES\n";  
    for (int i = 0; i < m; i ++ ) {  
        for (int j : type[i]) {  
            if (j) cout << "L";  
            else cout << "R";  
        }  
        cout << endl;  
    }  
}  
  
signed main () {  
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