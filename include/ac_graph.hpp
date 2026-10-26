#pragma once

#include "ac.hpp"

// LOCAL の core-only 構成でも、visual を後から include した順序を許容する。
// ac_visual.hpp は ac.hpp を再 include するが、pragma once で安全に戻る。
#if defined(LOCAL) && !defined(AC_VISUAL_HPP)
#include "ac_visual.hpp"
#endif

// Graph/Edge は「入力された1本の辺」と「隣接リスト上の向き」を同時に扱う。
// 頂点番号はライブラリ内部では常に 0-indexed。
// @acf using Edge — Edge<Cost> — グラフ辺（from/to/cost/id）。内部は 0-indexed
/// @acf-doc @brief `Edge<Cost>`。無向辺は隣接リストに両向きで現れ、同じ `id` を共有する
/// @acf-doc [type] Edge
template <class Cost = ll>
struct Edge {
    int from = -1;
    int to = -1;
    Cost cost = Cost{1};
    int id = -1;

    /// @brief 辺の端点・重み・IDがすべて一致するか比較する。
    friend bool operator==(const Edge&, const Edge&) = default;
};

// @acf using Graph — Graph<Cost> — 隣接リストと辺リストを持つグラフ型
/// @acf-doc @brief `Graph<Cost>`。`add_edge` / `reversed` / `edges`。要 `ac_graph.hpp`
/// @acf-doc [type] Graph
template <class Cost = ll>
struct Graph {
    using cost_type = Cost;
    using edge_type = Edge<Cost>;

    int n = 0;
    bool directed = false;
    vector<vector<edge_type>> adj;
    vector<edge_type> edges;  // 無向辺も入力1本につき1要素

    /// @brief 頂点数0の空グラフを構築する。
    Graph() = default;

    /// @brief `vertex_count` 頂点のグラフを構築する。頂点番号は0-indexed。
    explicit Graph(int vertex_count, bool is_directed = false)
        : n(vertex_count), directed(is_directed), adj(vertex_count) {
        assert(vertex_count >= 0);
    }

    /// @brief 頂点数を返す。
    int size() const { return n; }

    /// @brief 頂点数が0か判定する。
    bool empty() const { return n == 0; }

    /// @brief 頂点 `v` から出る隣接辺の一覧を返す（const版）。
    const vector<edge_type>& operator[](int v) const { return adj[v]; }

    /// @brief 頂点 `v` から出る隣接辺の一覧を返す。
    vector<edge_type>& operator[](int v) { return adj[v]; }

    /// @brief 辺を追加し、辺IDを返す。無向グラフでは逆向きの隣接辺も追加する。
    int add_edge(int from, int to, Cost cost = Cost{1}, int id = -1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        if (id < 0) id = static_cast<int>(edges.size());
        edge_type edge{from, to, cost, id};
        edges.push_back(edge);
        adj[from].push_back(edge);
        if (!directed) adj[to].push_back(edge_type{to, from, cost, id});
        return id;
    }

    /// @brief 有向グラフの全辺を逆向きにしたグラフを返す。無向グラフはコピーを返す。
    Graph reversed() const {
        if (!directed) return *this;
        Graph result(n, true);
        result.edges.reserve(edges.size());
        for (const auto& e : edges) result.add_edge(e.to, e.from, e.cost, e.id);
        return result;
    }
};

namespace ac_graph_detail {
template <class Cost>
Cost add_clamped(Cost a, Cost b, Cost inf) {
    if constexpr (is_integral_v<Cost> && sizeof(Cost) <= sizeof(ll)) {
        i128 sum = static_cast<i128>(a) + static_cast<i128>(b);
        if (sum > static_cast<i128>(inf)) return inf;
        if constexpr (is_signed_v<Cost>) {
            if (sum < -static_cast<i128>(inf)) return static_cast<Cost>(-inf);
        }
        return static_cast<Cost>(sum);
    } else {
        return a + b;
    }
}
}  // namespace ac_graph_detail

// 既存の vvi 隣接リストとの相互変換。
// 無向 vvi は read_graph と同様、各辺が両端のリストに入っている形を受け取る。
// @acf fn to_graph — (vvi[, directed]) -> Graph — 旧 vvi 隣接リストから Graph を作る
/// @acf-doc @brief 旧 vvi 隣接リストから Graph を作る
/// @acf-doc @code (vvi[, directed]) -> Graph
/// @acf-doc [function] to_graph
inline Graph<> to_graph(const vvi& adjacency, bool directed = false) {
    const int n = static_cast<int>(adjacency.size());
    Graph<> graph(n, directed);
    if (directed) {
        for (int from = 0; from < n; ++from) {
            for (int to : adjacency[from]) {
                assert(0 <= to && to < n);
                graph.add_edge(from, to);
            }
        }
        return graph;
    }

    map<pair<int, int>, pair<int, int>> counts;
    for (int from = 0; from < n; ++from) {
        for (int to : adjacency[from]) {
            assert(0 <= to && to < n);
            if (from < to) ++counts[{from, to}].first;
            else if (to < from) ++counts[{to, from}].second;
            else ++counts[{from, from}].first;
        }
    }
    for (const auto& [ends, count] : counts) {
        auto [from, to] = ends;
        if (from == to) {
            assert(count.first % 2 == 0 &&
                   "undirected self-loop must appear twice in adjacency");
            for (int i = 0; i < count.first / 2; ++i) graph.add_edge(from, to);
        } else {
            assert(count.first == count.second &&
                   "undirected adjacency must be symmetric");
            for (int i = 0; i < count.first; ++i) graph.add_edge(from, to);
        }
    }
    return graph;
}

template <class Cost>
// @acf fn to_adjacency_list — (Graph) -> vvi — Graph を行き先だけの隣接リストへ
/// @acf-doc @brief Graph を行き先だけの隣接リストへ
/// @acf-doc @code (Graph) -> vvi
/// @acf-doc [function] to_adjacency_list
vvi to_adjacency_list(const Graph<Cost>& graph) {
    vvi adjacency(graph.n);
    for (int v = 0; v < graph.n; ++v) {
        adjacency[v].reserve(graph[v].size());
        for (const auto& e : graph[v]) adjacency[v].push_back(e.to);
    }
    return adjacency;
}

template <class Cost>
// @acf fn graph_from_edges — (n, edges[, directed, index_base]) -> Graph — 辺リストから Graph
/// @acf-doc @brief 辺リストから Graph
/// @acf-doc @code (n, edges[, directed, index_base]) -> Graph
/// @acf-doc [function] graph_from_edges
Graph<Cost> graph_from_edges(
    int n, const vector<tuple<int, int, Cost>>& edges,
    bool directed = false, int index_base = 0) {
    Graph<Cost> graph(n, directed);
    graph.edges.reserve(edges.size());
    for (const auto& [input_from, input_to, cost] : edges)
        graph.add_edge(input_from - index_base, input_to - index_base, cost);
    return graph;
}

template <class Cost>
// @acf fn to_edge_list — (Graph[, index_base]) -> vector<tuple> — Graph を辺リストへ
/// @acf-doc @brief Graph を辺リストへ
/// @acf-doc @code (Graph[, index_base]) -> vector<tuple>
/// @acf-doc [function] to_edge_list
vector<tuple<int, int, Cost>> to_edge_list(
    const Graph<Cost>& graph, int index_base = 0) {
    vector<tuple<int, int, Cost>> result;
    result.reserve(graph.edges.size());
    for (const auto& e : graph.edges)
        result.emplace_back(e.from + index_base, e.to + index_base, e.cost);
    return result;
}

template <class Cost>
// @acf fn is_tree — (Graph) -> bool — 無向・連結・辺数 N-1 か
/// @acf-doc @brief 無向・連結・辺数 N-1 か
/// @acf-doc @code (Graph) -> bool
/// @acf-doc [function] is_tree
bool is_tree(const Graph<Cost>& graph) {
    if (graph.directed || graph.n == 0 ||
        static_cast<int>(graph.edges.size()) != graph.n - 1) return false;
    vi seen(graph.n);
    vector<int> stack{0};
    seen[0] = 1;
    int visited = 0;
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        ++visited;
        for (const auto& e : graph[v]) {
            if (seen[e.to]) continue;
            seen[e.to] = 1;
            stack.push_back(e.to);
        }
    }
    return visited == graph.n;
}

// a b を m 行読む。index_base=1 が AtCoder 標準、0 なら変換しない。
// @acf fn read_unweighted_graph — (n,m[,directed,index_base]) -> Graph — a b を m 行読む
/// @acf-doc @brief a b を m 行読む
/// @acf-doc @code (n,m[,directed,index_base]) -> Graph
/// @acf-doc [function] read_unweighted_graph
inline Graph<> read_unweighted_graph(
    int n, int m, bool directed = false, int index_base = 1) {
    assert(n >= 0 && m >= 0);
    Graph<> graph(n, directed);
    graph.edges.reserve(m);
    for (int id = 0; id < m; ++id) {
        int from, to;
        cin >> from >> to;
        from -= index_base;
        to -= index_base;
        graph.add_edge(from, to, 1, id);
    }
    return graph;
}

// a b cost を m 行読む。Cost は ll 以外（double 等）にもできる。
// @acf fn read_weighted_graph — (n,m[,directed,index_base]) -> Graph — a b cost を m 行読む
/// @acf-doc @brief a b cost を m 行読む
/// @acf-doc @code (n,m[,directed,index_base]) -> Graph
/// @acf-doc [function] read_weighted_graph
template <class Cost = ll>
Graph<Cost> read_weighted_graph(
    int n, int m, bool directed = false, int index_base = 1) {
    assert(n >= 0 && m >= 0);
    Graph<Cost> graph(n, directed);
    graph.edges.reserve(m);
    for (int id = 0; id < m; ++id) {
        int from, to;
        Cost cost;
        cin >> from >> to >> cost;
        from -= index_base;
        to -= index_base;
        graph.add_edge(from, to, cost, id);
    }
    return graph;
}

