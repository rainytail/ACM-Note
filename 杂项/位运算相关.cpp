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

位运算 子集枚举
二进制枚举  [0, (1>>n) - 1]

S ∪ {i}: S|1<<i
S ∩ T: S&T
S \ {i}: S&^(1<<i)

检测是否状态内只存在一个0: S&(S-1) == 0

异或和相关
https://www.luogu.com.cn/problem/P4462 异或前缀
*/

/*
bitset 用法：bitset一个模板类，相当于若干个位合在一起，存储每一位用一个 bit ，所以常数为 1/32
参考  https://en.cppreference.com/w/cpp/utility/bitset
bitset 可以直接输入，如 cin >> bit
*/

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