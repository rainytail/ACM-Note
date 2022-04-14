## Manacher算法

Manacher是一种**处理字符串最长回文子串**的算法，由Manacher于1975年发明。

Manacher算法与KMP思想类似，都是反复对以往已经处理过的信息的再次运用，以达到减少重复计算的目的。

在朴素算法中，我们枚举了每个中心点 $i$​​ 的位置，并从中心点由0不断增长，求出以 $i$​ 为中心点时的最长回文串，而Manacher正是对此的优化。

首先，回文串有奇偶之分，在朴素算法中需要分别处理，我们可以直接在每个字符前后加一个字符串中不出现的字符('#')，这样所有的回文串都变成了奇回文串，同时为了处理越界问题，需要在整个字符串前后加两个不同的字符(可以是'@'和'^'等)。

=="abac" -->"@#a#b#a#c#^"==

我们用 $p[i]$​​ ​表示以 $i$​​​​ 为中心点时的**最大回文半径**，即，对于上述例子，$p[4] = 4(b作为中心点)$​ 。​

假设我们正在处理以第 $i+1$ 为中心点的字符串，如果前 $i$ 个字符串中，第 $j$ 个字符串向右延伸地最长，为maxRange，存在下列两种情况：

1. $i+1<maxRange$​​ ，那么，根据回文串的对称性，在 $i+1$​​ 关于$j$​​对称的地方$(记作k)$​，$s[i] == s[k]$，我们可以用之前计算的 $k$ 来​​给 $i$ 赋一个初值。​ 

2. $i+1>=maxRange$​，直接记第 $i+1$​ 位初始值为1即可。



*Code*

```c++
ss[0] = '@'; ss[++len] = '#';
for (int i = 0; s[i]; i++)
{
    ss[++len] = s[i];
    ss[++len] = '#';
}
ss[++len] = '^';
for (int i = 1; i <= len; i++)
{
    if (maxRange > i) p[i] = min(maxRange - i, p[2 * id - i]);
    else p[i] = 1;
    while (s[i + p[i]] == s[i - p[i]]) p[i] ++ ;
    if (i + p[i] > maxRange) maxRange = i + p[i], id = i;
}
```

Manacher算法的时间复杂度时间复杂度为 $O(n)$，朴素算法时间复杂度为 $O(n^2)$。

时间复杂度证明：首先外层循环复杂度是 $O(n)$​ 的，只需要证明内层while循环总次数为 $O(n)$​ 级别即可。
对于每个中心点，如果它向右拓展了 $k$​ 次，那么对于后面的 $k$​ 个数字，每个都可以少拓展一次(赋初值)，while总循环次数为 $O(n)$​。所以总复杂度为 $O(n)$​。



例题链接: https://www.acwing.com/problem/content/description/1526/

```c++
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

const int N = 1010;

string s;
char ss[N << 1];
int p[N << 1], len;

int main ()
{
    getline(cin, s);
    ss[0] = '@';
    ss[++len] = '#';
    for (int i = 0; i < s.length(); i++)
    {
        ss[++len] = s[i];
        ss[++len] = '#';
    }
    ss[++len] = '^';
    int maxRange = -1, id = 0;
    for (int i = 1; i <= len; i++)
    {
        if (maxRange > i) p[i] = min(maxRange - i, p[2 * id - i]);
        else p[i] = 1;
        while (ss[i + p[i]] == ss[i - p[i]]) p[i]++;
        if (i + p[i] > maxRange) maxRange = i + p[i], id = i;
    }
    int res = 0;
    for (int i = 1; i <= len; i++) res = max(res, p[i] - 1); // id仅仅是拓展到最右边的位置，这个maxRange受到 i和p[i]共同影响，不能直接用p[id]-1作为结果
    cout << res << endl;
    return 0;
}
```

