#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include "ac.hpp"

// LOCALで可視化ヘッダーが未読込の場合、ここでac_visual.hppを読み込む。
// ac_visual.hppはac.hppを再度includeするが、pragma onceにより重複読み込みを防ぐ。
#if defined(LOCAL) && !defined(AC_VISUAL_HPP)
#include "ac_visual.hpp"
#endif

// Graph/Edge は「入力された1本の辺」と「隣接リスト上の向き」を同時に扱う。
// 頂点番号はライブラリ内部では常に 0-indexed。
// @acf using Edge — Edge<Cost> — グラフ辺（from/to/cost/id）。内部は 0-indexed

/// グラフの1本の辺を、始点・終点・重み・識別番号で表す値です。隣接する頂点や、復元した経路に使われた辺を調べるときに使います。
///
/// 注意: `from`と`to`は0-indexedの頂点番号です。`cost`は問題で定める辺重みの単位です。`id`は頂点番号ではなく辺の識別番号です。`Graph::add_edge`が作る無向辺は`adj`に両向きで入り、両方の`id`が同じです。辺だけを書き換えても`Graph`全体の対応は更新されません。
template <class Cost = ll>
struct Edge {
    /// 始点（0-indexed）。
    int from = -1;
    /// 終点（0-indexed）。
    int to = -1;
    /// 辺重み。未重みでは `1`。
    Cost cost = Cost{1};
    /// 入力辺番号。無向の両向きで共有。
    int id = -1;

    /// 辺の端点・重み・IDがすべて一致するか比較する。
    friend bool operator==(const Edge&, const Edge&) = default;
};

// @acf using Graph — Graph<Cost> — 隣接リストと辺リストを持つグラフ型

/// 頂点と辺のつながりを表すグラフです。各頂点から出る辺を`adj`、入力した辺を`edges`に保持し、BFS・最短路などの探索へ渡します。
///
/// 注意: 頂点番号は0から`n-1`です。`Graph(n)`は無向、`Graph(n,true)`は有向です。`add_edge`は`adj`と`edges`を両方更新し、無向辺は`adj`に両向き、`edges`に1本だけ入ります。未指定の重みは1です。自動の辺IDは使用済みを避け、明示IDの重複は`invalid_argument`になります。両配列を直接書き換えると対応やID管理が崩れるため、辺の追加には`add_edge`を使います。
template <class Cost = ll>
struct Graph {
    using cost_type = Cost;
    using edge_type = Edge<Cost>;

    /// 頂点数。
    int n = 0;
    /// 有向グラフか。
    bool directed = false;
    /// 隣接リスト。
    vector<vector<edge_type>> adj;
    /// 入力辺（無向も1入力につき1要素）。
    vector<edge_type> edges;  // 無向辺も入力1本につき1要素
private:
    unordered_set<int> used_edge_ids;
    long long next_auto_edge_id = 0;

public:

    /// 頂点数0の空グラフを構築する。
    Graph() = default;

    /// `vertex_count` 頂点のグラフを構築する。頂点番号は0-indexed。
    explicit Graph(int vertex_count, bool is_directed = false)
        : n(vertex_count), directed(is_directed), adj(vertex_count) {
        assert(vertex_count >= 0);
    }

    /// 頂点数を返す。
    int size() const { return n; }

    /// 頂点数が0か判定する。
    bool empty() const { return n == 0; }

    /// 頂点 `v` から出る隣接辺の一覧を返す（const版）。
    const vector<edge_type>& operator[](int v) const { return adj[v]; }

    /// 頂点 `v` から出る隣接辺の一覧を返す。
    vector<edge_type>& operator[](int v) { return adj[v]; }

    /// 辺を追加し、辺IDを返す。自動IDは使用済みを避け、明示IDの重複はinvalid_argument。辺はadd_edgeで追加し、edgesとadjを直接変更しない。無向辺は逆向きの隣接辺も追加する。
    int add_edge(int from, int to, Cost cost = Cost{1}, int id = -1) {
        assert(0 <= from && from < n && 0 <= to && to < n);
        if (id < 0) {
            next_auto_edge_id = max(next_auto_edge_id, static_cast<long long>(edges.size()));
            while (next_auto_edge_id <= numeric_limits<int>::max() &&
                   used_edge_ids.contains(static_cast<int>(next_auto_edge_id)))
                ++next_auto_edge_id;
            if (next_auto_edge_id > numeric_limits<int>::max())
                throw overflow_error("no automatic edge id available");
            id = static_cast<int>(next_auto_edge_id++);
        }
        if (!used_edge_ids.insert(id).second)
            throw invalid_argument("edge ids must be unique");
        edge_type edge{from, to, cost, id};
        edges.push_back(edge);
        adj[from].push_back(edge);
        if (!directed) adj[to].push_back(edge_type{to, from, cost, id});
        return id;
    }

