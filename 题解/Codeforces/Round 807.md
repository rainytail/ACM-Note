# E. Mark ans Professor Koro
## 题意
给定长度为 $n$ 的序列 $a$ ，有如下操作：
+ 选择两个相同的数字 $x$ ，将其删除，并将 $x+1$ 加入序列

给出 $q$ 次询问，每次将 $a[k]$ 改成 $l$ ，问对于每个询问，序列中最大的数字为多少？
其中 $2 \le n \le 2 \times 10^5$ ，$1 \le q \le 2 \times 10^5$ ，$1 \le a_i \le 2 \times 10^5$ 。

## 分析
#线段树  #Trick-不变性 
每次将两个 **相同的数字x** 变为 **一个x+1** ，考虑改变后对序列的影响。
由于有 $2^x + 2^x = 2^{x+1}$ ，因此对于序列 $a$ 而言，**$\sum_{i=1}^{size} 2^{a_i}$** 是不变的。
考虑最后变成的序列为 $b$ ，长度为 $k$ ，且 $b_1 \lt b_2 \lt \cdots \lt b_k$ 则有：
$$
\begin{aligned}
\sum_{i=1}^{size} 2^{a_i} = 2^{b_1} + 2^{b_2} + \cdots + 2^{b_k}
\le 2^1 + 2^2 + \cdots + 2^{b_k} &\lt 2^{b_k+1}
\end{aligned}
$$

那么 $b_k = \lfloor log2(\sum_{i=1}^{size} 2^{a_i}) \rfloor$ 。

由于底数都一样(都是2)，直接维护二进制位置就可以，比如 $2^3$ ，其实就是第三位为 $1$ 。

我们维护这样一个二进制序列，由于有修改，可以使用线段树维护。
为了方便，反向维护二进制序列，即二进制第一位就在线段树维护区间的第一位。
加入 $2^x$ ，其实就是在第 $x$ 位上加一，注意如果有 $1$ ，还需要找到后面第一个 $0$ ，把它变成 $1$ ，然后中间的区间改为 $0$ ，减去 $2^x$ 也类似。（**其实就是模拟二进制高精度加减法**）

根据推出来的结果，**$b_k$ 就是最高位的 $1$ 所在的位置。**

## Code
```c++
#include <algorithm>  
#include <iostream>  
using namespace std;  
  
// 注意维护的是逆序的二进制 即从第一位开始  
// 区间修改，查询第一个1，单点查询  
#define ls p << 1  
#define rs p << 1 | 1  
#define mid ((t[p].l + t[p].r) >> 1)  
struct Node {  
    int l, r;  
    int sum, tag; // 1的数量，懒标记  
    int length() { return r - l + 1; }  
};  
  
const int N = 201010;  
int n, q, a[N];  
Node t[N << 2];  
  
void update (Node &rt, int v) {  
    rt.sum = rt.length() * v;  
    rt.tag = v;  
}  
  
void pushup (int p) {  
    t[p].sum = t[ls].sum + t[rs].sum;  
}  
  
void pushdown (int p) {  
    if (t[p].tag == -1) return ;  
    update(t[ls], t[p].tag), update(t[rs], t[p].tag);  
    t[p].tag = -1;  
}  
  
void build (int p, int l, int r) {  
    t[p].l = l, t[p].r = r; t[p].tag = -1;  
    if (l == r) return ;  
    build(ls, l, mid), build(rs, mid + 1, r);  
}  
  
void update (int p, int l, int r, int v) {  
    if (l <= t[p].l && r >= t[p].r) {  
        update(t[p], v);  
        return ;  
    }  
    pushdown(p);  
    if (l <= mid) update(ls, l, r, v);  
    if (r > mid)  update(rs, l, r, v);  
    pushup(p);  
}  
  
int query_sum (int p, int l, int r) {  
    if (l <= t[p].l && r >= t[p].r) return t[p].sum;  
    pushdown(p);  
    int ans = 0;  
    if (l <= mid) ans += query_sum(ls, l, r);  
    if (r > mid)  ans += query_sum(rs, l, r);  
    return ans;  
}  
  
// 找到第x个0  
int posit0 (int p, int x) {  
    if (t[p].l == t[p].r) return t[p].l;  
    pushdown(p);  
    if (t[ls].length() - t[ls].sum >= x) return posit0(ls, x);  
    return posit0(rs, x - (t[ls].length() - t[ls].sum));  
}  
  
int posit1 (int p, int x) {  
    if (t[p].l == t[p].r) return t[p].l;  
    pushdown(p);  
    if (t[ls].sum >= x) return posit1(ls, x);  
    return posit1(rs, x - t[ls].sum);  
}  
  
// for test  
int query_p (int p, int x) {  
    if (t[p].l == t[p].r) return t[p].sum;  
    pushdown(p);  
    if (x <= mid) return query_p(ls, x);  
    return query_p(rs, x);  
}  
  
void insert (int p, int x) {  
    if (t[p].l == t[p].r) {  
        if (t[p].sum == 1) {  
            int y = posit0(1, t[p].l - query_sum(1, 1, t[p].l) + 1);  
            insert(1, y);  
            if (t[p].l + 1 <= y - 1) update(1, t[p].l + 1, y - 1, 0);  
            t[p].sum = 0;  
        } else t[p].sum = 1;  
        return ;  
    }  
    pushdown(p);  
    if (x <= mid) insert(ls, x);  
    else insert(rs, x);  
    pushup(p);  
}  
  
void Minus (int p, int x) {  
    if (t[p].l == t[p].r) {  
        if (t[p].sum == 0) {  
            int y = posit1(1, query_sum(1, 1, t[p].l) + 1);  
            Minus(1, y);  
            if (t[p].l + 1 <= y - 1) update(1, t[p].l + 1, y - 1, 1);  
            t[p].sum = 1;  
        } else t[p].sum = 0;  
        return ;  
    }  
    pushdown(p);  
    if (x <= mid) Minus(ls, x);  
    else Minus(rs, x);  
    pushup(p);  
}  
  
int main () {  
    cin.tie(nullptr); cout.tie(nullptr);  
    ios::sync_with_stdio(false);  
    cin >> n >> q;  
    build(1, 1, N - 1);  
    for (int i = 1; i <= n; i ++ ) {  
        cin >> a[i];  
        insert(1, a[i]);  
    }  
    while(q -- ) {  
        int k, l; cin >> k >> l;  
        Minus(1, a[k]);  
        a[k] = l;  
        insert(1, a[k]);  
        cout << posit1(1, query_sum(1, 1, N - 1)) << '\n';  
    }  
    return 0;  
}
```