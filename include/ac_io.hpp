#pragma once
// Library: https://github.com/facedolid/atcoder-headers

// 宣言しながら読む API と、既存変数・コンテナへ読む API の互換レイヤー。
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
/// @acf-doc @brief vector<T>(n) を読んで返す
/// @acf-doc @code (int n) -> vector<T>
/// @acf-doc [function] in_vector
template <class T>
vector<T> in_vector(int n) {
    vector<T> v(n);
    ac_detail::input::extract(cin, v);
    return v;
}

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
/// @acf-doc @brief n 行のタプル列を読む。各行は型列 Ts... を空白区切りで読む
/// @acf-doc @code (int n) -> vector<tuple<Ts...>>
/// @acf-doc [function] in_rows
template <class... Ts>
vector<tuple<Ts...>> in_rows(int n) {
    return in_vector<tuple<Ts...>>(n);
}

// @acf fn in_mat — (int n, int m) -> vector<vector<T>> — 2次元 vector を読んで返す
/// @acf-doc @brief 2次元 vector を読んで返す
/// @acf-doc @code (int n, int m) -> vector<vector<T>>
/// @acf-doc [function] in_mat
template <class T>
vector<vector<T>> in_mat(int n, int m) {
    vector<vector<T>> v(n, vector<T>(m));
    ac_detail::input::extract(cin, v);
    return v;
}

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
/// @acf-doc @brief 3次元 vector を読んで返す
/// @acf-doc @code (int n, int m, int k) -> vector<vector<vector<T>>>
/// @acf-doc [function] in_mat3
template <class T>
vector<vector<vector<T>>> in_mat3(int n, int m, int k) {
    vector<vector<vector<T>>> v(n, vector<vector<T>>(m, vector<T>(k)));
    ac_detail::input::extract(cin, v);
    return v;
}

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
/// @acf-doc @brief n 要素の deque を読んで返す
/// @acf-doc @code (int n) -> deque<T>
/// @acf-doc [function] in_deque
template <class T>
deque<T> in_deque(int n) {
    deque<T> d(n);
    ac_detail::input::extract(cin, d);
    return d;
}

// @acf fn in_list_seq — (int n) -> list<T> — n 要素の list を読んで返す
/// @acf-doc @brief n 要素の list を読んで返す
/// @acf-doc @code (int n) -> list<T>
/// @acf-doc [function] in_list_seq
template <class T>
list<T> in_list_seq(int n) {
    list<T> l(n);
    ac_detail::input::extract(cin, l);
    return l;
}

// @acf fn in_array — () -> array<T,N> — 固定長 array を読んで返す
/// @acf-doc @brief 固定長 array を読んで返す
/// @acf-doc @code () -> array<T,N>
/// @acf-doc [function] in_array
template <class T, size_t N>
array<T, N> in_array() {
    array<T, N> a;
    ac_detail::input::extract(cin, a);
    return a;
}

// @acf fn in_bs — () -> bitset<N> — 01 文字列を bitset として読んで返す
/// @acf-doc @brief 01 文字列を bitset として読んで返す（`operator>>` と同じ。10 進整数なら `in_bs32u` / `in_bs64ul`）
/// @acf-doc @code () -> bitset<N>
/// @acf-doc [function] in_bs
template <size_t N>
bitset<N> in_bs() {
    bitset<N> b;
    ac_detail::input::extract(cin, b);
    return b;
}

// @acf fn in_bs32u — () -> bs32 — ui を読んで bs32 にする
/// @acf-doc @brief ui を読んで bs32 にする（10 進。01 文字列なら `in_bs32`）
/// @acf-doc @code () -> bs32
/// @acf-doc [function] in_bs32u
inline bs32 in_bs32u() { return bs32(rd<ui>()); }

