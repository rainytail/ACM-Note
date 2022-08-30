AC自动机的fail树，本质是由fail指针连成的边构成的有向无环图。
树上的每个节点代表一个模式串的前缀，父节点表示的串是子节点表示串的后缀，这样能保证，子节点如果被匹配到，那么父节点也一定被匹配到。

在 build AC自动机时，fail指针实际上充当了KMP中next指针的作用，用当前的前缀串 $S$ 去匹配一个 $S$ 的后缀。
和next的求法有所不同，fail指针没用用到while，每次向上跑trie树直到一个节点有当前字符的子节点；而是直接对不存在的点建立虚拟节点，对后缀的当前字符点(也就是fail指针对应的位置)连边（共享同一份地址，在我的理解里这个trie树就不算是严格意义下的有向无环图了）。

匹配模式串流程：
1. 建立trie树，对模式串插入trie树中
2. 对trie树建立AC自动机，同时建立fail树
3. 把文本串 $S$ 放入AC自动机中，求得每个前缀在trie中匹配的前缀，在当前点贡献 + 1
4. 在fail树上求每个节点子树权值和，就是表示这个节点对应的前缀在文本串 $S$ 中出现的次数

AC自动机的复杂度为 $O(\sum {|t_i|} + |S|)$ ，其中 $t_i$ 表示模式串，$S$ 表示文本串。

还有一点需要注意：在AC自动机里，字符串一般以 $0$ 为下标起点。

```c++
const int N = 1000010, M = 1000010;
struct ACT {
    struct trie {
        int fail;
        int ch[26] = {0};
    };
    int idt[M]; // 每个模式串对应的trie上的编号
    trie tr[M]; int cnt;
    vector<int> g[M]; // fail树
    int tag[M]; // trie树上节点被遍历到的次数
    
    void insert (string &s, int i) {
        int p = 0;
        for (int i = 0; i < s.size(); i ++ ) {
            int id = s[i] - 'a';
            if (!tr[p].ch[id]) tr[p].ch[id] = ++ cnt;
            p = tr[p].ch[id];
        }
        idt[i] = p;
    }

    void build () {
        queue<int> q;
        for (int i = 0; i < 26; i ++ )
            if (tr[0].ch[i]) q.push(tr[0].ch[i]);
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for (int i = 0; i < 26; i ++ ) {
                if (tr[u].ch[i]) {
                    tr[tr[u].ch[i]].fail = tr[tr[u].fail].ch[i];
                    q.push(tr[u].ch[i]);
                    // 建立fail树
                    int v = tr[u].ch[i], fa = tr[tr[u].fail].ch[i];
                    g[fa].push_back(v);
                } else tr[u].ch[i] = tr[tr[u].fail].ch[i];
            }
        }
    }

    void query (string &s) {
        int p = 0;
        for (char c: s) {
            int id = c - 'a';
            p = tr[p].ch[id];
            tag[p] += 1;
        } dfs(0);
    }

    void dfs (int u) {
        for (int v: g[u]) {
            dfs(v);
            tag[u] += tag[v];
        }
    }
};
```