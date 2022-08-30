在一张有向图中求出任意两点的最短路，这个图上可以存在自环、重边、负边权等。

可以使用 Floyd 算法直接 $O(n^3)$ 求出。
如果图是正权图，可以使用堆优化的 Dijkstra 来优化到 $O(nm \log n)$ 。

注意，在求最短路时不能直接对边进行统一操作使得图变为正权图的，比如对边整体加数或者乘数等。
比如 $1 \to 5$ 的最短路为 $5$ ，但经过了 $3$ 条边，但是另一条路径经过了 $2$ 条边，路径长度为 $6$ 。
我们对边统一加上 $5$ ，那么对于最短路 $5$ 而言，它总路径权值加上了 $3 \times 5$ ，而长度为 $6$ 的路径，只加了 $2 \times 5$ ，**对边统一操作，那么路径权值受到经过边的条数的影响**。

使用物理学 “势能” 这一概念，可以把负权图变成正权图，使用 Dijkstra 来计算。
因为势能的大小不受经过的路径的影响，它的大小只与起始点和终点有关。

具体来说，建立一个超级源点，从这个源点向所有点连接一条权值为 $0$ 的有向边，使用 spfa 求出源点到达其他点的最短路 $h_i$ ，作为 $i$ 点的势能。
由于最短路具有三角形边的性质，对于边 $u \to v$ 来说，$h_v <= h_u + w(u, v)$ ，那么重置边 $u \to v$ 的权值为 $w(u, v) + h_u - h_v$ 即可。

假设 $i \to j \to k$ 是一条最短路径，那么权值为 $(w(i, j) + h_i - h_j) + (w(j, k) + h_j - h_k) = w(i, j) + w(j, k) + h_i - h_k$ ，原图的最短路和新图的最短路一一对应，只有势能的大小产生影响。

这个原理可以也适用于网络流中费用流使用 dijkstra 的解法，复杂度也更优秀。

例题： https://www.luogu.com.cn/problem/P5905
```c++
#include <bits/stdc++.h>
using namespace std;
using LL = long long;

const int N = 3010, M = 10010, INF = 1e9;
int n, m;
int ht[N]; // 每个点的势能
int h[N], e[M], w[M], ne[M], idx;
bool inq[N]; int cnt[N];
int d[N];

void add (int a, int b, int c) {
    e[idx] = b; w[idx] = c; ne[idx] = h[a]; h[a] = idx ++ ;
}

bool spfa (int s) {
    memset(ht, 0x3f, sizeof ht);
    ht[s] = 0; inq[s] = 1;
    queue<int> q; q.push(s); while(!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = 0;
        for (int i = h[u]; ~i; i = ne[i]) {
            int v = e[i], dist = w[i];
            if (ht[v] > ht[u] + dist) {
                ht[v] = ht[u] + dist;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] > n) return false;
                if (!inq[v]) q.push(v), inq[v] = 1;
            }
        }
    } return true;
}

void update () {
    for (int i = 1; i <= n; i ++ )
        for (int j = h[i]; ~j; j = ne[j]) {
            w[j] += ht[i] - ht[e[j]];
            assert(w[j] >= 0);
        }
}

LL dijkstra (int s) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > q;
    for (int i = 1; i <= n; i ++ ) d[i] = INF; d[s] = 0;
    memset(inq, 0, sizeof inq);
    q.push({0, s}); while(!q.empty()) {
        auto [dist, u] = q.top(); q.pop();
        if (inq[u]) continue; inq[u] = 1;
        for (int i = h[u], v; ~i && (v = e[i]); i = ne[i]) {
            if (d[v] > dist + w[i]) {
                d[v] = dist + w[i];
                if (!inq[v]) q.push({d[v], v});
            }
        }
    }
    LL ans = 0;
    for (int i = 1; i <= n; i ++ ) {
        if (d[i] == INF) ans = ans + (LL)INF * i;
        else ans = ans + ((LL)d[i] - ht[s] + ht[i]) * i;
    } return ans;
}

int main () {
    cin >> n >> m; memset(h, -1, sizeof h);
    for (int i = 1; i <= m; i ++ ) {
        int u, v, w; cin >> u >> v >> w;
        add(u, v, w);
    }
    for (int i = 1; i <= n; i ++ ) add(0, i, 0);
    if (!spfa(0)) return cout << "-1\n", 0;
    // for (int i = 1; i <= n; i ++ ) cout << ht[i] << endl;
    update();
    for (int i = 1; i <= n; i ++ ) {
        cout << dijkstra(i) << '\n';
    }
    return 0;
}
```