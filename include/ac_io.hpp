#pragma once
// Library: https://github.com/facedolid/atcoder-headers

// 宣言しながら読むAPIと、既存変数・コンテナへ読むAPIの互換レイヤー。
// ac_io.hpp を明示的にincludeする。公開短縮名は頻出のものだけに限定。
#include "ac.hpp"

namespace ac_detail::input {
template <class Assoc>
Assoc read_assoc(int n) {
    Assoc a;
    rep(_, n) {
        using K = typename Assoc::key_type;
        using V = typename Assoc::mapped_type;
        K k = rd<K>();
        V v = rd<V>();
        a.emplace(std::move(k), std::move(v));
    }
    return a;
}

template <class UniqAssoc>
UniqAssoc read_assoc_unique(int n) {
    UniqAssoc a;
    rep(_, n) {
        using K = typename UniqAssoc::key_type;
        using V = typename UniqAssoc::mapped_type;
        K k = rd<K>();
        V v = rd<V>();
        a[k] = v;
    }
    return a;
}
}  // namespace ac_detail::input

// @acf fn in_vector — (int n) -> vector<T> — vector<T>(n) を読んで返す

/// 指定した型・個数の配列を標準入力から作る。
///
/// 引数 `n`: 非負の入力要素数。
///
/// 引数 `base`: 第2引数形式で先頭に確保する空き要素数。base<=0なら通常形式と同じ。
///
/// 戻り値: `vector<T>`。1引数は長さn。base>0は長さn+baseで、先頭base要素は値初期化し、残りn要素へ入力します。n=0でもbaseの空き要素は確保されます。
///
/// 注意: 標準入力を消費します。入力失敗をbool等で返すAPIではありません。
///
/// 呼び分け: `in_vector<T>`(n)／`in_vector<T>`(n,base)。Tは要素型として明示します。n+baseがintに収まり確保できるサイズで使います。
template <class T>
vector<T> in_vector(int n) {
    vector<T> v(n);
    ac_detail::input::extract(cin, v);
    return v;
}

/// 指定した型・個数の配列を標準入力から作る。
///
/// 引数 `n`: 非負の入力要素数。
///
/// 引数 `base`: 第2引数形式で先頭に確保する空き要素数。base<=0なら通常形式と同じ。
///
/// 戻り値: `vector<T>`。1引数は長さn。base>0は長さn+baseで、先頭base要素は値初期化し、残りn要素へ入力します。n=0でもbaseの空き要素は確保されます。
///
/// 注意: 標準入力を消費します。入力失敗をbool等で返すAPIではありません。
///
/// 呼び分け: `in_vector<T>`(n)／`in_vector<T>`(n,base)。Tは要素型として明示します。n+baseがintに収まり確保できるサイズで使います。
template <class T>
vector<T> in_vector(int n, int base) {
    if (base <= 0) return in_vector<T>(n);
    vector<T> v(n + base);
    for (int i = base; i < n + base; i++) {
        if constexpr (std::is_same_v<T, bool>) {
            bool b;
            ac_detail::input::extract(cin, b);
            v[i] = b;
        } else {
            ac_detail::input::extract(cin, v[i]);
        }
    }
    return v;
}

// @acf fn in_rows — (int n) -> vector<tuple<Ts...>> — n 行のタプル列を読む（1行あたり Ts...）

/// 標準入力から型列ごとの組を読み、`vector<tuple<Ts...>>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `vector<tuple<Ts...>>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。 Ts...の各要素を読むため物理的な行末は要求しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。
template <class... Ts>
vector<tuple<Ts...>> in_rows(int n) {
    return in_vector<tuple<Ts...>>(n);
}

// @acf fn in_mat — (int n, int m) -> vector<vector<T>> — 2次元 vector を読んで返す

/// 標準入力から指定サイズの要素を読み、`vector<vector<T>>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 引数 `m`: 非負の列数。
///
/// 引数 `base（追加形式のみ）`: 正なら各軸の先頭にbase個の未入力領域を置く。サイズの和はintに収まること。省略時は `指定なし（base<=0も0始まり）`。
///
/// 戻り値: `vector<vector<T>>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。 base版は各軸の未入力部分を値初期化し、行・列どちらもbaseから読みます。
template <class T>
vector<vector<T>> in_mat(int n, int m) {
    vector<vector<T>> v(n, vector<T>(m));
    ac_detail::input::extract(cin, v);
    return v;
}

/// 標準入力から指定サイズの要素を読み、`vector<vector<T>>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 引数 `m`: 非負の列数。
///
/// 引数 `base（追加形式のみ）`: 正なら各軸の先頭にbase個の未入力領域を置く。サイズの和はintに収まること。省略時は `指定なし（base<=0も0始まり）`。
///
/// 戻り値: `vector<vector<T>>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。 base版は各軸の未入力部分を値初期化し、行・列どちらもbaseから読みます。
template <class T>
vector<vector<T>> in_mat(int n, int m, int base) {
    if (base <= 0) return in_mat<T>(n, m);
    vector<vector<T>> v(n + base, vector<T>(m + base));
    for (int i = base; i < n + base; i++) {
        for (int j = base; j < m + base; j++) {
            if constexpr (std::is_same_v<T, bool>) {
                bool b;
                ac_detail::input::extract(cin, b);
                v[i][j] = b;
            } else {
                ac_detail::input::extract(cin, v[i][j]);
            }
        }
    }
    return v;
}

// @acf fn in_mat3 — (int n, int m, int k) -> vector<vector<vector<T>>> — 3次元 vector を読んで返す

/// 標準入力から指定サイズの要素を読み、`vector<vector<vector<T>>>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 引数 `m`: 非負の列数。
///
/// 引数 `k`: 非負の第3軸の長さ。
///
/// 引数 `base（追加形式のみ）`: 正なら各軸の先頭にbase個の未入力領域を置く。サイズの和はintに収まること。省略時は `指定なし（base<=0も0始まり）`。
///
/// 戻り値: `vector<vector<vector<T>>>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。 base版は各軸の未入力部分を値初期化し、行・列どちらもbaseから読みます。
template <class T>
vector<vector<vector<T>>> in_mat3(int n, int m, int k) {
    vector<vector<vector<T>>> v(n, vector<vector<T>>(m, vector<T>(k)));
    ac_detail::input::extract(cin, v);
    return v;
}

/// 標準入力から指定サイズの要素を読み、`vector<vector<vector<T>>>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 引数 `m`: 非負の列数。
///
/// 引数 `k`: 非負の第3軸の長さ。
///
/// 引数 `base（追加形式のみ）`: 正なら各軸の先頭にbase個の未入力領域を置く。サイズの和はintに収まること。省略時は `指定なし（base<=0も0始まり）`。
///
/// 戻り値: `vector<vector<vector<T>>>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。 base版は各軸の未入力部分を値初期化し、行・列どちらもbaseから読みます。
template <class T>
vector<vector<vector<T>>> in_mat3(int n, int m, int k, int base) {
    if (base <= 0) return in_mat3<T>(n, m, k);
    vector<vector<vector<T>>> v(n + base, vector<vector<T>>(m + base, vector<T>(k + base)));
    for (int i = base; i < n + base; i++) {
        for (int j = base; j < m + base; j++) {
            for (int p = base; p < k + base; p++) {
                if constexpr (std::is_same_v<T, bool>) {
                    bool b = false;
                    ac_detail::input::extract(cin, b);
                    v[i][j][p] = b;
                } else {
                    ac_detail::input::extract(cin, v[i][j][p]);
                }
            }
        }
    }
    return v;
}

