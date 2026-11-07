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
/// 出力しないtrace。LOCALでない場合などに使い、引数も評価しない。
#define trace(...) ((void)0)
#endif
#ifndef trace_idx
/// 出力しないtrace_idx。LOCALでない場合などに使い、引数も評価しない。
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

/// 自動で mod 1,000,000,007 を取る型。定数は `MOD1000000007`
using mint1000000007 = atcoder::modint1000000007;
// @acf using mint998244353 — ACL modint（法 998,244,353）

/// 自動で mod 998,244,353 を取る型。定数は `MOD998244353`
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

/// 64ビットの符号付き整数。約±9×10^18まで表せる `long long` の短縮名。
using ll = long long;
// @acf using ull — unsigned long long

/// 64ビットの符号なし整数。負数は表せない `unsigned long long` の短縮名。
using ull = unsigned long long;
// @acf using i128 — __int128

/// `__int128`（GCC/Clang）。64bitを超える整数。`in` / `print` 対応
using i128 = __int128_t;
// @acf using u128 — unsigned __int128

/// `unsigned __int128`（GCC/Clang）。`in` / `print` 対応
using u128 = __uint128_t;
// @acf using ui — unsigned int

/// 符号なしの `unsigned int`。負数を扱う用途には使わない。
using ui = unsigned int;
// @acf using bs — bitset<N>

/// bitset<N>（長さ N は型の一部。例: bs<64>）
template <size_t N>
using bs = bitset<N>;
// @acf using bs32 — bitset<32>

/// bitset<32>
using bs32 = bitset<32>;
// @acf using bs64 — bitset<64>

/// bitset<64>
using bs64 = bitset<64>;
// @acf using db — double

/// 小数を扱う `double` の短縮名。
using db = double;
// @acf using ld — long double

/// 小数を扱う `long double` の短縮名。精度は処理系に依存する。
using ld = long double;
// @acf using str — string

/// 文字列を扱う `string` の短縮名。
using str = string;
// @acf using vi — vector<int>

/// intの可変長配列。例: `vi a(3, 0);` は0が3個の配列。
using vi = vector<int>;
// @acf using vll — vector<ll>

/// long longの可変長配列。大きな整数や累積和を保存するときに使う。
using vll = vector<ll>;
// @acf using vdb — vector<db>

/// doubleの可変長配列。
using vdb = vector<db>;
// @acf using vb — vector<bool>

/// 真偽値の可変長配列。`vector<bool>`なので要素は通常のbool参照ではない。
using vb = vector<bool>;
// @acf using vc — vector<char>

/// 文字の可変長配列。文字列として使う場合は `str` が便利。
using vc = vector<char>;
// @acf using vs — vector<str>

/// 文字列の可変長配列。文字の盤面や複数の単語を保存する。
using vs = vector<str>;
// @acf using vvi — vector<vector<int>>

/// intの2次元可変長配列。例: `vvi a(2, vi(3));` は2行3列。
using vvi = vector<vi>;
// @acf using vvll — vector<vector<ll>>

/// long longの2次元可変長配列。各行の長さは独立に変更できる。
using vvll = vector<vll>;
// @acf using pii — pair<int,int>

/// intを2つまとめる型。`.first` と `.second` で取り出す。
using pii = pair<int, int>;
// @acf using pll — pair<ll,ll>

/// long longを2つまとめる型。`.first` と `.second` で取り出す。
using pll = pair<ll, ll>;
// @acf using pss — pair<str,str>

/// 文字列を2つまとめる型。
using pss = pair<str, str>;
// @acf using vpii — vector<pii>

/// intの2要素組を並べた配列。座標や辺の一覧などに使う。
using vpii = vector<pii>;
// @acf using vpll — vector<pll>

/// long longの2要素組を並べた配列。
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

/// 整数なら一段広い型。`int`→`ll`、`ll`→`i128`、非整数はそのまま
template <class T>
using wide_t = typename decltype(ac_detail::wide_type_tag<T>())::type;

// @acf using product_type_t — 広い積の型（整数は wide_t<common_type>）

/// 明示的に広い積の型。整数なら `wide_t<common_type_t<...>>`（`ll`→`i128`）、それ以外は `common_type_t`。`wide_mul` 用
template <class... Ts>
using product_type_t = conditional_t<(is_integral_v<remove_cv_t<Ts>> && ...),
                                     wide_t<common_type_t<Ts...>>,
                                     common_type_t<Ts...>>;

// @acf using product_result_t — 幾何などの積APIの戻り型（int→ll、llはそのまま）

/// 積APIの戻り型。整数は `int`→`ll`、`ll`/`i128` はそのまま。中間計算はより広い型で行い、最後に戻り型へ変換する
template <class... Ts>
using product_result_t =
    conditional_t<(is_integral_v<remove_cv_t<Ts>> && ...),
                  typename decltype(ac_detail::product_result_tag<
                      common_type_t<Ts...>>())::type,
                  common_type_t<Ts...>>;

// @acf fn wide_mul — (a,b) -> product_type_t — オーバーフローしにくい掛け算（戻りも広げる）

/// 掛け算の前に型を広げて積を求める。
///
/// 引数 `a`: 第1因子。
///
/// 引数 `b`: 第2因子。
///
/// 戻り値: `product_type_t<A,B>`。int同士はll、llを含む整数はi128などへ広げます。無限精度やオーバーフロー検出ではないので戻り型の範囲に収まること。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 通常のa*bでは掛け算後のキャストでは防げない中間オーバーフローに注意。幾何APIの戻り型product_result_tとは区別します。
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

/// 積を法で割った非負の余りを求める。
///
/// 引数 `a`: 第1因子。
///
/// 引数 `b`: 第2因子。同じMintまたはそのMintへ構築可能な値。
///
/// 引数 `m`: 正の法。
///
/// 戻り値: 整数版はllで[0,m)。m<=0はinvalid_argument。modint版は同じMint型。
///
/// 注意: 整数版は各因子を正規化してi128で掛けるためllの積のオーバーフローを避けます。入力は値渡しで変更しません。
///
/// 呼び分け: mul_mod(a,b,m) / mul_mod(modint,factor)。modint版にはACLが必要で法の引数はありません。
inline ll mul_mod(ll a, ll b, ll m) {
    if (m <= 0) throw invalid_argument("mul_mod: mod must be positive");
    a %= m;
    if (a < 0) a += m;
    b %= m;
    if (b < 0) b += m;
    return static_cast<ll>(static_cast<i128>(a) * b % m);
}

// @acf fn wrap0 — (ll x, ll n) -> ll — xを0-indexの循環範囲 [0,n) へ正規化

/// 負数も含む整数を0始まりの循環範囲へ正規化する。
///
/// 引数 `x`: 正規化する値。
///
/// 引数 `n`: 正の周期。
///
/// 戻り値: llで[0,n)。n<=0はinvalid_argument。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 0-indexの循環はwrap0、1-indexはwrap1。
///
/// 例: `print(wrap0(-1,5),wrap0(5,5));`
constexpr ll wrap0(ll x, ll n) {
    if (n <= 0) throw invalid_argument("wrap0: n must be positive");
    const ll r = x % n;
    return r < 0 ? r + n : r;
}

// @acf fn wrap1 — (ll x, ll n) -> ll — xを1-indexの循環範囲 [1,n] へ正規化

/// 負数も含む整数を1始まりの循環範囲へ正規化する。
///
/// 引数 `x`: 正規化する値。
///
/// 引数 `n`: 正の周期。
///
/// 戻り値: llで[1,n]。n<=0はinvalid_argument。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 0-indexの循環はwrap0、1-indexはwrap1。
///
/// 例: `print(wrap1(0,5),wrap1(-1,5));`
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

/// `__int128` を10進文字列として読む。`in` / `rd` / `print` からも使える
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

/// `__int128` を10進で書く。`print` からも使える
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

/// map<int,int>
using mii = map<int, int>;
// @acf using mll — map<ll,ll>

/// map<ll,ll>
using mll = map<ll, ll>;
// @acf using msi — map<str,int>

/// map<str,int>
using msi = map<str, int>;
// @acf using si — set<int>

/// set<int>
using si = set<int>;
// @acf using sll — set<ll>

/// set<ll>
using sll = set<ll>;
// @acf using maxpq — priority_queue<T>（最大ヒープ）

/// 最大値から取り出す優先度付きキュー。`top()`で参照、`pop()`で削除する。
template <class T>
using maxpq = priority_queue<T>;
// @acf using minpq — priority_queue<T,vector<T>,greater<T>>（最小ヒープ）

/// 最小値から取り出す優先度付きキュー。`top()`で参照、`pop()`で削除する。
template <class T>
using minpq = priority_queue<T, vector<T>, greater<T>>;

// set と同じ同値判定で重複をまとめる優先度付きキュー。
// Compareはpriority_queueと同じ比較順。less<T>で最大、greater<T>で最小を先に取り出す。
// push/emplace/contains/erase: O(log N)、top/size/empty: O(1)、pop: 償却 O(1)。
/// 同じ値を1つにまとめる優先度付きキュー。既定では最大値から取り出す。
/// `setpq<int, greater<int>>` なら最小値から取り出す。重複を残す場合は `maxpq` / `minpq`。
template <class T, class Compare = less<T>>
class setpq {
    set<T, Compare> values_;

   public:
    using value_type = T;
    using size_type = typename set<T, Compare>::size_type;

    /// 空のキューを作る。
    setpq() = default;
    /// 指定した比較順で、空のキューを作る。
    explicit setpq(const Compare& comp) : values_(comp) {}

    /// 要素が0個ならtrueを返す。
    bool empty() const { return values_.empty(); }
    /// 重複を除いた要素数を返す。
    size_type size() const { return values_.size(); }
    /// 次に取り出す要素を参照で返す。要素は削除しない。空のときは使えない。
    const T& top() const {
#ifdef LOCAL
        assert(!empty());
#endif
        return *values_.rbegin();
    }
    /// 値を追加する。新しく追加できたらtrue、同値の要素が既にあればfalse。
    bool push(const T& value) { return values_.insert(value).second; }
    /// 値を追加する。新しく追加できたらtrue、同値の要素が既にあればfalse。
    bool push(T&& value) { return values_.insert(std::move(value)).second; }
    /// 引数から要素を作って追加する。追加できたらtrue、同値の要素が既にあればfalse。
    template <class... Args>
    bool emplace(Args&&... args) {
        return values_.emplace(std::forward<Args>(args)...).second;
    }
    /// top()の要素を1つ削除する。値は返さない。空のときは使えない。
    void pop() {
#ifdef LOCAL
        assert(!empty());
#endif
        values_.erase(prev(values_.end()));
    }
    /// 比較順で同値とみなされる要素があればtrueを返す。
    bool contains(const T& value) const { return values_.contains(value); }
    /// 指定した値を削除し、削除できたらtrueを返す。不在ならfalse。
    bool erase(const T& value) { return values_.erase(value) != 0; }
    /// 全要素を取り除いて空にする。
    void clear() { values_.clear(); }
};

// @acf const INF — 距離 DP 用の十分大きい値

/// 最短距離やDPの未到達を表すための大きな整数。値は2^60−1。無限大そのものではなく、文字列検索の不在判定には `NPOS` を使う。
constexpr ll INF = (1LL << 60) - 1;
// @acf macro NPOS — string::npos

/// 文字列検索で「見つからない」を判定する定数。
///
/// 戻り値: `string::npos`（`string::size_type`）。有効な添字ではありません。
///
/// 注意: 副作用なし。検索結果を int に変換する前に比較します。
///
/// 呼び分け: 括弧なしの NPOS。文字列以外の探索失敗値へ一律に使うものではありません。
#define NPOS string::npos
// @acf const EPS — 浮動小数比較の既定許容誤差（1e-12）

/// 浮動小数の絶対誤差比較で使う既定値。`eq` / `sgn` の第3・第2引数の既定
constexpr ld EPS = 1e-12L;
// @acf const PI — 円周率（long double）

/// 円周率。ラジアン計算・`to_rad` / `to_deg` で使う
constexpr ld PI = 3.141592653589793238462643383279502884L;
// @acf const MOD1000000007 — mod 1,000,000,007（mint1000000007 と対応）

/// mod 1,000,000,007。ACL型は `mint1000000007`
constexpr int MOD1000000007 = 1'000'000'007;
// @acf const MOD998244353 — mod 998,244,353（mint998244353 と対応）

/// mod 998,244,353。ACL型は `mint998244353`
constexpr int MOD998244353 = 998'244'353;
// @acf const O — 「丸」を表す小文字の char

/// グリッドなどで使う「丸」の小文字 `o`
constexpr char O = 'o';
// @acf const X — 「罰」を表す小文字の char

/// グリッドなどで使う「罰」の小文字 `x`
constexpr char X = 'x';
// @acf const ALPHABET_LOWER — 小文字a〜zを順に並べた26文字のstring

/// `inline const str`。内容は `abcdefghijklmnopqrstuvwxyz`。`str s = ALPHABET_LOWER;` で編集用にコピーできる
inline const str ALPHABET_LOWER = "abcdefghijklmnopqrstuvwxyz";
// @acf const ALPHABET_UPPER — 大文字A〜Zを順に並べた26文字のstring

/// `inline const str`。内容は `ABCDEFGHIJKLMNOPQRSTUVWXYZ`。`str s = ALPHABET_UPPER;` で編集用にコピーできる
inline const str ALPHABET_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// @acf const kuku — 1〜9の積を格納した9×9の九九表

/// `kuku[i][j] == (i+1)*(j+1)`。添字は0〜8、値はint。読み取り専用の固定長配列で、動的確保なし。`auto table = kuku;`で変更可能なコピーを作る
inline constexpr array<array<int, 9>, 9> kuku = [] {
    array<array<int, 9>, 9> table{};
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            table[i][j] = (i + 1) * (j + 1);
    return table;
}();

// @acf const dx4 — 4近傍の行方向差分（r + dx4[d]）

/// 4近傍の行方向差分。`r + dx4[d]`
inline constexpr array<int, 4> dx4 = {1, 0, -1, 0};
// @acf const dy4 — 4近傍の列方向差分（c + dy4[d]）

/// 4近傍の列方向差分。`c + dy4[d]`
inline constexpr array<int, 4> dy4 = {0, 1, 0, -1};
// @acf const dx8 — 8近傍の行方向差分（r + dx8[d]）

/// 8近傍の行方向差分。`r + dx8[d]`
inline constexpr array<int, 8> dx8 = {1, 1, 0, -1, -1, -1, 0, 1};
// @acf const dy8 — 8近傍の列方向差分（c + dy8[d]）

/// 8近傍の列方向差分。`c + dy8[d]`
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
    /// 引数tupleをunordered_map用のハッシュ値へ変換する。
    template <class... Ts>
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
    /// メモ化対象の関数オブジェクトを登録する。
    explicit memoized(F function) : function_(std::move(function)) {}

    /// 引数に対応する結果をキャッシュから返し、未計算なら計算して保存する。
    R operator()(Args... args) {
        key_type key(args...);
        if (auto it = cache_.find(key); it != cache_.end()) return it->second;

        R result = std::invoke(function_, *this, args...);
        auto [it, inserted] = cache_.emplace(std::move(key), std::move(result));
        (void)inserted;
        return it->second;
    }

    /// 保存済みのメモ化結果をすべて削除する。
    void clear() { cache_.clear(); }

    /// キャッシュの容量をあらかじめ確保する。
    void reserve(size_t state_count) { cache_.reserve(state_count); }

    /// 保存済みのメモ化結果の個数を返す。
    size_t cache_size() const { return cache_.size(); }
};
}  // namespace ac_detail

// @acf fn recur — (F function) -> callable — 自己参照を受け取る再帰 callable を作る

/// lambdaの第1引数から自分を呼べる再帰callableを作る。
///
/// 引数 `function`: 第1引数でselfを受け取り、残りで状態を受け取るcallable。
///
/// 戻り値: functionを値として持つcallable。戻り値はlambdaの戻り値をそのまま転送。停止条件と有効な引数はlambda側で決めます。
///
/// 注意: 構築時にcallableをコピー/移動。呼び出し時の更新・計算量・再帰の深さはlambdaに依存し、メモ化はしません。
///
/// 呼び分け: 引数ごとの結果を保存する場合はmemoize。
template <class F>
auto recur(F&& function) {
    return ac_detail::recursive_callable<std::decay_t<F>>(std::forward<F>(function));
}

// @acf fn memoize — Python の @cache 相当のメモ化再帰

/// 再帰callableの結果を引数tupleごとに保存する。
///
/// 引数 `function`: 第1引数にself、以後にSignatureの引数を取るcallable。
///
/// 戻り値: メモ化callable。Signatureは非voidでコピー可能な戻り値RのR(Args...)を明示。キーは減衰型のtupleで各要素のhash/==が必要。cache_sizeは保存数、clearは全削除。
///
/// 注意: 未保存の呼び出しだけlambdaを実行して内部cacheを変更。lambdaは同じ引数で安定した結果を返すこと。キーを保存する前に再帰するため、循環依存を自動検出しません。
///
/// 呼び分け: `memoize<Signature>`(function)。計算量は到達状態と遷移・hash/コピー費用に依存します。
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

#define AC_REP_2_0(i, n) rep_3(i, 0, n)
#define AC_REP_2_1(names, range) for (const auto& [AC_COLLECT_UNPACK_ names] : (range))
#define rep_2(i, n) AC_REP_JOIN_(AC_REP_2_, AC_COLLECT_IS_TUPLE_(i))(i, n)
#define rep_3(i, lo, hi) rep_5(i, lo, ac_detail::iteration::count_or_size(hi), true, false)
#define rep_5(i, lo, hi, include_lo, include_hi) AC_REP_LOOP_(i, lo, hi, include_lo, include_hi, false)
// @acf macro rep — rep(i,n) / rep(i,range) / rep(i,lo,hi) / rep(i,lo,range) / rep(i,lo,hi,include_lo,include_hi): 昇順

/// 整数区間やrangeの添字を昇順に走査する。rep((k,v),range)では各要素を構造化束縛で受け取る。
///
/// 引数 `i`: 新しく宣言されるループ変数。型はll。rangeでも値ではなく添字。
///
/// 引数 `n / lo, hi`: 2引数なら[0,n)、3引数なら[lo,hi)。2/3引数の上限にrangeを渡すとそのサイズ。
///
/// 引数 `(k,v,...) / range`: 構造化束縛の2引数形式。各要素をconst auto&で分解する。変数の数は要素に合わせる。サイズは不要。
///
/// 引数 `include_lo, include_hi`: 5引数形式のときだけ渡す。trueはその端点を含む。
///
/// 戻り値: 返り値はありません。for文へ展開します。空区間では本文を一度も実行しません。
///
/// 注意: 境界・rangeのサイズは開始時に1回評価します。iはループ内だけで使えます。開始後にrangeを変更しても上限は再計算されません。 構造化束縛形式はrange式を1回評価し、要素をコピーしません。束縛名はループ内だけで使えます。通常の範囲forと同じ走査順・break・continueの動作です。
///
/// 呼び分け: rep(i,n)、rep(i,lo,hi)、rep(i,lo,hi,include_lo,include_hi)の2/3/5引数。4引数形式はありません。3引数までの既定区間は左を含み右を含みません。 rep((k,v),range)は構造化束縛用です。{k,v}や[k,v]ではカンマがマクロ引数を分けるため、丸括弧を使います。値の更新やイテレータ区間の走査にはrepitを使います。rrepは構造化束縛に対応しません。
#define rep(...) AC_REP_SELECT_(__VA_ARGS__, rep_5, AC_REP_BAD_ARITY_, rep_3, rep_2, AC_REP_BAD_ARITY_)(__VA_ARGS__)
#define rrep_2(i, n) rrep_3(i, 0, n)
#define rrep_3(i, lo, hi) rrep_5(i, lo, ac_detail::iteration::count_or_size(hi), true, false)
#define rrep_5(i, lo, hi, include_lo, include_hi) AC_REP_LOOP_(i, lo, hi, include_lo, include_hi, true)
// @acf macro rrep — rrep(i,n) / rrep(i,range) / rrep(i,lo,hi) / rrep(i,lo,range) / rrep(i,lo,hi,include_lo,include_hi): 同じ区間を逆順

/// 整数区間やrangeの添字を逆順に走査する。
///
/// 引数 `i`: 新しく宣言されるループ変数。型はll。rangeでも値ではなく添字。
///
/// 引数 `n / lo, hi`: 2引数なら[0,n)、3引数なら[lo,hi)。2/3引数の上限にrangeを渡すとそのサイズ。
///
/// 引数 `include_lo, include_hi`: 5引数形式のときだけ渡す。trueはその端点を含む。
///
/// 戻り値: 返り値はありません。for文へ展開します。空区間では本文を一度も実行しません。
///
/// 注意: 境界・rangeのサイズは開始時に1回評価します。iはループ内だけで使えます。開始後にrangeを変更しても上限は再計算されません。
///
/// 呼び分け: rrep(i,n)、rrep(i,lo,hi)、rrep(i,lo,hi,include_lo,include_hi)の2/3/5引数。4引数形式はありません。3引数までの既定区間は左を含み右を含みません。
#define rrep(...) AC_REP_SELECT_(__VA_ARGS__, rrep_5, AC_REP_BAD_ARITY_, rrep_3, rrep_2, AC_REP_BAD_ARITY_)(__VA_ARGS__)
// イテレータの2/3引数ディスパッチ。整数ループの2/3/5引数とは分離する。
#define rep_select(_1, _2, _3, NAME, ...) NAME
#define repi_1(n) for (int n = rd<int>(), _ = 0; _ < n; ++_)
#define repi_2(i, n) for (int n = rd<int>(), i = 0; i < n; ++i)
#define repi_select(_1, _2, NAME, ...) NAME
// @acf macro repi — repi(n): int n を宣言+in して n 回 / repi(i,n): ループ変数 i 付き

/// 標準入力からintの回数を読み、0から順に繰り返す。
///
/// 引数 `i`: 新しく宣言するループ変数。型はint。
///
/// 引数 `n`: 入力した回数を格納する新しいint変数。値や式ではなく名前を渡します。
///
/// 戻り値: 戻り値なし。回数が0以下なら本文を実行しません。
///
/// 注意: 開始時に `rd<int>`() を1回呼びます。nとiはfor文のスコープ内だけ。本文でnを変えると次の継続判定も変わります。入力成功はcinの状態で確認します。
///
/// 呼び分け: repi(n) は内部変数 _、repi(i,n) は指定したiを使用。名前を重ねず、1引数のnに _ を使いません。
#define repi(...) repi_select(__VA_ARGS__, repi_2, repi_1)(__VA_ARGS__)
#define repl_1(n) for (ll n = rd<ll>(), _ = 0; _ < n; ++_)
#define repl_2(i, n) for (ll n = rd<ll>(), i = 0; i < n; ++i)
// @acf macro repl — repl(n): ll n を宣言+in して n 回 / repl(i,n): ループ変数 i 付き

/// 標準入力からllの回数を読み、0から順に繰り返す。
///
/// 引数 `i`: 新しく宣言するループ変数。型はll。
///
/// 引数 `n`: 入力した回数を格納する新しいll変数。値や式ではなく名前を渡します。
///
/// 戻り値: 戻り値なし。回数が0以下なら本文を実行しません。
///
/// 注意: 開始時に `rd<ll>`() を1回呼びます。nとiはfor文のスコープ内だけ。本文でnを変えると次の継続判定も変わります。入力成功はcinの状態で確認します。
///
/// 呼び分け: repl(n) は内部変数 _、repl(i,n) は指定したiを使用。名前を重ねず、1引数のnに _ を使いません。
#define repl(...) repi_select(__VA_ARGS__, repl_2, repl_1)(__VA_ARGS__)
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
// @acf macro repit — repit(it,r): コンテナ or equal_range の pair / repit(it,a,b): it∈[a,b)

/// コンテナまたはイテレータ区間の要素を順に走査する。
///
/// 引数 `it`: 新しいイテレータ変数。*itで要素へアクセスします。
///
/// 引数 `r`: 2引数形式。begin/endを持つ対象、またはequal_rangeの結果。
///
/// 引数 `a, b`: 3引数形式の始端と終端。[a,b)が有効な区間であること。
///
/// 戻り値: 戻り値なし。空区間は本文を実行しません。
///
/// 注意: r、またはaとbを開始時に各1回評価し、終端を保持します。イテレータはfor内だけ。要素への書き込みは可能ですが、走査中にイテレータを無効化しません。コンテナを所有しないため、一時コンテナを渡すと本文でダングリングになります。
///
/// 呼び分け: repit(it,r) / repit(it,a,b)。3引数はauto宣言を共有するので始端と終端が同じ型。添字が必要ならrep。
#define repit(...) rep_select(__VA_ARGS__, repit_3, repit_2)(__VA_ARGS__)
// @acf macro cauto — const auto

/// 型を推論したconst変数・const参照を宣言する。
///
/// 戻り値: 値は返しません。const autoという宣言用トークンへ置換します。
///
/// 注意: cauto x=vは値をコピー、cauto& x=vはconst参照で束縛します。初期化式の評価は通常のC++宣言と同じく1回。変数のスコープも宣言したブロックに従います。
///
/// 呼び分け: cauto value=expr; / cauto& ref=expr; / cauto& [a,b]=pair;。関数形式ではありません。
#define cauto const auto
// @acf macro ALL — (v).begin(), (v).end()