    /// 有向グラフの全辺を逆向きにしたグラフを返す。無向グラフはコピーを返す。
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
// 無向グラフのvviは、read_graphと同じく各辺が両端の隣接リストに入った形式を受け取る。
// @acf fn to_graph — (vvi[, directed]) -> Graph — 旧 vvi 隣接リストから Graph を作る

/// 旧vvi隣接リストを、辺ID付きのGraphへ変換する。
///
/// 引数 `adjacency`: 各隣接先は0..N-1。無向版は両方向を同数ずつ含め、自己ループ1本は2要素で表す。
///
/// 引数 `directed`: trueは各要素を有向辺として追加。省略時は `false`。
///
/// 戻り値: `Graph<ll>`。重みは1、辺IDは新たな連番。無向版は端点対順に再構築するので元の隣接順は保持しません。空は0頂点。
///
/// 注意: 元のvviは変更せず出力を確保。無向版は端点対をmapで数えます。非対称な無向表現はassert。
///
/// 呼び分け: 重み付きの辺リストを変換する場合はgraph_from_edges。to_graphは行き先だけの旧形式を受け取ります。
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

// @acf fn to_adjacency_list — (Graph) -> vvi — Graph を行き先だけの隣接リストへ

/// Graphから行き先だけの旧vvi隣接リストを取り出す。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 戻り値: vvi。各頂点の隣接順を保持。重みと辺IDは捨てます。無向辺は両方向に現れ、自己ループは2要素。空は空のvvi。
///
/// 注意: 入力を変更せず隣接先をコピー。時間/出力メモリO(N+M)。
///
/// 呼び分け: 重みを保持したい場合はto_edge_list。
template <class Cost>
vvi to_adjacency_list(const Graph<Cost>& graph) {
    vvi adjacency(graph.n);
    for (int v = 0; v < graph.n; ++v) {
        adjacency[v].reserve(graph[v].size());
        for (const auto& e : graph[v]) adjacency[v].push_back(e.to);
    }
    return adjacency;
}

// @acf fn graph_from_edges — (n, edges[, directed, index_base]) -> Graph — 辺リストから Graph

/// 端点と重みのtuple列からGraphを構築する。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `edges`: (from,to,cost)を1辺につき1要素。index_baseを引いた端点が0..n-1。
///
/// 引数 `directed`: 有向ならtrue。省略時は `false`。
///
/// 引数 `index_base`: 入力端点の添字の起点。省略時は `0`。
///
/// 戻り値: `Graph<Cost>`。重みと入力の辺順を保持し、辺IDは0から連番。無向の逆向き隣接辺は同じID。空辺列も可。
///
/// 注意: 入力を変更せず隣接リストと辺リストを構築。時間/メモリO(N+M)。不正端点はassert。
///
/// 呼び分け: graph_from_edgesは上の引数順で呼び出します。
template <class Cost>
Graph<Cost> graph_from_edges(
    int n, const vector<tuple<int, int, Cost>>& edges,
    bool directed = false, int index_base = 0) {
    Graph<Cost> graph(n, directed);
    graph.edges.reserve(edges.size());
    for (const auto& [input_from, input_to, cost] : edges)
        graph.add_edge(input_from - index_base, input_to - index_base, cost);
    return graph;
}

// @acf fn to_edge_list — (Graph[, index_base]) -> vector<tuple> — Graph を辺リストへ

/// Graphを端点・重みのtuple列へ変換する。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `index_base`: 出力端点に加える添字の起点。加算がintに収まること。省略時は `0`。
///
/// 戻り値: `vector<tuple<int,int,Cost>>`。graph.edgesの順で1辺につき1要素。無向辺も1回だけ。重みを保持し辺IDは捨てます。辺なしは空。
///
/// 注意: 入力を変更せず辺をコピー。時間/出力メモリO(M)。
///
/// 呼び分け: to_edge_listは上の引数順で呼び出します。
template <class Cost>
vector<tuple<int, int, Cost>> to_edge_list(
    const Graph<Cost>& graph, int index_base = 0) {
    vector<tuple<int, int, Cost>> result;
    result.reserve(graph.edges.size());
    for (const auto& e : graph.edges)
        result.emplace_back(e.from + index_base, e.to + index_base, e.cost);
    return result;
}

// @acf fn is_tree — (Graph) -> bool — 無向・連結・辺数 N-1 か

/// Graphが非空の無向木か判定する。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 戻り値: bool。無向・連結・辺数N-1ならtrue。有向グラフ、0頂点、非連結、自己ループや多重辺による閉路はfalse。1頂点0辺はtrue。
///
/// 注意: 入力を変更せず接続を探索。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: is_treeは上の引数順で呼び出します。
template <class Cost>
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

// a b を m 行読む。index_base=1 がAtCoder標準、0 なら変換しない。
// @acf fn read_unweighted_graph — (n,m[,directed,index_base]) -> Graph — a b を m 行読む

/// 未重みグラフのm辺を標準入力から読む。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `m`: 非負の読み取る辺数。
///
/// 引数 `directed`: trueは有向、falseは無向。省略時は `false`。
///
/// 引数 `index_base`: 入力の端点の起点。引いた端点は0..n-1。省略時は `1`。
///
/// 戻り値: `Graph<ll>`。内部は0-index、IDは入力順の連番。0辺なら辺なしのn頂点。 全辺の重みは1。
///
/// 注意: cinからm個の端点2個を消費。全項を正しく読み取れる入力が前提で、失敗を表す戻り値はありません。端点の有効範囲はassert。時間/メモリO(N+M)。
///
/// 呼び分け: 旧vvi形式はread_graph。こちらはGraphで重みと辺IDを利用できます。
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

/// 重み付きグラフのm辺を標準入力から読む。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `m`: 非負の読み取る辺数。
///
/// 引数 `directed`: trueは有向、falseは無向。省略時は `false`。
///
/// 引数 `index_base`: 入力の端点の起点。引いた端点は0..n-1。省略時は `1`。
///
/// 戻り値: `Graph<Cost>`（Costの既定はll）。内部は0-index、IDは入力順の連番。0辺なら辺なしのn頂点。 重みを入力から保持します。
///
/// 注意: cinからm個の端点2個と重みを消費。全項を正しく読み取れる入力が前提で、失敗を表す戻り値はありません。端点の有効範囲はassert。時間/メモリO(N+M)。
///
/// 呼び分け: 旧vvi形式はread_graph。こちらはGraphで重みと辺IDを利用できます。
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

/// 未重み木のn-1辺を標準入力から読む。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `index_base`: 入力の端点の起点。引いた端点は0..n-1。省略時は `1`。
///
/// 戻り値: `Graph<ll>`。内部は0-index、IDは入力順の連番。0/1頂点では0辺。木であるかは検査しないため必要ならis_treeを使います。 全辺の重みは1。
///
/// 注意: cinからmax(0,n-1)個の端点2個を消費。全項を正しく読み取れる入力が前提で、失敗を表す戻り値はありません。端点の有効範囲はassert。時間/メモリO(N+M)。
///
/// 呼び分け: 旧vvi形式はread_tree。こちらはGraphで重みと辺IDを利用できます。
inline Graph<> read_unweighted_tree(int n, int index_base = 1) {
    return read_unweighted_graph(n, max(0, n - 1), false, index_base);
}

// @acf fn read_weighted_tree — (n[,index_base]) -> Graph — 重み付き木を読む

/// 重み付き木のn-1辺を標準入力から読む。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `index_base`: 入力の端点の起点。引いた端点は0..n-1。省略時は `1`。
///
/// 戻り値: `Graph<Cost>`（Costの既定はll）。内部は0-index、IDは入力順の連番。0/1頂点では0辺。木であるかは検査しないため必要ならis_treeを使います。 重みを入力から保持します。
///
/// 注意: cinからmax(0,n-1)個の端点2個と重みを消費。全項を正しく読み取れる入力が前提で、失敗を表す戻り値はありません。端点の有効範囲はassert。時間/メモリO(N+M)。
///
/// 呼び分け: 旧vvi形式はread_tree。こちらはGraphで重みと辺IDを利用できます。
template <class Cost = ll>
Graph<Cost> read_weighted_tree(int n, int index_base = 1) {
    return read_weighted_graph<Cost>(n, max(0, n - 1), false, index_base);
}

// 入力形式へ戻したいときの出力。無向辺は1回だけ出力する。
// @acf fn write_graph — (os, Graph[, index_base, with_cost]) — 辺リスト形式で出力

/// Graphの各辺を端点と重みの行として出力する。
///
/// 引数 `os`: 出力先ストリーム。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `index_base`: 出力端点に加える値。intに収まること。省略時は `1`。
///
/// 引数 `with_cost`: trueは重みも出力。省略時は `true`。
///
/// 戻り値: void。頂点数・辺数の見出しは出力しません。graph.edges順で1辺1行、無向辺も1回。辺なしは何も出力しません。
///
/// 注意: osに書き込み、Graphは変更しません。時間O(M)。
///
/// 呼び分け: print(graph)のデバッグ表現とは形式が異なります。入力の辺行形式にはwrite_graphを使います。
template <class Cost>
void write_graph(ostream& os, const Graph<Cost>& graph,
                 int index_base = 1, bool with_cost = true) {
    for (const auto& e : graph.edges) {
        os << e.from + index_base << ' ' << e.to + index_base;
        if (with_cost) os << ' ' << e.cost;
        os << '\n';
    }
}

/// ある始点、または始点集合から各頂点までの最短距離と、選ばれた経路の復元情報を表す結果です。reachable(v) で到達を判定してから dist[v] を使い、path_to(v) で始点からの頂点列を取り出します。
template <class Dist>
struct ShortestPathResult {
    /// 始点からの最短距離。添字は頂点番号（届かない頂点もスロットがあり `inf`）。
    vector<Dist> dist;
    /// 最短路上の親。始点・到達不能は `-1`。
    vi parent;
    /// 親へ向かう辺の `id`。始点・到達不能は `-1`。
    vi parent_edge;
    /// 到達不能の番兵。既定は `numeric_limits<T>::max()/4`。`print(r)` の dist 表示では `inf` と出る。
    Dist inf{};

    /// 頂点 `v` が始点のいずれかから到達可能か判定する。
    bool reachable(int v) const { return dist[v] != inf; }

