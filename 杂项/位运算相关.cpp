#include <bits/stdc++.h>
using namespace std;

/*
https://oi-wiki.org/math/bit/

位运算常用等式
a|b = (a^b) + (a&b)    a^b = (a|b) - (a&b)
a+b = (a|b) + (a&b)
    = (a&b)*2 + (a^b)
    = (a|b)*2 - (a^b)
(a&b)^(a&c) = a&(b^c)

运算符优先级  https://www.sojson.com/operation/cxx.html

// is a power of 2
bool isPowerOfTwo (int n) {
    return n > 0 && (n & (n-1) == 0);
}

位运算 子集枚举
二进制枚举  [0, (1>>n) - 1]

位运算与集合相关
int A, B; // set
int c; // element
A |= 1<<c;       // insert c
A &= ~(1<<c);    // erase c
A ^= 1<<c;       // flip c
a & -a;          // lowbit of c
A = 0;           // empty set
A | B;           // union
A & B;           // intersection
int siz = 15;    // size of set
int ALL = (1<<siz)-1 // universal set
ALL ^ A          // complementary set of A
(A & B) == B;    // B is A's subset

// enumerate the subset of ALL
for (int i = 0; i <= ALL; i ++ )

// enumerate the subset of A
int subset = A;
do {
    // do something
    subset = (subset - 1) & A;
} while(subset);

// count the number of element in A
int cnt = 0;
for (int i = 0; i < si; i ++ )
    if (A >> i & 1) ++ cnt;
cnt;
// or
for (int i = A; i; i >>= 1)
    cnt += i & 1;

O(n) 求每个状态的 1 的数量
for (int i = 1; i <= ALL; i ++ )
    cnt[i] = cnt[i>>1] + (i & 1);

检测是否状态内只存在一个0: S&(S-1) == 0

异或和相关
//NOTE 根据异或和的性质, 对序列作前缀, 可以将区间异或和变成两点的异或和
构造前缀和来得到原序列 https://codeforces.com/problemset/problem/1174/D
异或前缀 https://www.luogu.com.cn/problem/P4462
*/

/*
bitset 用法：bitset一个模板类，相当于若干个位合在一起，存储每一位用一个 bit ，所以常数为 1/32
参考  https://en.cppreference.com/w/cpp/utility/bitset
bitset 可以直接输入，如 cin >> bit
*/

// bitset + dp: 我是仙人掌  https://www.luogu.com.cn/problem/P6328

/*
c++内置函数
参考  https://blog.csdn.net/jasonchen_gbd/article/details/44948523
__builtlin_ffs(x) x最后一位为1的位是从前往后的第几位
__builtlin_popcount(x) 返回x二进制下1的个数, 注意x为unsigned int类型  x为long long --> __builtlin_popcountll
__builtlin_ctz(x) x末尾0的个数
__builtlin_clz(x) x前导0的个数
__builtlin_parity(x) x二进制下1的奇偶性
*/

// 奇偶校验 https://codeforces.com/problemset/problem/1438/C

int main()
{
    return 0;
}