/// コンテナの通常のbegin/endを標準アルゴリズムへ渡す。
///
/// 引数 `v`: 同じコンテナ変数を渡す。関数呼び出しなど副作用のある式を渡さない。
///
/// 戻り値: 単一の戻り値ではなく、2つのiterator式（カンマで分離された引数列）へ展開します。
///
/// 注意: vは2回評価されます。ALL(make_vector())のような呼び出しは別々の一時コンテナのiteratorを組み合わせるため使いません。マクロ自身は要素を書き換えず、sortなど渡した先が変更します。
///
/// 呼び分け: std::sort(ALL(v))など、iteratorを2引数として受ける場所へ使います。
#define ALL(v) (v).begin(), (v).end()
// @acf macro RALL — (v).rbegin(), (v).rend()

/// コンテナの逆順begin/endを標準アルゴリズムへ渡す。
///
/// 引数 `v`: 同じコンテナ変数を渡す。関数呼び出しなど副作用のある式を渡さない。
///
/// 戻り値: 単一の戻り値ではなく、2つのiterator式（カンマで分離された引数列）へ展開します。
///
/// 注意: vは2回評価されます。ALL(make_vector())のような呼び出しは別々の一時コンテナのiteratorを組み合わせるため使いません。マクロ自身は要素を書き換えず、sortなど渡した先が変更します。
///
/// 呼び分け: std::sort(RALL(v))など、iteratorを2引数として受ける場所へ使います。
#define RALL(v) (v).rbegin(), (v).rend()
// @acf macro ALL_EQ — 全引数が等しければ true（本体は all_equal）

/// 全ての引数が最初の値と等しいかを判定する。
///
/// 引数 `values...`: 先頭と各後続値の==が有効な型。異なる型も比較が成立すれば可。
///
/// 戻り値: bool。0個・1個はtrue。1つでも異なる値があればfalse。
///
/// 注意: 全ての実引数は関数へ渡す前に各1回評価されます。実引数の評価順は左からとは限りません。==比較は先頭から短絡しますが、実引数の評価を省略するものではありません。
///
/// 呼び分け: ALL_EQ(a,b,...)はall_equal(a,b,...)の呼び出し。候補のどれかと等しいかはany_equal。
///
/// 例: `print(ALL_EQ(2,2,2),ALL_EQ(2,3),ALL_EQ());`
#define ALL_EQ(...) all_equal(__VA_ARGS__)
// @acf macro sz — (int)(v).size()

/// コンテナの要素数をintとして取得する。
///
/// 引数 `v`: 要素数がintで表せるコンテナや文字列。
///
/// 戻り値: int。空なら0。大きすぎるサイズの安全な検査や切り詰めは行いません。
///
/// 注意: vを1回評価してsize()を呼び、その結果をintへ明示変換。コンテナは変更しません。
///
/// 呼び分け: sz(v)。intへの変換が不要、またはintに収まらない場合はv.size()を使用します。
#define sz(v) ((int)(v).size())
// @acf macro cont_if — cont_if(cond): cond が true なら continue

/// 条件が真なら現在の反復を終え、次の反復へ進む。
///
/// 引数 `cond`: 一度評価する条件。ループ内（break_ifはswitch内でも可）で使います。
///
/// 戻り値: 戻り値なし。条件が偽ならそのまま次の文へ進みます。
///
/// 注意: 条件を1回評価します。continueの対象は通常のC++文と同じ。if/elseの入れ子でも外側のelseを奪わない展開です。
///
/// 呼び分け: cont_if(cond);。式の右辺には使えません。呼び出しの末尾にセミコロンを書きます。
#define cont_if(...) if (__VA_ARGS__) { continue; } else (void)0
// @acf macro break_if — break_if(cond): cond が true なら break

/// 条件が真なら最も内側のループまたはswitchを抜ける。
///
/// 引数 `cond`: 一度評価する条件。ループ内（break_ifはswitch内でも可）で使います。
///
/// 戻り値: 戻り値なし。条件が偽ならそのまま次の文へ進みます。
///
/// 注意: 条件を1回評価します。breakの対象は通常のC++文と同じ。if/elseの入れ子でも外側のelseを奪わない展開です。
///
/// 呼び分け: break_if(cond);。式の右辺には使えません。呼び出しの末尾にセミコロンを書きます。
#define break_if(...) if (__VA_ARGS__) { break; } else (void)0
// @acf macro ret_if — ret_if(cond): cond が true なら return / ret_if(cond, val): return val

/// 条件が真なら現在の関数・ラムダからreturnする。
///
/// 引数 `cond`: 一度評価する条件。コンマを含む条件は括弧で囲みます。
///
/// 引数 `value`: 真の場合だけ評価する戻り値。省略はvoidを返す関数で使います。
///
/// 戻り値: マクロ自体の戻り値はありません。真なら呼び出し元の関数を終了します。
///
/// 注意: condを1回評価し、真なら戻り値式を1回評価。偽なら戻り値式を評価しません。ラムダ内ならラムダだけを終了します。
///
/// 呼び分け: ret_if(cond); / ret_if(cond,value);。末尾にセミコロンを付ける文マクロです。
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

/// 整数とACL modintの代表値を明示的に相互変換する。
///
/// 引数 `To`: 整数からはMint型、Mintからはbool以外の整数型。
///
/// 引数 `value`: 変換する値。
///
/// 戻り値: To。整数からMintは法へ正規化。Mintから整数は.val()の非負代表値で、Toに収まらなければoverflow_error。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: ACLが必要。Mintから元の負数を復元する変換ではありません。
template <class To, class From,
          enable_if_t<ac_detail::io::is_modint_v<To> &&
                          is_integral_v<remove_cvref_t<From>>,
                      int> = 0>
To mint_cast(From value) {
    return To(value);
}

/// 整数とACL modintの代表値を明示的に相互変換する。
///
/// 引数 `To`: 整数からはMint型、Mintからはbool以外の整数型。
///
/// 引数 `value`: 変換する値。
///
/// 戻り値: To。整数からMintは法へ正規化。Mintから整数は.val()の非負代表値で、Toに収まらなければoverflow_error。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: ACLが必要。Mintから元の負数を復元する変換ではありません。
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
/// 積を法で割った非負の余りを求める。
///
/// 引数 `a`: 第1因子。
///
/// 引数 `b`: 第2因子。同じMintまたはそのMintへ構築可能な値。
///
/// 引数 `m`: 正の法。
///
/// 戻り値: 整数版はllで[0,m)。m<=0はinvalid_argument。modint版は同じMint型。
///
/// 注意: 整数版は各因子を正規化してi128で掛けるためllの積のオーバーフローを避けます。入力は値渡しで変更しません。
///
/// 呼び分け: mul_mod(a,b,m) / mul_mod(modint,factor)。modint版にはACLが必要で法の引数はありません。
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

/// STL mutable range / pair / tuple / optional の入力
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

/// 型を指定して標準入力の値を返す、または既存変数へ一括入力する。
///
/// 引数 `a（既存変数版）`: 左から順に書き換える変数。コンテナは既存サイズの要素だけを読む。
///
/// 戻り値: 既存変数版は常にtrue、1型版はT、多型版はtuple。値を返す版は値初期化した対象へ入力し、成功statusは返しません。
///
/// 注意: cinを順に消費。必要な全値が正しく読める入力が前提で、失敗はcinの状態で確認。modintなどの入力もextractへ委譲します。
///
/// 呼び分け: scan(a,b) / `scan<T>`() / `scan<T,U,...>`()。inは同じ形式の別名。
template <class... T>
bool scan(T&... a) {
    (ac_detail::input::extract(cin, a), ...);
    return true;
}

/// 型を指定して標準入力の値を返す、または既存変数へ一括入力する。
///
/// 引数 `a（既存変数版）`: 左から順に書き換える変数。コンテナは既存サイズの要素だけを読む。
///
/// 戻り値: 既存変数版は常にtrue、1型版はT、多型版はtuple。値を返す版は値初期化した対象へ入力し、成功statusは返しません。
///
/// 注意: cinを順に消費。必要な全値が正しく読める入力が前提で、失敗はcinの状態で確認。modintなどの入力もextractへ委譲します。
///
/// 呼び分け: scan(a,b) / `scan<T>`() / `scan<T,U,...>`()。inは同じ形式の別名。
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

/// 宣言済みの変数へ入力するか、指定した型の値を入力して返す。
///
/// 引数 `a...`: 引数ありでは書き換える既存変数。引数なし形式では`in<T>`()のTで型を指定。
///
/// 戻り値: in(a,b,...)はboolで常にtrue（入力成功の判定ではありません）。`in<T>`()はT、`in<T,U,...>`()は`tuple<T,U,...>`。引数なしのin()はbool trueで何も読みません。
///
/// 注意: 標準入力を消費し、参照形式では変数を変更します。失敗状態はcin.fail()等で確認します。型を指定して返す形式は値初期化した値へ読み込むため、失敗時の値で成功判定しません。
///
/// 呼び分け: int x; in(x); ／ auto x=`in<int>`(); ／ auto [x,y]=`in<int,ll>`();。宣言入力マクロInt(x)と、既存変数に読むin(x)を区別します。
template <class... T>
bool in(T&... a) {
    return scan(a...);
}

/// 宣言済みの変数へ入力するか、指定した型の値を入力して返す。
///
/// 引数 `a...`: 引数ありでは書き換える既存変数。引数なし形式では`in<T>`()のTで型を指定。
///
/// 戻り値: in(a,b,...)はboolで常にtrue（入力成功の判定ではありません）。`in<T>`()はT、`in<T,U,...>`()は`tuple<T,U,...>`。引数なしのin()はbool trueで何も読みません。
///
/// 注意: 標準入力を消費し、参照形式では変数を変更します。失敗状態はcin.fail()等で確認します。型を指定して返す形式は値初期化した値へ読み込むため、失敗時の値で成功判定しません。
///
/// 呼び分け: int x; in(x); ／ auto x=`in<int>`(); ／ auto [x,y]=`in<int,ll>`();。宣言入力マクロInt(x)と、既存変数に読むin(x)を区別します。
template <class T>
T in() {
    return scan<T>();
}

template <class T, class U, class... Rest>
tuple<T, U, Rest...> in() {
    return scan<T, U, Rest...>();
}

// @acf fn rd — () -> T — 読んで返す（式向け）

/// 型を指定して標準入力の値を返す。
///
/// 戻り値: T。値を返す版は値初期化した対象へ入力し、成功statusは返しません。
///
/// 注意: cinを順に消費。必要な全値が正しく読める入力が前提で、失敗はcinの状態で確認。modintなどの入力もextractへ委譲します。
///
/// 呼び分け: `rd<T>`()は`in<T>`()の値形式。
template <class T>
T rd() {
    return in<T>();
}

// @acf fn in_tuple — () -> tuple<Ts...> — 型を指定して読んで structured bindings 向けに返す

/// 型を指定して標準入力の値を返す。
///
/// 戻り値: `tuple<Ts...>`。型を並べる順に読み、空の型列は空tuple。値を返す版は値初期化した対象へ入力し、成功statusは返しません。
///
/// 注意: cinを順に消費。必要な全値が正しく読める入力が前提で、失敗はcinの状態で確認。modintなどの入力もextractへ委譲します。
///
/// 呼び分け: 引数はなく、型列はテンプレート引数で渡します。
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

/// 値や配列を標準出力へ書き、通常は末尾を改行する。
///
/// 引数 `args...`: 出力可能な値。複数値は空白区切り。単一rangeは自然な区切りで出力。
///
/// 戻り値: boolで常にtrue。出力・入力の成功判定ではありません。print()と空の1次元rangeでも末尾の改行は出力します。2次元vectorが0行なら出力は空です。
///
/// 注意: 標準出力へ書きます。入力の値は変更しません。通常はflushせず、AC_INTERACTIVE時はfinish()経由でflushします。
///
/// 呼び分け: print(1,2)は値を2つ出力。print(range,',')／print(range,":")は要素区切り指定。2次元vectorは行ごとの出力。nl=falseは区切り指定形式だけです。
template <class... T, enable_if_t<(sizeof...(T) <= 1), int> = 0>
bool print(const T&... args) {
    ac_detail::output::writeln(cout, ' ', args...);
    return ac_detail::output::finish();
}

/// 値や配列を標準出力へ書き、通常は末尾を改行する。
///
/// 引数 `a`: 行ごとに出力する2次元vector。
///
/// 引数 `sep`: 行内の要素区切り。省略時は `' '`。
///
/// 戻り値: boolで常にtrue。出力・入力の成功判定ではありません。print()と空の1次元rangeでも末尾の改行は出力します。2次元vectorが0行なら出力は空です。
///
/// 注意: 標準出力へ書きます。入力の値は変更しません。通常はflushせず、AC_INTERACTIVE時はfinish()経由でflushします。
///
/// 呼び分け: print(a)またはprint(a,sep)。行内の区切りはcharで省略時は空白。この2次元専用形式にはnl引数はありません。0行のvectorは空出力、空の行は改行を出します。
///
/// 例: `print(vvi{{1, 2}, {3, 4}});`
template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool print(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::writeln(cout, ' ', a, b, rest...);
    return ac_detail::output::finish();
}

// @acf fn pr — (const T&... args) -> bool — 引数を連結して出力（改行なし）。常に true

/// 値を区切りも改行も付けずに連結出力する。
///
/// 引数 `args`: 連結して出力する値。0個も可。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。引数0個は無出力。単一rangeの要素も連結。
///
/// 注意: coutへ出力。改行とflushは行いません。入力値は変更しません。
///
/// 呼び分け: 区切りはpr(a,' ',b)と明示。range+sep形式は要素間だけを区切ります。
///
/// 例: `pr("x",2,'\n');`
template <class... T, enable_if_t<(sizeof...(T) <= 1), int> = 0>
bool pr(const T&... args) {
    (ac_detail::output::write_pr_item(cout, args), ...);
    return true;
}

/// 値を区切りも改行も付けずに連結出力する。
///
/// 引数 `args`: 連結して出力する値。0個も可。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。引数0個は無出力。単一rangeの要素も連結。
///
/// 注意: coutへ出力。改行とflushは行いません。入力値は変更しません。
///
/// 呼び分け: 区切りはpr(a,' ',b)と明示。range+sep形式は要素間だけを区切ります。
///
/// 例: `pr("x",2,'\n');`
template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool pr(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::write_pr_item(cout, a);
    ac_detail::output::write_pr_item(cout, b);
    (ac_detail::output::write_pr_item(cout, rest), ...);
    return true;
}

// @acf fn pr — (const R& r, char sep) -> bool — range を sep 区切りで出力（改行なし）

/// rangeの要素を指定した区切りで出力し、改行を付けない。
///
/// 引数 `r`: 走査可能なrange。空も可。
///
/// 引数 `sep`: 要素間の区切り。
///
/// 戻り値: boolで常にtrue。空rangeは無出力、末尾の区切りは付けません。
///
/// 注意: coutへ要素順に出力。改行/flushなし、入力を変更しません。
///
/// 呼び分け: pr(r,sep)。charまたはstring_view/文字列リテラルの区切り。
///
/// 例: `pr(vi{1,2},',');`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool pr(const R& r, char sep) {
    ac_detail::output::write_range(cout, r, sep);
    return true;
}

// @acf fn pr — (const R& r, string_view sep) -> bool — range を文字列 sep 区切りで出力（改行なし）

/// rangeの要素を指定した区切りで出力し、改行を付けない。
///
/// 引数 `r`: 走査可能なrange。空も可。
///
/// 引数 `sep`: 要素間の区切り。
///
/// 戻り値: boolで常にtrue。空rangeは無出力、末尾の区切りは付けません。
///
/// 注意: coutへ要素順に出力。改行/flushなし、入力を変更しません。
///
/// 呼び分け: pr(r,sep)。charまたはstring_view/文字列リテラルの区切り。
///
/// 例: `pr(vi{1,2},string_view("::"));`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool pr(const R& r, string_view sep) {
    ac_detail::output::write_range(cout, r, sep);
    return true;
}

// @acf fn print — (const R& r, char sep, bool nl=true) -> bool — range を char sep 区切りで出力

/// 値や配列を標準出力へ書き、通常は末尾を改行する。
///
/// 引数 `r`: 出力するrange。
///
/// 引数 `sep`: 要素間の区切り文字。
///
/// 引数 `nl`: 末尾改行の有無。省略時は `true`。
///
/// 戻り値: boolで常にtrue。出力・入力の成功判定ではありません。print()と空の1次元rangeでも末尾の改行は出力します。2次元vectorが0行なら出力は空です。
///
/// 注意: 標準出力へ書きます。入力の値は変更しません。通常はflushせず、AC_INTERACTIVE時はfinish()経由でflushします。
///
/// 呼び分け: print(1,2)は値を2つ出力。print(range,',')／print(range,":")は要素区切り指定。2次元vectorは行ごとの出力。nl=falseは区切り指定形式だけです。
///
/// 例: `print(vi{1, 2, 3}, ',');`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool print(const R& r, char sep, bool nl = true) {
    // nl=false は既存互換。改行なしの新規コードは pr(r, sep) を使う。
    ac_detail::output::write_range(cout, r, sep);
    if (nl) cout << '\n';
    return ac_detail::output::finish();
}

// @acf fn print — (const R& r, string_view sep, bool nl=true) -> bool — range を文字列 sep 区切りで出力

/// 値や配列を標準出力へ書き、通常は末尾を改行する。
///
/// 引数 `r`: 出力するrange。
///
/// 引数 `sep`: 要素間の区切り文字列。リテラルも可。
///
/// 引数 `nl`: 末尾改行の有無。省略時は `true`。
///
/// 戻り値: boolで常にtrue。出力・入力の成功判定ではありません。print()と空の1次元rangeでも末尾の改行は出力します。2次元vectorが0行なら出力は空です。
///
/// 注意: 標準出力へ書きます。入力の値は変更しません。通常はflushせず、AC_INTERACTIVE時はfinish()経由でflushします。
///
/// 呼び分け: print(1,2)は値を2つ出力。print(range,',')／print(range,":")は要素区切り指定。2次元vectorは行ごとの出力。nl=falseは区切り指定形式だけです。
///
/// 例: `print(vi{1, 2, 3}, "::");`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool print(const R& r, string_view sep, bool nl = true) {
    // nl=false は既存互換。改行なしの新規コードは pr(r, sep) を使う。
    ac_detail::output::write_range(cout, r, sep);
    if (nl) cout << '\n';
    return ac_detail::output::finish();
}

// @acf fn println — (const T&... args) -> bool — すべて改行区切りで出力。空 range は無出力。常に true

/// 値を改行区切りで出力する。
///
/// 引数 `args`: 出力する値の列。
///
/// 戻り値: boolで常にtrue。引数0個は1改行、単一空rangeは無出力。単一rangeは1要素1行。複数引数は各引数を改行で区切ります。
///
/// 注意: coutへ出力。通常はflushしません。AC_INTERACTIVEではfinishがflush。入力を変更しません。
///
/// 呼び分け: range+sep版は既存互換でprint(r,sep,nl)と同じ。新規コードでは区切りはprint、改行なしはprを使います。
///
/// 例: `println(1,2);`
inline bool println() {
    cout << '\n';
    return ac_detail::output::finish();
}

/// 値を改行区切りで出力する。
///
/// 引数 `args`: 出力する値の列。
///
/// 戻り値: boolで常にtrue。引数0個は1改行、単一空rangeは無出力。単一rangeは1要素1行。複数引数は各引数を改行で区切ります。
///
/// 注意: coutへ出力。通常はflushしません。AC_INTERACTIVEではfinishがflush。入力を変更しません。
///
/// 呼び分け: range+sep版は既存互換でprint(r,sep,nl)と同じ。新規コードでは区切りはprint、改行なしはprを使います。
///
/// 例: `println(1,2);`
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

/// 値を改行区切りで出力する。
///
/// 引数 `args`: 出力する値の列。
///
/// 戻り値: boolで常にtrue。引数0個は1改行、単一空rangeは無出力。単一rangeは1要素1行。複数引数は各引数を改行で区切ります。
///
/// 注意: coutへ出力。通常はflushしません。AC_INTERACTIVEではfinishがflush。入力を変更しません。
///
/// 呼び分け: range+sep版は既存互換でprint(r,sep,nl)と同じ。新規コードでは区切りはprint、改行なしはprを使います。
///
/// 例: `println(1,2);`
template <class A, class B, class... Rest,
          enable_if_t<!ac_detail::io::prefer_range_sep_print_v<A, B, Rest...>, int> = 0>
bool println(const A& a, const B& b, const Rest&... rest) {
    ac_detail::output::writeln(cout, '\n', a, b, rest...);
    return ac_detail::output::finish();
}

// @acf fn println — (const R& r, char sep, bool nl=true) -> bool — 既存互換の区切り指定

/// rangeを指定区切りで出力する既存互換形式。
///
/// 引数 `r`: 走査可能なrange。
///
/// 引数 `sep`: 要素間の区切り。
///
/// 引数 `nl`: trueなら最後に改行。省略時は `true`。
///
/// 戻り値: boolで常にtrue。空rangeでもnl=trueなら改行を1つ出力。
///
/// 注意: coutへ出力。通常はflushせずAC_INTERACTIVE時はflush。入力は変更しません。
///
/// 呼び分け: print(r,sep,nl)へ委譲。改行なしはpr(r,sep)が明確です。
///
/// 例: `println(vi{1,2},',');`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool println(const R& r, char sep, bool nl = true) {
    return ::print(r, sep, nl);
}

// @acf fn println — (const R& r, string_view sep, bool nl=true) -> bool — 既存互換の区切り指定

/// rangeを指定区切りで出力する既存互換形式。
///
/// 引数 `r`: 走査可能なrange。
///
/// 引数 `sep`: 要素間の区切り。
///
/// 引数 `nl`: trueなら最後に改行。省略時は `true`。
///
/// 戻り値: boolで常にtrue。空rangeでもnl=trueなら改行を1つ出力。
///
/// 注意: coutへ出力。通常はflushせずAC_INTERACTIVE時はflush。入力は変更しません。
///
/// 呼び分け: print(r,sep,nl)へ委譲。改行なしはpr(r,sep)が明確です。
///
/// 例: `println(vi{1,2},string_view("::"));`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool println(const R& r, string_view sep, bool nl = true) {
    return ::print(r, sep, nl);
}

// @acf fn print — (const vector<vector<T>>& a, char sep=' ') -> bool — 2次元 vector を行ごとに出力

/// 値や配列を標準出力へ書き、通常は末尾を改行する。
///
/// 引数 `a`: 行ごとに出力する2次元vector。
///
/// 引数 `sep`: 行内の要素区切り。省略時は `' '`。
///
/// 戻り値: boolで常にtrue。出力・入力の成功判定ではありません。print()と空の1次元rangeでも末尾の改行は出力します。2次元vectorが0行なら出力は空です。
///
/// 注意: 標準出力へ書きます。入力の値は変更しません。通常はflushせず、AC_INTERACTIVE時はfinish()経由でflushします。
///
/// 呼び分け: print(a)またはprint(a,sep)。行内の区切りはcharで省略時は空白。この2次元専用形式にはnl引数はありません。0行のvectorは空出力、空の行は改行を出します。
///
/// 例: `print(vvi{{1, 2}, {3, 4}});`
template <class T>
bool print(const vector<vector<T>>& a, char sep = ' ') {
    for (const auto& row : a) ::print(row, sep, true);
    return true;
}

// @acf fn print_fixed — (int prec, const T&... args) -> bool — 固定小数点で print

/// 小数を固定小数点の精度でprintする。
///
/// 引数 `prec`: 非負の小数点以下の桁数。
///
/// 引数 `args`: printへ渡す値の列。
///
/// 戻り値: boolで常にtrue。出力形式以外はprintと同じ。戻るとcoutはdefaultfloatになるが、設定したprecisionは残ります。
///
/// 注意: coutのfixed/precisionを設定して出力後、defaultfloatへ戻す。以前のfloatfield/precision全体を保存・復元する形式ではありません。
///
/// 呼び分け: 通常の出力はprint。浮動小数以外への精度設定の効果はその型の出力に従います。
///
/// 例: `print_fixed(2,1.25);`
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

/// 候補が大きいときだけ現在値を更新する。
///
/// 引数 `a`: 更新する値。
///
/// 引数 `b`: 比較する候補。同じ型、または異なる算術型。
///
/// 戻り値: bool。比較条件を満たし代入したらtrue、同値や改善しない候補ならfalse。異なる算術型は共通型で比較し、更新時はTへ変換するため切り捨てや表現範囲に注意。
///
/// 注意: aだけを必要時に更新。候補の参照先がaでも同値なので変更しません。
///
/// 呼び分け: 同じ型はT&/const T&版。異なる算術型だけT&/U版が有効。比較成立が更新判定なので、変換後の値が同じでもtrueになることがあります。
template <class T>
bool chmax(T& a, const T& b) {
    return a < b ? (a = b, 1) : 0;
}
/// 候補が大きいときだけ現在値を更新する。
///
/// 引数 `a`: 更新する値。
///
/// 引数 `b`: 比較する候補。同じ型、または異なる算術型。
///
/// 戻り値: bool。比較条件を満たし代入したらtrue、同値や改善しない候補ならfalse。異なる算術型は共通型で比較し、更新時はTへ変換するため切り捨てや表現範囲に注意。
///
/// 注意: aだけを必要時に更新。候補の参照先がaでも同値なので変更しません。
///
/// 呼び分け: 同じ型はT&/const T&版。異なる算術型だけT&/U版が有効。比較成立が更新判定なので、変換後の値が同じでもtrueになることがあります。
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

