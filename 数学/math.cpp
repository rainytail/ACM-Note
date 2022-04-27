#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 100010, mod = 114514, INF = 0x3f3f3f3f;
const double eps = 1e-8;

/* 数学相关
   
   交换求和技巧  https://zhuanlan.zhihu.com/p/499839696
*/

// 快速幂 log(n) 取模版本
int quick_power (int a, int k, int p = mod)
{
    int res = 1;
    for (a %= p; k; k >>= 1, a = (ll)a * a % p)
        if (k & 1) res = (ll)res * a % p;
    return res;
}

// 逆元
// 无逆元下求除法: 转化为若干个乘法  https://www.luogu.com.cn/problem/P4588
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
// 转换  https://www.luogu.com.cn/problem/P1445
int prime[N], cnt, minp[N]; // minp记录某个数字最小质因子
bool st[N];
void getprimes (int n)
{
    st[1] = true; // true 表示非质数
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

// 区间筛 每个合数都有一个较小的质数 < √x , 先筛出 [1, √] 内的所有质数，再用这些质数筛去 [l, r]
void getIntervalPrimes (int l, int r)
{
    getprimes(50010);
    memset(st, 0, sizeof st);
    for (int i = 0; i < cnt; i ++ ) {
        ll p = prime[i];
        for (ll j = max(2 * p, (l + p - 1) / p * p); j <= r; j += p)
            st[j - l] = true;
    }
    cnt = 0;
    for (int i = 0; i <= r - l; i ++ )
        if (!st[i] && i + l > 1)
            prime[cnt ++ ] = i + l;
}

// 欧拉函数 φ(n) 求 [1, n-1] 中与 n 互质的数量
// φ(n) = n * Π(1-1/pi) = n * Π(pi-1)/p1
int euler (int n)
{
    int ans = n;
    for (int i = 2; i <= n / i; i ++ ) {
        int s = 0;
        while(n % i == 0) ++ s, n /= i;
        if (s) ans = ans / i * (i - 1);
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}
// 筛法求欧拉函数
// 欧拉函数是积性函数，可以使用欧拉筛
// https://www.luogu.com.cn/problem/P2398
int phi[N];
void getEulers (int n)
{
    phi[1] = 1;
    for (int i = 2; i <= n; i ++ ) {
        if (!st[i]) {
            prime[cnt ++ ] = i;
            phi[i] = i - 1;
        }
        // 筛去最小质因子为prime[j]的数字
        for (int j = 0; prime[j] <= n / i; j ++ ) {
            st[prime[j] * i] = true;
            if (i % prime[j] == 0) {
                // pj 是 i 的最小质因子，根据定义求出phi
                phi[i * prime[j]] = phi[i] * prime[j];
                break;
            }
            phi[i * prime[j]] = phi[i] * phi[prime[j]];
        }
    }
}
// 欧拉函数相关
// 欧拉定理  x^phi(m) = 1 (mod m)
// 注意这个 phi(m) 不一定是余数为 1 的最小数字 应该是 phi(m) 的因数
// 例题  https://www.luogu.com.cn/problem/P4861

/* 欧拉函数性质
https://zhuanlan.zhihu.com/p/77880887
1. n > 2 时, phi(n) 是偶数
2. n > 1, [1, n] 中与 n 互质的数的和为 (n * phi(n) + [n == 1]) / 2
3. Σd|n phi(d) = n
*/

// 拓展欧拉定理
// - a 与 m 互质    a^b = a^(b % phi(m)) (mod m)
// - a 与 m 不互质  当 b > phi(m) 时: a^b = a^(b % phi(m) + phi(m)) (mod m)
// 例题  https://www.luogu.com.cn/problem/P4139


// BSGS 算法
// BSGS 用于求满足 a^x = b (mod m) 的 x 值 (要求 a 与 m 互质)
// 复杂度O(√m) 使用map会增加一个log
// https://oi-wiki.org/math/number-theory/bsgs/
// (推荐) https://zhuanlan.zhihu.com/p/132603308
// 一些题目
// 模板题  https://www.luogu.com.cn/problem/P3846
// 转换  C题 https://codeforces.com/gym/103486
ll BSGS (ll a, ll b, ll m)
{
    static unordered_map<ll, ll> hs;
    hs.clear();
    ll t = sqrt(m) + 1, cur = 1;
    for (int B = 1; B <= t; B ++ ) {
        (cur *= a) %= m;
        hs[cur * b % m] = B;
    }
    ll now = cur; // cur is a^t
    for (int A = 1; A <= t; A ++ ) {
        if (hs.contains(now)) { // contains C++20语法, 可换成 find
            return A * t - hs[now];
        }
        (now *= cur) %= m;
    }
    return -1; // 没有结果
}

// 拓展BSGS算法
// 求当 a 和 m 不是互质的情况
// (推荐) https://zhuanlan.zhihu.com/p/132603308
// NOTE 注意基础BSGS函数需要略微修改一些
// 模板题  https://www.luogu.com.cn/problem/P4195
ll BSGS (ll a, ll b, ll m, ll k = 1)
{
    static unordered_map<ll, ll> hs;
    hs.clear();
    ll t = sqrt(m) + 1, cur = 1;
    for (int B = 1; B <= t; B ++ ) {
        (cur *= a) %= m;
        hs[cur * b % m] = B;
    }
    ll now = cur * k % m; // 多乘上前面的系数
    for (int A = 1; A <= t; A ++ ) {
        if (hs.contains(now)) {
            return A * t - hs[now];
        }
        (now *= cur) %= m;
    }
    return -INF; // 因为最后会加上值，这里取负无穷
}
ll exBSGS (ll a, ll b, ll m, ll k = 1)
{
    ll A = a %= m, B = b %= m, M = m;
    if (b == 1) return 0;
    ll cur = 1 % m;
    for (int i = 0; ; i ++ ) {
        if (cur == B) return i; // 拓展BSGS算出的是可行解, 真正答案可能在 [0, i] 之间
        (cur *= A) %= M;
        ll d = gcd(a, m);
        if (b % d) return -INF;
        // 如果a和p已经互质了，可以使用基础BSGS
        if (d == 1) return BSGS(a, b, m, k) + i;
        k = k * a / d % m, b /= d, m /= d; // 递归求 BSGS(a, b/d, m/d, k)
    }
}


// 数论分块/整除分块
// Σ(1..n) n/i
ll floorLoop1D (int n)
{
    ll ans = 0;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans += (n / l) * (r - l + 1);
    }
    return ans;
}

// 二维分块
// Σ(1..n, 1..m) n/i * m/i
ll floorLoop2D (int n, int m)
{
    ll ans = 0;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = min(n / (n / l), m / (m / l));
        ans += (n / l) * (m / l) * (r - l + 1);
    }
    return ans;
}


