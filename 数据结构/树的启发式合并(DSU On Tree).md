---
tags:
 - 算法
 - 数据结构
---

# 树的启发式合并 (DSU On Tree)

// https://oi-wiki.org/graph/dsu-on-tree/

// https://codeforces.com/blog/entry/44351

其实就是借用了 DSU 的按秩合并的思想，本身是优化暴力。
每次合并时，把小的集合合并到大的集合中。
> 对于每个元素，每次对它进行 $push\_back$ 时，会使得它所在的集合扩大至少一倍，由于整个集合大小最多为 $n$ ，所以每个元素最多被 $push\_back$ $log(n)$ 次，那么 $n$ 个元素最多被 $push\_back$ $nlog(n)$ 次，复杂度就是 $n log(n)$ 的。

一些题目：

1. Lomsat gelral https://codeforces.com/contest/600/problem/E

**例题**：给定 $n$ 个节点的有根树(Root = 1)，每个节点都有一个颜色。问：对于以每个节点为根的子树，有多少不同的颜色？

朴素思想就是将 $u$ 树的子树全部计算，计算答案后清除信息，再递归子树进行计算答案。

DSU On Tree 使用按秩合并的思想，每次将小的子树合并到大的子树上，简单来说就是下面步骤：

1. 首先 $O(n)$ 计算出每个子树的节点数量，按照树链刨分的思想求出重节点。
2. 对于每个子树 $u$ ，先递归到轻节点，计算答案，**不保留**其子树上的颜色信息。
3. 递归重节点，并**保留**其子树上的颜色信息。
4. 再递归轻节点，求出其对子树 $u$ 的贡献。

给出下面代码：

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 100010;
int n, col[N];
vector<int> g[N];
int siz[N], son[N];
int cnt[N], totColor, curSon, ans[N]; // curSon 维护当前的重节点

void dfs (int u, int p)
{
    siz[u] = 1;
    int maxSon = -1;
    for (int v: g[u]) {
        if (v == p) continue;
        dfs(v, u);
        siz[u] += siz[v];
        if (maxSon < siz[v]) maxSon = siz[v], son[u] = v;
    }
}

void calc (int u, int p, int val)
{
    if (val ==  1 && !cnt[col[u]])     ++ totColor;
    if (val == -1 && cnt[col[u]] == 1) -- totColor;
    cnt[col[u]] += val;
    for (int v: g[u]) {
        if (v != p && v != curSon) calc(v, u, val);
    }
}

void dsu (int u, int p, bool keep)
{
    // 遍历轻节点得到答案
    for (int v: g[u]) {
        if (v != p && v != son[u]) dsu(v, u, 0);
    }
    if (son[u]) dsu(son[u], u, 1), curSon = son[u];
    calc(u, p, 1); // 计算当前树的答案
    ans[u] = totColor;
    curSon = 0; // 别忘了，因为已经把curSon的信息保留了，这里要清零
    if (!keep) { // 轻边，需要清除贡献
        calc(u, p, -1);
    }
}

int main ()
{
    cin >> n;
    for (int i = 1; i <= n; i ++ ) cin >> col[i];
    for (int i = 1; i < n; i ++ ) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0); dsu(1, 0, 0);
    for (int i = 1; i <= n; i ++ ) cout << ans[i] << " \n"[i == n];
    return 0;
}
```



复杂度计算：

首先有个重要的结论，根节点到任意一点的轻边个数不超过 $log n$ ，因为每走一条轻边，节点个数就会被砍一半。

那么对于每个节点，最多走 $log n + 1$ 次， $log n$ 次因为要走轻边，然后作为重边再走一次。

所以复杂度就是 $O(nlogn)$ 。