/// 候補が小さいときだけ現在値を更新する。
///
/// 引数 `a`: 更新する値。
///
/// 引数 `b`: 比較する候補。同じ型、または異なる算術型。
///
/// 戻り値: bool。比較条件を満たし代入したらtrue、同値や改善しない候補ならfalse。異なる算術型は共通型で比較し、更新時はTへ変換するため切り捨てや表現範囲に注意。
///
/// 注意: aだけを必要時に更新。候補の参照先がaでも同値なので変更しません。
///
/// 呼び分け: 同じ型はT&/const T&版。異なる算術型だけT&/U版が有効。比較成立が更新判定なので、変換後の値が同じでもtrueになることがあります。
template <class T>
bool chmin(T& a, const T& b) {
    return a > b ? (a = b, 1) : 0;
}
/// 候補が小さいときだけ現在値を更新する。
///
/// 引数 `a`: 更新する値。
///
/// 引数 `b`: 比較する候補。同じ型、または異なる算術型。
///
/// 戻り値: bool。比較条件を満たし代入したらtrue、同値や改善しない候補ならfalse。異なる算術型は共通型で比較し、更新時はTへ変換するため切り捨てや表現範囲に注意。
///
/// 注意: aだけを必要時に更新。候補の参照先がaでも同値なので変更しません。
///
/// 呼び分け: 同じ型はT&/const T&版。異なる算術型だけT&/U版が有効。比較成立が更新判定なので、変換後の値が同じでもtrueになることがあります。
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

/// 合計の古い寄与を新しい寄与へ差し替える。
///
/// 引数 `total`: 更新する合計。
///
/// 引数 `old_value`: 差し引く寄与。
///
/// 引数 `new_value`: 加える寄与。
///
/// 戻り値: void。totalへnew_value-old_valueを加算。差と総和がTの範囲に収まること。
///
/// 注意: totalだけを更新。old_valueとnew_valueはconst参照で受け取り、差を計算してから合計へ反映します。
///
/// 呼び分け: 現在の合計を再計算せず、1要素の変更を反映したいときに使用します。
template <class T>
constexpr void delta_update(T& total, const T& old_value, const T& new_value) {
    total += new_value - old_value;
}
// @acf fn popv — (A& a) -> value_type — top/front を取り出して pop、値を返す

/// 先頭またはtopの要素を取り出し、コンテナから削除する。
///
/// 引数 `a`: 空でないstack/priority_queue/queue/dequeなど。
///
/// 戻り値: A::value_typeの値。空は未定義動作（LOCALではassert）。top+pop、front+pop、front+pop_frontの順で対応を選びます。
///
/// 注意: 選択した端の要素から戻り値を構築して、その要素を1つ削除します。stack/queue/dequeでは通常move、priority_queueではtopがconst参照のためコピーします。デフォルト構築・代入は不要です。priority_queueはコピー可能な要素型が必要です。要素操作の費用を除き、標準のstack/queue/dequeはO(1)、priority_queueはO(log N)、追加メモリはO(1)。空判定は呼び出し側で行います。
///
/// 呼び分け: vectorの末尾を取るならpopb。popv(vector)は対応する端操作がありません。
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

/// 末尾の値を取得してコンテナから削除する。
///
/// 引数 `a`: 空でないvector/dequeなど。backとpop_backが必要。
///
/// 戻り値: A::value_typeの値。空は未定義動作（LOCALではassert）。
///
/// 注意: 末尾から戻り値をmove構築してからpop_backします。要素への参照は返しません。デフォルト構築・代入は不要です。標準のvector/dequeでは時間・追加メモリO(1)（要素操作の費用を除く）。削除した要素への参照は使えなくなります。
///
/// 呼び分け: queue/stackの端操作はpopv。
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

/// 値が端点指定付き区間の内側にあるかを判定する。
///
/// 引数 `x`: 調べる値。
///
/// 引数 `lo`: 下端。
///
/// 引数 `hi`: 上端。
///
/// 引数 `InclLo, InclHi`: 各端点を含むか。省略時は `true, true`。
///
/// 戻り値: bool。既定は[lo,hi]。lo>hiでは条件を同時に満たせないためfalse。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: `between<true,false>`=半開、`<false,true>`=逆半開、`<false,false>`=開区間。EPS比較は行いません。
///
/// 例: `print(between(3,1,3),between<true,false>(3,1,3));`
template <bool InclLo = true, bool InclHi = true, class T, class U, class V>
bool between(const T& x, const U& lo, const V& hi) {
    return (InclLo ? lo <= x : lo < x) && (InclHi ? x <= hi : x < hi);
}

// @acf fn any_equal — (value, candidates...) -> bool — 第1引数が候補のいずれかと等しいか

/// 値が候補のいずれかと等しいかを判定する。
///
/// 引数 `value`: 照合する値。
///
/// 引数 `candidates...`: ==で比較できる候補。
///
/// 戻り値: bool。候補0個はfalse。最初の等しい比較でtrue。
///
/// 注意: 実引数は全て関数へ渡す前に各1回評価。比較は短絡しますが、候補式の評価は省略しません。
///
/// 呼び分け: 全て等しいかはall_equal。
///
/// 例: `print(any_equal(2,1,2,3),any_equal(2));`
template <class T, class... Rest>
constexpr bool any_equal(const T& value, const Rest&... candidates) {
    return ((value == candidates) || ...);
}

// @acf fn all_equal — (values...) -> bool — 全引数が等しいか。0個・1個は true

/// 全引数を先頭と比較し、全て等しいかを返す。
///
/// 引数 `first`: 比較の基準。引数なし版もあります。
///
/// 引数 `rest...`: 先頭と==が有効な後続の値。
///
/// 戻り値: bool。0個・1個はtrue。最初に異なる比較が出たらfalse。
///
/// 注意: 実引数は各1回、関数呼び出し前に評価されます。==は短絡します。実引数の評価順は左からとは限りません。
///
/// 呼び分け: all_equal() / all_equal(first,rest...)。ALL_EQは同じ関数のマクロ表記。
///
/// 例: `print(all_equal(2,2,2),all_equal(2,3),all_equal());`
constexpr bool all_equal() {
    return true;
}

/// 全引数を先頭と比較し、全て等しいかを返す。
///
/// 引数 `first`: 比較の基準。引数なし版もあります。
///
/// 引数 `rest...`: 先頭と==が有効な後続の値。
///
/// 戻り値: bool。0個・1個はtrue。最初に異なる比較が出たらfalse。
///
/// 注意: 実引数は各1回、関数呼び出し前に評価されます。==は短絡します。実引数の評価順は左からとは限りません。
///
/// 呼び分け: all_equal() / all_equal(first,rest...)。ALL_EQは同じ関数のマクロ表記。
///
/// 例: `print(all_equal(2,2,2),all_equal(2,3),all_equal());`
template <class First, class... Rest>
constexpr bool all_equal(const First& first, const Rest&... rest) {
    return ((first == rest) && ...);
}

// @acf fn same_parity — (values...) -> bool — 整数値の偶奇がすべて一致するか

/// 負数も含む整数の偶奇が全て一致するかを判定する。
///
/// 引数 `first`: 基準となる整数。
///
/// 引数 `rest...`: 整数の後続値。浮動小数点はコンパイル時に拒否。
///
/// 戻り値: bool。1引数はtrue。0引数形式はありません。負の奇数同士も同じ偶奇として扱います。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 1値の偶数判定はis_even、奇数判定はis_odd。
///
/// 例: `print(same_parity(-3,1,5),same_parity(2,3));`
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

/// 整数が偶数かを判定する。
///
/// 引数 `value`: 整数値。負数を含む。浮動小数点はコンパイル時に拒否。
///
/// 戻り値: bool。偶数ならtrue。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 複数値の偶奇が揃うかはsame_parity。
///
/// 例: `print(is_even(-2),is_even(-3));`
template <class T>
constexpr bool is_even(const T& value) {
    return same_parity(value, 0);
}

// @acf fn is_odd — (x) -> bool — 整数値が奇数か

/// 整数が奇数かを判定する。
///
/// 引数 `value`: 整数値。負数を含む。浮動小数点はコンパイル時に拒否。
///
/// 戻り値: bool。奇数ならtrue。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 複数値の偶奇が揃うかはsame_parity。
///
/// 例: `print(is_odd(-2),is_odd(-3));`
template <class T>
constexpr bool is_odd(const T& value) {
    return same_parity(value, 1);
}

// @acf fn sgn — (x[,eps=EPS]) -> int — 符号判定（+1 / 0 / -1）。浮動小数は誤差込み

/// 整数の符号、または誤差込みの実数の符号を返す。
///
/// 引数 `x`: 符号を調べる値。
///
/// 引数 `eps`: 浮動小数点のゼロ許容誤差。非負を渡します。省略時は `EPS`。
///
/// 戻り値: intの-1/0/1。実数はx>epsで1、x<-epsで-1、間は0。実数以外は厳密な0との大小比較。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: sgn(x) / sgn(x,eps)。整数版にはepsは影響しません。NaNはどちらの比較も偽で0になるため、有限値を想定します。
///
/// 例: `print(sgn(-2),sgn(0),sgn(3),sgn(0.01,0.1));`
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

/// 値の等価性を調べ、実数を含む場合は絶対誤差を許容する。
///
/// 引数 `a`: 第1値。
///
/// 引数 `b`: 第2値。
///
/// 引数 `eps`: 実数差に許す非負の絶対誤差。省略時は `EPS`。
///
/// 戻り値: bool。いずれかが浮動小数点ならsgn(ld(a)-ld(b),eps)==0。それ以外はa==b。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 相対誤差の判定ではありません。NaNや無限大を含む結果を有限値の等価性として扱わず、有限値で使用します。
///
/// 例: `print(eq(0.1+0.2,0.3),eq(2,3));`
template <class A, class B>
constexpr bool eq(const A& a, const B& b, ld eps = EPS) {
    if constexpr (is_floating_point_v<decay_t<A>> || is_floating_point_v<decay_t<B>>) {
        return sgn(static_cast<ld>(a) - static_cast<ld>(b), eps) == 0;
    } else {
        return a == b;
    }
}

namespace ac_detail::ascii {
/// 文字列を大文字にしたコピーを返す。
///
/// 引数 `s`: コピーする文字列。
///
/// 戻り値: str（string）。空入力は空文字列。ASCII英字だけを変換し、数字や他のバイトは保持します。
///
/// 注意: 元の文字列を変更せず、長さ分の文字列を作ります。時間・追加メモリO(N)。
///
/// 呼び分け: Unicodeの大小文字処理ではありません。
constexpr char to_upper(char c) {
    return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c;
}

/// 文字列を小文字にしたコピーを返す。
///
/// 引数 `s`: コピーする文字列。
///
/// 戻り値: str（string）。空入力は空文字列。ASCII英字だけを変換し、数字や他のバイトは保持します。
///
/// 注意: 元の文字列を変更せず、長さ分の文字列を作ります。時間・追加メモリO(N)。
///
/// 呼び分け: Unicodeの大小文字処理ではありません。
constexpr char to_lower(char c) {
    return 'A' <= c && c <= 'Z' ? c - 'A' + 'a' : c;
}
}  // namespace ac_detail::ascii

// @acf fn to_upper — (string_view s) -> str — ASCII 英字を大文字にした文字列のコピー

/// 文字列を大文字にしたコピーを返す。
///
/// 引数 `s`: コピーする文字列。
///
/// 戻り値: str（string）。空入力は空文字列。ASCII英字だけを変換し、数字や他のバイトは保持します。
///
/// 注意: 元の文字列を変更せず、長さ分の文字列を作ります。時間・追加メモリO(N)。
///
/// 呼び分け: Unicodeの大小文字処理ではありません。
inline str to_upper(string_view s) {
    str result(s);
    for (char& c : result) c = ac_detail::ascii::to_upper(c);
    return result;
}

// @acf fn to_lower — (string_view s) -> str — ASCII 英字を小文字にした文字列のコピー

/// 文字列を小文字にしたコピーを返す。
///
/// 引数 `s`: コピーする文字列。
///
/// 戻り値: str（string）。空入力は空文字列。ASCII英字だけを変換し、数字や他のバイトは保持します。
///
/// 注意: 元の文字列を変更せず、長さ分の文字列を作ります。時間・追加メモリO(N)。
///
/// 呼び分け: Unicodeの大小文字処理ではありません。
inline str to_lower(string_view s) {
    str result(s);
    for (char& c : result) c = ac_detail::ascii::to_lower(c);
    return result;
}

// @acf fn ctoi — (char c) -> int — 10進数字の文字を整数へ変換

/// ASCIIの数字1文字を0..9の整数へ変換する。
///
/// 引数 `c`: '0'..'9'の1文字。
///
/// 戻り値: intで0..9。範囲外はinvalid_argument。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 複数桁の文字列を解析するAPIではありません。逆変換はitoc。
///
/// 例: `print(ctoi('0'),ctoi('9'));`
constexpr int ctoi(char c) {
    if (c < '0' || c > '9') throw invalid_argument("ctoi: c must be an ASCII digit");
    return c - '0';
}

// @acf fn itoc — (int value) -> char — 0〜9を10進数字の文字へ変換

/// 0..9の整数をASCIIの数字1文字へ変換する。
///
/// 引数 `value`: 0..9の値。
///
/// 戻り値: charで'0'..'9'。範囲外はinvalid_argument。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 整数全体を文字列にするAPIではありません。逆変換はctoi。
///
/// 例: `print(itoc(0),itoc(9));`
constexpr char itoc(int value) {
    if (value < 0 || value > 9)
        throw invalid_argument("itoc: value must be in [0, 9]");
    return static_cast<char>('0' + value);
}

// @acf fn is_chars — (string_view s, Pred pred) -> bool — 各文字が判定関数を満たす文字列か

/// 空でない文字列の全ての文字が条件を満たすかを判定する。
///
/// 引数 `s`: 調べる文字列。
///
/// 引数 `pred`: unsigned charとして各文字を受け取る判定関数。
///
/// 戻り値: bool。空文字はfalse、最初の偽でfalse。全て真ならtrue。
///
/// 注意: 入力を変更せず走査します。predの副作用は利用者側の責任です。
///
/// 呼び分け: callable版と`<cctype>`の関数ポインタ版。all_ofの空入力=trueとは異なります。
///
/// 例: `print(is_chars("ABC",[](unsigned char c){return c>='A' && c<='Z';}),is_chars("",isdigit));`
template <class Pred>
inline bool is_chars(string_view s, Pred pred) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!pred(c)) return false;
    }
    return true;
}

// <cctype> の overload set（isdigit / isxdigit など）をそのまま受ける overload
/// 空でない文字列の全ての文字が条件を満たすかを判定する。
///
/// 引数 `s`: 調べる文字列。
///
/// 引数 `pred`: unsigned charとして各文字を受け取る判定関数。
///
/// 戻り値: bool。空文字はfalse、最初の偽でfalse。全て真ならtrue。
///
/// 注意: 入力を変更せず走査します。predの副作用は利用者側の責任です。
///
/// 呼び分け: callable版と`<cctype>`の関数ポインタ版。all_ofの空入力=trueとは異なります。
///
/// 例: `print(is_chars("ABC",[](unsigned char c){return c>='A' && c<='Z';}),is_chars("",isdigit));`
inline bool is_chars(string_view s, int (*pred)(int)) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!pred(c)) return false;
    }
    return true;
}

// @acf fn is_digits — (string_view s) -> bool — 10進数字だけからなる文字列か

/// 空でない文字列が全て10進数字かを判定する。
///
/// 引数 `s`: 調べる文字列。
///
/// 戻り値: bool。空はfalse。符号・空白・小数点は受け付けません。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 文字の分類を行うだけで、数値へ変換しません。is_charsをisdigitで呼ぶ短縮です。
///
/// 例: `print(is_digits("012"),is_digits("-1"),is_digits(""));`
inline bool is_digits(string_view s) {
    return is_chars(s, isdigit);
}

// @acf fn is_hex_digits — (string_view s) -> bool — 16進数字だけからなる文字列か

/// 空でない文字列が全て16進数字かを判定する。
///
/// 引数 `s`: 調べる文字列。
///
/// 戻り値: bool。空はfalse。符号・空白・0x接頭辞は受け付けません。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 文字の分類を行うだけで、数値へ変換しません。is_charsをisxdigitで呼ぶ短縮です。
///
/// 例: `print(is_hex_digits("aF09"),is_hex_digits("0x10"),is_hex_digits(""));`
inline bool is_hex_digits(string_view s) {
    return is_chars(s, isxdigit);
}

// @acf fn reversed — (string_view s) -> str — 文字列を反転したコピー

/// 文字列を逆順にしたコピーを返す。
///
/// 引数 `s`: コピーする文字列。
///
/// 戻り値: str（string）。空入力は空文字列。バイト単位で反転するためUTF-8の文字反転には使いません。
///
/// 注意: 元の文字列を変更せず、長さ分の文字列を作ります。時間・追加メモリO(N)。
///
/// 呼び分け: 反転した文字列を必要とするときに使用します。
inline str reversed(string_view s) {
    str result(s);
    ranges::reverse(result);
    return result;
}

// @acf fn eq_ignore_case — (string_view a, string_view b) -> bool — ASCII 英字の大小を無視した文字列比較

/// ASCII英字の大小を無視して文字列を比較する。
///
/// 引数 `a`: 第1文字列。
///
/// 引数 `b`: 第2文字列。
///
/// 戻り値: bool。長さが違えばfalse。英字以外はそのまま比較。空同士はtrue。
///
/// 注意: 文字列を変更せず、追加文字列も作りません。時間O(N)、追加メモリO(1)。
///
/// 呼び分け: Unicodeのcase foldingは行いません。
///
/// 例: `print(eq_ignore_case("AbC1","aBc1"),eq_ignore_case("ab","abc"));`
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

/// 数の文字列表記を別の基数へ変換する。
///
/// 引数 `s`: 非空の桁列。先頭の+/-は可、符号だけは不可。
///
/// 引数 `from_base / to_base`: 通常版の入力/出力基数。ともに2..36。
///
/// 引数 `digit_map（2引数版）`: 独自の文字→桁値。値は0または1から連続し基数>=2。内部int計算に収まる小さい基数を使う。
///
/// 戻り値: str。通常は0..9,A..Zの大文字表記、先頭0と+を除去、負の0は"0"。独自マップ版は0または1から始まる連続した桁値から基数を推定して10進へ変換。不正基数・桁・マップはinvalid_argument。
///
/// 注意: 入力を変更せず繰り返し割り算。桁数LでO(L^2)、追加メモリO(L)。独自基数は、途中のremainder*base+digitがintに収まること。
///
/// 呼び分け: base_convert(s,from_base,to_base) / base_convert(s,digit_map)。通常版は英字の大小を受け入れます。
///
/// 例: `print(base_convert("ff",16,2),base_convert("BA",map<char,int>{{'A',1},{'B',2}}));`
inline str base_convert(string_view s, int from_base, int to_base) {
    if (from_base < 2 || from_base > 36)
        throw invalid_argument("base_convert: from_base must be in [2, 36]");
    if (to_base < 2 || to_base > 36)
        throw invalid_argument("base_convert: to_base must be in [2, 36]");
    return ac_detail::radix::convert(s, from_base, to_base);
}

/// 数の文字列表記を別の基数へ変換する。
///
/// 引数 `s`: 非空の桁列。先頭の+/-は可、符号だけは不可。
///
/// 引数 `from_base / to_base`: 通常版の入力/出力基数。ともに2..36。
///
/// 引数 `digit_map（2引数版）`: 独自の文字→桁値。値は0または1から連続し基数>=2。内部int計算に収まる小さい基数を使う。
///
/// 戻り値: str。通常は0..9,A..Zの大文字表記、先頭0と+を除去、負の0は"0"。独自マップ版は0または1から始まる連続した桁値から基数を推定して10進へ変換。不正基数・桁・マップはinvalid_argument。
///
/// 注意: 入力を変更せず繰り返し割り算。桁数LでO(L^2)、追加メモリO(L)。独自基数は、途中のremainder*base+digitがintに収まること。
///
/// 呼び分け: base_convert(s,from_base,to_base) / base_convert(s,digit_map)。通常版は英字の大小を受け入れます。
///
/// 例: `print(base_convert("ff",16,2),base_convert("BA",map<char,int>{{'A',1},{'B',2}}));`
inline str base_convert(string_view s, const map<char, int>& digit_map) {
    const int from_base = ac_detail::radix::mapped_base(digit_map);
    return ac_detail::radix::convert(s, from_base, 10, &digit_map);
}

// @acf fn fill_range — (range&, l, r, value) -> void — 半開区間 [l,r) を同じ値で埋める

/// 配列の半開区間を同じ値で埋める。
///
/// 引数 `values`: 書き込み可能な、サイズ付きrandom-access range。
///
/// 引数 `l / r`: 0<=l<=r<=sizeの半開区間。
///
/// 引数 `value`: 代入する値。
///
/// 戻り値: void。l==rなら変更なし。
///
/// 注意: 選んだ要素だけを更新。時間O(r-l)、追加メモリO(1)。LOCALでは範囲をassert、通常ビルドでは呼び出し側で保証。
///
/// 呼び分け: fill_rangeは上の引数順で呼び出します。
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

/// 半開矩形の座標を行優先で走査する。
///
/// 引数 `r1 / r2`: 行の半開区間。
///
/// 引数 `c1 / c2`: 列の半開区間。
///
/// 引数 `f`: f(row,col)として呼ぶcallable。
///
/// 戻り値: void。r1>=r2またはc1>=c2なら呼び出し0回。
///
/// 注意: 各座標でcallbackを実行。時間O(面積)（callback費用は別）、追加メモリO(1)。盤面を受け取らず座標範囲は検査しません。
///
/// 呼び分け: 行端を2つ渡した後に列端を2つ渡します。fill_rectとは端点の引数順が異なります。
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

/// グリッドの半開矩形を同じ値で埋める。
///
/// 引数 `grid`: 行と列がサイズ付きrandom-accessで書き込み可能なグリッド。
///
/// 引数 `r1 / c1 / r2 / c2`: 行[r1,r2)、列[c1,c2)。非負で選んだ全行の列範囲内。
///
/// 引数 `value`: 代入する値。
///
/// 戻り値: void。幅または高さ0なら要素の変更なし。
///
/// 注意: 選んだ要素を更新。時間O(高さ+面積)、追加メモリO(1)。LOCALは範囲をassert、通常ビルドでは保証が必要。
///
/// 呼び分け: 左上(r1,c1)、右下の除外端(r2,c2)の順。既定構築は高さ・幅0。空矩形の追加は何も変更しません。for_rect/any_rectなどと引数順が異なります。
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

/// 半開矩形内で条件を満たすセルがあるか判定する。
///
/// 引数 `grid`: サイズ付きrandom-accessの行と列。
///
/// 引数 `r1 / r2`: 有効な行の半開区間。
///
/// 引数 `c1 / c2`: 選んだ全行で有効な列の半開区間。
///
/// 引数 `pred`: セル値を受け取り条件を判定するcallable。
///
/// 戻り値: bool。空矩形はfalse。
///
/// 注意: 要素を行優先で読み入力を変更しません。最初の真で終了。時間O(面積+高さ)、追加メモリO(1)、callbackの副作用は別。LOCALは範囲をassert。
///
/// 呼び分け: 行端r1,r2の後に列端c1,c2。fill_rectとは引数順が異なります。
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

/// 半開矩形内で全セルが条件を満たすか判定する。
///
/// 引数 `grid`: サイズ付きrandom-accessの行と列。
///
/// 引数 `r1 / r2`: 有効な行の半開区間。
///
/// 引数 `c1 / c2`: 選んだ全行で有効な列の半開区間。
///
/// 引数 `pred`: セル値を受け取り条件を判定するcallable。
///
/// 戻り値: bool。空矩形はtrue。
///
/// 注意: 要素を行優先で読み入力を変更しません。最初の偽で終了。時間O(面積+高さ)、追加メモリO(1)、callbackの副作用は別。LOCALは範囲をassert。
///
/// 呼び分け: 行端r1,r2の後に列端c1,c2。fill_rectとは引数順が異なります。
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

/// 半開矩形内で条件を満たすセルの個数を数える。
///
/// 引数 `grid`: サイズ付きrandom-accessの行と列。
///
/// 引数 `r1 / r2`: 有効な行の半開区間。
///
/// 引数 `c1 / c2`: 選んだ全行で有効な列の半開区間。
///
/// 引数 `pred`: セル値を受け取り条件を判定するcallable。
///
/// 戻り値: ll。空矩形は0。
///
/// 注意: 要素を行優先で読み入力を変更しません。全セルを調べます。時間O(面積+高さ)、追加メモリO(1)、callbackの副作用は別。LOCALは範囲をassert。
///
/// 呼び分け: 行端r1,r2の後に列端c1,c2。fill_rectとは引数順が異なります。
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