// 莫比乌斯反演
// https://oi-wiki.org/math/number-theory/mobius/
// https://fanfansann.blog.csdn.net/article/details/113765056

// 定义mobius(n)
// -   1,      n = 1
// -   0,      n存在平方因子
// -   (-1)^k, k为本质不同的质因子
// 莫比乌斯函数的一些性质
// Σ(d|n) u(d) = [n == 1]

// 莫比乌斯函数为积性函数，可以使用筛法求出
// 莫反经常与数论分块一起出现
// 一些题目
// 模板题  GCD 相关
// https://www.luogu.com.cn/problem/P3455
// https://www.luogu.com.cn/problem/P2257
// 简单容斥 https://www.luogu.com.cn/problem/P2522


// 组合数相关
// 基本排列组合  https://www.cnblogs.com/RioTian/p/13556477.html
// 隔板法  https://www.cnblogs.com/lfri/p/10439510.html

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

// 容斥原理
// https://oi-wiki.org/math/combinatorics/inclusion-exclusion-principle/
// 如果题目有较多限制，可以先不考虑这些限制得到解，再容斥减去不符合限制的解
// Devu和鲜花  https://www.acwing.com/problem/content/description/216/
// 一些题目
// dfs容斥
// 幸运数字 https://www.luogu.com.cn/problem/P2567
ll InclusionExclusion (int n, int a[])
{
    ll ans = 0;
    for (int s = 0; s < (1 << n); s ++ ) {
        ll res = 0, sign = 0;
        for (int i = 0; i < n; i ++ ) {
            if (s >> i & 1) {
                // .....
                sign += 1;
            }
        }
        res *= sign & 1 ? 1 : -1;
        ans += res;
    }
    return ans;
}


// 代数相关

// 线性基
// https://oi-wiki.org/math/basis/
// https://ouuan.github.io/post/%E7%BA%BF%E6%80%A7%E5%9F%BA%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/
// https://oi.men.ci/linear-basis-notes/

// 线性基就是向量空间的一组基，通常用来解决异或相关的题目
// 用一组集合来构造另外一个集合(基)，这个集合满足:
// - 线性基的元素能相互异或得到原集合的所有元素互相异或得到的值
// - 线性基是满足性质1的最小集合
// - 线性基没有异或为0的子集，即线性无关
// - 线性基不同的异或组合，得到的数字都是不同的
// - 线性基中每个元素的二进制最高位都不相同(有点像阶梯型矩阵)

// 线性基(基内元素用ai表示)构造：
// 对原集合的每个数字x，从高位向低位扫描，如果当前位为1：
// - 如果ai不存在，令ai = p，结束扫描
// - 否则，令x = x xor ai
void insert (int x)
{
    int p[N]; // p 为线性基
    for (int i = 30; i >= 0; i -- ) {
        if (!(x >> i)) continue;
        if (!p[i]) {
            p[i] = x;
            break;
        }
        x ^= p[i];
    }
}
// 线性基求 xor 组合的最大值
// 把线性基理解为阶梯型矩阵，从高位向低位扫描，因为当前位置a_i的i位是1，所以如果能使得答案变大，就选择ai
// 模板题  https://www.luogu.com.cn/problem/P3812

// 线性基求 xor 组合最小值
// 答案就是线性基里元素的最小值

// 求某个区间是否有异或为0的子集
// 依次将元素插入到线性基，如果某个元素没有被插入，那么就是存在异或组合为这个元素的值，再与这个元素异或得到0
// NOTE 根据矩阵的解结构，如果元素只有 k 位，那么元素数量超过 k ，就一定有子集异或为 0
// 例题  https://codeforces.com/gym/103486/problem/I


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