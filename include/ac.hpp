#pragma once
// Library: https://github.com/facedolid/atcoder-headers

// 基本型・軽量補助・共通入出力。専門アルゴリズムや入力糖衣は明示include。

// LOCAL 時は trace.hpp（-Iinclude）。IDE や古いローカル ac.hpp コピーで
// 見つからない場合は何もしないマクロにする（clangdの偽エラーを防ぐ）。
#if defined(LOCAL) && __has_include("trace.hpp")
#include "trace.hpp"
#elif defined(LOCAL) && __has_include(<trace.hpp>)
#include <trace.hpp>
#else
#ifndef trace
#define trace(...) ((void)0)
#endif
#ifndef trace_idx
#define trace_idx(...) ((void)0)
#endif
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <bitset>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <functional>
#include <iomanip>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#if __has_include(<print>)
#include <print>
#endif
#include <queue>
#if __has_include(<ranges>)
#include <ranges>
#endif
#include <set>
#if __has_include(<span>)
#include <span>
#endif
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// 解答側は vector / sort / views などを無修飾で使う。
// 実装補助はac_detailにまとめ、解答側で名前空間を意識せずに使えるようにする。
using namespace std;

// AtCoder Library（デフォルトで有効）。切る: 先に #define AC_NO_ACL
// AtCoder提出では、atc submit が __has_include を外し、#include <atcoder/all> にまとめる。
// ITP* 等のAOJ: ローカルは -DAC_NO_ACL、submit はACLブロック除去（展開すると提出サイズ超過）。
// @ac-acl-begin
#ifndef AC_NO_ACL
#if __has_include(<atcoder/all>)
#include <atcoder/all>
// ACLの公開APIだけを解答側へ公開する。using namespace atcoder は避け、
// internal や将来追加される名前とテンプレート独自APIの衝突を防ぐ。
using atcoder::convolution;
using atcoder::convolution_ll;
using atcoder::crt;
using atcoder::dsu;
using atcoder::dynamic_modint;
using atcoder::fenwick_tree;
using atcoder::floor_sum;
using atcoder::inv_mod;
using atcoder::lazy_segtree;
using atcoder::lcp_array;
using atcoder::mcf_graph;
using atcoder::mf_graph;
using atcoder::modint;
using atcoder::modint1000000007;
using atcoder::modint998244353;
// mod の数値定数と同じ数字で選べる短い別名。
//   mint1000000007 x;  // MOD1000000007
//   mint998244353 y;   // MOD998244353
// @acf using mint1000000007 — ACL modint（法 1,000,000,007）
/// @acf-doc @brief 自動で mod 1,000,000,007 を取る型。定数は `MOD1000000007`
/// @acf-doc [type] mint1000000007
using mint1000000007 = atcoder::modint1000000007;
// @acf using mint998244353 — ACL modint（法 998,244,353）
/// @acf-doc @brief 自動で mod 998,244,353 を取る型。定数は `MOD998244353`
/// @acf-doc [type] mint998244353
using mint998244353 = atcoder::modint998244353;
using atcoder::pow_mod;
using atcoder::scc_graph;
using atcoder::segtree;
using atcoder::static_modint;
using atcoder::suffix_array;
using atcoder::two_sat;
using atcoder::z_algorithm;
#define AC_HAS_ACL 1
#endif
#endif
// @ac-acl-end

// @acf using ll — long long
/// @acf-doc @brief long long
/// @acf-doc [type] ll
using ll = long long;
// @acf using ull — unsigned long long
/// @acf-doc @brief unsigned long long
/// @acf-doc [type] ull
using ull = unsigned long long;
// @acf using i128 — __int128
/// @acf-doc @brief `__int128`（GCC/Clang）。64bitを超える整数。`in` / `print` 対応
/// @acf-doc [type] i128
using i128 = __int128_t;
// @acf using u128 — unsigned __int128
/// @acf-doc @brief `unsigned __int128`（GCC/Clang）。`in` / `print` 対応
/// @acf-doc [type] u128
using u128 = __uint128_t;
// @acf using ui — unsigned int
/// @acf-doc @brief unsigned int
/// @acf-doc [type] ui
using ui = unsigned int;
// @acf using bs — bitset<N>
/// @acf-doc @brief bitset<N>（長さ N は型の一部。例: bs<64>）
/// @acf-doc [type] bs
template <size_t N>
using bs = bitset<N>;
// @acf using bs32 — bitset<32>
/// @acf-doc @brief bitset<32>
/// @acf-doc [type] bs32
using bs32 = bitset<32>;
// @acf using bs64 — bitset<64>
/// @acf-doc @brief bitset<64>
/// @acf-doc [type] bs64
using bs64 = bitset<64>;
// @acf using db — double
/// @acf-doc @brief double
/// @acf-doc [type] db
using db = double;
// @acf using ld — long double
/// @acf-doc @brief long double
/// @acf-doc [type] ld
using ld = long double;
// @acf using str — string
/// @acf-doc @brief string
/// @acf-doc [type] str
using str = string;
// @acf using vi — vector<int>
/// @acf-doc @brief vector<int>
/// @acf-doc [type] vi
using vi = vector<int>;
// @acf using vll — vector<ll>
/// @acf-doc @brief vector<ll>
/// @acf-doc [type] vll
using vll = vector<ll>;
// @acf using vdb — vector<db>
/// @acf-doc @brief vector<db>
/// @acf-doc [type] vdb
using vdb = vector<db>;
// @acf using vb — vector<bool>
/// @acf-doc @brief vector<bool>
/// @acf-doc [type] vb
using vb = vector<bool>;
// @acf using vc — vector<char>
/// @acf-doc @brief vector<char>
/// @acf-doc [type] vc
using vc = vector<char>;
// @acf using vs — vector<str>
/// @acf-doc @brief vector<str>
/// @acf-doc [type] vs
using vs = vector<str>;
// @acf using vvi — vector<vector<int>>
/// @acf-doc @brief vector<vector<int>>
/// @acf-doc [type] vvi
using vvi = vector<vi>;
// @acf using vvll — vector<vector<ll>>
/// @acf-doc @brief vector<vector<ll>>
/// @acf-doc [type] vvll
using vvll = vector<vll>;
// @acf using pii — pair<int,int>
/// @acf-doc @brief pair<int,int>
/// @acf-doc [type] pii
using pii = pair<int, int>;
// @acf using pll — pair<ll,ll>
/// @acf-doc @brief pair<ll,ll>
/// @acf-doc [type] pll
using pll = pair<ll, ll>;
// @acf using pss — pair<str,str>
/// @acf-doc @brief pair<str,str>
/// @acf-doc [type] pss
using pss = pair<str, str>;
// @acf using vpii — vector<pii>
/// @acf-doc @brief vector<pii>
/// @acf-doc [type] vpii
using vpii = vector<pii>;
// @acf using vpll — vector<pll>
/// @acf-doc @brief vector<pll>
/// @acf-doc [type] vpll
using vpll = vector<pll>;

namespace ac_detail {
template <class T>
struct type_identity {
    using type = T;
};

template <class T>
constexpr auto wide_type_tag() {
    using U = remove_cv_t<T>;
    if constexpr (!is_integral_v<U> || is_same_v<U, bool>) {
        return type_identity<U>{};
    } else if constexpr (sizeof(U) <= 4) {
        if constexpr (is_signed_v<U>) return type_identity<ll>{};
        else return type_identity<ull>{};
    } else if constexpr (sizeof(U) <= 8) {
        if constexpr (is_signed_v<U>) return type_identity<i128>{};
        else return type_identity<u128>{};
    } else {
        return type_identity<U>{};
    }
}

// int など32bit以下だけ ll/ull へ。ll / i128 はそのまま（幾何APIの戻り用）
template <class T>
constexpr auto product_result_tag() {
    using U = remove_cv_t<T>;
    if constexpr (!is_integral_v<U> || is_same_v<U, bool>) {
        return type_identity<U>{};
    } else if constexpr (sizeof(U) <= 4) {
        if constexpr (is_signed_v<U>) return type_identity<ll>{};
        else return type_identity<ull>{};
    } else {
        return type_identity<U>{};
    }
}
}  // namespace ac_detail

// @acf using wide_t — 整数積向けに一段広い型（int→ll、ll→i128）
/// @acf-doc @brief 整数なら一段広い型。`int`→`ll`、`ll`→`i128`、非整数はそのまま
/// @acf-doc [type] wide_t
template <class T>
using wide_t = typename decltype(ac_detail::wide_type_tag<T>())::type;

// @acf using product_type_t — 広い積の型（整数は wide_t<common_type>）
/// @acf-doc @brief 明示的に広い積の型。整数なら `wide_t<common_type_t<...>>`（`ll`→`i128`）、それ以外は `common_type_t`。`wide_mul` 用
/// @acf-doc [type] product_type_t
template <class... Ts>
using product_type_t = conditional_t<(is_integral_v<remove_cv_t<Ts>> && ...),
                                     wide_t<common_type_t<Ts...>>,
                                     common_type_t<Ts...>>;

// @acf using product_result_t — 幾何などの積APIの戻り型（int→ll、llはそのまま）
/// @acf-doc @brief 積APIの戻り型。整数は `int`→`ll`、`ll`/`i128` はそのまま。中間計算はより広い型で行い、最後に戻り型へ変換する
/// @acf-doc [type] product_result_t
template <class... Ts>
using product_result_t =
    conditional_t<(is_integral_v<remove_cv_t<Ts>> && ...),
                  typename decltype(ac_detail::product_result_tag<
                      common_type_t<Ts...>>())::type,
                  common_type_t<Ts...>>;

// @acf fn wide_mul — (a,b) -> product_type_t — オーバーフローしにくい掛け算（戻りも広げる）
/// @acf-doc @brief `product_type_t` へキャストしてから掛ける（`int`→`ll`、`ll`→`i128`）。広い積が必要なとき用。幾何の戻りは `product_result_t`
/// @acf-doc [function] wide_mul
template <class A, class B>
constexpr auto wide_mul(A a, B b) {
    using R = product_type_t<A, B>;
    return static_cast<R>(a) * static_cast<R>(b);
}

namespace ac_detail {
template <class... Ts, class W>
constexpr auto as_product_result(W value) {
    return static_cast<product_result_t<Ts...>>(value);
}

inline ll checked_ll(i128 value, const char* operation) {
    if (value < static_cast<i128>(numeric_limits<ll>::min()) ||
        value > static_cast<i128>(numeric_limits<ll>::max())) {
        throw overflow_error(string(operation) + ": result does not fit in long long");
    }
    return static_cast<ll>(value);
}

inline i128 checked_add_i128(i128 a, i128 b, const char* operation) {
    i128 result;
    if (__builtin_add_overflow(a, b, &result)) {
        throw overflow_error(string(operation) + ": intermediate value exceeds i128");
    }
    return result;
}

inline i128 checked_mul_i128(i128 a, i128 b, const char* operation) {
    i128 result;
    if (__builtin_mul_overflow(a, b, &result)) {
        throw overflow_error(string(operation) + ": intermediate value exceeds i128");
    }
    return result;
}
}  // namespace ac_detail

// @acf fn mul_mod — (ll a, ll b, ll m) -> ll / (Mint a, b) -> Mint — mod 上の掛け算
/// @acf-doc @brief 整数版は `(a*b)%m` を i128 経由で返す。modint版は型が持つ法を使うため第3引数不要
/// @acf-doc @code (ll a, ll b, ll m) -> ll / (Mint a, b) -> Mint
/// @acf-doc [function] mul_mod
inline ll mul_mod(ll a, ll b, ll m) {
    if (m <= 0) throw invalid_argument("mul_mod: mod must be positive");
    a %= m;
    if (a < 0) a += m;
    b %= m;
    if (b < 0) b += m;
    return static_cast<ll>(static_cast<i128>(a) * b % m);
}

// @acf fn wrap0 — (ll x, ll n) -> ll — xを0-indexの循環範囲 [0,n) へ正規化
/// @acf-doc @brief `n > 0`。負数を含む `x` を周期 `n` の範囲 `0..n-1` に正規化する。`n <= 0` は `invalid_argument`
/// @acf-doc @code (ll x, ll n) -> ll
/// @acf-doc [function] wrap0
constexpr ll wrap0(ll x, ll n) {
    if (n <= 0) throw invalid_argument("wrap0: n must be positive");
    const ll r = x % n;
    return r < 0 ? r + n : r;
}

// @acf fn wrap1 — (ll x, ll n) -> ll — xを1-indexの循環範囲 [1,n] へ正規化
/// @acf-doc @brief `n > 0`。負数を含む `x` を周期 `n` の範囲 `1..n` に正規化する。`n <= 0` は `invalid_argument`
/// @acf-doc @code (ll x, ll n) -> ll
/// @acf-doc [function] wrap1
constexpr ll wrap1(ll x, ll n) {
    if (n <= 0) throw invalid_argument("wrap1: n must be positive");
    const ll r = x % n;
    return r <= 0 ? r + n : r;
}

namespace ac_detail::int128_io {
inline bool parse_u128(const string& s, size_t begin, u128& value) {
    if (begin >= s.size()) return false;
    u128 x = 0;
    for (size_t i = begin; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        x = x * 10 + static_cast<u128>(s[i] - '0');
    }
    value = x;
    return true;
}

inline void write_u128(ostream& os, u128 value) {
    if (value == 0) {
        os << '0';
        return;
    }
    char buf[64];
    int n = 0;
    while (value > 0) {
        buf[n++] = static_cast<char>('0' + static_cast<int>(value % 10));
        value /= 10;
    }
    while (n > 0) os << buf[--n];
}
}  // namespace ac_detail::int128_io

// @acf helper operator>> — i128 / u128 の10進入力
/// @acf-doc @brief `__int128` を10進文字列として読む。`in` / `rd` / `print` からも使える
/// @acf-doc [helper] operator>>
namespace std {
inline istream& operator>>(istream& is, __int128& value) {
    string s;
    if (!(is >> s)) return is;
    size_t begin = 0;
    bool negative = false;
    if (!s.empty() && (s[0] == '+' || s[0] == '-')) {
        negative = s[0] == '-';
        begin = 1;
    }
    ::u128 mag = 0;
    if (!::ac_detail::int128_io::parse_u128(s, begin, mag)) {
        is.setstate(ios::failbit);
        return is;
    }
    value = negative ? -static_cast<__int128>(mag) : static_cast<__int128>(mag);
    return is;
}

inline istream& operator>>(istream& is, unsigned __int128& value) {
    string s;
    if (!(is >> s)) return is;
    if (!s.empty() && s[0] == '+') s.erase(s.begin());
    if (!::ac_detail::int128_io::parse_u128(s, 0, value)) is.setstate(ios::failbit);
    return is;
}

// @acf helper operator<< — i128 / u128 の10進出力
/// @acf-doc @brief `__int128` を10進で書く。`print` からも使える
/// @acf-doc [helper] operator<<
inline ostream& operator<<(ostream& os, __int128 value) {
    if (value < 0) {
        os << '-';
        ::ac_detail::int128_io::write_u128(
            os, static_cast<::u128>(-(value + 1)) + 1);
        return os;
    }
    ::ac_detail::int128_io::write_u128(os, static_cast<::u128>(value));
    return os;
}

inline ostream& operator<<(ostream& os, unsigned __int128 value) {
    ::ac_detail::int128_io::write_u128(os, value);
    return os;
}
}  // namespace std

// @acf using mii — map<int,int>
/// @acf-doc @brief map<int,int>
/// @acf-doc [type] mii
using mii = map<int, int>;
// @acf using mll — map<ll,ll>
/// @acf-doc @brief map<ll,ll>
/// @acf-doc [type] mll
using mll = map<ll, ll>;
// @acf using msi — map<str,int>
/// @acf-doc @brief map<str,int>
/// @acf-doc [type] msi
using msi = map<str, int>;
// @acf using si — set<int>
/// @acf-doc @brief set<int>
/// @acf-doc [type] si
using si = set<int>;
// @acf using sll — set<ll>
/// @acf-doc @brief set<ll>
/// @acf-doc [type] sll
using sll = set<ll>;
// @acf using maxpq — priority_queue<T>（最大ヒープ）
/// @acf-doc @brief priority_queue<T>（最大ヒープ）
/// @acf-doc [type] maxpq
template <class T>
using maxpq = priority_queue<T>;
// @acf using minpq — priority_queue<T,vector<T>,greater<T>>（最小ヒープ）
/// @acf-doc @brief priority_queue<T,vector<T>,greater<T>>（最小ヒープ）
/// @acf-doc [type] minpq
template <class T>
using minpq = priority_queue<T, vector<T>, greater<T>>;

// set と同じ同値判定で重複をまとめる優先度付きキュー。
// Compareはpriority_queueと同じ比較順。less<T>で最大、greater<T>で最小を先に取り出す。
// push/emplace/contains/erase: O(log N)、top/size/empty: O(1)、pop: 償却 O(1)。
template <class T, class Compare = less<T>>
class setpq {
    set<T, Compare> values_;

   public:
    using value_type = T;
    using size_type = typename set<T, Compare>::size_type;

    setpq() = default;
    explicit setpq(const Compare& comp) : values_(comp) {}

    bool empty() const { return values_.empty(); }
    size_type size() const { return values_.size(); }
    const T& top() const {
#ifdef LOCAL
        assert(!empty());
#endif
        return *values_.rbegin();
    }
    bool push(const T& value) { return values_.insert(value).second; }
    bool push(T&& value) { return values_.insert(std::move(value)).second; }
    template <class... Args>
    bool emplace(Args&&... args) {
        return values_.emplace(std::forward<Args>(args)...).second;
    }
    void pop() {
#ifdef LOCAL
        assert(!empty());
#endif
        values_.erase(prev(values_.end()));
    }
    bool contains(const T& value) const { return values_.contains(value); }
    bool erase(const T& value) { return values_.erase(value) != 0; }
    void clear() { values_.clear(); }
};

// @acf const INF — 距離 DP 用の十分大きい値
/// @acf-doc @brief 距離DP用の十分大きい値
/// @acf-doc [constant] INF
constexpr ll INF = (1LL << 60) - 1;
// @acf macro NPOS — string::npos
/// @acf-doc @brief `string::npos`。文字列検索で見つからなかった位置を表す
/// @acf-doc [macro] NPOS
#define NPOS string::npos
// @acf const EPS — 浮動小数比較の既定許容誤差（1e-12）
/// @acf-doc @brief 浮動小数の絶対誤差比較で使う既定値。`eq` / `sgn` の第3・第2引数の既定
/// @acf-doc [constant] EPS
constexpr ld EPS = 1e-12L;
// @acf const PI — 円周率（long double）
/// @acf-doc @brief 円周率。ラジアン計算・`to_rad` / `to_deg` で使う
/// @acf-doc [constant] PI
constexpr ld PI = 3.141592653589793238462643383279502884L;
// @acf const MOD1000000007 — mod 1,000,000,007（mint1000000007 と対応）
/// @acf-doc @brief mod 1,000,000,007。ACL型は `mint1000000007`
/// @acf-doc [constant] MOD1000000007
constexpr int MOD1000000007 = 1'000'000'007;
// @acf const MOD998244353 — mod 998,244,353（mint998244353 と対応）
/// @acf-doc @brief mod 998,244,353。ACL型は `mint998244353`
/// @acf-doc [constant] MOD998244353
constexpr int MOD998244353 = 998'244'353;
// @acf const O — 「丸」を表す小文字の char
/// @acf-doc @brief グリッドなどで使う「丸」の小文字 `o`
/// @acf-doc [constant] O
constexpr char O = 'o';
// @acf const X — 「罰」を表す小文字の char
/// @acf-doc @brief グリッドなどで使う「罰」の小文字 `x`
/// @acf-doc [constant] X
constexpr char X = 'x';
// @acf const ALPHABET_LOWER — 小文字a〜zを順に並べた26文字のstring
/// @acf-doc @brief `inline const str`。内容は `abcdefghijklmnopqrstuvwxyz`。`str s = ALPHABET_LOWER;` で編集用にコピーできる
/// @acf-doc [constant] ALPHABET_LOWER
inline const str ALPHABET_LOWER = "abcdefghijklmnopqrstuvwxyz";
// @acf const ALPHABET_UPPER — 大文字A〜Zを順に並べた26文字のstring
/// @acf-doc @brief `inline const str`。内容は `ABCDEFGHIJKLMNOPQRSTUVWXYZ`。`str s = ALPHABET_UPPER;` で編集用にコピーできる
/// @acf-doc [constant] ALPHABET_UPPER
inline const str ALPHABET_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// @acf const kuku — 1〜9の積を格納した9×9の九九表
/// @acf-doc @brief `kuku[i][j] == (i+1)*(j+1)`。添字は0〜8、値はint。読み取り専用の固定長配列で、動的確保なし。`auto table = kuku;`で変更可能なコピーを作る
/// @acf-doc [constant] kuku
inline constexpr array<array<int, 9>, 9> kuku = [] {
    array<array<int, 9>, 9> table{};
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            table[i][j] = (i + 1) * (j + 1);
    return table;
}();

// @acf const dx4 — 4近傍の行方向差分（r + dx4[d]）
/// @acf-doc @brief 4近傍の行方向差分。`r + dx4[d]`
/// @acf-doc [constant] dx4
inline constexpr array<int, 4> dx4 = {1, 0, -1, 0};
// @acf const dy4 — 4近傍の列方向差分（c + dy4[d]）
/// @acf-doc @brief 4近傍の列方向差分。`c + dy4[d]`
/// @acf-doc [constant] dy4
inline constexpr array<int, 4> dy4 = {0, 1, 0, -1};
// @acf const dx8 — 8近傍の行方向差分（r + dx8[d]）
/// @acf-doc @brief 8近傍の行方向差分。`r + dx8[d]`
/// @acf-doc [constant] dx8
inline constexpr array<int, 8> dx8 = {1, 1, 0, -1, -1, -1, 0, 1};
// @acf const dy8 — 8近傍の列方向差分（c + dy8[d]）
/// @acf-doc @brief 8近傍の列方向差分。`c + dy8[d]`
/// @acf-doc [constant] dy8
inline constexpr array<int, 8> dy8 = {0, 1, 1, 1, 0, -1, -1, -1};

namespace ac_detail {
template <class F>
class recursive_callable {
    F function_;

   public:
    explicit recursive_callable(F function) : function_(std::move(function)) {}

    template <class... Args>
    decltype(auto) operator()(Args&&... args) {
        return std::invoke(function_, *this, std::forward<Args>(args)...);
    }

    template <class... Args>
    decltype(auto) operator()(Args&&... args) const {
        return std::invoke(function_, *this, std::forward<Args>(args)...);
    }
};

struct memo_tuple_hash {
    template <class... Ts>
    /// @brief 引数tupleをunordered_map用のハッシュ値へ変換する。
    size_t operator()(const tuple<Ts...>& key) const {
        size_t seed = 0;
        apply(
            [&](const auto&... values) {
                ((seed ^= hash<decay_t<decltype(values)>>{}(values) +
                           0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2)),
                 ...);
            },
            key);
        return seed;
    }
};

template <class F, class Signature>
class memoized;

template <class F, class R, class... Args>
class memoized<F, R(Args...)> {
    static_assert(!is_void_v<R>, "memoize requires a non-void return type");
    static_assert(is_copy_constructible_v<R>, "memoize requires a copyable return type");

    using key_type = tuple<decay_t<Args>...>;
    F function_;
    unordered_map<key_type, R, memo_tuple_hash> cache_;

   public:
    /// @brief メモ化対象の関数オブジェクトを登録する。
    explicit memoized(F function) : function_(std::move(function)) {}

    /// @brief 引数に対応する結果をキャッシュから返し、未計算なら計算して保存する。
    R operator()(Args... args) {
        key_type key(args...);
        if (auto it = cache_.find(key); it != cache_.end()) return it->second;

        R result = std::invoke(function_, *this, args...);
        auto [it, inserted] = cache_.emplace(std::move(key), std::move(result));
        (void)inserted;
        return it->second;
    }

    /// @brief 保存済みのメモ化結果をすべて削除する。
    void clear() { cache_.clear(); }

    /// @brief キャッシュの容量をあらかじめ確保する。
    void reserve(size_t state_count) { cache_.reserve(state_count); }

    /// @brief 保存済みのメモ化結果の個数を返す。
    size_t cache_size() const { return cache_.size(); }
};
}  // namespace ac_detail

// @acf fn recur — (F function) -> callable — 自己参照を受け取る再帰 callable を作る
/// @acf-doc @brief `recur(lambda)` の第1引数 `self` から自分自身を呼べる再帰 callable を作る。状態・枝刈り・戻り値は lambda 側で定義する
/// @acf-doc @code auto dfs = recur([](auto& self, int state) -> void { ... self(next_state); });
/// @acf-doc [function] recur
template <class F>
auto recur(F&& function) {
    return ac_detail::recursive_callable<std::decay_t<F>>(std::forward<F>(function));
}

// @acf fn memoize — Python の @cache 相当のメモ化再帰
/// @acf-doc @brief `memoize<R(Args...)>(lambda)`。lambda の第1引数で自分を再帰呼出しし、引数 tuple ごとに結果を保存
/// @acf-doc @code auto f = memoize<ll(int)>([](auto& f, int n) -> ll { ... });
/// @acf-doc [function] memoize
template <class Signature, class F>
auto memoize(F&& function) {
    return ac_detail::memoized<std::decay_t<F>, Signature>(std::forward<F>(function));
}