    /// `target` までの頂点列を始点から復元する。到達不能なら空を返す。
    vi path_to(int target) const {
        if (target < 0 || target >= static_cast<int>(dist.size()) ||
            !reachable(target)) return {};
        vi path;
        for (int v = target; v != -1; v = parent[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }

    // @acf fn edge_path_to — (target) -> vi — 最短路上の辺ID列を復元

    /// `target` までの最短路に使う辺ID列を復元する。到達不能なら空を返す。
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

// @acf fn bfs — (Graph, s|sources) -> ShortestPathResult — 未重み最短手数（1辺=1）

/// 辺の重みを無視し、始点からの最小辺数を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 戻り値: `ShortestPathResult<ll>`を返します。distは辺数、未到達はinf=ll最大値/4です。根のparent=-1です。path_toで頂点列、edge_path_toで辺ID列を復元します。始点列が空なら全頂点が未到達です。
///
/// 注意: 入力を変更せずqueueで探索。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: bfs(graph,source) / bfs(graph,sources)。重みを使う最短路にはdijkstraなどを選びます。
template <class Cost>
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

/// 辺の重みを無視し、始点からの最小辺数を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 戻り値: `ShortestPathResult<ll>`を返します。distは辺数、未到達はinf=ll最大値/4です。根のparent=-1です。path_toで頂点列、edge_path_toで辺ID列を復元します。始点列が空なら全頂点が未到達です。
///
/// 注意: 入力を変更せずqueueで探索。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: bfs(graph,source) / bfs(graph,sources)。重みを使う最短路にはdijkstraなどを選びます。
template <class Cost>
ShortestPathResult<ll> bfs(const Graph<Cost>& graph, int source) {
    return bfs(graph, vi{source});
}

// グリッド／迷路: 頂点は密番号 id = r * W + c（壁マスも番号は持つが辺を張らない／訪問しない）。
// @acf fn grid_id — (r,c,w) -> int — グリッド座標を密頂点番号へ

/// 0-indexのグリッド座標を密な頂点番号へ変換する。
///
/// 引数 `r`: 非負の行番号。
///
/// 引数 `c`: 0..w-1の列番号。
///
/// 引数 `w`: 正の盤面幅。
///
/// 戻り値: intのr*w+c。盤面の高さや範囲は検査しません。計算結果がintに収まること。
///
/// 注意: 副作用なし。時間O(1)。
///
/// 呼び分け: 逆変換はgrid_rc。
///
/// 例: `print(grid_id(1,2,4));`
constexpr int grid_id(int r, int c, int w) {
    return r * w + c;
}

// @acf fn grid_rc — (id,w) -> pair<int,int> — 密頂点番号を (r,c) へ

/// 密な頂点番号を0-indexの行・列へ戻す。
///
/// 引数 `id`: 非負の密な頂点番号。
///
/// 引数 `w`: 正の盤面幅。0を渡してはいけません。
///
/// 戻り値: `pair<int,int>`の(id/w,id%w)。高さや上限は検査しません。
///
/// 注意: 副作用なし。時間O(1)。
///
/// 呼び分け: 順変換はgrid_id。
constexpr pair<int, int> grid_rc(int id, int w) {
    return {id / w, id % w};
}

// @acf const kGrid4 — 4近傍 (dr,dc)（上下左右）

/// 4近傍 (dr,dc)（上下左右）
inline constexpr array<pair<int, int>, 4> kGrid4 = {{
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
}};

// @acf const kGrid8 — 8近傍 (dr,dc)

/// 8近傍 (dr,dc)
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

/// 通行可能な隣接マスを、密な頂点番号の無向Graphへ変換する。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `Graph<ll>`。全H*Wマスが頂点となり、壁は孤立頂点。通行マス間の辺の重みは1。頂点番号はr*W+c。
///
/// 注意: 盤面を変更せずGraphを構築。passableは同じセルに複数回呼ばれ得るので安定した判定を使います。時間O(HW*方向数)、出力は頂点と辺に比例。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableが必要。常に無向グラフでu<vだけを追加するため、独自dirsは対称な方向列を使います。片方向移動はgrid_bfsで扱います。
template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
Graph<> graph_from_grid(const vector<string>& grid, Passable passable,
                        const Dirs& dirs = kGrid4) {
    return ac_graph_detail::graph_from_grid_impl(grid, passable, dirs);
}

/// 通行可能な隣接マスを、密な頂点番号の無向Graphへ変換する。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `Graph<ll>`。全H*Wマスが頂点となり、壁は孤立頂点。通行マス間の辺の重みは1。頂点番号はr*W+c。
///
/// 注意: 盤面を変更せずGraphを構築。passableは同じセルに複数回呼ばれ得るので安定した判定を使います。時間O(HW*方向数)、出力は頂点と辺に比例。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableが必要。常に無向グラフでu<vだけを追加するため、独自dirsは対称な方向列を使います。片方向移動はgrid_bfsで扱います。
template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
Graph<> graph_from_grid(const vector<vector<Cell>>& grid, Passable passable,
                        const Dirs& dirs = kGrid4) {
    return ac_graph_detail::graph_from_grid_impl(grid, passable, dirs);
}

/// 通行可能な隣接マスを、密な頂点番号の無向Graphへ変換する。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `Graph<ll>`。全H*Wマスが頂点となり、壁は孤立頂点。通行マス間の辺の重みは1。頂点番号はr*W+c。
///
/// 注意: 盤面を変更せずGraphを構築。passableは同じセルに複数回呼ばれ得るので安定した判定を使います。時間O(HW*方向数)、出力は頂点と辺に比例。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableが必要。常に無向グラフでu<vだけを追加するため、独自dirsは対称な方向列を使います。片方向移動はgrid_bfsで扱います。
template <class Dirs = decltype(kGrid4)>
Graph<> graph_from_grid(const vector<string>& grid, char wall = '#',
                        const Dirs& dirs = kGrid4) {
    return graph_from_grid(
        grid, [wall](char cell) { return cell != wall; }, dirs);
}

// @acf fn grid_passable — (盤面[, passable|wall]) -> vb — 通行マスの密マスク（長さ H*W）

/// 盤面の通行可否を密な頂点番号のマスクにする。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 戻り値: vbの長さH*W。r*W+cが通行可能ならtrue。孤立した通行マスもtrueになるので、盤面復元時に保持できます。
///
/// 注意: 盤面を変更せず各セルを1回判定。時間/出力メモリO(HW)。callbackの副作用は利用者側の責任です。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableを渡します。
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

/// 盤面の通行可否を密な頂点番号のマスクにする。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 戻り値: vbの長さH*W。r*W+cが通行可能ならtrue。孤立した通行マスもtrueになるので、盤面復元時に保持できます。
///
/// 注意: 盤面を変更せず各セルを1回判定。時間/出力メモリO(HW)。callbackの副作用は利用者側の責任です。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableを渡します。
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

/// 盤面の通行可否を密な頂点番号のマスクにする。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 戻り値: vbの長さH*W。r*W+cが通行可能ならtrue。孤立した通行マスもtrueになるので、盤面復元時に保持できます。
///
/// 注意: 盤面を変更せず各セルを1回判定。時間/出力メモリO(HW)。callbackの副作用は利用者側の責任です。
///
/// 呼び分け: 文字盤面はwallを省略可。一般のセル型はcallableを渡します。
inline vb grid_passable(const vector<string>& grid, char wall = '#') {
    return grid_passable(grid, [wall](char cell) { return cell != wall; });
}

// @acf fn grid_from_graph — (Graph, h, w[, passable][, floor, wall]) -> vs — 密 Graph を盤面へ戻す

/// 密なグリッド頂点の通行可否を文字盤面へ戻す。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `h / w`: 正の高さ/幅。h*wがintに収まりgraph.nと一致。
///
/// 引数 `passable（マスク/関数版）`: 長さh*wのマスク、または頂点IDの通行可否。
///
/// 引数 `floor`: 通行マスへ書く文字。省略時は `'.'`。
///
/// 引数 `wall`: 壁マスへ書く文字。省略時は `'#'`。
///
/// 戻り値: vsのh行w列。辺そのものの配置を復元する関数ではありません。通行指定なし版は次数0を壁にするため、孤立した通行マスを失います。
///
/// 注意: Graphを変更せず文字盤面を確保。時間/出力メモリO(HW)。
///
/// 呼び分け: (graph,h,w[,mask|callable][,floor,wall])。元盤面版は(graph,board[,wall,floor])で文字引数の順序が異なり、元の非壁文字は全てfloorへ置き換えます。
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

/// 密なグリッド頂点の通行可否を文字盤面へ戻す。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `h / w`: 正の高さ/幅。h*wがintに収まりgraph.nと一致。
///
/// 引数 `passable（マスク/関数版）`: 長さh*wのマスク、または頂点IDの通行可否。
///
/// 引数 `floor`: 通行マスへ書く文字。省略時は `'.'`。
///
/// 引数 `wall`: 壁マスへ書く文字。省略時は `'#'`。
///
/// 戻り値: vsのh行w列。辺そのものの配置を復元する関数ではありません。通行指定なし版は次数0を壁にするため、孤立した通行マスを失います。
///
/// 注意: Graphを変更せず文字盤面を確保。時間/出力メモリO(HW)。
///
/// 呼び分け: (graph,h,w[,mask|callable][,floor,wall])。元盤面版は(graph,board[,wall,floor])で文字引数の順序が異なり、元の非壁文字は全てfloorへ置き換えます。
template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, int h, int w, const vb& passable,
                   char floor = '.', char wall = '#') {
    assert(static_cast<int>(passable.size()) == h * w);
    return grid_from_graph(
        graph, h, w, [&](int id) { return passable[id]; }, floor, wall);
}

/// 密なグリッド頂点の通行可否を文字盤面へ戻す。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `h / w`: 正の高さ/幅。h*wがintに収まりgraph.nと一致。
///
/// 引数 `passable（マスク/関数版）`: 長さh*wのマスク、または頂点IDの通行可否。
///
/// 引数 `floor`: 通行マスへ書く文字。省略時は `'.'`。
///
/// 引数 `wall`: 壁マスへ書く文字。省略時は `'#'`。
///
/// 戻り値: vsのh行w列。辺そのものの配置を復元する関数ではありません。通行指定なし版は次数0を壁にするため、孤立した通行マスを失います。
///
/// 注意: Graphを変更せず文字盤面を確保。時間/出力メモリO(HW)。
///
/// 呼び分け: (graph,h,w[,mask|callable][,floor,wall])。元盤面版は(graph,board[,wall,floor])で文字引数の順序が異なり、元の非壁文字は全てfloorへ置き換えます。
template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, int h, int w, char floor = '.',
                   char wall = '#') {
    // Graphだけから復元すると、次数0の通行マスは壁になる。孤立マスを含む場合はgrid_passableを渡す。
    return grid_from_graph(
        graph, h, w, [&](int id) { return !graph[id].empty(); }, floor, wall);
}

/// 密なグリッド頂点の通行可否を文字盤面へ戻す。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `h / w`: 正の高さ/幅。h*wがintに収まりgraph.nと一致。
///
/// 引数 `passable（マスク/関数版）`: 長さh*wのマスク、または頂点IDの通行可否。
///
/// 引数 `floor`: 通行マスへ書く文字。省略時は `'.'`。
///
/// 引数 `wall`: 壁マスへ書く文字。省略時は `'#'`。
///
/// 戻り値: vsのh行w列。辺そのものの配置を復元する関数ではありません。通行指定なし版は次数0を壁にするため、孤立した通行マスを失います。
///
/// 注意: Graphを変更せず文字盤面を確保。時間/出力メモリO(HW)。
///
/// 呼び分け: (graph,h,w[,mask|callable][,floor,wall])。元盤面版は(graph,board[,wall,floor])で文字引数の順序が異なり、元の非壁文字は全てfloorへ置き換えます。
template <class Cost>
vs grid_from_graph(const Graph<Cost>& graph, const vector<string>& board,
                   char wall = '#', char floor = '.') {
    const int h = ac_graph_detail::grid_h(board);
    const int w = ac_graph_detail::grid_w(board);
    return grid_from_graph(graph, h, w, grid_passable(board, wall), floor, wall);
}

// @acf fn grid_bfs — (盤面, 始点|sources[, passable|wall][, dirs]) -> ShortestPathResult — 盤面上の最短手数

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, const vi& sources,
                                Passable passable, const Dirs& dirs = kGrid4) {
    return ac_graph_detail::grid_bfs_impl(grid, sources, passable, dirs);
}

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, char>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, int sr, int sc,
                                Passable passable, const Dirs& dirs = kGrid4) {
    const int w = ac_graph_detail::grid_w(grid);
    return grid_bfs(grid, vi{grid_id(sr, sc, w)}, passable, dirs);
}

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
ShortestPathResult<ll> grid_bfs(const vector<vector<Cell>>& grid, const vi& sources,
                                Passable passable, const Dirs& dirs = kGrid4) {
    return ac_graph_detail::grid_bfs_impl(grid, sources, passable, dirs);
}

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Cell, class Passable, class Dirs = decltype(kGrid4)>
    requires invocable<Passable&, const Cell&>