/// 行・列が高さ×幅のグリッド内か判定する。
///
/// 引数 `r / c`: 0-indexの行/列座標。
///
/// 引数 `h / w`: 非負の高さ/幅。比較が意図通り行える型を使います。
///
/// 戻り値: bool。0<=r<hかつ0<=c<w。高さ/幅0なら内側のマスはありません。
///
/// 注意: 副作用なし。時間O(1)。
///
/// 呼び分け: 引数順はrow,column,height,width。境界でクリップする関数ではありません。
///
/// 例: `print(in_grid(1,2,2,3),in_grid(2,0,2,3));`
template <class R, class C, class H, class W>
constexpr bool in_grid(R r, C c, H h, W w) {
    return 0 <= r && r < h && 0 <= c && c < w;
}

// @acf fn out_grid — (r,c,h,w) -> bool — (r,c) が h×w グリッド外か判定

/// 行・列が高さ×幅のグリッド外か判定する。
///
/// 引数 `r / c`: 0-indexの行/列座標。
///
/// 引数 `h / w`: 非負の高さ/幅。比較が意図通り行える型を使います。
///
/// 戻り値: bool。in_gridの否定。高さ/幅0なら内側のマスはありません。
///
/// 注意: 副作用なし。時間O(1)。
///
/// 呼び分け: 引数順はrow,column,height,width。境界でクリップする関数ではありません。
///
/// 例: `print(out_grid(1,2,2,3),out_grid(2,0,2,3));`
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

/// 条件を満たすマスの最小外接矩形を求める。
///
/// 引数 `grid`: 行を走査できるグリッド。不揃いな行も可、各サイズはintに収まること。
///
/// 引数 `pred`: セル値を判定するcallable。
///
/// 戻り値: `optional<GridBounds>`。見つかればtop,left,bottom,rightの0-index端点（両端を含む）。該当なし/空はnullopt。
///
/// 注意: 入力を変更せず各セルを走査。時間O(行数+セル数)、追加メモリO(1)。callback副作用は別。
///
/// 呼び分け: 返す矩形は両端を含みます。fill_rectの除外端へ渡すならbottom/rightに1を足します。
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

/// 2次元配列を時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90(grid)) / (rotate90(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90(grid)) / (rotate90(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 文字グリッドを時計回り90度したコピーを返す。
///
/// 引数 `grid`: 文字行の配列。fillなしは長方形。
///
/// 引数 `fill（追加形式）`: 欠けたマスを埋める文字。
///
/// 戻り値: `vector<string>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90(grid)) / (rotate90(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90(grid)) / (rotate90(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を反時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90_left(grid)) / (rotate90_left(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を反時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90_left(grid)) / (rotate90_left(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 文字グリッドを反時計回り90度したコピーを返す。
///
/// 引数 `grid`: 文字行の配列。fillなしは長方形。
///
/// 引数 `fill（追加形式）`: 欠けたマスを埋める文字。
///
/// 戻り値: `vector<string>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90_left(grid)) / (rotate90_left(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を反時計回り90度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate90_left(grid)) / (rotate90_left(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
inline vector<string> rotate90_left(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width, string(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[width - 1 - c][r] = grid[r][c];
    return result;
}

// @acf fn rotate180 — (const vector<vector<T>>& grid[, const T& fill]) -> vector<vector<T>> — グリッドを180度回転

/// 2次元配列を180度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate180(grid)) / (rotate180(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を180度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate180(grid)) / (rotate180(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 文字グリッドを180度したコピーを返す。
///
/// 引数 `grid`: 文字行の配列。fillなしは長方形。
///
/// 引数 `fill（追加形式）`: 欠けたマスを埋める文字。
///
/// 戻り値: `vector<string>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate180(grid)) / (rotate180(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を180度したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。fillなし版は長方形が前提。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。90度は幅×高さ、180度は高さ×幅。fillなしで入力/先頭行が空なら空の外側配列。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (rotate180(grid)) / (rotate180(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を転置したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。行長は不揃いも可。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。fillなしは存在する要素だけを各列へ集めるため、行長が不揃いでも可。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (transpose(grid)) / (transpose(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 2次元配列を転置したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。行長は不揃いも可。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。fillなしは存在する要素だけを各列へ集めるため、行長が不揃いでも可。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (transpose(grid)) / (transpose(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
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

/// 文字グリッドを転置したコピーを返す。
///
/// 引数 `grid`: 文字行の配列。不揃いな行も可。
///
/// 引数 `fill（追加形式）`: 欠けたマスを埋める文字。
///
/// 戻り値: `vector<string>`の新しい配列。fillなしは存在する要素だけを各列へ集めるため、行長が不揃いでも可。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (transpose(grid)) / (transpose(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
inline vector<string> transpose(const vector<string>& grid) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width);
    for (auto& column : result) column.reserve(grid.size());
    for (const auto& row : grid)
        for (size_t c = 0; c < row.size(); ++c) result[c].push_back(row[c]);
    return result;
}

/// 2次元配列を転置したコピーを返す。
///
/// 引数 `grid`: 入力の2次元配列。行長は不揃いも可。
///
/// 引数 `fill（追加形式）`: 不揃いな行の欠けたセルを埋める値。
///
/// 戻り値: `vector<vector<T>>`の新しい配列。fillなしは存在する要素だけを各列へ集めるため、行長が不揃いでも可。fillありは最大行長まで埋めて操作します。
///
/// 注意: 元の配列は変更しません。回転/fill版は時間O(H+H*最大幅)、出力メモリO(H*最大幅)。転置fillなしは存在するセルのみコピーしますが各列にH個の容量を予約します。
///
/// 呼び分け: (transpose(grid)) / (transpose(grid,fill))。回転fillなしの長方形条件はLOCALでassert、通常は呼び出し側が保証。
inline vector<string> transpose(const vector<string>& grid, char fill) {
    size_t width = 0;
    for (const auto& row : grid) width = std::max(width, row.size());
    vector<string> result(width, string(grid.size(), fill));
    for (size_t r = 0; r < grid.size(); ++r)
        for (size_t c = 0; c < grid[r].size(); ++c) result[c][r] = grid[r][c];
    return result;
}

// @acf fn sort_unique — (vector<T>& v) -> bool — ソートして重複を削除。常に true

/// 配列をソートし、隣接する等しい要素を取り除く。
///
/// 引数 `v`: 並べ替え可能で、operator==で重複を比較できる配列。1引数版ではoperator<も必要です。
///
/// 引数 `comp`: 厳密弱順序を満たす比較器。comp(a,b)がtrueならaをbより前へ並べます。省略時は従来どおりoperator<の昇順です。
///
/// 戻り値: 常にtrue。変更がなくてもtrueなので変更検出には使いません。空は空のまま。
///
/// 注意: vをその場でソートし、隣接するoperator==の重複をuniqueとeraseで取り除きます。compは並び順だけに使い、比較器同値だけでは要素を消しません。ソートは安定ではなく、比較器同値の要素の順序は不定です。部分キー比較などで同じ値が離れて並べば、その重複は残ります。比較回数O(N log N)、重複の比較はO(N)。要素の移動・比較の費用は別です。元の順序を変更し、erase位置以降の参照・イテレータは失効します。
///
/// 呼び分け: sort_unique(v) / sort_unique(v, comp)。greater{}なら降順。pairのfirstだけを比較しても{1,10}と{1,20}はoperator==で異なるため両方残します。完全一致の重複をまとめたい場合は、pair全体を比較するなど同じ値が連続する順序を指定します。元の配列を保持するならコピーしてから呼び出します。
template <class T>
bool sort_unique(vector<T>& v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return true;
}

/// 配列をソートし、隣接する等しい要素を取り除く。
///
/// 引数 `v`: 並べ替え可能で、operator==で重複を比較できる配列。1引数版ではoperator<も必要です。
///
/// 引数 `comp`: 厳密弱順序を満たす比較器。comp(a,b)がtrueならaをbより前へ並べます。省略時は従来どおりoperator<の昇順です。
///
/// 戻り値: 常にtrue。変更がなくてもtrueなので変更検出には使いません。空は空のまま。
///
/// 注意: vをその場でソートし、隣接するoperator==の重複をuniqueとeraseで取り除きます。compは並び順だけに使い、比較器同値だけでは要素を消しません。ソートは安定ではなく、比較器同値の要素の順序は不定です。部分キー比較などで同じ値が離れて並べば、その重複は残ります。比較回数O(N log N)、重複の比較はO(N)。要素の移動・比較の費用は別です。元の順序を変更し、erase位置以降の参照・イテレータは失効します。
///
/// 呼び分け: sort_unique(v) / sort_unique(v, comp)。greater{}なら降順。pairのfirstだけを比較しても{1,10}と{1,20}はoperator==で異なるため両方残します。完全一致の重複をまとめたい場合は、pair全体を比較するなど同じ値が連続する順序を指定します。元の配列を保持するならコピーしてから呼び出します。
template <class T, class Comp>
bool sort_unique(vector<T>& v, Comp comp) {
    sort(v.begin(), v.end(), std::move(comp));
    v.erase(unique(v.begin(), v.end()), v.end());
    return true;
}

// @acf fn recurrence — (T x, int n, Next next) -> vector<T> — 漸化式でちょうど n 項を生成

/// 初期値から漸化式で指定個数の項を生成する。
///
/// 引数 `x`: 最初の項。
///
/// 引数 `n`: 0以上の項数。
///
/// 引数 `next`: Tの項を受け取り次の項を返すcallable。
///
/// 戻り値: `vector<T>`。ちょうどn項。n=0なら空、n<0はassert対象。nextはn-1回（n=0は0回）。
///
/// 注意: xのコピーを更新し、各項を配列へ保存。nextの副作用やオーバーフロー、項数分の確保は利用者側の責任。
///
/// 呼び分け: recurrence(x,n,next) / recurrence(x,next,pred)。項数指定版と継続条件版は引数順が異なります。
///
/// 例: `print(recurrence(1,4,[](int x){return x*2;}));`
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

/// 継続条件が真のあいだ漸化式の項を生成する。
///
/// 引数 `x`: 最初の項。
///
/// 引数 `next`: 次の項を作るcallable。
///
/// 引数 `pred`: 現在の項を採用するbool条件。
///
/// 戻り値: `vector<T>`。最初からpredが偽なら空。条件が停止することを利用者が保証します。
///
/// 注意: 各項でpredを先に評価し、採用したら保存してnextを1回評価。最後の採用項にもnextを適用。LOCALでは項数10^7未満を検査します。
///
/// 呼び分け: recurrence(x,n,next) / recurrence(x,next,pred)。項数指定版と継続条件版は引数順が異なります。
///
/// 例: `print(recurrence(1,[](int x){return x*2;},[](int x){return x<10;}));`
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

/// 配列から、先頭0付きの累積和配列を作る。
///
/// 引数 `a`: 足し算できる要素、または成分を足せるtuple-likeな要素。結果も同じTなので総和が収まる型を使う。
///
/// 戻り値: `vector<T>`。長さN+1でsum[0]=T{}、sum[i]はa[0..i)の和。空入力は0だけの長さ1。pair/tuple等は成分ごとの和。
///
/// 注意: 元配列は変更しません。更新後に結果が自動更新されることもありません。
///
/// 呼び分け: prefix_sum(a)は1次元vector。長方形・直方体の領域集計はprefix_sum2d/prefix_sum3d。区間抽出はrange_sum。
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

/// 配列から、先頭0付きの累積和配列を作る。
///
/// 引数 `a`: 足し算できる要素、または成分を足せるtuple-likeな要素。結果も同じTなので総和が収まる型を使う。
///
/// 戻り値: `vector<T>`。長さN+1でsum[0]=T{}、sum[i]はa[0..i)の和。空入力は0だけの長さ1。pair/tuple等は成分ごとの和。
///
/// 注意: 元配列は変更しません。更新後に結果が自動更新されることもありません。
///
/// 呼び分け: prefix_sum(a)は1次元vector。長方形・直方体の領域集計はprefix_sum2d/prefix_sum3d。区間抽出はrange_sum。
template <class T>
vector<T> prefix_sum(const vector<T>& a) {
    vector<T> sum(a.size() + 1, T{});
    for (size_t i = 0; i < a.size(); ++i) {
        sum[i + 1] = ac_detail::prefix_sum::add(sum[i], a[i]);
    }
    return sum;
}

// @acf fn prefix_sum2d — (const vector<vector<T>>& a) -> vector<vector<T>> — 2次元累積和。返り値は (H+1)×(W+1)

/// 2次元配列の各左上矩形の累積和を作る。
///
/// 引数 `a`: 長方形のH×W配列。T{}が加算単位元で、途中の和がTで表せること。
///
/// 戻り値: `vector<vector<T>>`の(H+1)×(W+1)。0行/0列は全て0。半開矩形の和はs[r2][c2]-s[r1][c2]-s[r2][c1]+s[r1][c1]。外側が空なら1×1の0。
///
/// 注意: 入力を変更せず累積和を確保。時間/メモリO((H+1)(W+1))。LOCALは長方形をassert。
///
/// 呼び分け: 添字は累積和の番兵を含む0始まり、矩形の端は除外端です。
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

/// depth,row,column順の3次元配列の累積和を作る。
///
/// 引数 `a`: 各層・行の形が揃うD×H×W配列。T{}が加算単位元で途中の和がTで表せること。
///
/// 戻り値: `vector<vector<vector<T>>>`の(D+1)×(H+1)×(W+1)。先頭面は0。半開直方体の和は8頂点の包除。空外側なら1×1×1の0。
///
/// 注意: 入力を変更せず3軸で累積。時間/メモリO((D+1)(H+1)(W+1))。LOCALは形の整合性をassert。
///
/// 呼び分け: 軸順はdepth,row,column。
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

/// ネストした列の葉を変換し、全体を足し合わせる。
///
/// 引数 `value`: ネストしたrange、または葉の値。
///
/// 引数 `projection`: constの葉を受け取り、集計可能な型を返すcallable。
///
/// 戻り値: projectionの戻り値から参照・cvを外した型。空rangeはResult{}。型を自動で広げないので結果が型に収まること。
///
/// 注意: 入力を変更せず、projectionを各葉へ1回適用。projectionの副作用は利用者側の責任。時間O(葉数)、再帰の深さは次元数。
///
/// 呼び分け: sum_by(value,projection) / sum_by(value)。省略版はidentityで葉をそのまま集計します。
template <class Range, class Projection>
auto sum_by(const Range& value, Projection projection) {
    return ac_detail::nested_sum::sum_by_impl(value, projection);
}
/// ネストした列の葉を変換し、全体を足し合わせる。
///
/// 引数 `value`: ネストしたrange、または葉の値。
///
/// 引数 `projection`: constの葉を受け取り、集計可能な型を返すcallable。
///
/// 戻り値: projectionの戻り値から参照・cvを外した型。空rangeはResult{}。型を自動で広げないので結果が型に収まること。
///
/// 注意: 入力を変更せず、projectionを各葉へ1回適用。projectionの副作用は利用者側の責任。時間O(葉数)、再帰の深さは次元数。
///
/// 呼び分け: sum_by(value,projection) / sum_by(value)。省略版はidentityで葉をそのまま集計します。
template <class Range>
auto sum_by(const Range& value) {
    std::identity projection;
    return ac_detail::nested_sum::sum_by_impl(value, projection);
}

// @acf fn sum_as — <T>(const nested range& value) -> T — 指定した型で総和を計算

/// 各葉を指定した型へ変換してから総和を求めます。
///
/// 引数 `T`: 集計する値型。T{}とT同士の加算代入が必要です。各葉を`static_cast<T>`で変換できること。
///
/// 引数 `value`: constで走査できるネストしたrange、または葉の値。sum_byと同じ範囲を受け取ります。
///
/// 戻り値: Tを返します。空rangeはT{}。加算前に各葉をTへ変換するため、intの列もllで集計できます。変換結果・途中の和・最終結果がTに収まることが必要です。
///
/// 注意: 入力を変更・コピーせず、中間配列を作りません。変換と加算のコストを除き、訪問する要素と部分rangeの総数Nに対して時間O(N)、再帰用の追加メモリO(次元数)です。1次元では時間O(要素数)、追加メモリO(1)です。
///
/// 呼び分け: `sum_as<T>(value)`。葉の型のまま足すならsum_all、式の値を足すならsum_byまたはSUMを使います。
template <class T, class Range>
T sum_as(const Range& value) {
    return sum_by(value, [](const auto& leaf) -> T { return static_cast<T>(leaf); });
}

// @acf fn average — (const range& a) -> long double — 平均

/// 1次元の数値列の平均を返す。空でないこと。計算量 O(N)
/// 呼び出し: `average(a) -> long double`
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

/// 平均からの偏差の二乗の平均を返す。空でなく、繰り返し走査できること。計算量 O(N)
/// 呼び出し: `variance(a) -> long double`
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

/// 分散の平方根を返す。空でなく、繰り返し走査できること。計算量 O(N)
/// 呼び出し: `stddev(a) -> long double`
template <class Range>
long double stddev(const Range& a) {
    return sqrt(variance(a));
}

// @acf fn product_by — (const nested range& a, Projection projection) -> scalar / (const nested range& a) -> scalar — 葉を変換して全体を乗算

/// ネストした列の葉を変換し、全体を掛け合わせる。
///
/// 引数 `value`: ネストしたrange、または葉の値。
///
/// 引数 `projection`: constの葉を受け取り、集計可能な型を返すcallable。
///
/// 戻り値: projectionの戻り値から参照・cvを外した型。空rangeはResult(1)。型を自動で広げないので結果が型に収まること。
///
/// 注意: 入力を変更せず、projectionを各葉へ1回適用。projectionの副作用は利用者側の責任。時間O(葉数)、再帰の深さは次元数。
///
/// 呼び分け: product_by(value,projection) / product_by(value)。省略版はidentityで葉をそのまま集計します。
template <class Range, class Projection>
auto product_by(const Range& value, Projection projection) {
    return ac_detail::nested_sum::product_by_impl(value, projection);
}
/// ネストした列の葉を変換し、全体を掛け合わせる。
///
/// 引数 `value`: ネストしたrange、または葉の値。
///
/// 引数 `projection`: constの葉を受け取り、集計可能な型を返すcallable。
///
/// 戻り値: projectionの戻り値から参照・cvを外した型。空rangeはResult(1)。型を自動で広げないので結果が型に収まること。
///
/// 注意: 入力を変更せず、projectionを各葉へ1回適用。projectionの副作用は利用者側の責任。時間O(葉数)、再帰の深さは次元数。
///
/// 呼び分け: product_by(value,projection) / product_by(value)。省略版はidentityで葉をそのまま集計します。
template <class Range>
auto product_by(const Range& value) {
    std::identity projection;
    return ac_detail::nested_sum::product_by_impl(value, projection);
}

// @acf fn sum_all — (const nested range& a) -> scalar — N次元配列の全要素の総和

/// ネストした配列の全ての葉を足し合わせる。
///
/// 引数 `value`: 同じ葉の型を持つネストしたrange、または数値などの葉。
///
/// 戻り値: 葉の型。空のrangeは葉の型のT{}。整数を自動でllへ広げないので総和が型に収まること。
///
/// 注意: 入力を変更せず、全ての葉を1回足します。時間O(葉数)、再帰の深さはネストの次元数。
///
/// 呼び分け: sum_all(value)はsum_by(value)と同じ。llで集計するなら`sum_as<ll>(value)`を使います。
template <class T>
ac_detail::nested_sum::leaf_type_t<T> sum_all(const T& value) {
    return sum_by(value);
}

// @acf fn product_all — (const nested range& a) -> scalar — N次元配列の全要素の積

/// ネストした列の全ての葉を掛け合わせる。
///
/// 引数 `value`: ネストしたrange、または乗算可能な葉。
///
/// 戻り値: 葉の型。空rangeは1。葉単体ならその値。積が葉の型に収まること。
///
/// 注意: 入力を変更せず全ての葉を1回掛けます。時間O(葉数)。
///
/// 呼び分け: product_all(value)はproduct_by(value)と同じ。変換や型拡張はproduct_by(value,projection)。
///
/// 例: `print(product_all(vvi{{2,3},{4}}),product_all(vi{}));`
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

/// ネストした列で一致する葉の個数を数える。
///
/// 引数 `input`: ネストしたrangeや文字列。
///
/// 引数 `target`: ==で照合する値。
///
/// 戻り値: llを返します。空rangeでは0です。文字列へcharを渡すと文字ごとに比較し、文字列を渡すと文字列全体を比較します。
///
/// 注意: 入力を変更せず再帰走査。時間O(葉数)（文字列比較の費用は別）。
///
/// 呼び分け: 1次元rangeへprojectionを適用して数えるならcount。
///
/// 例: `print(count_all(vvi{{1,2},{1}},1),count_all(string("aba"),'a'));`
template <class Range, class Value>
ll count_all(const Range& input, const Value& target) {
    return ac_detail::nested_count::count_impl(input, target);
}

// @acf fn axis_sum — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の和

/// 指定した軸に沿って対応する要素を合計し、その軸を除いた結果を返す。
///
/// 引数 `Axis`: 外側を0とする軸。0..次元数-1。
///
/// 引数 `input`: 各階層の長さが一定の長方形のネストしたrange。
///
/// 戻り値: 軸を1つ減らしたnested vector（1次元入力なら葉の型）。空の縮約は葉の型の0、または空のrange。葉の型を自動で広げません。
///
/// 注意: 入力を変更せず時間O(葉数)。LOCALでは長方形を検査します。不揃いな形はサポート契約外。
///
/// 呼び分け: 2次元のAxis=0は列方向、Axis=1は行方向。軸は実行時引数ではなくテンプレート引数です。
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_sum(const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_sum_impl<Axis>(input);
}

// @acf fn axis_min — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の最小値

/// 指定した軸に沿って対応する要素の最小値を求め、その軸を除いた結果を返す。
///
/// 引数 `Axis`: 外側を0とする軸。0..次元数-1。
///
/// 引数 `input`: 各階層の長さが一定の長方形のネストしたrange。
///
/// 戻り値: 軸を1つ減らしたnested vector（1次元入力なら葉の型）。空の縮約で結果が葉ならinvalid_argument、rangeなら空結果。葉の型を自動で広げません。
///
/// 注意: 入力を変更せず時間O(葉数)。LOCALでは長方形を検査します。縮約対象は通常空でないことを想定。
///
/// 呼び分け: 2次元のAxis=0は列方向、Axis=1は行方向。軸は実行時引数ではなくテンプレート引数です。
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_min(
    const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_extreme_impl<Axis>(input, less<>{});
}

// @acf fn axis_max — (Axis, const nested range& a) -> nested vector — 指定軸を潰したN次元配列の最大値

/// 指定した軸に沿って対応する要素の最大値を求め、その軸を除いた結果を返す。
///
/// 引数 `Axis`: 外側を0とする軸。0..次元数-1。
///
/// 引数 `input`: 各階層の長さが一定の長方形のネストしたrange。
///
/// 戻り値: 軸を1つ減らしたnested vector（1次元入力なら葉の型）。空の縮約で結果が葉ならinvalid_argument、rangeなら空結果。葉の型を自動で広げません。
///
/// 注意: 入力を変更せず時間O(葉数)。LOCALでは長方形を検査します。縮約対象は通常空でないことを想定。
///
/// 呼び分け: 2次元のAxis=0は列方向、Axis=1は行方向。軸は実行時引数ではなくテンプレート引数です。
template <size_t Axis, class Range>
ac_detail::nested_sum::axis_result_t<Axis, Range> axis_max(
    const Range& input) {
#ifdef LOCAL
    assert(ac_detail::nested_sum::rectangular(input));
#endif
    return ac_detail::nested_sum::axis_extreme_impl<Axis>(input, greater<>{});
}

// @acf fn row_sums — (const 2D range& a) -> vector — 2次元配列の行ごとの和

/// 2次元配列の各行の総和を求める。
///
/// 引数 `grid`: 長方形の2次元range。
///
/// 戻り値: 葉の型のvector。行数分の値。0列なら各行の和は0。加算結果が葉の型に収まること。
///
/// 注意: 入力を変更せず時間O(HW)。LOCALでは長方形を検査。
///
/// 呼び分け: row_sums(grid)はaxis_sum<1>(grid)の別名。
template <class Grid>
auto row_sums(const Grid& grid) -> decltype(axis_sum<1>(grid)) {
    return axis_sum<1>(grid);
}

// @acf fn col_sums — (const 2D range& a) -> vector — 2次元配列の列ごとの和

/// 2次元配列の各列の総和を求める。
///
/// 引数 `grid`: 長方形の2次元range。
///
/// 戻り値: 葉の型のvector。列数分の値。0行なら空vector。加算結果が葉の型に収まること。
///
/// 注意: 入力を変更せず時間O(HW)。LOCALでは長方形を検査。
///
/// 呼び分け: col_sums(grid)はaxis_sum<0>(grid)の別名。
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