namespace ac_detail::iteration {
struct integer_loop_bounds {
    ll first;
    ll last;
    bool active;
};

template <class N>
ll count_or_size(N&& value) {
    if constexpr (std::ranges::sized_range<N>) {
        return static_cast<ll>(std::ranges::size(value));
    } else {
        return static_cast<ll>(value);
    }
}

// 端点を増減する前に空区間を判定し、LLONG_MIN/MAX でも溢れないようにする。
inline integer_loop_bounds integer_bounds(ll lo, ll hi, bool include_lo,
                                          bool include_hi, bool reverse) {
    if (lo > hi || (!include_lo && lo == numeric_limits<ll>::max()) ||
        (!include_hi && hi == numeric_limits<ll>::min())) return {0, 0, false};
    if (!include_lo) ++lo;
    if (!include_hi) --hi;
    return reverse ? integer_loop_bounds{hi, lo, lo <= hi}
                   : integer_loop_bounds{lo, hi, lo <= hi};
}

inline void advance_integer(ll& value, ll last, bool& active, bool reverse) {
    if (value == last ||
        value == (reverse ? numeric_limits<ll>::min() : numeric_limits<ll>::max())) {
        active = false;
    } else if (reverse) {
        --value;
    } else {
        ++value;
    }
}
}  // namespace ac_detail::iteration

// repとCOLLECTで使う、丸括弧で囲んだ分解変数の判定。
#define AC_COLLECT_CHECK_N_(x, n, ...) n
#define AC_COLLECT_CHECK_(...) AC_COLLECT_CHECK_N_(__VA_ARGS__, 0, )
#define AC_COLLECT_PROBE_(...) ~, 1,
#define AC_COLLECT_IS_TUPLE_(names) AC_COLLECT_CHECK_(AC_COLLECT_PROBE_ names)
#define AC_COLLECT_CAT_I_(a, b) a##b
#define AC_COLLECT_CAT_(a, b) AC_COLLECT_CAT_I_(a, b)
#define AC_COLLECT_UNPACK_(...) __VA_ARGS__

#define AC_REP_JOIN_IMPL_(a, b) a##b
#define AC_REP_JOIN_(a, b) AC_REP_JOIN_IMPL_(a, b)
#define AC_REP_LOOP_ID_(id, i, lo, hi, include_lo, include_hi, reverse) \
    for (auto [i, AC_REP_JOIN_(ac_rep_last_, id), AC_REP_JOIN_(ac_rep_active_, id)] = \
             ac_detail::iteration::integer_bounds((lo), (hi), (include_lo), (include_hi), (reverse)); \
         AC_REP_JOIN_(ac_rep_active_, id) && \
             ((reverse) ? i >= AC_REP_JOIN_(ac_rep_last_, id) : i <= AC_REP_JOIN_(ac_rep_last_, id)); \
         ac_detail::iteration::advance_integer(i, AC_REP_JOIN_(ac_rep_last_, id), \
                                              AC_REP_JOIN_(ac_rep_active_, id), (reverse)))
#define AC_REP_LOOP_(...) AC_REP_LOOP_ID_(__COUNTER__, __VA_ARGS__)
#define AC_REP_SELECT_(_1, _2, _3, _4, _5, NAME, ...) NAME
#define AC_REP_BAD_ARITY_(...) static_assert(false, "rep/rrep requires 2, 3 or 5 arguments")

// @acf macro rep — rep(i,n) / rep(i,range) / rep(i,lo,hi) / rep(i,lo,range) / rep(i,lo,hi,include_lo,include_hi): 昇順
/// @acf-doc @brief 変数を先頭に指定。2引数は整数なら [0,n)、サイズ付きrangeなら [0,size(range))。3引数の上限もサイズ付きrangeを渡せる。5引数で端点を指定。境界・rangeのサイズは開始時に1回評価。iはll。rep((k,v),range)はconst auto&の構造化束縛で要素を走査し、range式は1回評価
/// @acf-doc [macro] rep
#define AC_REP_2_0(i, n) rep_3(i, 0, n)
#define AC_REP_2_1(names, range) for (const auto& [AC_COLLECT_UNPACK_ names] : (range))
#define rep_2(i, n) AC_REP_JOIN_(AC_REP_2_, AC_COLLECT_IS_TUPLE_(i))(i, n)
#define rep_3(i, lo, hi) rep_5(i, lo, ac_detail::iteration::count_or_size(hi), true, false)
#define rep_5(i, lo, hi, include_lo, include_hi) AC_REP_LOOP_(i, lo, hi, include_lo, include_hi, false)
#define rep(...) AC_REP_SELECT_(__VA_ARGS__, rep_5, AC_REP_BAD_ARITY_, rep_3, rep_2, AC_REP_BAD_ARITY_)(__VA_ARGS__)
// @acf macro rrep — rrep(i,n) / rrep(i,range) / rrep(i,lo,hi) / rrep(i,lo,range) / rrep(i,lo,hi,include_lo,include_hi): 同じ区間を逆順
/// @acf-doc @brief repと同じ引数順・端点指定で、走査順だけを逆にする。2引数のrangeは size(range)-1..0、3引数の上限にもサイズ付きrangeを渡せる。iはll
/// @acf-doc [macro] rrep
#define rrep_2(i, n) rrep_3(i, 0, n)
#define rrep_3(i, lo, hi) rrep_5(i, lo, ac_detail::iteration::count_or_size(hi), true, false)
#define rrep_5(i, lo, hi, include_lo, include_hi) AC_REP_LOOP_(i, lo, hi, include_lo, include_hi, true)
#define rrep(...) AC_REP_SELECT_(__VA_ARGS__, rrep_5, AC_REP_BAD_ARITY_, rrep_3, rrep_2, AC_REP_BAD_ARITY_)(__VA_ARGS__)
// イテレータの2/3引数ディスパッチ。整数ループの2/3/5引数とは分離する。
#define rep_select(_1, _2, _3, NAME, ...) NAME
// @acf macro repi — repi(n): int n を宣言+in して n 回 / repi(i,n): ループ変数 i 付き
/// @acf-doc @brief repi(n): int n を宣言+in して n 回 / repi(i,n): ループ変数 i 付き
/// @acf-doc [macro] repi
#define repi_1(n) for (int n = rd<int>(), _ = 0; _ < n; ++_)
#define repi_2(i, n) for (int n = rd<int>(), i = 0; i < n; ++i)
#define repi_select(_1, _2, NAME, ...) NAME
#define repi(...) repi_select(__VA_ARGS__, repi_2, repi_1)(__VA_ARGS__)  ///< @acf-doc repi(n) / repi(i,n): 宣言+in して n 回ループ
// @acf macro repl — repl(n): ll n を宣言+in して n 回 / repl(i,n): ループ変数 i 付き
/// @acf-doc @brief repl(n): ll n を宣言+in して n 回 / repl(i,n): ループ変数 i 付き
/// @acf-doc [macro] repl
#define repl_1(n) for (ll n = rd<ll>(), _ = 0; _ < n; ++_)
#define repl_2(i, n) for (ll n = rd<ll>(), i = 0; i < n; ++i)
#define repl(...) repi_select(__VA_ARGS__, repl_2, repl_1)(__VA_ARGS__)  ///< @acf-doc repl(n) / repl(i,n): 宣言+in して n 回ループ
// @acf macro repit — repit(it,r): コンテナ or equal_range の pair / repit(it,a,b): it∈[a,b)
/// @acf-doc @brief repit(it,r): begin..end または equal_range 等の pair{first,second} / repit(it,a,b): it∈[a,b)
/// @acf-doc [macro] repit
namespace ac_detail::iteration {
template <class R>
auto bounds(R&& r, int) -> decltype(std::pair(begin(r), end(r))) {
    return {begin(r), end(r)};
}
template <class R>
auto bounds(R&& r, long) -> decltype(std::pair(r.first, r.second)) {
    return {r.first, r.second};
}
template <class R>
auto bounds(R&& r) {
    return bounds(std::forward<R>(r), 0);
}
}  // namespace ac_detail::iteration
#define repit_2(it, r) \
    for (auto [it, _repit_end_] = ac_detail::iteration::bounds(r); it != _repit_end_; ++it)
#define repit_3(it, a, b) for (auto it = (a), _repit_end_ = (b); it != _repit_end_; ++it)
#define repit(...) rep_select(__VA_ARGS__, repit_3, repit_2)(__VA_ARGS__)  ///< @acf-doc repit(it,r) / repit(it,a,b): イテレータ走査
// @acf macro cauto — const auto
/// @acf-doc @brief `const auto` の短縮。`for (cauto& x : v)` / `cauto& [a,b] : edges` など
/// @acf-doc [macro] cauto
#define cauto const auto
// @acf macro ALL — (v).begin(), (v).end()
/// @acf-doc @brief (v).begin(), (v).end()
/// @acf-doc [macro] ALL
#define ALL(v) (v).begin(), (v).end()  ///< @acf-doc (v).begin(), (v).end()
// @acf macro RALL — (v).rbegin(), (v).rend()
/// @acf-doc @brief 逆順 iterator の組 `(v).rbegin(), (v).rend()`。降順 sort などに使う
/// @acf-doc [macro] RALL
#define RALL(v) (v).rbegin(), (v).rend()
// @acf macro ALL_EQ — 全引数が等しければ true（本体は all_equal）
/// @acf-doc @brief `ALL_EQ(a,b,c,...)` は全引数が等しいか判定する。各実引数は一度だけ評価。0個・1個は true
/// @acf-doc [macro] ALL_EQ
#define ALL_EQ(...) all_equal(__VA_ARGS__)
// @acf macro sz — (int)(v).size()
/// @acf-doc @brief (int)(v).size()
/// @acf-doc [macro] sz
#define sz(v) ((int)(v).size())  ///< @acf-doc (int)(v).size()
// @acf macro cont_if — cont_if(cond): cond が true なら continue
/// @acf-doc @brief `cont_if(cond);` → `if (cond) continue;`（`continue` は式にできないので文マクロ）
/// @acf-doc [macro] cont_if
#define cont_if(...) if (__VA_ARGS__) { continue; } else (void)0
// @acf macro break_if — break_if(cond): cond が true なら break
/// @acf-doc @brief `break_if(cond);` → `if (cond) break;`（`break` は式にできないので文マクロ）
/// @acf-doc [macro] break_if
#define break_if(...) if (__VA_ARGS__) { break; } else (void)0
// @acf macro ret_if — ret_if(cond): cond が true なら return / ret_if(cond, val): return val
/// @acf-doc @brief `ret_if(cond);` → `if (cond) return;` / `ret_if(cond, val);` → `if (cond) return val;`（`return` は式にできないので文マクロ）
/// @acf-doc [macro] ret_if
#define ret_if(cond, ...) if (cond) { return __VA_ARGS__; } else (void)0

// ---------------------------------------------------------------------------
// Shared I/O traits (Clang-safe extract needs overloads declared before use).
// ---------------------------------------------------------------------------
namespace ac_detail::io {
// Prefer C++23 locally / on AtCoder. Traits stay C++17-safe for older judges (e.g. ITP2 C++17).
template <class T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

template <class T>
inline constexpr bool is_char_v =
    is_same_v<T, char> || is_same_v<T, signed char> || is_same_v<T, unsigned char> ||
    is_same_v<T, wchar_t> || is_same_v<T, char16_t> || is_same_v<T, char32_t>
#ifdef __cpp_char8_t
    || is_same_v<T, char8_t>
#endif
    ;

// bool値を代入してcharに入った0/1は、デバッグ出力でfalse/trueと表示する。
template <class Os>
void write_char_as_display(Os& os, char c) {
    if (c == 0) {
        os << "false";
    } else if (c == 1) {
        os << "true";
    } else {
        os << c;
    }
}

template <class T>
inline constexpr bool is_string_like_v =
    is_same_v<T, string> || is_same_v<T, string_view> || is_same_v<T, const char*> ||
    is_same_v<T, char*> || (is_array_v<T> && is_char_v<remove_extent_t<T>>);

template <class T, class = void>
struct is_range : false_type {};

template <class T>
struct is_range<
    T,
    void_t<decltype(std::begin(declval<const T&>())), decltype(std::end(declval<const T&>()))>>
    : bool_constant<!is_string_like_v<remove_cvref_t<T>>> {};

template <class T>
inline constexpr bool is_range_v = is_range<T>::value;

// print(range, sep) の sep 候補。可変長引数 print より range+sep を優先するため。
template <class T>
struct is_range_sep_arg : bool_constant<is_same_v<T, char> || is_same_v<T, string_view> ||
                                         is_same_v<T, const char*> || is_same_v<T, char*> ||
                                         (is_array_v<T> && is_char_v<remove_extent_t<T>>)> {};
template <class T>
inline constexpr bool is_range_sep_arg_v = is_range_sep_arg<remove_cvref_t<T>>::value;

template <class... Tail>
struct is_optional_nl_tail : false_type {};
template <>
struct is_optional_nl_tail<> : true_type {};
template <class B>
struct is_optional_nl_tail<B> : bool_constant<is_same_v<remove_cvref_t<B>, bool>> {};

template <class A, class B, class... Rest>
inline constexpr bool prefer_range_sep_print_v =
    is_range_v<A> && is_range_sep_arg_v<B> && is_optional_nl_tail<Rest...>::value;

template <class T>
struct is_pair : false_type {};
template <class A, class B>
struct is_pair<pair<A, B>> : true_type {};
template <class T>
inline constexpr bool is_pair_v = is_pair<remove_cvref_t<T>>::value;

template <class T>
struct is_tuple : false_type {};
template <class... Ts>
struct is_tuple<tuple<Ts...>> : true_type {};
template <class T>
inline constexpr bool is_tuple_v = is_tuple<remove_cvref_t<T>>::value;

template <class T>
struct is_bitset : false_type {};
template <size_t N>
struct is_bitset<bitset<N>> : true_type {};
template <class T>
inline constexpr bool is_bitset_v = is_bitset<remove_cvref_t<T>>::value;

// ACL modint は stream 演算子を持たないため、入出力層で整数と同様に扱う。
template <class T>
struct is_modint : false_type {};
#ifdef AC_HAS_ACL
template <int Mod>
struct is_modint<atcoder::static_modint<Mod>> : true_type {};
template <int Id>
struct is_modint<atcoder::dynamic_modint<Id>> : true_type {};
#endif
template <class T>
inline constexpr bool is_modint_v = is_modint<remove_cvref_t<T>>::value;

// Nested range/pair/tuple/bitset elements use newline as the natural separator.
template <class T>
struct is_multiline_elem
    : bool_constant<is_range_v<T> || is_pair_v<T> || is_tuple_v<T> || is_bitset_v<T>> {};
template <class T>
inline constexpr bool is_multiline_elem_v = is_multiline_elem<T>::value;

template <class T>
struct natural_range_sep {
    static constexpr char value = is_multiline_elem_v<T> ? '\n' : ' ';
};
template <class T>
inline constexpr char natural_range_sep_v = natural_range_sep<T>::value;

template <class T, class = void>
struct is_mutable_range : false_type {};

template <class T>
struct is_mutable_range<
    T,
    enable_if_t<
        is_range_v<T> && is_assignable_v<decltype(*std::begin(declval<T&>())),
                                         decltype(*std::begin(declval<T&>()))>>> : true_type {};

template <class T>
inline constexpr bool is_mutable_range_v = is_mutable_range<T>::value;
}  // namespace ac_detail::io

#ifdef AC_HAS_ACL
// @acf fn mint_cast — ACL modint と整数を明示的に相互変換
/// @acf-doc @brief `mint_cast<mint1000000007>(n)` で整数からmodintへ、`mint_cast<ll>(x)` でmodintの代表値へ変換
/// @acf-doc @code auto x=mint_cast<mint1000000007>(-1LL); ll v=mint_cast<ll>(x)
/// @acf-doc [function] mint_cast
template <class To, class From,
          enable_if_t<ac_detail::io::is_modint_v<To> &&
                          is_integral_v<remove_cvref_t<From>>,
                      int> = 0>
To mint_cast(From value) {
    return To(value);
}

template <class To, class Mint,
          enable_if_t<is_integral_v<remove_cv_t<To>> &&
                          !is_same_v<remove_cv_t<To>, bool> &&
                          ac_detail::io::is_modint_v<Mint>,
                      int> = 0>
To mint_cast(const Mint& value) {
    const unsigned int raw = value.val();
    if (static_cast<uintmax_t>(raw) >
        static_cast<uintmax_t>(numeric_limits<remove_cv_t<To>>::max())) {
        throw overflow_error("mint_cast: value does not fit in destination integer type");
    }
    return static_cast<To>(raw);
}
#endif

#ifdef AC_HAS_ACL
// modint では法を型が持つため、第3引数は不要。
template <class Mint, class Factor,
          enable_if_t<
              ac_detail::io::is_modint_v<Mint> &&
                  (is_integral_v<remove_cvref_t<Factor>> ||
                   is_same_v<remove_cvref_t<Factor>, remove_cvref_t<Mint>>),
              int> = 0>
Mint mul_mod(Mint a, Factor b) {
    return a * Mint(b);
}
#endif

// Input helpers (Clang-safe): pair/tuple/optional declared before read_seq.
// ---------------------------------------------------------------------------
namespace ac_detail::input {
template <class T>
istream& extract(istream& is, T& x);
template <class T1, class T2>
istream& extract(istream& is, pair<T1, T2>& p);
template <class... Args>
istream& extract(istream& is, tuple<Args...>& t);
template <class T>
istream& extract(istream& is, optional<T>& o);

template <class Seq>
istream& read_seq(istream& is, Seq& s) {
    for (auto it = s.begin(); it != s.end(); ++it) {
        extract(is, *it);
    }
    return is;
}

inline istream& read_seq(istream& is, vector<bool>& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        bool bit = false;
        is >> bit;
        s[i] = bit;
    }
    return is;
}

template <class T1, class T2>
istream& extract(istream& is, pair<T1, T2>& p) {
    extract(is, p.first);
    extract(is, p.second);
    return is;
}

template <class... Args>
istream& extract(istream& is, tuple<Args...>& t) {
    apply([&is](auto&... args) { (extract(is, args), ...); }, t);
    return is;
}

template <class T>
istream& extract(istream& is, optional<T>& o) {
    T x{};
    extract(is, x);
    o = std::move(x);
    return is;
}

template <class T>
istream& extract(istream& is, T& x) {
    if constexpr (is_same_v<ac_detail::io::remove_cvref_t<T>, vector<bool>>) {
        return read_seq(is, x);
    } else if constexpr (ac_detail::io::is_mutable_range_v<T>) {
        return read_seq(is, x);
    } else if constexpr (ac_detail::io::is_modint_v<T>) {
        ll value;
        is >> value;
        x = value;
        return is;
    } else {
        return is >> x;
    }
}
}  // namespace ac_detail::input

// ---------------------------------------------------------------------------
// Input API layer 1/5 — generic functions (source of truth)
// scan は「既存変数へ読む」「型を指定して返す」の共通入口。
// inはscanと同じ3形式を提供する従来の短い名前。
// rd / in_tuple は戻り値形式の用途別糖衣として残す。
// ---------------------------------------------------------------------------

// Stream adapters belong to common input, before geometry defines its readers.
#ifdef AC_HAS_ACL
// ACL modint の直接入力。負数や mod 以上の値も代入時に正規化される。
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
istream& operator>>(istream& is, Mint& value) {
    ll input;
    is >> input;
    value = input;
    return is;
}
#endif
// @acf helper operator>> — STL mutable range / pair / tuple / optional の入力
/// @acf-doc @brief STL mutable range / pair / tuple / optional の入力
/// @acf-doc [helper] operator>>
template <class R, enable_if_t<ac_detail::io::is_mutable_range_v<R>, int> = 0>
istream& operator>>(istream& is, R& r) {
    return ac_detail::input::extract(is, r);
}

template <class T1, class T2>
istream& operator>>(istream& is, pair<T1, T2>& p) {
    return ac_detail::input::extract(is, p);
}

template <class... Args>
istream& operator>>(istream& is, tuple<Args...>& t) {
    return ac_detail::input::extract(is, t);
}

template <class T>
istream& operator>>(istream& is, optional<T>& o) {
    return ac_detail::input::extract(is, o);
}

// @acf fn scan — (T&... a) -> bool — 宣言済みの変数へ一括入力する共通入口
/// @acf-doc @brief `scan(a,b)` は宣言済み変数へ入力。常に true。`scan<int>()` は値、`scan<int,ll>()` は tuple を返す
/// @acf-doc @code (T&... a) -> bool / scan<T>() -> T / scan<T,U,...>() -> tuple<T,U,...>
/// @acf-doc [function] scan
template <class... T>
bool scan(T&... a) {
    (ac_detail::input::extract(cin, a), ...);
    return true;
}

template <class T>
T scan() {
    T value{};
    ac_detail::input::extract(cin, value);
    return value;
}

template <class T, class U, class... Rest>
tuple<T, U, Rest...> scan() {
    tuple<T, U, Rest...> values;
    apply([](auto&... xs) { scan(xs...); }, values);
    return values;
}

// @acf fn in — scan と同じ引数・戻り値形式で入力する短い従来名
/// @acf-doc @brief `in(a,b)` は宣言済み変数へ入力して常に true。`in<T>()` は1値、`in<T,U,...>()` は tuple を返す。`scan` と同じ形式
/// @acf-doc @code (T&... a) -> bool / in<T>() -> T / in<T,U,...>() -> tuple<T,U,...>
/// @acf-doc [function] in
template <class... T>
bool in(T&... a) {
    return scan(a...);
}

template <class T>
T in() {
    return scan<T>();
}

template <class T, class U, class... Rest>
tuple<T, U, Rest...> in() {
    return scan<T, U, Rest...>();
}

// @acf fn rd — () -> T — 読んで返す（式向け）
/// @acf-doc @brief 読んで返す（式向け）
/// @acf-doc @code () -> T
/// @acf-doc [function] rd
template <class T>
T rd() {
    return in<T>();
}

// @acf fn in_tuple — () -> tuple<Ts...> — 型を指定して読んで structured bindings 向けに返す
/// @acf-doc @brief 型を指定して読んで structured bindings 向けに返す
/// @acf-doc @code () -> tuple<Ts...>
/// @acf-doc [function] in_tuple
template <class... Ts>
tuple<Ts...> in_tuple() {
    tuple<Ts...> values;
    apply([](auto&... xs) { scan(xs...); }, values);
    return values;
}

namespace ac_detail::output {
namespace detail {
using ac_detail::io::is_range_v;
using ac_detail::io::natural_range_sep_v;
using ac_detail::io::remove_cvref_t;
}  // namespace detail

using ac_detail::io::natural_range_sep_v;

template <class Os, class T>
void write_item(Os& os, const T& x);
template <class Os, class T1, class T2>
void write_item(Os& os, const pair<T1, T2>& p);
template <class Os, class... Args>
void write_item(Os& os, const tuple<Args...>& t);
template <class Os, class T>
void write_item(Os& os, const optional<T>& o);
template <class Os, size_t N>
void write_item(Os& os, const bitset<N>& x);
template <class Os, class T, class C>
void write_item(Os& os, stack<T, C> st);
template <class Os, class T, class C>
void write_item(Os& os, queue<T, C> q);
template <class Os, class T, class C, class Comp>
void write_item(Os& os, priority_queue<T, C, Comp> pq);
template <class Os, class T, class Comp>
void write_item(Os& os, setpq<T, Comp> pq);

template <class Os, class T1, class T2>
void write_item(Os& os, const pair<T1, T2>& p) {
    write_item(os, p.first);
    os << ' ';
    write_item(os, p.second);
}

template <class Os, size_t N>
void write_item(Os& os, const bitset<N>& x) {
    os << x;
}

template <class Os, class... Args>
void write_item(Os& os, const tuple<Args...>& t) {
    apply(
        [&os](const auto&... args) {
            bool first = true;
            ((first ? (first = false, write_item(os, args))
                    : (os << ' ', write_item(os, args))),
             ...);
        },
        t);
}

template <class Os, class T>
void write_item(Os& os, const optional<T>& o) {
    if (o) write_item(os, *o);
    else os << "nullopt";
}

template <class Os, class T, class C>
void write_item(Os& os, stack<T, C> st) {
    bool first = true;
    while (!st.empty()) {
        if (!first) os << ' ';
        first = false;
        write_item(os, st.top());
        st.pop();
    }
}

template <class Os, class T, class C>
void write_item(Os& os, queue<T, C> q) {
    bool first = true;
    while (!q.empty()) {
        if (!first) os << ' ';
        first = false;
        write_item(os, q.front());
        q.pop();
    }
}

template <class Os, class T, class C, class Comp>
void write_item(Os& os, priority_queue<T, C, Comp> pq) {
    bool first = true;
    while (!pq.empty()) {
        if (!first) os << ' ';
        first = false;
        write_item(os, pq.top());
        pq.pop();
    }
}

template <class Os, class T, class Comp>
void write_item(Os& os, setpq<T, Comp> pq) {
    bool first = true;
    while (!pq.empty()) {
        if (!first) os << ' ';
        first = false;
        write_item(os, pq.top());
        pq.pop();
    }
}

template <class Os, class T>
void write_item(Os& os, const T& x) {
    if constexpr (ac_detail::io::is_modint_v<T>) {
        os << x.val();
    } else if constexpr (detail::is_range_v<T>) {
        using Elem = detail::remove_cvref_t<decltype(*std::begin(x))>;
        constexpr char sep = detail::natural_range_sep_v<Elem>;
        bool first = true;
        for (const auto& e : x) {
            if (!first) os << sep;
            first = false;
            write_item(os, e);
        }
    } else if constexpr (is_same_v<T, char>) {
        ac_detail::io::write_char_as_display(os, x);
    } else {
        os << x;
    }
}

template <class Os, class T, class... Rest>
void write_items(Os& os, char sep, const T& first, const Rest&... rest) {
    write_item(os, first);
    if constexpr (sizeof...(Rest) > 0) {
        ((os << sep, write_item(os, rest)), ...);
    } else {
        (void)sep;
    }
}

template <class Os>
void writeln(Os& os) {
    os << '\n';
}

template <class Os, class... T>
void write(Os& os, char sep, const T&... args) {
    if constexpr (sizeof...(T) > 0) write_items(os, sep, args...);
    else (void)sep;
}

template <class Os, class... T>
void writeln(Os& os, char sep, const T&... args) {
    write(os, sep, args...);
    os << '\n';
}

template <class Os, class R, class Sep>
void write_range(Os& os, const R& r, const Sep& sep) {
    bool first = true;
    for (const auto& e : r) {
        if (!first) os << sep;
        first = false;
        write_item(os, e);
    }
}

template <class Os, class T>
void write_pr_item(Os& os, const T& x) {
    if constexpr (ac_detail::io::is_range_v<T>) write_range(os, x, string_view{});
    else write_item(os, x);
}

inline bool flush() {
    cout.flush();
    return true;
}

inline bool finish() {
#ifdef AC_INTERACTIVE
    return flush();
#else
    return true;
#endif
}
}  // namespace ac_detail::output

