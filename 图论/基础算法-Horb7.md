# 基础算法

以巩固基础算法为主，顺便留一下板子。

## 拓扑排序

BFS将入度为0的点入队，每次弹出队头，将临边入度减一，如果入度为0，加入队列。

使用数组模拟，则最后的拓扑序列在队列 q[hh, tt] 中。

判断是否存在环：tt == n - 1。

// 转换  无向图+有向图，求拓扑序  https://codeforces.com/problemset/problem/1385/E

```c++
vector<int> g[N];
int q[N], deg[N];

// toposort
int hh = 0, tt = -1; // 队头，队尾
for (int i = 1; i <= n; i ++ ) if (!deg[i]) q[++ tt] = i;
while(hh <= tt) {
    int u = q[hh ++ ];
    for (int &v: g[u])
        if (-- deg[v] == 0) q[++ tt] = v;
}
if (tt != n - 1) cout << "has circle!\n";
for (int i = 0; i <= tt; i ++ ) cout << q[i] << " \n"[i == tt];
```

// 从链尾开始存储，求出拓扑序

```c++
void dfs (int u) {
    vis[u] = true;
    for (int v: g[u]) {
        if (!vis[v]) dfs(v);
    }
    topo.push_back(u); // 从链尾开始存储
}
```



## Dijkstra

根据贪心思想求最短路，要求边权为非负数。

注意Dijkstra不能求最长路和有负环情况的最短路。

```c++
// 朴素Dijkstra 复杂度O(n^2)
int g[N][N]; // 记录边权
int dis[N]; bool vis[N];
memset(dis, 0x3f, sizeof dis);
dis[start] = 0;
for (int i = 1; i <= n; i ++ ) { // 迭代n次
    // 选择最小边权并松弛
    int t = -1;
    for (int j = 1; j <= n; j ++ )
        if (!vis[j] && (t == -1 || dis[j] < dis[t]))
            t = j;
    vis[t] = true; // 选择
    for (int i = 1; i <= n; i ++ )
        dis[i] = min(dis[i], dis[t] + g[t][i]);
}
cout << dis[n] << endl;

// 二叉堆优化Dijkstra 复杂度O(mlogn)
// 注意priority_queue是重载小于号来实现大根堆
// 可以理解为按照小于号排序，然后越在前面的优先级越小
struct T { 
    int v, w; 
    bool operator<(const T &rhs) const {
        if (w != rhs.w) return w > rhs.w;
        return v < rhs.v;
    }
};
vector<T> g[N];
priority_queue<T> q;
q.push({start, 0});
while(!q.empty()) {
    auto TS = q.top(); q.pop();
    int u = TS.v, dist = TS.w;
    if (u == n) {
        cout << dist << endl;
        break;
    }
    if (st[u]) continue;
    st[u] = true;
    for (auto &[v, w]: g[u]) 
        if (!st[v]) q.push({v, dist + w});
}
```



## Bellman_Fond

Bellman_Fond算法可以求得有边数限制的最短路。

$1$ 到 $n$ 不超过 $k$ 条边的最短路：https://www.acwing.com/problem/content/855/

```c++
const int N = 510, M = 10010;
struct node { int u, v, w; } edge[M];
int n, m, k;
int d[N], t[N];

int bellman_ford ()
{
    memset(d, 0x3f, sizeof d); d[1] = 0;
    for (int i = 1; i <= k; i ++ )
    {
        memcpy(t, d, sizeof d); // 加入备份数组防止连锁反应
        for (int j = 1; j <= m; j ++ )
        {
            int u = edge[j].u, v = edge[j].v, w = edge[j].w;
            t[v] = min(t[v], d[u] + w);
        }
        memcpy(d, t, sizeof t);
    }
    return d[n] >= 0x3f3f3f3f / 2 ? -1e9 : d[n];
}
```



## SPFA

~~关于SPFA, 它 __~~

SPFA是优化后的Belloman_Fond算法，可以在均摊 $O(m)$ 下求最短路，并且可以求负数情况下的最短路，可以用在差分约束等题目上，但是容易被卡 ~~成 sb~~ ，最坏情况是 $O(nm)$ 。

