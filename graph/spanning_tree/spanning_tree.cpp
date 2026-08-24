// 全域木の構築 (無向グラフから全域木を1つ取り出す)
// 重みなし無向グラフの全域木 (非連結なら全域森) を1つ求め, 辺リストを返す
// 応用: 任意のグラフを木に帰着 (木DP / オイラーツアー / LCA / 辺の彩色 など)
//       "N-1 本の辺を選んで連結にせよ" 型の構築問題
// 計算量: O(V + E) (BFS版) / O(E α(V)) (Union-Find版)
#include <bits/stdc++.h>
using namespace std;

// === ライブラリ: 全域木 (BFS版, 隣接リスト) ===
// 返り値: 全域木 (森) の辺リスト {親, 子}
// 各連結成分の最小番号の頂点を根とする BFS木. 辺の向きは根から遠ざかる方向
// 連結なら辺数は n-1, 非連結なら n - (成分数)
vector<pair<int,int>> spanning_tree_bfs(const vector<vector<int>>& g){
    int n = g.size();
    vector<pair<int,int>> tree;
    vector<bool> visited(n, false);
    for(int s = 0; s < n; s++){
        if(visited[s]) continue;
        queue<int> q;
        q.push(s);
        visited[s] = true;
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int u : g[v]){
                if(visited[u]) continue;
                visited[u] = true;
                tree.push_back({v, u});
                q.push(u);
            }
        }
    }
    return tree;
}

// === ライブラリ: 全域木 (Union-Find版, 辺リスト) ===
// edges[i] = {u, v} を入力順に見て, 閉路を作らない辺を採用
// 返り値: 採用した辺の添字 (元の辺番号を出力する問題向け)
// 辺を任意の順に並べ替えてから渡せば, その優先順位で全域木が作れる
struct UnionFind {
    vector<int> par;
    UnionFind(int n) : par(n) { iota(par.begin(), par.end(), 0); }
    int find(int x){ return par[x] == x ? x : par[x] = find(par[x]); }
    bool unite(int x, int y){
        x = find(x); y = find(y);
        if(x == y) return false;
        par[y] = x;
        return true;
    }
};

vector<int> spanning_tree_uf(int n, const vector<pair<int,int>>& edges){
    UnionFind uf(n);
    vector<int> used;
    for(int i = 0; i < (int)edges.size(); i++){
        auto [u, v] = edges[i];
        if(uf.unite(u, v)) used.push_back(i);
    }
    return used;
}

// === ライブラリ: 全域木を隣接リストに変換 ===
// 木DP などに続けて使うとき用
vector<vector<int>> tree_to_adj(int n, const vector<pair<int,int>>& tree){
    vector<vector<int>> t(n);
    for(auto [u, v] : tree){
        t[u].push_back(v);
        t[v].push_back(u);
    }
    return t;
}

// === 例題: N 頂点 M 辺の連結無向グラフから N-1 本の辺を選んで木にする ===
// 入力: N M, 続いて M 行 a_i b_i (1-indexed)
// 出力: 選んだ辺の番号 (1-indexed) を N-1 行
int main(){
    int N, M;
    cin >> N >> M;
    vector<pair<int,int>> edges(M);
    vector<vector<int>> g(N);
    for(int i = 0; i < M; i++){
        int a, b; cin >> a >> b;
        a--; b--;
        edges[i] = {a, b};
        g[a].push_back(b);
        g[b].push_back(a);
    }

    // Union-Find版: 辺番号を得る
    vector<int> used = spanning_tree_uf(N, edges);
    for(int i : used) cout << i + 1 << "\n";

    // BFS版: 辺リスト {親, 子} を得る (根 = 頂点0)
    // vector<pair<int,int>> tree = spanning_tree_bfs(g);
    // vector<vector<int>> t = tree_to_adj(N, tree); // 以降 木DP などに利用
    return 0;
}
