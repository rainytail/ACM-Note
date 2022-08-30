# [B. Paranoid String](https://codeforces.com/contest/1694/problem/B)

**题意**

对于一字符串，每次操作可以选择：

+ 选择任意形如 "01" 的子序列，将其变为 “1”
+ 选择任意形如 "10" 的子序列，将其变为 "0"

如果能将字符串变为一个字符，则称这个字符串为好字符串。

给定长度为 $n$ 的字符串，问有多少个子串是好字符串？

**分析**

首先，对于一个字符串，**只要最后两个字符不相同，就能通过若干次操作变为1个字符**。

> 对于字符串 $11000111000101$ (最后两个字符不同)，我们一定可以把它们变换成 01010101 这样01交替的字符串(从最后一个字符开始，把前面的连续相同的变为1个，然后从这个字符开始再对前面进行操作... )，那么只需要从前往后操作就可以只剩下最后一个字符了。

所以只需要从前往后遍历，如果当前字符和前面字符不相同，那么以这个字符为结尾的子串都为好字符串。

注意长度为 1 的子串一定也是好字符串。

```c++
#include <bits/stdc++.h>
using namespace std;

void solve ()
{
    long long n, ans = 0; cin >> n;
    string s; cin >> s;
    for (int i = n - 1; i >= 1; i -- ) {
        if (s[i] != s[i-1]) ans += i;
    }
    cout << ans + n << endl;
}

int main ()
{
    int t; cin >> t; while(t -- ) solve();
    return 0;
}
```





# [D. Fake Plastic Trees](https://codeforces.com/contest/1694/problem/D)

**题意：**
给出一棵根为 $1$ 的树，初始所有点的点权为 $0$，每次操作可以选择一个点 $v$，对于 $1 \sim v$ 路径上所有点，为它们的点权加上一个非递减序列。
要求最终第 $i$ 个点的点权必须满足 $l_i \le w_i \le r_i$ ，问：最少要操作几次？

**分析**
首先，所有点都要满足制约关系，那么选择的点一定是从下往上的，如果先选择上面的，那么下面的点还要多花操作次数去满足制约关系。

可以发现，每个节点最多只会被选中一次，比如叶子节点 $v$ ，直接让它加上 $r_v$ 即可满足，不需要多余操作。

对于所选择的路径序列 $a_1a_2a_3 \dots a_n$ ，我们一定会让 $a_n$ 变为 $r_n$ ，这样前面的可操作性就会更大，而且不会影响 $a_n$ 的制约关系。

定义 $dp(u)$ 表示对于以 $u$ 为根的子树，最少需要花费的操作次数。
**如果我们知道了儿子节点 $v$ 的增加量，就可以知道 $u$ 至多能加到多少** 。
使用 DFS 遍历所有儿子，把增加量累计，就是这个点 $u$ 最大的值。

如果这个值：
1. 小于 $l_u$ ，说明我们还需要对 $u$ 节点进行一次加法，根据制约关系，加到 $r_u$ 
2. 大于等于 $l_u$ ，假设最大增加量为 $sum$ ，那么这个点的点权就为：$min(sum, r_u)$ ，取最小是因为：如果最大增加量大于 $r_u$ ，为了满足制约关系，我们可以把这个点的增加量减小到 $r_u$ ，否则就直接取 $sum$ 。

**Code**
```c++
#include <bits/stdc++.h>
using namespace std;
#define int long long

const int N = 200010;
int n, l[N], r[N], ans;
vector<int> g[N];

int dfs (int u) {
    int sum = 0; // sum 为下面点对这个点加上的值
    for (int v: g[u]) {
        sum += dfs(v);
    }
    if (sum < l[u]) {
        ++ ans; // 在这个点还需要加上一次， 加到 r[u] ，对于上面的点，可以看作，u点直接加了一次 r[u] ，可以通过若干次加，合并成加的数字小于 r[u] 即可，所以肯定是使用 r[u]
        return r[u];
    }
    return min(sum, r[u]); // 假如假如的数据大于 r[u] ，只需要把下面的增加量改小一点就可以
}

void solve ()
{
    cin >> n;
    for (int i = 2; i <= n; i ++ ) {
        int x; cin >> x;
        g[x].push_back(i);
    }
    for (int i = 1; i <= n; i ++ ) {
        cin >> l[i] >> r[i];
    }
    ans = 0;
    dfs(1);
    cout << ans << '\n';
    for (int i = 1; i <= n; i ++ ) g[i].clear();
}

signed main ()
{
    cin.tie(0)->sync_with_stdio(0);
    int t; cin >> t; while(t -- ) solve();
    return 0;
}
```