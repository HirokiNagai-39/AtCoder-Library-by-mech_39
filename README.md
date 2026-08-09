# AtCoder Library by mech_39

競技プログラミング（AtCoder）用の C++ ライブラリ・提出コード集です。

各ディレクトリには、アルゴリズムの **汎用テンプレート**（`algorithm_name.cpp`）と、それを使って解いた **AtCoder の問題の提出コード**（`ABC123_D_Problem_Name.cpp` など）が入っています。

## ディレクトリ構成

### `DP/` — 動的計画法
`basic_dp` / `knapsack` / `unbounded_knapsack` / `subset_sum` / `cumsum_dp` / `LIS` / `LCS` / `edit_distance` / `grid_dp` / `intervalDP` / `bitDP` / `TSP` / `digitDP` / `treeDP` / `rerooting` / `dag_dp` / `game_dp` / `expected_value` / `dp_restoration`

### `data_structure/` — データ構造
`union_find` / `BIT` / `segment_tree` / `lazy_segment_tree`

### `graph/` — グラフ
`BFS` / `DFS` / `dijkstra` / `bellman_ford` / `warshall_floyd` / `MST` / `LCA` / `SCC` / `topological_sort` / `euler_tour` / `max_flow`

### `math/` — 数学
`prime` / `gcd_lcm` / `ext_gcd` / `mod_pow` / `modint` / `combinatorics` / `floor_sum` / `discrete_log` / `ntt`

### `string/` — 文字列
`rolling_hash` / `Z_algorithm` / `manacher` / `trie`

### `search/` — 探索
`binary_search` / `two_pointers` / `brute_force` / `mos_algorithm`

### `others/` — その他頻出テクニック
`cumulative_sum_1d` / `cumulative_sum_2d` / `imos` / `coordinate_compression` / `doubling` / `bit_operation` / `stl_tips`

### `greedy/` — 貪欲法
`coin` / `interval_scheduling`

### `heuristic/` — ヒューリスティック（AHC 向け）
ビームサーチ / 焼きなまし / 高速乱数

## 環境

- C++20
- g++ (Homebrew GCC 15) で動作確認

```sh
g++ -std=c++20 -O2 -o a.out path/to/file.cpp
```