ShortestPathResult<ll> grid_bfs(const vector<vector<Cell>>& grid, int sr, int sc,
                                Passable passable, const Dirs& dirs = kGrid4) {
    const int w = ac_graph_detail::grid_w(grid);
    return grid_bfs(grid, vi{grid_id(sr, sc, w)}, passable, dirs);
}

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Dirs = decltype(kGrid4)>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, const vi& sources,
                                char wall = '#', const Dirs& dirs = kGrid4) {
    return grid_bfs(
        grid, sources, [wall](char cell) { return cell != wall; }, dirs);
}

/// 盤面上で、指定方向に通行できる最短手数を求める。
///
/// 引数 `grid`: 高さH>=1、幅W>=1の長方形の盤面。H*Wがintに収まること。
///
/// 引数 `sources / sr,sc`: 密な0-index始点ID列、または有効な0-index行・列。空/重複始点も可。
///
/// 引数 `passable / wall`: callableはセルを受け取り通行可否を返す。文字盤面は指定した壁文字以外が通行可。省略時は `wall = '#'（文字盤面）`。
///
/// 引数 `dirs`: (dr,dc)の移動方向列。省略時は `kGrid4（上下左右）`。
///
/// 戻り値: `ShortestPathResult<ll>`。distは手数、未到達はinf。path_toは密な頂点ID列。parent_edgeは全て-1で辺ID復元には使えません。壁上の始点は無視し、空始点は全マス未到達。
///
/// 注意: 盤面を変更せずqueueで探索。時間O(HW*方向数)、追加メモリO(HW)。passableは安定した判定を使います。
///
/// 呼び分け: 文字盤面はwallを省略可、一般のセル型はcallableを渡します。dirsは一方向も可。空の盤面を渡す形式はありません。
template <class Dirs = decltype(kGrid4)>
ShortestPathResult<ll> grid_bfs(const vector<string>& grid, int sr, int sc,
                                char wall = '#', const Dirs& dirs = kGrid4) {
    return grid_bfs(
        grid, sr, sc, [wall](char cell) { return cell != wall; }, dirs);
}