// @acf fn print — (const T&... args) -> bool — 空白区切りで出力して改行（STL / bitset 対応）。常に true（`cond && print(x)` 用）
/// @acf-doc @brief 空白区切りで出力して改行（vector/set/map などSTL・bitset 対応。単一 range の自然区切りはスカラー空白・pair/tuple/bitset/ネストは改行）。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true（`cond && print(x)` のように && チェーン可）
/// @acf-doc @code (const T&... args) -> bool
/// @acf-doc [function] print
template <class... T, enable_if_t<(sizeof...(T) <= 1), int> = 0>
bool print(const T&... args) {
    ac_detail::output::writeln(cout, ' ', args...);
    return ac_detail::output::finish();
}

template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool print(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::writeln(cout, ' ', a, b, rest...);
    return ac_detail::output::finish();
}

// @acf fn pr — (const T&... args) -> bool — 引数を連結して出力（改行なし）。常に true
/// @acf-doc @brief 引数間に区切りを入れず出力。区切りが必要なら `pr(a, ' ', b)` のように明示する。単一 range の要素も連結し、`pr(range, sep)` で区切りを指定できる。改行・flush なし。戻り値は常に true（&& チェーン可）
/// @acf-doc @code (const T&... args) -> bool
/// @acf-doc [function] pr
template <class... T, enable_if_t<(sizeof...(T) <= 1), int> = 0>
bool pr(const T&... args) {
    (ac_detail::output::write_pr_item(cout, args), ...);
    return true;
}

template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool pr(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::write_pr_item(cout, a);
    ac_detail::output::write_pr_item(cout, b);
    (ac_detail::output::write_pr_item(cout, rest), ...);
    return true;
}

// @acf fn pr — (const R& r, char sep) -> bool — range を sep 区切りで出力（改行なし）
/// @acf-doc @brief range の要素区切りを char で指定して出力。改行・flush なし。戻り値は常に true
/// @acf-doc @code (const R& r, char sep) -> bool
/// @acf-doc [function] pr
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool pr(const R& r, char sep) {
    ac_detail::output::write_range(cout, r, sep);
    return true;
}

// @acf fn pr — (const R& r, string_view sep) -> bool — range を文字列 sep 区切りで出力（改行なし）
/// @acf-doc @brief range の要素区切りを string_view / 文字列リテラルで指定して出力。改行・flush なし。戻り値は常に true
/// @acf-doc @code (const R& r, string_view sep) -> bool
/// @acf-doc [function] pr
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool pr(const R& r, string_view sep) {
    ac_detail::output::write_range(cout, r, sep);
    return true;
}

// @acf fn print — (const R& r, char sep, bool nl=true) -> bool — range を char sep 区切りで出力
/// @acf-doc @brief vector/set/map/deque など range の要素区切りを char で指定。`print(r,' ')` / `print(r,'\\n')`。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (const R& r, char sep, bool nl=true) -> bool
/// @acf-doc [function] print
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool print(const R& r, char sep, bool nl = true) {
    // nl=false は既存互換。改行なしの新規コードは pr(r, sep) を使う。
    ac_detail::output::write_range(cout, r, sep);
    if (nl) cout << '\n';
    return ac_detail::output::finish();
}

// @acf fn print — (const R& r, string_view sep, bool nl=true) -> bool — range を文字列 sep 区切りで出力
/// @acf-doc @brief range の要素区切りを string_view / 文字列リテラルで指定（`print(v, \":\")` など）。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (const R& r, string_view sep, bool nl=true) -> bool
/// @acf-doc [function] print
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool print(const R& r, string_view sep, bool nl = true) {
    // nl=false は既存互換。改行なしの新規コードは pr(r, sep) を使う。
    ac_detail::output::write_range(cout, r, sep);
    if (nl) cout << '\n';
    return ac_detail::output::finish();
}

// @acf fn println — (const T&... args) -> bool — すべて改行区切りで出力。空 range は無出力。常に true
/// @acf-doc @brief 引数をすべて改行区切りで出力。単一 range は要素も改行（`print(r, '\\n')`）。空 range は何も出さない（末尾の空行も付けない。AOJの PE 対策）。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (const T&... args) -> bool
/// @acf-doc [function] println
inline bool println() {
    cout << '\n';
    return ac_detail::output::finish();
}

template <class T>
bool println(const T& x) {
    if constexpr (ac_detail::io::is_range_v<T>) {
        // 1要素1行の意味なので、0要素なら 0 行（空行すら出さない）。AOJの PE 対策。
        if (std::empty(x)) return ac_detail::output::finish();
        return ::print(x, '\n');
    }
    ac_detail::output::writeln(cout, '\n', x);
    return ac_detail::output::finish();
}

template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool println(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::writeln(cout, '\n', a, b, rest...);
    return ac_detail::output::finish();
}

// @acf fn println — (const R& r, char sep, bool nl=true) -> bool — 既存互換の区切り指定
/// @acf-doc @brief 既存互換の区切り指定（`print(r, sep, nl)` と同じ）。新規コードでは `print(r, sep)` または `pr(r, sep)` を使う。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (const R& r, char sep, bool nl=true) -> bool
/// @acf-doc [function] println
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool println(const R& r, char sep, bool nl = true) {
    return ::print(r, sep, nl);
}

// @acf fn println — (const R& r, string_view sep, bool nl=true) -> bool — 既存互換の区切り指定
/// @acf-doc @brief 既存互換の区切り指定（`print(r, sep, nl)` と同じ）。新規コードでは `print(r, sep)` または `pr(r, sep)` を使う。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (const R& r, string_view sep, bool nl=true) -> bool
/// @acf-doc [function] println
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool println(const R& r, string_view sep, bool nl = true) {
    return ::print(r, sep, nl);
}

// @acf fn print — (const vector<vector<T>>& a, char sep=' ') -> bool — 2次元 vector を行ごとに出力
/// @acf-doc @brief 2次元 vector を行ごとに出力。行内区切りは sep（デフォルト空白）。各行をprint経由で出力。通常はflushせず、`AC_INTERACTIVE`時だけ各行でflush。戻り値は常に true
/// @acf-doc @code (const vector<vector<T>>& a, char sep=' ') -> bool
/// @acf-doc [function] print
template <class T>
bool print(const vector<vector<T>>& a, char sep = ' ') {
    for (const auto& row : a) ::print(row, sep, true);
    return true;
}

// @acf fn print_fixed — (int prec, const T&... args) -> bool — 固定小数点で print
/// @acf-doc @brief 固定小数点で print。戻り値は常に true
/// @acf-doc @code (int prec, const T&... args) -> bool
/// @acf-doc [function] print_fixed
template <class... T>
bool print_fixed(int prec, const T&... args) {
    cout << fixed << setprecision(prec);
    ::print(args...);
    cout << defaultfloat;
    return true;
}

// C++23 std::print / std::println との ADL・using 衝突を避ける（自前を常に優先）
// 標準出力API・同名メンバーを使うコードではinclude前にAC_NO_PRINT_MACROS。
#ifndef AC_NO_PRINT_MACROS
#define print(...) ::print(__VA_ARGS__)
#define println(...) ::println(__VA_ARGS__)
#endif

// @acf fn chmax — (T& a, const T& b) / (T& a, U b) -> bool — a<b なら更新、更新したら true
/// @acf-doc @brief 同じ型、または異なる算術型の候補を受け取る。異なる算術型は共通型で比較し、更新時に a の型へ変換する
/// @acf-doc @code (T& a, const T& b) -> bool / (T& a, U b) -> bool for arithmetic T != U
/// @acf-doc [function] chmax
template <class T>
bool chmax(T& a, const T& b) {
    return a < b ? (a = b, 1) : 0;
}
template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> &&
         (!is_same_v<T, U>) && is_assignable_v<T&, U>
bool chmax(T& a, U b) {
    using Common = common_type_t<T, U>;
    if (static_cast<Common>(a) < static_cast<Common>(b)) {
        a = static_cast<T>(b);
        return true;
    }
    return false;
}
// @acf fn chmin — (T& a, const T& b) / (T& a, U b) -> bool — a>b なら更新、更新したら true
/// @acf-doc @brief 同じ型、または異なる算術型の候補を受け取る。異なる算術型は共通型で比較し、更新時に a の型へ変換する
/// @acf-doc @code (T& a, const T& b) -> bool / (T& a, U b) -> bool for arithmetic T != U
/// @acf-doc [function] chmin
template <class T>
bool chmin(T& a, const T& b) {
    return a > b ? (a = b, 1) : 0;
}
template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> &&
         (!is_same_v<T, U>) && is_assignable_v<T&, U>
bool chmin(T& a, U b) {
    using Common = common_type_t<T, U>;
    if (static_cast<Common>(a) > static_cast<Common>(b)) {
        a = static_cast<T>(b);
        return true;
    }
    return false;
}
// @acf fn delta_update — (T& total, const T& old_value, const T& new_value) -> void — 合計中の寄与を old_value から new_value へ差し替える
/// @acf-doc @brief `total` に `new_value - old_value` を加算する。合計中の1つの寄与を差し替えるときに使う
/// @acf-doc @code (T& total, const T& old_value, const T& new_value) -> void
/// @acf-doc [function] delta_update
template <class T>
constexpr void delta_update(T& total, const T& old_value, const T& new_value) {
    total += new_value - old_value;
}
// @acf fn popv — (A& a) -> value_type — top/front を取り出して pop、値を返す
/// @acf-doc @brief `stack` / `pq` は `top`+`pop`、`queue` は `front`+`pop`、`deque` は `front`+`pop_front`。値を返す。空は UB（LOCAL では assert）
/// @acf-doc @code (A& a) -> typename A::value_type
/// @acf-doc [function] popv
template <class A>
auto popv(A& a) {
#ifdef LOCAL
    assert(!a.empty());
#endif
    using T = typename A::value_type;
    if constexpr (requires {
                      a.top();
                      a.pop();
                  }) {
        T x = std::move(a.top());
        a.pop();
        return x;
    } else if constexpr (requires {
                             a.front();
                             a.pop();
                         }) {
        T x = std::move(a.front());
        a.pop();
        return x;
    } else if constexpr (requires {
                             a.front();
                             a.pop_front();
                         }) {
        T x = std::move(a.front());
        a.pop_front();
        return x;
    } else {
        static_assert(sizeof(A) == 0,
                      "popv: need top+pop, front+pop, or front+pop_front");
    }
}
// @acf fn popb — (A& a) -> value_type — back を取り出して pop_back、値を返す
/// @acf-doc @brief `deque` / `vector` などの `back` を取得してから `pop_back`。値を返す。空は UB（LOCAL では assert）
/// @acf-doc @code (A& a) -> typename A::value_type
/// @acf-doc [function] popb
template <class A>
auto popb(A& a) {
#ifdef LOCAL
    assert(!a.empty());
#endif
    using T = typename A::value_type;
    T x = std::move(a.back());
    a.pop_back();
    return x;
}
// @acf fn between — (const T& x, const U& lo, const V& hi) -> bool — 区間判定（InclLo/InclHi、既定は閉）
/// @acf-doc @brief 区間判定。既定は閉 `[lo,hi]`。`between<true,false>`=`[lo,hi)`、`<false,true>`=`(lo,hi]`、`<false,false>`=`(lo,hi)`。x/lo/hi は int・ll・double など混在可
/// @acf-doc @code template <bool InclLo=true, bool InclHi=true, class T, class U, class V> (const T& x, const U& lo, const V& hi) -> bool
/// @acf-doc [function] between
template <bool InclLo = true, bool InclHi = true, class T, class U, class V>
bool between(const T& x, const U& lo, const V& hi) {
    return (InclLo ? lo <= x : lo < x) && (InclHi ? x <= hi : x < hi);
}

// @acf fn any_equal — (value, candidates...) -> bool — 第1引数が候補のいずれかと等しいか
/// @acf-doc @brief 第1引数を後続の各引数と比較し、いずれかが等しければ true。候補が0個なら false
/// @acf-doc @code (const T& value, const Rest&... candidates) -> bool
/// @acf-doc [function] any_equal
template <class T, class... Rest>
constexpr bool any_equal(const T& value, const Rest&... candidates) {
    return ((value == candidates) || ...);
}

// @acf fn all_equal — (values...) -> bool — 全引数が等しいか。0個・1個は true
/// @acf-doc @brief 全引数を先頭と比較する。各実引数は一度だけ評価し、0個・1個は true
/// @acf-doc @code (const First& first, const Rest&... rest) -> bool
/// @acf-doc [function] all_equal
constexpr bool all_equal() {
    return true;
}

template <class First, class... Rest>
constexpr bool all_equal(const First& first, const Rest&... rest) {
    return ((first == rest) && ...);
}

// @acf fn same_parity — (values...) -> bool — 整数値の偶奇がすべて一致するか
/// @acf-doc @brief 整数値（負数を含む）の偶奇がすべて一致するかを判定する。実数型は受け付けない
/// @acf-doc @code (const First& first, const Rest&... rest) -> bool
/// @acf-doc [function] same_parity
template <class First, class... Rest>
constexpr bool same_parity(const First& first, const Rest&... rest) {
    constexpr bool valid =
        is_integral_v<ac_detail::io::remove_cvref_t<First>> &&
        (is_integral_v<ac_detail::io::remove_cvref_t<Rest>> && ...);
    static_assert(valid, "same_parity requires integral values");

    if constexpr (valid) {
        const auto parity = [](const auto& value) constexpr {
            return ((value % 2) + 2) % 2;
        };
        const auto first_parity = parity(first);
        return ((parity(rest) == first_parity) && ...);
    } else {
        return false;
    }
}

// @acf fn is_even — (x) -> bool — 整数値が偶数か
/// @acf-doc @brief 整数値（負数を含む）が偶数かを判定する。浮動小数点型は受け付けない
/// @acf-doc @code (const T& value) -> bool
/// @acf-doc [function] is_even
template <class T>
constexpr bool is_even(const T& value) {
    return same_parity(value, 0);
}

// @acf fn is_odd — (x) -> bool — 整数値が奇数か
/// @acf-doc @brief 整数値（負数を含む）が奇数かを判定する。浮動小数点型は受け付けない
/// @acf-doc @code (const T& value) -> bool
/// @acf-doc [function] is_odd
template <class T>
constexpr bool is_odd(const T& value) {
    return same_parity(value, 1);
}

// @acf fn sgn — (x[,eps=EPS]) -> int — 符号判定（+1 / 0 / -1）。浮動小数は誤差込み
/// @acf-doc @brief 浮動小数は `|x|<=eps` なら 0。整数などそれ以外は厳密に正負を見る
/// @acf-doc @code (T x, ld eps = EPS) -> int
/// @acf-doc [function] sgn
template <class T>
constexpr int sgn(T x, ld eps = EPS) {
    using U = decay_t<T>;
    if constexpr (is_floating_point_v<U>) {
        const ld v = static_cast<ld>(x);
        if (v > eps) return 1;
        if (v < -eps) return -1;
        return 0;
    } else {
        return (x > U{}) - (x < U{});
    }
}

// @acf fn eq — (a,b[,eps=EPS]) -> bool — 等価判定。浮動小数は誤差込み、それ以外は ==
/// @acf-doc @brief どちらかが浮動小数なら `|a-b|<=eps`、それ以外（整数・文字列・pair 等）は `a==b`
/// @acf-doc @code (const A& a, const B& b, ld eps = EPS) -> bool
/// @acf-doc [function] eq
template <class A, class B>
constexpr bool eq(const A& a, const B& b, ld eps = EPS) {
    if constexpr (is_floating_point_v<decay_t<A>> || is_floating_point_v<decay_t<B>>) {
        return sgn(static_cast<ld>(a) - static_cast<ld>(b), eps) == 0;
    } else {
        return a == b;
    }
}

namespace ac_detail::ascii {
constexpr char to_upper(char c) {
    return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c;
}

constexpr char to_lower(char c) {
    return 'A' <= c && c <= 'Z' ? c - 'A' + 'a' : c;
}
}  // namespace ac_detail::ascii

// @acf fn to_upper — (string_view s) -> str — ASCII 英字を大文字にした文字列のコピー
/// @acf-doc @brief ASCII 英字を大文字に変換した文字列を返す。元の文字列は変更しない
/// @acf-doc @code (string_view s) -> str
/// @acf-doc [function] to_upper
inline str to_upper(string_view s) {
    str result(s);
    for (char& c : result) c = ac_detail::ascii::to_upper(c);
    return result;
}

// @acf fn to_lower — (string_view s) -> str — ASCII 英字を小文字にした文字列のコピー
/// @acf-doc @brief ASCII 英字を小文字に変換した文字列を返す。元の文字列は変更しない
/// @acf-doc @code (string_view s) -> str
/// @acf-doc [function] to_lower
inline str to_lower(string_view s) {
    str result(s);
    for (char& c : result) c = ac_detail::ascii::to_lower(c);
    return result;
}

// @acf fn ctoi — (char c) -> int — 10進数字の文字を整数へ変換
/// @acf-doc @brief `'0'`〜`'9'` を `0`〜`9` に変換する。範囲外の文字は `invalid_argument`
/// @acf-doc @code (char c) -> int
/// @acf-doc [function] ctoi
constexpr int ctoi(char c) {
    if (c < '0' || c > '9') throw invalid_argument("ctoi: c must be an ASCII digit");
    return c - '0';
}

// @acf fn itoc — (int value) -> char — 0〜9を10進数字の文字へ変換
/// @acf-doc @brief `0`〜`9` を `'0'`〜`'9'` に変換する。範囲外の整数は `invalid_argument`
/// @acf-doc @code (int value) -> char
/// @acf-doc [function] itoc
constexpr char itoc(int value) {
    if (value < 0 || value > 9)
        throw invalid_argument("itoc: value must be in [0, 9]");
    return static_cast<char>('0' + value);
}

// @acf fn is_chars — (string_view s, Pred pred) -> bool — 各文字が判定関数を満たす文字列か
/// @acf-doc @brief 空でない文字列の全ての文字に `pred` を適用し、全て真なら true。文字は `unsigned char` として渡す
/// @acf-doc @code (string_view s, Pred pred) -> bool
/// @acf-doc [function] is_chars
template <class Pred>
inline bool is_chars(string_view s, Pred pred) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!pred(c)) return false;
    }
    return true;
}

// <cctype> の overload set（isdigit / isxdigit など）をそのまま受ける overload
inline bool is_chars(string_view s, int (*pred)(int)) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!pred(c)) return false;
    }
    return true;
}

// @acf fn is_digits — (string_view s) -> bool — 10進数字だけからなる文字列か
/// @acf-doc @brief 空でない文字列の全ての文字が `isdigit` を満たすか判定する。符号・小数点・空文字列は false
/// @acf-doc @code (string_view s) -> bool
/// @acf-doc [function] is_digits
inline bool is_digits(string_view s) {
    return is_chars(s, isdigit);
}

// @acf fn is_hex_digits — (string_view s) -> bool — 16進数字だけからなる文字列か
/// @acf-doc @brief 空でない文字列の全ての文字が `isxdigit` を満たすか判定する。`0x` 接頭辞は含めない
/// @acf-doc @code (string_view s) -> bool
/// @acf-doc [function] is_hex_digits
inline bool is_hex_digits(string_view s) {
    return is_chars(s, isxdigit);
}

// @acf fn reversed — (string_view s) -> str — 文字列を反転したコピー
/// @acf-doc @brief 文字列の文字順を反転したコピーを返す。元の文字列は変更しない
/// @acf-doc @code (string_view s) -> str
/// @acf-doc [function] reversed
inline str reversed(string_view s) {
    str result(s);
    ranges::reverse(result);
    return result;
}

// @acf fn eq_ignore_case — (string_view a, string_view b) -> bool — ASCII 英字の大小を無視した文字列比較
/// @acf-doc @brief ASCII 英字の大文字小文字を無視して、2つの文字列が等しいか判定する。英字以外はそのまま比較する
/// @acf-doc @code (string_view a, string_view b) -> bool
/// @acf-doc [function] eq_ignore_case
inline bool eq_ignore_case(string_view a, string_view b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (ac_detail::ascii::to_upper(a[i]) != ac_detail::ascii::to_upper(b[i]))
            return false;
    }
    return true;
}

namespace ac_detail::radix {
constexpr int digit_value(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'Z') return c - 'A' + 10;
    if ('a' <= c && c <= 'z') return c - 'a' + 10;
    return -1;
}

constexpr char digit_char(int value) {
    return value < 10 ? char('0' + value) : char('A' + value - 10);
}

inline int mapped_base(const map<char, int>& digit_map) {
    if (digit_map.empty())
        throw invalid_argument("base_convert: digit_map must not be empty");

    set<int> values;
    for (const auto& entry : digit_map) {
        const int value = entry.second;
        if (value < 0)
            throw invalid_argument("base_convert: digit_map values must be non-negative");
        values.insert(value);
    }

    const int first = *values.begin();
    const int base = first == 0 ? *values.rbegin() + 1 : *values.rbegin();
    if ((first != 0 && first != 1) || base < 2)
        throw invalid_argument("base_convert: digit_map values must start at 0 or 1");

    int expected = first;
    for (int value : values) {
        if (value != expected++)
            throw invalid_argument("base_convert: digit_map values must be consecutive");
    }
    return base;
}

inline str convert(string_view s, int from_base, int to_base,
                   const map<char, int>* digit_map = nullptr) {
    if (s.empty()) throw invalid_argument("base_convert: s must not be empty");

    size_t begin = 0;
    bool negative = false;
    if (s.front() == '+' || s.front() == '-') {
        negative = s.front() == '-';
        begin = 1;
    }
    if (begin == s.size()) throw invalid_argument("base_convert: s has no digits");

    vector<int> digits;
    digits.reserve(s.size() - begin);
    for (size_t i = begin; i < s.size(); ++i) {
        int digit = -1;
        if (digit_map) {
            const auto it = digit_map->find(s[i]);
            if (it != digit_map->end()) digit = it->second;
        } else {
            digit = digit_value(s[i]);
        }
        if (digit < 0 || (!digit_map && digit >= from_base))
            throw invalid_argument("base_convert: digit is outside from_base");
        digits.push_back(digit);
    }

    size_t first = 0;
    while (first < digits.size() && digits[first] == 0) ++first;
    if (first == digits.size()) return "0";
    digits.erase(digits.begin(), digits.begin() + first);

    str result;
    if (!digit_map && from_base == to_base) {
        result.reserve(digits.size() + static_cast<size_t>(negative));
        for (int digit : digits) result.push_back(digit_char(digit));
    } else {
        while (!digits.empty()) {
            int remainder = 0;
            vector<int> quotient;
            quotient.reserve(digits.size());
            for (int digit : digits) {
                const int current = remainder * from_base + digit;
                const int quotient_digit = current / to_base;
                remainder = current % to_base;
                if (!quotient.empty() || quotient_digit != 0)
                    quotient.push_back(quotient_digit);
            }
            result.push_back(digit_char(remainder));
            digits.swap(quotient);
        }
        std::reverse(result.begin(), result.end());
    }

    if (negative) result.insert(result.begin(), '-');
    return result;
}
}  // namespace ac_detail::radix

// @acf fn base_convert — (s, from_base, to_base) / (s, digit_map) -> str — 通常表記または独自の文字→桁値マップを使うn進数表記を変換
/// @acf-doc @brief 3引数版は2〜36進数の文字列を別の基数へ変換する。2引数版は独自の文字→桁値マップから基数を推定し、10進文字列を返す。文字列長を L として O(L^2)
/// @acf-doc @code (string_view s, int from_base, int to_base) / (string_view s, const map<char, int>& digit_map) -> str
/// @acf-doc [function] base_convert
inline str base_convert(string_view s, int from_base, int to_base) {
    if (from_base < 2 || from_base > 36)
        throw invalid_argument("base_convert: from_base must be in [2, 36]");
    if (to_base < 2 || to_base > 36)
        throw invalid_argument("base_convert: to_base must be in [2, 36]");
    return ac_detail::radix::convert(s, from_base, to_base);
}

