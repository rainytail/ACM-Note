### E. Xor Inverse
#### 题意
给定长度为 $n$ 的序列 $a$ ，求最小的 $x$ ，要求 $b$ 序列的逆序对最少。
其中 $b_i = a_i \oplus x$ 。
$1 \le n \le 3 \times 10^5, 0 \le a[i] \le 10^9$

#### 分析
#字典树 #DP #分治

对于位运算，通常要按位考虑。
两个数字比较大小，其实就是看**第一个不同的位**上的大小。
可以使用字典树来维护**前缀相同的数字**，同时保存这个前缀的所有数字。
那么就可以算出来对于这个前缀，填 0/1 的逆序对数量。
设 $dp[i][0/1]$ 表示第 i 位选择 0/1 的逆序对数量，取最小值就行。

#### Code
```c++
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 300010 * 30;
int n, a[N];
int son[N][2], idx;
vector<int> v[N];
ll dp[N][2];

void insert (int x, int pos) {
    int p = 0;
    for (int i = 30; i >= 0; i -- ) {
        int id = x >> i & 1;
        if (!son[p][id]) son[p][id] = ++ idx;
        p = son[p][id];
        v[p].push_back(pos);
    }
}

void dfs (int p, int pos) {
    int l = son[p][0], r = son[p][1];
    ll sz1 = v[l].size(), sz2 = v[r].size(), ans = 0, j = 0;
    for (int i = 0; i < sz1; i ++ ) {
        while(j < sz2 && v[r][j] < v[l][i]) ++ j;
        ans += j;
    }
    dp[pos][0] += ans;
    dp[pos][1] += 1ll * sz1 * sz2 - ans;
    if (l) dfs(l, pos - 1);
    if (r) dfs(r, pos - 1);
}

int main () {
    cin >> n;
    for (int i = 1; i <= n; i ++ ) cin >> a[i], insert(a[i], i);
    dfs(0, 30);
    ll ans1 = 0, ans2 = 0;
    for (int i = 30; i >= 0; i -- ) {
        if (dp[i][0] <= dp[i][1]) ans1 += dp[i][0];
        else {
            ans1 += dp[i][1];
            ans2 |= 1 << i;
        }
    }
    cout << ans1 << ' ' << ans2 << endl;
    return 0;
}
```

### F. Graph ans Queries
#### 题意
给出一张 $n$ 个点 $m$ 条边的无向图，每个点都由不同的权值 $p_i$，$q$ 次询问:
1. 给出点 $u$ ，求：$u$ 所在的连通块中点权最大值，并把这个点权改为 $0$ 。
2. 删除第 $i$ 条边。
其中 $1 \le n \le 2 \times 10^5, 1 \le m \le 3 \times 10^5, 1 \le q \le 5 \times 10^5$ 。

#### 分析
#并查集 #重构树 #线段树 #图的DFS序

对于删边操作，我们可以倒着做，从最后的情况倒推，这样**删边就变成了加边**。
使用并查集维护询问。

对于操作2，将 $u$ 和 $v$ 的集合合并，对于这次和之前的操作，可以通过 $u$ 访问到 $v$ ，反之也可以，那么可以建立新节点，向 $u$ 集合祖先和 $v$ 集合祖先连接**有向边**。这样，在之后的操作，$v$ 连通块的点就不能连接到 $u$ 。

对于操作1，在把边全部连接好后，就可以求出每个点的DFS序，然后使用线段树维护区间最大值。注意这个 $u$ 应该变为它的祖先，因为它可以遍历到它的集合内所有点。所以要从祖先开始遍历（因为并查集对图建立的是有向边）。

#### Code
```c++
#include <bits/stdc++.h>
using namespace std;

#define ls p << 1
#define rs p << 1 | 1
#define mid ((t[p].l + t[p].r) >> 1)
struct Node { int l, r, mx; };
struct Query { int op, id; }; // 对询问离线，反向维护图
struct Edge { int u, v, del; };

const int N = 500010;

int n, m, q, p[N], fa[N], pos[N];
vector<int> g[N];
Node t[N<<2];
Query Q[N];
Edge E[N];
int in[N], out[N], cnt; // 图的DFS序，需要有in和out，不然无法求连通块信息

int find (int x) {
    return x == fa[x] ? x : fa[x] = find(fa[x]);
}

void merge (int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return ;
    ++ n; fa[n] = n; fa[x] = fa[y] = n;
    g[n].push_back(x), g[n].push_back(y);
}

void dfs (int u) {
    in[u] = ++ cnt;
    for (int v: g[u])
        if (!in[v]) dfs(v);
    out[u] = cnt;
}

void build (int p, int l, int r) {
    t[p].l = l, t[p].r = r;
    if (l == r) return ;
    build(ls, l, mid), build(rs, mid + 1, r);
}

void update (int p, int x, int v) {
    if (t[p].l == t[p].r) return t[p].mx = v, void();
    if (x <= mid) update(ls, x, v);
    else update(rs, x, v);
    t[p].mx = max(t[ls].mx, t[rs].mx);
}

int query (int p, int l, int r) {
    if (l <= t[p].l && r >= t[p].r) return t[p].mx;
    int ans = 0;
    if (l <= mid) ans = max(ans, query(ls, l, r));
    if (r > mid)  ans = max(ans, query(rs, l, r));
    return ans;
}

int main () {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> q;
    for (int i = 1; i <= n; i ++ ) cin >> p[i], pos[p[i]] = i;
    for (int i = 1; i <= m; i ++ ) {
        auto &[u, v, del] = E[i];
        cin >> u >> v;
    }
    for (int i = 1; i <= q; i ++ ) {
        auto &[op, id] = Q[i];
        cin >> op >> id;
        if (op == 2) E[id].del = 1;
    }
    iota(fa + 1, fa + n + 1, 1);
    for (int i = 1; i <= m; i ++ ) {
        auto [u, v, del] = E[i];
        if (!del) merge(u, v);
    }
    for (int i = q; i >= 1; i -- ) {
        auto &[op, id] = Q[i];
        if (op == 1) id = find(id);
        else merge(E[id].u, E[id].v);
    }
    for (int i = 1; i <= n; i ++ )
        if (find(i) == i) dfs(i); // 求DFS序
    // 在图的DFS序上建树，维护最大值
    build(1, 1, n);
    for (int i = 1; i <= n; i ++ ) update(1, in[i], p[i]);
    for (int i = 1; i <= q; i ++ ) {
        auto [op, x] = Q[i];
        if (op == 1) {
            int ans = query(1, in[x], out[x]);
            cout << ans << '\n';
            if (ans) update(1, in[pos[ans]], 0);
        }
    }
    return 0;
}
```