/// 累積和表から指定区間・領域の合計を取り出す。
///
/// 引数 `sum`: 元配列ではなく累積和の表。要素型Tに引き算が必要。
///
/// 引数 `lo, hi / 各軸の始端・終端`: 元配列上の0始まり端点。1Dはlo,hi。2Dはr1,c1,r2,c2。3Dはd1,r1,c1,d2,r2,c2。
///
/// 戻り値: Tの区間和。既定は[lo,hi)。空区間はT{}。標準pair/tupleは引き算を持たないため、そのままrange_sumへ渡さず各成分を引きます。
///
/// 注意: 入力の表を変更しません。端点を勝手にclampしません。半開区間へ変換した境界は0<=first<=last<=Nを満たすように渡します。LOCAL時は非空区間の境界をassertで確認します。
///
/// 呼び分け: range_sum(sum,l,r)は左を含み右を含まない。`range_sum<true,true>`は両端を含む。2D/3Dは2個のboolで全軸共通、4個/6個で各軸の開閉指定。
template <bool InclLo = true, bool InclHi = false, class T, class L, class R>
T range_sum(const vector<T>& sum, L lo, R hi) {
    const auto [first, last] =
        ac_detail::prefix_sum::half_open<InclLo, InclHi>(lo, hi);
    if (first >= last) return T{};
    ac_detail::prefix_sum::assert_bound(sum, first, last);
    return sum[static_cast<size_t>(last)] - sum[static_cast<size_t>(first)];
}

/// 累積和表から指定区間・領域の合計を取り出す。
///
/// 引数 `sum`: 元配列ではなく累積和の表。要素型Tに引き算が必要。
///
/// 引数 `lo, hi / 各軸の始端・終端`: 元配列上の0始まり端点。1Dはlo,hi。2Dはr1,c1,r2,c2。3Dはd1,r1,c1,d2,r2,c2。
///
/// 戻り値: Tの区間和。既定は[lo,hi)。空区間はT{}。標準pair/tupleは引き算を持たないため、そのままrange_sumへ渡さず各成分を引きます。
///
/// 注意: 入力の表を変更しません。端点を勝手にclampしません。半開区間へ変換した境界は0<=first<=last<=Nを満たすように渡します。LOCAL時は非空区間の境界をassertで確認します。
///
/// 呼び分け: range_sum(sum,l,r)は左を含み右を含まない。`range_sum<true,true>`は両端を含む。2D/3Dは2個のboolで全軸共通、4個/6個で各軸の開閉指定。
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

/// 累積和表から指定区間・領域の合計を取り出す。
///
/// 引数 `sum`: 元配列ではなく累積和の表。要素型Tに引き算が必要。
///
/// 引数 `lo, hi / 各軸の始端・終端`: 元配列上の0始まり端点。1Dはlo,hi。2Dはr1,c1,r2,c2。3Dはd1,r1,c1,d2,r2,c2。
///
/// 戻り値: Tの区間和。既定は[lo,hi)。空区間はT{}。標準pair/tupleは引き算を持たないため、そのままrange_sumへ渡さず各成分を引きます。
///
/// 注意: 入力の表を変更しません。端点を勝手にclampしません。半開区間へ変換した境界は0<=first<=last<=Nを満たすように渡します。LOCAL時は非空区間の境界をassertで確認します。
///
/// 呼び分け: range_sum(sum,l,r)は左を含み右を含まない。`range_sum<true,true>`は両端を含む。2D/3Dは2個のboolで全軸共通、4個/6個で各軸の開閉指定。
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

/// C++ストリームの同期を外し、cinとcoutのtieを外す。
///
/// 戻り値: boolで常にtrue。入出力前に呼び出す設定用関数。読み取りや出力そのものは行いません。
///
/// 注意: このプログラムの標準ストリーム設定を変更。Cのstdioと混在した順序は保証せず、入力前の自動flushもなくなります。対話問題は明示flushを使います。
///
/// 呼び分け: fast_ioは上の引数順で呼び出します。
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

/// 肯否の固定語を出力する。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。
///
/// 注意: coutに語と改行/空白を出力。通常はflushせず、AC_INTERACTIVEではflush。
///
/// 呼び分け: include前のAC_YES_UPPER/AC_YES_LOWERで表記を変更可能（同時指定不可）。
///
/// 例: `Yes();`
inline bool Yes(bool nl = true) {
    cout << AC_YES_WORD << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn No — (bool nl=true) -> bool — AC_NO_WORD を出力（既定 "No"）。常に true

/// 肯否の固定語を出力する。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。
///
/// 注意: coutに語と改行/空白を出力。通常はflushせず、AC_INTERACTIVEではflush。
///
/// 呼び分け: include前のAC_YES_UPPER/AC_YES_LOWERで表記を変更可能（同時指定不可）。
///
/// 例: `No();`
inline bool No(bool nl = true) {
    cout << AC_NO_WORD << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn YN — (bool cond, bool nl=true) -> bool — cond なら Yes、でなければ No

/// 条件の真偽でYes/Noのどちらかを出力する。
///
/// 引数 `cond`: 判定する条件。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。条件の値を返す形式ではありません。2引数の第2引数は改行指定です。
///
/// 注意: coutへYes/Noの既定表記を出力。AC_YES_UPPER/LOWERを反映。通常はflushせずAC_INTERACTIVE時はflush。
///
/// 呼び分け: YN(cond[,nl])。3条件以上の形式は各条件を1行ずつ出力。
///
/// 例: `YN(false);`
inline bool YN(bool cond, bool nl = true) {
    return cond ? Yes(nl) : No(nl);
}
// @acf fn YN — (const First& first, const Second& second, const Rest&... rest) -> bool — 複数の bool 相当の条件を 1 条件 1 行で Yes/No 出力

/// 3個以上の条件を1条件1行のYes/Noとして出力する。
///
/// 引数 `first / second / rest`: boolへ暗黙変換できる3個以上の条件。
///
/// 戻り値: boolで常にtrue。各条件を左から順に出力します。
///
/// 注意: coutへYes/Noの既定表記を出力。AC_YES_UPPER/LOWERを反映。通常はflushせずAC_INTERACTIVE時はflush。
///
/// 呼び分け: YN(a,b,c,...)。YN(a,b)は条件2個ではなくcond,nlの形式。
///
/// 例: `YN(true,false,1);`
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

/// rangeの各要素をYes/Noとして出力する。
///
/// 引数 `r`: 各要素がboolへ明示変換できるrange。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。空rangeは無出力。nl=falseは各語の後ろに空白。
///
/// 注意: coutへYes/Noの既定表記を出力。AC_YES_UPPER/LOWERを反映。通常はflushせずAC_INTERACTIVE時はflush。
///
/// 呼び分け: YN(range[,nl])。読み取り順に各要素を判定します。
///
/// 例: `YN(vi{1,0});`
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool YN(const R& r, bool nl = true) {
    if (std::empty(r)) return ac_detail::output::finish();
    for (const auto& e : r) YN(static_cast<bool>(e), nl);
    return true;
}
// @acf fn YES — (bool nl=true) -> bool — 常に "YES" を出力。常に true

/// 肯否の固定語を出力する。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。
///
/// 注意: coutに語と改行/空白を出力。通常はflushせず、AC_INTERACTIVEではflush。
///
/// 呼び分け: 常に大文字で、AC_YES_UPPER/LOWER設定とは独立。
///
/// 例: `YES();`
inline bool YES(bool nl = true) {
    cout << "YES" << (nl ? '\n' : ' ');
    return ac_detail::output::finish();
}
// @acf fn NO — (bool nl=true) -> bool — 常に "NO" を出力。常に true

/// 肯否の固定語を出力する。
///
/// 引数 `nl`: trueは改行、falseは末尾に空白1個を付ける。省略時は `true`。
///
/// 戻り値: boolで常にtrue。失敗statusではありません。
///
/// 注意: coutに語と改行/空白を出力。通常はflushせず、AC_INTERACTIVEではflush。
///
/// 呼び分け: 常に大文字で、AC_YES_UPPER/LOWER設定とは独立。
///
/// 例: `NO();`
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

/// 引数をデータとして空白区切りで出力し、最後に改行する。常に true。
/// `ac::out(v, ':')` の ':' は区切り文字ではなく、出力するデータ。
template <class... T>
bool out(const T&... values) {
    ac_detail::output::writeln(cout, ' ', values...);
    return output_detail::finish();
}

/// 1引数・1要素につき1行出力する。空rangeは無出力、引数なしは空行。
template <class... T>
bool out_lines(const T&... values) {
    if constexpr (sizeof...(T) == 0) cout << '\n';
    else (output_detail::write_lines(values), ...);
    return output_detail::finish();
}

/// rangeを指定した文字列で区切って出力し、最後に改行する。
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool out_join(const R& values, string_view separator = " ") {
    ac_detail::output::write_range(cout, values, separator);
    cout << '\n';
    return output_detail::finish();
}

/// 要素を1文字の区切りで標準出力へ書き、最後に改行する。値は変更しない。
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool out_join(const R& values, char separator) {
    ac_detail::output::write_range(cout, values, separator);
    cout << '\n';
    return output_detail::finish();
}

/// 1条件だけをYes/Noで出力する。AC_YES_UPPER / AC_YES_LOWERを反映。
inline bool yesno(bool condition) {
    cout << (condition ? AC_YES_WORD : AC_NO_WORD) << '\n';
    return output_detail::finish();
}

/// 各条件を1行ずつ出力する。2引数でも、両方を条件として扱う。
template <class... T,
          enable_if_t<(sizeof...(T) > 0) &&
                          (is_convertible_v<const T&, bool> && ...),
                      int> = 0>
bool yesno_each(const T&... conditions) {
    ((cout << (static_cast<bool>(conditions) ? AC_YES_WORD : AC_NO_WORD)
           << '\n'), ...);
    return output_detail::finish();
}

/// 配列の各条件を1行ずつYes/Noで出力する。空配列は何も出力しない。
template <class R, enable_if_t<ac_detail::io::is_range_v<R>, int> = 0>
bool yesno_each(const R& conditions) {
    for (const auto& condition : conditions)
        cout << (static_cast<bool>(condition) ? AC_YES_WORD : AC_NO_WORD) << '\n';
    return output_detail::finish();
}

/// 対話問題などでstdoutを明示的にflushする。常に true。
inline bool flush() {
    cout.flush();
    return true;
}
}  // namespace ac

// @acf fn has_bit — (ull mask, int i) -> bool — i番目のbitが立っているか

/// bitが立っているか判定する。
///
/// 引数 `mask`: 64bitのビット集合。
///
/// 引数 `i`: 0-indexのbit位置。0..63。
///
/// 戻り値: bool。指定bitが1ならtrue。
///
/// 注意: 副作用なし。時間O(1)。位置の範囲はLOCALでassert、通常ビルドでも0..63が必要。
///
/// 呼び分け: has_bitは上の引数順で呼び出します。
///
/// 例: `print(has_bit(5,2));`
constexpr bool has_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return (mask >> i) & 1ULL;
}

// @acf fn set_bit — (ull mask, int i) -> ull — i番目のbitを立てた値

/// 指定bitを1にする値を返す。
///
/// 引数 `mask`: 64bitのビット集合。
///
/// 引数 `i`: 0-indexのbit位置。0..63。
///
/// 戻り値: ull。元のmaskは変更しません。
///
/// 注意: 副作用なし。時間O(1)。位置の範囲はLOCALでassert、通常ビルドでも0..63が必要。
///
/// 呼び分け: set_bitは上の引数順で呼び出します。
///
/// 例: `print(set_bit(1,2));`
constexpr ull set_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask | (1ULL << i);
}

// @acf fn clear_bit — (ull mask, int i) -> ull — i番目のbitを消した値

/// 指定bitを0にする値を返す。
///
/// 引数 `mask`: 64bitのビット集合。
///
/// 引数 `i`: 0-indexのbit位置。0..63。
///
/// 戻り値: ull。元のmaskは変更しません。
///
/// 注意: 副作用なし。時間O(1)。位置の範囲はLOCALでassert、通常ビルドでも0..63が必要。
///
/// 呼び分け: clear_bitは上の引数順で呼び出します。
///
/// 例: `print(clear_bit(5,2));`
constexpr ull clear_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask & ~(1ULL << i);
}

// @acf fn toggle_bit — (ull mask, int i) -> ull — i番目のbitを反転した値

/// 指定bitを反転した値を返す。
///
/// 引数 `mask`: 64bitのビット集合。
///
/// 引数 `i`: 0-indexのbit位置。0..63。
///
/// 戻り値: ull。元のmaskは変更しません。
///
/// 注意: 副作用なし。時間O(1)。位置の範囲はLOCALでassert、通常ビルドでも0..63が必要。
///
/// 呼び分け: toggle_bitは上の引数順で呼び出します。
///
/// 例: `print(toggle_bit(5,2));`
constexpr ull toggle_bit(ull mask, int i) {
#ifdef LOCAL
    assert(0 <= i && i < numeric_limits<ull>::digits);
#endif
    return mask ^ (1ULL << i);
}

// @acf fn subsets — (int n) -> iota_view<ull, ull> — n要素の全部分集合を昇順に列挙

/// 全てのn要素の部分集合をbitmaskで列挙する。
///
/// 引数 `n`: 要素数0..63。64はシフトできないので不可。
///
/// 戻り値: `ranges::iota_view<ull,ull>`。maskは[0,2^n)の昇順。n=0は0だけ（空集合）。
///
/// 注意: 遅延列挙で全maskを確保しません。生成O(1)、全走査O(2^n)、追加メモリO(1)。LOCALは範囲をassert。
///
/// 呼び分け: subsetsは上の引数順で呼び出します。
///
/// 例: `for(ull mask:subsets(2)) print(mask);`
constexpr auto subsets(int n) {
#ifdef LOCAL
    assert(0 <= n && n < numeric_limits<ull>::digits);
#endif
    return views::iota(0ULL, 1ULL << n);
}

// @acf fn nonempty_subsets — (int n) -> iota_view<ull, ull> — 空集合を除く全部分集合を昇順に列挙

/// 空集合を除いたn要素の部分集合をbitmaskで列挙する。
///
/// 引数 `n`: 要素数0..63。64はシフトできないので不可。
///
/// 戻り値: `ranges::iota_view<ull,ull>`。maskは[1,2^n)の昇順。n=0は空range。
///
/// 注意: 遅延列挙で全maskを確保しません。生成O(1)、全走査O(2^n)、追加メモリO(1)。LOCALは範囲をassert。
///
/// 呼び分け: nonempty_subsetsは上の引数順で呼び出します。
///
/// 例: `for(ull mask:nonempty_subsets(2)) print(mask);`
constexpr auto nonempty_subsets(int n) {
#ifdef LOCAL
    assert(0 <= n && n < numeric_limits<ull>::digits);
#endif
    return views::iota(1ULL, 1ULL << n);
}

// @acf fn bit_count — (T x) -> int — 整数の立っているビット数

/// 整数のbit表現について立っているbit数。0は0。
///
/// 引数 `x`: bool以外の整数型。i128/u128も可。
///
/// 戻り値: int。立っているbit数。0は0。負数は絶対値ではなく同幅の符号なし型へ変換したbit表現を扱います。
///
/// 注意: 副作用なし。固定幅整数の演算で時間/メモリO(1)。
///
/// 呼び分け: bit_countは上の引数順で呼び出します。
///
/// 例: `print(bit_count(7));`
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

/// 整数のbit表現について末尾の0bit数。0は入力型のbit幅（bit位置として使えません）。
///
/// 引数 `x`: bool以外の整数型。i128/u128も可。
///
/// 戻り値: int。末尾の0bit数。0は入力型のbit幅（bit位置として使えません）。負数は絶対値ではなく同幅の符号なし型へ変換したbit表現を扱います。
///
/// 注意: 副作用なし。固定幅整数の演算で時間/メモリO(1)。
///
/// 呼び分け: bit_ctzは上の引数順で呼び出します。
///
/// 例: `print(bit_ctz(8));`
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

/// 立っているbitの位置を小さい順にcallbackへ渡す。
///
/// 引数 `mask`: 64bitのビット集合。
///
/// 引数 `f`: 0-indexのbit位置intを受け取るcallable。
///
/// 戻り値: void。mask=0は0回、bit63も扱います。
///
/// 注意: maskは値渡しで元の値を変更せずcallbackを実行。時間O(bit_count(mask))（callback除く）、追加メモリO(1)。
///
/// 呼び分け: for_each_bitは上の引数順で呼び出します。
template <class F>
constexpr void for_each_bit(ull mask, F f) {
    while (mask) {
        std::invoke(f, bit_ctz(mask));
        mask &= mask - 1;
    }
}

// @acf fn bit_length — (T x) -> int — 同幅の符号なし値のビット幅（0は0）

/// 整数のbit表現について上位の1までに必要なbit幅。0は0。
///
/// 引数 `x`: bool以外の整数型。i128/u128も可。
///
/// 戻り値: int。上位の1までに必要なbit幅。0は0。負数は絶対値ではなく同幅の符号なし型へ変換したbit表現を扱います。
///
/// 注意: 副作用なし。固定幅整数の演算で時間/メモリO(1)。
///
/// 呼び分け: bit_lengthは上の引数順で呼び出します。
///
/// 例: `print(bit_length(8));`
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

/// iteratorを0-indexの添字へ変換する。
///
/// 引数 `range`: random-access range。サイズと位置がintに収まること。
///
/// 引数 `it`: 同じrangeのbegin..endのiterator。
///
/// 戻り値: intのbeginからの距離。endはsize。返す添字がintの最大値を超える場合はoverflow_errorです。
///
/// 注意: rangeは変更しません。時間O(1)、不正/別rangeのiteratorや範囲外添字は渡せません。検査は行いません。
///
/// 呼び分け: iter_idxは上の引数順で呼び出します。
template <ranges::random_access_range R>
int iter_idx(R& range, ranges::iterator_t<R> it) {
    return ac_detail::checked_range_index(it - ranges::begin(range));
}

// @acf fn idx_iter — (random-access range, int idx) -> iterator — 添字を iterator へ変換

/// 0-indexの添字をiteratorへ変換する。
///
/// 引数 `range`: random-access range。サイズと位置がintに収まること。
///
/// 引数 `idx`: 0..sizeの添字。sizeはendを返す。
///
/// 戻り値: `ranges::iterator_t<R>`。idx==sizeはendで、逆参照してはいけません。
///
/// 注意: rangeは変更しません。時間O(1)、不正/別rangeのiteratorや範囲外添字は渡せません。検査は行いません。
///
/// 呼び分け: idx_iterは上の引数順で呼び出します。
template <ranges::random_access_range R>
auto idx_iter(R& range, int idx) {
    using D = ranges::range_difference_t<R>;
    return ranges::begin(range) + static_cast<D>(idx);
}

// @acf fn lower_bound_idx — (const vector<T>& a, const T& x, Comp comp={}) -> int — lower_bound の添字

/// ソート済み配列で最初の「x以上」位置を添字で返す。
///
/// 引数 `a`: compとprojectionの順序で整列した列。
///
/// 引数 `x`: 検索する境界値。既存vector版では要素型Tと一致。range版ではprojection適用後の値と比較可能な型。
///
/// 引数 `comp`: 順序を比較するcallable。既存vector版の既定型はless<>、range版はranges::less。省略時は `{}`。
///
/// 引数 `projection`: range版で各要素を比較前に変換するcallable。既定型はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。境界が無いときは要素数、空入力は0。不在の添字を参照しないでください。返す添字がintの最大値を超える場合はoverflow_errorです。
///
/// 注意: 配列は変更しません。勝手にソートもしません。比較関数の副作用は利用者側の責任です。
///
/// 呼び分け: lower_bound_idx(a,x)は昇順。降順配列はlower_bound_idx(a,x,greater{})。range版はlower_bound_idx(values,target,comp,projection)。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。std::lower_boundはiteratorを返す別のAPIです。
template <class T, class Comp = less<>>
int lower_bound_idx(const vector<T>& a, const T& x, Comp comp = {}) {
    return ac_detail::checked_range_index(lower_bound(a.begin(), a.end(), x, comp) - a.begin());
}
// @acf fn upper_bound_idx — (const vector<T>& a, const T& x, Comp comp={}) -> int — upper_bound の添字

/// ソート済み配列で最初の「xより大きい」位置を添字で返す。
///
/// 引数 `a`: compとprojectionの順序で整列した列。
///
/// 引数 `x`: 検索する境界値。既存vector版では要素型Tと一致。range版ではprojection適用後の値と比較可能な型。
///
/// 引数 `comp`: 順序を比較するcallable。既存vector版の既定型はless<>、range版はranges::less。省略時は `{}`。
///
/// 引数 `projection`: range版で各要素を比較前に変換するcallable。既定型はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。境界が無いときは要素数、空入力は0。不在の添字を参照しないでください。返す添字がintの最大値を超える場合はoverflow_errorです。
///
/// 注意: 配列は変更しません。勝手にソートもしません。比較関数の副作用は利用者側の責任です。
///
/// 呼び分け: upper_bound_idx(a,x)は昇順。降順配列はupper_bound_idx(a,x,greater{})。range版はupper_bound_idx(values,target,comp,projection)。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。std::upper_boundはiteratorを返す別のAPIです。
template <class T, class Comp = less<>>
int upper_bound_idx(const vector<T>& a, const T& x, Comp comp = {}) {
    return ac_detail::checked_range_index(upper_bound(a.begin(), a.end(), x, comp) - a.begin());
}
// @acf fn binsearch — (ll ok, ll ng, Pred check) -> ll — 答えの二分探索（ok 側を残す）

/// 単調な判定の真側を残し、真偽の境界を求める。
///
/// 引数 `ok`: 初期状態でcheck(ok)がtrueとなる値。
///
/// 引数 `ng`: 初期状態でcheck(ng)がfalseとなる値。
///
/// 引数 `check`: llを受け取る単調な真偽判定。初期端点の真偽は関数が検証しない。
///
/// 戻り値: ll。okとngが隣接するまで更新した真側の値。ng<okなら最初の真、ok<ngなら最後の真。隣接している初期値ならokをそのまま返し、checkを呼びません。
///
/// 注意: ok/ngは値渡しで、呼出元の変数は変更しません。checkの副作用は利用者側の責任です。初期の真偽境界が無いケースは呼出元で扱います。
///
/// 呼び分け: binsearch(ok,ng,check)の順。std::binary_searchによる配列の存在判定とは異なります。
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

/// 負数を含む整数除算を数学的に切り上げる。
///
/// 引数 `a`: 分子。
///
/// 引数 `b`: 0以外の分母。負数も可。
///
/// 戻り値: ll。b=0はinvalid_argument。LLONG_MIN/-1はoverflow_error。その他は商がllに収まります。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: C++の/は0方向への丸めなので負数では結果が異なる場合があります。
///
/// 例: `print(ceil_div(7,3),ceil_div(-7,3),ceil_div(7,-3));`
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

/// 負数を含む整数除算を数学的に切り下げる。
///
/// 引数 `a`: 分子。
///
/// 引数 `b`: 0以外の分母。負数も可。
///
/// 戻り値: ll。b=0はinvalid_argument。LLONG_MIN/-1はoverflow_error。その他は商がllに収まります。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: C++の/は0方向への丸めなので負数では結果が異なる場合があります。
///
/// 例: `print(floor_div(7,3),floor_div(-7,3),floor_div(7,-3));`
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

/// 配列の値を昇順の順位へ、その場で座標圧縮する。
///
/// 引数 `a`: ソート/比較でき、0..k-1の順位をTへ変換して保存できる配列。
///
/// 戻り値: 常にtrue。異なる値の数kに対して、各要素を0..k-1へ書き換えます。同じ値は同じ順位。空は空。
///
/// 注意: 元の値をコピーしてソートし、aの各要素を書き換えます。比較回数O(N log N)、追加メモリO(N)。元の値や逆変換表は返しません。
///
/// 呼び分け: 元の配列を保ち圧縮/復元したい場合はCoordinateCompressor。文字列の配列へ順位を書き戻す用途には使えません。
template <class T>
bool compress(vector<T>& a) {
    vector<T> vals = a;
    sort(ALL(vals));
    vals.erase(unique(ALL(vals)), vals.end());
    for (auto& x : a) x = (T)(lower_bound(ALL(vals), x) - vals.begin());
    return true;
}

// @acf fn sort_by_value — (const map<K,V,KeyComp,Alloc>& values, Comp comp={}) -> vector<pair<K,V>> — mapを値順に並べた列

/// mapを値の順に並べ替えた配列として取り出す。
///
/// 引数 `values`: 元の順序付きmap。
///
/// 引数 `comp`: secondを比較する厳密弱順序。省略時は `{}（less<>）`。
///
/// 戻り値: `vector<pair<Key,Value>>`。省略時は値の昇順。同じ値は元のmapのキー比較順。空mapなら空配列。
///
/// 注意: 元のmapを変更しません。要素をコピーしてソート。比較回数O(N log N)、追加メモリO(N)（比較・コピーの費用は除く）。
///
/// 呼び分け: sort_by_value(values) / sort_by_value(values,`greater<>`{})。同値のキー比較にはvalues.key_comp()を使います。
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

