#border #kmp

在字符串类算法中，我们用到的字符串一般以 $1$ 为下标开头。

## border
如果字符串 $S$ 的某一个长度相同的前后缀完全相同，则称这个前缀(后缀)字符串为 $S$ 的一个 border。
即：字符串 $S$ 的一个border满足: $pre_i = suf_i$
一个字符串会具有多个border。
比如说字符串 $S = bbabbab$
它的border有 $b$ ，$bbab$ ，$bbabbab$(本身) ，换成序列表示形式就是 $[1, 0, 0, 1, 0 , 0, 1]$

**字符串的最大border**
在一般情况下，这个border不包括这个字符串本身。
border有一个传递性：字符串的border的border也是这个字符串的border。
求某个字符串的最大border，可以使用类似 DP 的形式，设 $nxt[i]$ 表示 $pre_i$ 字符串的最大 border。
首先有 $nxt[1] = 0$
求 $pre_i$ ，则可以先把 $i$ 字符去掉，那么 $pre_i$ 的border就会变成 $pre_{i-1}$ 的border。
枚举这所有的border，看是否border的后一位与 $i$ 字符相同即可。
根据border的传递性，枚举border，其实就是 $nxt[i] = nxt[nxt[i]]$ 。

```c++
void get_next (string &s) {
    nxt[1] = 0;
    for (int i = 2; i < s.size(); i ++ ) {
        nxt[i] = nxt[i-1];
        while(nxt[i] && s[i] != s[nxt[i]+1])
            nxt[i] = nxt[nxt[i]];
        if (s[i] == s[nxt[i] + 1]) ++ nxt[i];
    }
}
```

复杂度：对于每个 $nxt[i]$ ，它最多减任意个，但是只能向上走一个（$(\max nxt[i]) = nxt[i-1] + 1$），
在函数图上是多段斜率为 $1$ 的直线，拼接起来后， $f(|S|) = |S|$ ，因此复杂度是 $O(n)$ 的。

## 字符串匹配
求得 $nxt$ 数组后，可以使用 $kmp$ 对模式串进行 $O(n+m)$ 的匹配。
设当前串当前匹配到 $j$ 位，模式串已经匹配完第 $i$ 位，接下来要匹配 $s[j] == p[i+1]$ 。
如果不相同，根据 $nxt$ 数组的性质，$p[1, nxt[i]] = p[i-nxt[i]+1, i]$ 。
把 $i$ 移动到 $nxt[i]$ 的位置，这样可以保证 $i$ 前面的字符和 $j$ 都一一匹配了，就可以直接再匹配 $s[j]$ 和 $p[i+1]$ 了。

```c++
int i = 0;
    for (int j = 1; j <= m; j ++ ) {
        while(i && s[j] != p[i + 1]) i = nxt[i];
        if (s[j] == p[i + 1]) ++ i;
        if (i == n) {
            /* 匹配成功 */
            i = nxt[i];
        }
    }
```

## border树
在求 $S$ 所有border的过程中，我们从 $S$ 开始(因为 $S$ 是最大的border)，每次把指针 $i$ 变成 $nxt[i]$ 求出当前字符串最大的border的最大border，根据传递性，这个border也是当前字符串的border。
```c++
vector<int> border;
for (int i = n; i >= 1; i = nxt[i])
	border.push_back(i);
```
上述代码其实就是 $i \to nxt[i]$ 连边的过程，最后会形成一棵树，树的根节点为 $0$ ，我们称这棵树为 border 树。
这样就能求出，每个border在所有border中作为前缀出现的次数了，这个次数其实就是子树的大小（类似于字典树维护前缀？）。
那么，如何求出每个border在整个字符串作为子串出现的次数呢？
我们只需要对每个 $i$ ，连向 $nxt[i]$ 即可，最后使用上述代码来遍历所有border即可。
在这种情况下，树上的节点不全是border。
例题： https://ac.nowcoder.com/acm/contest/28737/F
```c++
for (int i = 1; i <= n; i ++ )
	g[nxt[i]].push_back(i);
dfs(0, -1);
memset(cnt, -1, sizeof cnt); // 不是border的位置用-1来代替
for (int i = n; i >= 1; i = nxt[i])
	cnt[i] = siz[i];
```

```c++
/*
	求出最长的border，要求这个border作为子串在字符串S中出现至少k次

	朴素：求出所有border，计算每个border作为子串出现的次数
	根据border的传递性，S的border的border也是S的border，而这个border是S border的前缀
	建立border树，按 i -> nxt[i] 建树，则如果某个点是border，则这个border在S中出现的次数就是子树的大小
*/


// T 是S的一个border
// 于是问题就是求出最大的一个border，要求这个border作为子串在S中出现至少k次
// 求满足条件的最长的border
// next数组本身在匹配时是一个爬树的过程，从 i 爬到 next[i] ，直到到达0
// 根据border的传递性，可以建立一棵树
// 那么每个节点的子树大小，就是这个border作为子串出现的次数

#include <bits/stdc++.h>
using namespace std;

const int N = 1000010;
int n, k;
string S;
int nxt[N];
vector<int> g[N];
int sz[N];

void get_next(string &s) {
    nxt[1] = 0;
    for (int i = 2; i < s.size(); i ++ ) {
        nxt[i] = nxt[i-1];
        while(nxt[i] && s[i] != s[nxt[i]+1]) nxt[i] = nxt[nxt[i]];
        if (s[i] == s[nxt[i] + 1]) ++ nxt[i];
    }
}

void dfs (int u) {
    sz[u] = 1;
    for (int v: g[u]) {
        dfs(v); sz[u] += sz[v];
    }
}

int main () {
    cin >> n >> k >> S; S = ' ' + S;
    get_next(S);
    // 根据next数组建立border树
    for (int i = 1; i <= n; i ++ ) {
        g[nxt[i]].push_back(i);
    }
    dfs(0); // 注意border树的根节点是0
    for (int i = n; i >= 1; i = nxt[i]) {
        if (sz[i] >= k) {
            for (int j = 1; j <= i; j ++ ) cout << S[j];
            return 0;
        }
    } cout << -1 << endl;
    return 0;
}
```