/// 深さ優先探索でできた木または森と、各頂点へ入る／出る順番を保持する結果です。DFS木上の親子関係、祖先判定、根からの経路を調べるときに使います。
struct DfsResult {
    /// DFSで頂点へ入った順の頂点列。
    vi preorder;     // 行きがけ順（頂点へ入った順）
    /// DFSで頂点から出た順の頂点列。
    vi postorder;    // 帰りがけ順（頂点を出た順）
    /// DFS木の親頂点。根・未訪問は -1。
    vi parent;
    /// DFS木で親から来た辺の ID。根・未訪問は -1。
    vi parent_edge;
    /// DFS木の根からの辺数。根0、未訪問 -1。
    vi depth;
    /// 所属するDFS木の根の頂点番号。未訪問 -1。
    vi root;         // 各頂点が属するDFS木の根
    /// DFS木の番号（0-indexed）。未訪問 -1。
    vi component;    // DFS木の番号。未訪問は -1
    /// 頂点へ入ったイベントの連番。未訪問 -1。
    vi entry_time;   // 入時刻
    /// 頂点から出たイベントの連番。未訪問 -1。
    vi exit_time;    // 出時刻
    /// 探索を開始した根の頂点列。添字はDFS木の番号。
    vi roots;        // 実際に探索を開始した根

    /// 頂点 `v` がDFSで訪問済みか判定する。
    bool reachable(int v) const { return component[v] != -1; }

    /// 同じDFS木で `ancestor` が `descendant` の祖先か判定する。
    bool is_ancestor(int ancestor, int descendant) const {
        if (!reachable(ancestor) || !reachable(descendant) ||
            component[ancestor] != component[descendant]) return false;
        return entry_time[ancestor] <= entry_time[descendant] &&
               exit_time[descendant] <= exit_time[ancestor];
    }

    /// DFS木の根から `target` までの頂点列を復元する。
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

/// 指定始点から到達できる部分のDFS順と探索木を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 戻り値: DfsResult。preorder/postorderは訪問順。未訪問のcomponent/depth/parent等は-1。rootsは実際に探索を開始した根だけ。複数始点の空列は何も訪問しません。
///
/// 注意: 入力を変更せず反復DFS。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: dfs(graph,source) / dfs(graph,sources)。有向グラフのcomponentはDFS木の番号であり強連結成分ではありません。全頂点はdfs_forest。
template <class Cost>
DfsResult dfs(const Graph<Cost>& graph, int source) {
    return ac_graph_detail::dfs_impl(graph, vi{source}, false);
}

// sources を順に根候補として探索する。先の根から訪問済みなら新しい木にしない。
/// 指定始点から到達できる部分のDFS順と探索木を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 戻り値: DfsResult。preorder/postorderは訪問順。未訪問のcomponent/depth/parent等は-1。rootsは実際に探索を開始した根だけ。複数始点の空列は何も訪問しません。
///
/// 注意: 入力を変更せず反復DFS。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: dfs(graph,source) / dfs(graph,sources)。有向グラフのcomponentはDFS木の番号であり強連結成分ではありません。全頂点はdfs_forest。
template <class Cost>
DfsResult dfs(const Graph<Cost>& graph, const vi& sources) {
    return ac_graph_detail::dfs_impl(graph, sources, false);
}

// 非連結部分も含め、全頂点のDFS森を作る。
// @acf fn dfs_forest — (Graph) -> DfsResult — 全頂点の DFS 森

/// 全頂点を覆うDFS森の順序と探索木を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 戻り値: DfsResult。未訪問頂点を番号の小さい順に根にして全頂点を訪問。空グラフは全配列が空。componentはDFS木番号。
///
/// 注意: 入力を変更せず反復DFS。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: 指定始点だけならdfs。有向グラフのDFS木はSCCではありません。
template <class Cost>
DfsResult dfs_forest(const Graph<Cost>& graph) {
    return ac_graph_detail::dfs_impl(graph, {}, true);
}

// @acf fn dijkstra — (Graph, sources[, inf]) -> ShortestPathResult — 非負重みの単一/複数始点最短路

/// 非負重みのグラフで、単一または複数始点からの最短距離を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 引数 `inf`: 到達不能を表す正の値。全ての有限距離の絶対値より十分大きく取る。省略時は `numeric_limits<Cost>::max() / Cost{4}`。
///
/// 戻り値: `ShortestPathResult<Cost>`を返します。dist/parent/parent_edgeを持ち、未到達のdistはinfです。始点列が空なら全頂点が未到達です。inf以上に丸められた距離も未到達として扱うため、infを適切に選びます。
///
/// 注意: 入力を変更せずpriority_queueで探索。時間O((N+M)log N)、追加状態は頂点とqueueに比例。64bit以下の整数の和は±infで飽和。到達辺の負重みはassert。
///
/// 呼び分け: dijkstra(graph,source[,inf]) / dijkstra(graph,sources[,inf])。負辺はbellman_ford、0/1辺はzero_one_bfs。
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


/// 非負重みのグラフで、単一または複数始点からの最短距離を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `sources / source`: 複数の0-index始点、または単一の始点。複数版は空・重複も可。
///
/// 引数 `inf`: 到達不能を表す正の値。全ての有限距離の絶対値より十分大きく取る。省略時は `numeric_limits<Cost>::max() / Cost{4}`。
///
/// 戻り値: `ShortestPathResult<Cost>`を返します。dist/parent/parent_edgeを持ち、未到達のdistはinfです。始点列が空なら全頂点が未到達です。inf以上に丸められた距離も未到達として扱うため、infを適切に選びます。
///
/// 注意: 入力を変更せずpriority_queueで探索。時間O((N+M)log N)、追加状態は頂点とqueueに比例。64bit以下の整数の和は±infで飽和。到達辺の負重みはassert。
///
/// 呼び分け: dijkstra(graph,source[,inf]) / dijkstra(graph,sources[,inf])。負辺はbellman_ford、0/1辺はzero_one_bfs。
template <class Cost>
ShortestPathResult<Cost> dijkstra(
    const Graph<Cost>& graph, int source,
    Cost inf = numeric_limits<Cost>::max() / Cost{4}) {
    return dijkstra(graph, vi{source}, inf);
}

// @acf fn zero_one_bfs — (Graph, s[, inf]) -> ShortestPathResult — 辺重み 0/1 の最短路

/// 0または1の辺重みを持つグラフの最短距離を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `source`: 0..N-1の始点。N>=1。
///
/// 引数 `inf`: 到達不能を表す正の値。全ての有限距離の絶対値より十分大きく取る。省略時は `numeric_limits<Cost>::max() / Cost{4}`。
///
/// 戻り値: `ShortestPathResult<Cost>`。未到達はinf、path_to/edge_path_toで経路復元。全ての辺重みが0/1で、距離を表せるinfを選ぶこと。
///
/// 注意: 入力を変更せずdequeで0辺を先頭、1辺を末尾へ追加。時間O(N+M)、追加メモリO(N+M)。到達辺の0/1以外はassert。
///
/// 呼び分け: 単一始点の形式。一般の非負重みはdijkstra。
template <class Cost>
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

/// 無向グラフを「互いに道でつながっている頂点のまとまり」へ分けた結果です。各頂点の成分番号と、各成分に属する頂点一覧を相互に参照できます。
struct ConnectedComponents {
    /// `id[v]` = 成分番号。
    vi id;
    /// `groups[i]` = 成分 i の頂点列。
    vector<vi> groups;

    /// 連結成分の個数を返す。
    int count() const { return static_cast<int>(groups.size()); }