```c++
// spfa本质是使得图上任意一条边满足三角不等式，即对于(u, v, w) 有dis[v] <= dis[u] + w
struct T { int v, w; };
vector<T> g[N];
int q[N], dis[N]; bool inq[N]; // 点有可能入队多次，需要使用循环队列

fill(dis + 1, dis + n + 1, 1e9);
d[start] = 0;
int hh = 0, tt = 1;
q[0] = start;
while(hh != tt) {
    int u = q[hh ++ ]; if (hh == N) hh = 0;
    inq[u] = 0;
    for (auto &[v, w]: g[u]) {
        if (dis[u] + w < dis[v]) {
            dis[v] = dis[u] + w;
            if (!inq[v]) {
                q[++ tt] = v; if (tt == N) tt = 0;
                inq[v] = 1;
            }
        }
    }
}
```

SPFA判断负环：使用 $cnt$ 数组记录 $1 \sim x$ 的最短路径上出现了多少条边，则 $cnt$ 不可能超过 $n$ ，如果超过 $n$ 说明有负环。

// NOTE：只判断负环，需要把所有点放进队列。

// 注意不能只从一个点出发找负环，因为这个点可能无法到达负环。

```c++
if (dis[u] + w < dis[v]) {
    dis[v] = dis[u] + w;
    cnt[v] = cnt[u] + 1;
    if (cnt[v] > n) {
        // has circle
    }
}
```



## Floyd

Floyd算法可以 $O(n^3)$ 地求图上任意两点的最短路，一般用于求多源最短路或者传递闭包。

```c++
for (int i = 1; i <= n; i ++ ) dis[i][i] = 0;
for (int k = 1; k <= n; k ++ )
    for (int i = 1; i <= n; i ++ )
        for (int j = 1; j <= n; j ++ )
            dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
```



## Prim

Prim求最小生成树，复杂度 $O(n^2)$ 。适用于稠密图。



## Kruskal

Kruskal求最小生成树，复杂度 $O(mlogm)$ 。适用于稀疏图。

除此之外，Kruskal还可以对图进行重构Kruskal树，使得题目可以使用树形结构相关算法和数据结构解题。



## LCA

LCA就是两个节点的最近公共祖先。

求LCA有三种求法，倍增，tarjan(离线)，树链刨分。

这里以一个例题来展示三种解法。

题意：给出 n 个点的一棵树，每个点有各自的点权，多次询问两个点简单路径所构成的点集的异或和。

### 倍增

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 200010;
int n, m, a[N];
vector<int> g[N];
int depth[N], f[N][21], Xor[N]; // Xor(i)表示从根节点到i的路径上的异或和

void dfs (int u, int fa)
{
    depth[u] = depth[fa] + 1; Xor[u] = Xor[fa] ^ a[u];
    f[u][0] = fa; for (int i = 1; i < 21; i ++ ) f[u][i] = f[f[u][i-1]][i-1];
    for (int v: g[u]) if (v != fa) dfs(v, u);
}

int lca (int a, int b)
{
    if (depth[a] < depth[b]) swap(a, b);
    for (int i = 20; i >= 0; i -- ) if (depth[f[a][i]] >= depth[b]) a = f[a][i];
    if (a == b) return b;
    for (int i = 20; i >= 0; i -- ) if (f[a][i] != f[b][i]) a = f[a][i], b = f[b][i];
    return f[a][0];
}

int main ()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++ ) scanf("%d", &a[i]);
    for (int i = 1; i <= n - 1; i ++ )
    {
        int u, v; scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0); // 以1为树的根
    while(m -- )
    {
        int u, v; scanf("%d%d", &u, &v);
        printf("%d\n", Xor[u] ^ Xor[v] ^ a[lca(u, v)]);
    }
    return 0;
}
```

### tarjan

```c++
#include <bits/stdc++.h>
using namespace std;

using T = pair<int, int>;
struct A { int id, u, v, _lca; };

const int N = 1000010;
int n, m, a[N], Xor[N];
int p[N], st[N];
vector<int> g[N];
vector<A> ans;
vector<T> query[N]; // query(i) 表示以i为某一点的所有lca查询

int find (int x) { return x == p[x] ? x : p[x] = find(p[x]); }

void Tarjan (int u)
{
    st[u] = 1; // 正在搜索
    for (int v: g[u]) if (!st[v]) { Xor[v] ^= Xor[u]; Tarjan(v); p[v] = u; }
    for (auto &[v, id]: query[u]) if (st[v] == 2) ans.push_back(A{ id, u, v, find(v) });
    st[u] = 2; // 完成搜索，回溯
}