// @acf fn read_unweighted_tree — (n[,index_base]) -> Graph — 木（n-1 辺）を読む
/// @acf-doc @brief 木（n-1 辺）を読む
/// @acf-doc @code (n[,index_base]) -> Graph
/// @acf-doc [function] read_unweighted_tree
inline Graph<> read_unweighted_tree(int n, int index_base = 1) {
    return read_unweighted_graph(n, max(0, n - 1), false, index_base);
}

// @acf fn read_weighted_tree — (n[,index_base]) -> Graph — 重み付き木を読む
/// @acf-doc @brief 重み付き木を読む
/// @acf-doc @code (n[,index_base]) -> Graph
/// @acf-doc [function] read_weighted_tree
template <class Cost = ll>
Graph<Cost> read_weighted_tree(int n, int index_base = 1) {
    return read_weighted_graph<Cost>(n, max(0, n - 1), false, index_base);
}

// 入力形式へ戻したいときの出力。無向辺は1回だけ出力する。
// @acf fn write_graph — (os, Graph[, index_base, with_cost]) — 辺リスト形式で出力
/// @acf-doc @brief 辺リスト形式で出力
/// @acf-doc @code (os, Graph[, index_base, with_cost])
/// @acf-doc [function] write_graph
template <class Cost>
void write_graph(ostream& os, const Graph<Cost>& graph,
                 int index_base = 1, bool with_cost = true) {
    for (const auto& e : graph.edges) {
        os << e.from + index_base << ' ' << e.to + index_base;
        if (with_cost) os << ' ' << e.cost;
        os << '\n';
    }
}

template <class Dist>
struct ShortestPathResult {
    vector<Dist> dist;
    vi parent;
    vi parent_edge;
    Dist inf{};

    /// @brief 頂点 `v` が始点のいずれかから到達可能か判定する。
    bool reachable(int v) const { return dist[v] != inf; }

    /// @brief `target` までの頂点列を始点から復元する。到達不能なら空を返す。
    vi path_to(int target) const {
        if (target < 0 || target >= static_cast<int>(dist.size()) ||
            !reachable(target)) return {};
        vi path;
        for (int v = target; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }

    // @acf fn edge_path_to — (target) -> vi — 最短路上の辺ID列を復元
    /// @brief `target` までの最短路に使う辺ID列を復元する。到達不能なら空を返す。
    vi edge_path_to(int target) const {
        if (target < 0 || target >= static_cast<int>(dist.size()) ||
            !reachable(target)) return {};
        vi path;
        for (int v = target; parent[v] != -1; v = parent[v]) {
            path.push_back(parent_edge[v]);
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

template <class Cost>
// @acf fn bfs — (Graph, s|sources) -> ShortestPathResult — 未重み最短手数（1辺=1）
/// @acf-doc @brief 未重み最短手数（1辺=1）
/// @acf-doc @code (Graph, s|sources) -> ShortestPathResult
/// @acf-doc [function] bfs
ShortestPathResult<ll> bfs(const Graph<Cost>& graph, const vi& sources) {
    constexpr ll INF = numeric_limits<ll>::max() / 4;
    ShortestPathResult<ll> result{
        vll(graph.n, INF), vi(graph.n, -1), vi(graph.n, -1), INF};
    queue<int> que;
    for (int source : sources) {
        assert(0 <= source && source < graph.n);
        if (result.dist[source] == 0) continue;
        result.dist[source] = 0;
        que.push(source);
    }
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (const auto& e : graph[v]) {
            if (result.dist[e.to] != INF) continue;
            result.dist[e.to] = result.dist[v] + 1;
            result.parent[e.to] = v;
            result.parent_edge[e.to] = e.id;
            que.push(e.to);
        }
    }
    return result;
}

template <class Cost>
ShortestPathResult<ll> bfs(const Graph<Cost>& graph, int source) {
    return bfs(graph, vi{source});
}

// グリッド／迷路: 頂点は密番号 id = r * W + c（壁マスも番号は持つが辺を張らない／訪問しない）。
// @acf fn grid_id — (r,c,w) -> int — グリッド座標を密頂点番号へ
/// @acf-doc @brief グリッド座標を密頂点番号へ
/// @acf-doc @code (r,c,w) -> int
/// @acf-doc [function] grid_id
constexpr int grid_id(int r, int c, int w) {
    return r * w + c;
}

// @acf fn grid_rc — (id,w) -> pair<int,int> — 密頂点番号を (r,c) へ
/// @acf-doc @brief 密頂点番号を (r,c) へ
/// @acf-doc @code (id,w) -> pair<int,int>
/// @acf-doc [function] grid_rc
constexpr pair<int, int> grid_rc(int id, int w) {
    return {id / w, id % w};
}

// @acf const kGrid4 — 4近傍 (dr,dc)（上下左右）
/// @acf-doc @brief 4近傍 (dr,dc)（上下左右）
/// @acf-doc [constant] kGrid4
inline constexpr array<pair<int, int>, 4> kGrid4 = {{
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
}};

// @acf const kGrid8 — 8近傍 (dr,dc)
/// @acf-doc @brief 8近傍 (dr,dc)
/// @acf-doc [constant] kGrid8
inline constexpr array<pair<int, int>, 8> kGrid8 = {{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1},
}};

namespace ac_graph_detail {
template <class Grid>
int grid_h(const Grid& grid) {
    assert(!grid.empty());
    return static_cast<int>(grid.size());
}

template <class Grid>
int grid_w(const Grid& grid) {
    assert(!grid.empty());
    return static_cast<int>(grid[0].size());
}

template <class Grid, class Passable, class Dirs>
Graph<> graph_from_grid_impl(const Grid& grid, Passable&& passable, const Dirs& dirs) {
    const int h = grid_h(grid);
    const int w = grid_w(grid);
    assert(w > 0);
    Graph<> graph(h * w);
    for (int r = 0; r < h; ++r) {
        assert(static_cast<int>(grid[r].size()) == w);
        for (int c = 0; c < w; ++c) {
            if (!passable(grid[r][c])) continue;
            const int u = grid_id(r, c, w);
            for (const auto& [dr, dc] : dirs) {
                const int nr = r + dr;
                const int nc = c + dc;
                if (out_grid(nr, nc, h, w) || !passable(grid[nr][nc])) continue;
                const int v = grid_id(nr, nc, w);
                if (u < v) graph.add_edge(u, v);
            }
        }
    }
    return graph;
}

template <class Grid, class Passable, class Dirs>
ShortestPathResult<ll> grid_bfs_impl(const Grid& grid, const vi& sources,
                                     Passable&& passable, const Dirs& dirs) {
    const int h = grid_h(grid);
    const int w = grid_w(grid);
    assert(w > 0);
    const int n = h * w;
    constexpr ll INF = numeric_limits<ll>::max() / 4;
    ShortestPathResult<ll> result{vll(n, INF), vi(n, -1), vi(n, -1), INF};
    queue<int> que;
    for (int source : sources) {
        assert(0 <= source && source < n);
        const auto [sr, sc] = grid_rc(source, w);
        assert(static_cast<int>(grid[sr].size()) == w);
        if (!passable(grid[sr][sc])) continue;
        if (result.dist[source] == 0) continue;
        result.dist[source] = 0;
        que.push(source);
    }
    while (!que.empty()) {
        const int u = que.front();
        que.pop();
        const auto [r, c] = grid_rc(u, w);
        for (const auto& [dr, dc] : dirs) {
            const int nr = r + dr;
            const int nc = c + dc;
            if (out_grid(nr, nc, h, w) || !passable(grid[nr][nc])) continue;
            const int v = grid_id(nr, nc, w);
            if (result.dist[v] != INF) continue;
            result.dist[v] = result.dist[u] + 1;
            result.parent[v] = u;
            que.push(v);
        }
    }
    return result;
}
}  // namespace ac_graph_detail

// @acf fn graph_from_grid — (盤面[, passable|wall][, dirs]) -> Graph — グリッドを密 Graph 化
/// @acf-doc @brief グリッドを密 Graph 化
/// @acf-doc @code (盤面[, passable|wall][, dirs]) -> Graph
/// @acf-doc [function] graph_from_grid
template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
Graph<> graph_from_grid(const vector<string>& grid, Passable passable,
                        const Dirs& dirs = kGrid4) {
    return ac_graph_detail::graph_from_grid_impl(grid, passable, dirs);
}

template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
Graph<> graph_from_grid(const vector<vector<Cell>>& grid, Passable passable,
                        const Dirs& dirs = kGrid4) {
    return ac_graph_detail::graph_from_grid_impl(grid, passable, dirs);
}

template <class Dirs = decltype(kGrid4)>
Graph<> graph_from_grid(const vector<string>& grid, char wall = '#',
                        const Dirs& dirs = kGrid4) {
    return graph_from_grid(
        grid, [wall](char cell) { return cell != wall; }, dirs);
}

// @acf fn grid_passable — (盤面[, passable|wall]) -> vb — 通行マスの密マスク（長さ H*W）
/// @acf-doc @brief 通行マスの密マスク（長さ H*W）
/// @acf-doc @code (盤面[, passable|wall]) -> vb
/// @acf-doc [function] grid_passable
template <class Passable>
    requires invocable<Passable&, char>
vb grid_passable(const vector<string>& grid, Passable passable) {
    const int h = ac_graph_detail::grid_h(grid);
    const int w = ac_graph_detail::grid_w(grid);
    vb ok(h * w, false);
    for (int r = 0; r < h; ++r) {
        assert(static_cast<int>(grid[r].size()) == w);
        for (int c = 0; c < w; ++c) {
            ok[grid_id(r, c, w)] = static_cast<bool>(passable(grid[r][c]));
        }
    }
    return ok;
}

template <class Cell, class Passable>
    requires invocable<Passable&, const Cell&>
vb grid_passable(const vector<vector<Cell>>& grid, Passable passable) {
    const int h = ac_graph_detail::grid_h(grid);
    const int w = ac_graph_detail::grid_w(grid);
    vb ok(h * w, false);
    for (int r = 0; r < h; ++r) {
        assert(static_cast<int>(grid[r].size()) == w);
        for (int c = 0; c < w; ++c) {
            ok[grid_id(r, c, w)] = static_cast<bool>(passable(grid[r][c]));
        }
    }
    return ok;
}

inline vb grid_passable(const vector<string>& grid, char wall = '#') {
    return grid_passable(grid, [wall](char cell) { return cell != wall; });
}

// @acf fn grid_from_graph — (Graph, h, w[, passable][, floor, wall]) -> vs — 密 Graph を盤面へ戻す
/// @acf-doc @brief 密 Graph を盤面へ戻す
/// @acf-doc @code (Graph, h, w[, passable][, floor, wall]) -> vs
/// @acf-doc [function] grid_from_graph
template <class Cost, class Passable>
    requires invocable<Passable&, int>
vs grid_from_graph(const Graph<Cost>& graph, int h, int w, Passable passable,
                   char floor = '.', char wall = '#') {
    assert(h > 0 && w > 0);
    assert(graph.n == h * w);
    vs board(h, string(w, wall));
    for (int id = 0; id < graph.n; ++id) {
        if (!passable(id)) continue;
        const auto [r, c] = grid_rc(id, w);
        board[r][c] = floor;
    }
    return board;
}

template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, int h, int w, const vb& passable,
                   char floor = '.', char wall = '#') {
    assert(static_cast<int>(passable.size()) == h * w);
    return grid_from_graph(
        graph, h, w, [&](int id) { return passable[id]; }, floor, wall);
}

template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, int h, int w, char floor = '.',
                   char wall = '#') {
    // Graph だけの復元: 次数0の通行マスは壁になる（孤立マスは grid_passable を渡す）
    return grid_from_graph(
        graph, h, w, [&](int id) { return !graph[id].empty(); }, floor, wall);
}

