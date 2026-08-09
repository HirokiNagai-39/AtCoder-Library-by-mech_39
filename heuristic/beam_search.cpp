// ビームサーチ (Beam Search)
// 用途: 多段階の意思決定問題で, 各ターン上位 W 個 (ビーム幅) の状態のみを残す貪欲探索.
//       AHC の逐次的な問題 (マラソン系), パズル, ゲーム AI などで有用.
// 計算量: O(T * W * 近傍数 * 状態コピー/評価コスト)
//   T: ターン数, W: ビーム幅
// ポイント: 重複除去 (ハッシュなど), 評価関数の設計, ビーム幅の調整が鍵.
// 例題: AHC や yukicoder のマラソン系問題
#include <bits/stdc++.h>
using namespace std;

// === ライブラリ: 汎用ビームサーチ ===
// State: 以下のインタフェースを要求:
//   double score() const;                          // 評価値 (大きいほど良い)
//   bool is_terminal() const;                      // 終了判定
//   vector<Action> candidate_actions() const;      // 次にとりうる行動
//   void apply(const Action& a);                   // 行動を適用
//   uint64_t hash() const;                         // 重複除去用 (不要なら 0)
// Action: 行動を表す任意の型 (POD 推奨. 復元用に親を辿るため軽量に).
template <typename State, typename Action>
State beam_search(const State& init, int beam_width, int max_turn) {
    struct Node {
        State state;
        double score;
    };
    vector<Node> beam;
    beam.push_back({init, init.score()});

    State best = init;
    double best_score = init.score();

    for (int turn = 0; turn < max_turn; turn++) {
        vector<Node> next_beam;
        next_beam.reserve(beam.size() * 4);

        for (auto& node : beam) {
            if (node.state.is_terminal()) {
                next_beam.push_back(node);
                continue;
            }
            for (auto& act : node.state.candidate_actions()) {
                State ns = node.state;
                ns.apply(act);
                next_beam.push_back({ns, ns.score()});
            }
        }
        if (next_beam.empty()) break;

        // 重複除去 (ハッシュが 0 でない場合のみ)
        {
            unordered_set<uint64_t> seen;
            vector<Node> uniq;
            uniq.reserve(next_beam.size());
            for (auto& n : next_beam) {
                uint64_t h = n.state.hash();
                if (h != 0 && !seen.insert(h).second) continue;
                uniq.push_back(move(n));
            }
            next_beam = move(uniq);
        }

        // 上位 beam_width 個を選ぶ (nth_element で O(N))
        if ((int)next_beam.size() > beam_width) {
            nth_element(next_beam.begin(),
                        next_beam.begin() + beam_width,
                        next_beam.end(),
                        [](const Node& a, const Node& b) {
                            return a.score > b.score;
                        });
            next_beam.resize(beam_width);
        }

        for (auto& n : next_beam) {
            if (n.score > best_score) {
                best_score = n.score;
                best = n.state;
            }
        }
        beam = move(next_beam);
    }
    return best;
}

// === 使用例: 1 次元のおもちゃ問題 ===
// 長さ N の数列 a[] が与えられ, 位置 p (初期 0) から毎ターン {+1, +2, -1} のいずれかへ動く.
// 訪れたマスの値の合計を最大化 (同じマスは一度だけカウント). T ターン終了.
struct ToyAction { int d; };

struct ToyState {
    int N, T, turn;
    int pos;
    double total;
    vector<int> a;
    vector<bool> visited;

    double score() const { return total; }
    bool is_terminal() const { return turn >= T; }

    vector<ToyAction> candidate_actions() const {
        vector<ToyAction> res;
        for (int d : {1, 2, -1}) {
            int np = pos + d;
            if (0 <= np && np < N) res.push_back({d});
        }
        return res;
    }
    void apply(const ToyAction& ac) {
        pos += ac.d;
        if (!visited[pos]) { total += a[pos]; visited[pos] = true; }
        turn++;
    }
    uint64_t hash() const {
        // (turn, pos, 簡易ビットマスクハッシュ)
        uint64_t h = (uint64_t)turn * 1000003 + pos;
        uint64_t bits = 0;
        for (int i = 0; i < N && i < 60; i++)
            if (visited[i]) bits |= (1ULL << i);
        return h ^ (bits * 0x9e3779b97f4a7c15ULL);
    }
};

int main() {
    int N, T;
    cin >> N >> T;
    ToyState init;
    init.N = N; init.T = T; init.turn = 0; init.pos = 0; init.total = 0;
    init.a.assign(N, 0);
    init.visited.assign(N, false);
    for (int i = 0; i < N; i++) cin >> init.a[i];
    init.total = init.a[0];
    init.visited[0] = true;

    ToyState ans = beam_search<ToyState, ToyAction>(init, 1000, T);
    cout << "best score: " << ans.score() << endl;
    return 0;
}
