#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 100010, mod = 114514;
const double eps = 1e-8;

// 快速幂 log(n) 取模版本
int quick_power (int a, int k, int p = mod)
{
    int res = 1;
    for (a %= p; k; k >>= 1, a = (ll)a * a % p)
        if (k & 1) res = (ll)res * a % p;
    return res;
}

// 逆元
// 费马小定理  a^-1 = a^(p-2) p为模数，要求p为质数
int inv (int x) { return quick_power(x, mod-2); }
// 拓展欧几里得 可以求出 a 和 p 非互质下 a 的逆元, 返回的 x%p 即为逆元
int exgcd (int a, int b, int &x, int &y)
{
    if (!b) return x = 1, y = 0, a;
    int r = exgcd(b, a % b, x, y);
    tie(x, y) = make_tuple(y, x - (a / b) * y);
    return r;
}

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

// 组合数相关
// 求组合数模板
// 1. C(a, b) = C(a-1, b-1) + C(a-1, b) 从 a 个苹果中选 b 个苹果, 要么选择第 1 个苹果， 要么不选择
// 2. C(a, b) = a! / (b! * (a-b)!) 存储阶乘
// 3. Lucas 定理：C(a, b) = C(a % p, b % p) * C(a / p, b / p) mod p
void getComb_1 ()
{
    int c[N][N];
    for (int i = 0; i < N; i ++ )
        for (int j = 0; j <= i; j ++ )
            if (!j) c[i][j] = 1;
            else c[i][j] = c[i-1][j-1] + c[i-1][j]; // % mod
}

int fac[N], infac[N];
void getComb_2 ()
{
    fac[0] = 1;
    for (int i = 1; i < N; i ++ ) fac[i] = 1ll * fac[i-1] * i % mod;
    // 倒推
    infac[N-1] = inv(fac[N-1]);
    for (int i = N-2; i >= 0; i -- ) infac[i] = 1ll * infac[i+1] * (i+1) % mod;
}
int C (int n, int m) {
    if (n < m) return 0; // 这一句记得加上
    return 1ll * fac[n] * infac[m] % mod * infac[n-m] % mod;
}

int lucas (int n, int m) {
    if (n < mod && m < mod) return C(n, m);
    return 1ll * C(n % mod, n % mod) * lucas(n / mod, m / mod) % mod;
}

// 求数值在 [0, dat - 1] 上的长度为 d 的前 n 个排列
// 适用于数值较小的情况
// https://codeforces.com/problemset/problem/459/C
void getPreviousArgment (int dat, int d, int n)
{
    int ret[N][N]; // ret(i, j) 第i个排列
    for (int i = 1; i < n; i ++ ) {
        for (int j = 0; j < d; j ++ ) ret[i][j] = ret[i-1][j];
        for (int j = d - 1; j >= 0; j -- ) {
            (ret[i][j] += 1) %= dat;
            if (ret[i][j]) break;
        }
    }
}

// 卡特兰数 Cn = C(2n, n) / (n+1) = C(2n, n) - C(2n, n-1)
// https://oeis.org/A000108
// 从 n = 0 开始: 	1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786,
// 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190, 6564120420,
// 24466267020, 91482563640, 343059613650, 1289904147324, 4861946401452, 18367353072152, 69533550916004,
// 263747951750360, 1002242216651368, 3814986502092304


// 代数相关

// 高斯定理 求解 n 个线性方程的解
int gauss (int n, double a[][n])
{
    int c, r; // 表示枚举的列和行
    for (c = 0, r; c < n; c ++ )
    {
        int t = r; // 找到当前这一列的绝对值最大的数字
        // 寻找绝对值最大的数字是为了防止系数过大而导致精度较高
        for (int i = t; i < n; i ++ )
            if (fabs(a[i][c]) > fabs(a[t][c]))
                t = i;

        if (fabs(a[t][c]) < eps) continue;

        // 把第t列换到最上面（第r行，因为r行上面已经固定了）
        for (int i = c; i < n + 1; i ++ ) swap(a[t][i], a[r][i]);

        // 把这一行c列的数字变成1，方便消除后面行的这一列数字
        for (int i = n; i >= c; i -- ) a[r][i] /= a[r][c];

        // 把当前列下面的数字全部变成0
        for (int i = r + 1; i < n; i ++ )
            if (fabs(a[i][c]) > eps) // 已经是0没必要改变
                for (int j = n; j >= c; j -- )
                    a[i][j] -= a[r][j] * a[i][c];

        // 固定好第r行，换下一行
        ++ r;
    }

    // 判断解的情况
    if (r < n) // 没有化简到最后一行，无解或者无限解
    {
        // 判断最后一行是否出现 0 = x 这样的式子
        for (int i = r; i < n; i ++ )
            if (fabs(a[i][n]) > eps)
                return 2; // 无穷解
        return 1; // 否则无解
    }

    // 由唯一解，从下往上求方程解
    for (int i = n - 1; i >= 0; i -- )
        for (int j = i + 1; j < n; j ++ )
            a[i][n] -= a[j][n] * a[i][j];
    return 0; // 唯一解
}

int main () {
    return 0;
}