template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, const vector<string>& board,
                   char wall = '#', char floor = '.') {
    const int h = ac_graph_detail::grid_h(board);
    const int w = ac_graph_detail::grid_w(board);
    return grid_from_graph(graph, h, w, grid_passable(board, wall), floor, wall);
}

// @acf fn grid_bfs — (盤面, 始点|sources[, passable|wall][, dirs]) -> ShortestPathResult — 盤面上の最短手数
/// @acf-doc @brief 盤面上の最短手数
/// @acf-doc @code (盤面, 始点|sources[, passable|wall][, dirs]) -> ShortestPathResult
/// @acf-doc [function] grid_bfs
template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, const vi& sources,
                                Passable passable, const Dirs& dirs = kGrid4) {
    return ac_graph_detail::grid_bfs_impl(grid, sources, passable, dirs);
}

template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, int sr, int sc,
                                Passable passable, const Dirs& dirs = kGrid4) {
    const int w = ac_graph_detail::grid_w(grid);
    return grid_bfs(grid, vi{grid_id(sr, sc, w)}, passable, dirs);
}

template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
ShortestPathResult<ll> grid_bfs(const vector<vector<Cell>>& grid, const vi& sources,
                                Passable passable, const Dirs& dirs = kGrid4) {
    return ac_graph_detail::grid_bfs_impl(grid, sources, passable, dirs);
}

template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
ShortestPathResult<ll> grid_bfs(const vector<vector<Cell>>& grid, int sr, int sc,
                                Passable passable, const Dirs& dirs = kGrid4) {
    const int w = ac_graph_detail::grid_w(grid);
    return grid_bfs(grid, vi{grid_id(sr, sc, w)}, passable, dirs);
}

template <class Dirs = decltype(kGrid4)>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, const vi& sources,
                                char wall = '#', const Dirs& dirs = kGrid4) {
    return grid_bfs(
        grid, sources, [wall](char cell) { return cell != wall; }, dirs);
}

template <class Dirs = decltype(kGrid4)>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, int sr, int sc,
                                char wall = '#', const Dirs& dirs = kGrid4) {
    return grid_bfs(
        grid, sr, sc, [wall](char cell) { return cell != wall; }, dirs);
}

struct DfsResult {
    vi preorder;     // 行きがけ順（頂点へ入った順）
    vi postorder;    // 帰りがけ順（頂点を出た順）
    vi parent;
    vi parent_edge;
    vi depth;
    vi root;         // 各頂点が属する DFS 木の根
    vi component;    // DFS 木の番号。未訪問は -1
    vi entry_time;   // 入時刻
    vi exit_time;    // 出時刻
    vi roots;        // 実際に探索を開始した根

    /// @brief 頂点 `v` がDFSで訪問済みか判定する。
    bool reachable(int v) const { return component[v] != -1; }

    /// @brief 同じDFS木で `ancestor` が `descendant` の祖先か判定する。
    bool is_ancestor(int ancestor, int descendant) const {
        if (!reachable(ancestor) || !reachable(descendant) ||
            component[ancestor] != component[descendant]) return false;
        return entry_time[ancestor] <= entry_time[descendant] &&
               exit_time[descendant] <= exit_time[ancestor];
    }

    /// @brief DFS木の根から `target` までの頂点列を復元する。
    vi path_to(int target) const {
        if (target < 0 || target >= static_cast<int>(parent.size()) ||
            !reachable(target)) return {};
        vi path;
        for (int v = target; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }
};

namespace ac_graph_detail {
template <class Cost>
DfsResult dfs_impl(const Graph<Cost>& graph, const vi& initial_roots,
                   bool cover_all) {
    DfsResult result;
    result.parent.assign(graph.n, -1);
    result.parent_edge.assign(graph.n, -1);
    result.depth.assign(graph.n, -1);
    result.root.assign(graph.n, -1);
    result.component.assign(graph.n, -1);
    result.entry_time.assign(graph.n, -1);
    result.exit_time.assign(graph.n, -1);
    result.preorder.reserve(graph.n);
    result.postorder.reserve(graph.n);

    vi state(graph.n);  // 0=未訪問, 1=探索中, 2=完了
    int timer = 0;
    auto run_from = [&](int start) {
        assert(0 <= start && start < graph.n);
        if (state[start] != 0) return;
        const int component = static_cast<int>(result.roots.size());
        result.roots.push_back(start);
        result.depth[start] = 0;
        result.root[start] = start;
        result.component[start] = component;
        result.entry_time[start] = timer++;
        result.preorder.push_back(start);
        state[start] = 1;

        vector<pair<int, int>> stack{{start, 0}};
        while (!stack.empty()) {
            int v = stack.back().first;
            int& edge_index = stack.back().second;
            if (edge_index == static_cast<int>(graph[v].size())) {
                state[v] = 2;
                result.exit_time[v] = timer++;
                result.postorder.push_back(v);
                stack.pop_back();
                continue;
            }
            const auto& edge = graph[v][edge_index++];
            if (state[edge.to] != 0) continue;
            result.parent[edge.to] = v;
            result.parent_edge[edge.to] = edge.id;
            result.depth[edge.to] = result.depth[v] + 1;
            result.root[edge.to] = start;
            result.component[edge.to] = component;
            result.entry_time[edge.to] = timer++;
            result.preorder.push_back(edge.to);
            state[edge.to] = 1;
            stack.emplace_back(edge.to, 0);
        }
    };

    for (int start : initial_roots) run_from(start);
    if (cover_all)
        for (int start = 0; start < graph.n; ++start) run_from(start);
    return result;
}
}  // namespace ac_graph_detail

// source から到達できる部分だけを深さ優先探索する。
// @acf fn dfs — (Graph, s|sources) -> DfsResult — 到達部分の DFS（行きがけ／帰りがけ）
/// @acf-doc @brief 到達部分の DFS（行きがけ／帰りがけ）
/// @acf-doc @code (Graph, s|sources) -> DfsResult
/// @acf-doc [function] dfs
template <class Cost>
DfsResult dfs(const Graph<Cost>& graph, int source) {
    return ac_graph_detail::dfs_impl(graph, vi{source}, false);
}

// sources を順に根候補として探索する。先の根から訪問済みなら新しい木にしない。
template <class Cost>
DfsResult dfs(const Graph<Cost>& graph, const vi& sources) {
    return ac_graph_detail::dfs_impl(graph, sources, false);
}

// 非連結部分も含め、全頂点の DFS 森を作る。
// @acf fn dfs_forest — (Graph) -> DfsResult — 全頂点の DFS 森
/// @acf-doc @brief 全頂点の DFS 森
/// @acf-doc @code (Graph) -> DfsResult
/// @acf-doc [function] dfs_forest
template <class Cost>
DfsResult dfs_forest(const Graph<Cost>& graph) {
    return ac_graph_detail::dfs_impl(graph, {}, true);
}

// @acf fn dijkstra — (Graph, sources[, inf]) -> ShortestPathResult — 非負重みの単一/複数始点最短路
/// @acf-doc @brief 非負重みの単一/複数始点最短路
/// @acf-doc @code (Graph, sources[, inf]) -> ShortestPathResult
/// @acf-doc [function] dijkstra
template <class Cost>
ShortestPathResult<Cost> dijkstra(
    const Graph<Cost>& graph, const vi& sources,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    ShortestPathResult<Cost> result{
        vector<Cost>(graph.n, inf), vi(graph.n, -1), vi(graph.n, -1), inf};
    using State = pair<Cost, int>;
    priority_queue<State, vector<State>, greater<State>> que;
    for (int source : sources) {
        assert(0 <= source && source < graph.n);
        if (result.dist[source] == Cost{}) continue;
        result.dist[source] = Cost{};
        que.emplace(Cost{}, source);
    }
    while (!que.empty()) {
        auto [distance, v] = que.top();
        que.pop();
        if (distance != result.dist[v]) continue;
        for (const auto& e : graph[v]) {
            assert(!(e.cost < Cost{}) && "dijkstra requires non-negative costs");
            Cost next = ac_graph_detail::add_clamped(result.dist[v], e.cost, inf);
            if (next >= result.dist[e.to]) continue;
            result.dist[e.to] = next;
            result.parent[e.to] = v;
            result.parent_edge[e.to] = e.id;
            que.emplace(next, e.to);
        }
    }
    return result;
}


template <class Cost>
ShortestPathResult<Cost> dijkstra(
    const Graph<Cost>& graph, int source,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    return dijkstra(graph, vi{source}, inf);
}

template <class Cost>
// @acf fn zero_one_bfs — (Graph, s[, inf]) -> ShortestPathResult — 辺重み 0/1 の最短路
/// @acf-doc @brief 辺重み 0/1 の最短路
/// @acf-doc @code (Graph, s[, inf]) -> ShortestPathResult
/// @acf-doc [function] zero_one_bfs
ShortestPathResult<Cost> zero_one_bfs(
    const Graph<Cost>& graph, int source,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    assert(0 <= source && source < graph.n);
    ShortestPathResult<Cost> result{
        vector<Cost>(graph.n, inf), vi(graph.n, -1), vi(graph.n, -1), inf};
    deque<int> que;
    result.dist[source] = Cost{};
    que.push_back(source);
    while (!que.empty()) {
        int v = que.front();
        que.pop_front();
        for (const auto& e : graph[v]) {
            assert((e.cost == Cost{} || e.cost == Cost{1}) &&
                   "zero_one_bfs requires costs 0 or 1");
            Cost next = result.dist[v] + e.cost;
            if (next >= result.dist[e.to]) continue;
            result.dist[e.to] = next;
            result.parent[e.to] = v;
            result.parent_edge[e.to] = e.id;
            if (e.cost == Cost{}) que.push_front(e.to);
            else que.push_back(e.to);
        }
    }
    return result;
}

struct ConnectedComponents {
    vi id;
    vector<vi> groups;