int main ()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++ ) scanf("%d", &a[i]), Xor[i] = a[i];
    for (int i = 1; i <= n - 1; i ++ )
    {
        int u, v; scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    iota(p + 1, p + n + 1, 1);
    for (int i = 1; i <= m; i ++ )
    {
        int u, v; scanf("%d%d", &u, &v);
        query[u].push_back({v, i});
        query[v].push_back({u, i});
    }
    Tarjan(1);
    sort(ans.begin(), ans.end(), [&](A a, A b){ return a.id < b.id; });
    for (auto &[id, u, v, _lca]: ans) printf("%d\n", Xor[u] ^ Xor[v] ^ a[_lca]);
    return 0;
}
```



### 树刨

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 1000010;

struct fenwick
{
    int c[N];
    void add (int x, int v) { if (x) for (; x < N; x += x & -x) c[x] ^= v; }
    int query (int x)
    {
        int ans = 0;
        if (x) for (; x; x -= x & -x) ans ^= c[x];
        return ans;
    }
    int query (int l, int r) { return query(r) ^ query(l - 1); }
};

int n, m, a[N];
vector<int> g[N];
fenwick F;
int son[N], top[N], depth[N], siz[N], father[N], id[N], wt[N], cnt;

void dfs1 (int u, int fa)
{
    depth[u] = depth[fa] + 1;
    father[u] = fa;
    siz[u] = 1;
    int maxson = -1;
    for (int v : g[u])
    {
        if (v == fa) continue;
        dfs1(v, u); siz[u] += siz[v];
        if (siz[v] > maxson) son[u] = v, maxson = siz[v];
    }
}

void dfs2 (int u, int topfather)
{
    top[u] = topfather;
    id[u] = ++ cnt;
    wt[cnt] = a[u]; F.add(cnt, wt[cnt]);
    if (!son[u]) return ;
    dfs2(son[u], topfather);
    for (int v : g[u]) if (v != son[u] && v != father[u]) dfs2(v, v);
}

int query (int u, int v)
{
    int ans = 0;
    while(top[u] != top[v])
    {
        if (depth[top[u]] < depth[top[v]]) swap(u, v);
        ans ^= F.query(id[top[u]], id[u]);
        u = father[top[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    ans ^= F.query(id[u], id[v]);
    return ans;
}

int main ()
{
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i ++ ) scanf("%d", &a[i]);
    for (int i = 1; i <= n - 1; i ++ )
    {
        int u, v; scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1, 0); // 第一次dfs求出depth, siz, father, son
    dfs2(1, 1); // 第二次dfs链接重链, 求出top, id, wt
    for (int i = 1; i <= m; i ++ )
    {
        int u, v; scanf("%d%d", &u, &v);
        printf("%d\n", query(u, v));
    }
    return 0;
}
```



## Tarjan 缩点

https://blog.csdn.net/weixin_45775050/article/details/107307757

题单
tarjan缩点 + 建反向图  https://atcoder.jp/contests/abc245/tasks/abc245_f

```c++
int n, m;
vector<int> g[N]; // 原图
int scc_cnt, sz[N], id[N]; // 缩点的数量，每个缩点内的点数量，每个点所在的缩点
int dfn[N], low[N], timestamp; // 每个点的 dfn, low
int stk[N], top; // 用栈维护Tarjan
bool in_stk[N]; // 是否在栈中
int dout[N]; // 每个缩点的出边，也可以维护其他的性质

void tarjan (int u)
{
    dfn[u] = low[u] = ++ timestamp;
    stk[++ top] = u; in_stk[u] = true;
    for (int v: g[u]) {
        if (!dfn[v]) { // 连通分量本身dfn最小值
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else {
            // 横叉边到达这个连通分量
            if (in_stk[v]) low[u] = min(low[u], dfn[v]);
        }
    }
    
    // 这个点在强连通分量内
    if (dfn[u] == low[u]) {
        ++ scc_cnt;
        int y;
        do {
            y = stk[top -- ];
            in_stk[y] = false;
            id[y] = scc_cnt;
            /* 维护缩点的性质，如里面的点数量
            sz[scc_cnt] ++ ;
            */
        } while(y != u);
    }
}

// 求强连通分量
for (int i = 1; i <= n; i ++ )
    if (!dfn[i]) tarjan(i);

// 缩点
for (int u = 1; u <= n; u ++ ) {
    for (int v: g[u]) {
        int a = id[u], b = id[v];
        if (a != b) {
            /* 维护缩点信息，如出边
            dout[a] ++ ;
            */
        }
    }
}

// 遍历缩点
for (int i = 1; i <= scc_cnt; i ++ ) {
    
}
```

## 环图
解决带环图的问题
一般题目的解法与环有关，但是又不能直接生成一张图(点太多，边数太大等)
因为解题只和环有关，可以用并查集或者度来解决。