inline str base_convert(string_view s, const map<char, int>& digit_map) {
    const int from_base = ac_detail::radix::mapped_base(digit_map);
    return ac_detail::radix::convert(s, from_base, 10, &digit_map);
}

// @acf fn fill_range — (range&, l, r, value) -> void — 半開区間 [l,r) を同じ値で埋める
/// @acf-doc @brief random access range の [l,r) を value で埋める。空区間は何もしない。LOCAL では 0<=l<=r<=size を検査。時間 O(r-l)、追加メモリ O(1)
/// @acf-doc @code (range&, int l, int r, const T& value) -> void
/// @acf-doc [function] fill_range
template <ranges::random_access_range R, class T>
requires ranges::sized_range<R> &&
         indirectly_writable<ranges::iterator_t<R>, const T&>
void fill_range(R& values, int l, int r, const T& value) {
#ifdef LOCAL
    assert(0 <= l && l <= r && r <= ranges::ssize(values));
#endif
    ranges::fill(ranges::begin(values) + l, ranges::begin(values) + r, value);
}

// @acf fn for_rect — (r1, r2, c1, c2, F) -> void — 半開矩形を行優先で走査
/// @acf-doc @brief `[r1,r2)×[c1,c2)` の各座標に `f(row,col)` を行優先で呼ぶ。空矩形は何もしない。時間 O(面積)、追加メモリ O(1)
/// @acf-doc @code (int r1, int r2, int c1, int c2, F f) -> void
/// @acf-doc [function] for_rect
template <class F>
void for_rect(int r1, int r2, int c1, int c2, F f) {
    for (int row = r1; row < r2; ++row)
        for (int col = c1; col < c2; ++col) std::invoke(f, row, col);
}

#ifdef LOCAL
namespace ac_detail::grid {
template <class Grid>
void assert_rect_bounds(Grid& grid, int r1, int c1, int r2, int c2) {
    assert(0 <= r1 && r1 <= r2 && r2 <= ranges::ssize(grid));
    assert(0 <= c1 && c1 <= c2);
    for (int row = r1; row < r2; ++row)
        assert(c2 <= ranges::ssize(grid[row]));
}
}  // namespace ac_detail::grid
#endif

// @acf fn fill_rect — (grid&, r1, c1, r2, c2, value) -> void — 半開矩形を同じ値で埋める
/// @acf-doc @brief grid の [r1,r2)×[c1,c2) を value で埋める。空矩形は何もしない。LOCAL では行範囲と選択行の列範囲を検査。時間 O((r2-r1)(c2-c1))、追加メモリ O(1)
/// @acf-doc @code (grid&, int r1, int c1, int r2, int c2, const T& value) -> void
/// @acf-doc [function] fill_rect
template <ranges::random_access_range Grid, class T>
requires ranges::sized_range<Grid> &&
         ranges::random_access_range<ranges::range_reference_t<Grid>> &&
         ranges::sized_range<ranges::range_reference_t<Grid>> &&
         indirectly_writable<ranges::iterator_t<ranges::range_reference_t<Grid>>, const T&>
void fill_rect(Grid& grid, int r1, int c1, int r2, int c2, const T& value) {
#ifdef LOCAL
    ac_detail::grid::assert_rect_bounds(grid, r1, c1, r2, c2);
#endif
    for (int row = r1; row < r2; ++row) {
        fill_range(grid[row], c1, c2, value);
    }
}

// @acf fn any_rect — (const grid&, r1, r2, c1, c2, pred) -> bool — 矩形内に条件を満たすセルがあるか
/// @acf-doc @brief `[r1,r2)×[c1,c2)` を行優先で調べ、最初の真で終了する。空矩形は false。LOCAL では範囲を検査。時間 O(面積)、追加メモリ O(1)
/// @acf-doc @code (const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) -> bool
/// @acf-doc [function] any_rect
template <class Grid, class Pred>
requires ranges::random_access_range<const Grid> &&
         ranges::sized_range<const Grid> &&
         ranges::random_access_range<ranges::range_reference_t<const Grid>> &&
         ranges::sized_range<ranges::range_reference_t<const Grid>>
bool any_rect(const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) {
#ifdef LOCAL
    ac_detail::grid::assert_rect_bounds(grid, r1, c1, r2, c2);
#endif
    for (int row = r1; row < r2; ++row)
        for (int col = c1; col < c2; ++col)
            if (std::invoke(pred, grid[row][col])) return true;
    return false;
}

// @acf fn all_rect — (const grid&, r1, r2, c1, c2, pred) -> bool — 矩形内の全セルが条件を満たすか
/// @acf-doc @brief `[r1,r2)×[c1,c2)` を行優先で調べ、最初の偽で終了する。空矩形は true。LOCAL では範囲を検査。時間 O(面積)、追加メモリ O(1)
/// @acf-doc @code (const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) -> bool
/// @acf-doc [function] all_rect
template <class Grid, class Pred>
requires ranges::random_access_range<const Grid> &&
         ranges::sized_range<const Grid> &&
         ranges::random_access_range<ranges::range_reference_t<const Grid>> &&
         ranges::sized_range<ranges::range_reference_t<const Grid>>
bool all_rect(const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) {
#ifdef LOCAL
    ac_detail::grid::assert_rect_bounds(grid, r1, c1, r2, c2);
#endif
    for (int row = r1; row < r2; ++row)
        for (int col = c1; col < c2; ++col)
            if (!std::invoke(pred, grid[row][col])) return false;
    return true;
}

// @acf fn count_rect — (const grid&, r1, r2, c1, c2, pred) -> ll — 矩形内で条件を満たすセル数
/// @acf-doc @brief `[r1,r2)×[c1,c2)` を走査し、条件を満たすセル数を `ll` で返す。空矩形は0。LOCAL では範囲を検査。時間 O(面積)、追加メモリ O(1)
/// @acf-doc @code (const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) -> ll
/// @acf-doc [function] count_rect
template <class Grid, class Pred>
requires ranges::random_access_range<const Grid> &&
         ranges::sized_range<const Grid> &&
         ranges::random_access_range<ranges::range_reference_t<const Grid>> &&
         ranges::sized_range<ranges::range_reference_t<const Grid>>
ll count_rect(const Grid& grid, int r1, int r2, int c1, int c2, Pred pred) {
#ifdef LOCAL
    ac_detail::grid::assert_rect_bounds(grid, r1, c1, r2, c2);
#endif
    ll result = 0;
    for (int row = r1; row < r2; ++row)
        for (int col = c1; col < c2; ++col)
            if (std::invoke(pred, grid[row][col])) ++result;
    return result;
}

// @acf fn in_grid — (r,c,h,w) -> bool — (r,c) が h×w グリッド内か判定
/// @acf-doc @brief `(r,c)` が `h×w` グリッド内か判定。引数順は row, column, height, width
/// @acf-doc @code (r, c, h, w) -> bool
/// @acf-doc [function] in_grid
template <class R, class C, class H, class W>
constexpr bool in_grid(R r, C c, H h, W w) {
    return 0 <= r && r < h && 0 <= c && c < w;
}

// @acf fn out_grid — (r,c,h,w) -> bool — (r,c) が h×w グリッド外か判定
/// @acf-doc @brief `(r,c)` が `h×w` グリッド外か判定。`in_grid` の否定
/// @acf-doc @code (r, c, h, w) -> bool
/// @acf-doc [function] out_grid
template <class R, class C, class H, class W>
constexpr bool out_grid(R r, C c, H h, W w) {
    return !in_grid(r, c, h, w);
}

struct GridBounds {
    int top;
    int left;
    int bottom;
    int right;

    constexpr bool contains(int row, int col) const {
        return top <= row && row <= bottom && left <= col && col <= right;
    }
};

// @acf fn grid_bounds_if — (grid,pred) -> optional<GridBounds> — 条件を満たすマスの外接矩形を取得
/// @acf-doc @brief 条件を満たすマスを囲む最小の矩形を両端含みで返す。該当マスがなければ nullopt。マス数を HW として O(HW)
/// @acf-doc @code (const Grid& grid, Pred pred) -> optional<GridBounds>
/// @acf-doc [function] grid_bounds_if
template <class Grid, class Pred>
optional<GridBounds> grid_bounds_if(const Grid& grid, Pred pred) {
    optional<GridBounds> result;
    for (int row = 0; row < sz(grid); ++row) {
        for (int col = 0; col < sz(grid[row]); ++col) {
            if (!std::invoke(pred, grid[row][col])) continue;
            if (!result) {
                result = GridBounds{row, col, row, col};
                continue;
            }
            chmin(result->top, row);
            chmin(result->left, col);
            chmax(result->bottom, row);
            chmax(result->right, col);
        }
    }
    return result;
}

namespace ac_detail::grid {
template <class Rows>
bool rectangular(const Rows& grid) {
    if (grid.empty()) return true;
    const auto width = grid.front().size();
    return all_of(grid.begin(), grid.end(), [&](const auto& row) {
        return row.size() == width;
    });
}

template <class Planes>
bool rectangular3d(const Planes& volume) {
    if (volume.empty()) return true;
    const auto height = volume.front().size();
    const auto width = height == 0 ? 0 : volume.front().front().size();
    return all_of(volume.begin(), volume.end(), [&](const auto& plane) {
        return plane.size() == height &&
               all_of(plane.begin(), plane.end(), [&](const auto& row) {
                   return row.size() == width;
               });
    });
}
}  // namespace ac_detail::grid

// @acf fn rotate90 — (const vector<vector<T>>& grid[, const T& fill]) -> vector<vector<T>> — グリッドを時計回りに90度回転
/// @acf-doc @brief 長方形の2次元vectorを時計回りに90度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<vector<T>>& grid) -> vector<vector<T>>
/// @acf-doc @code (const vector<vector<T>>& grid, const T& fill) -> vector<vector<T>>
/// @acf-doc [function] rotate90
template <class T>
vector<vector<T>> rotate90(const vector<vector<T>>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<vector<T>> result(w, vector<T>(h));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[c][h - 1 - r] = grid[r][c];
    return result;
}