    /// @brief 連結成分の個数を返す。
    int count() const { return static_cast<int>(groups.size()); }

    /// @brief 頂点 `a` と `b` が同じ連結成分か判定する。
    bool same(int a, int b) const { return id[a] == id[b]; }
};

template <class Cost>
// @acf fn connected_components — (Graph) -> ConnectedComponents — 無向の連結成分
/// @acf-doc @brief 無向の連結成分
/// @acf-doc @code (Graph) -> ConnectedComponents
/// @acf-doc [function] connected_components
ConnectedComponents connected_components(const Graph<Cost>& graph) {
    ConnectedComponents result{vi(graph.n, -1), {}};
    for (int start = 0; start < graph.n; ++start) {
        if (result.id[start] != -1) continue;
        int component = result.count();
        result.groups.push_back({});
        vector<int> stack{start};
        result.id[start] = component;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            result.groups.back().push_back(v);
            for (const auto& e : graph[v]) {
                if (result.id[e.to] != -1) continue;
                result.id[e.to] = component;
                stack.push_back(e.to);
            }
        }
    }
    return result;
}

template <class Cost>
// @acf fn bipartite_coloring — (Graph) -> optional<vi> — 2色塗り。不可なら nullopt
/// @acf-doc @brief 2色塗り。不可なら nullopt
/// @acf-doc @code (Graph) -> optional<vi>
/// @acf-doc [function] bipartite_coloring
optional<vi> bipartite_coloring(const Graph<Cost>& graph) {
    vi color(graph.n, -1);
    for (int start = 0; start < graph.n; ++start) {
        if (color[start] != -1) continue;
        queue<int> que;
        color[start] = 0;
        que.push(start);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const auto& e : graph[v]) {
                if (color[e.to] == -1) {
                    color[e.to] = color[v] ^ 1;
                    que.push(e.to);
                } else if (color[e.to] == color[v]) {
                    return nullopt;
                }
            }
        }
    }
    return color;
}

// DAG なら順序、閉路があれば nullopt。有向グラフ向け。
template <class Cost>
// @acf fn topological_sort — (Graph) -> optional<vi> — 有向のトポロジカル順。閉路なら nullopt
/// @acf-doc @brief 有向のトポロジカル順。閉路なら nullopt
/// @acf-doc @code (Graph) -> optional<vi>
/// @acf-doc [function] topological_sort
optional<vi> topological_sort(const Graph<Cost>& graph) {
    vi indegree(graph.n);
    for (int v = 0; v < graph.n; ++v)
        for (const auto& e : graph[v]) ++indegree[e.to];
    queue<int> que;
    for (int v = 0; v < graph.n; ++v)
        if (indegree[v] == 0) que.push(v);
    vi order;
    order.reserve(graph.n);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        order.push_back(v);
        for (const auto& e : graph[v])
            if (--indegree[e.to] == 0) que.push(e.to);
    }
    if (static_cast<int>(order.size()) != graph.n) return nullopt;
    return order;
}

// 有向DAG上の通常形ゲーム。辺のコストは無視し、終端のGrundy値を0とする。
template <class Cost>
// @acf fn grundy_dag — (Graph) -> optional<vi> — 有向DAGの各頂点のGrundy数。閉路/無向ならnullopt
/// @acf-doc @brief 有向DAGの各頂点のGrundy数。閉路/無向ならnullopt
/// @acf-doc @code (Graph) -> optional<vi>
/// @acf-doc [function] grundy_dag
optional<vi> grundy_dag(const Graph<Cost>& graph) {
    if (!graph.directed) return nullopt;
    auto order = topological_sort(graph);
    if (!order) return nullopt;

    vi result(graph.n, 0);
    for (auto it = order->rbegin(); it != order->rend(); ++it) {
        vi next_values;
        next_values.reserve(graph[*it].size());
        for (const auto& edge : graph[*it]) next_values.push_back(result[edge.to]);
        result[*it] = mex(next_values);
    }
    return result;
}

struct TreeInfo {
    vi parent;
    vi parent_edge;
    vi depth;
    vi order;         // 親が子より先
    vi subtree_size;
};

template <class Cost>
// @acf fn tree_info — (tree[, root]) -> TreeInfo — 親・深さ・部分木サイズ・順序
/// @acf-doc @brief 親・深さ・部分木サイズ・順序
/// @acf-doc @code (tree[, root]) -> TreeInfo
/// @acf-doc [function] tree_info
TreeInfo tree_info(const Graph<Cost>& tree, int root = 0) {
    assert(0 <= root && root < tree.n);
    TreeInfo result{vi(tree.n, -1), vi(tree.n, -1), vi(tree.n, -1), {},
                    vi(tree.n, 1)};
    result.order.reserve(tree.n);
    vector<int> stack{root};
    result.depth[root] = 0;
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        result.order.push_back(v);
        for (const auto& e : tree[v]) {
            if (e.to == result.parent[v] || result.depth[e.to] != -1) continue;
            result.parent[e.to] = v;
            result.parent_edge[e.to] = e.id;
            result.depth[e.to] = result.depth[v] + 1;
            stack.push_back(e.to);
        }
    }
    for (auto it = result.order.rbegin(); it != result.order.rend(); ++it) {
        int v = *it;
        if (result.parent[v] != -1)
            result.subtree_size[result.parent[v]] += result.subtree_size[v];
    }
    return result;
}

template <class Cost>
struct BellmanFordResult {
    vector<Cost> dist;
    vi parent;
    vi parent_edge;
    vb negative;  // 始点から到達でき、負閉路の影響を受ける
    Cost inf{};

    /// @brief 頂点 `v` が始点から到達可能か判定する。
    bool reachable(int v) const { return dist[v] != inf; }

    /// @brief 到達可能で、負閉路の影響を受けない距離か判定する。
    bool well_defined(int v) const { return reachable(v) && !negative[v]; }

    /// @brief `target` までの経路を復元する。不定または到達不能なら空を返す。
    vi path_to(int target) const {
        if (target < 0 || target >= static_cast<int>(dist.size()) ||
            !well_defined(target)) return {};
        vi path;
        for (int v = target; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }
};

// 負辺を許す単一始点最短路。negative[v] は距離を -∞ にできる頂点。
template <class Cost>
// @acf fn bellman_ford — (Graph, s[, inf]) -> BellmanFordResult — 負辺可の単一始点最短路
/// @acf-doc @brief 負辺可の単一始点最短路
/// @acf-doc @code (Graph, s[, inf]) -> BellmanFordResult
/// @acf-doc [function] bellman_ford
BellmanFordResult<Cost> bellman_ford(
    const Graph<Cost>& graph, int source,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    assert(0 <= source && source < graph.n);
    BellmanFordResult<Cost> result{
        vector<Cost>(graph.n, inf), vi(graph.n, -1), vi(graph.n, -1),
        vb(graph.n, false), inf};
    // 判定中に -inf へ丸めると負閉路の更新を見失う。64bit 以下の整数は
    // i128 で保持し、各反復を前回の距離から更新して経路長を高々 n 辺に保つ。
    constexpr bool widen = is_integral_v<Cost> && sizeof(Cost) <= sizeof(ll);
    using Distance = conditional_t<widen, i128, Cost>;
    const Distance distance_inf = widen ? numeric_limits<Distance>::max()
                                       : static_cast<Distance>(inf);
    vector<Distance> dist(graph.n, distance_inf);
    dist[source] = Distance{};

    for (int iteration = 0; iteration < graph.n - 1; ++iteration) {
        const auto previous = dist;
        bool changed = false;
        for (int v = 0; v < graph.n; ++v) {
            if (previous[v] == distance_inf) continue;
            for (const auto& e : graph[v]) {
                const Distance next = ac_graph_detail::add_clamped(
                    previous[v], static_cast<Distance>(e.cost), distance_inf);
                if (next >= dist[e.to]) continue;
                dist[e.to] = next;
                result.parent[e.to] = v;
                result.parent_edge[e.to] = e.id;
                changed = true;
            }
        }
        if (!changed) break;
    }

    queue<int> que;
    for (int v = 0; v < graph.n; ++v) {
        if (dist[v] == distance_inf) continue;
        for (const auto& e : graph[v]) {
            if (ac_graph_detail::add_clamped(
                    dist[v], static_cast<Distance>(e.cost), distance_inf) >=
                dist[e.to]) continue;
            if (!result.negative[e.to]) {
                result.negative[e.to] = true;
                que.push(e.to);
            }
        }
    }
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (const auto& e : graph[v]) {
            if (result.negative[e.to]) continue;
            result.negative[e.to] = true;
            que.push(e.to);
        }
    }
    // 負閉路の判定後にだけ公開距離型へ戻す。有限距離の丸め範囲は従来どおり。
    for (int v = 0; v < graph.n; ++v) {
        if (dist[v] == distance_inf) continue;
        if constexpr (widen) {
            Distance value = min(dist[v], static_cast<Distance>(inf));
            if constexpr (is_signed_v<Cost>)
                value = max(value, -static_cast<Distance>(inf));
            result.dist[v] = static_cast<Cost>(value);
        } else {
            result.dist[v] = dist[v];
        }
        // 大きな正辺の先でも、負閉路の影響を受ける頂点を未到達扱いしない。
        if constexpr (is_signed_v<Cost>) {
            if (result.negative[v]) result.dist[v] = -inf;
        }
    }
    return result;
}