并查集解环图：
https://atcoder.jp/contests/abc256/tasks/abc256_e

## 欧拉回路
欧拉路径：在图上的一个通路，对于所有边都在通路上出现且**仅出现一次**。
欧拉回路：如果一个欧拉路径是一个环，那么这个环为欧拉回路
欧拉回路可以用于构造或者二分图之类的题目上。

欧拉路径必须满足：对于无向图，度数至多两个奇数；对于有向图，要么入度等于出度，要么有一个入度-出度=1，一个出度-入度=1
欧拉回路必须满足：对于无向图，度数为偶数；对于有向图，入度等于出度。

求欧拉路径的边遍历顺序，使用dfs模拟。
注意答案的push顺序，类似于tarjan的栈功能，对于一个点，它可能dfs多个环，在得到一个环后，再从后往前把环上的数据存下来，这样输出的时候需要逆序。
欧拉路径遍历的时候需要删边，保证每条边只被遍历一次，对于vector建图，可以从后往前遍历，这样可以 $O(1)$ 删边。

以无向图为例：
```c++
struct Node { int v, t; }; // t表示哪一条边，负数表示反向边
int n, m;
vector<Node> g[N];
vector<int> ans;
bool vis[N];
void dfs (int u) {
	while(!g[u].empty()) {
		auto [v, t] = g[u].back();
		g[u].pop_back();
		if (vis[abs(t)]) continue;
		vis[abs(t)] = true;
		dfs(v);
		ans.push_back(t);
	}
}

for (int i = m - 1; i >= 0; i -- ) cout << ans[i] << ' ';
```

## 匈牙利算法
匈牙利算法用来解决二分图的最大匹配问题。
#二分图
二分图的一些结论：
最小点覆盖、最大独立集、最小路径点覆盖（最小路径重复点覆盖）
最大匹配数 = 最小点覆盖 = 总点数 - 最大独立集 = 总点数 - 最小路径点覆盖

最大匹配数问题：
在图上找到最多的边，使得这些边无公共点（每个点度数为1）

最小点覆盖：
在图上找到最少的点集，使得图上每一条边都至少连接其中一个点

最大独立集：
在图上找到最大的点集，使得这些点之间不存在边

最小路径点覆盖：
在图上找到最少的路径，使得这些路径遍历所有点
> 证明（有向无环图，不相交路径点覆盖）：
> 对于每个点，把它们当作长度为 $0$ 的路径。
> 把每个点 $u$ 拆成两个点 $u_x 、 u_y$ ，分别作为弧头和弧尾。
> 对于一条有向边 $u \to v$ ，连接 $u_x \to v_y$ ，那么原图变成了一张二分图。
> 每一次匹配，都会使得两个路径合并为 $1$ 个，减少一条路径。
> 所以最小路径点覆盖就是 总点数 - 最大匹配数
> 求方案：根据match求出每个点的弧头，没有弧尾的是开头

如果是可相交最小路径覆盖，先使用Floyd求出原图的传递闭包，然后如果 $a \to b$ ，则连接 $a 、 b$ ，问题就变成了不相交最小路径覆盖。因为相交其实就是多个路径经过了某些相同点，我们只让其中一条经过，这样就是不相交了。

求最大匹配数，以 n 点匹配 m 点为例
```c++
int match[N]; // 匹配对象
bool st[N]; // 是否在这一轮有匹配

bool find (int i) {
	for (int j = 1; j <= m; j ++ ) {
		if (!st[j]) {
			st[j] = 1;
			if (!match[j] || find(match[j])) {
				match[j] = i;
				return true;
			}
		}
	}
	return false;
}

int ret = 0;
for (int i = 1; i <= n; i ++ ) {
	memset(st, 0, sizeof st);
	if (find(i)) ++ ret;
}
```

二分图求方案数：注意 $match$ 数组就是每个点对应的匹配对象。

二分图的多重最大匹配问题：
使用最大流解决，在原来二分图的基础上，建立源点和汇点。
源点向左集合每个点连边，容量为这个点可以被选择的次数。
右集合每个点向汇点连边，容量为这个点可以匹配的点数量。
由于每个点的性质都已经在图中确定好了，左集合与右集合原来有的边上，设置容量为正无穷即可，因为匹配数量已经由源点、汇点连接的边决定了，设置正无穷保证了如果中间流的连通性。
然后最大流就是匹配的数量，方案记录在残余网络上。

二分图的多重最优匹配问题（最大权匹配问题）：
使用费用流解决，在原图的边上设置费用即可，容量不变。
源点与汇点连接的边一般设费用为 $0$ 。