/// 配列の値をキー、添字を値とする逆引きmapを作る。
///
/// 引数 `values`: pair_likeでない1次元input_range。
///
/// 戻り値: `map<Value,int>`。0始まり添字へ逆引き。空は空map。重複値があればinvalid_argument。
///
/// 注意: 入力を変更せずmapへコピー。時間O(N log(N+1))、追加メモリO(N)。添字はintに収まること。
///
/// 呼び分け: pair列やmapの場合はfirstとsecondを反転する別オーバーロード。元の値が一意であることが必要です。
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

/// mapまたはpair列のキーと値を反転する。
///
/// 引数 `values`: 要素にfirst/secondを持つ1次元input_range。
///
/// 戻り値: `map<Value,Key>`。空は空map。元のsecondが重複するとinvalid_argument。
///
/// 注意: 入力を変更せずmapへコピー。時間O(N log(N+1))、追加メモリO(N)。添字はintに収まること。
///
/// 呼び分け: pair列やmapの場合はfirstとsecondを反転する別オーバーロード。元の値が一意であることが必要です。
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

/// 要素・部分文字列の最初の位置、またはネスト配列の座標を返す。
///
/// 引数 `values`: 1次元range、string_viewに変換できる文字列、またはnested range。
///
/// 引数 `target`: 比較する値。文字列同士なら探す部分文字列。
///
/// 戻り値: 1次元・部分文字列はintの0始まり添字。不在は要素数/文字列長N。nested rangeはviの座標で、不在は空vi。空文字列を探す文字列形式は位置0。
///
/// 注意: 入力は変更しません。iteratorは返しません。不在時のNを添字として参照しないでください。
///
/// 呼び分け: find(a,x)は自作API。iteratorが必要ならstd::ranges::find(a,x)。nested rangeの座標、文字列の部分文字列は戻り値が違うので入力型で選びます。
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
/// 要素・部分文字列の最初の位置、またはネスト配列の座標を返す。
///
/// 引数 `values`: 1次元range、string_viewに変換できる文字列、またはnested range。
///
/// 引数 `target`: 比較する値。文字列同士なら探す部分文字列。
///
/// 戻り値: 1次元・部分文字列はintの0始まり添字。不在は要素数/文字列長N。nested rangeはviの座標で、不在は空vi。空文字列を探す文字列形式は位置0。
///
/// 注意: 入力は変更しません。iteratorは返しません。不在時のNを添字として参照しないでください。
///
/// 呼び分け: find(a,x)は自作API。iteratorが必要ならstd::ranges::find(a,x)。nested rangeの座標、文字列の部分文字列は戻り値が違うので入力型で選びます。
int find(const R& values, const V& target) {
    const string_view source(values);
    const size_t position = source.find(string_view(target));
    return position == string_view::npos ? static_cast<int>(source.size())
                                         : static_cast<int>(position);
}

/// 要素・部分文字列の最初の位置、またはネスト配列の座標を返す。
///
/// 引数 `values`: 1次元range、string_viewに変換できる文字列、またはnested range。
///
/// 引数 `target`: 比較する値。文字列同士なら探す部分文字列。
///
/// 戻り値: 1次元・部分文字列はintの0始まり添字。不在は要素数/文字列長N。nested rangeはviの座標で、不在は空vi。空文字列を探す文字列形式は位置0。
///
/// 注意: 入力は変更しません。iteratorは返しません。不在時のNを添字として参照しないでください。
///
/// 呼び分け: find(a,x)は自作API。iteratorが必要ならstd::ranges::find(a,x)。nested rangeの座標、文字列の部分文字列は戻り値が違うので入力型で選びます。
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

// @acf fn has_common — (range, range) -> bool — 共通する要素があるか判定

/// 2つのrangeに共通する要素があればtrue、なければfalse。各要素を共通型に変換し、大小比較で同値か判定する。文字列そのものは文字のrangeとして扱う。入力は変更しない。空rangeはfalse。aがN要素・D種類、bがM要素のとき、時間O((N+M) log(D+1))、追加メモリO(D)（要素の変換・コピー・比較の費用を除く）
/// 呼び出し: `(const input_range& a, const input_range& b) -> bool`
template <class A, class B>
requires std::ranges::input_range<const A> && std::ranges::input_range<const B>
bool has_common(const A& a, const B& b) {
    using T = common_type_t<std::ranges::range_value_t<const A>,
                            std::ranges::range_value_t<const B>>;
    set<T> values;
    for (const auto& value : a) values.insert(static_cast<T>(value));
    if (values.empty()) return false;
    for (const auto& value : b)
        if (values.contains(static_cast<T>(value))) return true;
    return false;
}

// @acf fn rfind — (range, value) -> int — 配列から値を右から探す

/// 1次元列で最後に一致する値の添字を求める。
///
/// 引数 `values`: 1次元range。
///
/// 引数 `target`: ==で比較する値。
///
/// 戻り値: int。最後の0-indexed添字。不在は要素数N、空は0。stringでも部分文字列ではなく1文字の値探索です。
///
/// 注意: 左から全体を1回走査し、最後の一致位置を保持します。入力を変更しません。時間O(N)、追加メモリO(1)。
///
/// 呼び分け: 最初の一致はfind。std::string::rfindのnpos規則とは異なります。
///
/// 例: `print(rfind(vi{2,1,2},2),rfind(vi{2,1,2},9));`
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

/// 整数列に含まれない最小の非負整数を求める。
///
/// 引数 `values`: bool以外の整数型を要素とする1次元range。要素数はINT_MAX未満。
///
/// 戻り値: int。空なら0。負数と要素数より大きい値は無視します。要素数がINT_MAX以上ならinvalid_argument。
///
/// 注意: 入力をコピーし、出現を記録します。元の値は変更しません。時間・追加メモリO(N)。
///
/// 呼び分け: 順序と重複数は結果に影響しません。浮動小数点や`vector<bool>`はコンパイル時に拒否します。
///
/// 例: `print(mex(vi{-2,0,2,2}),mex(vi{}));`
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

/// 値ごとの出現回数をmapにまとめる。
///
/// 引数 `values`: 比較できる値を持つinput_range。文字列なら文字ごとの集計。
///
/// 戻り値: `map<T,ll>`。キーは要素型Tで比較順。値はllの個数。空入力には空map。存在しないキーは追加されていません。
///
/// 注意: 入力を変更しません。返されたmapの[]で不在キーを読むと追加されます。追加したくない確認はcontains/find、存在を確認後にat。
///
/// 呼び分け: 回数だけならfrequencies、位置が必要ならpositions。一つの値を一回だけ数えるならcountでもできます。
template <std::ranges::input_range R>
map<std::ranges::range_value_t<R>, ll> frequencies(R&& values) {
    map<std::ranges::range_value_t<R>, ll> result;
    for (const auto& value : values) ++result[value];
    return result;
}

// @acf fn positions — (range, ll start=0) -> map<T,vll> — 値ごとの出現位置一覧

/// 値ごとの出現位置をmapにまとめる。
///
/// 引数 `values`: 比較できる値を持つinput_range。文字列なら文字ごとの集計。
///
/// 引数 `start`: 先頭要素の位置。1なら1始まり、負の開始番号も指定できます。llに収まる値を渡します。
///
/// 戻り値: `map<T,vll>`。キーは要素型Tで比較順です。各値の位置を入力順に保持し、i番目の要素の位置はstart+iです。省略時は従来どおり0始まり。空入力は空mapで、存在しないキーは追加しません。
///
/// 注意: 入力を変更しません。要素をmapのキーへコピーし、各位置をllとして保存します。N要素、D種類で時間O(N log(D+1))、追加メモリO(N+D)です。比較・コピーのコストを除きます。位置がllの最大値を超える場合はoverflow_errorを送出します。最後の位置が最大値と等しい場合は成功します。返されたmapの[]で不在キーを読むと追加されるため、追加しない確認にはcontains/findを使います。
///
/// 呼び分け: `positions(values)`と`positions(values,0)`は同じ結果です。1始まりなら`positions(values,1)`。位置を保存せず回数だけ数えるならfrequenciesを使います。
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

/// 連続する同じ値を、値と個数の組へ圧縮する。
///
/// 引数 `values`: 値の!=が成立する1次元列。
///
/// 戻り値: `vector<pair<T,int>>`。入力順の各連続区間を保持。空は空。離れた同値は別の区間です。個数はintに収まること。
///
/// 注意: 入力を変更せず1回走査。時間O(N)、追加メモリO(連続区間数)。
///
/// 呼び分け: 値ごとの総個数はfrequencies。連続区間はrun_length_encode。
///
/// 例: `for(auto [value,count]:run_length_encode(string("aaabb"))) print(value,count);`
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

/// 値と個数の組を元の長さの配列へ展開する。
///
/// 引数 `encoded`: 各組は{値,0以上の個数}。
///
/// 戻り値: `vector<T>`。空は空。個数0は追加なし。負の個数はassert対象。展開後の全要素を確保できること。
///
/// 注意: 入力を変更せず、各値を個数分コピー。時間・追加メモリO(展開後の長さ)。
///
/// 呼び分け: 文字列からのencode結果もdecodeの戻り値は`vector<char>`。文字列へはto_str。
///
/// 例: `print(run_length_decode(vector<pii>{{2,2},{5,1}}));`
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

/// 1次元列の最小値を求める。
///
/// 引数 `values`: 空でもよい1次元range。値の<が成立すること。
///
/// 戻り値: `optional<T>`。空はnullopt。それ以外は最小値のコピー。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)（要素コピーの費用は別）。
///
/// 呼び分け: 多次元の軸ごとの値はaxis_min/axis_max。
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

/// 1次元列の最大値を求める。
///
/// 引数 `values`: 空でもよい1次元range。値の<が成立すること。
///
/// 戻り値: `optional<T>`。空はnullopt。それ以外は最大値のコピー。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)（要素コピーの費用は別）。
///
/// 呼び分け: 多次元の軸ごとの値はaxis_min/axis_max。
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

/// 1次元列の最小値と最大値を同じ走査で求める。
///
/// 引数 `values`: 1次元range。値の<が成立すること。
///
/// 戻り値: `optional<pair<T,T>>`の{最小値,最大値}。空はnullopt。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)。
///
/// 呼び分け: 片側だけ必要ならrange_min/range_max。
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

/// 整数列の最大値と最小値の差を求める。
///
/// 引数 `values`: bool以外の整数値を持つ1次元range。 入力値と最大値-最小値の中間差がi128で表せること。
///
/// 戻り値: `optional<ll>`。空はnullopt、1要素は0。差がllに収まらないとoverflow_error。
///
/// 注意: 値をi128に広げて差を計算し、llへ範囲を確認して変換。入力を変更しません。時間O(N)、追加メモリO(1)。
///
/// 呼び分け: range_minmaxは各元の値の型を返し、range_spanは差をllで返します。
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
/// 1次元列の最小値を求める。
///
/// 引数 `values`: 空でもよい1次元range。値の<が成立すること。
///
/// 戻り値: `optional<T>`。空はnullopt。それ以外は最小値のコピー。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)（要素コピーの費用は別）。
///
/// 呼び分け: 多次元の軸ごとの値はaxis_min/axis_max。
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

/// 1次元列の最大値を求める。
///
/// 引数 `values`: 空でもよい1次元range。値の<が成立すること。
///
/// 戻り値: `optional<T>`。空はnullopt。それ以外は最大値のコピー。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)（要素コピーの費用は別）。
///
/// 呼び分け: 多次元の軸ごとの値はaxis_min/axis_max。
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

/// 1次元列の最小値と最大値を同じ走査で求める。
///
/// 引数 `values`: 1次元range。値の<が成立すること。
///
/// 戻り値: `optional<pair<T,T>>`の{最小値,最大値}。空はnullopt。
///
/// 注意: 1回走査。単回走査の入力rangeは消費される。時間O(N)、追加メモリO(1)。
///
/// 呼び分け: 片側だけ必要ならrange_min/range_max。
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

/// 1-indexの端点対を読み、旧vviの隣接リストを作る。
///
/// 引数 `n`: 非負の頂点数。
///
/// 引数 `m`: 非負の辺数。
///
/// 引数 `directed`: trueは有向辺、falseは両方向に追加。省略時は `false`。
///
/// 戻り値: vvi。内部は0-index。m辺を指定方向で追加。空辺列も可。 重み・辺IDは持ちません。
///
/// 注意: cinから端点対を消費。全入力の読み取り成功、端点1..nを呼び出し側が保証します。時間/メモリO(N+M)。
///
/// 呼び分け: 0-index入力や辺IDが必要ならread_unweighted_graph。
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

/// 1-indexの端点対を読み、旧vviの隣接リストを作る。
///
/// 引数 `n`: 非負の頂点数。
///
/// 戻り値: vvi。内部は0-index。n-1辺を無向で読むだけで木の検査はしません。0/1頂点は辺なし。 重み・辺IDは持ちません。
///
/// 注意: cinから端点対を消費。全入力の読み取り成功、端点1..nを呼び出し側が保証します。時間/メモリO(N+M)。
///
/// 呼び分け: 0-index入力や辺IDが必要ならread_unweighted_tree。
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

/// STL range / pair / tuple / optional / adapter の出力。bitset は標準 `operator<<`（01 文字列）のまま
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

/// 各要素を変換し、入力順の新しい配列を作る。
///
/// 引数 `values`: 走査するrange。Rはinput_range。値はコピーして保存します。tuple/`pair中の参照とvector<bool>`のproxyは値にしますが、string_viewやポインタの参照先までは複製しません。
///
/// 引数 `function`: 各要素を受け取り、保存したい値を返すcallable。
///
/// 戻り値: `vector<U>`を返します。Uは変換結果から推論した保存用の値型です。空入力には空vectorを返し、functionは各要素へ1回適用します。
///
/// 注意: 関数自身は入力を変更しません。渡した述語・変換関数による書き換えや外部への副作用は利用者側の責任です。
///
/// 呼び分け: map_vec(values, function)。条件付き変換はmap_if、変換せず選ぶだけならfilter_vec。
template <ranges::input_range R, class F>
auto map_vec(R&& values, F function) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), ac_detail::collection::accept_all{}, std::move(function));
}

// @acf fn filter_vec — (range, predicate) -> vector<T> — 条件に合う要素を入力順に取り出す

/// 条件に合う要素だけを入力順に新しい配列へ取り出す。
///
/// 引数 `values`: 走査するinput_range。
///
/// 引数 `pred`: 要素を受け取り、採用するときtrueを返すcallable。
///
/// 戻り値: `vector<T>`を返します。Tは要素の保存用の値型です。空入力・全件不採用なら空vectorを返し、predは各要素へ1回適用します。
///
/// 注意: 関数自身は入力を変更しません。渡した述語・変換関数による書き換えや外部への副作用は利用者側の責任です。採用したK要素を保存用の値型へコピーして新しいvectorに保持する。時間O(N)、追加メモリO(K)。述語・要素コピーのコストを除く。
///
/// 呼び分け: filter_vec(values, pred)。選んだ後に変換するならmap_if。
template <ranges::input_range R, class Pred>
auto filter_vec(R&& values, Pred pred) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), std::move(pred), identity{});
}

// @acf fn map_if — (range, predicate, function) -> vector<U> — 条件に合う要素だけ変換する

/// 条件に合う要素だけ変換し、入力順の配列を作る。
///
/// 引数 `values`: 走査するinput_range。
///
/// 引数 `pred`: 各要素を判定する。trueなら採用。
///
/// 引数 `function`: 採用要素を変換し、保存する値を返す。
///
/// 戻り値: `vector<U>`を返します。空入力・全件不採用なら空です。predは全要素、functionは採用要素だけへ各1回適用します。
///
/// 注意: 関数自身は入力を変更しません。渡した述語・変換関数による書き換えや外部への副作用は利用者側の責任です。
///
/// 呼び分け: 引数順はrange、条件、変換。filter_vecとmap_vecの中間配列を作らずに処理します。
template <ranges::input_range R, class Pred, class F>
auto map_if(R&& values, Pred pred, F function) {
    return ac_detail::collection::collect<false>(
        std::forward<R>(values), std::move(pred), std::move(function));
}

// @acf fn to_vec — (range) -> vector<T> — 走査結果を値の配列にする

/// rangeの要素を値の配列へ保存する。
///
/// 引数 `values`: input_range。コピーできる保存用の値を持つ列。
///
/// 戻り値: `vector<T>`で入力順。空なら空。tuple/pair内の参照と`vector<bool>`のproxyは値で保存。ただしstring_viewやポインタの参照先は複製しません。
///
/// 注意: 入力自身を変更せず走査。時間・追加メモリO(N)。要素のコピー・比較の費用は別。
///
/// 呼び分け: 変換も行うならmap_vec、条件で選ぶならfilter_vec。
template <ranges::input_range R>
auto to_vec(R&& values) {
    return ::map_vec(std::forward<R>(values), identity{});
}

// @acf fn to_str — (char range) -> str — 文字列へ変換する

/// charのrangeを入力順に文字列へコピーする。
///
/// 引数 `values`: 要素の値型がcharのinput_range。
///
/// 戻り値: str（string）。空入力なら空文字列。
///
/// 注意: 元の列を変更せず、charを順に追加します。時間・追加メモリO(N)。
///
/// 呼び分け: 数字のrangeを10進の表示文字列へ変換するAPIではありません。
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

/// 文字列やvectorから、Pythonの[start:stop:step]と同じ添字で部分列をコピーする。
///
/// 引数 `values`: string、文字列リテラル、string_view、またはコピー可能な要素を持つvector。文字列はchar単位で、UTF-8ではバイト単位。
///
/// 引数 `start`: 開始添字。負値は末尾から数える。省略またはnulloptなら、正stepでは0、負stepでは末尾。
///
/// 引数 `stop`: 含めない終了添字。負値は末尾から数える。省略またはnulloptなら、正stepでは末尾の次、負stepでは先頭の前。逆順の明示-1は末尾を指すため、省略stopとは異なる。
///
/// 引数 `step`: 添字の進み幅。既定値1。負値で逆順、0はinvalid_argument。添字とstepはllの範囲で指定する。
///
/// 戻り値: 文字列入力はstr、vector入力は同じ`vector<T, Allocator>`型の新しい所有コンテナ。範囲外の境界はPythonと同じくクリップし、対象がなければ空。vectorのallocatorは入力から引き継ぐ。要素は通常のコピーで、ポインタやstring_viewの参照先までは複製しない。
///
/// 注意: 入力を変更しない。返した列は入力コンテナの寿命に依存しない。採用数をKとして時間・追加メモリO(K)で、要素のコピー費用は別。step=0は空入力でもinvalid_argument。
///
/// 呼び分け: sliced(values)は全体のコピー、sliced(values,start)は末尾まで、sliced(values,start,stop,step)は指定したスライス。途中の省略境界はnullopt。sliced(values,nullopt,nullopt,-1)で逆順全体。文字列だけを反転する既存のreversedも使える。
inline str sliced(string_view values, optional<ll> start = nullopt,
                 optional<ll> stop = nullopt, ll step = 1) {
    return ac_detail::slicing::copy(values, start, stop, step, str{});
}

/// 文字列やvectorから、Pythonの[start:stop:step]と同じ添字で部分列をコピーする。
///
/// 引数 `values`: string、文字列リテラル、string_view、またはコピー可能な要素を持つvector。文字列はchar単位で、UTF-8ではバイト単位。
///
/// 引数 `start`: 開始添字。負値は末尾から数える。省略またはnulloptなら、正stepでは0、負stepでは末尾。
///
/// 引数 `stop`: 含めない終了添字。負値は末尾から数える。省略またはnulloptなら、正stepでは末尾の次、負stepでは先頭の前。逆順の明示-1は末尾を指すため、省略stopとは異なる。
///
/// 引数 `step`: 添字の進み幅。既定値1。負値で逆順、0はinvalid_argument。添字とstepはllの範囲で指定する。
///
/// 戻り値: 文字列入力はstr、vector入力は同じ`vector<T, Allocator>`型の新しい所有コンテナ。範囲外の境界はPythonと同じくクリップし、対象がなければ空。vectorのallocatorは入力から引き継ぐ。要素は通常のコピーで、ポインタやstring_viewの参照先までは複製しない。
///
/// 注意: 入力を変更しない。返した列は入力コンテナの寿命に依存しない。採用数をKとして時間・追加メモリO(K)で、要素のコピー費用は別。step=0は空入力でもinvalid_argument。
///
/// 呼び分け: sliced(values)は全体のコピー、sliced(values,start)は末尾まで、sliced(values,start,stop,step)は指定したスライス。途中の省略境界はnullopt。sliced(values,nullopt,nullopt,-1)で逆順全体。文字列だけを反転する既存のreversedも使える。
template <class T, class Allocator>
vector<T, Allocator> sliced(const vector<T, Allocator>& values,
                           optional<ll> start = nullopt, optional<ll> stop = nullopt,
                           ll step = 1) {
    return ac_detail::slicing::copy(values, start, stop, step,
                                   vector<T, Allocator>(values.get_allocator()));
}

// @acf fn to_set — (range) -> set<T> — 重複を除いた昇順の集合にする

/// rangeの要素を値の重複を除く集合へ保存する。
///
/// 引数 `values`: input_range。コピーできる保存用の値を持つ列。
///
/// 戻り値: `set<T>`で比較順。要素を<で比較できること。空なら空。tuple/pair内の参照と`vector<bool>`のproxyは値で保存。ただしstring_viewやポインタの参照先は複製しません。
///
/// 注意: 入力自身を変更せず走査。時間O(N log(D+1))、追加メモリO(D)。Dは異なる値の数。要素のコピー・比較の費用は別。
///
/// 呼び分け: 変換も行うならmap_vec、条件で選ぶならfilter_vec。
template <ranges::input_range R>
auto to_set(R&& values) {
    return ac_detail::collection::collect<true>(
        std::forward<R>(values), ac_detail::collection::accept_all{}, identity{});
}

// @acf fn is_unique — (range) -> bool — 全要素に重複がないか

/// 全要素に重複がないかを判定する。
///
/// 引数 `values`: setの順序比較が成立する保存用の値を持つinput_range。
///
/// 戻り値: bool。空・1要素はtrue。最初の重複でfalse。判定はsetの比較上の同値関係に従います。
///
/// 注意: 元の列を変更せず、setへコピーして挿入。時間O(N log(D+1))、追加メモリO(D)。
///
/// 呼び分け: all_uniqueはis_uniqueの別名。個数を知りたいならfrequencies。
///
/// 例: `print(is_unique(vi{1,2,3}),is_unique(vi{1,2,1}));`
template <ranges::input_range R>
bool is_unique(R&& values) {
    using T = ac_detail::collection::stored_t<ranges::range_value_t<R>>;
    set<T> seen;
    for (auto&& value : values)
        if (!seen.emplace(value).second) return false;
    return true;
}

// @acf fn all_unique — (range) -> bool — 全要素が互いに異なるか

/// 全要素に重複がないかを判定する。
///
/// 引数 `values`: setの順序比較が成立する保存用の値を持つinput_range。
///
/// 戻り値: bool。空・1要素はtrue。最初の重複でfalse。判定はsetの比較上の同値関係に従います。
///
/// 注意: 元の列を変更せず、setへコピーして挿入。時間O(N log(D+1))、追加メモリO(D)。
///
/// 呼び分け: all_uniqueはis_uniqueの別名。個数を知りたいならfrequencies。
///
/// 例: `print(all_unique(vi{1,2,3}),all_unique(vi{1,2,1}));`
template <ranges::input_range R>
bool all_unique(R&& values) {
    return ::is_unique(std::forward<R>(values));
}

// @acf fn flat_map — (range, function) -> vector<T> — 各要素から作る列を1段平坦化する

/// 各要素から作った列を、入力順に1段平坦化する。
///
/// 引数 `values`: 走査するinput_range。
///
/// 引数 `function`: 各要素を受け取り、input_rangeを返すcallable。
///
/// 戻り値: `vector<T>`。返された各列の要素を順に値で連結。空入力・全て空の内側列は空。tuple/pairの参照とbool proxyは値へ。
///
/// 注意: functionを各要素へ1回適用。参照で返る列は丸ごとコピーせず走査、一時列はその回の走査後に破棄。時間O(N+M)、追加メモリO(M)と内側の一時列（生成・コピー費用は別）。callableの副作用は利用者側の責任です。
///
/// 呼び分け: map_vecは内側列をそのまま要素として保存します。std::flat_mapも宣言されるC++23環境では、::flat_mapでこの関数を指定します。
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

/// 0始まりの添字と要素を一緒に走査するviewを作る。
///
/// 引数 `values`: viewable_range。lvalueの寿命は利用者が管理。
///
/// 戻り値: view。各要素は{符号付き添字,元の要素}のtuple。添字型は元rangeのdifference_typeで、int固定ではない。空入力は空。参照できる元の要素は参照のまま。
///
/// 注意: 配列を新しく作らず、走査時に添字を付けます。rvalueコンテナはviewが所有。lvalueに基づくviewを元の寿命より長く保持しません。構築は元rangeをview化する費用、添字付与は1要素O(1)。走査O(N)、追加メモリO(1)。元rangeの走査コストを除く。
///
/// 呼び分け: C++23のviews::enumerate対応標準ライブラリが必要。値の保存はto_vecなどで明示します。
template <ranges::viewable_range R>
auto enumerate(R&& values) {
    return views::enumerate(std::forward<R>(values));
}
#endif