template <class Cost>
struct AllPairsShortestPath {
    vector<vector<Cost>> dist;
    Cost inf{};

    /// @brief `from` から `to` へ到達可能か判定する。
    bool reachable(int from, int to) const { return dist[from][to] != inf; }

    /// @brief 距離行列が負閉路を含むか判定する。
    bool has_negative_cycle() const {
        for (int v = 0; v < static_cast<int>(dist.size()); ++v)
            if (dist[v][v] < Cost{}) return true;
        return false;
    }
};

template <class Cost>
// @acf fn floyd_warshall — (Graph[, inf]) -> AllPairsShortestPath — 全点対最短路
/// @acf-doc @brief 全点対最短路
/// @acf-doc @code (Graph[, inf]) -> AllPairsShortestPath
/// @acf-doc [function] floyd_warshall
AllPairsShortestPath<Cost> floyd_warshall(
    const Graph<Cost>& graph,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    vector<vector<Cost>> dist(graph.n, vector<Cost>(graph.n, inf));
    for (int v = 0; v < graph.n; ++v) dist[v][v] = Cost{};
    for (int v = 0; v < graph.n; ++v)
        for (const auto& e : graph[v]) chmin(dist[v][e.to], e.cost);
    for (int via = 0; via < graph.n; ++via) {
        for (int from = 0; from < graph.n; ++from) {
            if (dist[from][via] == inf) continue;
            for (int to = 0; to < graph.n; ++to) {
                if (dist[via][to] == inf) continue;
                chmin(dist[from][to], ac_graph_detail::add_clamped(
                                           dist[from][via], dist[via][to], inf));
            }
        }
    }
    return {std::move(dist), inf};
}

struct StronglyConnectedComponents {
    vi id;
    vector<vi> groups;  // トポロジカル順

    /// @brief 強連結成分の個数を返す。
    int count() const { return static_cast<int>(groups.size()); }

    /// @brief 頂点 `a` と `b` が同じ強連結成分か判定する。
    bool same(int a, int b) const { return id[a] == id[b]; }
};

// Kosaraju 法。再帰を使わず O(V+E)。
// @acf fn strongly_connected_components — (Graph) -> SCC — 有向の強連結成分
/// @acf-doc @brief 有向の強連結成分
/// @acf-doc @code (Graph) -> SCC
/// @acf-doc [function] strongly_connected_components
template <class Cost>
StronglyConnectedComponents strongly_connected_components(
    const Graph<Cost>& graph) {
    vi used(graph.n), order;
    order.reserve(graph.n);
    for (int start = 0; start < graph.n; ++start) {
        if (used[start]) continue;
        vector<pair<int, int>> stack{{start, 0}};
        used[start] = 1;
        while (!stack.empty()) {
            int v = stack.back().first;
            int& index = stack.back().second;
            if (index < static_cast<int>(graph[v].size())) {
                int to = graph[v][index++].to;
                if (!used[to]) {
                    used[to] = 1;
                    stack.emplace_back(to, 0);
                }
            } else {
                order.push_back(v);
                stack.pop_back();
            }
        }
    }

    Graph<Cost> reversed = graph.reversed();
    StronglyConnectedComponents result{vi(graph.n, -1), {}};
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int start = *it;
        if (result.id[start] != -1) continue;
        int component = result.count();
        result.groups.push_back({});
        vector<int> stack{start};
        result.id[start] = component;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            result.groups.back().push_back(v);
            for (const auto& e : reversed[v]) {
                if (result.id[e.to] != -1) continue;
                result.id[e.to] = component;
                stack.push_back(e.to);
            }
        }
    }
    return result;
}

template <class Cost>
// @acf fn condensation_graph — (Graph, scc) -> Graph — SCC を頂点にした DAG
/// @acf-doc @brief SCC を頂点にした DAG
/// @acf-doc @code (Graph, scc) -> Graph
/// @acf-doc [function] condensation_graph
Graph<> condensation_graph(
    const Graph<Cost>& graph, const StronglyConnectedComponents& scc) {
    Graph<> dag(scc.count(), true);
    set<pair<int, int>> added;
    for (const auto& e : graph.edges) {
        int from = scc.id[e.from], to = scc.id[e.to];
        if (from != to && added.emplace(from, to).second)
            dag.add_edge(from, to);
    }
    return dag;
}

struct LowLinkResult {
    vi order;
    vi low;
    vi bridges;              // 橋の edge.id
    vi articulation_points;  // 関節点の頂点番号
};

// 無向グラフの橋・関節点。辺IDで親辺だけを除外するため多重辺にも対応。
template <class Cost>
// @acf fn low_link — (Graph) -> LowLinkResult — 無向の橋・関節点
/// @acf-doc @brief 無向の橋・関節点
/// @acf-doc @code (Graph) -> LowLinkResult
/// @acf-doc [function] low_link
LowLinkResult low_link(const Graph<Cost>& graph) {
    assert(!graph.directed && "low_link requires an undirected graph");
    vi order(graph.n, -1), low(graph.n), parent(graph.n, -1),
       parent_edge(graph.n, -1), children(graph.n);
    vb articulation(graph.n, false);
    vi bridges;
    int timer = 0;
    for (int root = 0; root < graph.n; ++root) {
        if (order[root] != -1) continue;
        order[root] = low[root] = timer++;
        vector<pair<int, int>> stack{{root, 0}};
        while (!stack.empty()) {
            int v = stack.back().first;
            int& index = stack.back().second;
            if (index < static_cast<int>(graph[v].size())) {
                const auto& e = graph[v][index++];
                if (e.id == parent_edge[v]) continue;
                if (order[e.to] == -1) {
                    parent[e.to] = v;
                    parent_edge[e.to] = e.id;
                    ++children[v];
                    order[e.to] = low[e.to] = timer++;
                    stack.emplace_back(e.to, 0);
                } else {
                    chmin(low[v], order[e.to]);
                }
            } else {
                stack.pop_back();
                if (parent[v] == -1) {
                    articulation[v] = children[v] >= 2;
                    continue;
                }
                int p = parent[v];
                chmin(low[p], low[v]);
                if (low[v] > order[p]) bridges.push_back(parent_edge[v]);
                if (parent[p] != -1 && low[v] >= order[p]) articulation[p] = true;
            }
        }
    }
    sort(bridges.begin(), bridges.end());
    vi points;
    for (int v = 0; v < graph.n; ++v)
        if (articulation[v]) points.push_back(v);
    return {std::move(order), std::move(low), std::move(bridges),
            std::move(points)};
}

template <class Cost>
struct MinimumSpanningForest {
    Cost cost{};
    vi edge_ids;
    int components = 0;

    /// @brief グラフ全体が1本の全域木になったか判定する。
    bool is_spanning_tree() const { return components == 1; }
};

template <class Cost>
// @acf fn kruskal — (Graph) -> MinimumSpanningForest — 無向の最小全域森
/// @acf-doc @brief 無向の最小全域森。64ビット以下の整数重みはi128で集計し、最終値がCostに収まらなければoverflow_error。
/// @acf-doc @code (Graph) -> MinimumSpanningForest
/// @acf-doc [function] kruskal
MinimumSpanningForest<Cost> kruskal(const Graph<Cost>& graph) {
    assert(!graph.directed && "kruskal requires an undirected graph");
    vi parent(graph.n, -1);
    auto leader = [&](auto&& self, int v) -> int {
        return parent[v] < 0 ? v : parent[v] = self(self, parent[v]);
    };
    auto merge = [&](int a, int b) {
        a = leader(leader, a);
        b = leader(leader, b);
        if (a == b) return false;
        if (parent[a] > parent[b]) swap(a, b);
        parent[a] += parent[b];
        parent[b] = a;
        return true;
    };

    vector<int> indices(graph.edges.size());
    iota(indices.begin(), indices.end(), 0);
    stable_sort(indices.begin(), indices.end(), [&](int a, int b) {
        return graph.edges[a].cost < graph.edges[b].cost;
    });
    MinimumSpanningForest<Cost> result;
    constexpr bool wide_sum = is_integral_v<Cost> && sizeof(Cost) <= sizeof(ll);
    using Sum = conditional_t<wide_sum, i128, Cost>;
    Sum total{};
    result.components = graph.n;
    for (int index : indices) {
        const auto& e = graph.edges[index];
        if (!merge(e.from, e.to)) continue;
        total += e.cost;
        result.edge_ids.push_back(e.id);
        --result.components;
    }
    if constexpr (wide_sum) {
        if (total < i128(numeric_limits<Cost>::lowest()) ||
            total > i128(numeric_limits<Cost>::max()))
            throw overflow_error("kruskal cost does not fit Cost");
    }
    result.cost = static_cast<Cost>(total);
    return result;
}

