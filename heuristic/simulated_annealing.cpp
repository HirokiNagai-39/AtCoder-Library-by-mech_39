// 焼きなまし法 (Simulated Annealing)
// 用途: 最適化問題のヒューリスティック (AHC など).
//       近傍解をランダムに生成し, 悪化する解も確率的に受け入れて局所解を脱出する.
// 温度 T を時間と共に下げる: T = T0 * (T1/T0)^(t/TL)
// 受理確率: exp(-ΔE / T) (最小化の場合, ΔE = new - cur)
// 例題: AHC 系問題全般 (例: AHC001 など)
#include <bits/stdc++.h>
using namespace std;

// === ライブラリ: xorshift (焼きなまし用) ===
struct XorShift {
    uint32_t x = 123456789, y = 362436069, z = 521288629, w;
    XorShift(uint32_t seed = 88675123) : w(seed) {}
    inline uint32_t next() {
        uint32_t t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    inline double next_double() { return (double)next() / (1ULL << 32); }
};

// === ライブラリ: 焼きなまし法 ===
// State: 状態クラス. 以下のインタフェースを要求:
//   double score() const;               // 現在スコア (最大化)
//   void neighbor(XorShift& rng);       // 近傍へ遷移 (直前の変更を undo 可能にする)
//   void undo();                        // 直前の neighbor の変更を戻す
// 最大化を前提. 最小化したい場合は score を負にするか受理条件を反転.
template <typename State>
void simulated_annealing(
    State& state,
    double time_limit_sec,
    double T_start = 1e3,
    double T_end = 1e0
) {
    XorShift rng(20250418);
    auto start = chrono::steady_clock::now();
    auto now_sec = [&]() {
        return chrono::duration<double>(
                   chrono::steady_clock::now() - start).count();
    };

    double best_score = state.score();
    State best_state = state;
    double cur_score = best_score;

    int iter = 0;
    while (true) {
        double t = now_sec() / time_limit_sec;
        if (t >= 1.0) break;
        // 温度スケジュール (指数スケジュール)
        double T = T_start * pow(T_end / T_start, t);

        state.neighbor(rng);
        double new_score = state.score();
        double delta = new_score - cur_score; // 最大化なので +が良い

        // 受理判定: 改善 or exp(delta/T) の確率で悪化を受理
        if (delta >= 0 || rng.next_double() < exp(delta / T)) {
            cur_score = new_score;
            if (cur_score > best_score) {
                best_score = cur_score;
                best_state = state;
            }
        } else {
            state.undo();
        }
        iter++;
    }
    state = best_state;
    cerr << "iter = " << iter << ", best = " << best_score << endl;
}

// === 使用例: TSP (巡回セールスマン) の焼きなまし ===
// N 都市の座標が与えられ, 全都市を一度ずつ訪れる巡路の最短距離を求める.
// 近傍: 2-opt (区間反転)
struct TSPState {
    int N;
    vector<vector<double>> dist;
    vector<int> order;
    // undo 用
    int last_l = -1, last_r = -1;

    TSPState(int N_, vector<vector<double>> d)
        : N(N_), dist(move(d)), order(N_) {
        iota(order.begin(), order.end(), 0);
    }

    double score() const {
        double s = 0;
        for (int i = 0; i < N; i++) s += dist[order[i]][order[(i + 1) % N]];
        return -s; // 最大化なので負号
    }

    void neighbor(XorShift& rng) {
        int l = rng.next() % N;
        int r = rng.next() % N;
        if (l > r) swap(l, r);
        if (l == r) { last_l = last_r = -1; return; }
        reverse(order.begin() + l, order.begin() + r + 1);
        last_l = l; last_r = r;
    }
    void undo() {
        if (last_l < 0) return;
        reverse(order.begin() + last_l, order.begin() + last_r + 1);
    }
};

int main() {
    int N;
    cin >> N;
    vector<double> X(N), Y(N);
    for (int i = 0; i < N; i++) cin >> X[i] >> Y[i];
    vector<vector<double>> d(N, vector<double>(N));
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
        d[i][j] = hypot(X[i] - X[j], Y[i] - Y[j]);

    TSPState state(N, d);
    simulated_annealing(state, 1.9, 100.0, 0.1);

    for (int v : state.order) cout << v << " ";
    cout << endl;
    return 0;
}