// @acf fn in_bs64ul — () -> bs64 — ull を読んで bs64 にする
/// @acf-doc @brief ull を読んで bs64 にする（10 進。01 文字列なら `in_bs64`）
/// @acf-doc @code () -> bs64
/// @acf-doc [function] in_bs64ul
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
/// @acf-doc @brief n 個読んで set に
/// @acf-doc @code (int n) -> set<T>
/// @acf-doc [function] in_set
template <class T>
set<T> in_set(int n) {
    set<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_mset — (int n) -> multiset<T> — 重複を保って n 個読む
/// @acf-doc @brief 重複を保って n 個読む
/// @acf-doc @code (int n) -> multiset<T>
/// @acf-doc [function] in_mset
template <class T>
multiset<T> in_mset(int n) {
    multiset<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_uset — (int n) -> unordered_set<T> — 順序なし集合へ n 個読む
/// @acf-doc @brief 順序なし集合へ n 個読む
/// @acf-doc @code (int n) -> unordered_set<T>
/// @acf-doc [function] in_uset
template <class T>
unordered_set<T> in_uset(int n) {
    unordered_set<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_umset — (int n) -> unordered_multiset<T> — 順序なしで重複を保って n 個読む
/// @acf-doc @brief 順序なしで重複を保って n 個読む
/// @acf-doc @code (int n) -> unordered_multiset<T>
/// @acf-doc [function] in_umset
template <class T>
unordered_multiset<T> in_umset(int n) {
    unordered_multiset<T> s;
    rep(_, n) s.insert(rd<T>());
    return s;
}

// @acf fn in_map — (int n) -> map<K,V> — n 組の key,value を map に
/// @acf-doc @brief n 組の key,value を map に
/// @acf-doc @code (int n) -> map<K,V>
/// @acf-doc [function] in_map
template <class K, class V>
map<K, V> in_map(int n) {
    return ac_detail::input::read_assoc_unique<map<K, V>>(n);
}

// @acf fn in_mmap — (int n) -> multimap<K,V> — 同じキーを保って n 組読む
/// @acf-doc @brief 同じキーを保って n 組読む
/// @acf-doc @code (int n) -> multimap<K,V>
/// @acf-doc [function] in_mmap
template <class K, class V>
multimap<K, V> in_mmap(int n) {
    return ac_detail::input::read_assoc<multimap<K, V>>(n);
}

// @acf fn in_umap — (int n) -> unordered_map<K,V> — 順序なし map へ n 組読む
/// @acf-doc @brief 順序なし map へ n 組読む
/// @acf-doc @code (int n) -> unordered_map<K,V>
/// @acf-doc [function] in_umap
template <class K, class V>
unordered_map<K, V> in_umap(int n) {
    return ac_detail::input::read_assoc_unique<unordered_map<K, V>>(n);
}

// @acf fn in_ummap — (int n) -> unordered_multimap<K,V> — 順序なしで同じキーを保って n 組読む
/// @acf-doc @brief 順序なしで同じキーを保って n 組読む
/// @acf-doc @code (int n) -> unordered_multimap<K,V>
/// @acf-doc [function] in_ummap
template <class K, class V>
unordered_multimap<K, V> in_ummap(int n) {
    return ac_detail::input::read_assoc<unordered_multimap<K, V>>(n);
}

// @acf fn in_queue — (int n) -> queue<T> — 入力順に n 個を queue へ積む
/// @acf-doc @brief 入力順に n 個を queue へ積む
/// @acf-doc @code (int n) -> queue<T>
/// @acf-doc [function] in_queue
template <class T>
queue<T> in_queue(int n) {
    queue<T> q;
    rep(_, n) q.push(rd<T>());
    return q;
}

// @acf fn in_stack — (int n) -> stack<T> — 入力順に n 個を stack へ積む
/// @acf-doc @brief 入力順に n 個を stack へ積む
/// @acf-doc @code (int n) -> stack<T>
/// @acf-doc [function] in_stack
template <class T>
stack<T> in_stack(int n) {
    stack<T> st;
    rep(_, n) st.push(rd<T>());
    return st;
}

// @acf fn in_pq — (int n) -> priority_queue<T,...> — n 個を優先度付きキューへ積む
/// @acf-doc @brief n 個を優先度付きキューへ積む
/// @acf-doc @code (int n) -> priority_queue<T,...>
/// @acf-doc [function] in_pq
template <class T, class Comp>
priority_queue<T, vector<T>, Comp> in_pq(int n) {
    priority_queue<T, vector<T>, Comp> pq;
    rep(_, n) pq.push(rd<T>());
    return pq;
}

template <class T>
priority_queue<T> in_pq(int n) {
    return in_pq<T, less<T>>(n);
}

// @acf fn in_setpq — (int n) -> setpq<T, Comp> — n 個を読み、重複をまとめる優先度付きキューを返す
/// @acf-doc @brief n 個すべてを読み、比較上同値の値を1個にまとめる。既定は最大値優先、`greater<T>`で最小値優先。nは非負。n=0は空。時間O(n log(n+1))、メモリO(n)
/// @acf-doc @code (int n) -> setpq<T, Comp>
/// @acf-doc [function] in_setpq
template <class T, class Comp = less<T>>
setpq<T, Comp> in_setpq(int n) {
    setpq<T, Comp> pq;
    rep(_, n) pq.push(rd<T>());
    return pq;
}

// @acf fn in_maxpq — (int n) -> maxpq<T> — n 個を最大ヒープへ積む
/// @acf-doc @brief n 個を読み、最大値を top に持つ priority_queue を返す
/// @acf-doc @code (int n) -> maxpq<T>
/// @acf-doc [function] in_maxpq
template <class T>
maxpq<T> in_maxpq(int n) {
    return in_pq<T, less<T>>(n);
}

// @acf fn in_minpq — (int n) -> minpq<T> — n 個を最小ヒープへ積む
/// @acf-doc @brief n 個を読み、最小値を top に持つ priority_queue を返す
/// @acf-doc @code (int n) -> minpq<T>
/// @acf-doc [function] in_minpq
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
template <class Expected>
struct typed_in {
    template <class... T>
    /// @brief 指定型の既存変数へ入力し、常に `true` を返す。
    bool operator()(T&... xs) const {
        static_assert((is_same_v<ac_detail::io::remove_cvref_t<T>, Expected> && ...),
                      "typed in: variable type mismatch");
        return in(xs...);
    }
};
}  // namespace ac_detail::input

// @acf fn I32 — (int&... a) -> bool — int へ読む。常に true（`cond && I32(i)` 可）
/// @acf-doc @brief 既存の int へ読む。常に true。宣言つきは `Int`
/// @acf-doc @code (int&... a) -> bool
/// @acf-doc [function] I32
inline constexpr ac_detail::input::typed_in<int> I32{};
// @acf fn LL — (ll&... a) -> bool — ll へ読む。常に true（`cond && LL(x)` 可）
/// @acf-doc @brief 既存の ll へ読む。常に true。宣言つきは `Long`
/// @acf-doc @code (ll&... a) -> bool
/// @acf-doc [function] LL
inline constexpr ac_detail::input::typed_in<ll> LL{};
// @acf fn UI32 — (ui&... a) -> bool — ui へ読む。常に true
/// @acf-doc @brief 既存の ui へ読む。常に true。宣言つきは `UInt`
/// @acf-doc @code (ui&... a) -> bool
/// @acf-doc [function] UI32
inline constexpr ac_detail::input::typed_in<ui> UI32{};
// @acf fn ULL — (ull&... a) -> bool — ull へ読む。常に true
/// @acf-doc @brief 既存の ull へ読む。常に true。宣言つきは `ULong`
/// @acf-doc @code (ull&... a) -> bool
/// @acf-doc [function] ULL
inline constexpr ac_detail::input::typed_in<ull> ULL{};
// @acf fn DB — (db&... a) -> bool — double へ読む。常に true
/// @acf-doc @brief 既存の double へ読む。常に true。宣言つきは `Dbl`
/// @acf-doc @code (db&... a) -> bool
/// @acf-doc [function] DB
inline constexpr ac_detail::input::typed_in<db> DB{};
// @acf fn LD — (ld&... a) -> bool — long double へ読む。常に true
/// @acf-doc @brief 既存の long double へ読む。常に true。宣言つきは `Ld`
/// @acf-doc @code (ld&... a) -> bool
/// @acf-doc [function] LD
inline constexpr ac_detail::input::typed_in<ld> LD{};
// @acf fn STR — (string&... a) -> bool — string へ読む。常に true
/// @acf-doc @brief 既存の string へ読む。常に true。宣言つきは `Str`
/// @acf-doc @code (string&... a) -> bool
/// @acf-doc [function] STR
inline constexpr ac_detail::input::typed_in<string> STR{};
// @acf fn CHAR — (char&... a) -> bool — char へ読む。常に true
/// @acf-doc @brief 既存の char へ読む。常に true。宣言つきは `Chr`
/// @acf-doc @code (char&... a) -> bool
/// @acf-doc [function] CHAR
inline constexpr ac_detail::input::typed_in<char> CHAR{};
// @acf fn BOOL — (bool&... a) -> bool — bool へ読む。常に true
/// @acf-doc @brief 既存の bool へ読む。常に true。宣言つきは `Bol`
/// @acf-doc @code (bool&... a) -> bool
/// @acf-doc [function] BOOL
inline constexpr ac_detail::input::typed_in<bool> BOOL{};

#ifdef AC_HAS_ACL
// @acf fn MINT1000000007 — (mint1000000007&... a) -> bool — modint 1e9+7 へ読む
/// @acf-doc @brief 既存の `mint1000000007` へ読む。宣言つきは `Mint1000000007`
/// @acf-doc @code (mint1000000007&... a) -> bool
/// @acf-doc [function] MINT1000000007
inline constexpr ac_detail::input::typed_in<mint1000000007> MINT1000000007{};
// @acf fn MINT998244353 — (mint998244353&... a) -> bool — modint 998244353 へ読む
/// @acf-doc @brief 既存の `mint998244353` へ読む。宣言つきは `Mint998244353`
/// @acf-doc @code (mint998244353&... a) -> bool
/// @acf-doc [function] MINT998244353
inline constexpr ac_detail::input::typed_in<mint998244353> MINT998244353{};
#endif

// @acf fn GETLINE — (string&... a) -> bool — 1行ずつ読む（先行空白は捨てる）。常に true
/// @acf-doc @brief 既存の string へ `getline`（先に `cin >> ws`）。`Int` 直後でも空行にならない。宣言つきは `Getline`
/// @acf-doc @code (string&... a) -> bool
/// @acf-doc [function] GETLINE
template <class... T>
bool GETLINE(T&... a) {
    static_assert((is_same_v<ac_detail::io::remove_cvref_t<T>, string> && ...),
                  "GETLINE: string& only");
    ((cin >> ws, getline(cin, a)), ...);
    return true;
}

// @acf fn rd_line — () -> string — 1行読んで返す（先行空白は捨てる）
/// @acf-doc @brief 1行読んで返す（`GETLINE` と同じく先行空白を捨てる）
/// @acf-doc @code () -> string
/// @acf-doc [function] rd_line
inline string rd_line() {
    string s;
    GETLINE(s);
    return s;
}

// @acf fn rd_line_raw — () -> string — 先行空白を保持して1行読む
/// @acf-doc @brief `cin >> ws` を行わず、そのまま `getline`。直前の改行も入力として扱う
/// @acf-doc @code () -> string
/// @acf-doc [function] rd_line_raw
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
/// @acf-doc @brief 空白区切りトークンを `C` へ（`push_back` / `push` / `insert`。例: `vi` / `si` / `stack<str>` / `deque<int>`）
/// @acf-doc @code (string_view s) -> C
/// @acf-doc [function] split_into
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
/// @acf-doc @brief `sep` で分割して `C` へ。`string` は空トークンも残す。他型は空スキップ＋`>>` パース。空白版（`>>`）とは別（`sep==' '` でも連続空白で空が出うる）
/// @acf-doc @code (string_view s, char sep) -> C
/// @acf-doc [function] split_into
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

// @acf fn split — (string_view s) -> vector<T> — 空白区切りで vector へ
/// @acf-doc @brief 空白区切りトークンを `vector<T>` へ（`split_into<vector<T>>`）
/// @acf-doc @code (string_view s) -> vector<T>
/// @acf-doc [function] split
template <class T>
vector<T> split(string_view s) {
    return split_into<vector<T>>(s);
}

// @acf fn split — (string_view s, char sep) -> vector<T> — char sep 区切りで vector へ
/// @acf-doc @brief `sep` 区切りで `vector<T>` へ（`split_into<vector<T>>(s, sep)`）。空白版とは別物
/// @acf-doc @code (string_view s, char sep) -> vector<T>
/// @acf-doc [function] split
template <class T>
vector<T> split(string_view s, char sep) {
    return split_into<vector<T>>(s, sep);
}

// @acf fn getline_into — () -> C — 1行読んでコンテナへ
/// @acf-doc @brief 1行読んで `split_into<C>`（例: `getline_into<si>()`）
/// @acf-doc @code () -> C
/// @acf-doc [function] getline_into
template <class C>
C getline_into() {
    return split_into<C>(rd_line());
}

// @acf fn getline_into — (char sep) -> C — 1行読んで char sep 区切りでコンテナへ
/// @acf-doc @brief 1行読んで `split_into<C>(rd_line(), sep)`
/// @acf-doc @code (char sep) -> C
/// @acf-doc [function] getline_into
template <class C>
C getline_into(char sep) {
    return split_into<C>(rd_line(), sep);
}

// @acf fn getline_vec — () -> vector<T> — 1行読んで vector へ
/// @acf-doc @brief 1行読んで `vector<T>`（`split<T>(rd_line())`）
/// @acf-doc @code () -> vector<T>
/// @acf-doc [function] getline_vec
template <class T>
vector<T> getline_vec() {
    return split<T>(rd_line());
}

// @acf fn getline_vec — (char sep) -> vector<T> — 1行読んで char sep 区切りで vector へ
/// @acf-doc @brief 1行読んで `split<T>(rd_line(), sep)`
/// @acf-doc @code (char sep) -> vector<T>
/// @acf-doc [function] getline_vec
template <class T>
vector<T> getline_vec(char sep) {
    return split<T>(rd_line(), sep);
}

// ---------------------------------------------------------------------------
// Selected declaration + input shortcuts
// Int / Long / Str など。条件付き入力は宣言後に if (cond) in(x)。
// ---------------------------------------------------------------------------

// @acf macro Int — int 宣言 + I32
/// @acf-doc @brief int 宣言 + `I32`。右辺チェーン読みは宣言済みへ `I32(x)`
/// @acf-doc [macro] Int
#define Int(...) int __VA_ARGS__; I32(__VA_ARGS__)
// @acf macro Long — ll 宣言 + LL
/// @acf-doc @brief ll 宣言 + `LL`
/// @acf-doc [macro] Long
#define Long(...) ll __VA_ARGS__; LL(__VA_ARGS__)
// @acf macro UInt — ui 宣言 + UI32
/// @acf-doc @brief ui 宣言 + `UI32`
/// @acf-doc [macro] UInt
#define UInt(...) ui __VA_ARGS__; UI32(__VA_ARGS__)
// @acf macro ULong — ull 宣言 + ULL
/// @acf-doc @brief ull 宣言 + `ULL`
/// @acf-doc [macro] ULong
#define ULong(...) ull __VA_ARGS__; ULL(__VA_ARGS__)
// @acf macro Dbl — double 宣言 + DB
/// @acf-doc @brief double 宣言 + `DB`
/// @acf-doc [macro] Dbl
#define Dbl(...) db __VA_ARGS__; DB(__VA_ARGS__)
// @acf macro Ld — long double 宣言 + LD
/// @acf-doc @brief long double 宣言 + `LD`
/// @acf-doc [macro] Ld
#define Ld(...) ld __VA_ARGS__; LD(__VA_ARGS__)
// @acf macro Str — string 宣言 + STR
/// @acf-doc @brief string 宣言 + `STR`
/// @acf-doc [macro] Str
#define Str(...) string __VA_ARGS__; STR(__VA_ARGS__)
// @acf macro Getline — string 宣言 + GETLINE（1行読み、先行空白は捨てる）
/// @acf-doc @brief string 宣言 + `GETLINE`。空白/区切り加工は `split` / `split_into`（`sep` 可）、即コンテナは `getline_vec` / `getline_into`
/// @acf-doc [macro] Getline
#define Getline(...) string __VA_ARGS__; GETLINE(__VA_ARGS__)
// @acf macro Chr — char 宣言 + CHAR
/// @acf-doc @brief char 宣言 + `CHAR`
/// @acf-doc [macro] Chr
#define Chr(...) char __VA_ARGS__; CHAR(__VA_ARGS__)
// @acf macro Bol — bool 宣言 + BOOL
/// @acf-doc @brief bool 宣言 + `BOOL`
/// @acf-doc [macro] Bol
#define Bol(...) bool __VA_ARGS__; BOOL(__VA_ARGS__)

#ifdef AC_HAS_ACL
// @acf macro Mint1000000007 — mint1000000007 宣言 + MINT1000000007
/// @acf-doc @brief `mint1000000007` を宣言して入力。出力は `print(x)` で可
/// @acf-doc [macro] Mint1000000007
#define Mint1000000007(...) mint1000000007 __VA_ARGS__; MINT1000000007(__VA_ARGS__)
// @acf macro Mint998244353 — mint998244353 宣言 + MINT998244353
/// @acf-doc @brief `mint998244353` を宣言して入力。出力は `print(x)` で可
/// @acf-doc [macro] Mint998244353
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
/// @acf-doc @brief `VI(name)`は個数をintで読んでから要素を読む。`VI(name, n)`は指定したn要素を読む。型は`vi`。個数変数は宣言しない
/// @acf-doc [macro] VI
#define VI(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vi, __VA_ARGS__)
// @acf macro VLL — vll a(n); in(a)
/// @acf-doc @brief `VLL(name)`は個数をintで読んでから要素を読む。`VLL(name, n)`は指定したn要素を読む。型は`vll`。個数変数は宣言しない
/// @acf-doc [macro] VLL
#define VLL(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vll, __VA_ARGS__)
// @acf macro VDB — vdb a(n); in(a)
/// @acf-doc @brief `VDB(name)`は個数をintで読んでから要素を読む。`VDB(name, n)`は指定したn要素を読む。型は`vdb`。個数変数は宣言しない
/// @acf-doc [macro] VDB
#define VDB(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vdb, __VA_ARGS__)
// @acf macro VB — vb a(n); in(a)（vector<bool> の特殊化に注意）
/// @acf-doc @brief `VB(name)`は個数をintで読んでから要素を読む。`VB(name, n)`は指定したn要素を読む。型は`vb`。個数変数は宣言しない
/// @acf-doc [macro] VB
#define VB(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vb, __VA_ARGS__)
// @acf macro VC — vc a(n); in(a)
/// @acf-doc @brief `VC(name)`は個数をintで読んでから要素を読む。`VC(name, n)`は指定したn要素を読む。型は`vc`。個数変数は宣言しない
/// @acf-doc [macro] VC
#define VC(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vc, __VA_ARGS__)
// @acf macro VS — vs a(n); in(a)
/// @acf-doc @brief `VS(name)`は個数をintで読んでから要素を読む。`VS(name, n)`は指定したn要素を読む。型は`vs`。個数変数は宣言しない
/// @acf-doc [macro] VS
#define VS(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vs, __VA_ARGS__)
// @acf macro VPII — vpii a(n); in(a)
/// @acf-doc @brief `VPII(name)`は個数をintで読んでから要素を読む。`VPII(name, n)`は指定したn要素を読む。型は`vpii`。個数変数は宣言しない
/// @acf-doc [macro] VPII
#define VPII(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vpii, __VA_ARGS__)
// @acf macro VPLL — vpll a(n); in(a)
/// @acf-doc @brief `VPLL(name)`は個数をintで読んでから要素を読む。`VPLL(name, n)`は指定したn要素を読む。型は`vpll`。個数変数は宣言しない
/// @acf-doc [macro] VPLL
#define VPLL(...) AC_INPUT_VECTOR_SELECT_(__VA_ARGS__, AC_INPUT_VECTOR_2_, AC_INPUT_VECTOR_1_)(vpll, __VA_ARGS__)
// @acf macro ROWS — ROWS(name, n, T1, T2, ...) → auto name = in_vector<tuple<T1,T2,...>>(n)
/// @acf-doc @brief n 行のタプル列を宣言して読む。例: `ROWS(a, n, int, char, ll, string);`。大きい整数は `ll`
/// @acf-doc [macro] ROWS
#define ROWS(name, n, ...) auto name = in_vector<tuple<__VA_ARGS__>>(n)
// @acf macro VVI — vvi a(n, vi(m)); in(a)
/// @acf-doc @brief vvi a(n, vi(m)); in(a)
/// @acf-doc [macro] VVI
#define VVI(name, n, m) vvi name(n, vi(m)); in(name)
// @acf macro VVLL — vvll a(n, vll(m)); in(a)
/// @acf-doc @brief vvll a(n, vll(m)); in(a)
/// @acf-doc [macro] VVLL
#define VVLL(name, n, m) vvll name(n, vll(m)); in(name)

// 短縮名はこの選抜リストのみ。型別名の追加に入力マクロを連動させない。

// @acf macro SI — si を n 個読む
/// @acf-doc @brief si を n 個読む
/// @acf-doc [macro] SI
#define SI(name, n) auto name = in_set<int>(n)
// @acf macro SLL — sll を n 個読む
/// @acf-doc @brief sll を n 個読む
/// @acf-doc [macro] SLL
#define SLL(name, n) auto name = in_set<ll>(n)
// @acf macro BS32 — bs32 宣言 + in（01 文字列、複数可）
/// @acf-doc @brief bs32 宣言 + in（01 文字列。`Int` と同様に複数可。10 進なら `BS32U`）
/// @acf-doc [macro] BS32
#define BS32(...) bs32 __VA_ARGS__; in(__VA_ARGS__)
// @acf macro BS64 — bs64 宣言 + in（01 文字列、複数可）
/// @acf-doc @brief bs64 宣言 + in（01 文字列。`Int` と同様に複数可。10 進なら `BS64UL`）
/// @acf-doc [macro] BS64
#define BS64(...) bs64 __VA_ARGS__; in(__VA_ARGS__)
// @acf macro BS32U — bs32 宣言 + ui から構築（10 進、複数可）
/// @acf-doc @brief bs32 宣言 + ui から構築（10 進。`Int` と同様に複数可。01 文字列なら `BS32`）
/// @acf-doc [macro] BS32U
#define BS32U(...) bs32 __VA_ARGS__; ac_detail::input::assign_bs_from_int<ui>(__VA_ARGS__)
// @acf macro BS64UL — bs64 宣言 + ull から構築（10 進、複数可）
/// @acf-doc @brief bs64 宣言 + ull から構築（10 進。`Int` と同様に複数可。01 文字列なら `BS64`）
/// @acf-doc [macro] BS64UL
#define BS64UL(...) bs64 __VA_ARGS__; ac_detail::input::assign_bs_from_int<ull>(__VA_ARGS__)

// ---------------------------------------------------------------------------
// Selected expression shortcut (returns a value)
// in_vi は既存の頻出名として残す。その他は rd<T>() / in_vector<T>(n)。
// ---------------------------------------------------------------------------

// --- vector ---
// @acf macro in_vi — vi を式として返す
/// @acf-doc @brief vi を式として返す
/// @acf-doc [macro] in_vi
#define in_vi(n) in_vector<int>(n)

// in_bs32u / in_bs64ul は layer 1 の関数（10 進）
