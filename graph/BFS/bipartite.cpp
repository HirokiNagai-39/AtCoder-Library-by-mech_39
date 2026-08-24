// 二部グラフ判定 (BFS) + 奇数長閉路の復元
// グラフを2色で塗り分けられるか判定. 塗り分けられないときは奇数長閉路を1つ返す
// 例題: ABC327 D - Good Tuple Problem
// https://atcoder.jp/contests/abc327/tasks/abc327_d
// 計算量: O(V + E)
#include <bits/stdc++.h>
using namespace std;

// === ライブラリ: 二部グラフ判定 + 奇数長閉路復元 ===
// color[v] = 0 or 1 で2色に塗り分ける
// 二部グラフなら true を返し cycle は空
// 二部グラフでなければ false を返し, cycle に奇数長の単純閉路を頂点順で格納
//   (cycle[0] → cycle[1] → ... → cycle.back() → cycle[0] が閉路. 長さ = cycle.size())
//   自己ループなら cycle = {v} (長さ1)
// 復元: 同色の辺 (v,u) を見つけたら BFS木上で v, u から LCA まで登る.
//   v→LCA→u の木上パス + 辺 (u,v) が閉路. 同色 ⇔ 深さの偶奇が等しい ⇔ 長さが奇数
bool is_bipartite(const vector<vector<int>> &g, vector<int> &color, vector<int> &cycle)
{
    int n = g.size();
    color.assign(n, -1);
    cycle.clear();
    vector<int> par(n, -1), depth(n, 0);
    for (int s = 0; s < n; s++)
    {
        if (color[s] != -1)
            continue;
        color[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            for (int u : g[v])
            {
                if (color[u] == -1)
                {
                    color[u] = color[v] ^ 1;
                    par[u] = v;
                    depth[u] = depth[v] + 1;
                    q.push(u);
                }
                else if (color[u] == color[v])
                {
                    // 奇数長閉路 → 復元して終了
                    vector<int> pv, pu; // v側, u側のパス (LCA は pv 側にだけ含める)
                    int a = v, b = u;
                    while (depth[a] > depth[b])
                    {
                        pv.push_back(a);
                        a = par[a];
                    }
                    while (depth[b] > depth[a])
                    {
                        pu.push_back(b);
                        b = par[b];
                    }
                    while (a != b)
                    {
                        pv.push_back(a);
                        a = par[a];
                        pu.push_back(b);
                        b = par[b];
                    }
                    pv.push_back(a); // LCA
                    // v → ... → LCA → ... → u (→ v に戻る)
                    cycle = pv;
                    cycle.insert(cycle.end(), pu.rbegin(), pu.rend());
                    return false;
                }
            }
        }
    }
    return true;
}

// 閉路が不要なときの簡易版
bool is_bipartite(const vector<vector<int>> &g, vector<int> &color)
{
    vector<int> cycle;
    return is_bipartite(g, color, cycle);
}

// === 例題: ABC327 D ===
int main()
{
    int T;
    cin >> T;
    for (int hoge = 0; hoge < T; hoge++)
    {
        int N, M;
        cin >> N >> M;
        vector<int> A(M), B(M);
        for (int i = 0; i < M; i++)
        {
            cin >> A[i], A[i]--;
            cin >> B[i], B[i]--;
        }

        vector<vector<int>> g(N);
        for (int i = 0; i < M; i++)
        {
            g[A[i]].push_back(B[i]);
            g[B[i]].push_back(A[i]);
        }

        vector<int> color, cycle;
        if (is_bipartite(g, color, cycle))
        {
            cout << -1 << endl;
        }
        else
        {
            cout << cycle.size() << endl;
            for (int i = 0; i < (int)cycle.size(); i++)
            {
                cout << cycle[i] + 1 << (i + 1 < (int)cycle.size() ? " " : "\n");
            }
        }
    }
    return 0;
}