template <class T>
vector<vector<T>> rotate90(const vector<vector<T>>& grid, const T& fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<vector<T>> result(width, vector<T>(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c)
            result[c][grid.size() - 1 - r] = grid[r][c];
    return result;
}

// @acf fn rotate90 — (const vector<string>& grid[, char fill]) -> vector<string> — 文字グリッドを時計回りに90度回転
/// @acf-doc @brief 長方形の文字グリッドを時計回りに90度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<string>& grid) -> vector<string>
/// @acf-doc @code (const vector<string>& grid, char fill) -> vector<string>
/// @acf-doc [function] rotate90
inline vector<string> rotate90(const vector<string>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<string> result(w, string(h, '\0'));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[c][h - 1 - r] = grid[r][c];
    return result;
}

inline vector<string> rotate90(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width, string(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c)
            result[c][grid.size() - 1 - r] = grid[r][c];
    return result;
}

// @acf fn rotate90_left — (const vector<vector<T>>& grid[, const T& fill]) -> vector<vector<T>> — グリッドを反時計回りに90度回転
/// @acf-doc @brief 長方形の2次元vectorを反時計回りに90度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<vector<T>>& grid) -> vector<vector<T>>
/// @acf-doc @code (const vector<vector<T>>& grid, const T& fill) -> vector<vector<T>>
/// @acf-doc [function] rotate90_left
template <class T>
vector<vector<T>> rotate90_left(const vector<vector<T>>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<vector<T>> result(w, vector<T>(h));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[w - 1 - c][r] = grid[r][c];
    return result;
}

template <class T>
vector<vector<T>> rotate90_left(const vector<vector<T>>& grid, const T& fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<vector<T>> result(width, vector<T>(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[width - 1 - c][r] = grid[r][c];
    return result;
}

// @acf fn rotate90_left — (const vector<string>& grid[, char fill]) -> vector<string> — 文字グリッドを反時計回りに90度回転
/// @acf-doc @brief 長方形の文字グリッドを反時計回りに90度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<string>& grid) -> vector<string>
/// @acf-doc @code (const vector<string>& grid, char fill) -> vector<string>
/// @acf-doc [function] rotate90_left
inline vector<string> rotate90_left(const vector<string>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<string> result(w, string(h, '\0'));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[w - 1 - c][r] = grid[r][c];
    return result;
}

inline vector<string> rotate90_left(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width, string(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[width - 1 - c][r] = grid[r][c];
    return result;
}

// @acf fn rotate180 — (const vector<vector<T>>& grid[, const T& fill]) -> vector<vector<T>> — グリッドを180度回転
/// @acf-doc @brief 長方形の2次元vectorを180度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<vector<T>>& grid) -> vector<vector<T>>
/// @acf-doc @code (const vector<vector<T>>& grid, const T& fill) -> vector<vector<T>>
/// @acf-doc [function] rotate180
template <class T>
vector<vector<T>> rotate180(const vector<vector<T>>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<vector<T>> result(h, vector<T>(w));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[h - 1 - r][w - 1 - c] = grid[r][c];
    return result;
}

template <class T>
vector<vector<T>> rotate180(const vector<vector<T>>& grid, const T& fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<vector<T>> result(grid.size(), vector<T>(width, fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c)
            result[grid.size() - 1 - r][width - 1 - c] = grid[r][c];
    return result;
}

// @acf fn rotate180 — (const vector<string>& grid[, char fill]) -> vector<string> — 文字グリッドを180度回転
/// @acf-doc @brief 長方形の文字グリッドを180度回転する。fillを指定すると不揃いな行を最大行長まで埋めてから回転する。時間・追加メモリ O(HW)
/// @acf-doc @code (const vector<string>& grid) -> vector<string>
/// @acf-doc @code (const vector<string>& grid, char fill) -> vector<string>
/// @acf-doc [function] rotate180
inline vector<string> rotate180(const vector<string>& grid) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(grid));
#endif
    if (grid.empty() || grid.front().empty()) return {};
    const size_t h = grid.size(), w = grid.front().size();
    vector<string> result(h, string(w, '\0'));
    for (size_t r = 0; r < h; ++r)
        for (size_t c = 0; c < w; ++c) result[h - 1 - r][w - 1 - c] = grid[r][c];
    return result;
}

inline vector<string> rotate180(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(grid.size(), string(width, fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c)
            result[grid.size() - 1 - r][width - 1 - c] = grid[r][c];
    return result;
}

// @acf fn transpose — (const vector<vector<T>>& grid[, const T& fill]) -> vector<vector<T>> — 2次元配列を転置
/// @acf-doc @brief 行長が異なる2次元vectorも転置できる。fillなしは存在する要素だけを各列へ集め、fillありは最大行長×行数にして欠けた位置をfillで埋める
/// @acf-doc @code (const vector<vector<T>>& grid) -> vector<vector<T>>
/// @acf-doc @code (const vector<vector<T>>& grid, const T& fill) -> vector<vector<T>>
/// @acf-doc [function] transpose
template <class T>
vector<vector<T>> transpose(const vector<vector<T>>& grid) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<vector<T>> result(width);
    for (auto& column : result) column.reserve(grid.size());
    for (const auto& row : grid)
        for (size_t c = 0; c < row.size(); ++c) result[c].push_back(row[c]);
    return result;
}

template <class T>
vector<vector<T>> transpose(const vector<vector<T>>& grid, const T& fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<vector<T>> result(width, vector<T>(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[c][r] = grid[r][c];
    return result;
}

// @acf fn transpose — (const vector<string>& grid[, char fill]) -> vector<string> — 文字列配列を転置
/// @acf-doc @brief 行長が異なる文字列も転置できる。fillなしは存在する文字だけを各列へ集め、fillありは最大行長×行数にして欠けた位置をfillで埋める
/// @acf-doc @code (const vector<string>& grid) -> vector<string>
/// @acf-doc @code (const vector<string>& grid, char fill) -> vector<string>
/// @acf-doc [function] transpose
inline vector<string> transpose(const vector<string>& grid) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width);
    for (auto& column : result) column.reserve(grid.size());
    for (const auto& row : grid)
        for (size_t c = 0; c < row.size(); ++c) result[c].push_back(row[c]);
    return result;
}

inline vector<string> transpose(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width, string(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[c][r] = grid[r][c];
    return result;
}

// @acf fn sort_unique — (vector<T>& v) -> bool — ソートして重複を削除。常に true
/// @acf-doc @brief vectorをソートし、隣接するoperator==の重複を削除。省略時は昇順、第2引数compは並び順だけを指定。同じ値が離れれば残る。常にtrue（`&&`チェーン可）
/// @acf-doc @code (vector<T>& v) / (vector<T>& v, Comp comp) -> bool
/// @acf-doc [function] sort_unique
template <class T>
bool sort_unique(vector<T>& v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return true;
}

template <class T, class Comp>
bool sort_unique(vector<T>& v, Comp comp) {
    sort(v.begin(), v.end(), std::move(comp));
    v.erase(unique(v.begin(), v.end()), v.end());
    return true;
}

// @acf fn recurrence — (T x, int n, Next next) -> vector<T> — 漸化式でちょうど n 項を生成
/// @acf-doc @brief `x, next(x), next²(x), ...` をちょうど `n` 項返す（`n>=0`）。等比・Shell 間隔など
/// @acf-doc @code (T x, int n, Next next) -> vector<T>
/// @acf-doc [function] recurrence
template <class T, class Next, enable_if_t<is_invocable_v<Next&, T>, int> = 0>
vector<T> recurrence(T x, int n, Next next) {
    assert(n >= 0);
    vector<T> a;
    a.reserve(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        a.push_back(x);
        if (i + 1 < n) x = next(x);
    }
    return a;
}
// @acf fn recurrence — (T x, Next next, Pred pred) -> vector<T> — 漸化式で pred が真のあいだ生成
/// @acf-doc @brief `pred(x)` が真のあいだ項を積み `x=next(x)`。初期が偽なら空。Shell の Knuth 列など。LOCAL では長さ 10^7 超で assert
/// @acf-doc @code (T x, Next next, Pred pred) -> vector<T>
/// @acf-doc [function] recurrence
template <class T, class Next, class Pred,
          enable_if_t<is_invocable_v<Next&, T> && is_invocable_r_v<bool, Pred&, T>, int> = 0>
vector<T> recurrence(T x, Next next, Pred pred) {
    vector<T> a;
    while (pred(x)) {
#ifdef LOCAL
        assert(a.size() < 10'000'000);
#endif
        a.push_back(x);
        x = next(x);
    }
    return a;
}

// @acf fn prefix_sum — (const vector<T>& a) -> vector<T> — 1次元累積和。返り値は N+1 要素
/// @acf-doc @brief `sum[i]=a[0]+...+a[i-1]` の番兵付き累積和を返す。要素が `pair` / `tuple` / `array` など tuple-like の場合は各成分を独立に累積する。通常の加算型では半開区間 `[l,r)` の和を `sum[r]-sum[l]` で求められる。計算量・追加メモリ O(N)
/// @acf-doc @code (const vector<T>& a) -> vector<T>
/// @acf-doc [function] prefix_sum
namespace ac_detail::prefix_sum {
template <class T>
inline constexpr bool tuple_like_v = requires {
    typename std::tuple_size<std::remove_cvref_t<T>>::type;
};

template <class T>
T add(const T& lhs, const T& rhs) {
    if constexpr (tuple_like_v<T>) {
        return [&]<size_t... I>(std::index_sequence<I...>) {
            return T{add(std::get<I>(lhs), std::get<I>(rhs))...};
        }(std::make_index_sequence<std::tuple_size_v<T>>{});
    } else {
        return lhs + rhs;
    }
}
}  // namespace ac_detail::prefix_sum

template <class T>
vector<T> prefix_sum(const vector<T>& a) {
    vector<T> sum(a.size() + 1, T{});
    for (size_t i = 0; i < a.size(); ++i) {
        sum[i + 1] = ac_detail::prefix_sum::add(sum[i], a[i]);
    }
    return sum;
}

// @acf fn prefix_sum2d — (const vector<vector<T>>& a) -> vector<vector<T>> — 2次元累積和。返り値は (H+1)×(W+1)
/// @acf-doc @brief 番兵付き2次元累積和を返す。行 `[r1,r2)`・列 `[c1,c2)` の和は `sum[r2][c2]-sum[r1][c2]-sum[r2][c1]+sum[r1][c1]`。計算量・追加メモリ O(HW)
/// @acf-doc @code (const vector<vector<T>>& a) -> vector<vector<T>>
/// @acf-doc [function] prefix_sum2d
template <class T>
vector<vector<T>> prefix_sum2d(const vector<vector<T>>& a) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(a));
#endif
    const size_t h = a.size();
    const size_t w = a.empty() ? 0 : a.front().size();
    vector<vector<T>> sum(h + 1, vector<T>(w + 1, T{}));
    for (size_t r = 0; r < h; ++r) {
        T row_sum{};
        for (size_t c = 0; c < w; ++c) {
            row_sum = row_sum + a[r][c];
            sum[r + 1][c + 1] = sum[r][c + 1] + row_sum;
        }
    }
    return sum;
}

// @acf fn prefix_sum3d — (const vector<vector<vector<T>>>& a) -> vector<vector<vector<T>>> — 3次元累積和。返り値は (D+1)×(H+1)×(W+1)
/// @acf-doc @brief `[depth][row][column]` 順の番兵付き3次元累積和を返す。半開直方体 `[d1,d2)×[r1,r2)×[c1,c2)` は8頂点の包除で求める。計算量・追加メモリ O(DHW)
/// @acf-doc @code (const vector<vector<vector<T>>>& a) -> vector<vector<vector<T>>>
/// @acf-doc [function] prefix_sum3d
template <class T>
vector<vector<vector<T>>> prefix_sum3d(const vector<vector<vector<T>>>& a) {
#ifdef LOCAL
    assert(ac_detail::grid::rectangular3d(a));
#endif
    const size_t d = a.size();
    const size_t h = a.empty() ? 0 : a.front().size();
    const size_t w = h == 0 ? 0 : a.front().front().size();
    vector<vector<vector<T>>> sum(
        d + 1, vector<vector<T>>(h + 1, vector<T>(w + 1, T{})));
    for (size_t z = 0; z < d; ++z)
        for (size_t r = 0; r < h; ++r)
            for (size_t c = 0; c < w; ++c) sum[z + 1][r + 1][c + 1] = a[z][r][c];
    for (size_t z = 1; z <= d; ++z)
        for (size_t r = 1; r <= h; ++r)
            for (size_t c = 1; c <= w; ++c)
                sum[z][r][c] = sum[z][r][c] + sum[z - 1][r][c];
    for (size_t z = 1; z <= d; ++z)
        for (size_t r = 1; r <= h; ++r)
            for (size_t c = 1; c <= w; ++c)
                sum[z][r][c] = sum[z][r][c] + sum[z][r - 1][c];
    for (size_t z = 1; z <= d; ++z)
        for (size_t r = 1; r <= h; ++r)
            for (size_t c = 1; c <= w; ++c)
                sum[z][r][c] = sum[z][r][c] + sum[z][r][c - 1];
    return sum;
}

namespace ac_detail::nested_sum {
template <class T>
using bare_t = ac_detail::io::remove_cvref_t<T>;

template <class T>
using element_t = ac_detail::io::remove_cvref_t<
    decltype(*std::begin(declval<const bare_t<T>&>()))>;

template <class T, bool IsRange = ac_detail::io::is_range_v<bare_t<T>>>
struct leaf_type {
    using type = bare_t<T>;
};

template <class T>
struct leaf_type<T, true> : leaf_type<element_t<T>> {};

template <class T>
using leaf_type_t = typename leaf_type<T>::type;

// 1つの軸を集約した結果のコンテナ構造。出力は入力コンテナの種類によらず vector に統一する。
template <class T, bool IsRange = ac_detail::io::is_range_v<bare_t<T>>>
struct reduced_shape {
    using type = bare_t<T>;
};

template <class T>
struct reduced_shape<T, true> {
    using type = vector<typename reduced_shape<element_t<T>>::type>;
};

template <class T>
using reduced_shape_t = typename reduced_shape<T>::type;

template <size_t Axis, class T>
struct axis_result;

template <class T>
struct axis_result<0, T> {
    static_assert(ac_detail::io::is_range_v<bare_t<T>>,
                  "axis reduction axis is deeper than the nested range");
    using type = reduced_shape_t<element_t<T>>;
};

template <size_t Axis, class T>
struct axis_result {
    static_assert(ac_detail::io::is_range_v<bare_t<T>>,
                  "axis reduction axis is deeper than the nested range");
    using type = vector<typename axis_result<Axis - 1, element_t<T>>::type>;
};

template <size_t Axis, class T>
using axis_result_t = typename axis_result<Axis, T>::type;

template <class T>
reduced_shape_t<T> zero_like(const T& value) {
    if constexpr (ac_detail::io::is_range_v<bare_t<T>>) {
        reduced_shape_t<T> result;
        for (const auto& child : value) result.push_back(zero_like(child));
        return result;
    } else {
        return {};
    }
}

template <class Out, class In>
void add_assign(Out& out, const In& in) {
    if constexpr (ac_detail::io::is_range_v<bare_t<In>>) {
        auto out_it = std::begin(out);
        auto in_it = std::begin(in);
        const auto out_end = std::end(out);
        const auto in_end = std::end(in);
        for (; out_it != out_end && in_it != in_end; ++out_it, ++in_it)
            add_assign(*out_it, *in_it);
#ifdef LOCAL
        assert(out_it == out_end && in_it == in_end);
#endif
    } else {
        out += in;
    }
}

template <class A, class B>
bool same_shape(const A& a, const B& b) {
    constexpr bool a_range = ac_detail::io::is_range_v<bare_t<A>>;
    constexpr bool b_range = ac_detail::io::is_range_v<bare_t<B>>;
    if constexpr (a_range != b_range) {
        return false;
    } else if constexpr (a_range) {
        auto a_it = std::begin(a);
        auto b_it = std::begin(b);
        const auto a_end = std::end(a);
        const auto b_end = std::end(b);
        for (; a_it != a_end && b_it != b_end; ++a_it, ++b_it)
            if (!same_shape(*a_it, *b_it)) return false;
        return a_it == a_end && b_it == b_end;
    } else {
        return true;
    }
}

template <class T>
bool rectangular(const T& value) {
    if constexpr (!ac_detail::io::is_range_v<bare_t<T>>) {
        return true;
    } else {
        auto it = std::begin(value);
        const auto end = std::end(value);
        if (it == end) return true;
        const auto& first = *it;
        ++it;
        for (; it != end; ++it)
            if (!same_shape(first, *it) || !rectangular(*it)) return false;
        return rectangular(first);
    }
}

template <size_t Axis, class Range>
axis_result_t<Axis, Range> axis_sum_impl(const Range& input) {
    using Bare = bare_t<Range>;
    static_assert(ac_detail::io::is_range_v<Bare>,
                  "axis_sum requires a nested range");
    using Result = axis_result_t<Axis, Range>;
    if constexpr (Axis == 0) {
        Result result{};
        auto it = std::begin(input);
        const auto end = std::end(input);
        if (it == end) return result;
        result = zero_like(*it);
        for (; it != end; ++it) add_assign(result, *it);
        return result;
    } else {
        Result result;
        for (const auto& child : input)
            result.push_back(axis_sum_impl<Axis - 1>(child));
        return result;
    }
}

template <class T>
reduced_shape_t<T> copy_like(const T& value) {
    if constexpr (ac_detail::io::is_range_v<bare_t<T>>) {
        reduced_shape_t<T> result;
        for (const auto& child : value) result.push_back(copy_like(child));
        return result;
    } else {
        return value;
    }
}

template <class Out, class In, class Compare>
void extreme_assign(Out& out, const In& in, Compare compare) {
    if constexpr (ac_detail::io::is_range_v<bare_t<In>>) {
        auto out_it = std::begin(out);
        auto in_it = std::begin(in);
        const auto out_end = std::end(out);
        const auto in_end = std::end(in);
        for (; out_it != out_end && in_it != in_end; ++out_it, ++in_it)
            extreme_assign(*out_it, *in_it, compare);
#ifdef LOCAL
        assert(out_it == out_end && in_it == in_end);
#endif
    } else if (compare(in, out)) {
        out = in;
    }
}

template <size_t Axis, class Range, class Compare>
axis_result_t<Axis, Range> axis_extreme_impl(
    const Range& input, Compare compare) {
    using Bare = bare_t<Range>;
    static_assert(ac_detail::io::is_range_v<Bare>,
                  "axis_min/axis_max requires a nested range");
    using Result = axis_result_t<Axis, Range>;
    if constexpr (Axis == 0) {
        Result result{};
        auto it = std::begin(input);
        const auto end = std::end(input);
        if (it == end) {
            if constexpr (ac_detail::io::is_range_v<bare_t<Result>>) {
                return result;
            } else {
                throw invalid_argument(
                    "axis_min/axis_max: reduced axis must be non-empty");
            }
        }
        result = copy_like(*it);
        for (++it; it != end; ++it) extreme_assign(result, *it, compare);
        return result;
    } else {
        Result result;
        for (const auto& child : input)
            result.push_back(axis_extreme_impl<Axis - 1>(child, compare));
        return result;
    }
}

template <class Range, class Projection>
using projected_leaf_t = std::remove_cvref_t<
    std::invoke_result_t<Projection&, const leaf_type_t<Range>&>>;

template <class Range, class Projection>
projected_leaf_t<Range, Projection> sum_by_impl(
    const Range& value, Projection& projection) {
    using Result = projected_leaf_t<Range, Projection>;
    if constexpr (ac_detail::io::is_range_v<bare_t<Range>>) {
        Result result{};
        for (const auto& child : value)
            result += sum_by_impl(child, projection);
        return result;
    } else {
        return std::invoke(projection, value);
    }
}

template <class Range, class Projection>
projected_leaf_t<Range, Projection> product_by_impl(
    const Range& value, Projection& projection) {
    using Result = projected_leaf_t<Range, Projection>;
    if constexpr (ac_detail::io::is_range_v<bare_t<Range>>) {
        Result result = 1;
        for (const auto& child : value)
            result *= product_by_impl(child, projection);
        return result;
    } else {
        return std::invoke(projection, value);
    }
}
}  // namespace ac_detail::nested_sum

// @acf fn sum_by — (const nested range& a, Projection projection) -> scalar / (const nested range& a) -> scalar — 葉を変換して全体を加算
/// @acf-doc @brief 任意次元の nested range の葉に射影を適用して加算する。射影を省略した場合は値をそのまま使う
/// @acf-doc @code (const nested range& a, Projection projection) -> scalar / (const nested range& a) -> scalar
/// @acf-doc [function] sum_by
template <class Range, class Projection>
auto sum_by(const Range& value, Projection projection) {
    return ac_detail::nested_sum::sum_by_impl(value, projection);
}
template <class Range>
auto sum_by(const Range& value) {
    std::identity projection;
    return ac_detail::nested_sum::sum_by_impl(value, projection);
}

// @acf fn sum_as — <T>(const nested range& value) -> T — 指定した型で総和を計算
/// @acf-doc @brief 各葉を加算前に`static_cast<T>`で変換する。戻り値はT、空rangeはT{}。入力のコピーや中間配列は作らない。訪問する要素・部分rangeの総数Nに対して時間O(N)、再帰用の追加メモリO(次元数)。変換と加算のコストを除く
/// @acf-doc @code <T>(const nested range& value) -> T
/// @acf-doc [function] sum_as
template <class T, class Range>
T sum_as(const Range& value) {
    return sum_by(value, [](const auto& leaf) -> T { return static_cast<T>(leaf); });
}

// @acf fn average — (const range& a) -> long double — 平均
/// @acf-doc @brief 1次元の数値列の平均を返す。空でないこと。計算量 O(N)
/// @acf-doc @code average(a) -> long double
/// @acf-doc [function] average
template <class Range>
long double average(const Range& a) {
    long double total = 0;
    size_t count = 0;
    for (const auto& x : a) {
        total += static_cast<long double>(x);
        ++count;
    }
    assert(count > 0);
    return total / count;
}

// @acf fn variance — (const range& a) -> long double — 分散
/// @acf-doc @brief 平均からの偏差の二乗の平均を返す。空でなく、繰り返し走査できること。計算量 O(N)
/// @acf-doc @code variance(a) -> long double
/// @acf-doc [function] variance
template <class Range>
long double variance(const Range& a) {
    const long double avg = average(a);
    long double total = 0;
    size_t count = 0;
    for (const auto& x : a) {
        const long double diff = static_cast<long double>(x) - avg;
        total += diff * diff;
        ++count;
    }
    return total / count;
}

// @acf fn stddev — (const range& a) -> long double — 標準偏差
/// @acf-doc @brief 分散の平方根を返す。空でなく、繰り返し走査できること。計算量 O(N)
/// @acf-doc @code stddev(a) -> long double
/// @acf-doc [function] stddev
template <class Range>
long double stddev(const Range& a) {
    return sqrt(variance(a));
}

// @acf fn product_by — (const nested range& a, Projection projection) -> scalar / (const nested range& a) -> scalar — 葉を変換して全体を乗算
/// @acf-doc @brief 任意次元の nested range の葉に射影を適用して乗算する。射影を省略した場合は値をそのまま使う。空の配列は `1`
/// @acf-doc @code (const nested range& a, Projection projection) -> scalar / (const nested range& a) -> scalar
/// @acf-doc [function] product_by
template <class Range, class Projection>
auto product_by(const Range& value, Projection projection) {
    return ac_detail::nested_sum::product_by_impl(value, projection);
}
template <class Range>
auto product_by(const Range& value) {
    std::identity projection;
    return ac_detail::nested_sum::product_by_impl(value, projection);
}

// @acf fn sum_all — (const nested range& a) -> scalar — N次元配列の全要素の総和
/// @acf-doc @brief `sum_by(a)` と同じく、任意次元のネストした range の葉をすべて足す。空の配列は `T{}`。計算量 O(要素数)
/// @acf-doc @code (const nested range& a) -> leaf_type
/// @acf-doc [function] sum_all
template <class T>
ac_detail::nested_sum::leaf_type_t<T> sum_all(const T& value) {
    return sum_by(value);
}

// @acf fn product_all — (const nested range& a) -> scalar — N次元配列の全要素の積
/// @acf-doc @brief `product_by(a)` と同じく、任意次元のネストした range の葉をすべて掛ける。空の配列は `1`。計算量 O(要素数)
/// @acf-doc @code (const nested range& a) -> leaf_type
/// @acf-doc [function] product_all
template <class T>
ac_detail::nested_sum::leaf_type_t<T> product_all(const T& value) {
    return product_by(value);
}

namespace ac_detail::nested_count {
template <class T>
using bare_t = ac_detail::io::remove_cvref_t<T>;

template <class T>
inline constexpr bool is_text_v =
    is_same_v<bare_t<T>, string> || is_same_v<bare_t<T>, string_view>;

template <class Value, class Needle>
ll count_impl(const Value& value, const Needle& needle) {
    using Bare = bare_t<Value>;
    if constexpr (is_text_v<Bare>) {
        if constexpr (ac_detail::io::is_char_v<bare_t<Needle>>) {
            ll result = 0;
            for (const auto c : value) result += static_cast<ll>(c == needle);
            return result;
        } else {
            return static_cast<ll>(value == needle);
        }
    } else if constexpr (ac_detail::io::is_range_v<Bare>) {
        ll result = 0;
        for (const auto& child : value) result += count_impl(child, needle);
        return result;
    } else {
        return static_cast<ll>(value == needle);
    }
}
}  // namespace ac_detail::nested_count

// @acf fn count_all — (const nested range& a, const value& target) -> ll — N次元配列の一致要素数
/// @acf-doc @brief ネストした range を再帰的にたどり、`target` と一致する葉の個数を返す。`string` / `string_view` に char を渡すと文字単位で数え、文字列を渡すと文字列単位で比較する。計算量 O(要素数)
/// @acf-doc @code (const nested range& a, const value& target) -> ll
/// @acf-doc [function] count_all
template <class Range, class Value>
ll count_all(const Range& input, const Value& target) {
    return ac_detail::nested_count::count_impl(input, target);
}

// @acf fn axis_sum — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の和
/// @acf-doc @brief `axis_sum<Axis>(a)` は指定した軸を消し、その軸方向に要素を足した nested `vector` を返す。外側の軸を0とし、2次元なら `axis_sum<1>` が行和、`axis_sum<0>` が列和。入力は長方形（各階層の長さが一定）を想定し、LOCAL では検査する。計算量 O(要素数)
/// @acf-doc @code axis_sum<Axis>(a) -> nested vector
/// @acf-doc [function] axis_sum
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_sum(const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_sum_impl<Axis>(input);
}

// @acf fn axis_min — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の最小値
/// @acf-doc @brief `axis_min<Axis>(a)` は指定した軸を消し、その軸方向の対応する要素ごとに最小値を取った nested `vector` を返す。外側の軸を0とし、2次元なら `axis_min<1>` が各行、`axis_min<0>` が各列。入力は長方形、潰す軸の各列は空でないことを想定する。計算量 O(要素数)
/// @acf-doc @code axis_min<Axis>(a) -> nested vector
/// @acf-doc [function] axis_min
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_min(
    const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_extreme_impl<Axis>(input, less<>{});
}

// @acf fn axis_max — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の最大値
/// @acf-doc @brief `axis_max<Axis>(a)` は指定した軸を消し、その軸方向の対応する要素ごとに最大値を取った nested `vector` を返す。外側の軸を0とし、2次元なら `axis_max<1>` が各行、`axis_max<0>` が各列。入力は長方形、潰す軸の各列は空でないことを想定する。計算量 O(要素数)
/// @acf-doc @code axis_max<Axis>(a) -> nested vector
/// @acf-doc [function] axis_max
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_max(
    const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_extreme_impl<Axis>(input, greater<>{});
}

// @acf fn row_sums — (const 2D range& a) -> vector — 2次元配列の行ごとの和
/// @acf-doc @brief 2次元配列の各行を足した `vector` を返す。`row_sums(a)` は `axis_sum<1>(a)` の別名。計算量 O(HW)
/// @acf-doc @code row_sums(a) -> vector
/// @acf-doc [function] row_sums
template <class Grid>
auto row_sums(const Grid& grid) -> decltype(axis_sum<1>(grid)) {
    return axis_sum<1>(grid);
}

// @acf fn col_sums — (const 2D range& a) -> vector — 2次元配列の列ごとの和
/// @acf-doc @brief 2次元配列の各列を足した `vector` を返す。`col_sums(a)` は `axis_sum<0>(a)` の別名。計算量 O(HW)
/// @acf-doc @code col_sums(a) -> vector
/// @acf-doc [function] col_sums
template <class Grid>
auto col_sums(const Grid& grid) -> decltype(axis_sum<0>(grid)) {
    return axis_sum<0>(grid);
}

namespace ac_detail::prefix_sum {
template <bool InclLo, bool InclHi, class L, class R>
pair<long long, long long> half_open(L lo, R hi) {
    const long long first = static_cast<long long>(lo) + (InclLo ? 0 : 1);
    const long long last = static_cast<long long>(hi) + (InclHi ? 1 : 0);
    return {first, last};
}

template <class T>
void assert_bound([[maybe_unused]] const vector<T>& sum,
                  [[maybe_unused]] long long first,
                  [[maybe_unused]] long long last) {
#ifdef LOCAL
    assert(0 <= first && first <= last);
    assert(last < static_cast<long long>(sum.size()));
#else
    (void)sum;
    (void)first;
    (void)last;
#endif
}
}  // namespace ac_detail::prefix_sum

// @acf fn range_sum — (prefix_sum, endpoints...) -> T — 1/2/3次元累積和の区間和。端点の開閉をテンプレート指定
/// @acf-doc @brief `prefix_sum*` の返り値から区間和を O(1) で返す。`true` は端点を含む。既定は `<true,false>` で半開区間。`<true,true>`=閉、`<false,false>`=開。2D/3D でテンプレート引数を2個だけ指定すると全軸に同じ開閉を使い、4個/6個で軸ごとに指定できる
/// @acf-doc @code template <bool InclLo=true, bool InclHi=false, ...> (const prefix_sum&, endpoints...) -> T
/// @acf-doc [function] range_sum
template <bool InclLo = true, bool InclHi = false, class T, class L, class R>
T range_sum(const vector<T>& sum, L lo, R hi) {
    const auto [first, last] =
        ac_detail::prefix_sum::half_open<InclLo, InclHi>(lo, hi);
    if (first >= last) return T{};
    ac_detail::prefix_sum::assert_bound(sum, first, last);
    return sum[static_cast<size_t>(last)] - sum[static_cast<size_t>(first)];
}

template <bool InclRowLo = true, bool InclRowHi = false,
          bool InclColLo = InclRowLo, bool InclColHi = InclRowHi,
          class T, class R1, class C1, class R2, class C2>
T range_sum(const vector<vector<T>>& sum, R1 r1, C1 c1, R2 r2, C2 c2) {
    const auto [top, bottom] =
        ac_detail::prefix_sum::half_open<InclRowLo, InclRowHi>(r1, r2);
    const auto [left, right] =
        ac_detail::prefix_sum::half_open<InclColLo, InclColHi>(c1, c2);
    if (top >= bottom || left >= right) return T{};
#ifdef LOCAL
    assert(ac_detail::grid::rectangular(sum));
#endif
    ac_detail::prefix_sum::assert_bound(sum, top, bottom);
    ac_detail::prefix_sum::assert_bound(sum.front(), left, right);
    return sum[static_cast<size_t>(bottom)][static_cast<size_t>(right)]
         - sum[static_cast<size_t>(top)][static_cast<size_t>(right)]
         - sum[static_cast<size_t>(bottom)][static_cast<size_t>(left)]
         + sum[static_cast<size_t>(top)][static_cast<size_t>(left)];
}

template <bool InclDepthLo = true, bool InclDepthHi = false,
          bool InclRowLo = InclDepthLo, bool InclRowHi = InclDepthHi,
          bool InclColLo = InclDepthLo, bool InclColHi = InclDepthHi,
          class T, class D1, class R1, class C1, class D2, class R2, class C2>
T range_sum(const vector<vector<vector<T>>>& sum,
            D1 d1, R1 r1, C1 c1, D2 d2, R2 r2, C2 c2) {
    const auto [front, back] =
        ac_detail::prefix_sum::half_open<InclDepthLo, InclDepthHi>(d1, d2);
    const auto [top, bottom] =
        ac_detail::prefix_sum::half_open<InclRowLo, InclRowHi>(r1, r2);
    const auto [left, right] =
        ac_detail::prefix_sum::half_open<InclColLo, InclColHi>(c1, c2);
    if (front >= back || top >= bottom || left >= right) return T{};
#ifdef LOCAL
    assert(ac_detail::grid::rectangular3d(sum));
#endif
    ac_detail::prefix_sum::assert_bound(sum, front, back);
    ac_detail::prefix_sum::assert_bound(sum.front(), top, bottom);
    ac_detail::prefix_sum::assert_bound(sum.front().front(), left, right);
    const auto at = [&](long long z, long long r, long long c) -> const T& {
        return sum[static_cast<size_t>(z)][static_cast<size_t>(r)]
                  [static_cast<size_t>(c)];
    };
    return at(back, bottom, right) - at(front, bottom, right)
         - at(back, top, right) - at(back, bottom, left)
         + at(front, top, right) + at(front, bottom, left)
         + at(back, top, left) - at(front, top, left);
}


// @acf fn fast_io — () -> bool — ios::sync_with_stdio(false); cin.tie(nullptr)。常に true
/// @acf-doc @brief ios::sync_with_stdio(false); cin.tie(nullptr)。常に true（`&&` チェーン可）
/// @acf-doc @code () -> bool
/// @acf-doc [function] fast_io
inline bool fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return true;
}

// Yes/No の既定表記。include 前に切り替える（同時指定は不可）。
// 問題が YES/NO なら `#define AC_YES_UPPER`、yes/no なら `#define AC_YES_LOWER`。
#if defined(AC_YES_UPPER) && defined(AC_YES_LOWER)
#error "define at most one of AC_YES_UPPER and AC_YES_LOWER"
#endif
#ifdef AC_YES_UPPER
inline constexpr const char* AC_YES_WORD = "YES";
inline constexpr const char* AC_NO_WORD = "NO";
#elif defined(AC_YES_LOWER)
inline constexpr const char* AC_YES_WORD = "yes";
inline constexpr const char* AC_NO_WORD = "no";
#else
inline constexpr const char* AC_YES_WORD = "Yes";
inline constexpr const char* AC_NO_WORD = "No";
#endif

// @acf fn Yes — (bool nl=true) -> bool — AC_YES_WORD を出力（既定 "Yes"）。常に true
/// @acf-doc @brief 既定は `"Yes"`。`AC_YES_UPPER` で `"YES"`、`AC_YES_LOWER` で `"yes"`。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (bool nl=true) -> bool
/// @acf-doc [function] Yes
inline bool Yes(bool nl = true) {
    cout << AC_YES_WORD << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn No — (bool nl=true) -> bool — AC_NO_WORD を出力（既定 "No"）。常に true
/// @acf-doc @brief 既定は `"No"`。`AC_YES_UPPER` で `"NO"`、`AC_YES_LOWER` で `"no"`。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (bool nl=true) -> bool
/// @acf-doc [function] No
inline bool No(bool nl = true) {
    cout << AC_NO_WORD << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn YN — (bool cond, bool nl=true) -> bool — cond なら Yes、でなければ No
/// @acf-doc @brief cond なら Yes、でなければ No（`AC_YES_UPPER` / `AC_YES_LOWER` 反映）。戻り値は常に true
/// @acf-doc @code (bool cond, bool nl=true) -> bool
/// @acf-doc [function] YN
inline bool YN(bool cond, bool nl = true) {
    return cond ? Yes(nl) : No(nl);
}
// @acf fn YN — (const First& first, const Second& second, const Rest&... rest) -> bool — 複数の bool 相当の条件を 1 条件 1 行で Yes/No 出力
/// @acf-doc @brief `YN(a,b,c)` のように複数の bool 相当の条件を渡すと、各条件を1行ずつ Yes/No 出力。3引数以上で使用し、2引数の `YN(cond,nl)` との互換性を保つ
/// @acf-doc @code (const First& first, const Second& second, const Rest&... rest) -> bool
/// @acf-doc [function] YN
template <class First, class Second, class... Rest,
          enable_if_t<
              (sizeof...(Rest) >= 1) &&
                  is_convertible_v<const First&, bool> &&
                  is_convertible_v<const Second&, bool> &&
                  (is_convertible_v<const Rest&, bool> && ...),
              int> = 0>
bool YN(const First& first, const Second& second, const Rest&... rest) {
    YN(static_cast<bool>(first));
    YN(static_cast<bool>(second));
    (YN(static_cast<bool>(rest)), ...);
    return true;
}
// @acf fn YN — (const R& r, bool nl=true) -> bool — range の各要素を Yes/No で 1 行ずつ出力
/// @acf-doc @brief `vb` / `vc` / `vi` など bool 相当の range を 1 要素 1 行で Yes/No 出力。空 range は無出力
/// @acf-doc @code (const R& r, bool nl=true) -> bool
/// @acf-doc [function] YN
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool YN(const R& r, bool nl = true) {
    if (std::empty(r)) return ac_detail::output::finish();
    for (const auto& e : r) YN(static_cast<bool>(e), nl);
    return true;
}
// @acf fn YES — (bool nl=true) -> bool — 常に "YES" を出力。常に true
/// @acf-doc @brief 常に `"YES"`（`AC_YES_UPPER` / `AC_YES_LOWER` とは独立）。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (bool nl=true) -> bool
/// @acf-doc [function] YES
inline bool YES(bool nl = true) {
    cout << "YES" << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn NO — (bool nl=true) -> bool — 常に "NO" を出力。常に true
/// @acf-doc @brief 常に `"NO"`（`AC_YES_UPPER` / `AC_YES_LOWER` とは独立）。通常はflushせず、`AC_INTERACTIVE`時だけ末尾でflush。戻り値は常に true
/// @acf-doc @code (bool nl=true) -> bool
/// @acf-doc [function] NO
inline bool NO(bool nl = true) {
    cout << "NO" << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}

// 明示的な出力API。旧 print / println / YN は互換用として残す。
// ac::out 系は引数から区切り文字・改行オプションを推測しない。
// 通常はバッファ付き。対話問題では ac::flush() または AC_INTERACTIVE を使う。
namespace ac {
namespace output_detail {
inline bool finish() {
#ifdef AC_INTERACTIVE
    cout.flush();
#endif
    return true;
}

template <class T>
void write_lines(const T& value) {
    if constexpr (ac_detail::io::is_range_v<T>) {
        for (const auto& item : value)
            ac_detail::output::writeln(cout, ' ', item);
    } else {
        ac_detail::output::writeln(cout, ' ', value);
    }
}
}  // namespace output_detail

/// @brief 引数をデータとして空白区切りで出力し、最後に改行する。常に true。
/// `ac::out(v, ':')` の ':' は区切り文字ではなく、出力するデータ。
template <class... T>
bool out(const T&... values) {
    ac_detail::output::writeln(cout, ' ', values...);
    return output_detail::finish();
}

/// @brief 1引数・1要素につき1行出力する。空rangeは無出力、引数なしは空行。
template <class... T>
bool out_lines(const T&... values) {
    if constexpr (sizeof...(T) == 0) cout << '\n';
    else (output_detail::write_lines(values), ...);
    return output_detail::finish();
}

/// @brief rangeを指定した文字列で区切って出力し、最後に改行する。
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool out_join(const R& values, string_view separator = " ") {
    ac_detail::output::write_range(cout, values, separator);
    cout << '\n';
    return output_detail::finish();
}

template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool out_join(const R& values, char separator) {
    ac_detail::output::write_range(cout, values, separator);
    cout << '\n';
    return output_detail::finish();
}

/// @brief 1条件だけをYes/Noで出力する。AC_YES_UPPER / AC_YES_LOWERを反映。
inline bool yesno(bool condition) {
    cout << (condition ? AC_YES_WORD : AC_NO_WORD) << '\n';
    return output_detail::finish();
}

/// @brief 各条件を1行ずつ出力する。2引数でも、両方を条件として扱う。
template <class... T,
          enable_if_t<(sizeof...(T) > 0) &&
                          (is_convertible_v<const T&, bool> && ...),
                      int> = 0>
bool yesno_each(const T&... conditions) {
    ((cout << (static_cast<bool>(conditions) ? AC_YES_WORD : AC_NO_WORD)
           << '\n'), ...);
    return output_detail::finish();
}

template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool yesno_each(const R& conditions) {
    for (const auto& condition : conditions)
        cout << (static_cast<bool>(condition) ? AC_YES_WORD : AC_NO_WORD) << '\n';
    return output_detail::finish();
}

/// @brief 対話問題などでstdoutを明示的にflushする。常に true。
inline bool flush() {
    cout.flush();
    return true;
}
}  // namespace ac

// @acf fn has_bit — (ull mask, int i) -> bool — i番目のbitが立っているか
/// @acf-doc @brief 0始まりのbit位置iを判定する。0 <= i < 64。時間・追加メモリ O(1)
/// @acf-doc @code (ull mask, int i) -> bool
/// @acf-doc [function] has_bit
constexpr bool has_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return (mask >> i) & 1ULL;
}

// @acf fn set_bit — (ull mask, int i) -> ull — i番目のbitを立てた値
/// @acf-doc @brief 0始まりのbit位置iを1にした値を返す。入力は変更しない。0 <= i < 64。時間・追加メモリ O(1)
/// @acf-doc @code (ull mask, int i) -> ull
/// @acf-doc [function] set_bit
constexpr ull set_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask | (1ULL << i);
}

// @acf fn clear_bit — (ull mask, int i) -> ull — i番目のbitを消した値
/// @acf-doc @brief 0始まりのbit位置iを0にした値を返す。入力は変更しない。0 <= i < 64。時間・追加メモリ O(1)
/// @acf-doc @code (ull mask, int i) -> ull
/// @acf-doc [function] clear_bit
constexpr ull clear_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask & ~(1ULL << i);
}

// @acf fn toggle_bit — (ull mask, int i) -> ull — i番目のbitを反転した値
/// @acf-doc @brief 0始まりのbit位置iを反転した値を返す。入力は変更しない。0 <= i < 64。時間・追加メモリ O(1)
/// @acf-doc @code (ull mask, int i) -> ull
/// @acf-doc [function] toggle_bit
constexpr ull toggle_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask ^ (1ULL << i);
}

