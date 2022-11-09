### E - Packing Potatoes
#### 题意
给定无限多的土豆，第 $i$ 个土豆的重量为 $w[i\%n]$ ，每袋土豆重量最多为 $x$ 。
$Q$ 次询问，第 $k$ 个袋子里有多少个土豆。

#### 分析
#环图 #循环节
对于每个袋子，由于称重都是 $x$ ，那么只要给定了初始值(选哪种土豆)，就可以知道它的数量。
土豆一共有 $n$ 种重量，那么袋子一共有 $n$ 种。对袋子出现的顺序连边，一共有 $n$ 条边，那么一定有环。
对于每个种类的袋子，二分出下一个袋子的种类（第一个使用的土豆是哪种）。
然后求出循环节。

#### Code
```c++
#include <bits/stdc++.h>
using namespace std;
#define int long long

const int N = 200010;
int n, q, x, w[N], ne[N], pre[N]; // ne(i)开头装i的包，下一个包的开头为多少
int re[N], cnt[N], ans[N]; // cnt表示i包有多少个，re表示当前包的遍历顺序

int check (int x) {
    return pre[n] * (x/n) + pre[x%n];
}

signed main () {
    cin >> n >> q >> x;
    for (int i = 0; i < n; i ++ ) {
        cin >> w[i]; pre[i+1] = pre[i] + w[i];
    }
    for (int i = 0; i < n; i ++ ) {
        int l = i + 1, r = i + x;
        while(l < r) {
            int mid = (l + r) >> 1;
            if (check(mid) - pre[i] >= x) r = mid;
            else l = mid + 1;
        }
        ne[i] = l % n; cnt[i] = l - i;
    }
    int sz = 0; // 环上包的数量
    int limit = 0; // 没有在环上的包数量
    int j = 0; // 当前枚举到多少包
    bitset<N> st;
    for (int i = 0; ; i = ne[i]) {
        if (st[i] == 1) {
            sz = j - re[i] + 1; limit = re[i] - 1;
            break;
        }
        st[i] = 1;
        ++ j;
        re[i] = j; ans[j] = cnt[i];
    }
    while(q -- ) {
        int k; cin >> k;
        if (k <= limit) cout << ans[k] << endl;
        else cout << ans[(k - limit - 1) % sz + limit + 1] << endl;
    }
    return 0;
}
```