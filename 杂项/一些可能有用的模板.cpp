// 线段树维护区间 mex
// 对每个前缀建立主席树, 每个权值上维护这个权值出现的最后的位置
// 求区间 mex, 对 [1, r] 的线段树, 找出第一个(也就是最小的, 因为是权值线段树)最后出现位置小于 l 的, 这个值就是区间 mex
// 注意离散化, 区间 mex 可能不出现在给定的数字中, 因此在 all 数组中需要加上
// 模板题  https://www.luogu.com.cn/problem/P4137
#include <bits/stdc++.h>
using namespace std;

#define lc t[p].ls
#define rc t[p].rs
struct Node { int ls, rs, mv; };
const int N = 400010, INF = 0x3f3f3f3f;
int n, m, a[N], b[N], ns, root[N], idx;
Node t[N * 40];

void pushup (int p) {
    t[p].mv = min(t[lc].mv, t[rc].mv);
}

int build (int p, int l, int r)
{
    if (!p) p = ++ idx;
    if (l == r) {
        t[p].mv = 0;
        return p;
    }
    int mid = (l + r) >> 1;
    lc = build(lc, l, mid);
    rc = build(rc, mid + 1, r);
    pushup(p); return p;
}

int update (int p, int l, int r, int x, int v)
{
    int q = ++ idx;
    t[q] = t[p];
    if (l == r) {
        t[q].mv = v;
        return q;
    }
    int mid = (l + r) >> 1;
    if (x <= mid) t[q].ls = update(t[p].ls, l, mid, x, v);
    else t[q].rs = update(t[p].rs, mid + 1, r, x, v);
    pushup(q); return q;
}

int query (int p, int l, int r, int k)
{
    if (!p || l == r) return l;
    int mid = (l + r) >> 1;
    if (t[lc].mv < k) return query(lc, l, mid, k);
    else return query(rc, mid + 1, r, k);
}

int main ()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i ++ ) {
        cin >> a[i];
        b[++ ns] = a[i];
        b[++ ns] = a[i] + 1;
    }
    b[++ ns] = 0;

    sort(b + 1, b + 1 + ns);
    ns = unique(b + 1, b + 1 + ns) - b - 1;

    root[0] = build(root[0], 1, ns);
    for (int i = 1; i <= n; i ++ ) {
        a[i] = lower_bound(b + 1, b + 1 + ns, a[i]) - b;
        root[i] = update(root[i-1], 1, ns, a[i], i);
    }

    while(m -- ) {
        int l, r; cin >> l >> r;
        cout << b[query(root[r], 1, ns, l)] << endl;
    }
    return 0;
}