// @acf fn subsets — (int n) -> iota_view<ull, ull> — n要素の全部分集合を昇順に列挙
/// @acf-doc @brief 0 <= n < 64。maskを [0, 2^n) の昇順に遅延列挙する。n=0は空集合0のみ。生成 O(1)、全列挙 O(2^n)、追加メモリ O(1)
/// @acf-doc @code (int n) -> ranges::iota_view<ull, ull>
/// @acf-doc [function] subsets
constexpr auto subsets(int n) {
#ifdef LOCAL
    assert(0 <= n && n < numeric_limits<ull>::digits);
#endif
    return views::iota(0ULL, 1ULL << n);
}

// @acf fn nonempty_subsets — (int n) -> iota_view<ull, ull> — 空集合を除く全部分集合を昇順に列挙
/// @acf-doc @brief 0 <= n < 64。maskを [1, 2^n) の昇順に遅延列挙する。n=0は空range。生成 O(1)、全列挙 O(2^n)、追加メモリ O(1)
/// @acf-doc @code (int n) -> ranges::iota_view<ull, ull>
/// @acf-doc [function] nonempty_subsets
constexpr auto nonempty_subsets(int n) {
#ifdef LOCAL
    assert(0 <= n && n < numeric_limits<ull>::digits);
#endif
    return views::iota(1ULL, 1ULL << n);
}

// @acf fn bit_count — (T x) -> int — 整数の立っているビット数
/// @acf-doc @brief bool以外の整数型（i128/u128含む）。同幅の符号なし型へ変換して数える。負数は絶対値ではなくbit表現を扱う
/// @acf-doc @code (T x) -> int
/// @acf-doc [function] bit_count
template <class T, enable_if_t<is_integral_v<T> && !is_same_v<T, bool>, int> = 0>
constexpr int bit_count(T x) {
    using U = make_unsigned_t<T>;
    const U bits = static_cast<U>(x);
    constexpr int word_bits = numeric_limits<ull>::digits;
    int count = __builtin_popcountll(static_cast<ull>(bits));
    if constexpr (sizeof(U) > sizeof(ull)) {
        count += __builtin_popcountll(static_cast<ull>(bits >> word_bits));
    }
    return count;
}
// @acf fn bit_ctz — (T x) -> int — 末尾ゼロビット数（0は入力型のビット幅）
/// @acf-doc @brief bool以外の整数。0は同幅の符号なし型のビット数（intなら通常32、llなら64、i128なら128）。結果が幅と等しいときはbit位置として使わない
/// @acf-doc @code (T x) -> int
/// @acf-doc [function] bit_ctz
template <class T, enable_if_t<is_integral_v<T> && !is_same_v<T, bool>, int> = 0>
constexpr int bit_ctz(T x) {
    using U = make_unsigned_t<T>;
    const U bits = static_cast<U>(x);
    if (bits == 0) return numeric_limits<U>::digits;
    const ull low = static_cast<ull>(bits);
    if (low != 0) return __builtin_ctzll(low);
    if constexpr (sizeof(U) > sizeof(ull)) {
        constexpr int word_bits = numeric_limits<ull>::digits;
        return word_bits + __builtin_ctzll(static_cast<ull>(bits >> word_bits));
    }
    return numeric_limits<U>::digits;
}
// @acf fn for_each_bit — (ull mask, F f) -> void — 立っているbit位置を昇順に処理
/// @acf-doc @brief 立っている各bitの0始まりの位置iでf(i)を呼ぶ。mask=0は呼ばない。入力maskは変更しない。時間 O(bit_count(mask))（各呼び出しを除く）、追加メモリ O(1)
/// @acf-doc @code (ull mask, F f) -> void
/// @acf-doc [function] for_each_bit
template <class F>
constexpr void for_each_bit(ull mask, F f) {
    while (mask) {
        std::invoke(f, bit_ctz(mask));
        mask &= mask - 1;
    }
}

// @acf fn bit_length — (T x) -> int — 同幅の符号なし値のビット幅（0は0）
/// @acf-doc @brief bool以外の整数。0は0、負数は絶対値ではなく同幅の符号なし値として扱う。int/long/ll/size_t/i128/u128を暗黙に切り詰めない
/// @acf-doc @code (T x) -> int
/// @acf-doc [function] bit_length
template <class T, enable_if_t<is_integral_v<T> && !is_same_v<T, bool>, int> = 0>
constexpr int bit_length(T x) {
    using U = make_unsigned_t<T>;
    const U bits = static_cast<U>(x);
    constexpr int word_bits = numeric_limits<ull>::digits;
    if constexpr (sizeof(U) > sizeof(ull)) {
        const ull high = static_cast<ull>(bits >> word_bits);
        if (high != 0) return 2 * word_bits - __builtin_clzll(high);
    }
    const ull low = static_cast<ull>(bits);
    return low != 0 ? word_bits - __builtin_clzll(low) : 0;
}

namespace ac_detail {
template <class Index>
int checked_range_index(Index index) {
    if (index > numeric_limits<int>::max())
        throw overflow_error("range index exceeds int maximum");
    return static_cast<int>(index);
}
} // namespace ac_detail

// @acf fn iter_idx — (random-access range, iterator) -> int — iterator を添字へ変換
/// @acf-doc @brief random-access range の `begin()` から iterator までの 0-based 添字を O(1) で返す。`end()` は `sz(range)`。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (R& range, iterator it) -> int
/// @acf-doc [function] iter_idx
template <ranges::random_access_range R>
int iter_idx(R& range, ranges::iterator_t<R> it) {
    return ac_detail::checked_range_index(it - ranges::begin(range));
}

// @acf fn idx_iter — (random-access range, int idx) -> iterator — 添字を iterator へ変換
/// @acf-doc @brief random-access range の 0-based 添字を O(1) で iterator へ変換。`idx == sz(range)` は `end()`。範囲外は未定義
/// @acf-doc @code (R& range, int idx) -> iterator
/// @acf-doc [function] idx_iter
template <ranges::random_access_range R>
auto idx_iter(R& range, int idx) {
    using D = ranges::range_difference_t<R>;
    return ranges::begin(range) + static_cast<D>(idx);
}

// @acf fn lower_bound_idx — (const vector<T>& a, const T& x, Comp comp={}) -> int — lower_bound の添字
/// @acf-doc @brief `comp` と `projection` の順序で整列した列に対する `lower_bound` の0始まりの添字。境界がなければ要素数。既存のvector版に加え、forward rangeとprojectionを受け取る。既定は昇順、降順は `greater{}`。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。入力は変更しない。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (const vector<T>& a, const T& x, Comp comp={}) / (forward range, const T& target, Comp comp={}, Proj projection={}) -> int
/// @acf-doc [function] lower_bound_idx
template <class T, class Comp = less<>>
int lower_bound_idx(const vector<T>& a, const T& x, Comp comp = {}) {
    return ac_detail::checked_range_index(lower_bound(a.begin(), a.end(), x, comp) - a.begin());
}
// @acf fn upper_bound_idx — (const vector<T>& a, const T& x, Comp comp={}) -> int — upper_bound の添字
/// @acf-doc @brief `comp` と `projection` の順序で整列した列に対する `upper_bound` の0始まりの添字。境界がなければ要素数。既存のvector版に加え、forward rangeとprojectionを受け取る。既定は昇順、降順は `greater{}`。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。入力は変更しない。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (const vector<T>& a, const T& x, Comp comp={}) / (forward range, const T& target, Comp comp={}, Proj projection={}) -> int
/// @acf-doc [function] upper_bound_idx
template <class T, class Comp = less<>>
int upper_bound_idx(const vector<T>& a, const T& x, Comp comp = {}) {
    return ac_detail::checked_range_index(upper_bound(a.begin(), a.end(), x, comp) - a.begin());
}
// @acf fn binsearch — (ll ok, ll ng, Pred check) -> ll — 答えの二分探索（ok 側を残す）
/// @acf-doc @brief `check` が単調なとき、`ok`（可）と `ng`（不可）の境界で可側を返す。`|ok-ng|>1` のあいだ mid を更新。`std::binary_search` とは別物
/// @acf-doc @code (ll ok, ll ng, Pred check) -> ll
/// @acf-doc [function] binsearch
template <class Pred>
ll binsearch(ll ok, ll ng, Pred check) {
    while (true) {
        const ll mid = std::midpoint(ok, ng);
        if (mid == ok || mid == ng) break;
        (check(mid) ? ok : ng) = mid;
    }
    return ok;
}

// @acf fn ceil_div — (ll a, ll b) -> ll — 負数対応の切り上げ除算
/// @acf-doc @brief b!=0。負数を含め、数学的な a/b の切り上げを返す
/// @acf-doc @code (ll a, ll b) -> ll
/// @acf-doc [function] ceil_div
inline ll ceil_div(ll a, ll b) {
    if (b == 0) throw invalid_argument("ceil_div: division by zero");
    if (a == numeric_limits<ll>::min() && b == -1) {
        throw overflow_error("ceil_div: result does not fit in long long");
    }
    const ll q = a / b;
    const ll r = a % b;
    return q + (r != 0 && ((r > 0) == (b > 0)));
}
// @acf fn floor_div — (ll a, ll b) -> ll — 負数対応の床除算
/// @acf-doc @brief 負数対応の床除算
/// @acf-doc @code (ll a, ll b) -> ll
/// @acf-doc [function] floor_div
inline ll floor_div(ll a, ll b) {
    if (b == 0) throw invalid_argument("floor_div: division by zero");
    if (a == numeric_limits<ll>::min() && b == -1) {
        throw overflow_error("floor_div: result does not fit in long long");
    }
    const ll q = a / b;
    const ll r = a % b;
    return q - (r != 0 && ((r > 0) != (b > 0)));
}

// @acf fn compress — (vector<T>& a) -> bool — 座標圧縮して 0..k-1 に書き換え。常に true
/// @acf-doc @brief 座標圧縮して 0..k-1 に書き換え。常に true（`&&` チェーン可）
/// @acf-doc @code (vector<T>& a) -> bool
/// @acf-doc [function] compress
template <class T>
bool compress(vector<T>& a) {
    vector<T> vals = a;
    sort(ALL(vals));
    vals.erase(unique(ALL(vals)), vals.end());
    for (auto& x : a) x = (T)(lower_bound(ALL(vals), x) - vals.begin());
    return true;
}

// @acf fn sort_by_value — (const map<K,V,KeyComp,Alloc>& values, Comp comp={}) -> vector<pair<K,V>> — mapを値順に並べた列
/// @acf-doc @brief mapの要素を値（second）順に並べたvectorで返す。省略時は昇順、greater{}なら降順。同値は元mapのキー順。入力は変更しない。比較回数 O(N log N)、追加メモリ O(N)（比較・コピーのコストを除く）
/// @acf-doc @code (const map<K,V,KeyComp,Alloc>& values, Comp comp={}) -> vector<pair<K,V>>
/// @acf-doc [function] sort_by_value
template <class Key, class Value, class KeyComp, class Alloc, class Comp = less<>>
vector<pair<Key, Value>> sort_by_value(
    const map<Key, Value, KeyComp, Alloc>& values, Comp comp = {}) {
    vector<pair<Key, Value>> result(values.begin(), values.end());
    const auto key_comp = values.key_comp();
    std::sort(result.begin(), result.end(), [&](const auto& a, const auto& b) {
        if (std::invoke(comp, a.second, b.second)) return true;
        if (std::invoke(comp, b.second, a.second)) return false;
        return std::invoke(key_comp, a.first, b.first);
    });
    return result;
}

template <class T>
concept pair_like = requires(const T& value) {
    value.first;
    value.second;
};

// @acf fn swap_key_value — (const range) -> map<value_t,int> — インデックスと要素を入れ替える
/// @acf-doc @brief 1次元配列・set の値をキー、0始まりのインデックスを値として map に変換。重複値があると `invalid_argument`
/// @acf-doc @code (const Range& values) -> map<value_t, int>
/// @acf-doc [function] swap_key_value
template <class R>
requires(std::ranges::input_range<R> &&
         !pair_like<std::ranges::range_value_t<R>> &&
         !ac_detail::io::is_range_v<std::ranges::range_value_t<R>>)
map<std::decay_t<std::ranges::range_value_t<R>>, int> swap_key_value(
    const R& values) {
    using Value = std::decay_t<std::ranges::range_value_t<R>>;
    map<Value, int> result;
    int index = 0;
    for (const auto& value : values) {
        const auto inserted = result.try_emplace(value, index).second;
        if (!inserted)
            throw invalid_argument("swap_key_value: duplicated values are not invertible");
        ++index;
    }
    return result;
}

// @acf fn swap_key_value — (const range) -> map<value_t,key_t> — map / pair列の key/value を入れ替える
/// @acf-doc @brief `pair_like<T>::first` を key、`pair_like<T>::second` を value とみなして入れ替える。重複 value があると `invalid_argument`
/// @acf-doc @code (const Range& values) -> map<value_t, key_t>
/// @acf-doc [function] swap_key_value
template <class R>
requires(std::ranges::input_range<R> &&
         pair_like<std::ranges::range_value_t<R>> &&
         !ac_detail::io::is_range_v<std::ranges::range_value_t<R>>)
map<std::decay_t<decltype(std::declval<std::ranges::range_value_t<R>>().second)>,
    std::decay_t<decltype(std::declval<std::ranges::range_value_t<R>>().first)>>
swap_key_value(const R& values) {
    using Pair = std::ranges::range_value_t<R>;
    using Key = std::decay_t<decltype(declval<Pair>().first)>;
    using Value = std::decay_t<decltype(declval<Pair>().second)>;
    map<Value, Key> result;
    for (const auto& [key, value] : values) {
        const auto inserted = result.try_emplace(value, key).second;
        if (!inserted)
            throw invalid_argument("swap_key_value: duplicated values are not invertible");
    }
    return result;
}

namespace ac_detail::algorithm {
template <class R>
using range_value_t = decay_t<decltype(*std::begin(declval<const R&>()))>;
}  // namespace ac_detail::algorithm

namespace ac_detail::nested_find {
template <class R, class V>
optional<vi> find_impl(const R& values, const V& target) {
    using Element = ac_detail::algorithm::range_value_t<R>;
    if constexpr (ac_detail::io::is_range_v<Element>) {
        int index = 0;
        for (const auto& child : values) {
            if (auto position = find_impl(child, target)) {
                position->insert(position->begin(), index);
                return position;
            }
            ++index;
        }
        return nullopt;
    } else {
        int index = 0;
        for (const auto& value : values) {
            if (value == target) return vi{index};
            ++index;
        }
        return nullopt;
    }
}
}  // namespace ac_detail::nested_find

// @acf fn find — (range, value) -> int / (string, string) -> int — 要素または部分文字列を左から探す
/// @acf-doc @brief 1次元 range では `target` と一致する最初の添字、ネストした range では最初の要素の座標、文字列同士では部分文字列 `target` の最初の添字を返す。1次元・文字列で見つからないときは要素数 `N`、ネストした range で見つからないときは空の `vi`
/// @acf-doc @code (range, const value& target) -> int
/// @acf-doc @code (string_view values, string_view target) -> int
/// @acf-doc @code (const nested range& values, const value& target) -> vi
/// @acf-doc [function] find
template <class R, class V>
requires (ac_detail::io::is_range_v<ac_detail::algorithm::range_value_t<R>>)
vi find(const R& values, const V& target) {
    if (auto position = ac_detail::nested_find::find_impl(values, target))
        return *position;
    return {};
}

template <class R, class V>
requires requires(const R& values, const V& target) {
    string_view(values);
    string_view(target);
}
int find(const R& values, const V& target) {
    const string_view source(values);
    const size_t position = source.find(string_view(target));
    return position == string_view::npos ? static_cast<int>(source.size())
                                         : static_cast<int>(position);
}

template <class R, class V>
int find(const R& values, const V& target) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "find requires a one-dimensional range");

    int index = 0;
    for (const auto& value : values) {
        if (value == target) return index;
        ++index;
    }
    return index;
}

// @acf fn rfind — (range, value) -> int — 配列から値を右から探す
/// @acf-doc @brief 1次元 range を左から走査し、`target` と一致する最後の添字を返す。見つからないときは要素数 `N`
/// @acf-doc @code (range, const value& target) -> int
/// @acf-doc [function] rfind
template <class R, class V>
int rfind(const R& values, const V& target) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "rfind requires a one-dimensional range");

    int index = 0;
    int result = 0;
    bool found = false;
    for (const auto& value : values) {
        if (value == target) {
            result = index;
            found = true;
        }
        ++index;
    }
    return found ? result : index;
}

// @acf fn mex — (range) -> int — 非負整数列の最小除外値
/// @acf-doc @brief 非負整数列に含まれない最小の整数を返す。負数は無視し、計算量は O(N)、追加メモリは O(N)
/// @acf-doc @code (range) -> int
/// @acf-doc [function] mex
template <class R>
int mex(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "mex requires an integral range");
    static_assert(!is_same_v<remove_cv_t<T>, bool>,
                  "mex does not accept bool ranges");

    vector<T> copied(std::begin(values), std::end(values));
    if (copied.size() >= static_cast<size_t>(numeric_limits<int>::max())) {
        throw invalid_argument("mex: range is too large");
    }
    const int limit = static_cast<int>(copied.size());
    vector<char> present(limit + 1, false);
    for (const T& raw : copied) {
        if constexpr (is_signed_v<T>) {
            if (raw < 0 || raw > static_cast<T>(limit)) continue;
        } else {
            if (raw > static_cast<make_unsigned_t<T>>(limit)) continue;
        }
        present[static_cast<int>(raw)] = true;
    }
    for (int candidate = 0; candidate <= limit; ++candidate) {
        if (!present[candidate]) return candidate;
    }
    assert(false && "mex: unreachable");
    return limit + 1;
}

// @acf fn frequencies — (range) -> map<T,ll> — 値ごとの出現回数
/// @acf-doc @brief 配列・文字列の値ごとの個数を昇順の map に集計。N 要素、D 種類で時間 O(N log(D+1))、追加メモリ O(D)。空入力は空 map。入力は変更しない
/// @acf-doc @code (range) -> map<T,ll>
/// @acf-doc [function] frequencies
template <std::ranges::input_range R>
map<std::ranges::range_value_t<R>, ll> frequencies(R&& values) {
    map<std::ranges::range_value_t<R>, ll> result;
    for (const auto& value : values) ++result[value];
    return result;
}

// @acf fn positions — (range, ll start=0) -> map<T,vll> — 値ごとの出現位置一覧
/// @acf-doc @brief 値ごとにstartから始まる出現位置を入力順で集める。省略時は0始まり、1なら1始まり。N要素、D種類で時間O(N log(D+1))、追加メモリO(N+D)。空入力は空map。入力は変更しない。位置がllの最大値を超える場合はoverflow_error
/// @acf-doc @code (range, ll start=0) -> map<T,vll>
/// @acf-doc [function] positions
template <std::ranges::input_range R>
map<std::ranges::range_value_t<R>, vll> positions(R&& values, ll start = 0) {
    map<std::ranges::range_value_t<R>, vll> result;
    ll index = start;
    bool first = true;
    for (const auto& value : values) {
        if (!first) {
            if (index == numeric_limits<ll>::max())
                throw overflow_error("positions: index exceeds ll maximum");
            ++index;
        }
        first = false;
        result[value].push_back(index);
    }
    return result;
}

// @acf fn run_length_encode — (range) -> vector<pair<T,int>> — 連続する同値をまとめる
/// @acf-doc @brief 文字列・配列を `(値, 連続個数)` の列へ O(N) で変換
/// @acf-doc [function] run_length_encode
template <class R>
vector<pair<ac_detail::algorithm::range_value_t<R>, int>> run_length_encode(
    const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<pair<T, int>> result;
    for (const T& value : values) {
        if (result.empty() || result.back().first != value)
            result.emplace_back(value, 1);
        else
            ++result.back().second;
    }
    return result;
}

// @acf fn run_length_decode — (vector<pair<T,int>>) -> vector<T> — ランレングスを展開
/// @acf-doc @brief `(値, 個数)` の列を元の配列へ O(展開後の長さ) で戻す
/// @acf-doc [function] run_length_decode
template <class T>
vector<T> run_length_decode(const vector<pair<T, int>>& encoded) {
    vector<T> result;
    for (const auto& [value, count] : encoded) {
        assert(count >= 0);
        result.insert(result.end(), count, value);
    }
    return result;
}

// @acf fn range_min — (range) -> optional<T> — 1次元range全体の最小値
/// @acf-doc @brief 1次元の range を1回走査し、要素全体の最小値を返す。空列は nullopt。多次元配列は `axis_min<Axis>` を使う
/// @acf-doc @code (range) -> optional<T>
/// @acf-doc [function] range_min
template <class R>
optional<ac_detail::algorithm::range_value_t<R>> range_min(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_min requires a one-dimensional range; use axis_min");

    auto it = begin(values);
    const auto last = end(values);
    if (it == last) return nullopt;

    if constexpr (std::forward_iterator<decltype(it)>) {
        auto best = it;
        for (++it; it != last; ++it)
            if (*it < *best) best = it;
        return *best;
    } else {
        optional<T> best(in_place, *it);
        for (++it; it != last; ++it)
            if (*it < *best) best.emplace(*it);
        return *best;
    }
}

// @acf fn range_max — (range) -> optional<T> — 1次元range全体の最大値
/// @acf-doc @brief 1次元の range を1回走査し、要素全体の最大値を返す。空列は nullopt。多次元配列は `axis_max<Axis>` を使う
/// @acf-doc @code (range) -> optional<T>
/// @acf-doc [function] range_max
template <class R>
optional<ac_detail::algorithm::range_value_t<R>> range_max(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_max requires a one-dimensional range; use axis_max");

    auto it = begin(values);
    const auto last = end(values);
    if (it == last) return nullopt;

    if constexpr (std::forward_iterator<decltype(it)>) {
        auto best = it;
        for (++it; it != last; ++it)
            if (*best < *it) best = it;
        return *best;
    } else {
        optional<T> best(in_place, *it);
        for (++it; it != last; ++it)
            if (*best < *it) best.emplace(*it);
        return *best;
    }
}

// @acf fn range_minmax — (range) -> optional<pair<T,T>> — 1次元range全体の最小値と最大値
/// @acf-doc @brief 1次元の range を1回走査し、`{最小値, 最大値}` を返す。空列は nullopt。minとmaxを同時に使うときに2回走査を避けられる。多次元配列は `axis_min` / `axis_max` を使う
/// @acf-doc @code (range) -> optional<pair<T,T>>
/// @acf-doc [function] range_minmax
template <class R>
optional<pair<ac_detail::algorithm::range_value_t<R>,
             ac_detail::algorithm::range_value_t<R>>>
range_minmax(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_minmax requires a one-dimensional range; use axis_min/axis_max");

    auto it = begin(values);
    const auto last = end(values);
    if (it == last) return nullopt;

    if constexpr (std::forward_iterator<decltype(it)>) {
        auto minimum = it;
        auto maximum = it;
        for (++it; it != last; ++it) {
            if (*it < *minimum) minimum = it;
            if (*maximum < *it) maximum = it;
        }
        return pair<T, T>{*minimum, *maximum};
    } else {
        optional<T> minimum(in_place, *it), maximum(in_place, *it);
        for (++it; it != last; ++it) {
            if (*it < *minimum) minimum.emplace(*it);
            if (*maximum < *it) maximum.emplace(*it);
        }
        return pair<T, T>{*minimum, *maximum};
    }
}