// @acf fn in_deque — (int n) -> deque<T> — n 要素の deque を読んで返す

/// 標準入力から指定サイズの要素を読み、`deque<T>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `deque<T>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。
template <class T>
deque<T> in_deque(int n) {
    deque<T> d(n);
    ac_detail::input::extract(cin, d);
    return d;
}

// @acf fn in_list_seq — (int n) -> list<T> — n 要素の list を読んで返す

/// 標準入力から指定サイズの要素を読み、`list<T>`を返す。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `list<T>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...はテンプレート引数。
template <class T>
list<T> in_list_seq(int n) {
    list<T> l(n);
    ac_detail::input::extract(cin, l);
    return l;
}

// @acf fn in_array — () -> array<T,N> — 固定長 array を読んで返す

/// 標準入力から指定サイズの要素を読み、`array<T,N>`を返す。
///
/// 戻り値: `array<T,N>`。順序は入力順、行列は軸の内側から埋めます。要素数0なら入力を消費しません。
///
/// 注意: cinを消費して新しいコンテナを確保。読み取り失敗のstatusは返さずcinの状態で確認します。必要な値を全て読み取れる入力を前提とします。要素数に比例した処理（各値の入力費用は別）。
///
/// 呼び分け: 型TまたはTs...と要素数Nはテンプレート引数。
template <class T, size_t N>
array<T, N> in_array() {
    array<T, N> a;
    ac_detail::input::extract(cin, a);
    return a;
}

// @acf fn in_bs — () -> bitset<N> — 01 文字列を bitset として読んで返す

/// 標準入力の01文字列をビット集合として読む。
///
/// 戻り値: `bitset<N>`。Nはテンプレート引数。標準のbitset抽出と同じで、文字列の右端が下位bit。
///
/// 注意: cinを消費。全項が有効で型の範囲に収まる入力が前提。失敗を表す別の戻り値はありません。
///
/// 呼び分け: in_bsは上の引数順で呼び出します。
template <size_t N>
bitset<N> in_bs() {
    bitset<N> b;
    ac_detail::input::extract(cin, b);
    return b;
}

// @acf fn in_bs32u — () -> bs32 — ui を読んで bs32 にする

/// 標準入力の10進の符号なし整数をビット集合として読む。
///
/// 戻り値: `bs32`。読んだ整数の各bitを保持。01文字列を読む形式とは異なります。
///
/// 注意: cinを消費。全項が有効で型の範囲に収まる入力が前提。失敗を表す別の戻り値はありません。
///
/// 呼び分け: in_bs32uは上の引数順で呼び出します。
inline bs32 in_bs32u() { return bs32(rd<ui>()); }

// @acf fn in_bs64ul — () -> bs64 — ull を読んで bs64 にする

/// 標準入力の10進の符号なし整数をビット集合として読む。
///
/// 戻り値: `bs64`。読んだ整数の各bitを保持。01文字列を読む形式とは異なります。
///
/// 注意: cinを消費。全項が有効で型の範囲に収まる入力が前提。失敗を表す別の戻り値はありません。
///
/// 呼び分け: in_bs64ulは上の引数順で呼び出します。
inline bs64 in_bs64ul() { return bs64(rd<ull>()); }

namespace ac_detail::input {
// BS32U / BS64UL の複数引数用（`in` は 01 文字列なので使わない）
template <class Int, class... Bs>
bool assign_bs_from_int(Bs&... xs) {
    ((xs = Bs(rd<Int>())), ...);
    return true;
}
}  // namespace ac_detail::input

// @acf fn in_set — (int n) -> set<T> — n 個読んで set に