// @acf fn LowestCommonAncestor — (tree[, root]) -> LCA — 祖先・距離のダブリング
/// @acf-doc @brief 祖先・距離のダブリング
/// @acf-doc @code (tree[, root]) -> LCA
/// @acf-doc [function] LowestCommonAncestor
template <class Cost = ll>
struct LowestCommonAncestor {
    vector<vi> up;
    vi depth;
    vector<Cost> root_distance;

    /// @brief 未構築のLCAオブジェクトを作る。
    LowestCommonAncestor() = default;

    /// @brief 木を前処理し、祖先・距離クエリを準備する。
    explicit LowestCommonAncestor(const Graph<Cost>& tree, int root = 0) {
        assert(is_tree(tree));
        int log = 1;
        while ((1ULL << log) <= static_cast<unsigned int>(tree.n)) ++log;
        up.assign(log, vi(tree.n, -1));
        depth.assign(tree.n, -1);
        root_distance.assign(tree.n, Cost{});
        vector<int> stack{root};
        depth[root] = 0;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            for (const auto& e : tree[v]) {
                if (e.to == up[0][v]) continue;
                up[0][e.to] = v;
                depth[e.to] = depth[v] + 1;
                root_distance[e.to] = root_distance[v] + e.cost;
                stack.push_back(e.to);
            }
        }
        for (int k = 1; k < log; ++k)
            for (int v = 0; v < tree.n; ++v)
                if (up[k - 1][v] != -1) up[k][v] = up[k - 1][up[k - 1][v]];
    }

    /// @brief 頂点 `v` の `steps` 個上の祖先を返す。存在しなければ `-1`。
    int kth_ancestor(int v, int steps) const {
        assert(0 <= v && v < static_cast<int>(depth.size()) && steps >= 0);
        for (int k = 0; k < static_cast<int>(up.size()) && v != -1; ++k)
            if ((steps >> k) & 1) v = up[k][v];
        if ((static_cast<unsigned int>(steps) >> up.size()) != 0) return -1;
        return v;
    }

    /// @brief 頂点 `a` と `b` の最小共通祖先を返す。
    int query(int a, int b) const {
        assert(0 <= a && a < static_cast<int>(depth.size()));
        assert(0 <= b && b < static_cast<int>(depth.size()));
        if (depth[a] < depth[b]) swap(a, b);
        a = kth_ancestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int k = static_cast<int>(up.size()) - 1; k >= 0; --k) {
            if (up[k][a] == up[k][b]) continue;
            a = up[k][a];
            b = up[k][b];
        }
        return up[0][a];
    }

    /// @brief `a` と `b` の木上の辺数距離を返す。
    int distance_edges(int a, int b) const {
        int ancestor = query(a, b);
        return depth[a] + depth[b] - 2 * depth[ancestor];
    }

    /// @brief `a` と `b` の木上の辺重み和を返す。
    Cost distance_cost(int a, int b) const {
        int ancestor = query(a, b);
        return root_distance[a] + root_distance[b] - Cost{2} * root_distance[ancestor];
    }
};

template <class Cost>
struct TreeDiameterResult {
    int from = -1;
    int to = -1;
    Cost distance{};
    vi path;
};

// 非負重み木の直径。未重み木では各辺 cost=1。
template <class Cost>
// @acf fn tree_diameter — (tree) -> TreeDiameterResult — 非負重み木の直径
/// @acf-doc @brief 非負重み木の直径
/// @acf-doc @code (tree) -> TreeDiameterResult
/// @acf-doc [function] tree_diameter
TreeDiameterResult<Cost> tree_diameter(const Graph<Cost>& tree) {
    assert(is_tree(tree));
    auto farthest = [&](int source) {
        vector<Cost> distance(tree.n);
        vi parent(tree.n, -1);
        vector<int> stack{source};
        vi seen(tree.n);
        seen[source] = 1;
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            for (const auto& e : tree[v]) {
                assert(!(e.cost < Cost{}) &&
                       "tree_diameter requires non-negative costs");
                if (seen[e.to]) continue;
                seen[e.to] = 1;
                parent[e.to] = v;
                distance[e.to] = distance[v] + e.cost;
                stack.push_back(e.to);
            }
        }
        int vertex = source;
        for (int v = 0; v < tree.n; ++v)
            if (distance[vertex] < distance[v]) vertex = v;
        return tuple{vertex, std::move(distance), std::move(parent)};
    };

    auto [from, ignored_distance, ignored_parent] = farthest(0);
    auto [to, distance, parent] = farthest(from);
    vi path;
    for (int v = to; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return {from, to, distance[to], std::move(path)};
}

struct GraphCycle {
    vi vertices;
    vi edge_ids;
};

// 有向・無向のどちらでも、最初に見つけた閉路を辺ID付きで返す。
// vertices[i] --edge_ids[i]--> vertices[(i+1)%k] の対応。
template <class Cost>
// @acf fn find_cycle — (Graph) -> optional<GraphCycle> — 閉路を1つ復元。なければ nullopt
/// @acf-doc @brief 閉路を1つ復元。なければ nullopt
/// @acf-doc @code (Graph) -> optional<GraphCycle>
/// @acf-doc [function] find_cycle
optional<GraphCycle> find_cycle(const Graph<Cost>& graph) {
    vi state(graph.n), parent(graph.n, -1), parent_edge(graph.n, -1);
    for (int start = 0; start < graph.n; ++start) {
        if (state[start] != 0) continue;
        state[start] = 1;
        vector<pair<int, int>> stack{{start, 0}};
        while (!stack.empty()) {
            int v = stack.back().first;
            int& index = stack.back().second;
            if (index == static_cast<int>(graph[v].size())) {
                state[v] = 2;
                stack.pop_back();
                continue;
            }
            const auto& e = graph[v][index++];
            if (!graph.directed && e.id == parent_edge[v]) continue;
            if (state[e.to] == 0) {
                state[e.to] = 1;
                parent[e.to] = v;
                parent_edge[e.to] = e.id;
                stack.emplace_back(e.to, 0);
                continue;
            }
            if (state[e.to] != 1) continue;

            vi vertices{v};
            while (vertices.back() != e.to)
                vertices.push_back(parent[vertices.back()]);
            reverse(vertices.begin(), vertices.end());
            vi edge_ids;
            for (int i = 1; i < static_cast<int>(vertices.size()); ++i)
                edge_ids.push_back(parent_edge[vertices[i]]);
            edge_ids.push_back(e.id);
            return GraphCycle{std::move(vertices), std::move(edge_ids)};
        }
    }
    return nullopt;
}

struct EulerTrail {
    vi vertices;  // 辺数+1 個（辺がなく頂点もなければ空）
    vi edge_ids;
};

// 全辺をちょうど1回通る路。存在しなければ nullopt。
// start=-1 は次数条件から始点を自動選択する。
template <class Cost>
// @acf fn euler_trail — (Graph[, start]) -> optional<EulerTrail> — オイラー路。不可なら nullopt
/// @acf-doc @brief オイラー路。不可なら nullopt
/// @acf-doc @code (Graph[, start]) -> optional<EulerTrail>
/// @acf-doc [function] euler_trail
optional<EulerTrail> euler_trail(const Graph<Cost>& graph, int start = -1) {
    if (graph.n == 0) {
        if (start != -1) return nullopt;
        return EulerTrail{};
    }
    if (start < -1 || start >= graph.n) return nullopt;

    if (graph.directed) {
        vi indegree(graph.n), outdegree(graph.n);
        for (const auto& e : graph.edges) {
            ++outdegree[e.from];
            ++indegree[e.to];
        }
        int required_start = -1, required_end = -1;
        for (int v = 0; v < graph.n; ++v) {
            int difference = outdegree[v] - indegree[v];
            if (difference == 1) {
                if (required_start != -1) return nullopt;
                required_start = v;
            } else if (difference == -1) {
                if (required_end != -1) return nullopt;
                required_end = v;
            } else if (difference != 0) {
                return nullopt;
            }
        }
        if ((required_start == -1) != (required_end == -1)) return nullopt;
        if (start == -1) {
            start = required_start;
            if (start == -1)
                for (int v = 0; v < graph.n && start == -1; ++v)
                    if (outdegree[v] > 0) start = v;
        } else if (required_start != -1 && start != required_start) {
            return nullopt;
        } else if (!graph.edges.empty() && outdegree[start] == 0) {
            return nullopt;
        }
    } else {
        vi odd;
        for (int v = 0; v < graph.n; ++v)
            if (graph[v].size() % 2 == 1) odd.push_back(v);
        if (odd.size() != 0 && odd.size() != 2) return nullopt;
        if (start == -1) {
            if (odd.size() == 2) start = odd[0];
            else
                for (int v = 0; v < graph.n && start == -1; ++v)
                    if (!graph[v].empty()) start = v;
        } else if (odd.size() == 2 && start != odd[0] && start != odd[1]) {
            return nullopt;
        } else if (!graph.edges.empty() && graph[start].empty()) {
            return nullopt;
        }
    }
    if (start == -1) start = 0;

    unordered_map<int, int> edge_index;
    edge_index.reserve(graph.edges.size() * 2 + 1);
    for (int i = 0; i < static_cast<int>(graph.edges.size()); ++i) {
        bool inserted = edge_index.emplace(graph.edges[i].id, i).second;
        assert(inserted && "edge ids must be unique");
    }
    vb used(graph.edges.size());
    vi next_index(graph.n), vertex_stack{start}, edge_stack;
    EulerTrail result;
    while (!vertex_stack.empty()) {
        int v = vertex_stack.back();
        while (next_index[v] < static_cast<int>(graph[v].size()) &&
               used[edge_index.at(graph[v][next_index[v]].id)])
            ++next_index[v];
        if (next_index[v] == static_cast<int>(graph[v].size())) {
            result.vertices.push_back(v);
            vertex_stack.pop_back();
            if (!edge_stack.empty()) {
                result.edge_ids.push_back(edge_stack.back());
                edge_stack.pop_back();
            }
            continue;
        }
        const auto& e = graph[v][next_index[v]++];
        int index = edge_index.at(e.id);
        if (used[index]) continue;
        used[index] = true;
        vertex_stack.push_back(e.to);
        edge_stack.push_back(e.id);
    }
    if (result.edge_ids.size() != graph.edges.size()) return nullopt;
    reverse(result.vertices.begin(), result.vertices.end());
    reverse(result.edge_ids.begin(), result.edge_ids.end());
    return result;
}