// @acf fn range_span — (range) -> optional<ll> — 整数列全体の max-min
/// @acf-doc @brief 整数列全体の `max-min` を O(N) で求める。空列は nullopt
/// @acf-doc @code (range) -> optional<ll>
/// @acf-doc [function] range_span
template <class R>
optional<ll> range_span(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "range_span requires an integral range");
    static_assert(!is_same_v<remove_cv_t<T>, bool>,
                  "range_span does not accept bool ranges");

    auto it = begin(values);
    const auto last = end(values);
    if (it == last) return nullopt;

    i128 minimum = static_cast<i128>(*it);
    i128 maximum = minimum;
    for (++it; it != last; ++it) {
        const i128 value = static_cast<i128>(*it);
        if (value < minimum) minimum = value;
        if (value > maximum) maximum = value;
    }
    return ac_detail::checked_ll(maximum - minimum, "range_span");
}

// Non-const-only input ranges (for example views::istream) cannot bind to the
// const overloads above. Retain values because an input iterator is single-pass.
template <ranges::input_range R>
requires (!ranges::range<const remove_reference_t<R>>)
auto range_min(R&& values) -> optional<ranges::range_value_t<R>> {
    using T = ranges::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_min requires a one-dimensional range; use axis_min");
    auto it = ranges::begin(values);
    const auto last = ranges::end(values);
    if (it == last) return nullopt;
    optional<T> best(in_place, *it);
    for (++it; it != last; ++it)
        if (*it < *best) best.emplace(*it);
    return *best;
}

template <ranges::input_range R>
requires (!ranges::range<const remove_reference_t<R>>)
auto range_max(R&& values) -> optional<ranges::range_value_t<R>> {
    using T = ranges::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_max requires a one-dimensional range; use axis_max");
    auto it = ranges::begin(values);
    const auto last = ranges::end(values);
    if (it == last) return nullopt;
    optional<T> best(in_place, *it);
    for (++it; it != last; ++it)
        if (*best < *it) best.emplace(*it);
    return *best;
}

template <ranges::input_range R>
requires (!ranges::range<const remove_reference_t<R>>)
auto range_minmax(R&& values) -> optional<pair<ranges::range_value_t<R>, ranges::range_value_t<R>>> {
    using T = ranges::range_value_t<R>;
    static_assert(!ac_detail::io::is_range_v<T>,
                  "range_minmax requires a one-dimensional range; use axis_min/axis_max");
    auto it = ranges::begin(values);
    const auto last = ranges::end(values);
    if (it == last) return nullopt;
    optional<T> minimum(in_place, *it), maximum(in_place, *it);
    for (++it; it != last; ++it) {
        if (*it < *minimum) minimum.emplace(*it);
        if (*maximum < *it) maximum.emplace(*it);
    }
    return pair<T, T>{*minimum, *maximum};
}

// @acf fn read_graph — (int n, int m, bool directed=false) -> vvi — 無向/有向グラフ入力（1-indexed 辺）
/// @acf-doc @brief 無向/有向グラフ入力（1-indexed 辺）
/// @acf-doc @code (int n, int m, bool directed=false) -> vvi
/// @acf-doc [function] read_graph
inline vvi read_graph(int n, int m, bool directed = false) {
    vvi g(n);
    rep(_, m) {
        int u, v;
        in(u, v);
        --u, --v;
        g[u].emplace_back(v);
        if (!directed) g[v].emplace_back(u);
    }
    return g;
}
// @acf fn read_tree — (int n) -> vvi — 木を read_graph(n,n-1) で読む
/// @acf-doc @brief 木を read_graph(n,n-1) で読む
/// @acf-doc @code (int n) -> vvi
/// @acf-doc [function] read_tree
inline vvi read_tree(int n) {
    return read_graph(n, n - 1, false);
}

#ifdef AC_HAS_ACL
// ACL modint の直接出力。print / range 出力も同じく val() を使う。
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
ostream& operator<<(ostream& os, const Mint& value) {
    return os << value.val();
}
#endif

// @acf helper operator<< — STL range / pair / tuple / optional / adapter の出力（bitset は標準）
/// @acf-doc @brief STL range / pair / tuple / optional / adapter の出力。bitset は標準 `operator<<`（01 文字列）のまま
/// @acf-doc [helper] operator<<
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
ostream& operator<<(ostream& os, const R& r) {
    ac_detail::output::write_item(os, r);
    return os;
}

template <class T1, class T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
    ac_detail::output::write_item(os, p);
    return os;
}

template <class... Args>
ostream& operator<<(ostream& os, const tuple<Args...>& t) {
    ac_detail::output::write_item(os, t);
    return os;
}

template <class T>
ostream& operator<<(ostream& os, const optional<T>& o) {
    ac_detail::output::write_item(os, o);
    return os;
}

template <class T, class C>
ostream& operator<<(ostream& os, stack<T, C> st) {
    ac_detail::output::write_item(os, std::move(st));
    return os;
}

template <class T, class C>
ostream& operator<<(ostream& os, queue<T, C> q) {
    ac_detail::output::write_item(os, std::move(q));
    return os;
}

template <class T, class C, class Comp>
ostream& operator<<(ostream& os, priority_queue<T, C, Comp> pq) {
    ac_detail::output::write_item(os, std::move(pq));
    return os;
}

template <class T, class Comp>
ostream& operator<<(ostream& os, setpq<T, Comp> pq) {
    ac_detail::output::write_item(os, std::move(pq));
    return os;
}

// コレクション化では、走査用 tuple/pair の参照と vector<bool> の proxy を値にする。
// string_view やポインタなど、利用者が明示した非所有型までは深いコピーをしない。
namespace ac_detail::collection {
template <class T>
struct stored_type { using type = T; };
template <class T>
using stored_t = typename stored_type<decay_t<T>>::type;
template <class... Ts>
struct stored_type<tuple<Ts...>> { using type = tuple<stored_t<Ts>...>; };
template <class A, class B>
struct stored_type<pair<A, B>> { using type = pair<stored_t<A>, stored_t<B>>; };
template <>
struct stored_type<vector<bool>::reference> { using type = bool; };

// range-for の変数として一度受けた要素を渡す。proxy/prvalue もこの呼び出し中は生存する。
template <class R>
using visit_arg_t = add_lvalue_reference_t<remove_reference_t<ranges::range_reference_t<R>>>;

struct accept_all {
    template <class T>
    constexpr bool operator()(const T&) const { return true; }
};

template <bool AsSet, ranges::input_range R, class Pred, class F>
auto collect(R&& values, Pred pred, F function) {
    using T = stored_t<invoke_result_t<F&, visit_arg_t<R>>>;
    conditional_t<AsSet, set<T>, vector<T>> result;
    if constexpr (!AsSet && is_same_v<Pred, accept_all> && ranges::sized_range<R>)
        result.reserve(ranges::size(values));
    for (auto&& value : values) {
        if (!std::invoke(pred, value)) continue;
        if constexpr (AsSet) result.emplace(std::invoke(function, value));
        else result.emplace_back(std::invoke(function, value));
    }
    return result;
}
}  // namespace ac_detail::collection

// @acf fn map_vec — (range, function) -> vector<U> — 全要素を変換して配列にする
/// @acf-doc @brief 入力順に function を各要素へ1回適用して値の vector を返す。空入力は空。関数自身は入力を変更しない。時間・追加メモリ O(N)（変換・コピーを除く）
/// @acf-doc @code (range, function) -> vector<U>
/// @acf-doc [function] map_vec
template <ranges::input_range R, class F>
auto map_vec(R&& values, F function) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), ac_detail::collection::accept_all{}, std::move(function));
}

// @acf fn filter_vec — (range, predicate) -> vector<T> — 条件に合う要素を入力順に取り出す
/// @acf-doc @brief predicate を各要素へ1回適用し、真の要素を値で保存。時間 O(N)、追加メモリ O(K)（コピーを除く、K は採用数）。入力は関数自身では変更しない
/// @acf-doc @code (range, predicate) -> vector<T>
/// @acf-doc [function] filter_vec
template <ranges::input_range R, class Pred>
auto filter_vec(R&& values, Pred pred) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), std::move(pred), identity{});
}

// @acf fn map_if — (range, predicate, function) -> vector<U> — 条件に合う要素だけ変換する
/// @acf-doc @brief predicate は各要素へ1回、function は採用要素だけへ1回適用。入力順を保ち、中間配列なし。時間 O(N)、追加メモリ O(K)（述語・変換・コピーを除く）
/// @acf-doc @code (range, predicate, function) -> vector<U>
/// @acf-doc [function] map_if
template <ranges::input_range R, class Pred, class F>
auto map_if(R&& values, Pred pred, F function) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), std::move(pred), std::move(function));
}

// @acf fn to_vec — (range) -> vector<T> — 走査結果を値の配列にする
/// @acf-doc @brief 入力順にコピーし、tuple/pair 内の参照と vector<bool> の proxy を値にする。時間・追加メモリ O(N)（コピーを除く）。string_view/ポインタの参照先はコピーしない
/// @acf-doc @code (range) -> vector<T>
/// @acf-doc [function] to_vec
template <ranges::input_range R>
auto to_vec(R&& values) {
    return ::map_vec(std::forward<R>(values), identity{});
}

// @acf fn to_str — (char range) -> str — 文字列へ変換する
/// @acf-doc @brief char の配列・view を入力順にコピーして string を返す。空入力は空文字列。時間・追加メモリ O(N)
/// @acf-doc @code (char range) -> str
/// @acf-doc [function] to_str
template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, char>
str to_str(R&& values) {
    str result;
    if constexpr (ranges::sized_range<R>) result.reserve(ranges::size(values));
    for (auto&& value : values) result.push_back(value);
    return result;
}

namespace ac_detail::slicing {
template <class Result, class R>
Result copy(const R& values, optional<ll> start, optional<ll> stop, ll step,
            Result result) {
    if (step == 0) throw invalid_argument("sliced: step must not be zero");
    // Widen before adding bounds or negating LLONG_MIN.
    const i128 n = values.size(), stride = step;
    const i128 low = step > 0 ? 0 : -1, high = step > 0 ? n : n - 1;
    const auto bound = [&](optional<ll> value, i128 omitted) -> i128 {
        if (!value) return omitted;
        i128 index = *value;
        if (index < 0) index += n;
        return std::clamp(index, low, high);
    };
    const i128 first = bound(start, step > 0 ? 0 : n - 1);
    const i128 last = bound(stop, step > 0 ? n : -1);
    const i128 count = step > 0
        ? (first < last ? 1 + (last - first - 1) / stride : 0)
        : (first > last ? 1 + (first - last - 1) / -stride : 0);
    result.reserve(static_cast<size_t>(count));
    for (i128 index = first; step > 0 ? index < last : index > last; index += stride)
        result.push_back(values[static_cast<size_t>(index)]);
    return result;
}
}  // namespace ac_detail::slicing

// @acf fn sliced — (string/vector[, start, stop, step]) -> copy — Python型のスライスで部分列をコピー
/// @acf-doc @brief stringとvectorからPythonの[start:stop:step]と同じ添字で部分列をコピー。負の添字・正負step・範囲外クリップに対応し、元の列は変更しない
/// @acf-doc @code (values, optional<ll> start=nullopt, optional<ll> stop=nullopt, ll step=1) -> str / vector<T, Allocator>
/// @acf-doc [function] sliced
inline str sliced(string_view values, optional<ll> start = nullopt,
                 optional<ll> stop = nullopt, ll step = 1) {
    return ac_detail::slicing::copy(values, start, stop, step, str{});
}

template <class T, class Allocator>
vector<T, Allocator> sliced(const vector<T, Allocator>& values,
                           optional<ll> start = nullopt, optional<ll> stop = nullopt,
                           ll step = 1) {
    return ac_detail::slicing::copy(values, start, stop, step,
                                   vector<T, Allocator>(values.get_allocator()));
}

// @acf fn to_set — (range) -> set<T> — 重複を除いた昇順の集合にする
/// @acf-doc @brief 値を set へ直接挿入。tuple/pair 内の参照と vector<bool> の proxy は値になる。時間 O(N log(D+1))、追加メモリ O(D)（比較・コピーを除く、D は異なる値の数）
/// @acf-doc @code (range) -> set<T>
/// @acf-doc [function] to_set
template <ranges::input_range R>
auto to_set(R&& values) {
    return ac_detail::collection::collect<true>(
        std::forward<R>(values), ac_detail::collection::accept_all{}, identity{});
}

// @acf fn is_unique — (range) -> bool — 全要素に重複がないか
/// @acf-doc @brief 比較可能な要素を set へ順に挿入し、最初の重複で false。空・1要素は true。入力は変更しない。時間 O(N log(D+1))、追加メモリ O(D)（比較・コピーを除く、D は異なる値の数）
/// @acf-doc @code (range) -> bool
/// @acf-doc [function] is_unique
template <ranges::input_range R>
bool is_unique(R&& values) {
    using T = ac_detail::collection::stored_t<ranges::range_value_t<R>>;
    set<T> seen;
    for (auto&& value : values)
        if (!seen.emplace(value).second) return false;
    return true;
}

// @acf fn all_unique — (range) -> bool — 全要素が互いに異なるか
/// @acf-doc @brief `is_unique` の別名。最初の重複で false、空・1要素なら true。入力は変更しない
/// @acf-doc @code (range) -> bool
/// @acf-doc [function] all_unique
template <ranges::input_range R>
bool all_unique(R&& values) {
    return ::is_unique(std::forward<R>(values));
}

// @acf fn flat_map — (range, function) -> vector<T> — 各要素から作る列を1段平坦化する
/// @acf-doc @brief function を各要素へ1回適用し、返された列を順に値で連結。参照で返る列は丸ごとコピーせず走査。一時的な列はその回の走査後に破棄。時間 O(N+M)、追加メモリ O(M) と内側の一時列（生成・コピーを除く）
/// @acf-doc @code (range, function) -> vector<T>
/// @acf-doc [function] flat_map
template <ranges::input_range R, class F>
auto flat_map(R&& values, F function) {
    using Inner = invoke_result_t<F&, ac_detail::collection::visit_arg_t<R>>;
    static_assert(ranges::input_range<Inner>, "flat_map: function must return a range");
    using T = ac_detail::collection::stored_t<ranges::range_value_t<Inner>>;
    vector<T> result;
    for (auto&& value : values) {
        decltype(auto) inner = std::invoke(function, value);
        for (auto&& item : inner) result.emplace_back(item);
    }
    return result;
}

#if defined(__cpp_lib_ranges_enumerate) && __cpp_lib_ranges_enumerate >= 202302L
// @acf fn enumerate — (range) -> view — 0始まりの添字と要素を走査する
/// @acf-doc @brief views::enumerate の短縮。添字は符号付き、要素は元の参照規則を保つ。lvalue の寿命は呼び出し側で管理し、rvalue コンテナは view が所有。C++23 enumerate 対応の標準ライブラリが必要
/// @acf-doc @code (range) -> view
/// @acf-doc [function] enumerate
template <ranges::viewable_range R>
auto enumerate(R&& values) {
    return views::enumerate(std::forward<R>(values));
}
#endif

#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
// @acf fn zip — (ranges...) -> view — 同じ位置の要素をまとめて最短列まで走査する
/// @acf-doc @brief views::zip の短縮。要素の参照規則を保つ tuple を返し、最短列で終了。組合せ配列は作らない。C++23 zip 対応の標準ライブラリが必要
/// @acf-doc @code (ranges...) -> view
/// @acf-doc [function] zip
template <ranges::viewable_range... Rs>
auto zip(Rs&&... values) {
    return views::zip(std::forward<Rs>(values)...);
}
#endif

#if defined(__cpp_lib_ranges_cartesian_product) && __cpp_lib_ranges_cartesian_product >= 202207L
// @acf fn product — (ranges...) -> view — 左の範囲を外側にして全組合せを走査する
/// @acf-doc @brief views::cartesian_product の短縮。左ほど外側のループ。全組合せの配列は作らず、空の範囲があれば結果も空。2本目以降は再走査可能な範囲が必要。C++23 cartesian_product 対応の標準ライブラリが必要
/// @acf-doc @code (ranges...) -> view
/// @acf-doc [function] product
template <ranges::viewable_range... Rs>
auto product(Rs&&... values) {
    return views::cartesian_product(std::forward<Rs>(values)...);
}
#endif

// @acf fn irange — (n) / (lo, hi) / (lo, hi, step) -> view — 半開区間の整数を走査する
/// @acf-doc @brief [0,n) または [lo,hi) を1ずつ走査。3引数版は Python range と同様に step 間隔で走査し、step<0 なら降順。step==0 は invalid_argument。向きが区間と合わなければ空。要素型は引数の符号付き型と int の共通型。表現できない符号なし値は out_of_range。配列を確保しない
/// @acf-doc @code (n) / (lo, hi) / (lo, hi, step) -> view
/// @acf-doc [function] irange
template <class L, class H>
requires(is_integral_v<L> && is_integral_v<H> && !is_same_v<L, bool> && !is_same_v<H, bool>)
auto irange(L lo, H hi) {
    using T = common_type_t<int, make_signed_t<L>, make_signed_t<H>>;
    if (!std::in_range<T>(+lo) || !std::in_range<T>(+hi))
        throw out_of_range("irange: endpoints do not fit the signed value type");
    const T first = static_cast<T>(lo), last = static_cast<T>(hi);
    return views::iota(first, std::max(first, last));
}

template <class L, class H, class S>
requires(is_integral_v<L> && is_integral_v<H> && is_integral_v<S> &&
         !is_same_v<L, bool> && !is_same_v<H, bool> && !is_same_v<S, bool>)
auto irange(L lo, H hi, S step) {
    using T = common_type_t<int, make_signed_t<L>, make_signed_t<H>, make_signed_t<S>>;
    static_assert(sizeof(T) <= sizeof(ll),
                  "irange(lo, hi, step) supports up to 64-bit integers");
    if (!std::in_range<T>(+lo) || !std::in_range<T>(+hi) || !std::in_range<T>(+step))
        throw out_of_range("irange: endpoints or step do not fit the signed value type");

    const T first = static_cast<T>(lo);
    const T last = static_cast<T>(hi);
    const T stride = static_cast<T>(step);
    if (stride == 0) throw invalid_argument("irange: step must be non-zero");

    const i128 stride_w = static_cast<i128>(stride);
    const i128 distance =
        stride_w > 0 ? static_cast<i128>(last) - static_cast<i128>(first)
                     : static_cast<i128>(first) - static_cast<i128>(last);
    const i128 width = stride_w > 0 ? stride_w : -stride_w;
    const i128 count_w =
        distance <= 0 ? 0 : distance / width + (distance % width != 0);
    const ull count = static_cast<ull>(count_w);

    return views::iota(ull{0}, count) |
           views::transform([first, stride](ull index) {
               return static_cast<T>(
                   static_cast<i128>(first) +
                   static_cast<i128>(index) * static_cast<i128>(stride));
           });
}

template <class T>
requires(is_integral_v<T> && !is_same_v<T, bool>)
auto irange(T n) {
    return ::irange(0, n);
}

namespace ac_detail::adjacent {
template <class Iter, size_t Width, class Pred>
bool matches(const array<Iter, Width>& window, Pred& pred) {
    return apply(
        [&](const auto&... it) { return static_cast<bool>(std::invoke(pred, *it...)); },
        window);
}

template <size_t Width, ranges::forward_range R, class Visit>
void for_each_window(R&& values, Visit visit) {
    static_assert(Width >= 2, "adjacent window width must be at least 2");
    array<ranges::iterator_t<R>, Width> window{};
    auto next = ranges::begin(values);
    const auto last = ranges::end(values);
    for (auto& it : window) {
        if (next == last) return;
        it = next++;
    }
    while (true) {
        if (!std::invoke(visit, window)) return;
        if (next == last) return;
        for (size_t i = 1; i < Width; ++i) window[i - 1] = window[i];
        window[Width - 1] = next++;
    }
}
}  // namespace ac_detail::adjacent

// @acf fn all_adjacent — (range, predicate) / all_adjacent<Width>(range, predicate) -> bool — 全連続区間が条件を満たすか
/// @acf-doc @brief 既定は隣接2要素。`all_adjacent<3>` は連続する3要素を predicate に渡す。Width>=2。最初の偽で終了し、要素数がWidth未満なら true。入力は変更しない。時間 O(N*Width)、追加メモリ O(Width)
/// @acf-doc @code (range, predicate) / all_adjacent<Width>(range, predicate) -> bool
/// @acf-doc [function] all_adjacent
template <size_t Width, ranges::forward_range R, class Pred>
bool all_adjacent(R&& values, Pred pred) {
    bool result = true;
    ac_detail::adjacent::for_each_window<Width>(std::forward<R>(values), [&](const auto& window) {
        if (ac_detail::adjacent::matches(window, pred)) return true;
        result = false;
        return false;
    });
    return result;
}

template <ranges::forward_range R, class Pred>
bool all_adjacent(R&& values, Pred pred) {
    return ::all_adjacent<2>(std::forward<R>(values), std::move(pred));
}

// @acf fn any_adjacent — (range, predicate) / any_adjacent<Width>(range, predicate) -> bool — 条件を満たす連続区間があるか
/// @acf-doc @brief 既定は隣接2要素。`any_adjacent<3>` は連続する3要素を predicate に渡す。Width>=2。最初の真で終了し、要素数がWidth未満なら false。入力は変更しない。時間 O(N*Width)、追加メモリ O(Width)
/// @acf-doc @code (range, predicate) / any_adjacent<Width>(range, predicate) -> bool
/// @acf-doc [function] any_adjacent
template <size_t Width, ranges::forward_range R, class Pred>
bool any_adjacent(R&& values, Pred pred) {
    bool result = false;
    ac_detail::adjacent::for_each_window<Width>(std::forward<R>(values), [&](const auto& window) {
        if (!ac_detail::adjacent::matches(window, pred)) return true;
        result = true;
        return false;
    });
    return result;
}

template <ranges::forward_range R, class Pred>
bool any_adjacent(R&& values, Pred pred) {
    return ::any_adjacent<2>(std::forward<R>(values), std::move(pred));
}

// @acf fn filter_adjacent — (range, predicate) / filter_adjacent<Width>(range, predicate) -> vector<T> — 連続区間の条件で末尾を抽出
/// @acf-doc @brief 既定は隣接2要素。先頭のWidth-1要素は残し、その後は元の列の連続Width要素をpredicateに渡し、真なら末尾を保存。Width>=2。要素数がWidth未満なら全要素を返す。時間 O(N*Width)、追加メモリ O(K+Width)（Kは採用数、コピーを除く）
/// @acf-doc @code (range, predicate) / filter_adjacent<Width>(range, predicate) -> vector<T>
/// @acf-doc [function] filter_adjacent
template <size_t Width = 2, ranges::forward_range R, class Pred>
auto filter_adjacent(R&& values, Pred pred) {
    static_assert(Width >= 2, "adjacent window width must be at least 2");
    using T = ac_detail::collection::stored_t<ranges::range_reference_t<R>>;
    vector<T> result;
    auto it = ranges::begin(values);
    const auto last = ranges::end(values);
    for (size_t i = 1; i < Width && it != last; ++i, ++it) result.emplace_back(*it);
    ac_detail::adjacent::for_each_window<Width>(std::forward<R>(values), [&](const auto& window) {
        if (ac_detail::adjacent::matches(window, pred)) result.emplace_back(*window.back());
        return true;
    });
    return result;
}

// @acf fn any_of — (range, predicate, projection=identity) -> bool — 条件を満たす要素があるか
/// @acf-doc @brief ranges::any_of の範囲版。最初の真で終了、空なら false。projection は各要素を述語へ渡す前に変換する。std::any_of(first,last,pred) はそのまま使用可能。時間 O(N)、追加メモリ O(1)
/// @acf-doc @code (range, predicate[, projection]) -> bool
/// @acf-doc [function] any_of
template <ranges::input_range R, class Pred, class Proj = identity>
bool any_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::any_of(std::forward<R>(values), std::move(pred),
                          std::move(projection));
}

// @acf fn all_of — (range, predicate, projection=identity) -> bool — 全要素が条件を満たすか
/// @acf-doc @brief ranges::all_of の範囲版。最初の偽で終了、空なら true。projection は各要素を述語へ渡す前に変換する。std::all_of(first,last,pred) はそのまま使用可能。時間 O(N)、追加メモリ O(1)
/// @acf-doc @code (range, predicate[, projection]) -> bool
/// @acf-doc [function] all_of
template <ranges::input_range R, class Pred, class Proj = identity>
bool all_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::all_of(std::forward<R>(values), std::move(pred),
                          std::move(projection));
}

// @acf fn none_of — (range, predicate, projection=identity) -> bool — 条件を満たす要素がないか
/// @acf-doc @brief ranges::none_of の範囲版。最初の真で終了し、空なら true。projection は各要素を述語へ渡す前に変換する。std::none_of(first,last,pred) はそのまま使用可能。時間 O(N)、追加メモリ O(1)
/// @acf-doc @code (range, predicate[, projection]) -> bool
/// @acf-doc [function] none_of
template <ranges::input_range R, class Pred, class Proj = identity>
bool none_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::none_of(std::forward<R>(values), std::move(pred),
                           std::move(projection));
}

// @acf fn count — (range, value, projection=identity) -> ll — 値と等しい要素数
/// @acf-doc @brief ranges::count の範囲版。projection 適用後の値を target と比較する。空なら0。std::count(first,last,value) はそのまま使用可能。時間 O(N)、追加メモリ O(1)
/// @acf-doc @code (range, value[, projection]) -> ll
/// @acf-doc [function] count
template <ranges::input_range R, class T, class Proj = identity>
ll count(R&& values, const T& target, Proj projection = {}) {
    return static_cast<ll>(ranges::count(std::forward<R>(values), target,
                                         std::move(projection)));
}