#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip >= 202110L
// @acf fn zip — (ranges...) -> view — 同じ位置の要素をまとめて最短列まで走査する

/// 複数の列の同じ位置の要素をまとめて走査する。
///
/// 引数 `values...`: viewable_rangeの列。各lvalueの寿命は利用者が管理。
///
/// 戻り値: tupleを要素とするview。最短の列で終了。空の列があれば空、0本も空。元の要素の参照規則を保ちます。
///
/// 注意: 組の配列を確保しません。rvalueコンテナはviewが所有、lvalueへは参照します。参照要素への書き込みは元の列を変えます。
///
/// 呼び分け: 全組合せはproduct。同位置の組はzip。C++23 views::zip対応が必要。
template <ranges::viewable_range... Rs>
auto zip(Rs&&... values) {
    return views::zip(std::forward<Rs>(values)...);
}
#endif

#if defined(__cpp_lib_ranges_cartesian_product) && __cpp_lib_ranges_cartesian_product >= 202207L
// @acf fn product — (ranges...) -> view — 左の範囲を外側にして全組合せを走査する

/// 複数の列の直積を、左の列を外側にして走査する。
///
/// 引数 `values...`: viewable_rangeの列。2本目以降は再走査できるforward_range。
///
/// 戻り値: tupleを要素とするview。空の列を1本でも含めば空。0本の場合は空tupleを1個。要素の参照規則を保ちます。
///
/// 注意: 全組合せの配列を先に作りません。rvalueコンテナはviewが所有。結果を保存する場合は組合せ数とメモリに注意。
///
/// 呼び分け: 同じ位置の組だけならzip。C++23 views::cartesian_product対応が必要。
///
/// 例: `for(auto [x,y]:product(vi{1,2},vc{'a','b'})) print(x,y);`
template <ranges::viewable_range... Rs>
auto product(Rs&&... values) {
    return views::cartesian_product(std::forward<Rs>(values)...);
}
#endif

// @acf fn irange — (n) / (lo, hi) / (lo, hi, step) -> view — 半開区間の整数を走査する

/// 半開区間の整数を、指定した間隔で走査する。
///
/// 引数 `n / lo, hi`: 1引数は[0,n)、2/3引数はloからhiの手前。
///
/// 引数 `step`: 正なら昇順、負なら降順。0は不可。
///
/// 戻り値: view。要素型は各引数の符号付き型とintの共通型。向きに合わない区間は空。step=0はinvalid_argument、符号付き型へ表せない値はout_of_range。
///
/// 注意: 整数の配列を確保しません。端点は含む下端と含まない上端。3引数版は64ビットまでの型に対応。構築・1回の遷移はO(1)、K個の走査はO(K)、追加メモリO(1)。
///
/// 呼び分け: irange(n) / irange(lo,hi)は1ずつ昇順。irange(lo,hi,step)で負のstepも指定できます。
///
/// 例: `print(to_vec(irange(5,0,-2)));`
template <class L, class H>
requires(is_integral_v<L> && is_integral_v<H> && !is_same_v<L, bool> && !is_same_v<H, bool>)
auto irange(L lo, H hi) {
    using T = common_type_t<int, make_signed_t<L>, make_signed_t<H>>;
    if (!std::in_range<T>(+lo) || !std::in_range<T>(+hi))
        throw out_of_range("irange: endpoints do not fit the signed value type");
    const T first = static_cast<T>(lo), last = static_cast<T>(hi);
    return views::iota(first, std::max(first, last));
}

/// 半開区間の整数を、指定した間隔で走査する。
///
/// 引数 `n / lo, hi`: 1引数は[0,n)、2/3引数はloからhiの手前。
///
/// 引数 `step`: 正なら昇順、負なら降順。0は不可。
///
/// 戻り値: view。要素型は各引数の符号付き型とintの共通型。向きに合わない区間は空。step=0はinvalid_argument、符号付き型へ表せない値はout_of_range。
///
/// 注意: 整数の配列を確保しません。端点は含む下端と含まない上端。3引数版は64ビットまでの型に対応。構築・1回の遷移はO(1)、K個の走査はO(K)、追加メモリO(1)。
///
/// 呼び分け: irange(n) / irange(lo,hi)は1ずつ昇順。irange(lo,hi,step)で負のstepも指定できます。
///
/// 例: `print(to_vec(irange(5,0,-2)));`
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

/// 半開区間の整数を、指定した間隔で走査する。
///
/// 引数 `n / lo, hi`: 1引数は[0,n)、2/3引数はloからhiの手前。
///
/// 引数 `step`: 正なら昇順、負なら降順。0は不可。
///
/// 戻り値: view。要素型は各引数の符号付き型とintの共通型。向きに合わない区間は空。step=0はinvalid_argument、符号付き型へ表せない値はout_of_range。
///
/// 注意: 整数の配列を確保しません。端点は含む下端と含まない上端。3引数版は64ビットまでの型に対応。構築・1回の遷移はO(1)、K個の走査はO(K)、追加メモリO(1)。
///
/// 呼び分け: irange(n) / irange(lo,hi)は1ずつ昇順。irange(lo,hi,step)で負のstepも指定できます。
///
/// 例: `print(to_vec(irange(5,0,-2)));`
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

/// 連続区間が全て条件を満たすかを判定する。
///
/// 引数 `values`: forward_range。判定中にイテレータを無効化しません。
///
/// 引数 `pred`: Width個の連続した要素を受け取るbool相当のcallable。
///
/// 引数 `Width`: 2以上の窓幅。既定は2。
///
/// 戻り値: bool。列がWidth未満ならtrue。最初の偽で終了。
///
/// 注意: 関数自身は元の列を変更しません。窓は抽出済みの結果ではなく元の列に対して進めます。述語による副作用は利用者側の責任。時間O(N*Width)、窓のメモリO(Width)。
///
/// 呼び分け: all_adjacent(values,pred) / `all_adjacent<Width>`(values,pred)。非連続な全組合せの判定ではありません。
///
/// 例: `print(all_adjacent(vi{1,2,3},[](int a,int b){return a<b;}),all_adjacent(vi{},[](int a,int b){return a<b;}));`
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

/// 連続区間が全て条件を満たすかを判定する。
///
/// 引数 `values`: forward_range。判定中にイテレータを無効化しません。
///
/// 引数 `pred`: Width個の連続した要素を受け取るbool相当のcallable。
///
/// 引数 `Width`: 2以上の窓幅。既定は2。
///
/// 戻り値: bool。列がWidth未満ならtrue。最初の偽で終了。
///
/// 注意: 関数自身は元の列を変更しません。窓は抽出済みの結果ではなく元の列に対して進めます。述語による副作用は利用者側の責任。時間O(N*Width)、窓のメモリO(Width)。
///
/// 呼び分け: all_adjacent(values,pred) / `all_adjacent<Width>`(values,pred)。非連続な全組合せの判定ではありません。
///
/// 例: `print(all_adjacent(vi{1,2,3},[](int a,int b){return a<b;}),all_adjacent(vi{},[](int a,int b){return a<b;}));`
template <ranges::forward_range R, class Pred>
bool all_adjacent(R&& values, Pred pred) {
    return ::all_adjacent<2>(std::forward<R>(values), std::move(pred));
}

// @acf fn any_adjacent — (range, predicate) / any_adjacent<Width>(range, predicate) -> bool — 条件を満たす連続区間があるか

/// 連続区間が1つでも条件を満たすかを判定する。
///
/// 引数 `values`: forward_range。判定中にイテレータを無効化しません。
///
/// 引数 `pred`: Width個の連続した要素を受け取るbool相当のcallable。
///
/// 引数 `Width`: 2以上の窓幅。既定は2。
///
/// 戻り値: bool。列がWidth未満ならfalse。最初の真で終了。
///
/// 注意: 関数自身は元の列を変更しません。窓は抽出済みの結果ではなく元の列に対して進めます。述語による副作用は利用者側の責任。時間O(N*Width)、窓のメモリO(Width)。
///
/// 呼び分け: any_adjacent(values,pred) / `any_adjacent<Width>`(values,pred)。非連続な全組合せの判定ではありません。
///
/// 例: `print(any_adjacent(vi{1,2,3},[](int a,int b){return a<b;}),any_adjacent(vi{},[](int a,int b){return a<b;}));`
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

/// 連続区間が1つでも条件を満たすかを判定する。
///
/// 引数 `values`: forward_range。判定中にイテレータを無効化しません。
///
/// 引数 `pred`: Width個の連続した要素を受け取るbool相当のcallable。
///
/// 引数 `Width`: 2以上の窓幅。既定は2。
///
/// 戻り値: bool。列がWidth未満ならfalse。最初の真で終了。
///
/// 注意: 関数自身は元の列を変更しません。窓は抽出済みの結果ではなく元の列に対して進めます。述語による副作用は利用者側の責任。時間O(N*Width)、窓のメモリO(Width)。
///
/// 呼び分け: any_adjacent(values,pred) / `any_adjacent<Width>`(values,pred)。非連続な全組合せの判定ではありません。
///
/// 例: `print(any_adjacent(vi{1,2,3},[](int a,int b){return a<b;}),any_adjacent(vi{},[](int a,int b){return a<b;}));`
template <ranges::forward_range R, class Pred>
bool any_adjacent(R&& values, Pred pred) {
    return ::any_adjacent<2>(std::forward<R>(values), std::move(pred));
}

// @acf fn filter_adjacent — (range, predicate) / filter_adjacent<Width>(range, predicate) -> vector<T> — 連続区間の条件で末尾を抽出

/// 連続区間の条件で末尾の要素を抽出する。
///
/// 引数 `values`: forward_range。判定中にイテレータを無効化しません。
///
/// 引数 `pred`: Width個の連続した要素を受け取るbool相当のcallable。
///
/// 引数 `Width`: 2以上の窓幅。既定は2。
///
/// 戻り値: `vector<T>`。先頭Width-1要素は残し、以後は元の列の窓で条件が真なら末尾を保存。短い列は全てを返します。
///
/// 注意: 関数自身は元の列を変更しません。窓は抽出済みの結果ではなく元の列に対して進めます。述語による副作用は利用者側の責任。時間O(N*Width)、窓のメモリO(Width)と出力O(K)。
///
/// 呼び分け: filter_adjacent(values,pred) / `filter_adjacent<Width>`(values,pred)。非連続な全組合せの判定ではありません。
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

/// rangeに条件を満たす要素があるかを判定する。
///
/// 引数 `values`: input_range。
///
/// 引数 `pred`: projection後の値を受け取る条件。
///
/// 引数 `projection`: 要素を条件へ渡す前の変換。省略時は `{}（identity）`。
///
/// 戻り値: bool。空入力はfalse。最初の真で判定を終了します。
///
/// 注意: 関数自身は入力を変更しません。pred/projectionによる副作用は利用者側の責任。最大O(N)回の走査、追加メモリO(1)。
///
/// 呼び分け: range版の短縮。std::any_of(first,last,pred)のイテレータ版もそのまま使用可能。
///
/// 例: `print(any_of(vi{1,2,3},[](int x){return x>0;}),any_of(vi{},[](int x){return x>0;}));`
template <ranges::input_range R, class Pred, class Proj = identity>
bool any_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::any_of(std::forward<R>(values), std::move(pred),
                          std::move(projection));
}

// @acf fn all_of — (range, predicate, projection=identity) -> bool — 全要素が条件を満たすか

/// rangeに全要素が条件を満たすかを判定する。
///
/// 引数 `values`: input_range。
///
/// 引数 `pred`: projection後の値を受け取る条件。
///
/// 引数 `projection`: 要素を条件へ渡す前の変換。省略時は `{}（identity）`。
///
/// 戻り値: bool。空入力はtrue。最初の偽で判定を終了します。
///
/// 注意: 関数自身は入力を変更しません。pred/projectionによる副作用は利用者側の責任。最大O(N)回の走査、追加メモリO(1)。
///
/// 呼び分け: range版の短縮。std::all_of(first,last,pred)のイテレータ版もそのまま使用可能。
///
/// 例: `print(all_of(vi{1,2,3},[](int x){return x>0;}),all_of(vi{},[](int x){return x>0;}));`
template <ranges::input_range R, class Pred, class Proj = identity>
bool all_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::all_of(std::forward<R>(values), std::move(pred),
                          std::move(projection));
}

// @acf fn none_of — (range, predicate, projection=identity) -> bool — 条件を満たす要素がないか

/// rangeに条件を満たす要素がないかを判定する。
///
/// 引数 `values`: input_range。
///
/// 引数 `pred`: projection後の値を受け取る条件。
///
/// 引数 `projection`: 要素を条件へ渡す前の変換。省略時は `{}（identity）`。
///
/// 戻り値: bool。空入力はtrue。最初の真で判定を終了します。
///
/// 注意: 関数自身は入力を変更しません。pred/projectionによる副作用は利用者側の責任。最大O(N)回の走査、追加メモリO(1)。
///
/// 呼び分け: range版の短縮。std::none_of(first,last,pred)のイテレータ版もそのまま使用可能。
///
/// 例: `print(none_of(vi{1,2,3},[](int x){return x>0;}),none_of(vi{},[](int x){return x>0;}));`
template <ranges::input_range R, class Pred, class Proj = identity>
bool none_of(R&& values, Pred pred, Proj projection = {}) {
    return ranges::none_of(std::forward<R>(values), std::move(pred),
                           std::move(projection));
}

// @acf fn count — (range, value, projection=identity) -> ll — 値と等しい要素数

/// range内の値と等しい要素数を数える。
///
/// 引数 `values`: 走査するinput_range。
///
/// 引数 `target`: 射影後の要素と比較する値。
///
/// 引数 `projection`: 要素を判定・比較の前に変換する。省略時は `{} (identity)`。
///
/// 戻り値: llの個数。空入力は0。
///
/// 注意: 関数自身は入力を変更しません。渡した述語・変換関数による書き換えや外部への副作用は利用者側の責任です。
///
/// 呼び分け: count(values,target)。第3引数にprojectionを指定できます。iterator版std::countはそのまま別に使えます。
///
/// 例: `print(count(vi{1,2,2},2));`
template <ranges::input_range R, class T, class Proj = identity>
ll count(R&& values, const T& target, Proj projection = {}) {
    return static_cast<ll>(ranges::count(std::forward<R>(values), target,
                                         std::move(projection)));
}

// @acf fn count_if — (range, predicate, projection=identity) -> ll — 条件を満たす要素数

/// range内の条件を満たす要素数を数える。
///
/// 引数 `values`: 走査するinput_range。
///
/// 引数 `pred`: 射影後の要素を判定するcallable。
///
/// 引数 `projection`: 要素を判定・比較の前に変換する。省略時は `{} (identity)`。
///
/// 戻り値: llの個数。空入力は0。
///
/// 注意: 関数自身は入力を変更しません。渡した述語・変換関数による書き換えや外部への副作用は利用者側の責任です。
///
/// 呼び分け: count_if(values,pred)。第3引数にprojectionを指定できます。iterator版std::count_ifはそのまま別に使えます。
///
/// 例: `print(count_if(vi{1,2,2},[](int x){return x>1;}));`
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

/// 各要素に関数を適用する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::for_each` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(for_each)
/// 2つの列が初めて異なる位置を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::mismatch` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(mismatch)
/// 2つの列の長さと各要素が等しいかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::equal` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(equal)
/// 条件を満たす最初の要素へのイテレータを返す。見つからなければ終端。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_if)
/// 条件を満たさない最初の要素へのイテレータを返す。見つからなければ終端。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_if_not` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_if_not)
/// 部分列が最後に現れる区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_end` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_end)
/// 候補のいずれかと一致する最初の要素を探す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_first_of` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_first_of)
/// 隣り合う2要素が一致する最初の位置を探す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::adjacent_find` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(adjacent_find)
/// 部分列が最初に現れる区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::search` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(search)
/// 同じ値が指定回数続く最初の区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::search_n` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(search_n)
/// 各要素を指定した出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(copy)
/// 条件を満たす要素だけを出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::copy_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(copy_if)
/// 末尾から逆順にコピーする。出力先の終端を渡す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::copy_backward` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(copy_backward)
/// 各要素を出力先へムーブする。元の要素の状態は型に依存する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::move` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(move)
/// 末尾から逆順にムーブする。出力先の終端を渡す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::move_backward` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(move_backward)
/// 2つの列の対応する要素を交換する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::swap_ranges` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(swap_ranges)
/// 全要素を指定した値に書き換える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fill` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fill)
/// 各要素を関数で変換し、出力先へ書く。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::transform` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(transform)
/// 関数を呼び出して生成した値で全要素を埋める。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::generate` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(generate)
/// 指定値以外を前に詰め、不要になった末尾区間を返す。コンテナの長さは変わらない。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::remove` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(remove)
/// 条件を満たさない要素を前に詰め、末尾区間を返す。長さは変わらない。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::remove_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(remove_if)
/// 指定値以外を出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::remove_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(remove_copy)
/// 条件を満たさない要素を出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::remove_copy_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(remove_copy_if)
/// 指定値と一致する要素を新しい値に書き換える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::replace` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(replace)
/// 条件を満たす要素を新しい値に書き換える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::replace_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(replace_if)
/// 指定値を置き換えながら出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::replace_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(replace_copy)
/// 条件を満たす値を置き換えながら出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::replace_copy_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(replace_copy_if)
/// 連続する同じ値を1つにまとめ、不要な末尾区間を返す。長さは変わらない。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::unique` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(unique)
/// 連続する同じ値を1つにまとめてコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::unique_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(unique_copy)
/// 要素の順序を逆にする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::reverse` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(reverse)
/// 逆順に出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::reverse_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(reverse_copy)
/// 指定位置を先頭にするように要素を循環移動する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::rotate` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(rotate)
/// 指定位置を先頭にした順序で出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::rotate_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(rotate_copy)
/// 乱数生成器を使って要素を並べ替える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::shuffle` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(shuffle)
/// 指定個数までの要素を無作為に選び、出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::sample` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(sample)
/// 条件を満たす要素が前、満たさない要素が後に分かれているかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_partitioned` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(is_partitioned)
/// 条件を満たす要素を前に集め、後半の区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::partition` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(partition)
/// 条件に応じて2つの出力先へコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::partition_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(partition_copy)
/// 元の順序を保ちながら条件で前半と後半に分ける。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::stable_partition` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(stable_partition)
/// 条件で前後に分かれた列の境界を返す。未分割の列には使わない。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::partition_point` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(partition_point)
/// 列を昇順に並べ替える。比較関数を渡せば順序を変えられる。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::sort` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(sort)
/// 同じ値どうしの元の順序を保って並べ替える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::stable_sort` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(stable_sort)
/// 指定した中間位置までに小さい要素を整列して集める。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::partial_sort` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(partial_sort)
/// 小さい要素から出力先の容量分を整列してコピーする。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::partial_sort_copy` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(partial_sort_copy)
/// 列が昇順に並んでいるかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_sorted` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(is_sorted)
/// 昇順が初めて崩れる位置を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_sorted_until` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(is_sorted_until)
/// 指定位置の要素を整列後と同じ値にし、小さい側と大きい側に分ける。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::nth_element` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(nth_element)
/// 整列済みの列で、指定値以上となる最初の位置を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::lower_bound` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(lower_bound)
/// 整列済みの列で、指定値より大きい最初の位置を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::upper_bound` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(upper_bound)
/// 整列済みの列に指定値があるかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::binary_search` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(binary_search)
/// 整列済みの列で指定値と等しい要素の区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::equal_range` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(equal_range)
/// 2つの整列済み列を1つの整列済み列として出力する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::merge` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(merge)
/// 連続する2つの整列済み区間を、その場で1つに併合する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::inplace_merge` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(inplace_merge)
/// 整列済みの列が、もう一方の列の要素を重複数も含めて全て含むかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::includes` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(includes)
/// 整列済みの2列について、前者だけにある要素を出力する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::set_difference` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(set_difference)
/// 整列済みの2列に共通する要素を出力する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::set_intersection` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(set_intersection)
/// 整列済みの2列の片方だけにある要素を出力する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::set_symmetric_difference` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(set_symmetric_difference)
/// 整列済みの2列の和集合を出力する。重複数は多い方を採用する。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::set_union` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(set_union)
/// ヒープ末尾に追加した1要素を適切な位置へ動かす。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::push_heap` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(push_heap)
/// ヒープ先頭を末尾へ移し、残りをヒープに戻す。要素は削除しない。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::pop_heap` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(pop_heap)
/// 列をヒープに並べ替える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::make_heap` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(make_heap)
/// ヒープを昇順の列に並べ替える。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::sort_heap` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(sort_heap)
/// 列がヒープ条件を満たすかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_heap` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(is_heap)
/// ヒープ条件が初めて崩れる位置を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_heap_until` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(is_heap_until)
/// 最小要素へのイテレータを返す。空なら終端。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::min_element` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(min_element)
/// 最大要素へのイテレータを返す。空なら終端。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::max_element` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(max_element)
/// 最小・最大要素へのイテレータをまとめて返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::minmax_element` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(minmax_element)
/// 2つの列を辞書順で比較し、前者が小さいかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::lexicographical_compare` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(lexicographical_compare)
/// 辞書順で次の並びにする。次がなければ最初に戻してfalseを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::next_permutation` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(next_permutation)
/// 辞書順で前の並びにする。前がなければ最後に戻してfalseを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::prev_permutation` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(prev_permutation)
/// 並び順を無視すると2つの列が同じかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::is_permutation` と同じ。
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

/// 空でない列の最小値をコピーして返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::min` と同じ。
AC_RANGE_EXTREMA_WRAPPER_(min)
/// 空でない列の最大値をコピーして返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::max` と同じ。
AC_RANGE_EXTREMA_WRAPPER_(max)
/// 空でない列の最小値と最大値をコピーして返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::minmax` と同じ。
AC_RANGE_EXTREMA_WRAPPER_(minmax)

// std::min/max/minmax の2値版は同じ型を要求する。
// 異なる算術型の場合だけ共通型で比較する。
/// 異なる算術型の2値を共通の型へ変換し、小さい値を返す。符号付きと符号なしの混在では変換に注意。
template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> && (!is_same_v<T, U>)
constexpr common_type_t<T, U> min(T first, U second) {
    using Common = common_type_t<T, U>;
    const Common a = static_cast<Common>(first);
    const Common b = static_cast<Common>(second);
    return b < a ? b : a;
}

/// 異なる算術型の2値を共通の型へ変換し、大きい値を返す。符号付きと符号なしの混在では変換に注意。
template <class T, class U>
requires is_arithmetic_v<T> && is_arithmetic_v<U> && (!is_same_v<T, U>)
constexpr common_type_t<T, U> max(T first, U second) {
    using Common = common_type_t<T, U>;
    const Common a = static_cast<Common>(first);
    const Common b = static_cast<Common>(second);
    return a < b ? b : a;
}

/// 異なる算術型の2値を共通の型へ変換し、{小さい値, 大きい値}を返す。
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
/// 列に指定値が含まれるかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::contains` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(contains)
/// 列に指定した部分列が含まれるかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::contains_subrange` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(contains_subrange)
#endif

#if defined(__cpp_lib_ranges_starts_ends_with) && \
    __cpp_lib_ranges_starts_ends_with >= 202106L
/// 列の先頭が指定した列と一致するかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::starts_with` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(starts_with)
/// 列の末尾が指定した列と一致するかを返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::ends_with` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(ends_with)
#else
/// 列の先頭がprefixと一致するかを返す。空のprefixは常に一致する。
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
/// 指定値が最後に現れる位置から終端までの区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_last` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_last)
/// 条件を満たす最後の位置から終端までの区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_last_if` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_last_if)
/// 条件を満たさない最後の位置から終端までの区間を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::find_last_if_not` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(find_last_if_not)
#endif

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold >= 202207L
/// 先頭から順に二項演算で値をまとめる。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_left` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_left)
/// 最初の要素を初期値にして左から値をまとめる。空なら空のoptional。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_left_first` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_first)
/// 末尾側から順に二項演算で値をまとめる。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_right` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_right)
/// 最後の要素を初期値にして右から値をまとめる。空なら空のoptional。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_right_last` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_right_last)
/// 左から値をまとめ、走査終了位置と結果を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_left_with_iter` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_with_iter)
/// 最初の要素から左から順に畳み込み、終了位置とoptionalの結果を返す。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::fold_left_first_with_iter` と同じ。
AC_RANGE_ALGORITHM_WRAPPER_(fold_left_first_with_iter)
#endif