    /// 頂点 `a` と `b` が同じ連結成分か判定する。
    bool same(int a, int b) const { return id[a] == id[b]; }
};

// @acf fn connected_components — (Graph) -> ConnectedComponents — 無向の連結成分

/// 無向グラフの連結成分を列挙する。
///
/// 引数 `graph`: 無向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: ConnectedComponents。id[v]は0-index成分番号、groupsは成分内の頂点列。0頂点はcount()=0。
///
/// 注意: 入力を変更せず探索。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: 無向であることは自動検査しません。有向グラフでは強連結成分にstrongly_connected_componentsを使います。
template <class Cost>
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

// @acf fn bipartite_coloring — (Graph) -> optional<vi> — 2色塗り。不可なら nullopt

/// 無向グラフを二部グラフとして2色に塗る。
///
/// 引数 `graph`: 無向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: `optional<vi>`。可能なら長さNの0/1色列、各成分の探索根は色0。奇閉路/自己ループがあればnullopt。空グラフは値を持つ空列。
///
/// 注意: 入力を変更せずBFS。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: 無向グラフを前提とし、有向性を自動で拒否する形式ではありません。
template <class Cost>
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

// DAGなら順序、閉路があれば nullopt。有向グラフ向け。
// @acf fn topological_sort — (Graph) -> optional<vi> — 有向のトポロジカル順。閉路なら nullopt

/// 有向DAGの頂点を、各辺の始点が終点より先になる順に並べる。
///
/// 引数 `graph`: 有向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: `optional<vi>`。DAGなら全N頂点の順序、閉路ありはnullopt。空の有向グラフは値を持つ空列。辞書順最小は保証しません。
///
/// 注意: 入力を変更せず入次数とFIFO queueでKahn法。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: 有向グラフを渡します。有向性そのものを自動検査する形式ではありません。
template <class Cost>
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
// @acf fn grundy_dag — (Graph) -> optional<vi> — 有向DAGの各頂点のGrundy数。閉路/無向ならnullopt

/// 有向DAGで、辺を移動として各頂点のGrundy数を求める。
///
/// 引数 `graph`: 有向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: `optional<vi>`。各頂点は後続のGrundy数のmex、終端は0。閉路あり、または無向グラフならnullopt。空DAGは値を持つ空列。
///
/// 注意: 入力を変更せず逆トポロジカル順で集合のmexを計算。辺の重みは使いません。
///
/// 呼び分け: 閉路のあるゲームのGrundy数を扱う形式ではありません。
template <class Cost>
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

/// 根を決めた木の親・根からの深さ・部分木の大きさ・走査順を表す前計算結果です。order を親から子へ、逆順を子から親へ処理して木DPに使います。
struct TreeInfo {
    /// 親。根は `-1`。
    vi parent;
    /// 親辺ID。
    vi parent_edge;
    /// 根からの深さ。
    vi depth;
    /// 親が子より先の走査順。
    vi order;         // 親が子より先
    /// 部分木サイズ（自分含む）。
    vi subtree_size;
};

// @acf fn tree_info — (tree[, root]) -> TreeInfo — 親・深さ・部分木サイズ・順序

/// 根付き木の親・深さ・部分木サイズ・探索順を求める。
///
/// 引数 `tree`: N>=1の無向・連結・閉路なしの木。
///
/// 引数 `root`: 0..N-1の根。省略時は `0`。
///
/// 戻り値: TreeInfo。parent[root]=-1、depth[root]=0、subtree_size[root]=N。orderは親が子より先。木の重みは深さに使いません。
///
/// 注意: 入力を変更せず反復DFS。時間/追加メモリO(N)。根の範囲はassertしますが、木かどうかは検査しません。
///
/// 呼び分け: 一般グラフの探索木はdfs。tree_infoへ渡す木の条件は必要ならis_treeで確認します。
template <class Cost>
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

/// 負の辺を含むグラフの、単一始点からの最短路計算結果です。到達不能と、到達できても負閉路の影響で最短距離が定まらない頂点を区別します。
template <class Cost>
struct BellmanFordResult {
    /// 各頂点への距離。到達不能は inf、negative[v] なら確定距離として使わない。
    vector<Cost> dist;
    /// 最短路の親頂点（0-indexed）。到達不能は -1。
    vi parent;
    /// 親から来た辺の ID。到達不能は -1。
    vi parent_edge;
    /// 始点から到達できる負閉路の影響で距離が定まらない頂点の印。
    vb negative;  // 始点から到達でき、負閉路の影響を受ける
    /// 到達不能の距離番兵。計算に渡した`inf`と同じ。
    Cost inf{};

    /// 頂点 `v` が始点から到達可能か判定する。
    bool reachable(int v) const { return dist[v] != inf; }

    /// 到達可能で、負閉路の影響を受けない距離か判定する。
    bool well_defined(int v) const { return reachable(v) && !negative[v]; }

    /// `target` までの経路を復元する。不定または到達不能なら空を返す。
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
// @acf fn bellman_ford — (Graph, s[, inf]) -> BellmanFordResult — 負辺可の単一始点最短路

/// 負の辺重みを許し、始点からの距離と負閉路の影響を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `source`: 0..N-1の始点。N>=1。
///
/// 引数 `inf`: 到達不能を表す正の値。全ての有限距離の絶対値より十分大きく取る。省略時は `numeric_limits<Cost>::max() / Cost{4}`。
///
/// 戻り値: `BellmanFordResult<Cost>`。未到達はdist=inf。到達可能な負閉路から辿れる頂点はnegative=true、符号付きCostではdist=-inf。経路はwell_definedを確認して使います。
///
/// 注意: 入力を変更せず辺の緩和と負閉路影響の伝播。時間O(NM)、追加メモリO(N)。64bit以下の整数はi128で計算して±infへ丸めるため、有限距離を十分表せるinfが必要です。
///
/// 呼び分け: 非負辺だけならdijkstra。負閉路の影響がある頂点は有限の最短路を持ちません。
template <class Cost>
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

/// すべての始点・終点の組について、辺重みの和で測った距離を保持する結果です。何組もの距離問い合わせと、グラフ内の負閉路の検出に使います。
template <class Cost>
struct AllPairsShortestPath {
    /// `dist[from][to]`は始点`from`から終点`to`の辺重み和。到達不能は`inf`。
    vector<vector<Cost>> dist;
    /// 到達不能の距離番兵。
    Cost inf{};

    /// `from` から `to` へ到達可能か判定する。
    bool reachable(int from, int to) const { return dist[from][to] != inf; }

    /// 距離行列が負閉路を含むか判定する。
    bool has_negative_cycle() const {
        for (int v = 0; v < static_cast<int>(dist.size()); ++v)
            if (dist[v][v] < Cost{}) return true;
        return false;
    }
};

// @acf fn floyd_warshall — (Graph[, inf]) -> AllPairsShortestPath — 全点対最短路

/// 全ての頂点対の最短距離を求める。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `inf`: 到達不能を表す正の値。全ての有限距離の絶対値より十分大きく取る。省略時は `numeric_limits<Cost>::max() / Cost{4}`。
///
/// 戻り値: `AllPairsShortestPath<Cost>`。dist[u][v]の未到達はinf、対角の初期値は0。平行辺は最小重み。負閉路はhas_negative_cycle()で検出し、その影響を受ける距離を有限最短距離として使いません。空は空行列。
///
/// 注意: 入力を変更せず距離行列を更新。時間O(N^3)、追加メモリO(N^2)。64bit以下整数の加算は±infで飽和。有限距離を表せるinfを選びます。
///
/// 呼び分け: 負閉路の影響を頂点ごとに識別したい単一始点はbellman_ford。
template <class Cost>
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

/// 有向グラフを「どの2頂点の間も互いに到達できるまとまり」へ分けた結果です。閉路のまとまりを1頂点へ縮約してDAGとして扱うときに使います。
struct StronglyConnectedComponents {
    /// 成分番号。
    vi id;
    /// 成分ごとの頂点列（トポ順）。
    vector<vi> groups;  // トポロジカル順