// @acf fn count_if — (range, predicate, projection=identity) -> ll — 条件を満たす要素数
/// @acf-doc @brief ranges::count_if の範囲版。空なら0。projection は各要素を述語へ渡す前に変換する。std::count_if(first,last,pred) はそのまま使用可能。時間 O(N)、追加メモリ O(1)
/// @acf-doc @code (range, predicate[, projection]) -> ll
/// @acf-doc [function] count_if
template <ranges::input_range R, class Pred, class Proj = identity>
ll count_if(R&& values, Pred pred, Proj projection = {}) {
    return static_cast<ll>(ranges::count_if(std::forward<R>(values),
                                            std::move(pred),
                                            std::move(projection)));
}

// Range を直接受け取る std::ranges アルゴリズムの短い入口。
// find(range, value) は上の独自API（添字・座標を返す）を維持する。
#define AC_RANGE_ALGORITHM_WRAPPER_(name)                                      \
    template <class R, class... Args>                                          \
    requires ranges::input_range<R> &&                                        \
             requires(R&& values, Args&&... args) {                           \
                 ranges::name(std::forward<R>(values),                        \
                              std::forward<Args>(args)...);                    \
             }                                                                \
    decltype(auto) name(R&& values, Args&&... args) {                          \
        return ranges::name(std::forward<R>(values),                          \
                            std::forward<Args>(args)...);                      \
    }

AC_RANGE_ALGORITHM_WRAPPER_(for_each)
AC_RANGE_ALGORITHM_WRAPPER_(mismatch)
AC_RANGE_ALGORITHM_WRAPPER_(equal)
AC_RANGE_ALGORITHM_WRAPPER_(find_if)
AC_RANGE_ALGORITHM_WRAPPER_(find_if_not)
AC_RANGE_ALGORITHM_WRAPPER_(find_end)
AC_RANGE_ALGORITHM_WRAPPER_(find_first_of)
AC_RANGE_ALGORITHM_WRAPPER_(adjacent_find)
AC_RANGE_ALGORITHM_WRAPPER_(search)
AC_RANGE_ALGORITHM_WRAPPER_(search_n)
AC_RANGE_ALGORITHM_WRAPPER_(copy)
AC_RANGE_ALGORITHM_WRAPPER_(copy_if)
AC_RANGE_ALGORITHM_WRAPPER_(copy_backward)
AC_RANGE_ALGORITHM_WRAPPER_(move)
AC_RANGE_ALGORITHM_WRAPPER_(move_backward)
AC_RANGE_ALGORITHM_WRAPPER_(swap_ranges)
AC_RANGE_ALGORITHM_WRAPPER_(fill)
AC_RANGE_ALGORITHM_WRAPPER_(transform)
AC_RANGE_ALGORITHM_WRAPPER_(generate)
AC_RANGE_ALGORITHM_WRAPPER_(remove)
AC_RANGE_ALGORITHM_WRAPPER_(remove_if)
AC_RANGE_ALGORITHM_WRAPPER_(remove_copy)
AC_RANGE_ALGORITHM_WRAPPER_(remove_copy_if)
AC_RANGE_ALGORITHM_WRAPPER_(replace)
AC_RANGE_ALGORITHM_WRAPPER_(replace_if)
AC_RANGE_ALGORITHM_WRAPPER_(replace_copy)
AC_RANGE_ALGORITHM_WRAPPER_(replace_copy_if)
AC_RANGE_ALGORITHM_WRAPPER_(unique)
AC_RANGE_ALGORITHM_WRAPPER_(unique_copy)
AC_RANGE_ALGORITHM_WRAPPER_(reverse)
AC_RANGE_ALGORITHM_WRAPPER_(reverse_copy)
AC_RANGE_ALGORITHM_WRAPPER_(rotate)
AC_RANGE_ALGORITHM_WRAPPER_(rotate_copy)
AC_RANGE_ALGORITHM_WRAPPER_(shuffle)
AC_RANGE_ALGORITHM_WRAPPER_(sample)
AC_RANGE_ALGORITHM_WRAPPER_(is_partitioned)
AC_RANGE_ALGORITHM_WRAPPER_(partition)
AC_RANGE_ALGORITHM_WRAPPER_(partition_copy)
AC_RANGE_ALGORITHM_WRAPPER_(stable_partition)
AC_RANGE_ALGORITHM_WRAPPER_(partition_point)
AC_RANGE_ALGORITHM_WRAPPER_(sort)
AC_RANGE_ALGORITHM_WRAPPER_(stable_sort)
AC_RANGE_ALGORITHM_WRAPPER_(partial_sort)
AC_RANGE_ALGORITHM_WRAPPER_(partial_sort_copy)
AC_RANGE_ALGORITHM_WRAPPER_(is_sorted)
AC_RANGE_ALGORITHM_WRAPPER_(is_sorted_until)
AC_RANGE_ALGORITHM_WRAPPER_(nth_element)
AC_RANGE_ALGORITHM_WRAPPER_(lower_bound)
AC_RANGE_ALGORITHM_WRAPPER_(upper_bound)
AC_RANGE_ALGORITHM_WRAPPER_(binary_search)
AC_RANGE_ALGORITHM_WRAPPER_(equal_range)
AC_RANGE_ALGORITHM_WRAPPER_(merge)
AC_RANGE_ALGORITHM_WRAPPER_(inplace_merge)
AC_RANGE_ALGORITHM_WRAPPER_(includes)
AC_RANGE_ALGORITHM_WRAPPER_(set_difference)
AC_RANGE_ALGORITHM_WRAPPER_(set_intersection)
AC_RANGE_ALGORITHM_WRAPPER_(set_symmetric_difference)
AC_RANGE_ALGORITHM_WRAPPER_(set_union)
AC_RANGE_ALGORITHM_WRAPPER_(push_heap)
AC_RANGE_ALGORITHM_WRAPPER_(pop_heap)
AC_RANGE_ALGORITHM_WRAPPER_(make_heap)
AC_RANGE_ALGORITHM_WRAPPER_(sort_heap)
AC_RANGE_ALGORITHM_WRAPPER_(is_heap)
AC_RANGE_ALGORITHM_WRAPPER_(is_heap_until)
AC_RANGE_ALGORITHM_WRAPPER_(min_element)
AC_RANGE_ALGORITHM_WRAPPER_(max_element)
AC_RANGE_ALGORITHM_WRAPPER_(minmax_element)
AC_RANGE_ALGORITHM_WRAPPER_(lexicographical_compare)
AC_RANGE_ALGORITHM_WRAPPER_(next_permutation)
AC_RANGE_ALGORITHM_WRAPPER_(prev_permutation)
AC_RANGE_ALGORITHM_WRAPPER_(is_permutation)

// ranges::min/max/minmax には2値版もある。range版だけを公開し、
// min(a, b) / max(a, b) / minmax(a, b) との曖昧さを避ける。
#define AC_RANGE_EXTREMA_WRAPPER_(name)                                        \
    template <ranges::input_range R, class Comp = ranges::less,                \
              class Proj = identity>                                          \
    requires(!ranges::input_range<Comp> &&                                    \
             requires(R&& values, Comp comp, Proj projection) {               \
                 ranges::name(std::forward<R>(values), std::move(comp),       \
                              std::move(projection));                         \
             })                                                               \
    auto name(R&& values, Comp comp = {}, Proj projection = {}) {               \
        return ranges::name(std::forward<R>(values), std::move(comp),           \
                            std::move(projection));                             \
    }

AC_RANGE_EXTREMA_WRAPPER_(min)
AC_RANGE_EXTREMA_WRAPPER_(max)
AC_RANGE_EXTREMA_WRAPPER_(minmax)

// std::min/max/minmax の2値版は同じ型を要求する。
// 異なる算術型の場合だけ共通型で比較する。
template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> && (!is_same_v<T, U>)
constexpr common_type_t<T, U> min(T first, U second) {
    using Common = common_type_t<T, U>;
    const Common a = static_cast<Common>(first);
    const Common b = static_cast<Common>(second);
    return b < a ? b : a;
}

template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> && (!is_same_v<T, U>)
constexpr common_type_t<T, U> max(T first, U second) {
    using Common = common_type_t<T, U>;
    const Common a = static_cast<Common>(first);
    const Common b = static_cast<Common>(second);
    return a < b ? b : a;
}

template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> && (!is_same_v<T, U>)
constexpr pair<common_type_t<T, U>, common_type_t<T, U>> minmax(T first,
                                                               U second) {
    using Common = common_type_t<T, U>;
    const Common a = static_cast<Common>(first);
    const Common b = static_cast<Common>(second);
    return b < a ? pair<Common, Common>{b, a} : pair<Common, Common>{a, b};
}

#if defined(__cpp_lib_ranges_contains) && __cpp_lib_ranges_contains >= 202207L
AC_RANGE_ALGORITHM_WRAPPER_(contains)
AC_RANGE_ALGORITHM_WRAPPER_(contains_subrange)
#endif

#if defined(__cpp_lib_ranges_starts_ends_with) && \
    __cpp_lib_ranges_starts_ends_with >= 202106L
AC_RANGE_ALGORITHM_WRAPPER_(starts_with)
AC_RANGE_ALGORITHM_WRAPPER_(ends_with)
#else
template <ranges::input_range R1, ranges::input_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity,
          class Proj2 = identity>
requires indirectly_comparable<ranges::iterator_t<R1>, ranges::iterator_t<R2>,
                              Pred, Proj1, Proj2>
constexpr bool starts_with(R1&& values, R2&& prefix, Pred pred = {},
                           Proj1 proj1 = {}, Proj2 proj2 = {}) {
    return ranges::mismatch(values, prefix, std::move(pred), std::move(proj1),
                            std::move(proj2)).in2 == ranges::end(prefix);
}
#endif

#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
AC_RANGE_ALGORITHM_WRAPPER_(find_last)
AC_RANGE_ALGORITHM_WRAPPER_(find_last_if)
AC_RANGE_ALGORITHM_WRAPPER_(find_last_if_not)
#endif

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
AC_RANGE_ALGORITHM_WRAPPER_(fold_left)
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_first)
AC_RANGE_ALGORITHM_WRAPPER_(fold_right)
AC_RANGE_ALGORITHM_WRAPPER_(fold_right_last)
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_with_iter)
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_first_with_iter)
#endif

#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
AC_RANGE_ALGORITHM_WRAPPER_(iota)
#endif

#if defined(__cpp_lib_ranges_to_container) && \
    __cpp_lib_ranges_to_container >= 202202L
template <class Container, class R, class... Args>
requires ranges::input_range<R> &&
         requires(R&& values, Args&&... args) {
             ranges::to<Container>(std::forward<R>(values),
                                  std::forward<Args>(args)...);
         }
decltype(auto) to(R&& values, Args&&... args) {
    return ranges::to<Container>(std::forward<R>(values),
                                 std::forward<Args>(args)...);
}
#endif

#undef AC_RANGE_EXTREMA_WRAPPER_
#undef AC_RANGE_ALGORITHM_WRAPPER_

template <ranges::forward_range R, class T, class Comp = ranges::less,
          class Proj = identity>
requires requires(R& values, const T& target, Comp comp, Proj projection) {
    ranges::lower_bound(values, target, comp, projection);
}
int lower_bound_idx(R&& values, const T& target, Comp comp = {}, Proj projection = {}) {
    const auto it = ranges::lower_bound(values, target, std::move(comp),
                                        std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), it));
}

template <ranges::forward_range R, class T, class Comp = ranges::less,
          class Proj = identity>
requires requires(R& values, const T& target, Comp comp, Proj projection) {
    ranges::upper_bound(values, target, comp, projection);
}
int upper_bound_idx(R&& values, const T& target, Comp comp = {}, Proj projection = {}) {
    const auto it = ranges::upper_bound(values, target, std::move(comp),
                                        std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), it));
}

// @acf fn find_idx — (range, target, projection={}) -> int — 値と一致する最初の添字
/// @acf-doc @brief projection適用後にtargetと等しい最初の要素を探す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。既定のprojectionはidentity。文字列の部分一致にはsearch_idxを使う。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, target, projection={}) -> int
/// @acf-doc [function] find_idx
template <ranges::forward_range R, class T, class Proj = identity>
int find_idx(R&& values, const T& target, Proj projection = {}) {
    const auto result = ranges::find(values, target, std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn rfind_idx — (range, target, projection={}) -> int — 値と一致する最後の添字
/// @acf-doc @brief projection適用後にtargetと等しい最後の要素を左からの走査で探す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。既定のprojectionはidentity。時間O(N)、追加メモリO(1)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, target, projection={}) -> int
/// @acf-doc [function] rfind_idx
template <ranges::forward_range R, class T, class Proj = identity>
int rfind_idx(R&& values, const T& target, Proj projection = {}) {
    i128 index = 0, result = -1;
    for (auto&& value : values) {
        if (std::invoke(projection, value) == target) result = index;
        ++index;
    }
    return ac_detail::checked_range_index(result < 0 ? index : result);
}

// @acf fn find_if_idx — (range, pred, projection={}) -> int — 条件を満たす最初の添字
/// @acf-doc @brief projection適用後にpredが真になる最初の要素を探す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred, projection={}) -> int
/// @acf-doc [function] find_if_idx
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_if_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_if(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn find_if_not_idx — (range, pred, projection={}) -> int — 条件を満たさない最初の添字
/// @acf-doc @brief projection適用後にpredが偽になる最初の要素を探す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred, projection={}) -> int
/// @acf-doc [function] find_if_not_idx
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_if_not_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_if_not(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn adjacent_find_idx — (range, pred={}, projection={}) -> int — 条件を満たす最初の隣接ペアの添字
/// @acf-doc @brief projection適用後の隣接2要素を左、右の順でpredへ渡し、最初に真になるペアの左側の添字を返す。既定のpredは等値比較。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。1要素なら1。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred={}, projection={}) -> int
/// @acf-doc [function] adjacent_find_idx
template <ranges::forward_range R, class Pred = ranges::equal_to, class Proj = identity>
int adjacent_find_idx(R&& values, Pred pred = {}, Proj projection = {}) {
    const auto result = ranges::adjacent_find(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn find_first_of_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 候補のいずれかと一致する最初の添字
/// @acf-doc @brief otherのいずれかとpredで一致する最初の要素を探す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, other, pred={}, projection1={}, projection2={}) -> int
/// @acf-doc [function] find_first_of_idx
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int find_first_of_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::find_first_of(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn search_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 連続部分列が最初に一致する添字
/// @acf-doc @brief otherが連続部分列として最初に一致する始点を返す。otherが空なら0。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, other, pred={}, projection1={}, projection2={}) -> int
/// @acf-doc [function] search_idx
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int search_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::search(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_end_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 連続部分列が最後に一致する添字
/// @acf-doc @brief otherが連続部分列として最後に一致する始点を返す。otherが空ならvaluesの要素数。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, other, pred={}, projection1={}, projection2={}) -> int
/// @acf-doc [function] find_end_idx
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int find_end_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::find_end(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn search_n_idx — (range, count, target, pred={}, projection={}) -> int — 同じ値が指定個数続く最初の添字
/// @acf-doc @brief projection適用後にtargetとpredで一致する要素がcount個続く最初の始点を返す。countが0以下なら0。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, count, target, pred={}, projection={}) -> int
/// @acf-doc [function] search_n_idx
template <ranges::forward_range R, class T, class Pred = ranges::equal_to,
          class Proj = identity>
int search_n_idx(R&& values, ranges::range_difference_t<R> count, const T& target,
                 Pred pred = {}, Proj projection = {}) {
    const auto result = ranges::search_n(values, count, target, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn equal_range_idx — (range, target, comp={}, projection={}) -> pair<int,int> — 等しい値の区間を添字で返す
/// @acf-doc @brief compとprojectionの順序で整列した列の一致区間を0始まりの半開区間[l,r)として返す。不在は挿入位置pに対する{p,p}、空入力は{0,0}。入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, target, comp={}, projection={}) -> pair<int,int>
/// @acf-doc [function] equal_range_idx
template <ranges::forward_range R, class T, class Comp = ranges::less,
          class Proj = identity>
pair<int, int> equal_range_idx(R&& values, const T& target, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::equal_range(values, target, std::move(comp),
                                           std::move(projection));
    return {ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin())),
            ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.end()))};
}

// @acf fn mismatch_idx — (range, other, pred={}, projection1={}, projection2={}) -> pair<int,int> — 最初に一致しなくなる2列の添字
/// @acf-doc @brief 2列を左から比較し、最初の不一致位置を各列の0始まりの添字で返す。共通部分が全て一致するなら短い列の要素数を両方に返す。両方空なら{0,0}。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, other, pred={}, projection1={}, projection2={}) -> pair<int,int>
/// @acf-doc [function] mismatch_idx
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
pair<int, int> mismatch_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::mismatch(values, other, std::move(pred),
                                        std::move(projection1), std::move(projection2));
    return {ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.in1)),
            ac_detail::checked_range_index(ranges::distance(ranges::begin(other), result.in2))};
}

// @acf fn min_element_idx — (range, comp={}, projection={}) -> int — 最小要素の添字
/// @acf-doc @brief compとprojectionによる最小要素の添字を返す。同値が複数あれば最初。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, comp={}, projection={}) -> int
/// @acf-doc [function] min_element_idx
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int min_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::min_element(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn max_element_idx — (range, comp={}, projection={}) -> int — 最大要素の添字
/// @acf-doc @brief compとprojectionによる最大要素の添字を返す。同値が複数あれば最初。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, comp={}, projection={}) -> int
/// @acf-doc [function] max_element_idx
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int max_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::max_element(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn is_sorted_until_idx — (range, comp={}, projection={}) -> int — 整列順序が初めて崩れる添字
/// @acf-doc @brief compとprojectionによる整列順序が初めて崩れる要素の添字を返す。全体が整列していれば要素数。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, comp={}, projection={}) -> int
/// @acf-doc [function] is_sorted_until_idx
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int is_sorted_until_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::is_sorted_until(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn minmax_element_idx — (range, comp={}, projection={}) -> pair<int,int> — 最小要素と最大要素の添字
/// @acf-doc @brief compとprojectionによる最小要素と最大要素の0始まりの添字を返す。同値なら最小は最初、最大は最後。空入力は{0,0}。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, comp={}, projection={}) -> pair<int,int>
/// @acf-doc [function] minmax_element_idx
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
pair<int, int> minmax_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::minmax_element(values, std::move(comp),
                                             std::move(projection));
    return {ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.min)),
            ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.max))};
}

// @acf fn partition_point_idx — (range, pred, projection={}) -> int — 条件が真から偽へ切り替わる添字
/// @acf-doc @brief projection適用後のpredが真、偽の順に分割された列で、最初の偽の添字を返す。全て真なら要素数、全て偽または空なら0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred, projection={}) -> int
/// @acf-doc [function] partition_point_idx
template <ranges::forward_range R, class Pred, class Proj = identity>
int partition_point_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::partition_point(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn is_heap_until_idx — (random-access range, comp={}, projection={}) -> int — heap条件が初めて崩れる添字
/// @acf-doc @brief compとprojectionによるheap条件が初めて崩れる要素の0始まりの添字を返す。全体がheapなら要素数、空なら0。既定はmax-heap。random-access rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (random-access range, comp={}, projection={}) -> int
/// @acf-doc [function] is_heap_until_idx
template <ranges::random_access_range R, class Comp = ranges::less, class Proj = identity>
int is_heap_until_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::is_heap_until(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
// @acf fn find_last_idx — (range, target, projection={}) -> int — 値と一致する最後の添字
/// @acf-doc @brief 値と一致する最後の添字を返す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。C++23のranges::find_last対応時のみ使える。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, target, projection={}) -> int
/// @acf-doc [function] find_last_idx
template <ranges::forward_range R, class T, class Proj = identity>
int find_last_idx(R&& values, const T& target, Proj projection = {}) {
    const auto result = ranges::find_last(values, target, std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_last_if_idx — (range, pred, projection={}) -> int — 条件を満たす最後の添字
/// @acf-doc @brief 条件を満たす最後の添字を返す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。C++23のranges::find_last対応時のみ使える。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred, projection={}) -> int
/// @acf-doc [function] find_last_if_idx
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_last_if_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_last_if(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_last_if_not_idx — (range, pred, projection={}) -> int — 条件を満たさない最後の添字
/// @acf-doc @brief 条件を満たさない最後の添字を返す。0始まりのintを返し、不在は要素数、空入力は0。forward rangeを受け取り、入力は変更しない。比較関数・projectionの引数順と既定値は標準のrange版と同じ。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。C++23のranges::find_last対応時のみ使える。返す添字がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (range, pred, projection={}) -> int
/// @acf-doc [function] find_last_if_not_idx
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_last_if_not_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_last_if_not(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

#endif

// @acf fn argsort — (const range, comp={}, projection={}) -> vector<int> — 値順に並べた元添字を返す
/// @acf-doc @brief 入力を変更せず、compとprojectionで並べた0始まりの元添字を返す。同値は降順でも元添字順。constで読めるsized random-access rangeが対象。比較回数O(N log N)、追加メモリO(N)。要素数がintの最大値を超える場合はoverflow_error。
/// @acf-doc @code (const R& values, Comp comp={}, Proj projection={}) -> vector<int>
/// @acf-doc [function] argsort
template <class R, class Comp = ranges::less, class Proj = identity>
requires ranges::random_access_range<const R> && ranges::sized_range<const R> &&
         indirect_strict_weak_order<Comp, projected<ranges::iterator_t<const R>, Proj>>
vector<int> argsort(const R& values, Comp comp = {}, Proj projection = {}) {
    const int n = ac_detail::checked_range_index(ranges::size(values));
    vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    const auto first = ranges::begin(values);
    std::sort(order.begin(), order.end(), [&](int lhs, int rhs) {
        if (std::invoke(comp, std::invoke(projection, first[lhs]),
                             std::invoke(projection, first[rhs]))) return true;
        if (std::invoke(comp, std::invoke(projection, first[rhs]),
                             std::invoke(projection, first[lhs]))) return false;
        return lhs < rhs;
    });
    return order;
}

// マクロは変数束縛とラムダ生成だけを行う。処理本体は上の関数と共通。
#define AC_COLLECT_BIND_0(names, value) auto&& names = value;
#define AC_COLLECT_BIND_1(names, value) auto&& [AC_COLLECT_UNPACK_ names] = value;
#define AC_COLLECT_LAMBDA_I_(names, id, ...) \
    [&](auto&& AC_COLLECT_CAT_(ac_collect_value_, id)) { \
        AC_COLLECT_CAT_(AC_COLLECT_BIND_, AC_COLLECT_IS_TUPLE_(names))( \
            names, AC_COLLECT_CAT_(ac_collect_value_, id)) \
        return (__VA_ARGS__); \
    }
#define AC_COLLECT_LAMBDA_(names, ...) AC_COLLECT_LAMBDA_I_(names, __COUNTER__, __VA_ARGS__)

// @acf macro COLLECT — (vars, range, expression) -> vector<T> — 配列の内包記法
/// @acf-doc @brief 変数 x または分解変数 (x,y,...) を束縛し、式を各要素へ1回適用。入力式は1回評価、入力順を保つ。map_vec と同じ値保存・計算量。外側の変数は参照キャプチャ
/// @acf-doc [macro] COLLECT
#define COLLECT(vars, range, ...) \
    ::map_vec((range), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro SUM — (vars, range, expression) -> expression_type — 式の値を総和に集計
/// @acf-doc @brief sum_byへの入口。COLLECTと同じ束縛構文でconstの葉を参照し、入力式を1回、式を各葉へ1回評価する。式の値型で集計し、空rangeはその型の{}。中間配列は作らない。外側の変数は参照キャプチャ
/// @acf-doc [macro] SUM
#define SUM(vars, range, ...) \
    ::sum_by((range), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_IF — (vars, range, condition, expression) -> vector<T> — 条件付き配列内包記法
/// @acf-doc @brief 条件は各要素へ1回、式は採用要素だけへ1回適用。中間配列なし。map_if と同じ値保存・計算量。範囲や条件内のカンマは括弧で囲む
/// @acf-doc [macro] COLLECT_IF
#define COLLECT_IF(vars, range, condition, ...) \
    ::map_if((range), AC_COLLECT_LAMBDA_(vars, condition), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_SET — (vars, range, expression) -> set<T> — 集合の内包記法
/// @acf-doc @brief 式の値を set に直接挿入して重複を除く。変数束縛は COLLECT と同じ。時間 O(N log(D+1))、追加メモリ O(D)（変換・比較・コピーを除く）
/// @acf-doc [macro] COLLECT_SET
#define COLLECT_SET(vars, range, ...) \
    ::ac_detail::collection::collect<true>( \
        (range), ::ac_detail::collection::accept_all{}, AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_SET_IF — (vars, range, condition, expression) -> set<T> — 条件付き集合内包記法
/// @acf-doc @brief 条件に合う要素だけ変換して set に直接挿入。条件は各要素へ1回、式は採用要素だけへ1回。時間 O(N+K log(D+1))、追加メモリ O(D)（述語・変換・比較・コピーを除く）
/// @acf-doc [macro] COLLECT_SET_IF
#define COLLECT_SET_IF(vars, range, condition, ...) \
    ::ac_detail::collection::collect<true>( \
        (range), AC_COLLECT_LAMBDA_(vars, condition), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))
