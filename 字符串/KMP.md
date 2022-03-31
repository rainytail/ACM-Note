# KMP

## 基础KMP

详解(推荐)：https://www.acwing.com/solution/content/14666/

模板，求所有被匹配的位置：

```c++
#include <bits/stdc++.h>
using namespace std;

const int N = 100010, M = 1000010;
int n, m;
char p[N], s[M];
int nxt[N]; // KMP最重要的部分，next数组

int main ()
{
    scanf("%d%s%d%s", &n, p+1, &m, s+1);

    // get next array
    for (int i = 2, j = 0; i <= n; i ++ ) {
        while(j && p[i] != p[j+1]) j = nxt[j];
        if (p[i] == p[j+1]) ++ j;
        nxt[i] = j;
    }

    // match
    for (int i = 1, j = 0; i <= m; i ++ ) {
        while(j && s[i] != p[j+1]) j = nxt[j];
        if (s[i] == p[j+1]) ++ j;
        if (j == n) {
            printf("%d ", i - n);
            j = nxt[j];
        }
    }
    return 0;
}
```



## 最小循环节

> 引理：$S[1 \sim i]$ 具有长度为 $len < i$ 的循环元的充要条件是 $len$ 能整除 $i$ 并且 $S[len + 1 \sim i] = S[1 \sim i - len]$  。

我们对字符串 $S$ 自匹配得到的 $next$ 数组，根据定义，对每个 $i$ 有 $S[i - next[i] + 1 \sim i] = S[1 \sim next]$ 。并且不存在更大的 $next$ 值满足此条件。

那么，当 $i - next[i]$ 能够整除 $i$ ，$S[1 \sim i - next[i]]$ 就是 $S[1 \sim i]$ 的最小循环元，它的循环次数为 $i / (i - next[i])$ 。

更进一步地，如果 $i - next[next[i]]$ 能够整除 $i$ ，那么$S[1 \sim i - next[next[i]]]$ 就是$S[1 \sim i]$ 的次小循环元。

注意，一个字符串的任意循环元的长度必然是最小循环元的整数倍。



## 最小表示法

> 给定一个字符串 $S[1 \sim n]$ ，不断将其最后一个字符放到开头，得到 $n$ 个字符串，这 $n$ 个字符串循环同构，其中字典序最小的字符串被称为 $S$ 的最小表示。

```c++
const int N = 100010;
char S[N << 1]; // 原字符串要复制一份
// 返回最小表示的第一个字符位置
int min_repr ()
{
    int n = strlen(S + 1);
    memcpy(S + n + 1, S + 1, sizeof(char) * n);
    int i = 1, j = 2, k = 0;
    while(k < n && i <= n && j <= n) {
        if (S[i + k] == S[j + k]) ++ k;
        else {
            if (S[i + k] > S[j + k]) i = i + k + 1;
            else j = j + k + 1;
            if (i == j) ++ i; // 如果相同，把其中一个换成下一个表示即可
            k = 0; // 重新匹配
        }
    }
    return min(i, j);
}
```

