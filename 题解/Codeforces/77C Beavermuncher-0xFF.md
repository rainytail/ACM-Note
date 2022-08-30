## Beavermuncher-0xFF
### 题意
给定一棵 $n$ 个节点的树，每个节点上有 $k[i]$ 点权。
一个机器从根节点 $root$ 开始，每次可以向一个点权不为 $0$ 的子节点走，并把这个点权减去 $1$ 。最后需要走回到根节点 $root$ ，问这个过程最多可以减去多少点权。

其中 $1 \le n \le 10^5, 1 \le k_i \le 10^5$ 。
https://codeforces.com/problemset/problem/77/C

### 分析
#树 #贪心 
考虑从每个点开始的贡献。
对于每个点，我们用 $dfs(u)$ 表示 $u$ 点产生的最大贡献，以及产生贡献后，本身点权剩余的大小。
由于 $u$ 本身点权有限，不一定能够遍历完所有子节点并累加它们的贡献，我们可以把子节点贡献存起来，再贪心地找最大贡献。
注意，对于 $u$ 点本身，如果它不是根节点，需要从父节点往下走到达，这时候点权是减去 $1$ 了的。
然后在累加贡献时，需要从 $u$ 走向子节点，最后走回 $u$ 节点，这个过程 $u$ 节点需要减去 $1$ ，同时产生 $2$ 的贡献。
如果 $u$ 还有剩余的点权，可以让他和子节点来回走，每次产生 $2$ 贡献。
因此：$dfs(u) = \sum_{v \in son(u)} dfs(v) + 2 \times \min (left, sum)$ ，其中 $left$ 是 $u$ 节点最后剩余的点权，$sum$ 是子节点剩余的点权。

### Code
```c++
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 100010;
ll n, k[N], root;
vector<int> g[N];

// 返回pair: 当前点产生的贡献，以及当前点还剩下的海狸数量
pair<ll, ll> dfs (int u, int p) {
    ll sum = 0, left = k[u] - (u != root);
    vector<ll> V;
    for (int v: g[u]) {
        if (v == p) continue;
        auto [k, val] = dfs(v, u);
        V.push_back(k);
        sum += val;
    }
    sort(V.begin(), V.end(), greater<>());
    ll eat = 0;
    for (ll v: V) {
        if (!left) break;
        eat += v + 2ll;
        left -= 1ll;
    }
    eat += 2ll * min(left, sum);
    left -= min(left, sum);
    return { eat, left };
}

int main () {
    cin >> n;
    for (int i = 1; i <= n; i ++ ) cin >> k[i];
    for (int i = 1; i < n; i ++ ) {
        int x, y; cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    cin >> root;
    cout << dfs(root, -1).first << endl;
    return 0;
}
```