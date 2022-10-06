# 2-SAT

## 问题描述
给定 $n$ 个二元组 $<A_1, A_2>$  ，每个二元组必须取其中一个值。
给定 $m$ 个限制条件，如 $A1$ 与 $C2$ 变量不能同时选择。
试求出一种方案，使得选择的 $n$ 的变量之间没有限制。

## 解法
对于两个二元组 $< A1, A2>$, $<B1, B2>$ 

假设现在不允许 $A1$ 和 $B2$ 同时选择，由于每个二元组必须选择一个变量，所以选择的方案如下：
1. 如果选择了 $A1$ ，那么**必须选择** $B1$
2. 如果选择了 $B2$ ，那么**必须选择** $A2$

我们建立一张 $2N$ 个节点的有向图，令第一个变量的取值范围为 $[1, n]$ , 第二个变量的取值范围为 $[n+1, n+n]$ 。
对于如上限制，我们连接 $A1 \to B1$ , $B2 \to A2$ 。

使用 Tarjan 算法求出缩点图。
则：如果一个二元组 $<A1, A2>$; 两个变量处于同一个缩点中，则表明：如果选择了 $A1$ , 那么必须选择 $A2$ ，产生矛盾，所以无解。

否则，对于缩点图上的拓扑序，如 $u \to v$ ，对 $u$ 赋值，则 $v$ 的值也确定了。
但是我们不能确定哪一种赋值( $0 \ or \ 1$ )是合法的。

NOTE: 选择一只合法方案，对这个方案取反，不一定也合法。

我们可以不断对缩点图上出度为 $0$ 的点进行赋值，因为对其赋值不会影响其他任何二元组的取值产生影响。
由于一定有解，所以通过这种方式能找到一种合法解。

上述过程其实是逆拓扑序，我们可以对缩点后的图建立反向图，作拓扑排序。
由于Tarjan算法本身找到的scc编号就是按逆拓扑序的(在反向图就是正拓扑序)，所以也可以直接判断每个变量的取值。
比如 $id[i] < id[i+n]$ ，由于 $i$ 指代第一个变量，而且拓扑序更早，所以可以选择第 $i$ 个二元组中第一个变量。(选择两个变量中拓扑序更小的)

如果题目要求合法方案必须包含某个变量，如 $<A1, A2>$ ， 二元组中必须选择 $A2$ ，那么可以连边 $A1 \to A2$ ，这样就保证了 $A1$ 的scc编号一定大于 $A2$ (tarjan对点的遍历顺序为 $1 \sim 2n$ )，那么就一定会选择拓扑序更小的 $A2$ 变量。
比如这一题：[婚礼](https://www.acwing.com/activity/content/problem/content/720/)

例题：[牧师约翰最忙碌的一天](https://www.acwing.com/activity/content/problem/content/687/)
```c++
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

const int N = 2010, M = 2000010;
int n;
int h[N], e[M], ne[M], idx;
int st[N], ed[N], last[N]; // 每场开始时间、结束时间、持续时间
int dfn[N], low[N], timestamp;
int scc_cnt, id[N];
int stk[N], top; bool in_stk[N];
int val[N]; // 每个二元组的取值

void add (int a, int b) {
    // cout << "add " << a << ' ' << b << endl;
    e[idx] = b; ne[idx] = h[a]; h[a] = idx ++ ;
}

bool intersect (int t1, int l1, int t2, int l2) {
    return !(t2 >= t1 + l1 || t2 + l2 <= t1);
}

void format (int x) {
    int hour = x / 60, minute = x % 60;
    if (hour < 10) cout << "0" << hour;
    else cout << hour;
    cout << ":";
    if (minute < 10) cout << "0" << minute;
    else cout << minute;
}

void tarjan (int u) {
    dfn[u] = low[u] = ++ timestamp;
    stk[++ top] = u; in_stk[u] = true;
    for (int i = h[u]; ~i; i = ne[i]) {
        int v = e[i];
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stk[v]) low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        ++ scc_cnt;
        int y;
        do {
            y = stk[top -- ];
            in_stk[y] = false;
            id[y] = scc_cnt;
        } while(y != u);
    }
}

int main () {
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++ ) {
        int st1, st2, ed1, ed2;
        scanf("%d:%d %d:%d %d", &st1, &st2, &ed1, &ed2, &last[i]);
        st[i] = st1 * 60 + st2;
        ed[i] = ed1 * 60 + ed2 - last[i];
    }
    memset(h, -1, sizeof h);
    bool check = true;
    // 判断矛盾
    // 在仪式前[1, n] 仪式后[n+1, n+n]
    for (int i = 1; i <= n; i ++ ) {
        for (int j = i + 1; j <= n; j ++ ) {
            // i头和j头重叠
            if (intersect(st[i], last[i], st[j], last[j])) {
                add(i, j+n), add(j, i+n);
            }
            // i尾和j尾重叠
            if (intersect(ed[i], last[i], ed[j], last[j])) {
                add(i+n, j), add(j+n, i);
            }
            // i头和j尾重叠
            if (intersect(st[i], last[i], ed[j], last[j])) {
                add(i, j), add(j+n, i+n);
            }
            // i尾和j头重叠
            if (intersect(ed[i], last[i], st[j], last[j])) {
                add(i+n, j+n), add(j, i);
            }
        }
    }
    for (int i = 1; i <= 2 * n; i ++ )
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; i ++ ) {
        if (id[i] == id[i+n]) return cout << "NO\n", 0;
    }
    cout << "YES\n";
    for (int i = 1; i <= n; i ++ ) val[i] = id[i] < id[i+n]; // 拓扑序小，则val为1，选择第一个变量
    for (int i = 1; i <= n; i ++ ) {
        if (val[i]) format(st[i]), cout << ' ', format(st[i] + last[i]), cout << endl;
        else format(ed[i]), cout << ' ', format(ed[i] + last[i]), cout << endl;
    }
    return 0;
}
```