    /// 強連結成分の個数を返す。
    int count() const { return static_cast<int>(groups.size()); }

    /// 頂点 `a` と `b` が同じ強連結成分か判定する。
    bool same(int a, int b) const { return id[a] == id[b]; }
};

// Kosaraju 法。再帰を使わず O(V+E)。
// @acf fn strongly_connected_components — (Graph) -> SCC — 有向の強連結成分

/// 有向グラフを互いに到達できる強連結成分に分ける。
///
/// 引数 `graph`: 有向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: StronglyConnectedComponents。id[v]は成分番号、groupsは縮約DAGのトポロジカル順。異なる成分へ向かう辺は小さいIDから大きいID。0頂点はcount()=0。
///
/// 注意: 入力を変更せず反復Kosaraju法と逆グラフ。時間/追加メモリO(N+M)。
///
/// 呼び分け: 無向の連結成分はconnected_components。縮約グラフはcondensation_graph。
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

// @acf fn condensation_graph — (Graph, scc) -> Graph — SCC を頂点にした DAG

/// 強連結成分を頂点とした縮約DAGを作る。
///
/// 引数 `graph`: 有向グラフ。内部頂点番号は0..N-1。
///
/// 引数 `scc`: 同じgraphから得た整合するSCC分解。
///
/// 戻り値: 有向`Graph<ll>`。成分内辺を除き、成分間の同じ向きの辺を1本へ統合。重みは1、IDは新規連番。元の重み/IDは保持しません。0成分は0頂点DAG。
///
/// 注意: 入力を変更せず成分対をsetで重複除去。SCCを自動再計算せず、分解の整合性は呼び出し側が保証します。
///
/// 呼び分け: condensation_graphは上の引数順で呼び出します。
template <class Cost>
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

/// 無向グラフの、取り除くと連結性が変わる辺（橋）と頂点（関節点）を保持する結果です。どこを切るとグラフが分断されるかを調べるときに使います。
struct LowLinkResult {
    /// order[v] は頂点 v のDFS訪問順。
    vi order;
    /// low[v] はDFS木の子孫と後退辺から到達できる最小の訪問順。
    vi low;
    /// 取り除くと成分数が増える橋の辺 ID（昇順）。
    vi bridges;              // 橋の edge.id
    /// 取り除くと成分数が増える関節点の頂点番号（昇順）。
    vi articulation_points;  // 関節点の頂点番号
};

// 無向グラフの橋・関節点。辺IDで親辺だけを除外するため多重辺にも対応。
// @acf fn low_link — (Graph) -> LowLinkResult — 無向の橋・関節点

/// 無向グラフの橋と関節点を求める。
///
/// 引数 `graph`: 無向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: LowLinkResult。bridgesは昇順の辺ID、articulation_pointsは昇順の頂点番号。order/lowも保持。多重辺と自己ループに対応し、空は空配列。
///
/// 注意: 入力を変更せず反復DFS。探索O(N+M)、橋の整列O(B log B)。有向入力はassert。
///
/// 呼び分け: 各入力辺のIDが一意であることが必要です。Graph.add_edgeの既定連番なら満たします。
template <class Cost>
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

/// 無向グラフの各連結成分を最小コストでつなぐ森と、その重み和を保持する結果です。連結なグラフなら最小全域木になります。
template <class Cost>
struct MinimumSpanningForest {
    /// 採用辺のコスト和。
    Cost cost{};
    /// 採用した辺のID。
    vi edge_ids;
    /// 森の連結成分数。
    int components = 0;

    /// グラフ全体が1本の全域木になったか判定する。
    bool is_spanning_tree() const { return components == 1; }
};

// @acf fn kruskal — (Graph) -> MinimumSpanningForest — 無向の最小全域森

/// 無向グラフの最小全域森を求める。
///
/// 引数 `graph`: 無向グラフ。内部頂点番号は0..N-1。
///
/// 戻り値: `MinimumSpanningForest<Cost>`。costは採用辺の重み和、edge_idsは採用順、componentsは残る連結成分数。非連結でも森を返し、空はcost=0/components=0。
///
/// 注意: 入力を変更せずstable_sortとDSU。時間O(M log M)、追加メモリO(N+M)。負重み可、自己ループは採用せず、同重みは入力順。64ビット以下の整数型はi128で集計し、最終的な重み和がCostの範囲外ならoverflow_errorを送出する。それ以外の型は集計途中もCostに収まること。有向入力はassert。
///
/// 呼び分け: 全域木が得られたかはis_spanning_tree()を確認します。
template <class Cost>
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

/// 根付き木を前処理し、共通祖先と木上の距離を繰り返し求める。
///
/// 引数 `tree`: N>=1の無向・連結・閉路なしの木。
///
/// 引数 `root`: 0..N-1の根。省略時は `0`。
///
/// 戻り値: `LowestCommonAncestor<Cost>`オブジェクト。query(a,b)はLCA、kth_ancestor(v,k)はk個上（不在は-1）、distance_edges/distance_costは辺数/重み和。空木は不可。
///
/// 注意: 木を変更せず表を構築。時間/メモリO(N log N)、各クエリO(log N)。is_tree条件はassert。重み和と距離式の中間値がCostに収まること。
///
/// 呼び分け: 既定構築は未構築状態でクエリ不可。クエリ頂点は0..N-1、kは非負。型とメンバの詳細は既存のLowestCommonAncestor型ページ。
template <class Cost = ll>
struct LowestCommonAncestor {
    /// up[k][v] は 2^k 個上の祖先頂点。存在しなければ -1。
    vector<vi> up;
    /// 根からの辺数（頂点番号を添字にする）。
    vi depth;
    /// 根からの辺重み和（頂点番号を添字にする）。
    vector<Cost> root_distance;

    /// 未構築のLCAオブジェクトを作る。
    LowestCommonAncestor() = default;

    /// 木を前処理し、祖先・距離クエリを準備する。
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

    /// 頂点 `v` の `steps` 個上の祖先を返す。存在しなければ `-1`。
    int kth_ancestor(int v, int steps) const {
        assert(0 <= v && v < static_cast<int>(depth.size()) && steps >= 0);
        for (int k = 0; k < static_cast<int>(up.size()) && v != -1; ++k)
            if ((steps >> k) & 1) v = up[k][v];
        if ((static_cast<unsigned int>(steps) >> up.size()) != 0) return -1;
        return v;
    }

    /// 頂点 `a` と `b` の最小共通祖先を返す。
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

    /// `a` と `b` の木上の辺数距離を返す。
    int distance_edges(int a, int b) const {
        int ancestor = query(a, b);
        return depth[a] + depth[b] - 2 * depth[ancestor];
    }