#if defined(__cpp_lib_ranges_iota) && __cpp_lib_ranges_iota >= 202202L
/// 指定値から1ずつ増やした値で列を埋める。
/// 第1引数は対象の列。残りの引数と戻り値は `std::ranges::iota` と同じ。
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
/// ソート済み配列で最初の「x以上」位置を添字で返す。
///
/// 引数 `a`: compとprojectionの順序で整列した列。
///
/// 引数 `x`: 検索する境界値。既存vector版では要素型Tと一致。range版ではprojection適用後の値と比較可能な型。
///
/// 引数 `comp`: 順序を比較するcallable。既存vector版の既定型はless<>、range版はranges::less。省略時は `{}`。
///
/// 引数 `projection`: range版で各要素を比較前に変換するcallable。既定型はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。境界が無いときは要素数、空入力は0。不在の添字を参照しないでください。返す添字がintの最大値を超える場合はoverflow_errorです。
///
/// 注意: 配列は変更しません。勝手にソートもしません。比較関数の副作用は利用者側の責任です。
///
/// 呼び分け: lower_bound_idx(a,x)は昇順。降順配列はlower_bound_idx(a,x,greater{})。range版はlower_bound_idx(values,target,comp,projection)。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。std::lower_boundはiteratorを返す別のAPIです。
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
/// ソート済み配列で最初の「xより大きい」位置を添字で返す。
///
/// 引数 `a`: compとprojectionの順序で整列した列。
///
/// 引数 `x`: 検索する境界値。既存vector版では要素型Tと一致。range版ではprojection適用後の値と比較可能な型。
///
/// 引数 `comp`: 順序を比較するcallable。既存vector版の既定型はless<>、range版はranges::less。省略時は `{}`。
///
/// 引数 `projection`: range版で各要素を比較前に変換するcallable。既定型はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。境界が無いときは要素数、空入力は0。不在の添字を参照しないでください。返す添字がintの最大値を超える場合はoverflow_errorです。
///
/// 注意: 配列は変更しません。勝手にソートもしません。比較関数の副作用は利用者側の責任です。
///
/// 呼び分け: upper_bound_idx(a,x)は昇順。降順配列はupper_bound_idx(a,x,greater{})。range版はupper_bound_idx(values,target,comp,projection)。添字への変換はrandom-access rangeでO(1)、それ以外でO(N)。std::upper_boundはiteratorを返す別のAPIです。
int upper_bound_idx(R&& values, const T& target, Comp comp = {}, Proj projection = {}) {
    const auto it = ranges::upper_bound(values, target, std::move(comp),
                                        std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), it));
}

// @acf fn find_idx — (range, target, projection={}) -> int — 値と一致する最初の添字

/// projection適用後にtargetと等しい最初の要素を探す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `target`: 射影後の要素と比較する値。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_idx(a, 2), find_idx(a, 8));`
template <ranges::forward_range R, class T, class Proj = identity>
int find_idx(R&& values, const T& target, Proj projection = {}) {
    const auto result = ranges::find(values, target, std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn rfind_idx — (range, target, projection={}) -> int — 値と一致する最後の添字

/// projection適用後にtargetと等しい最後の要素を左からの走査で探す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `target`: 射影後の要素と比較する値。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。ranges::find_lastの対応状況によらず使える。
///
/// 例: `vi a{2, 7, 2, 9}; print(rfind_idx(a, 2), rfind_idx(a, 8));`
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

/// projection適用後にpredが真になる最初の要素を探す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_if_idx(a, [](int x){ return x > 5; }), find_if_idx(a, [](int x){ return x < 0; }));`
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_if_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_if(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn find_if_not_idx — (range, pred, projection={}) -> int — 条件を満たさない最初の添字

/// projection適用後にpredが偽になる最初の要素を探す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_if_not_idx(a, [](int x){ return x < 5; }), find_if_not_idx(a, [](int x){ return x > 0; }));`
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_if_not_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_if_not(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn adjacent_find_idx — (range, pred={}, projection={}) -> int — 条件を満たす最初の隣接ペアの添字

/// projection適用後の隣接2要素を左、右の順でpredへ渡し、最初に真になるペアの左側の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の2要素へ適用する二項述語。既定はranges::equal_to。隣接ペアは左、右の順。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。隣接ペアの左側の添字。1要素では1。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi h{27, 1828, 1828, 9242}; print(adjacent_find_idx(h, greater_equal{})); print(adjacent_find_idx(vi{1, 2, 3}));`
template <ranges::forward_range R, class Pred = ranges::equal_to, class Proj = identity>
int adjacent_find_idx(R&& values, Pred pred = {}, Proj projection = {}) {
    const auto result = ranges::adjacent_find(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn find_first_of_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 候補のいずれかと一致する最初の添字

/// otherのいずれかとpredで一致する最初の要素を探す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `other`: 比較するforward range。Mは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の2要素へ適用する二項述語。既定はranges::equal_to。引数順はvalues側、other側。省略時は `{}`。
///
/// 引数 `projection1`: values側の射影。既定はidentity。省略時は `{}`。
///
/// 引数 `projection2`: other側の射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N*M+N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_first_of_idx(a, vi{7, 9}), find_first_of_idx(a, vi{}));`
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int find_first_of_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::find_first_of(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn search_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 連続部分列が最初に一致する添字

/// otherが連続部分列として最初に一致する始点を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `other`: 比較するforward range。Mは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の2要素へ適用する二項述語。既定はranges::equal_to。引数順はvalues側、other側。省略時は `{}`。
///
/// 引数 `projection1`: values側の射影。既定はidentity。省略時は `{}`。
///
/// 引数 `projection2`: other側の射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。otherが空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N*M+N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `str s = "ababa"; print(search_idx(s, string_view("ba")), search_idx(s, string_view("z")), search_idx(s, string_view{}));`
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int search_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::search(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_end_idx — (range, other, pred={}, projection1={}, projection2={}) -> int — 連続部分列が最後に一致する添字

/// otherが連続部分列として最後に一致する始点を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `other`: 比較するforward range。Mは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の2要素へ適用する二項述語。既定はranges::equal_to。引数順はvalues側、other側。省略時は `{}`。
///
/// 引数 `projection1`: values側の射影。既定はidentity。省略時は `{}`。
///
/// 引数 `projection2`: other側の射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。otherが空ならN。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N*M+N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `str s = "ababa"; print(find_end_idx(s, string_view("ba")), find_end_idx(s, string_view("z")), find_end_idx(s, string_view{}));`
template <ranges::forward_range R1, ranges::forward_range R2,
          class Pred = ranges::equal_to, class Proj1 = identity, class Proj2 = identity>
int find_end_idx(R1&& values, R2&& other, Pred pred = {},
                      Proj1 projection1 = {}, Proj2 projection2 = {}) {
    const auto result = ranges::find_end(values, other, std::move(pred), std::move(projection1),
                                           std::move(projection2));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn search_n_idx — (range, count, target, pred={}, projection={}) -> int — 同じ値が指定個数続く最初の添字

/// projection適用後にtargetとpredで一致する要素がcount個続く最初の始点を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `count`: 連続して一致する必要個数。0以下なら始点0。省略時は `None`。
///
/// 引数 `target`: 射影後の要素と比較する値。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。count<=0なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{1, 2, 2, 3}; print(search_n_idx(a, 2, 2), search_n_idx(a, 3, 2), search_n_idx(a, 0, 9));`
template <ranges::forward_range R, class T, class Pred = ranges::equal_to,
          class Proj = identity>
int search_n_idx(R&& values, ranges::range_difference_t<R> count, const T& target,
                 Pred pred = {}, Proj projection = {}) {
    const auto result = ranges::search_n(values, count, target, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn equal_range_idx — (range, target, comp={}, projection={}) -> pair<int,int> — 等しい値の区間を添字で返す

/// compとprojectionの順序で整列した列の一致区間を0始まりの半開区間[l,r)として返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `target`: 射影後の要素と比較する値。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: `pair<int,int>`の半開区間[l,r)。不在なら挿入位置pに対する{p,p}、空なら{0,0}。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: compとprojectionの順序で整列した列を渡す。入力をコピー・変更せず、中間配列を作らない。比較O(log(N+1))回、イテレータ移動O(N)以下。random-accessかつ終端との差をO(1)で得られる列なら全体O(log(N+1))。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{1, 3, 3, 7}; auto [l, r] = equal_range_idx(a, 3); print(l, r); auto [p, q] = equal_range_idx(a, 4); print(p, q);`
template <ranges::forward_range R, class T, class Comp = ranges::less,
          class Proj = identity>
pair<int, int> equal_range_idx(R&& values, const T& target, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::equal_range(values, target, std::move(comp),
                                           std::move(projection));
    return {ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin())),
            ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.end()))};
}

// @acf fn mismatch_idx — (range, other, pred={}, projection1={}, projection2={}) -> pair<int,int> — 最初に一致しなくなる2列の添字

/// 2列を左から比較し、最初の不一致位置を各列の0始まりの添字で返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `other`: 比較するforward range。Mは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の2要素へ適用する二項述語。既定はranges::equal_to。引数順はvalues側、other側。省略時は `{}`。
///
/// 引数 `projection1`: values側の射影。既定はidentity。省略時は `{}`。
///
/// 引数 `projection2`: other側の射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: `pair<int,int>`。最初の不一致位置を各列の0始まり添字で返す。共通部分が一致なら短い列の要素数を両方に返す。片方が空なら{0,0}。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(min(N,M))。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
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

/// compとprojectionによる最小要素の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。同値の最小要素は最初を選ぶ。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{3, 1, 1, 5}; print(min_element_idx(a), min_element_idx(vi{}));`
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int min_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::min_element(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn max_element_idx — (range, comp={}, projection={}) -> int — 最大要素の添字

/// compとprojectionによる最大要素の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。同値の最大要素は最初を選ぶ。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{3, 5, 1, 5}; print(max_element_idx(a), max_element_idx(vi{}));`
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int max_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::max_element(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn is_sorted_until_idx — (range, comp={}, projection={}) -> int — 整列順序が初めて崩れる添字

/// compとprojectionによる整列順序が初めて崩れる要素の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの整列順序が初めて崩れる位置。全体が整列済みならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{1, 3, 2, 4}; print(is_sorted_until_idx(a), is_sorted_until_idx(vi{1, 2, 3}));`
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
int is_sorted_until_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::is_sorted_until(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn minmax_element_idx — (range, comp={}, projection={}) -> pair<int,int> — 最小要素と最大要素の添字

/// compとprojectionによる最小要素と最大要素の0始まりの添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: `pair<int,int>`で最小・最大の添字。同値なら最小は最初、最大は最後。空なら{0,0}。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{1, 5, 1, 5}; auto [i, j] = minmax_element_idx(a); print(i, j); auto [p, q] = minmax_element_idx(vi{}); print(p, q);`
template <ranges::forward_range R, class Comp = ranges::less, class Proj = identity>
pair<int, int> minmax_element_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::minmax_element(values, std::move(comp),
                                             std::move(projection));
    return {ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.min)),
            ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.max))};
}

// @acf fn partition_point_idx — (range, pred, projection={}) -> int — 条件が真から偽へ切り替わる添字

/// projection適用後のpredが真、偽の順に分割された列で、最初の偽の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの最初の偽の添字。全て真ならN、全て偽または空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: projection適用後のpredが真、偽の順に分割された列を渡す。入力をコピー・変更せず、中間配列を作らない。比較O(log(N+1))回、イテレータ移動O(N)以下。random-accessかつ終端との差をO(1)で得られる列なら全体O(log(N+1))。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{1, 3, 5, 6, 8}; print(partition_point_idx(a, [](int x){ return x % 2 == 1; }));`
template <ranges::forward_range R, class Pred, class Proj = identity>
int partition_point_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::partition_point(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

// @acf fn is_heap_until_idx — (random-access range, comp={}, projection={}) -> int — heap条件が初めて崩れる添字

/// compとprojectionによるheap条件が初めて崩れる要素の0始まりの添字を返す。
///
/// 引数 `values`: random-access range。Nは要素数。省略時は `None`。
///
/// 引数 `comp`: 射影後の値の厳密弱順序。既定はranges::less。省略時は `{}`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intのheap条件が初めて崩れる位置。全体がheapならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。
///
/// 例: `vi a{9, 5, 7, 6}; print(is_heap_until_idx(a), is_heap_until_idx(vi{9, 5, 7}));`
template <ranges::random_access_range R, class Comp = ranges::less, class Proj = identity>
int is_heap_until_idx(R&& values, Comp comp = {}, Proj projection = {}) {
    const auto result = ranges::is_heap_until(values, std::move(comp), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result));
}

#if defined(__cpp_lib_ranges_find_last) && __cpp_lib_ranges_find_last >= 202207L
// @acf fn find_last_idx — (range, target, projection={}) -> int — 値と一致する最後の添字

/// 値と一致する最後の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `target`: 射影後の要素と比較する値。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。C++23のranges::find_last系に対応する標準ライブラリでのみ定義される。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_last_idx(a, 2), find_last_idx(a, 8));`
template <ranges::forward_range R, class T, class Proj = identity>
int find_last_idx(R&& values, const T& target, Proj projection = {}) {
    const auto result = ranges::find_last(values, target, std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_last_if_idx — (range, pred, projection={}) -> int — 条件を満たす最後の添字

/// 条件を満たす最後の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。C++23のranges::find_last系に対応する標準ライブラリでのみ定義される。
///
/// 例: `vi a{2, 7, 2, 9}; print(find_last_if_idx(a, [](int x){ return x < 5; }), find_last_if_idx(a, [](int x){ return x < 0; }));`
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_last_if_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_last_if(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

// @acf fn find_last_if_not_idx — (range, pred, projection={}) -> int — 条件を満たさない最後の添字

/// 条件を満たさない最後の添字を返す。
///
/// 引数 `values`: 走査するforward range。Nは要素数。省略時は `None`。
///
/// 引数 `pred`: 射影後の要素、targetの順に適用する二項述語。既定はranges::equal_to。省略時は `None`。
///
/// 引数 `projection`: 各要素へ適用する射影。既定はidentity。省略時は `{}`。
///
/// 戻り値: intの0始まり添字。不在ならN、空なら0。返す添字がintの最大値を超えればoverflow_error。
///
/// 注意: 入力をコピー・変更せず、中間配列を作らない。時間O(N)。追加メモリO(1)。述語・比較・射影のコストを除く。コールバック自身の副作用は利用者が管理する。
///
/// 呼び分け: イテレータではなく添字を返す入口。C++23のranges::find_last系に対応する標準ライブラリでのみ定義される。
template <ranges::forward_range R, class Pred, class Proj = identity>
int find_last_if_not_idx(R&& values, Pred pred, Proj projection = {}) {
    const auto result = ranges::find_last_if_not(values, std::move(pred), std::move(projection));
    return ac_detail::checked_range_index(ranges::distance(ranges::begin(values), result.begin()));
}

#endif

// @acf fn argsort — (const range, comp={}, projection={}) -> vector<int> — 値順に並べた元添字を返す

/// 値の並び順に元の添字を並べた配列を作る。
///
/// 引数 `values`: constで読めるsized random-access range。vector・array・組み込み配列など。要素数はintの最大値以下。
///
/// 引数 `comp`: projection後の値を比較する厳密弱順序。sort(range, comp, projection)と同じ引数順。省略時は `{}（ranges::less）`。
///
/// 引数 `projection`: 比較する値を取り出すcallableやメンバポインタ。const要素を受け取ります。省略時は `{}（identity）`。
///
/// 戻り値: `vector<int>`。返り値`p`の`p[k]`は、並べ替え後のk番目の要素が元の入力で持つ0始まりの添字。同値は昇順・降順とも元添字の小さい順。空入力には空配列。要素数がintの最大値を超えるとoverflow_error。
///
/// 注意: 入力要素をコピー・移動・並べ替えません。比較回数O(N log N)、追加メモリO(N)。比較関数・projectionの費用は除きます。渡したcallableの外部への副作用は利用者側の責任です。
///
/// 呼び分け: `argsort(values)` / `argsort(values, comp)` / `argsort(values, comp, projection)`。降順は`greater{}`、メンバで比較するなら`argsort(items, less{}, &Item::score)`。引数順は`sort`・`stable_sort`のrange版と同じ。
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

/// 全ての要素の式から入力順の配列を作る。
///
/// 引数 `vars`: 各要素を束縛する変数。名前の有効範囲は生成したラムダ内。
///
/// 引数 `range`: 走査する範囲。入力式は1回評価。
///
/// 引数 `expression`: 全ての要素へ1回評価。外側の変数は参照キャプチャ。
///
/// 戻り値: `vector<T>`。式から推論する保存型で入力順。空入力は空。全要素を変換します。
///
/// 注意: マクロ自身は入力を変更しませんが、式は要素や外側の変数を書き換えられます。外側の変数は参照キャプチャ。range内のカンマを含む式は括弧で囲んで一つのマクロ引数にします。
///
/// 呼び分け: `COLLECT(vars,range,expression)`。分解束縛は`COLLECT((i,x),enumerate(a),pair(i,x))`のように書きます。条件で選ぶならCOLLECT_IF。
#define COLLECT(vars, range, ...) \
    ::map_vec((range), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro SUM — (vars, range, expression) -> expression_type — 式の値を総和に集計

/// 各葉について式を評価し、その値の総和を求めます。
///
/// 引数 `vars`: constの葉への参照を束縛します。COLLECTと同じ束縛構文で、変数の有効範囲は生成するラムダ内です。
///
/// 引数 `range`: 入力式は1回だけ評価します。ネストしたrangeではsum_byと同じく葉まで走査します。カンマを含む入力式は括弧で囲みます。
///
/// 引数 `expression`: 各葉につき1回評価します。外側の変数は参照キャプチャします。
///
/// 戻り値: 式からautoで推論した値型で集計します。参照と最上位のcvは外れ、型は自動で広がりません。空rangeはその型の{}を返し、式は評価しません。intの合計をllにするなら`SUM(x,a,ll(x))`、二乗和なら`SUM(x,a,ll(x)*x)`のように演算前に型を広げます。
///
/// 注意: sum_byを呼ぶ式マクロです。入力コンテナや葉を束縛のためにコピーせず、中間vectorも作りません。式の結果は値として扱うため、式によってコピーが生じます。式・加算・値の受け渡しのコストを除き、訪問する要素と部分rangeの総数Nに対して時間O(N)、追加メモリO(次元数)です。1次元では追加メモリO(1)。式は外側の変数を変更でき、その副作用は利用者が管理します。
///
/// 呼び分け: `SUM(vars,range,expression)`。mapなら`SUM((k,v),f,v/2)`。COLLECTと同様に常時定義され、AC_NO_PRINT_MACROSでは無効になりません。マクロを使わない場合はsum_byを呼べます。
#define SUM(vars, range, ...) \
    ::sum_by((range), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_IF — (vars, range, condition, expression) -> vector<T> — 条件付き配列内包記法

/// 条件に合う要素の式から入力順の配列を作る。
///
/// 引数 `vars`: 各要素を束縛する変数。名前の有効範囲は生成したラムダ内。
///
/// 引数 `range`: 走査する範囲。入力式は1回評価。
///
/// 引数 `condition`: 各要素へ1回評価。trueの要素だけ採用。
///
/// 引数 `expression`: 採用要素だけへ1回評価。外側の変数は参照キャプチャ。
///
/// 戻り値: `vector<T>`。式から推論する保存型で入力順。空入力・全件不採用は空。
///
/// 注意: マクロ自身は入力を変更しませんが、式・条件は要素や外側の変数を書き換えられます。外側の変数は参照キャプチャ。rangeやcondition内のカンマを含む式は括弧で囲んで一つのマクロ引数にします。
///
/// 呼び分け: `COLLECT_IF(vars,range,condition,expression)`。分解束縛は`COLLECT_IF((i,x),enumerate(a),i>0,pair(i,x))`のように書きます。条件を省くならCOLLECT。
#define COLLECT_IF(vars, range, condition, ...) \
    ::map_if((range), AC_COLLECT_LAMBDA_(vars, condition), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_SET — (vars, range, expression) -> set<T> — 集合の内包記法

/// 全ての要素の式から重複を除いた集合を作る。
///
/// 引数 `vars`: 各要素を束縛する変数。名前の有効範囲は生成したラムダ内。
///
/// 引数 `range`: 走査する範囲。入力式は1回評価。
///
/// 引数 `expression`: 全ての要素へ1回評価。外側の変数は参照キャプチャ。
///
/// 戻り値: `set<T>`。重複を除き比較順。比較可能な保存型が必要。空入力は空。全要素を変換します。
///
/// 注意: マクロ自身は入力を変更しませんが、式は要素や外側の変数を書き換えられます。外側の変数は参照キャプチャ。range内のカンマを含む式は括弧で囲んで一つのマクロ引数にします。 結果が重複しても式の評価は省略しません。
///
/// 呼び分け: `COLLECT_SET(vars,range,expression)`。分解束縛は`COLLECT_SET((i,x),enumerate(a),pair(i,x))`のように書きます。条件で選ぶならCOLLECT_SET_IF。
#define COLLECT_SET(vars, range, ...) \
    ::ac_detail::collection::collect<true>( \
        (range), ::ac_detail::collection::accept_all{}, AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))

// @acf macro COLLECT_SET_IF — (vars, range, condition, expression) -> set<T> — 条件付き集合内包記法

/// 条件に合う要素の式から重複を除いた集合を作る。
///
/// 引数 `vars`: 各要素を束縛する変数。名前の有効範囲は生成したラムダ内。
///
/// 引数 `range`: 走査する範囲。入力式は1回評価。
///
/// 引数 `condition`: 各要素へ1回評価。trueの要素だけ採用。
///
/// 引数 `expression`: 採用要素だけへ1回評価。外側の変数は参照キャプチャ。
///
/// 戻り値: `set<T>`。重複を除き比較順。比較可能な保存型が必要。空入力・全件不採用は空。
///
/// 注意: マクロ自身は入力を変更しませんが、式・条件は要素や外側の変数を書き換えられます。外側の変数は参照キャプチャ。rangeやcondition内のカンマを含む式は括弧で囲んで一つのマクロ引数にします。 結果が重複しても式の評価は省略しません。
///
/// 呼び分け: `COLLECT_SET_IF(vars,range,condition,expression)`。分解束縛は`COLLECT_SET_IF((i,x),enumerate(a),i>0,pair(i,x))`のように書きます。条件を省くならCOLLECT_SET。
#define COLLECT_SET_IF(vars, range, condition, ...) \
    ::ac_detail::collection::collect<true>( \
        (range), AC_COLLECT_LAMBDA_(vars, condition), AC_COLLECT_LAMBDA_(vars, __VA_ARGS__))


// @acf fn diagonal — (const grid&) -> row_type — 左上から右下の斜め成分

/// 長方形の各行の (i,i) を先頭から min(H,W) 個返す。vector<string> は string、vector<vector<T>> は vector<T> を返す。空の行列または幅0なら空。不揃いな行は invalid_argument。要素コピーの費用を除き、時間 O(H)、返り値の領域 O(min(H,W))
template <class T>
vector<T> diagonal(const vector<vector<T>>& grid) {
    if (!ac_detail::grid::rectangular(grid)) throw invalid_argument("diagonal: ragged grid");
    if (grid.empty()) return {};
    vector<T> result;
    const size_t n = min(grid.size(), grid.front().size());
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) result.push_back(grid[i][i]);
    return result;
}
/// 長方形の各行の (i,i) を先頭から min(H,W) 個返す。vector<string> は string、vector<vector<T>> は vector<T> を返す。空の行列または幅0なら空。不揃いな行は invalid_argument。要素コピーの費用を除き、時間 O(H)、返り値の領域 O(min(H,W))
inline string diagonal(const vector<string>& grid) {
    if (!ac_detail::grid::rectangular(grid)) throw invalid_argument("diagonal: ragged grid");
    if (grid.empty()) return {};
    string result;
    const size_t n = min(grid.size(), grid.front().size());
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) result.push_back(grid[i][i]);
    return result;
}

// @acf fn anti_diagonal — (const grid&) -> row_type — 右上から左下の斜め成分

/// 長方形の各行の (i,W-1-i) を先頭から min(H,W) 個返す。vector<string> は string、vector<vector<T>> は vector<T> を返す。空の行列または幅0なら空。不揃いな行は invalid_argument。要素コピーの費用を除き、時間 O(H)、返り値の領域 O(min(H,W))
template <class T>
vector<T> anti_diagonal(const vector<vector<T>>& grid) {
    if (!ac_detail::grid::rectangular(grid)) throw invalid_argument("anti_diagonal: ragged grid");
    if (grid.empty()) return {};
    vector<T> result;
    const size_t w = grid.front().size(), n = min(grid.size(), w);
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) result.push_back(grid[i][w - 1 - i]);
    return result;
}
/// 長方形の各行の (i,W-1-i) を先頭から min(H,W) 個返す。vector<string> は string、vector<vector<T>> は vector<T> を返す。空の行列または幅0なら空。不揃いな行は invalid_argument。要素コピーの費用を除き、時間 O(H)、返り値の領域 O(min(H,W))
inline string anti_diagonal(const vector<string>& grid) {
    if (!ac_detail::grid::rectangular(grid)) throw invalid_argument("anti_diagonal: ragged grid");
    if (grid.empty()) return {};
    string result;
    const size_t w = grid.front().size(), n = min(grid.size(), w);
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) result.push_back(grid[i][w - 1 - i]);
    return result;
}


// @acf macro repeach — repeach(value, range): 各要素を読み取り専用の参照で走査

/// `for (const auto& value : (range))` と同じ。range式は1回評価し、要素をコピーしない。break / continue は通常の範囲forと同じ。走査時間は要素数と本体の処理による
#define repeach(value, range) for (const auto& value : (range))