struct FunctionalGraphInfo {
    vi cycle_id;
    vi distance_to_cycle;
    vi cycle_entry;
    vector<vi> cycles;
};

// @acf fn functional_graph_info — (next) -> FunctionalGraphInfo — 出次数1のサイクル構造
/// @acf-doc @brief 出次数1のサイクル構造
/// @acf-doc @code (next) -> FunctionalGraphInfo
/// @acf-doc [function] functional_graph_info
inline FunctionalGraphInfo functional_graph_info(const vi& next) {
    const int n = static_cast<int>(next.size());
    vi indegree(n), cycle_id(n, -1), distance(n, -1), entry(n, -1), order;
    for (int v = 0; v < n; ++v) {
        assert(0 <= next[v] && next[v] < n);
        ++indegree[next[v]];
    }
    queue<int> que;
    for (int v = 0; v < n; ++v)
        if (indegree[v] == 0) que.push(v);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        order.push_back(v);
        if (--indegree[next[v]] == 0) que.push(next[v]);
    }

    vector<vi> cycles;
    for (int start = 0; start < n; ++start) {
        if (indegree[start] == 0 || cycle_id[start] != -1) continue;
        int id = static_cast<int>(cycles.size());
        cycles.push_back({});
        int v = start;
        do {
            cycle_id[v] = id;
            distance[v] = 0;
            entry[v] = v;
            cycles.back().push_back(v);
            v = next[v];
        } while (v != start);
    }
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int v = *it, to = next[v];
        cycle_id[v] = cycle_id[to];
        distance[v] = distance[to] + 1;
        entry[v] = entry[to];
    }
    return {std::move(cycle_id), std::move(distance), std::move(entry),
            std::move(cycles)};
}

struct FunctionalGraphOrbit {
    vi route;
    int cycle_begin = 0;
    int cycle_length = 0;

    /// @brief `start` から `steps` 回遷移した頂点を返す。
    int at(ull steps) const {
        assert(!route.empty());
        if (steps < static_cast<ull>(cycle_begin))
            return route[static_cast<int>(steps)];
        const ull offset =
            (steps - static_cast<ull>(cycle_begin)) %
            static_cast<ull>(cycle_length);
        return route[cycle_begin + static_cast<int>(offset)];
    }
};

// @acf fn functional_graph_orbit — (next, start) -> FunctionalGraphOrbit — 1始点の軌跡を周期分解
/// @acf-doc @brief 1始点の軌跡を周期分解
/// @acf-doc @code (next, start) -> FunctionalGraphOrbit
/// @acf-doc [function] functional_graph_orbit
inline FunctionalGraphOrbit functional_graph_orbit(const vi& next, int start) {
    const int n = static_cast<int>(next.size());
    assert(n > 0 && 0 <= start && start < n);
    for (int to : next) assert(0 <= to && to < n);

    vi first_seen(n, -1), route;
    route.reserve(n);
    int vertex = start;
    while (first_seen[vertex] == -1) {
        first_seen[vertex] = static_cast<int>(route.size());
        route.push_back(vertex);
        vertex = next[vertex];
    }
    const int cycle_begin = first_seen[vertex];
    const int cycle_length = static_cast<int>(route.size()) - cycle_begin;
    return {std::move(route), cycle_begin, cycle_length};
}

// @acf fn FunctionalGraphDoubling — (next[, bits]) -> doubling — K 回遷移のダブリング
/// @acf-doc @brief K 回遷移のダブリング
/// @acf-doc @code (next[, bits]) -> doubling
/// @acf-doc [function] FunctionalGraphDoubling
struct FunctionalGraphDoubling {
    vector<vi> jump_table;

    /// @brief 未構築のダブリング表を作る。
    FunctionalGraphDoubling() = default;

    /// @brief `next[v]` を `bits` 段前処理し、最大 `2^bits-1` 回の遷移に備える。
    explicit FunctionalGraphDoubling(const vi& next, int bits = 60) {
        assert(1 <= bits && bits <= 64);
        jump_table.assign(bits, vi(next.size()));
        jump_table[0] = next;
        for (int v : next) assert(0 <= v && v < static_cast<int>(next.size()));
        for (int k = 1; k < bits; ++k)
            for (int v = 0; v < static_cast<int>(next.size()); ++v)
                jump_table[k][v] = jump_table[k - 1][jump_table[k - 1][v]];
    }

    /// @brief 頂点 `v` から `steps` 回遷移した頂点を返す。
    int jump(int v, ull steps) const {
        assert(!jump_table.empty());
        assert(0 <= v && v < static_cast<int>(jump_table[0].size()));
        if (jump_table.size() < 64)
            assert((steps >> jump_table.size()) == 0 &&
                   "steps does not fit the prepared doubling table");
        for (int k = 0; k < static_cast<int>(jump_table.size()); ++k)
            if ((steps >> k) & 1ULL) v = jump_table[k][v];
        return v;
    }
};

// ---------------------------------------------------------------------------
// print / pr / cerr / ostringstream support for every public graph type.
// write_graph is the judge-input-like edge format; these operators are a
// descriptive representation intended for inspection and debugging.
// ---------------------------------------------------------------------------

template <class Cost>
ostream& operator<<(ostream& os, const Edge<Cost>& edge) {
    return os << "Edge{from=" << edge.from << ", to=" << edge.to
              << ", cost=" << edge.cost << ", id=" << edge.id << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const Graph<Cost>& graph) {
    os << "Graph{n=" << graph.n
       << ", directed=" << (graph.directed ? "true" : "false") << ", adjacency=[";
    if (graph.n > 0) os << '\n';
    for (int v = 0; v < graph.n; ++v) {
        os << "  " << v << ": [";
        for (int i = 0; i < static_cast<int>(graph[v].size()); ++i) {
            if (i > 0) os << ", ";
            const auto& edge = graph[v][i];
            os << "{to=" << edge.to << ", cost=" << edge.cost
               << ", id=" << edge.id << '}';
        }
        os << ']';
        if (v + 1 < graph.n) os << ',';
        os << '\n';
    }
    return os << "]}";
}

namespace ac_graph_detail {
template <class Dist>
void write_dist_value(ostream& os, Dist value, Dist inf) {
    if (value == inf) os << "inf";
    else os << value;
}

template <class Dist>
void write_dist_vector(ostream& os, const vector<Dist>& dist, Dist inf) {
    for (size_t i = 0; i < dist.size(); ++i) {
        if (i) os << ' ';
        write_dist_value(os, dist[i], inf);
    }
}

template <class Dist>
void write_dist_matrix(ostream& os, const vector<vector<Dist>>& dist, Dist inf) {
    os << '[';
    for (size_t i = 0; i < dist.size(); ++i) {
        if (i) os << "; ";
        write_dist_vector(os, dist[i], inf);
    }
    os << ']';
}
}  // namespace ac_graph_detail

template <class Dist>
ostream& operator<<(ostream& os, const ShortestPathResult<Dist>& result) {
    os << "ShortestPathResult{dist=";
    ac_graph_detail::write_dist_vector(os, result.dist, result.inf);
    return os << ", parent=" << result.parent
              << ", parent_edge=" << result.parent_edge
              << ", inf=" << result.inf << '}';
}

inline ostream& operator<<(ostream& os, const DfsResult& result) {
    return os << "DfsResult{preorder=" << result.preorder
              << ", postorder=" << result.postorder
              << ", parent=" << result.parent
              << ", parent_edge=" << result.parent_edge
              << ", depth=" << result.depth
              << ", root=" << result.root
              << ", component=" << result.component
              << ", entry_time=" << result.entry_time
              << ", exit_time=" << result.exit_time
              << ", roots=" << result.roots << '}';
}

inline ostream& operator<<(ostream& os, const ConnectedComponents& result) {
    return os << "ConnectedComponents{id=" << result.id
              << ", groups=" << result.groups << '}';
}

inline ostream& operator<<(ostream& os, const TreeInfo& result) {
    return os << "TreeInfo{parent=" << result.parent
              << ", parent_edge=" << result.parent_edge
              << ", depth=" << result.depth
              << ", order=" << result.order
              << ", subtree_size=" << result.subtree_size << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const BellmanFordResult<Cost>& result) {
    os << "BellmanFordResult{dist=";
    ac_graph_detail::write_dist_vector(os, result.dist, result.inf);
    return os << ", parent=" << result.parent
              << ", parent_edge=" << result.parent_edge
              << ", negative=" << result.negative
              << ", inf=" << result.inf << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const AllPairsShortestPath<Cost>& result) {
    os << "AllPairsShortestPath{dist=";
    ac_graph_detail::write_dist_matrix(os, result.dist, result.inf);
    return os << ", inf=" << result.inf << '}';
}

