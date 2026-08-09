// 高速乱択 (xorshift / xoshiro)
// 用途: ヒューリスティック (AHC) などで大量の乱数を高速に生成する
// std::mt19937 より数倍高速. 期待値ベースの乱択アルゴリズム全般で有用.
// 計算量: 1回あたり O(1) (mt19937 より 2〜4 倍速い)
#include <bits/stdc++.h>
using namespace std;

// === ライブラリ: 高速乱数生成器 (xorshift128) ===
struct XorShift {
    uint32_t x, y, z, w;
    XorShift(uint32_t seed = 88675123)
        : x(123456789), y(362436069), z(521288629), w(seed) {}

    // [0, 2^32) の一様整数
    inline uint32_t next() {
        uint32_t t = x ^ (x << 11);
        x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }
    // [0, n) の一様整数
    inline uint32_t next(uint32_t n) { return next() % n; }
    // [l, r) の一様整数
    inline int64_t next(int64_t l, int64_t r) {
        return l + (int64_t)(next() | ((uint64_t)next() << 32)) % (r - l);
    }
    // [0, 1) の一様実数
    inline double next_double() {
        return (double)next() / (1ULL << 32);
    }
    // [l, r) の一様実数
    inline double next_double(double l, double r) {
        return l + next_double() * (r - l);
    }
};

// === ライブラリ: xoshiro256++ (64bit, より高品質・高速) ===
struct Xoshiro256 {
    uint64_t s[4];
    Xoshiro256(uint64_t seed = 88675123) {
        // splitmix64 で初期化
        for (int i = 0; i < 4; i++) {
            seed += 0x9e3779b97f4a7c15ULL;
            uint64_t z = seed;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
            s[i] = z ^ (z >> 31);
        }
    }
    static inline uint64_t rotl(uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }
    inline uint64_t next() {
        uint64_t result = rotl(s[0] + s[3], 23) + s[0];
        uint64_t t = s[1] << 17;
        s[2] ^= s[0]; s[3] ^= s[1];
        s[1] ^= s[2]; s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], 45);
        return result;
    }
    inline uint64_t next(uint64_t n) { return next() % n; }
    inline double next_double() {
        // [0, 1) を返す (上位53bitを使う)
        return (next() >> 11) * (1.0 / (1ULL << 53));
    }
};

// === ライブラリ: ランダムシャッフル ===
template <typename T, typename RNG>
void random_shuffle(vector<T>& v, RNG& rng) {
    int n = (int)v.size();
    for (int i = n - 1; i > 0; i--) {
        int j = rng.next(i + 1);
        swap(v[i], v[j]);
    }
}

// === 使用例 ===
int main() {
    XorShift rng(chrono::steady_clock::now().time_since_epoch().count());

    // [0, 100) の整数
    cout << rng.next(100) << endl;
    // [0, 1) の実数
    cout << rng.next_double() << endl;
    // [-5, 5) の整数
    cout << rng.next(-5LL, 5LL) << endl;

    // シャッフル
    vector<int> v = {1, 2, 3, 4, 5};
    random_shuffle(v, rng);
    for (int x : v) cout << x << " ";
    cout << endl;
    return 0;
}