/// 標準入力からn個を読んで`set<T>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `set<T>`。重複値は1個に統合。走査順はTの比較順。n=0は空。
///
/// 注意: cinをn個消費。比較またはhash/==が成立する型を使います。時間O(n log n)。読み取り失敗のstatusは返しません。
///
/// 呼び分け: in_setは上の引数順で呼び出します。
template <class T>
set<T> in_set(int n) {
    set<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_mset — (int n) -> multiset<T> — 重複を保って n 個読む

/// 標準入力からn個を読んで`multiset<T>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `multiset<T>`。同じ値も全て保持。走査順はTの比較順。n=0は空。
///
/// 注意: cinをn個消費。比較またはhash/==が成立する型を使います。時間O(n log n)。読み取り失敗のstatusは返しません。
///
/// 呼び分け: in_msetは上の引数順で呼び出します。
template <class T>
multiset<T> in_mset(int n) {
    multiset<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_uset — (int n) -> unordered_set<T> — 順序なし集合へ n 個読む

/// 標準入力からn個を読んで`unordered_set<T>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `unordered_set<T>`。重複値は1個に統合。走査順は保証しません。n=0は空。
///
/// 注意: cinをn個消費。比較またはhash/==が成立する型を使います。期待時間O(n)、最悪はhashの衝突に依存。読み取り失敗のstatusは返しません。
///
/// 呼び分け: in_usetは上の引数順で呼び出します。
template <class T>
unordered_set<T> in_uset(int n) {
    unordered_set<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_umset — (int n) -> unordered_multiset<T> — 順序なしで重複を保って n 個読む

/// 標準入力からn個を読んで`unordered_multiset<T>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `unordered_multiset<T>`。同じ値も全て保持。走査順は保証しません。n=0は空。
///
/// 注意: cinをn個消費。比較またはhash/==が成立する型を使います。期待時間O(n)、最悪はhashの衝突に依存。読み取り失敗のstatusは返しません。
///
/// 呼び分け: in_umsetは上の引数順で呼び出します。
template <class T>
unordered_multiset<T> in_umset(int n) {
    unordered_multiset<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_map — (int n) -> map<K,V> — n 組の key,value を map に

/// 標準入力からキーと値のn組を読み、`map<K,V>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `map<K,V>`。同じキーは後から読んだ値で上書き。キーの比較順に走査。n=0は空。
///
/// 注意: cinをキー・値の順に消費。時間O(n log n)。入力失敗のstatusは返しません。キー比較/hash、非multi版はVの既定構築も必要です。
///
/// 呼び分け: in_mapは上の引数順で呼び出します。
template <class K, class V>
map<K, V> in_map(int n) {
    return ac_detail::input::read_assoc_unique<map<K, V>>(n);
}

// @acf fn in_mmap — (int n) -> multimap<K,V> — 同じキーを保って n 組読む

/// 標準入力からキーと値のn組を読み、`multimap<K,V>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `multimap<K,V>`。同じキーの各組を保持。キーの比較順に走査。n=0は空。
///
/// 注意: cinをキー・値の順に消費。時間O(n log n)。入力失敗のstatusは返しません。キー比較/hash、非multi版はVの既定構築も必要です。
///
/// 呼び分け: in_mmapは上の引数順で呼び出します。
template <class K, class V>
multimap<K, V> in_mmap(int n) {
    return ac_detail::input::read_assoc<multimap<K, V>>(n);
}

// @acf fn in_umap — (int n) -> unordered_map<K,V> — 順序なし map へ n 組読む

/// 標準入力からキーと値のn組を読み、`unordered_map<K,V>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `unordered_map<K,V>`。同じキーは後から読んだ値で上書き。走査順は保証しません。n=0は空。
///
/// 注意: cinをキー・値の順に消費。期待時間O(n)、最悪はhashの衝突に依存。入力失敗のstatusは返しません。キー比較/hash、非multi版はVの既定構築も必要です。
///
/// 呼び分け: in_umapは上の引数順で呼び出します。
template <class K, class V>
unordered_map<K, V> in_umap(int n) {
    return ac_detail::input::read_assoc_unique<unordered_map<K, V>>(n);
}

// @acf fn in_ummap — (int n) -> unordered_multimap<K,V> — 順序なしで同じキーを保って n 組読む

/// 標準入力からキーと値のn組を読み、`unordered_multimap<K,V>`へ入れる。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `unordered_multimap<K,V>`。同じキーの各組を保持。走査順は保証しません。n=0は空。
///
/// 注意: cinをキー・値の順に消費。期待時間O(n)、最悪はhashの衝突に依存。入力失敗のstatusは返しません。キー比較/hash、非multi版はVの既定構築も必要です。
///
/// 呼び分け: in_ummapは上の引数順で呼び出します。
template <class K, class V>
unordered_multimap<K, V> in_ummap(int n) {
    return ac_detail::input::read_assoc<unordered_multimap<K, V>>(n);
}

// @acf fn in_queue — (int n) -> queue<T> — 入力順に n 個を queue へ積む

/// 標準入力からn個を読んで`queue<T>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `queue<T>`。frontは最初に読んだ値。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_queueは上の引数順で呼び出します。
template <class T>
queue<T> in_queue(int n) {
    queue<T> q;
    rep(_, n) q.push(rd<T>());
    return q;
}

// @acf fn in_stack — (int n) -> stack<T> — 入力順に n 個を stack へ積む

/// 標準入力からn個を読んで`stack<T>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `stack<T>`。topは最後に読んだ値。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_stackは上の引数順で呼び出します。
template <class T>
stack<T> in_stack(int n) {
    stack<T> st;
    rep(_, n) st.push(rd<T>());
    return st;
}

// @acf fn in_pq — (int n) -> priority_queue<T,...> — n 個を優先度付きキューへ積む

/// 標準入力からn個を読んで`priority_queue<T,vector<T>,Comp>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `priority_queue<T,vector<T>,Comp>`。既定は最大値がtop、Compを指定すればその比較順。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_pqは上の引数順で呼び出します。
template <class T, class Comp>
priority_queue<T, vector<T>, Comp> in_pq(int n) {
    priority_queue<T, vector<T>, Comp> pq;
    rep(_, n) pq.push(rd<T>());
    return pq;
}

/// 標準入力からn個を読んで`priority_queue<T,vector<T>,Comp>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `priority_queue<T,vector<T>,Comp>`。既定は最大値がtop、Compを指定すればその比較順。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_pqは上の引数順で呼び出します。
template <class T>
priority_queue<T> in_pq(int n) {
    return in_pq<T, less<T>>(n);
}

// @acf fn in_setpq — (int n) -> setpq<T, Comp> — n 個を読み、重複をまとめる優先度付きキューを返す

/// 標準入力からn個を読み、比較上同値の値を1個にまとめた優先度付きキューを返す。
///
/// 引数 `n`: 読み取る値の個数。非負で、必要な全入力を正しく読み取れること。
///
/// 戻り値: `setpq<T, Comp>`。既定は最大値優先。`n=0`は空で、`top()`と`pop()`は呼べません。要素数は重複を除いた個数です。
///
/// 注意: 標準入力を重複分も含めてn個消費します。時間O(n log(n+1))、使用メモリO(n)。
///
/// 呼び分け: `in_setpq<T>(n)`は最大値優先、`in_setpq<T, greater<T>>(n)`は最小値優先です。同値判定は`set`と同じです。
///
/// 例: `auto q = in_setpq<int>(3); print(q);`
template <class T, class Comp = less<T>>
setpq<T, Comp> in_setpq(int n) {
    setpq<T, Comp> pq;
    rep(_, n) pq.push(rd<T>());
    return pq;
}

// @acf fn in_maxpq — (int n) -> maxpq<T> — n 個を最大ヒープへ積む

/// 標準入力からn個を読んで`maxpq<T>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `maxpq<T>`。最大値がtop。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_maxpqは上の引数順で呼び出します。
template <class T>
maxpq<T> in_maxpq(int n) {
    return in_pq<T, less<T>>(n);
}

// @acf fn in_minpq — (int n) -> minpq<T> — n 個を最小ヒープへ積む

/// 標準入力からn個を読んで`minpq<T>`へ積む。
///
/// 引数 `n`: 非負の読み取る要素/組数。必要な全入力を正しく読み取れること。
///
/// 戻り値: `minpq<T>`。最小値がtop。n=0は空でfront/topを呼べません。
///
/// 注意: cinをn個消費。queue/stackは時間O(n)、priority_queueはO(n log n)。新しいコンテナを返し失敗statusはありません。
///
/// 呼び分け: in_minpqは上の引数順で呼び出します。
template <class T>
minpq<T> in_minpq(int n) {
    return in_pq<T, greater<T>>(n);
}

// ---------------------------------------------------------------------------
// Optional typed readers — validate the type of existing variables
// I32 / LL / UI32 / ULL / DB / LD / STR / CHAR / BOOL
// 役割: 宣言は Int、既存へ読むのは I32、式は rd_int（3 層）。
// 名前: I32/UI32 だけ幅タグ。ほかは using 名に近い（LL/ULL/DB/STR）。
// ---------------------------------------------------------------------------

// 型付き in（常に true）。`between(op,0,3) && I32(i)` のように && チェーン可。
// 宣言つきは Int / Long など（下記）。型検査が不要なら in(x) を使う。
namespace ac_detail::input {
/// 指定した型の既存変数だけを標準入力から読む関数オブジェクトです。I32 / LL / STR などの実体で、変数の宣言と型を合わせて入力したいときに使います。
template <class Expected>
struct typed_in {
    /// 指定型の既存変数へ入力し、常に `true` を返す。
    template <class... T>
    bool operator()(T&... xs) const {
        static_assert((is_same_v<ac_detail::io::remove_cvref_t<T>, Expected> && ...),
                      "typed in: variable type mismatch");
        return in(xs...);
    }
};
}  // namespace ac_detail::input

// @acf fn I32 — (int&... a) -> bool — int へ読む。常に true（`cond && I32(i)` 可）

/// 既に宣言した int 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: I32(x)、I32(x,y,...)、I32()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<int> I32{};
// @acf fn LL — (ll&... a) -> bool — ll へ読む。常に true（`cond && LL(x)` 可）

/// 既に宣言した long long 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: LL(x)、LL(x,y,...)、LL()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<ll> LL{};
// @acf fn UI32 — (ui&... a) -> bool — ui へ読む。常に true

/// 既に宣言した unsigned int 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: UI32(x)、UI32(x,y,...)、UI32()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<ui> UI32{};
// @acf fn ULL — (ull&... a) -> bool — ull へ読む。常に true

/// 既に宣言した unsigned long long 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: ULL(x)、ULL(x,y,...)、ULL()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<ull> ULL{};
// @acf fn DB — (db&... a) -> bool — double へ読む。常に true

/// 既に宣言した double 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: DB(x)、DB(x,y,...)、DB()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<db> DB{};
// @acf fn LD — (ld&... a) -> bool — long double へ読む。常に true

/// 既に宣言した long double 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。型で表現できる数値を標準入力から読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: LD(x)、LD(x,y,...)、LD()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<ld> LD{};
// @acf fn STR — (string&... a) -> bool — string へ読む。常に true

/// 既に宣言した string 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。空白で区切られた文字列トークンを読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: STR(x)、STR(x,y,...)、STR()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<string> STR{};
// @acf fn CHAR — (char&... a) -> bool — char へ読む。常に true

/// 既に宣言した char 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。先行空白を飛ばし1文字を読む。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: CHAR(x)、CHAR(x,y,...)、CHAR()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<char> CHAR{};
// @acf fn BOOL — (bool&... a) -> bool — bool へ読む。常に true

/// 既に宣言した bool 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。既定の cin 設定では0/1を読む（boolalpha を変更した場合は別形式）。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。読み取り失敗でも返り値は成功判定にならないため、使う値の有効性は cin の状態で判断します。
///
/// 呼び分け: BOOL(x)、BOOL(x,y,...)、BOOL()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。
inline constexpr ac_detail::input::typed_in<bool> BOOL{};

#ifdef AC_HAS_ACL
// @acf fn MINT1000000007 — (mint1000000007&... a) -> bool — modint 1e9+7 へ読む

/// 既に宣言した 法1000000007のmodint 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。読み取る整数は ll に収まる正しい入力。法以上・負数は代入時に正規化。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。内部では未初期化の ll 一時値を読み、その値を modint へ代入します。正しい整数入力があることを前提に使い、EOF・不正入力後の値を保証しません。
///
/// 呼び分け: MINT1000000007(x)、MINT1000000007(x,y,...)、MINT1000000007()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。ACLヘッダーが include パスにあり、AC_HAS_ACL が有効なビルドで使えます。
inline constexpr ac_detail::input::typed_in<mint1000000007> MINT1000000007{};
// @acf fn MINT998244353 — (mint998244353&... a) -> bool — modint 998244353 へ読む

/// 既に宣言した 法998244353のmodint 型の変数へ標準入力の値を読み込む入力オブジェクトです。宣言を増やさず、渡す変数の型も確認したいときに使います。
///
/// 引数 `xs...`: 書き換え可能な既存変数を左から順に渡す。すべて同じ指定型で、型違いはコンパイル時に拒否。読み取る整数は ll に収まる正しい入力。法以上・負数は代入時に正規化。
///
/// 戻り値: この関数は正常終了時に常に`true`を返します。入力の成功を示す値ではありません。引数が0個なら何も読まず`true`を返します。
///
/// 注意: 変数と標準入力の状態を変更します。共通契約は [`typed_in`](../struct/typed_in.md#type-invariants) を参照。内部では未初期化の ll 一時値を読み、その値を modint へ代入します。正しい整数入力があることを前提に使い、EOF・不正入力後の値を保証しません。
///
/// 呼び分け: MINT998244353(x)、MINT998244353(x,y,...)、MINT998244353()。新しい変数を宣言するマクロではなく、const の入力オブジェクトを関数のように呼びます。ACLヘッダーが include パスにあり、AC_HAS_ACL が有効なビルドで使えます。
inline constexpr ac_detail::input::typed_in<mint998244353> MINT998244353{};
#endif

// @acf fn GETLINE — (string&... a) -> bool — 1行ずつ読む（先行空白は捨てる）。常に true

/// 既存のstringへ、行頭の空白を捨てて1行ずつ読む。
///
/// 引数 `a`: 書き換えるstringの列。0個も可。
///
/// 戻り値: boolで常にtrue。読み取り成功の判定ではありません。先頭の空白・改行・空行をcin>>wsで飛ばし、次の非空白文字から行末までを保存。末尾の空白は保持。
///
/// 注意: cinを消費して各変数を左から順に更新。EOF/失敗はcinの状態で確認します。
///
/// 呼び分け: 字下げや空行を保持したい場合はrd_line_raw。
template <class... T>
bool GETLINE(T&... a) {
    static_assert((is_same_v<ac_detail::io::remove_cvref_t<T>, string> && ...),
                  "GETLINE: string& only");
    ((cin >> ws, getline(cin, a)), ...);
    return true;
}

// @acf fn rd_line — () -> string — 1行読んで返す（先行空白は捨てる）

/// 標準入力から先行空白・空行を飛ばして1行を返す。
///
/// 戻り値: string。行末の改行は含みません。行頭の空白は捨て、末尾の空白は保持。EOFはcinの状態で確認し、空文字だけではEOFと空行を区別できません。
///
/// 注意: cinから1行を消費。cin>>wsを行うので複数の改行を読み飛ばします。
///
/// 呼び分け: 既存stringへ複数行を読む場合はGETLINE。
inline string rd_line() {
    string s;
    GETLINE(s);
    return s;
}

// @acf fn rd_line_raw — () -> string — 先行空白を保持して1行読む

/// 標準入力から先行空白を保持して1行を返す。
///
/// 戻り値: string。行末の改行は含みません。直前のトークン入力の改行も1行として読むため空文字になり得ます。EOFはcinの状態で確認し、空文字だけではEOFと空行を区別できません。
///
/// 注意: cinから1行を消費。cin>>wsは行いません。
///
/// 呼び分け: 既存stringへ複数行を読む場合はGETLINE。
inline string rd_line_raw() {
    string s;
    getline(cin, s);
    return s;
}

namespace ac_detail::split {

template <class C, class T>
void append(C& out, T&& x) {
    if constexpr (requires(C& c, T v) { c.push_back(std::forward<T>(v)); }) {
        out.push_back(std::forward<T>(x));
    } else if constexpr (requires(C& c, T v) { c.push(std::forward<T>(v)); }) {
        out.push(std::forward<T>(x));
    } else if constexpr (requires(C& c, T v) { c.insert(std::forward<T>(v)); }) {
        out.insert(std::forward<T>(x));
    } else {
        static_assert(sizeof(C) == 0,
                      "split_into: container needs push_back, push, or insert");
    }
}

template <class C, class T>
void append_parsed(C& out, string_view tok) {
    if constexpr (is_same_v<T, string>) {
        append(out, string(tok));
    } else if (!tok.empty()) {
        istringstream iss{string(tok)};
        T x{};
        if (iss >> x) append(out, std::move(x));
    }
}

}  // namespace ac_detail::split

// @acf fn split_into — (string_view s) -> C — 空白区切りでコンテナへ

/// 文字列を空白区切りの値としてコンテナへ分割する。
///
/// 引数 `s`: 分割する文字列。コピーしてストリームで読む。
///
/// 戻り値: `C`。C/Tはテンプレート引数。空白を飛ばして順に抽出し、最初の失敗で停止。空文字列は空。集合は重複を統合し、stack/queueの取り出し順はそのコンテナに従います。
///
/// 注意: 元文字列を変更せず、各値をoperator>>で読む。文字列長とコンテナへの挿入費用に比例。失敗したトークン以降は空白版では読みません。
///
/// 呼び分け: 区切り文字を指定する形式は不正トークン後も処理を続け、string要素では空トークンを保持します。
template <class C>
C split_into(string_view s) {
    C out;
    istringstream iss{string(s)};
    using T = typename C::value_type;
    T x{};
    while (iss >> x) ac_detail::split::append(out, std::move(x));
    return out;
}

// @acf fn split_into — (string_view s, char sep) -> C — char sep 区切りでコンテナへ

/// 文字列を指定文字で分割してコンテナへ入れる。
///
/// 引数 `s`: 分割する文字列。コピーしてストリームで読む。
///
/// 引数 `sep`: 1バイトの区切り文字。連続区切りや末尾の区切りも分割。
///
/// 戻り値: `C`。string要素は空トークン・内部空白を保持。他の型は空トークンを飛ばし、抽出できた値だけ入れます。数値の後ろの文字は全消費を要求せず、数値接頭辞を受け入れます。
///
/// 注意: 入力は変更しません。トークンごとに独立して解析し、不正トークンだけを飛ばします。
///
/// 呼び分け: sepを空白文字にしても空白抽出版とは挙動が異なります。Cはpush_back/push/insertを持ち、Tはstringまたはoperator>>で読める型。
template <class C>
C split_into(string_view s, char sep) {
    C out;
    using T = typename C::value_type;
    size_t i = 0;
    while (true) {
        size_t j = s.find(sep, i);
        string_view tok = (j == string_view::npos) ? s.substr(i) : s.substr(i, j - i);
        ac_detail::split::append_parsed<C, T>(out, tok);
        if (j == string_view::npos) break;
        i = j + 1;
    }
    return out;
}

// Character-separated splitting keeps all empty tokens unless explicitly trimmed.
// trim_empty removes every empty token at both edges, never an interior token.
enum class split_edges { keep, trim_empty };

// @acf fn split_into — (string_view s, char sep, split_edges edges) -> C — 両端の空要素の扱いを指定して分割

/// 指定文字で分割し、両端の空要素を残すか除くかを選ぶ。
///
/// 引数 `s`: 分割する文字列。入力自体は変更しません。
///
/// 引数 `sep`: 1バイトの区切り文字。
///
/// 引数 `edges`: keepは2引数版と同じ。trim_emptyは先頭・末尾に連続する空要素をすべて除きます。途中の空要素や非空要素は削りません。
///
/// 戻り値: `C`。trim_emptyで空文字列または区切り文字だけを渡すと空。string要素は所有するコピーで、元文字列の寿命に依存しません。
///
/// 注意: 境界の空トークンを型変換前に除き、残りを2引数版と同じ方法で分割します。文字列長と出力への挿入費用に比例します。数値などstring以外の型は空・不正トークンを従来どおり飛ばし、数値接頭辞も受け入れます。
///
/// 呼び分け: 空白抽出版や2引数版の型・挙動は変更しません。明示的なkeepは2引数版と同じです。
template <class C>
C split_into(string_view s, char sep, split_edges edges) {
    if (edges == split_edges::trim_empty) {
        while (!s.empty() && s.front() == sep) s.remove_prefix(1);
        while (!s.empty() && s.back() == sep) s.remove_suffix(1);
        if (s.empty()) return C{};
    }
    return split_into<C>(s, sep);
}

// @acf fn split — (string_view s) -> vector<T> — 空白区切りで vector へ

/// 文字列を空白区切りの値としてコンテナへ分割する。
///
/// 引数 `s`: 分割する文字列。コピーしてストリームで読む。
///
/// 戻り値: `vector<T>`。C/Tはテンプレート引数。空白を飛ばして順に抽出し、最初の失敗で停止。空文字列は空。集合は重複を統合し、stack/queueの取り出し順はそのコンテナに従います。
///
/// 注意: 元文字列を変更せず、各値をoperator>>で読む。文字列長とコンテナへの挿入費用に比例。失敗したトークン以降は空白版では読みません。
///
/// 呼び分け: 区切り文字を指定する形式は不正トークン後も処理を続け、string要素では空トークンを保持します。
template <class T>
vector<T> split(string_view s) {
    return split_into<vector<T>>(s);
}

// @acf fn split — (string_view s, char sep) -> vector<T> — char sep 区切りで vector へ

/// 文字列を指定文字で分割してコンテナへ入れる。
///
/// 引数 `s`: 分割する文字列。コピーしてストリームで読む。
///
/// 引数 `sep`: 1バイトの区切り文字。連続区切りや末尾の区切りも分割。
///
/// 戻り値: `vector<T>`。string要素は空トークン・内部空白を保持。他の型は空トークンを飛ばし、抽出できた値だけ入れます。数値の後ろの文字は全消費を要求せず、数値接頭辞を受け入れます。
///
/// 注意: 入力は変更しません。トークンごとに独立して解析し、不正トークンだけを飛ばします。
///
/// 呼び分け: sepを空白文字にしても空白抽出版とは挙動が異なります。Cはpush_back/push/insertを持ち、Tはstringまたはoperator>>で読める型。
template <class T>
vector<T> split(string_view s, char sep) {
    return split_into<vector<T>>(s, sep);
}

// @acf fn split — (string_view s, char sep, split_edges edges) -> vector<T> — 両端の空要素の扱いを指定して分割

/// 指定文字で分割し、両端の空要素を残すか除くかを選ぶ。
///
/// 引数 `s`: 分割する文字列。入力自体は変更しません。
///
/// 引数 `sep`: 1バイトの区切り文字。
///
/// 引数 `edges`: keepは2引数版と同じ。trim_emptyは先頭・末尾に連続する空要素をすべて除きます。途中の空要素や非空要素は削りません。
///
/// 戻り値: `vector<T>`。trim_emptyで空文字列または区切り文字だけを渡すと空。string要素は所有するコピーで、元文字列の寿命に依存しません。
///
/// 注意: 境界の空トークンを型変換前に除き、残りを2引数版と同じ方法で分割します。文字列長と出力への挿入費用に比例します。数値などstring以外の型は空・不正トークンを従来どおり飛ばし、数値接頭辞も受け入れます。
///
/// 呼び分け: 空白抽出版や2引数版の型・挙動は変更しません。明示的なkeepは2引数版と同じです。
template <class T>
vector<T> split(string_view s, char sep, split_edges edges) {
    return split_into<vector<T>>(s, sep, edges);
}

// @acf fn getline_into — () -> C — 1行読んでコンテナへ

/// 標準入力の1行を空白区切りの値としてコンテナへ分割する。
///
/// 戻り値: `C`。C/Tはテンプレート引数。空白を飛ばして順に抽出し、最初の失敗で停止。空文字列は空。集合は重複を統合し、stack/queueの取り出し順はそのコンテナに従います。
///
/// 注意: rd_lineで先行空白を捨てて1行消費し、各値をoperator>>で読む。文字列長とコンテナへの挿入費用に比例。失敗したトークン以降は空白版では読みません。
///
/// 呼び分け: 区切り文字を指定する形式は不正トークン後も処理を続け、string要素では空トークンを保持します。
template <class C>
C getline_into() {
    return split_into<C>(rd_line());
}

// @acf fn getline_into — (char sep) -> C — 1行読んで char sep 区切りでコンテナへ

/// 標準入力の1行を指定文字で分割してコンテナへ入れる。
///
/// 引数 `sep`: 1バイトの区切り文字。連続区切りや末尾の区切りも分割。
///
/// 戻り値: `C`。string要素は空トークン・内部空白を保持。他の型は空トークンを飛ばし、抽出できた値だけ入れます。数値の後ろの文字は全消費を要求せず、数値接頭辞を受け入れます。
///
/// 注意: rd_lineで先行空白を捨てて1行消費。トークンごとに独立して解析し、不正トークンだけを飛ばします。
///
/// 呼び分け: sepを空白文字にしても空白抽出版とは挙動が異なります。Cはpush_back/push/insertを持ち、Tはstringまたはoperator>>で読める型。
template <class C>
C getline_into(char sep) {
    return split_into<C>(rd_line(), sep);
}

// @acf fn getline_vec — () -> vector<T> — 1行読んで vector へ

/// 標準入力の1行を空白区切りの値としてコンテナへ分割する。
///
/// 戻り値: `vector<T>`。C/Tはテンプレート引数。空白を飛ばして順に抽出し、最初の失敗で停止。空文字列は空。集合は重複を統合し、stack/queueの取り出し順はそのコンテナに従います。
///
/// 注意: rd_lineで先行空白を捨てて1行消費し、各値をoperator>>で読む。文字列長とコンテナへの挿入費用に比例。失敗したトークン以降は空白版では読みません。
///
/// 呼び分け: 区切り文字を指定する形式は不正トークン後も処理を続け、string要素では空トークンを保持します。
template <class T>
vector<T> getline_vec() {
    return split<T>(rd_line());
}

// @acf fn getline_vec — (char sep) -> vector<T> — 1行読んで char sep 区切りで vector へ

/// 標準入力の1行を指定文字で分割してコンテナへ入れる。
///
/// 引数 `sep`: 1バイトの区切り文字。連続区切りや末尾の区切りも分割。
///
/// 戻り値: `vector<T>`。string要素は空トークン・内部空白を保持。他の型は空トークンを飛ばし、抽出できた値だけ入れます。数値の後ろの文字は全消費を要求せず、数値接頭辞を受け入れます。
///
/// 注意: rd_lineで先行空白を捨てて1行消費。トークンごとに独立して解析し、不正トークンだけを飛ばします。
///
/// 呼び分け: sepを空白文字にしても空白抽出版とは挙動が異なります。Cはpush_back/push/insertを持ち、Tはstringまたはoperator>>で読める型。
template <class T>
vector<T> getline_vec(char sep) {
    return split<T>(rd_line(), sep);
}

// ---------------------------------------------------------------------------
// Selected declaration + input shortcuts
// Int / Long / Str など。条件付き入力は宣言後に if (cond) in(x)。
// ---------------------------------------------------------------------------

// @acf macro Int — int 宣言 + I32

/// int 型の変数を宣言し、標準入力から値を読み込む。
///
/// 引数 `名前...`: 宣言する新しい変数名。既にある変数へ読むならin(x)。
///
/// 戻り値: 戻り値はありません。int型の変数を呼び出し元のスコープへ宣言します。
///
/// 注意: 標準入力を消費します。複数文へ展開するのでif/forの本文に置くときは必ず{...}で囲みます。同じスコープに同名の変数があると再宣言になります。読取失敗の成功判定は返しません。
///
/// 呼び分け: Int(x)またはInt(x,y,...)。型が決まっている宣言入力です。
#define Int(...) int __VA_ARGS__; I32(__VA_ARGS__)
// @acf macro Long — ll 宣言 + LL

/// ll（long long） 型の変数を宣言し、標準入力から値を読み込む。
///
/// 引数 `名前...`: 宣言する新しい変数名。既にある変数へ読むならin(x)。
///
/// 戻り値: 戻り値はありません。ll型の変数を呼び出し元のスコープへ宣言します。
///
/// 注意: 標準入力を消費します。複数文へ展開するのでif/forの本文に置くときは必ず{...}で囲みます。同じスコープに同名の変数があると再宣言になります。読取失敗の成功判定は返しません。
///
/// 呼び分け: Long(x)またはLong(x,y,...)。型が決まっている宣言入力です。
#define Long(...) ll __VA_ARGS__; LL(__VA_ARGS__)
// @acf macro UInt — ui 宣言 + UI32

/// uiの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 宣言だけでは基本型を初期化しません。読み込み失敗後の値は保証しません。
///
/// 呼び分け: UInt(a,b,...);。既存変数へ読むならUI32(a,b,...)。
#define UInt(...) ui __VA_ARGS__; UI32(__VA_ARGS__)
// @acf macro ULong — ull 宣言 + ULL

/// ullの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 宣言だけでは基本型を初期化しません。読み込み失敗後の値は保証しません。
///
/// 呼び分け: ULong(a,b,...);。既存変数へ読むならULL(a,b,...)。
#define ULong(...) ull __VA_ARGS__; ULL(__VA_ARGS__)
// @acf macro Dbl — double 宣言 + DB

/// dbの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 宣言だけでは基本型を初期化しません。読み込み失敗後の値は保証しません。
///
/// 呼び分け: Dbl(a,b,...);。既存変数へ読むならDB(a,b,...)。
#define Dbl(...) db __VA_ARGS__; DB(__VA_ARGS__)
// @acf macro Ld — long double 宣言 + LD

/// ldの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 宣言だけでは基本型を初期化しません。読み込み失敗後の値は保証しません。
///
/// 呼び分け: Ld(a,b,...);。既存変数へ読むならLD(a,b,...)。
#define Ld(...) ld __VA_ARGS__; LD(__VA_ARGS__)
// @acf macro Str — string 宣言 + STR

/// string 型の変数を宣言し、標準入力から値を読み込む。
///
/// 引数 `名前...`: 宣言する新しい変数名。既にある変数へ読むならin(x)。
///
/// 戻り値: 戻り値はありません。str型の変数を呼び出し元のスコープへ宣言します。
///
/// 注意: 標準入力を消費します。複数文へ展開するのでif/forの本文に置くときは必ず{...}で囲みます。同じスコープに同名の変数があると再宣言になります。読取失敗の成功判定は返しません。
///
/// 呼び分け: Str(x)またはStr(x,y,...)。Strは空白区切りの文字列。行全体の入力とは異なります。
#define Str(...) string __VA_ARGS__; STR(__VA_ARGS__)
// @acf macro Getline — string 宣言 + GETLINE（1行読み、先行空白は捨てる）

/// stringの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 各行の前にcin>>wsを実行するため、先頭の空白・空行を飛ばします。途中と末尾の空白は保持します。空行や先頭空白の保存には直接getlineを使用します。
///
/// 呼び分け: Getline(a,b,...);。既存変数へ読むならGETLINE(a,b,...)。
#define Getline(...) string __VA_ARGS__; GETLINE(__VA_ARGS__)
// @acf macro Chr — char 宣言 + CHAR

/// charの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 空白を飛ばして1文字ずつ読みます。
///
/// 呼び分け: Chr(a,b,...);。既存変数へ読むならCHAR(a,b,...)。
#define Chr(...) char __VA_ARGS__; CHAR(__VA_ARGS__)
// @acf macro Bol — bool 宣言 + BOOL

/// boolの変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 標準状態では0/1を読みます。boolalphaを設定すると入力形式も変わります。
///
/// 呼び分け: Bol(a,b,...);。既存変数へ読むならBOOL(a,b,...)。
#define Bol(...) bool __VA_ARGS__; BOOL(__VA_ARGS__)

#ifdef AC_HAS_ACL
// @acf macro Mint1000000007 — mint1000000007 宣言 + MINT1000000007

/// mint1000000007の変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 ACLのmodintが利用可能で、llとして読める正しい整数入力が必要です。負数も法の範囲へ正規化。失敗入力の値は保証しません。
///
/// 呼び分け: Mint1000000007(a,b,...);。既存変数へ読むならMINT1000000007(a,b,...)。
#define Mint1000000007(...) mint1000000007 __VA_ARGS__; MINT1000000007(__VA_ARGS__)
// @acf macro Mint998244353 — mint998244353 宣言 + MINT998244353

/// mint998244353の変数を宣言し、標準入力から読む。
///
/// 引数 `names...`: 同じ型で新規宣言する1つ以上の変数名。
///
/// 戻り値: 式としての戻り値はありません。宣言した変数に入力値を格納します。
///
/// 注意: 指定した名前を呼び出し元のブロックに宣言し、標準入力を左から読みます。既存変数の更新用ではありません。入力成功を返さないのでcinの状態を確認します。 ACLのmodintが利用可能で、llとして読める正しい整数入力が必要です。負数も法の範囲へ正規化。失敗入力の値は保証しません。
///
/// 呼び分け: Mint998244353(a,b,...);。既存変数へ読むならMINT998244353(a,b,...)。
#define Mint998244353(...) mint998244353 __VA_ARGS__; MINT998244353(__VA_ARGS__)
#endif

// ---------------------------------------------------------------------------
// Selected container declaration + input shortcuts
// VI / VLL / SI / BS32 など。その他は汎用入力関数を使う。
// ---------------------------------------------------------------------------

// The one-argument form reads its size before the elements. No caller-visible
// size variable is introduced; the two-argument expansion stays unchanged.
#define AC_INPUT_VECTOR_SELECT_(_1, _2, SELECT, ...) SELECT
#define AC_INPUT_VECTOR_1_(type, name) type name(rd<int>()); in(name)
#define AC_INPUT_VECTOR_2_(type, name, n) type name(n); in(name)

// @acf macro VI — VI(a, n) → vi a(n); in(a)

/// 配列を宣言し、決まった個数の要素を入力する。
///
/// 引数 `name`: 新しく宣言する配列名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 戻り値はありません。vi型で長さnの配列nameを宣言します。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: nは配列構築時に1回評価します。標準入力をn要素分消費します。宣言＋入力の複数文なので条件付きで使うときは{...}を付けます。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VI(name); / VI(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VI(name,N);とします。既存配列を読むならin(name)。
#define VI(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vi, __VA_ARGS__)
// @acf macro VLL — vll a(n); in(a)

/// 配列を宣言し、決まった個数の要素を入力する。
///
/// 引数 `name`: 新しく宣言する配列名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 戻り値はありません。vll型で長さnの配列nameを宣言します。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: nは配列構築時に1回評価します。標準入力をn要素分消費します。宣言＋入力の複数文なので条件付きで使うときは{...}を付けます。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VLL(name); / VLL(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VLL(name,N);とします。既存配列を読むならin(name)。
#define VLL(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vll, __VA_ARGS__)
// @acf macro VDB — vdb a(n); in(a)

/// dbの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vdb（`vector<db>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VDB(name); / VDB(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VDB(name,N);とします。既存配列を読むならin(name)。
#define VDB(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vdb, __VA_ARGS__)
// @acf macro VB — vb a(n); in(a)（vector<bool> の特殊化に注意）

/// boolの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vb（`vector<bool>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 boolは標準状態で0/1、charは空白を飛ばした1文字、stringは空白区切りの単語です。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VB(name); / VB(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VB(name,N);とします。既存配列を読むならin(name)。
#define VB(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vb, __VA_ARGS__)
// @acf macro VC — vc a(n); in(a)

/// charの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vc（`vector<char>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 boolは標準状態で0/1、charは空白を飛ばした1文字、stringは空白区切りの単語です。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VC(name); / VC(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VC(name,N);とします。既存配列を読むならin(name)。
#define VC(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vc, __VA_ARGS__)
// @acf macro VS — vs a(n); in(a)

/// stringの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vs（`vector<string>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 boolは標準状態で0/1、charは空白を飛ばした1文字、stringは空白区切りの単語です。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VS(name); / VS(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VS(name,N);とします。既存配列を読むならin(name)。
#define VS(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vs, __VA_ARGS__)
// @acf macro VPII — vpii a(n); in(a)

/// piiの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vpii（`vector<pii>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 pairは各要素のfirst、secondの順です。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VPII(name); / VPII(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VPII(name,N);とします。既存配列を読むならin(name)。
#define VPII(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vpii, __VA_ARGS__)
// @acf macro VPLL — vpll a(n); in(a)

/// pllの配列を宣言し、n要素を順に読む。
///
/// 引数 `name`: 新しい配列変数名。
///
/// 引数 `n`: 省略可能。明示時は0以上で確保可能な要素数を1回評価します。省略時は標準入力から0以上のintを1つ読み、個数にします。
///
/// 戻り値: 式の戻り値なし。vpll（`vector<pll>`）の変数ができます。個数が0なら空配列です。明示形式では入力を消費せず、省略形式では個数の0だけ読みます。
///
/// 注意: 呼び出し元のスコープへ宣言し、標準入力を要素順に消費します。入力成功はcinの状態で確認。 pairは各要素のfirst、secondの順です。 省略形式では個数、各要素の順に読みます。固定名Nなどの個数変数を追加・変更せず、個数はname.size()で取得できます。有効な個数と必要数の要素が入力されることが前提です。
///
/// 呼び分け: VPLL(name); / VPLL(name,n);。引数の個数で分岐し、Nの宣言有無は判定しません。入力がN W 配列の順なら先にInt(N,W);を使い、VPLL(name,N);とします。既存配列を読むならin(name)。
#define VPLL(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vpll, __VA_ARGS__)
// @acf macro ROWS — ROWS(name, n, T1, T2, ...) → auto name = in_vector<tuple<T1,T2,...>>(n)

/// 複数の型を持つ行を、tupleの配列としてまとめて読む。
///
/// 引数 `name`: 新しい配列の変数名。
///
/// 引数 `n`: 0以上で確保可能な行数。1回評価。
///
/// 引数 `types...`: 各行の列型。1つ以上をコンマ区切りで指定。
///
/// 戻り値: `vector<tuple<types...>>`の変数を宣言。n=0なら空配列で入力を消費しません。
///
/// 注意: 行の順、行内の列順に標準入力を読みます。改行そのものを行境界と判定せず、各列の型の読み込み規則に従います。変数は呼び出し元のスコープ。入力成功はcinの状態で確認。
///
/// 呼び分け: ROWS(rows,n,int,string,ll);。同じ型の列を行列へ読むならVVI/VVLL。
#define ROWS(name, n, ...) auto name = in_vector<tuple<__VA_ARGS__>>(n)
// @acf macro VVI — vvi a(n, vi(m)); in(a)

/// intのn行m列の行列を宣言して読む。
///
/// 引数 `name`: 新しい行列変数名。
///
/// 引数 `n`: 0以上の行数。1回評価。
///
/// 引数 `m`: 0以上の列数。1回評価。n=0でも行用の初期値構築に評価します。
///
/// 戻り値: vviの変数を宣言。n=0は0行、m=0はn個の空行。どちらも入力を消費しません。
///
/// 注意: 呼び出し元へ宣言し、先頭行から各列を順に標準入力から読みます。確保可能なサイズを渡し、入力成功はcinの状態で確認します。
///
/// 呼び分け: VVI(matrix,n,m);。既存行列にはin(matrix)。
#define VVI(name, n, m) vvi name(n, vi(m)); in(name)
// @acf macro VVLL — vvll a(n, vll(m)); in(a)

/// llのn行m列の行列を宣言して読む。
///
/// 引数 `name`: 新しい行列変数名。
///
/// 引数 `n`: 0以上の行数。1回評価。
///
/// 引数 `m`: 0以上の列数。1回評価。n=0でも行用の初期値構築に評価します。
///
/// 戻り値: vvllの変数を宣言。n=0は0行、m=0はn個の空行。どちらも入力を消費しません。
///
/// 注意: 呼び出し元へ宣言し、先頭行から各列を順に標準入力から読みます。確保可能なサイズを渡し、入力成功はcinの状態で確認します。
///
/// 呼び分け: VVLL(matrix,n,m);。既存行列にはin(matrix)。
#define VVLL(name, n, m) vvll name(n, vll(m)); in(name)

// 短縮名はこの選抜リストのみ。型別名の追加に入力マクロを連動させない。

// @acf macro SI — si を n 個読む

/// intのn個の入力値を重複なしの順序付き集合へ読む。
///
/// 引数 `name`: 新しいset変数の名前。
///
/// 引数 `n`: 読む値の個数。通常は0以上。開始時に1回評価。
///
/// 戻り値: `set<int>`を宣言。重複を除くためsize()はn以下。nが0以下なら空集合で入力を消費しません。
///
/// 注意: 標準入力をn回読み、集合へ挿入します。入力順は保持せず昇順に並びます。変数は呼び出し元のスコープ。入力成功はcinの状態で確認。
///
/// 呼び分け: SI(values,n);。重複を残すなら`in_mset<T>`(n)。
#define SI(name, n) auto name = in_set<int>(n)
// @acf macro SLL — sll を n 個読む

/// llのn個の入力値を重複なしの順序付き集合へ読む。
///
/// 引数 `name`: 新しいset変数の名前。
///
/// 引数 `n`: 読む値の個数。通常は0以上。開始時に1回評価。
///
/// 戻り値: `set<ll>`を宣言。重複を除くためsize()はn以下。nが0以下なら空集合で入力を消費しません。
///
/// 注意: 標準入力をn回読み、集合へ挿入します。入力順は保持せず昇順に並びます。変数は呼び出し元のスコープ。入力成功はcinの状態で確認。
///
/// 呼び分け: SLL(values,n);。重複を残すなら`in_mset<T>`(n)。
#define SLL(name, n) auto name = in_set<ll>(n)
// @acf macro BS32 — bs32 宣言 + in（01 文字列、複数可）

/// 01文字列を読み、32ビットのbitset変数を宣言する。
///
/// 引数 `names...`: 新しく宣言する1つ以上のbitset変数名。
///
/// 戻り値: 式の戻り値なし。bs32（`bitset<32>`）に格納します。
///
/// 注意: 呼び出し元へ変数を宣言し、左から1つずつ入力します。入力成功はcinの状態で確認。 std::bitsetの抽出演算子で先頭から最大32個の0/1を読みます。右端がbit0。幅より長いトークンの残りは次の入力へ残るので、通常は幅以下の01トークンを渡します。
///
/// 呼び分け: BS32(a,b,...);。10進整数ならBS32U。
#define BS32(...) bs32 __VA_ARGS__; in(__VA_ARGS__)
// @acf macro BS64 — bs64 宣言 + in（01 文字列、複数可）

/// 01文字列を読み、64ビットのbitset変数を宣言する。
///
/// 引数 `names...`: 新しく宣言する1つ以上のbitset変数名。
///
/// 戻り値: 式の戻り値なし。bs64（`bitset<64>`）に格納します。
///
/// 注意: 呼び出し元へ変数を宣言し、左から1つずつ入力します。入力成功はcinの状態で確認。 std::bitsetの抽出演算子で先頭から最大64個の0/1を読みます。右端がbit0。幅より長いトークンの残りは次の入力へ残るので、通常は幅以下の01トークンを渡します。
///
/// 呼び分け: BS64(a,b,...);。10進整数ならBS64UL。
#define BS64(...) bs64 __VA_ARGS__; in(__VA_ARGS__)
// @acf macro BS32U — bs32 宣言 + ui から構築（10 進、複数可）

/// 10進の符号なし整数を読み、32ビットのbitset変数を宣言する。
///
/// 引数 `names...`: 新しく宣言する1つ以上のbitset変数名。
///
/// 戻り値: 式の戻り値なし。bs32（`bitset<32>`）に格納します。
///
/// 注意: 呼び出し元へ変数を宣言し、左から1つずつ入力します。入力成功はcinの状態で確認。 `rd<ui>`()の値からbitsetを構築。uiで表せる正しい非負整数を渡します。01文字列の読み込みではありません。
///
/// 呼び分け: BS32U(a,b,...);。01文字列ならBS32。
#define BS32U(...) bs32 __VA_ARGS__; ac_detail::input::assign_bs_from_int<ui>(__VA_ARGS__)
// @acf macro BS64UL — bs64 宣言 + ull から構築（10 進、複数可）

/// 10進の符号なし整数を読み、64ビットのbitset変数を宣言する。
///
/// 引数 `names...`: 新しく宣言する1つ以上のbitset変数名。
///
/// 戻り値: 式の戻り値なし。bs64（`bitset<64>`）に格納します。
///
/// 注意: 呼び出し元へ変数を宣言し、左から1つずつ入力します。入力成功はcinの状態で確認。 `rd<ull>`()の値からbitsetを構築。ullで表せる正しい非負整数を渡します。01文字列の読み込みではありません。
///
/// 呼び分け: BS64UL(a,b,...);。01文字列ならBS64。
#define BS64UL(...) bs64 __VA_ARGS__; ac_detail::input::assign_bs_from_int<ull>(__VA_ARGS__)

// ---------------------------------------------------------------------------
// Selected expression shortcut (returns a value)
// in_vi は既存の頻出名として残す。その他は rd<T>() / in_vector<T>(n)。
// ---------------------------------------------------------------------------

// --- vector ---
// @acf macro in_vi — vi を式として返す

/// intの配列を入力し、式の値として受け取る。
///
/// 引数 `n`: 読む要素数。非負。サイズ自体を入力する処理は含みません。
///
/// 戻り値: vi（`vector<int>`）。長さn。n=0なら空。読取成否を返すAPIではありません。
///
/// 注意: nを1回評価し、標準入力を消費します。元のコンテナを書き換えるのでなく、新しい配列を返します。
///
/// 呼び分け: auto a = in_vi(n)。汎用の`in_vector<int>`(n)と同じ。VI(a,n)は宣言マクロなので別の構文です。
#define in_vi(n) in_vector<int>(n)

// in_bs32u / in_bs64ul は layer 1 の関数（10 進）