    /// `a` と `b` の木上の辺重み和を返す。
    Cost distance_cost(int a, int b) const {
        int ancestor = query(a, b);
        return root_distance[a] + root_distance[b] - Cost{2} * root_distance[ancestor];
    }
};

/// 木の中で距離が最も大きい2頂点と、その間の経路を保持する結果です。木の直径の長さだけでなく、その頂点列も取り出したいときに使います。
template <class Cost>
struct TreeDiameterResult {
    /// 直径経路の始点（0-indexed）。未計算の既定状態は -1。
    int from = -1;
    /// 直径経路の終点（0-indexed）。未計算の既定状態は -1。
    int to = -1;
    /// `from`から`to`の辺重み和。
    Cost distance{};
    /// `from`から`to`の頂点列。両端を含む。
    vi path;
};

// 非負重み木の直径。未重み木では各辺 cost=1。
// @acf fn tree_diameter — (tree) -> TreeDiameterResult — 非負重み木の直径

/// 非負重みの無向木で、最長の単純パスを求める。
///
/// 引数 `tree`: N>=1の無向木。全辺重みが非負、経路和がCostに収まること。
///
/// 戻り値: `TreeDiameterResult<Cost>`。distanceは重み和、from/toは両端、pathは両端を含む頂点列。1頂点は距離0、path={0}。最長パスが複数なら特定の選択順は契約にしません。
///
/// 注意: 入力を変更せず木を2回走査。時間/追加メモリO(N)。木でない入力と負重みはassert。
///
/// 呼び分け: 一般グラフの最長単純路を求める形式ではありません。
template <class Cost>
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

/// グラフ内の1つの閉路を、周回する頂点列と各移動に使う辺 ID で表す値です。閉路があることの証拠や、その辺を取り出すときに使います。
struct GraphCycle {
    /// 閉路上の頂点（周回順）。
    vi vertices;
    /// 各ステップの辺ID。
    vi edge_ids;
};

// 有向・無向のどちらでも、最初に見つけた閉路を辺ID付きで返す。
// vertices[i] --edge_ids[i]--> vertices[(i+1)%k] の対応。
// @acf fn find_cycle — (Graph) -> optional<GraphCycle> — 閉路を1つ復元。なければ nullopt

/// 有向または無向グラフから閉路を1つ復元する。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 戻り値: `optional<GraphCycle>`。閉路ありはverticesとedge_ids、なければnullopt。頂点列は開始点を末尾へ繰り返さず、辺列は最後から最初へ戻る辺を含みます。空グラフはnullopt。
///
/// 注意: 入力を変更せず反復DFS。時間O(N+M)、追加メモリO(N)。
///
/// 呼び分け: 無向版は入力辺ごとの一意なIDが必要です。多重辺2本による閉路と自己ループも検出します。
template <class Cost>
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

/// グラフの全辺をちょうど1回ずつ通る道を、通る頂点と辺 ID の順序で表す値です。一筆書きの具体的な経路を出力するときに使います。
struct EulerTrail {
    /// 通る頂点列（辺数+1）。
    vi vertices;  // 辺数+1 個（辺がなく頂点もなければ空）
    /// 通る辺IDの順。
    vi edge_ids;
};

// 全辺をちょうど1回通る路。存在しなければ nullopt。
// start=-1 は次数条件から始点を自動選択する。
// @acf fn euler_trail — (Graph[, start]) -> optional<EulerTrail> — オイラー路。不可なら nullopt

/// 全ての辺をちょうど1回ずつ通る有向/無向のオイラー路を復元する。
///
/// 引数 `graph`: 内部頂点番号が0..N-1のグラフ。
///
/// 引数 `start`: 開始頂点0..N-1、または-1で自動選択。省略時は `-1`。
///
/// 戻り値: `optional<EulerTrail>`。成立ならvertices（M+1個）とedge_ids（M個）、次数条件/辺の接続/開始点が不適合ならnullopt。N>0,M=0は1頂点だけ。N=0で自動開始は値を持つ空列。
///
/// 注意: 元の辺は削除せず使用状態のコピーでHierholzer法。ハッシュ操作を含む期待時間O(N+M)、追加メモリO(N+M)。各入力辺IDは一意（assert）。
///
/// 呼び分け: 孤立頂点はあっても可。開始点自動は奇数次数/出入次数差を満たす頂点を優先し、全辺を通れない場合はnullopt。
template <class Cost>
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

/// 各頂点から次の頂点が1つだけ決まるグラフを、周期部分とそこへ流れ込む部分に分解した結果です。どの周期へ何回の遷移で入るかをまとめて調べるときに使います。
struct FunctionalGraphInfo {
    /// 属するサイクル番号。
    vi cycle_id;
    /// サイクルまでの距離（上は0）。
    vi distance_to_cycle;
    /// 初めて乗るサイクル頂点。
    vi cycle_entry;
    /// 各サイクルの頂点列。
    vector<vi> cycles;
};

// @acf fn functional_graph_info — (next) -> FunctionalGraphInfo — 出次数1のサイクル構造

/// 出次数1のグラフを、サイクルとそこへ入る経路に分解する。
///
/// 引数 `next`: next[v]が0..N-1の遷移先。各頂点から必ず1本出る。
///
/// 戻り値: FunctionalGraphInfo。cycle_idは到達するサイクル番号、distance_to_cycleは入るまでの遷移数、cycle_entryは最初のサイクル頂点、cyclesは遷移順のサイクル列。空入力は全て空。
///
/// 注意: 入力を変更せず入次数0を除去後に逆伝播。時間/追加メモリO(N)。遷移先の範囲はassert。
///
/// 呼び分け: 1始点だけの周期分解はfunctional_graph_orbit。
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

/// 1つの始点からの遷移列を、最初の一度だけ通る部分と繰り返す周期部分に分けた結果です。大きい回数の遷移先を、周期の余りで求めるときに使います。
struct FunctionalGraphOrbit {
    /// 再訪する頂点を除いた始点からの頂点列。
    vi route;
    /// `route` のサイクル開始位置。始点からサイクルまでの距離。
    int cycle_begin = 0;
    /// サイクルの長さ。
    int cycle_length = 0;

    /// `start` から `steps` 回遷移した頂点を返す。
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

/// 1つの始点からの遷移を、前半の非周期部分と周期に分ける。
///
/// 引数 `next`: next[v]が0..N-1の遷移先。各頂点から必ず1本出る。
///
/// 引数 `start`: N>=1、0..N-1の始点。
///
/// 戻り値: FunctionalGraphOrbit。routeは最初の重複直前までの頂点列、cycle_beginは周期開始の配列位置、cycle_lengthは正の周期長。at(k)はk回後の頂点でk=0は始点。
///
/// 注意: 入力を変更せず初訪問位置を記録。前処理の時間/追加メモリO(N)、at(k)はO(1)。空入力と範囲外遷移はassert。
///
/// 呼び分け: 全頂点の構造はfunctional_graph_info、複数始点からの遷移クエリはFunctionalGraphDoubling。
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

/// 出次数1のグラフを前処理し、任意始点からK回後の頂点を求める。
///
/// 引数 `next`: next[v]が0..N-1の遷移先。各頂点から必ず1本出る。
///
/// 引数 `bits`: 前処理する段数1..64。省略時は `60`。
///
/// 戻り値: FunctionalGraphDoublingオブジェクト。jump(v,steps)はsteps回後の頂点。bits<64ではsteps<2^bitsが必要、64ならull全域。空のnextの表は作れますが、有効なクエリ頂点はありません。
///
/// 注意: nextを変更せずbits*Nの表を構築。時間/メモリO(bits*N)、jumpはO(bits)。bits・遷移先・クエリ範囲はassert。
///
/// 呼び分け: 既定構築は未構築でjump不可。1始点の周期だけならfunctional_graph_orbit。
struct FunctionalGraphDoubling {
    vector<vi> jump_table;

    /// 未構築のダブリング表を作る。
    FunctionalGraphDoubling() = default;

    /// `next[v]` を `bits` 段前処理し、最大 `2^bits-1` 回の遷移に備える。
    explicit FunctionalGraphDoubling(const vi& next, int bits = 60) {
        assert(1 <= bits && bits <= 64);
        jump_table.assign(bits, vi(next.size()));
        jump_table[0] = next;
        for (int v : next) assert(0 <= v && v < static_cast<int>(next.size()));
        for (int k = 1; k < bits; ++k)
            for (int v = 0; v < static_cast<int>(next.size()); ++v)
                jump_table[k][v] = jump_table[k - 1][jump_table[k - 1][v]];
    }

    /// 頂点 `v` から `steps` 回遷移した頂点を返す。
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
