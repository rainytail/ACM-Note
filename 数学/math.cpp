#include <bits/stdc++.h>
using namespace std;
const int N = 100010;

// 欧拉筛法, O(n)筛素数
// 使用minp: https://codeforces.com/problemset/problem/1366/D
// 性质: x = p1^a1 + p2^a2 + ...
// gcd(p1*p2*...*px + p_x+1*...*pk, x) = 1
int prime[N], cnt, minp[N]; // minp记录某个数字最小质因子
bool st[N];
void getprimes (int n)
{
    for (int i = 2; i <= n; i ++ ) {
        if (!st[i]) {
            prime[cnt ++ ] = i;
            minp[i] = i;
        }
        // 筛去最小质因子为prime[j]的数字
        for (int j = 0; prime[j] <= n / i; j ++ ) {
            st[prime[j] * i] = true;
            minp[prime[j] * i] = prime[j];
            if (i % prime[j] == 0) break;
        }
    }
}

int main () {
    return 0;
}