inline ostream& operator<<(ostream& os,
                           const StronglyConnectedComponents& result) {
    return os << "StronglyConnectedComponents{id=" << result.id
              << ", groups=" << result.groups << '}';
}

inline ostream& operator<<(ostream& os, const LowLinkResult& result) {
    return os << "LowLinkResult{order=" << result.order
              << ", low=" << result.low
              << ", bridges=" << result.bridges
              << ", articulation_points=" << result.articulation_points << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const MinimumSpanningForest<Cost>& result) {
    return os << "MinimumSpanningForest{cost=" << result.cost
              << ", edge_ids=" << result.edge_ids
              << ", components=" << result.components << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const LowestCommonAncestor<Cost>& result) {
    return os << "LowestCommonAncestor{up=" << result.up
              << ", depth=" << result.depth
              << ", root_distance=" << result.root_distance << '}';
}

template <class Cost>
ostream& operator<<(ostream& os, const TreeDiameterResult<Cost>& result) {
    return os << "TreeDiameterResult{from=" << result.from
              << ", to=" << result.to
              << ", distance=" << result.distance
              << ", path=" << result.path << '}';
}

inline ostream& operator<<(ostream& os, const GraphCycle& result) {
    return os << "GraphCycle{vertices=" << result.vertices
              << ", edge_ids=" << result.edge_ids << '}';
}

inline ostream& operator<<(ostream& os, const EulerTrail& result) {
    return os << "EulerTrail{vertices=" << result.vertices
              << ", edge_ids=" << result.edge_ids << '}';
}

inline ostream& operator<<(ostream& os, const FunctionalGraphInfo& result) {
    return os << "FunctionalGraphInfo{cycle_id=" << result.cycle_id
              << ", distance_to_cycle=" << result.distance_to_cycle
              << ", cycle_entry=" << result.cycle_entry
              << ", cycles=" << result.cycles << '}';
}

inline ostream& operator<<(ostream& os, const FunctionalGraphOrbit& result) {
    return os << "FunctionalGraphOrbit{route=" << result.route
              << ", cycle_begin=" << result.cycle_begin
              << ", cycle_length=" << result.cycle_length << '}';
}

inline ostream& operator<<(ostream& os, const FunctionalGraphDoubling& result) {
    return os << "FunctionalGraphDoubling{jump_table=" << result.jump_table << '}';
}

#if defined(LOCAL) && defined(AC_VISUAL_HPP)
namespace ac_graph_detail {
template <class Cost>
void graphical_render_graph(const Graph<Cost>& graph, int width, int height,
                            ostream& os) {
    using namespace debug::graphical;
    Canvas canvas(max(width, 16), max(height, 8));
    const int n = graph.n;
    vector<pair<long double, long double>> position(n);
    if (n == 1) {
        position[0] = {0, 0};
    } else {
        for (int v = 0; v < n; ++v) {
            const long double angle = PI / 2 - 2 * PI * v / max(1, n);
            position[v] = {cosl(angle), sinl(angle)};
        }
    }
    canvas.expand(-1.35L, -1.35L);
    canvas.expand(1.35L, 1.35L);
    canvas.pad_bounds();

    map<pair<int, int>, int> parallel_count, parallel_seen;
    for (const auto& e : graph.edges)
        ++parallel_count[minmax(e.from, e.to)];

    auto point_on_curve = [](long double x0, long double y0,
                             long double cx, long double cy,
                             long double x1, long double y1, long double t) {
        const long double s = 1 - t;
        return pair{s * s * x0 + 2 * s * t * cx + t * t * x1,
                    s * s * y0 + 2 * s * t * cy + t * t * y1};
    };
    auto draw_curve = [&](const Edge<Cost>& e, int color) {
        auto [x0, y0] = position[e.from];
        auto [x1, y1] = position[e.to];
        if (e.from == e.to) {
            const long double center_x = x0 * 1.12L;
            const long double center_y = y0 * 1.12L;
            const long double radius = 0.16L;
            long double previous_x = center_x + radius, previous_y = center_y;
            for (int step = 1; step <= 32; ++step) {
                const long double angle = 2 * PI * step / 32;
                const long double x = center_x + radius * cosl(angle);
                const long double y = center_y + radius * sinl(angle);
                canvas.draw_segment(previous_x, previous_y, x, y, color);
                previous_x = x;
                previous_y = y;
            }
            return;
        }

        const auto key = minmax(e.from, e.to);
        const int index = parallel_seen[key]++;
        const int total = parallel_count[key];
        const long double dx = x1 - x0, dy = y1 - y0;
        const long double length = max(1e-18L, hypotl(dx, dy));
        const long double offset =
            (index - static_cast<long double>(total - 1) / 2) * 0.18L;
        const long double cx = (x0 + x1) / 2 - dy / length * offset;
        const long double cy = (y0 + y1) / 2 + dx / length * offset;
        auto previous = pair{x0, y0};
        for (int step = 1; step <= 40; ++step) {
            auto current = point_on_curve(x0, y0, cx, cy, x1, y1,
                                          static_cast<long double>(step) / 40);
            canvas.draw_segment(previous.first, previous.second,
                                current.first, current.second, color);
            previous = current;
        }
        if (graph.directed) {
            const long double t = 0.82L;
            auto tip = point_on_curve(x0, y0, cx, cy, x1, y1, t);
            const long double s = 1 - t;
            long double tx = 2 * s * (cx - x0) + 2 * t * (x1 - cx);
            long double ty = 2 * s * (cy - y0) + 2 * t * (y1 - cy);
            const long double norm = max(1e-18L, hypotl(tx, ty));
            tx /= norm;
            ty /= norm;
            const long double size = 0.12L;
            canvas.draw_segment(tip.first, tip.second,
                                tip.first - tx * size - ty * size * 0.55L,
                                tip.second - ty * size + tx * size * 0.55L, color);
            canvas.draw_segment(tip.first, tip.second,
                                tip.first - tx * size + ty * size * 0.55L,
                                tip.second - ty * size - tx * size * 0.55L, color);
        }
    };

    for (int i = 0; i < static_cast<int>(graph.edges.size()); ++i)
        draw_curve(graph.edges[i], i);
    for (int v = 0; v < n; ++v)
        canvas.plot_point(position[v].first, position[v].second, v);
    const int drawn_vertex_label_limit = min(n, 24);
    for (int v = 0; v < drawn_vertex_label_limit; ++v) {
        auto [col, row] = canvas.to_cell(position[v].first, position[v].second);
        const string label = to_string(v);
        const int label_col = position[v].first < -0.15L
                                  ? col - static_cast<int>(label.size())
                                  : col + 1;
        canvas.draw_overlay_text(label_col, row, label);
    }

    const char* connector = graph.directed ? " -> " : " -- ";
    const char* cost_name = [] {
        if constexpr (is_same_v<Cost, int>) return "int";
        else if constexpr (is_same_v<Cost, ll>) return "ll";
        else if constexpr (is_same_v<Cost, double>) return "double";
        else if constexpr (is_same_v<Cost, long double>) return "long double";
        else return "custom";
    }();
    os << kDim << "  graphical_print  Graph<" << cost_name << ">  "
       << (graph.directed ? "directed" : "undirected") << "  V=" << graph.n
       << " E=" << graph.edges.size() << "  index=0-based" << kReset << '\n';
    os << paint_axis("╭");
    for (int col = 0; col < canvas.width; ++col) os << paint_axis("─");
    os << paint_axis("╮") << '\n';
    for (int row = 0; row < canvas.height; ++row) {
        os << paint_axis("│");
        for (int col = 0; col < canvas.width; ++col)
            os << canvas.render_cell(canvas.grid[row][col]);
        os << paint_axis("│") << '\n';
    }
    os << paint_axis("╰");
    for (int col = 0; col < canvas.width; ++col) os << paint_axis("─");
    os << paint_axis("╯") << '\n';

    os << kDim << "  vertices (0-based)" << kReset << '\n';
    const int vertex_limit = min(n, 24);
    for (int v = 0; v < vertex_limit; ++v) {
        const char* color = kColors[static_cast<size_t>(v) %
                                    (sizeof(kColors) / sizeof(kColors[0]))];
        os << "  " << paint(color, "●") << " " << v
           << "  out=" << graph[v].size();
        if (graph.directed) {
            int indegree = 0;
            for (const auto& e : graph.edges) indegree += e.to == v;
            os << " in=" << indegree;
        }
        os << '\n';
    }
    if (n > vertex_limit) os << kDim << "  ... " << n - vertex_limit << " more" << kReset << '\n';

    os << kDim << "  edges" << kReset << '\n';
    const int edge_limit = min(static_cast<int>(graph.edges.size()), 32);
    for (int i = 0; i < edge_limit; ++i) {
        const auto& e = graph.edges[i];
        const char* color = kColors[static_cast<size_t>(i) %
                                    (sizeof(kColors) / sizeof(kColors[0]))];
        os << "  " << paint(color, "─") << " #" << e.id << "  " << e.from
           << connector << e.to << "  cost=" << e.cost << '\n';
    }
    if (static_cast<int>(graph.edges.size()) > edge_limit)
        os << kDim << "  ... " << graph.edges.size() - edge_limit << " more" << kReset << '\n';
    os.flush();
}
}  // namespace ac_graph_detail

template <class Cost>
void graphical_print(const Graph<Cost>& graph) {
    ac_graph_detail::graphical_render_graph(
        graph, debug::graphical::kDefaultWidth, debug::graphical::kDefaultHeight, cerr);
}

template <class Cost>
void graphical_print(int width, int height, const Graph<Cost>& graph) {
    ac_graph_detail::graphical_render_graph(graph, width, height, cerr);
}
#endif