上面的题目是静态的，如何维护一个动态的border树？
例题： https://ac.nowcoder.com/acm/contest/28737/G
给出初始字符串 $S$ 和 $q$ 次操作：
1. 在 $S$ 后面添加字符 $ch$
2. 求出最长的border，要求这个border在S中出现至少 $k$ 次

可以把询问离线下来，然后对最后的S建立border树。
上个例题说了，某个border出现的次数就是子树的大小，但是在建立的border树上存在未来建立的点，怎么办呢？
我们可以给每个点赋上一个点权，1表示这个点现在存在，0表示这个点现在不存在。
那么，border出现的次数就是子树内的点权和了。
现在的问题就变成了如何对某个点快速赋值 + 求某个点的子树大小。
可以使用dfs序，转化为单点修改 + 区间查询，使用线段树或者树状数组维护。
在查询的时候，我们还需要知道，深度最大的满足 $siz_u \ge k$ 的点，因为是从 $n$ 开始向上爬的，所以 $siz$ 具有单调性，可以二分 + 树上倍增求出。

```c++
// 离线 对最后的字符串的next数组建border树
// 每次有添加字符的操作，使对应点的权值+1
// 查询某个border的出现次数，只需要知道它的子树大小
// 使用dfs序，把上述问题变成 单点修改 + 区间查询问题
// 对于每个查询，需要从当前字符串长度 n 开始向上爬树，找到第一个大小 >= k 的位置
// 满足单调性，使用二分（树上倍增）

#include <bits/stdc++.h>
using namespace std;

struct Qr {
    int op, k;
    char ch;
};

const int N = 400010;
int n, q;
string S;
int nxt[N];
Qr Q[N];
vector<int> g[N];
int in[N], out[N], timestamp;
int f[N][20];
int c[N];

void get_next (string &s) {
    nxt[1] = 0;
    for (int i = 2; i < s.size(); i ++ ) {
        nxt[i] = nxt[i-1];
        while (nxt[i] && s[i] != s[nxt[i] + 1]) nxt[i] = nxt[nxt[i]];
        if (s[i] == s[nxt[i] + 1]) ++ nxt[i];
    }
}

// 获得树上的dfs序
void dfs (int u, int p) {
    f[u][0] = p;
    for (int i = 1; i < 20; i ++ )
        if (f[u][i-1] != -1)
            f[u][i] = f[f[u][i-1]][i-1];
    in[u] = ++ timestamp;
    for (int v: g[u]) if (v != p) dfs(v, u);
    out[u] = timestamp;
}

void add (int x, int v) {
    for (; x < N; x += x & -x) c[x] += v;
}

int query (int x) {
    int ans = 0;
    for (; x; x -= x & -x) ans += c[x];
    return ans;
}

int query (int l, int r) {
    return query(r) - query(l - 1);
}

int qr (int p) {
    return query(in[p], out[p]);
}

int up (int x) {
    int p = n;
    for (int i = 19; i >= 0; i -- ) {
        if ((1 << i) <= x) {
            p = f[p][i], x -= (1 << i);
            if (p == -1) break;
        }
    } return p;
}

bool check (int x, int k) {
    int p = up(x);
    if (p == -1 || qr(p) >= k) return true;
    else return false;
}

int main () {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q >> S; S = ' ' + S;
    for (int i = 1; i <= q; i ++ ) {
        cin >> Q[i].op;
        if (Q[i].op == 1) cin >> Q[i].ch, S += Q[i].ch;
        else cin >> Q[i].k;
    }
    get_next(S);
    for (int i = 1; i < S.size(); i ++ ) g[nxt[i]].push_back(i);
    memset(f, -1, sizeof f);
    dfs(0, -1);
    for (int i = 1; i <= n; i ++ ) add(in[i], 1);
    for (int i = 1; i <= q; i ++ ) {
        int op = Q[i].op, k = Q[i].k;
        char ch = Q[i].ch;
        if (op == 1) {
            add(in[++ n], 1);
        } else {
            int l = 0, r = N;
            while(l < r) {
                int mid = (l + r) >> 1;
                if (check(mid, k)) r = mid;
                else l = mid + 1;
            }
            if (r == 1e9 + 10 || !up(r)) cout << -1 << '\n';
            else cout << up(r) << '\n';
        }
    }
    return 0;
}
```