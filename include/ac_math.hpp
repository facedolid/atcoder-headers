#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include "ac.hpp"

// 数論・組合せ・行列。アルゴリズム本体はコアから移動し、仕様は維持する。

// @acf fn arith_sum — (a, d, n) -> ll / modint — 等差数列和

/// 等差数列の先頭n項の和を求める。
///
/// 引数 `a`: 初項。
///
/// 引数 `d`: 公差。Mint版では整数または同じMint型。
///
/// 引数 `n`: 0以上の項数。
///
/// 戻り値: 整数版はll、Mint版は同じMint型。n=0は0。n<0はinvalid_argument。整数版で結果がll、または中間計算がi128を超えるとoverflow_error。
///
/// 注意: 入力は値渡し。整数版は広い中間計算を検査。Mint版は同じ法で計算。2の逆元を使わず整数因子を先に約分します。
///
/// 呼び分け: arith_sum(ll,ll,ll) / arith_sum(Mint,integer-or-Mint,ll)。Mint版にはACLが必要。
inline ll arith_sum(ll a, ll d, ll n) {
    if (n < 0) throw invalid_argument("arith_sum: n must be non-negative");
    if (n == 0) return 0;
    i128 count = n;
    i128 edge_sum = i128(2) * a + i128(n - 1) * d;
    if (count % 2 == 0) count /= 2;
    else edge_sum /= 2;
    return ac_detail::checked_ll(
        ac_detail::checked_mul_i128(count, edge_sum, "arith_sum"), "arith_sum");
}

#ifdef AC_HAS_ACL
/// 等差数列の先頭n項の和を求める。
///
/// 引数 `a`: 初項。
///
/// 引数 `d`: 公差。Mint版では整数または同じMint型。
///
/// 引数 `n`: 0以上の項数。
///
/// 戻り値: 整数版はll、Mint版は同じMint型。n=0は0。n<0はinvalid_argument。整数版で結果がll、または中間計算がi128を超えるとoverflow_error。
///
/// 注意: 入力は値渡し。整数版は広い中間計算を検査。Mint版は同じ法で計算。2の逆元を使わず整数因子を先に約分します。
///
/// 呼び分け: arith_sum(ll,ll,ll) / arith_sum(Mint,integer-or-Mint,ll)。Mint版にはACLが必要。
template <class Mint, class Diff,
          enable_if_t<
              ac_detail::io::is_modint_v<Mint> &&
                  (is_integral_v<remove_cvref_t<Diff>> ||
                   is_same_v<remove_cvref_t<Diff>, remove_cvref_t<Mint>>),
              int> = 0>
Mint arith_sum(Mint a, Diff d, ll n) {
    if (n < 0) throw invalid_argument("arith_sum: n must be non-negative");
    if (n == 0) return Mint(0);
    ll left = n;
    ll right = n - 1;
    if (left % 2 == 0) left /= 2;
    else right /= 2;
    return a * Mint(n) + Mint(d) * Mint(left) * Mint(right);
}
#endif

namespace ac_detail {
// 1 + r + ... + r^{n-1} と r^n を同時に返す。
inline pair<i128, i128> geom_pow_sum(i128 r, ll n) {
    if (n == 0) return {0, 1};
    if (n % 2 == 0) {
        const auto [s, p] = geom_pow_sum(r, n / 2);
        const i128 one_plus_p = checked_add_i128(1, p, "geom_sum");
        return {checked_mul_i128(s, one_plus_p, "geom_sum"),
                checked_mul_i128(p, p, "geom_sum")};
    }
    const auto [s, p] = geom_pow_sum(r, n - 1);
    return {checked_add_i128(s, p, "geom_sum"),
            checked_mul_i128(p, r, "geom_sum")};
}


#ifdef AC_HAS_ACL
template <class Mint, enable_if_t<io::is_modint_v<Mint>, int> = 0>
pair<Mint, Mint> geom_pow_sum(Mint r, ll n) {
    if (n == 0) return {Mint(0), Mint(1)};
    const auto [half_sum, half_power] = geom_pow_sum(r, n / 2);
    const Mint even_sum = half_sum * (Mint(1) + half_power);
    const Mint even_power = half_power * half_power;
    if (n % 2 == 0) return {even_sum, even_power};
    return {even_sum + even_power, even_power * r};
}
#endif
}  // namespace ac_detail

// @acf fn geom_sum — (a, r, n) -> ll / modint — 等比数列和

/// 等比数列の先頭n項の和を求める。
///
/// 引数 `a`: 初項。
///
/// 引数 `r`: 公比。Mint版では整数または同じMint型。
///
/// 引数 `n`: 0以上の項数。
///
/// 戻り値: 整数版はll、Mint版は同じMint型。n=0は0。n<0はinvalid_argument。整数版で結果がll、または中間計算がi128を超えるとoverflow_error。
///
/// 注意: 入力は値渡し。整数版は広い中間計算を検査。Mint版は同じ法で計算。除算を使わず二分して計算します。
///
/// 呼び分け: geom_sum(ll,ll,ll) / geom_sum(Mint,integer-or-Mint,ll)。Mint版にはACLが必要。
inline ll geom_sum(ll a, ll r, ll n) {
    if (n < 0) throw invalid_argument("geom_sum: n must be non-negative");
    if (n == 0 || a == 0) return 0;
    const i128 series = ac_detail::geom_pow_sum(r, n).first;
    return ac_detail::checked_ll(
        ac_detail::checked_mul_i128(a, series, "geom_sum"), "geom_sum");
}

#ifdef AC_HAS_ACL
/// 等比数列の先頭n項の和を求める。
///
/// 引数 `a`: 初項。
///
/// 引数 `r`: 公比。Mint版では整数または同じMint型。
///
/// 引数 `n`: 0以上の項数。
///
/// 戻り値: 整数版はll、Mint版は同じMint型。n=0は0。n<0はinvalid_argument。整数版で結果がll、または中間計算がi128を超えるとoverflow_error。
///
/// 注意: 入力は値渡し。整数版は広い中間計算を検査。Mint版は同じ法で計算。除算を使わず二分して計算します。
///
/// 呼び分け: geom_sum(ll,ll,ll) / geom_sum(Mint,integer-or-Mint,ll)。Mint版にはACLが必要。
template <class Mint, class Ratio,
          enable_if_t<
              ac_detail::io::is_modint_v<Mint> &&
                  (is_integral_v<remove_cvref_t<Ratio>> ||
                   is_same_v<remove_cvref_t<Ratio>, remove_cvref_t<Mint>>),
              int> = 0>
Mint geom_sum(Mint a, Ratio r, ll n) {
    if (n < 0) throw invalid_argument("geom_sum: n must be non-negative");
    if (n == 0 || a == Mint(0)) return Mint(0);
    return a * ac_detail::geom_pow_sum(Mint(r), n).first;
}
#endif

// @acf fn sum_sq — (ll n) -> ll / Mint — 1^2+...+n^2

/// 1からnまでの2乗の和を求める。
///
/// 引数 `n`: 0以上の上端。
///
/// 引数 `Mint`: 指定すると法の下の値を返す別オーバーロード。
///
/// 戻り値: 通常はll、Mint指定時はMint。n=0は0、n<0はinvalid_argument。通常版はll結果/i128中間値の超過をoverflow_errorで検出。
///
/// 注意: 入力を変更しません。Mint版も逆元に依存せず因子を約分するので合成数法で使用できます。
///
/// 呼び分け: sum_sq(n) / `sum_sq<Mint>`(n)。
///
/// 例: `print(sum_sq(3),sum_sq<mint998244353>(3).val());`
inline ll sum_sq(ll n) {
    if (n < 0) throw invalid_argument("sum_sq: n must be non-negative");
    i128 factors[3] = {i128(n), i128(n) + 1, i128(2) * n + 1};
    for (i128 divisor : {i128(2), i128(3)}) {
        for (auto& factor : factors) {
            if (factor % divisor == 0) {
                factor /= divisor;
                break;
            }
        }
    }
    const i128 first = ac_detail::checked_mul_i128(factors[0], factors[1], "sum_sq");
    return ac_detail::checked_ll(
        ac_detail::checked_mul_i128(first, factors[2], "sum_sq"), "sum_sq");
}

#ifdef AC_HAS_ACL
/// 1からnまでの2乗の和を求める。
///
/// 引数 `n`: 0以上の上端。
///
/// 引数 `Mint`: 指定すると法の下の値を返す別オーバーロード。
///
/// 戻り値: 通常はll、Mint指定時はMint。n=0は0、n<0はinvalid_argument。通常版はll結果/i128中間値の超過をoverflow_errorで検出。
///
/// 注意: 入力を変更しません。Mint版も逆元に依存せず因子を約分するので合成数法で使用できます。
///
/// 呼び分け: sum_sq(n) / `sum_sq<Mint>`(n)。
///
/// 例: `print(sum_sq(3),sum_sq<mint998244353>(3).val());`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint sum_sq(ll n) {
    if (n < 0) throw invalid_argument("sum_sq: n must be non-negative");
    ull factors[3] = {static_cast<ull>(n), static_cast<ull>(n) + 1,
                      static_cast<ull>(n) * 2 + 1};
    for (ull divisor : {2ULL, 3ULL}) {
        for (auto& factor : factors) {
            if (factor % divisor == 0) {
                factor /= divisor;
                break;
            }
        }
    }
    return Mint(factors[0]) * Mint(factors[1]) * Mint(factors[2]);
}
#endif

// @acf fn sum_cu — (ll n) -> ll / Mint — 1^3+...+n^3

/// 1からnまでの3乗の和を求める。
///
/// 引数 `n`: 0以上の上端。
///
/// 引数 `Mint`: 指定すると法の下の値を返す別オーバーロード。
///
/// 戻り値: 通常はll、Mint指定時はMint。n=0は0、n<0はinvalid_argument。通常版はll結果/i128中間値の超過をoverflow_errorで検出。
///
/// 注意: 入力を変更しません。Mint版も逆元に依存せず因子を約分するので合成数法で使用できます。
///
/// 呼び分け: sum_cu(n) / `sum_cu<Mint>`(n)。
///
/// 例: `print(sum_cu(3),sum_cu<mint998244353>(3).val());`
inline ll sum_cu(ll n) {
    if (n < 0) throw invalid_argument("sum_cu: n must be non-negative");
    i128 left = n;
    i128 right = i128(n) + 1;
    if (left % 2 == 0) left /= 2;
    else right /= 2;
    const i128 half = ac_detail::checked_mul_i128(left, right, "sum_cu");
    return ac_detail::checked_ll(
        ac_detail::checked_mul_i128(half, half, "sum_cu"), "sum_cu");
}

#ifdef AC_HAS_ACL
/// 1からnまでの3乗の和を求める。
///
/// 引数 `n`: 0以上の上端。
///
/// 引数 `Mint`: 指定すると法の下の値を返す別オーバーロード。
///
/// 戻り値: 通常はll、Mint指定時はMint。n=0は0、n<0はinvalid_argument。通常版はll結果/i128中間値の超過をoverflow_errorで検出。
///
/// 注意: 入力を変更しません。Mint版も逆元に依存せず因子を約分するので合成数法で使用できます。
///
/// 呼び分け: sum_cu(n) / `sum_cu<Mint>`(n)。
///
/// 例: `print(sum_cu(3),sum_cu<mint998244353>(3).val());`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint sum_cu(ll n) {
    if (n < 0) throw invalid_argument("sum_cu: n must be non-negative");
    ull left = static_cast<ull>(n);
    ull right = left + 1;
    if (left % 2 == 0) left /= 2;
    else right /= 2;
    const Mint half = Mint(left) * Mint(right);
    return half * half;
}
#endif

namespace ac_detail::prime_detail {
inline ull mul_mod_u64(ull a, ull b, ull mod) {
    return static_cast<ull>(static_cast<u128>(a) * b % mod);
}

inline ull pow_mod_u64(ull a, ull n, ull mod) {
    ull result = 1;
    while (n > 0) {
        if (n & 1) result = mul_mod_u64(result, a, mod);
        a = mul_mod_u64(a, a, mod);
        n >>= 1;
    }
    return result;
}

inline bool is_prime_u64(ull n) {
    if (n < 2) return false;
    for (ull p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
                  23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }

    ull d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    // 64bit 整数全域で決定的になる Miller-Rabin の底。
    for (ull a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
                  9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        ull x = pow_mod_u64(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_u64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

inline ull splitmix64(ull& state) {
    ull z = (state += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

inline ull rho_step(ull x, ull c, ull mod) {
    return static_cast<ull>((static_cast<u128>(x) * x + c) % mod);
}

inline ull abs_diff(ull a, ull b) {
    return a >= b ? a - b : b - a;
}

// Pollard's Rho の Brent 版。n は合成数。
inline ull pollard_rho(ull n) {
    for (ull p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
                  23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return p;
    }

    ull state = n ^ 0xd1b54a32d192ed03ULL;
    constexpr ull block_size = 128;
    while (true) {
        ull y = splitmix64(state) % (n - 1) + 1;
        const ull c = splitmix64(state) % (n - 1) + 1;
        ull x = 0, ys = 0;
        ull divisor = 1;
        ull cycle_length = 1;

        while (divisor == 1) {
            x = y;
            for (ull i = 0; i < cycle_length; ++i) y = rho_step(y, c, n);

            for (ull offset = 0; offset < cycle_length && divisor == 1;
                 offset += block_size) {
                ys = y;
                ull product = 1;
                const ull count = min(block_size, cycle_length - offset);
                for (ull i = 0; i < count; ++i) {
                    y = rho_step(y, c, n);
                    product = mul_mod_u64(product, abs_diff(x, y), n);
                }
                divisor = gcd(product, n);
            }
            if (cycle_length > numeric_limits<ull>::max() / 2) break;
            cycle_length <<= 1;
        }

        if (divisor == n) {
            do {
                ys = rho_step(ys, c, n);
                divisor = gcd(abs_diff(x, ys), n);
            } while (divisor == 1);
        }
        if (1 < divisor && divisor < n) return divisor;
    }
}

inline void collect_prime_factors(ull n, vector<ull>& factors) {
    if (n < 2) return;
    if (is_prime_u64(n)) {
        factors.push_back(n);
        return;
    }
    const ull divisor = pollard_rho(n);
    collect_prime_factors(divisor, factors);
    collect_prime_factors(n / divisor, factors);
}

template <class T>
inline vector<T> sieve(T n) {
    if (n < 2) return {};
    vector<char> prime(static_cast<size_t>(n) + 1, true);
    prime[0] = prime[1] = false;
    for (T p = 2; p <= n / p; ++p) {
        if (!prime[static_cast<size_t>(p)]) continue;
        for (T x = p * p; x <= n; x += p) {
            prime[static_cast<size_t>(x)] = false;
        }
    }
    vector<T> result;
    for (T x = 2; x <= n; ++x) {
        if (prime[static_cast<size_t>(x)]) result.push_back(x);
    }
    return result;
}
}  // namespace ac_detail::prime_detail

// @acf fn is_prime — (integer n) -> bool — 64bit 整数の高速な素数判定

/// 64ビットまでの整数が素数かを判定する。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 戻り値: bool。負数・0・1はfalse。64ビットの範囲では決定的Miller–Rabinで判定します。
///
/// 注意: 入力を変更しません。
///
/// 呼び分け: 浮動小数点やi128はこのオーバーロードの対象外。連続した全素数にはprimes_up_to。
///
/// 例: `print(is_prime(2),is_prime(9),is_prime(-7));`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline bool is_prime(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 2) return false;
    }
    return ac_detail::prime_detail::is_prime_u64(static_cast<ull>(n));
}

// @acf fn prime_factorize — (integer n) -> vector<pair<ull,int>> — 64bit 整数を高速に素因数分解

/// 64ビット整数を素因数と指数へ分解する。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 戻り値: `vector<pair<ull,int>>`で{素因数,指数}を素因数の昇順に返します。n<2は空。
///
/// 注意: 入力を変更せず、Miller–RabinとPollard–Rhoで分解してソートします。負数の絶対値の分解ではありません。
///
/// 呼び分け: 小さい整数を大量に分解するならsmallest_prime_factor_sieveとprime_factorize_with_sieve。
///
/// 例: `for(auto [prime,count]:prime_factorize(72)) print(prime,count);`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline vector<pair<ull, int>> prime_factorize(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 2) return {};
    }
    const ull value = static_cast<ull>(n);
    if (value < 2) return {};

    vector<ull> factors;
    ac_detail::prime_detail::collect_prime_factors(value, factors);
    sort(factors.begin(), factors.end());

    vector<pair<ull, int>> result;
    for (ull factor : factors) {
        if (result.empty() || result.back().first != factor) {
            result.push_back({factor, 1});
        } else {
            ++result.back().second;
        }
    }
    return result;
}

// @acf fn smallest_prime_factor_sieve — (int n) -> vi — n 以下の各整数の最小素因数

/// n以下の整数の最小素因数表を作る。
///
/// 引数 `n`: 表の上端。n>=2の場合はn+1要素を確保できること。
///
/// 戻り値: vi。n>=2ならspf[x]がxの最小素因数、spf[0]=spf[1]=0。n<2は空。
///
/// 注意: 線形篩。時間・追加メモリO(n)。
///
/// 呼び分け: 表をprime_factorize_with_sieveへ再利用します。
inline vi smallest_prime_factor_sieve(int n) {
    if (n < 2) return {};

    vi smallest_prime(static_cast<size_t>(n) + 1);
    vi primes;
    for (int x = 2; x <= n; ++x) {
        if (smallest_prime[x] == 0) {
            smallest_prime[x] = x;
            primes.push_back(x);
        }
        for (int prime : primes) {
            if (prime > smallest_prime[x] ||
                static_cast<i128>(x) * prime > n) {
                break;
            }
            smallest_prime[x * prime] = prime;
        }
    }
    return smallest_prime;
}

// @acf fn prime_factorize_with_sieve — (int n, vi smallest_prime_factors) -> vector<pair<ull,int>> — SPF表を使う小さい整数の素因数分解

/// 既存の最小素因数表を使って整数を分解する。
///
/// 引数 `n`: 分解する値。
///
/// 引数 `smallest_prime_factors`: nを覆う正確なSPF表。
///
/// 戻り値: `vector<pair<ull,int>>`の{素因数,指数}。n<2は空。表が小さい、または読んだ因子が不正ならinvalid_argument。表の正しさ全体を検証するAPIではありません。
///
/// 注意: nは値渡しで割りながら分解。表は変更しません。
///
/// 呼び分け: prime_factorize_with_sieveは上の引数順で呼び出します。
inline vector<pair<ull, int>> prime_factorize_with_sieve(
    int n, const vi& smallest_prime_factors) {
    if (n < 2) return {};
    if (static_cast<size_t>(n) >= smallest_prime_factors.size()) {
        throw invalid_argument(
            "prime_factorize_with_sieve: SPF table is too small");
    }

    vector<pair<ull, int>> result;
    while (n > 1) {
        const int prime = smallest_prime_factors[n];
        if (prime < 2 || n % prime != 0) {
            throw invalid_argument(
                "prime_factorize_with_sieve: invalid SPF table");
        }
        int exponent = 0;
        while (n > 1 && n % prime == 0) {
            n /= prime;
            ++exponent;
        }
        result.push_back({static_cast<ull>(prime), exponent});
    }
    return result;
}

// @acf fn euler_phi — (integer n) -> ull — オイラーのトーシェント関数

/// n以下でnと互いに素な正整数の個数を求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 戻り値: ull。n=1は1、n<1は0。
///
/// 注意: nを素因数分解し、各異なる素因数へ結果を更新。入力は変更しません。
///
/// 呼び分け: euler_phiは上の引数順で呼び出します。
///
/// 例: `print(euler_phi(12),euler_phi(1),euler_phi(0));`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline ull euler_phi(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 1) return 0;
    }
    const ull value = static_cast<ull>(n);
    if (value < 1) return 0;

    ull result = value;
    for (auto [prime, exponent] : prime_factorize(value)) {
        (void)exponent;
        result = result / prime * (prime - 1);
    }
    return result;
}

// @acf fn multiplicative_order_prime — (ull a, ull p, factors) -> ull — 素数法での乗法位数

/// 素数法でaの累乗が1へ戻る最小の正指数を求める。
///
/// 引数 `a`: 1<=a<p。
///
/// 引数 `p`: 素数。
///
/// 引数 `factors_of_p_minus_one`: p-1の正確な素因数分解。
///
/// 戻り値: ullの乗法位数。p<2、a=0、a>=pはinvalid_argument。素数性と因数表の正しさは利用者が保証します。
///
/// 注意: p-1を候補位数とし、素因数で割れるかを法の累乗で検査。入力表を変更しません。
///
/// 呼び分け: multiplicative_order_primeは上の引数順で呼び出します。
///
/// 例: `print(multiplicative_order_prime(2,7,prime_factorize(6)));`
inline ull multiplicative_order_prime(
    ull a, ull p, const vector<pair<ull, int>>& factors_of_p_minus_one) {
    if (p < 2 || a == 0 || a >= p) {
        throw invalid_argument(
            "multiplicative_order_prime: require p >= 2 and 1 <= a < p");
    }

    ull order = p - 1;
    for (auto [prime, exponent] : factors_of_p_minus_one) {
        for (int i = 0; i < exponent; ++i) {
            if (ac_detail::prime_detail::pow_mod_u64(a, order / prime, p) != 1)
                break;
            order /= prime;
        }
    }
    return order;
}

// @acf fn primitive_root_prime — (ull p, factors) -> ull — 素数法の原始根

/// 素数法の原始根を1つ求める。
///
/// 引数 `p`: 素数。
///
/// 引数 `factors_of_p_minus_one`: p-1の正確な素因数分解。
///
/// 戻り値: ull。p=2は1、p<2はinvalid_argument。候補が見つからない場合はlogic_error。ただし素数性/因数表の検証として使わず、前提を満たします。
///
/// 注意: 候補を2から順に検査します。入力表を変更しません。
///
/// 呼び分け: primitive_root_primeは上の引数順で呼び出します。
///
/// 例: `print(primitive_root_prime(7,prime_factorize(6)));`
inline ull primitive_root_prime(
    ull p, const vector<pair<ull, int>>& factors_of_p_minus_one) {
    if (p < 2) {
        throw invalid_argument("primitive_root_prime: p must be at least 2");
    }
    if (p == 2) return 1;

    for (ull candidate = 2; candidate < p; ++candidate) {
        bool ok = true;
        for (auto [prime, exponent] : factors_of_p_minus_one) {
            (void)exponent;
            if (ac_detail::prime_detail::pow_mod_u64(
                    candidate, (p - 1) / prime, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return candidate;
    }
    throw logic_error("primitive_root_prime: p must be prime");
}

// @acf fn divisors — (integer n / factors) -> vull — 正の約数を昇順で列挙

/// 全ての正の約数を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `vector<ull>`。整数版のn<1は空。空の因数表は整数1を表すので{1}。約数は昇順。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisors(n) / divisors(factors)。
///
/// 例: `print(divisors(12));`
inline vector<ull> divisors(const vector<pair<ull, int>>& factors) {
    vector<ull> result{1};
    for (auto [prime, exponent] : factors) {
        const size_t current_size = result.size();
        ull power = 1;
        for (int e = 1; e <= exponent; ++e) {
            power *= prime;
            for (size_t i = 0; i < current_size; ++i) {
                result.push_back(result[i] * power);
            }
        }
    }
    sort(result.begin(), result.end());
    return result;
}

/// 全ての正の約数を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `vector<ull>`。整数版のn<1は空。空の因数表は整数1を表すので{1}。約数は昇順。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisors(n) / divisors(factors)。
///
/// 例: `print(divisors(12));`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline vector<ull> divisors(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 1) return {};
    }
    const ull value = static_cast<ull>(n);
    if (value < 1) return {};
    return divisors(prime_factorize(value));
}

// @acf fn divisor_count — (integer n / factors) -> ull — 正の約数の個数

/// 正の約数の個数を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `ull`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_count(n) / divisor_count(factors)。
///
/// 例: `print(divisor_count(12));`
inline ull divisor_count(const vector<pair<ull, int>>& factors) {
    ull result = 1;
    for (auto [prime, exponent] : factors) {
        (void)prime;
        result *= static_cast<ull>(exponent + 1);
    }
    return result;
}

/// 正の約数の個数を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `ull`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_count(n) / divisor_count(factors)。
///
/// 例: `print(divisor_count(12));`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline ull divisor_count(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 1) return 0;
    }
    const ull value = static_cast<ull>(n);
    if (value < 1) return 0;
    return divisor_count(prime_factorize(value));
}

// @acf fn divisor_sum — (integer n / factors) -> u128 / Mint — 正の約数の総和

/// 正の約数の和を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `u128 / Mint`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_sum(n) / divisor_sum(factors)。`divisor_sum<Mint>`(n-or-factors)はACL modintで同じ法の結果を返します。
///
/// 例: `print(divisor_sum(12));`
inline u128 divisor_sum(const vector<pair<ull, int>>& factors) {
    u128 result = 1;
    for (auto [prime, exponent] : factors) {
        u128 term = 1;
        u128 power = 1;
        for (int e = 1; e <= exponent; ++e) {
            power *= prime;
            term += power;
        }
        result *= term;
    }
    return result;
}

/// 正の約数の和を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `u128 / Mint`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_sum(n) / divisor_sum(factors)。`divisor_sum<Mint>`(n-or-factors)はACL modintで同じ法の結果を返します。
///
/// 例: `print(divisor_sum(12));`
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline u128 divisor_sum(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 1) return 0;
    }
    const ull value = static_cast<ull>(n);
    if (value < 1) return 0;
    return divisor_sum(prime_factorize(value));
}

#ifdef AC_HAS_ACL
/// 正の約数の和を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `u128 / Mint`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_sum(n) / divisor_sum(factors)。`divisor_sum<Mint>`(n-or-factors)はACL modintで同じ法の結果を返します。
///
/// 例: `print(divisor_sum(12));`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint divisor_sum(const vector<pair<ull, int>>& factors) {
    Mint result = 1;
    for (auto [prime, exponent] : factors) {
        result *= geom_sum(Mint(1), prime, static_cast<ll>(exponent) + 1);
    }
    return result;
}

/// 正の約数の和を素因数分解から求める。
///
/// 引数 `n`: sizeof(T)<=8の整数型。符号付き・符号なしに対応。
///
/// 引数 `factors`: prime_factorizeの正確な結果。異なる素因数と正の指数。積・中間値が戻り型に収まること。
///
/// 戻り値: `u128 / Mint`。整数版のn<1は0。空の因数表は整数1を表すので1。
///
/// 注意: 入力を変更せず、指数の組合せや積を計算。因数表を手で渡すときは正確な分解と、途中の積が型に収まることを保証します。
///
/// 呼び分け: divisor_sum(n) / divisor_sum(factors)。`divisor_sum<Mint>`(n-or-factors)はACL modintで同じ法の結果を返します。
///
/// 例: `print(divisor_sum(12));`
template <class Mint, class T,
          enable_if_t<ac_detail::io::is_modint_v<Mint> &&
                          is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8),
                      int> = 0>
Mint divisor_sum(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 1) return Mint(0);
    }
    const ull value = static_cast<ull>(n);
    if (value < 1) return Mint(0);
    return divisor_sum<Mint>(prime_factorize(value));
}
#endif

// @acf fn primes_up_to — (int n) -> vi — n 以下の素数をふるいで列挙

/// n以下の素数をふるいで求める。
///
/// 引数 `n`: 上端。n+1規模の表を確保できること。
///
/// 戻り値: vi。素数を昇順。n<2は空。
///
/// 注意: 新しい表を確保。エラトステネスの篩。
///
/// 呼び分け: 1値の判定・分解はis_prime/prime_factorize。
///
/// 例: `print(primes_up_to(10));`
inline vi primes_up_to(int n) {
    return ac_detail::prime_detail::sieve(n);
}

// @acf fn mobius_sieve — (int n) -> vi — n 以下のメビウス関数をふるいで列挙

/// 各整数のメビウス関数をふるいで求める。
///
/// 引数 `n`: 上端。n+1規模の表を確保できること。
///
/// 戻り値: vi。添字0は0、添字1は1、各xに関数値。n<1は空。
///
/// 注意: 新しい表を確保。線形篩で時間・追加メモリO(n)。
///
/// 呼び分け: 1値の判定・分解はis_prime/prime_factorize。
///
/// 例: `print(mobius_sieve(4));`
inline vi mobius_sieve(int n) {
    if (n < 1) return {};

    vi mu(static_cast<size_t>(n) + 1);
    vi least_prime(static_cast<size_t>(n) + 1);
    vi primes;
    mu[1] = 1;
    for (int x = 2; x <= n; ++x) {
        if (least_prime[x] == 0) {
            least_prime[x] = x;
            primes.push_back(x);
            mu[x] = -1;
        }
        for (int p : primes) {
            if (p > least_prime[x] || static_cast<i128>(x) * p > n)
                break;
            const int y = x * p;
            least_prime[y] = p;
            if (x % p == 0) {
                mu[y] = 0;
                break;
            }
            mu[y] = -mu[x];
        }
    }
    return mu;
}

// @acf fn totient_sieve — (int n) -> vi — n 以下のオイラーのトーシェント関数を線形篩で列挙

/// 各整数のトーシェント関数をふるいで求める。
///
/// 引数 `n`: 上端。n+1規模の表を確保できること。
///
/// 戻り値: vi。添字0は0、添字1は1、各xに関数値。n<1は空。
///
/// 注意: 新しい表を確保。線形篩で時間・追加メモリO(n)。
///
/// 呼び分け: 1値の判定・分解はis_prime/prime_factorize。
///
/// 例: `print(totient_sieve(4));`
inline vi totient_sieve(int n) {
    if (n < 1) return {};

    vi phi(static_cast<size_t>(n) + 1);
    vi least_prime(static_cast<size_t>(n) + 1);
    vi primes;
    phi[1] = 1;
    for (int x = 2; x <= n; ++x) {
        if (least_prime[x] == 0) {
            least_prime[x] = x;
            primes.push_back(x);
            phi[x] = x - 1;
        }
        for (int p : primes) {
            if (p > least_prime[x] || static_cast<i128>(x) * p > n)
                break;
            const int y = x * p;
            least_prime[y] = p;
            if (x % p == 0) {
                phi[y] = phi[x] * p;
                break;
            }
            phi[y] = phi[x] * (p - 1);
        }
    }
    return phi;
}

// @acf fn primes_in_range — (ll left, ll right) -> vll — 区間内の素数を区間ふるいで列挙

/// 閉区間に入る素数を昇順で列挙する。
///
/// 引数 `left`: 含む下端。2未満は2へ切り上げます。
///
/// 引数 `right`: 含む上端。区間幅とsqrt(right)規模の配列を確保できること。
///
/// 戻り値: vll。left>rightまたはright<2は空。両端を含みます。
///
/// 注意: 区間ふるい。入力を変更せず、区間幅とsqrt(right)に応じた表を確保します。
///
/// 呼び分け: primes_in_rangeは上の引数順で呼び出します。
///
/// 例: `print(primes_in_range(10,20));`
inline vll primes_in_range(ll left, ll right) {
    if (left > right || right < 2) return {};
    left = max(2LL, left);

    const i128 width = static_cast<i128>(right) - left + 1;
    assert(width <= static_cast<i128>(numeric_limits<size_t>::max()));
    vector<char> prime(static_cast<size_t>(width), true);

    ll limit = static_cast<ll>(sqrtl(static_cast<long double>(right)));
    while (limit > right / limit) --limit;
    while (limit + 1 <= right / (limit + 1)) ++limit;
    for (ll p : ac_detail::prime_detail::sieve(limit)) {
        i128 start = max(static_cast<i128>(p) * p,
                         ((static_cast<i128>(left) + p - 1) / p) * p);
        for (i128 x = start; x <= right; x += p) {
            prime[static_cast<size_t>(x - left)] = false;
        }
    }

    vll result;
    for (size_t i = 0; i < prime.size(); ++i) {
        if (prime[i]) result.push_back(left + static_cast<ll>(i));
    }
    return result;
}

// @acf fn lcm_bounded — (ll a, ll b, ll limit) -> optional<ll> — 上限を超えたLCMをnulloptにする

/// 最小公倍数を、指定上限に収まる場合だけ返す。
///
/// 引数 `a`: 正の整数。
///
/// 引数 `b`: 正の整数。
///
/// 引数 `limit`: 0以上の許容上限。
///
/// 戻り値: `optional<ll>`。上限以下はLCM、超過はnullopt。a/b<=0またはlimit<0はinvalid_argument。limit=0なら正のLCMは入りません。
///
/// 注意: gcdで割ってから積の前に上限を検査。入力を変更しません。
///
/// 呼び分け: lcm_boundedは上の引数順で呼び出します。
inline optional<ll> lcm_bounded(ll a, ll b, ll limit) {
    if (a <= 0 || b <= 0) {
        throw invalid_argument("lcm_bounded: a and b must be positive");
    }
    if (limit < 0) {
        throw invalid_argument("lcm_bounded: limit must be non-negative");
    }
    const ll reduced = a / gcd(a, b);
    if (reduced > limit / b) return nullopt;
    return reduced * b;
}

// @acf fn ipow — (ll base, ll exp) -> ll — 整数の累乗

/// 法を取らず整数の累乗を求める。
///
/// 引数 `base`: 累乗の底。負数も可。
///
/// 引数 `exp`: 0以上の指数。
///
/// 戻り値: ll。exp=0は1（0^0も1）。exp<0はinvalid_argument。llに収まらない計算はoverflow_error。
///
/// 注意: 繰り返し二乗法でO(log exp)。入力を変更せず、中間の積をi128経由で検査。
///
/// 呼び分け: 法の下で計算する場合はpow_mod。
///
/// 例: `print(ipow(-2,3),ipow(0,0));`
inline ll ipow(ll base, ll exp) {
    if (exp < 0) throw invalid_argument("ipow: exp must be non-negative");
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = ac_detail::checked_ll(
                static_cast<i128>(result) * base, "ipow");
        }
        exp >>= 1;
        if (exp > 0) {
            base = ac_detail::checked_ll(
                static_cast<i128>(base) * base, "ipow");
        }
    }
    return result;
}

// @acf fn nth_root — (ll x, ll n) -> ll — x の整数 n 乗根を切り捨て

/// 非負整数のn乗根を整数へ切り捨てる。
///
/// 引数 `x`: 0以上の値。
///
/// 引数 `n`: 1以上の根の次数。
///
/// 戻り値: ll。floor(x^(1/n))。x=0/1、n=1にも対応。前提違反はassert対象。
///
/// 注意: 浮動小数点に頼らず二分探索/除算で積の超過を避けます。入力を変更しません。
///
/// 呼び分け: nth_rootは上の引数順で呼び出します。
///
/// 例: `print(nth_root(27,3),nth_root(26,3));`
inline ll nth_root(ll x, ll n) {
    assert(x >= 0 && n >= 1);
    if (x <= 1 || n == 1) return x;
    if (n >= numeric_limits<ll>::digits) return 1;

    const auto ok = [x, n](ll base) {
        ll value = 1;
        for (ll i = 0; i < n; ++i) {
            if (base != 0 && value > x / base) return false;
            value *= base;
        }
        return true;
    };

    ll lo = 0;
    ll hi = x;
    while (lo < hi) {
        const ll diff = hi - lo;
        const ll mid = lo + diff / 2 + diff % 2;
        if (ok(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

// @acf fn exact_nth_root — (ll x, ll n) -> ll — x が完全な n 乗なら根、違えば -1

/// 非負整数のn乗根を完全な整数根のときだけ返す。
///
/// 引数 `x`: 0以上の値。
///
/// 引数 `n`: 1以上の根の次数。
///
/// 戻り値: ll。完全なn乗でなければ-1。x=0/1、n=1にも対応。前提違反はassert対象。
///
/// 注意: 浮動小数点に頼らず二分探索/除算で積の超過を避けます。入力を変更しません。
///
/// 呼び分け: exact_nth_rootは上の引数順で呼び出します。
///
/// 例: `print(exact_nth_root(27,3),exact_nth_root(26,3));`
inline ll exact_nth_root(ll x, ll n) {
    const ll root = nth_root(x, n);
    if (x <= 1 || n == 1) return x;
    if (root == 1) return -1;

    ll value = 1;
    for (ll i = 0; i < n; ++i) {
        if (value > x / root) return -1;
        value *= root;
    }
    return value == x ? root : -1;
}

// @acf fn pow_mod — (ll x, ll n, int m) -> ll / (Mint x, ll n) -> Mint — mod 上の累乗

/// 整数またはmodintを、法の下で非負整数乗する。
///
/// 引数 `x`: 累乗の底。整数版は負数も法へ正規化。
///
/// 引数 `n`: 0以上の指数。
///
/// 引数 `m`: 1以上の法。modint版は型の法を使用。
///
/// 戻り値: 整数版はllで[0,m)。modint版は同じMint型。n=0なら1（m=1なら0）。整数版の不正なn/mはassert対象、modint版のn<0はinvalid_argument。
///
/// 注意: 底のコピーを計算し、入力を変更しません。繰り返し二乗法。整数版はACLがあればその公開pow_modを使用、なければヘッダー内実装。
///
/// 呼び分け: pow_mod(x,n,m) / pow_mod(modint,n)。modint版にはACLが必要。
#ifndef AC_HAS_ACL
inline ll pow_mod(ll x, ll n, int m) {
    assert(0 <= n && 1 <= m);
    if (m == 1) return 0;
    ll res = 1;
    x %= m;
    if (x < 0) x += m;
    while (n) {
        if (n & 1) res = mul_mod(res, x, m);
        x = mul_mod(x, x, m);
        n >>= 1;
    }
    return res;
}
#endif

#ifdef AC_HAS_ACL
/// 整数またはmodintを、法の下で非負整数乗する。
///
/// 引数 `x`: 累乗の底。整数版は負数も法へ正規化。
///
/// 引数 `n`: 0以上の指数。
///
/// 引数 `m`: 1以上の法。modint版は型の法を使用。
///
/// 戻り値: 整数版はllで[0,m)。modint版は同じMint型。n=0なら1（m=1なら0）。整数版の不正なn/mはassert対象、modint版のn<0はinvalid_argument。
///
/// 注意: 底のコピーを計算し、入力を変更しません。繰り返し二乗法。整数版はACLがあればその公開pow_modを使用、なければヘッダー内実装。
///
/// 呼び分け: pow_mod(x,n,m) / pow_mod(modint,n)。modint版にはACLが必要。
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint pow_mod(Mint x, ll n) {
    if (n < 0) throw invalid_argument("pow_mod: n must be non-negative");
    return x.pow(n);
}
#endif

// @acf fn extended_gcd — (ll a, ll b, ll& x, ll& y) -> ll — ax+by=gcd(a,b) の係数も返す拡張ユークリッド互除法

/// 最大公約数と、ax+by=gcdを満たす係数を求める。
///
/// 引数 `a`: 第1整数。負数も可。
///
/// 引数 `b`: 第2整数。負数も可。
///
/// 引数 `x`: 係数xの出力先。
///
/// 引数 `y`: 係数yの出力先。2つの係数が必要ならxと別の変数。
///
/// 戻り値: llの非負gcd。(0,0)は0、x=y=0。gcdまたは係数がllに収まらなければoverflow_error。
///
/// 注意: a/bは値渡し。成功時にxとyを書き込みます。広い中間値でユークリッド互除法を計算します。
///
/// 呼び分け: extended_gcdは上の引数順で呼び出します。
inline ll extended_gcd(ll a, ll b, ll& x, ll& y) {
    if (a == 0 && b == 0) {
        x = 0;
        y = 0;
        return 0;
    }

    const i128 max_ll = static_cast<i128>(numeric_limits<ll>::max());
    const i128 abs_a = a < 0 ? -static_cast<i128>(a) : static_cast<i128>(a);
    const i128 abs_b = b < 0 ? -static_cast<i128>(b) : static_cast<i128>(b);

    i128 old_r = abs_a;
    i128 r = abs_b;
    i128 old_x = 1;
    i128 current_x = 0;
    i128 old_y = 0;
    i128 current_y = 1;
    while (r != 0) {
        const i128 quotient = old_r / r;
        const i128 next_r = old_r - quotient * r;
        old_r = r;
        r = next_r;

        const i128 next_x = old_x - quotient * current_x;
        old_x = current_x;
        current_x = next_x;
        const i128 next_y = old_y - quotient * current_y;
        old_y = current_y;
        current_y = next_y;
    }

    if (a < 0) old_x = -old_x;
    if (b < 0) old_y = -old_y;
    if (old_r > max_ll) {
        throw overflow_error("extended_gcd: gcd does not fit in long long");
    }
    if (old_x < static_cast<i128>(numeric_limits<ll>::min()) ||
        old_x > max_ll || old_y < static_cast<i128>(numeric_limits<ll>::min()) ||
        old_y > max_ll) {
        throw overflow_error("extended_gcd: coefficient does not fit in long long");
    }
    x = static_cast<ll>(old_x);
    y = static_cast<ll>(old_y);
    return static_cast<ll>(old_r);
}

// @acf fn inv_mod — (ll x, ll m) -> ll / (Mint x) -> Mint — mod での逆元（値と法が互いに素）

/// 値と法が互いに素な場合の乗法逆元を求める。
///
/// 引数 `x`: 逆元を求める値。
///
/// 引数 `m`: 1以上の法。gcd(x,m)=1が必要。
///
/// 戻り値: 整数版はllの非負代表値。Mint版は同じMint型。整数版の前提違反はassert対象、Mint版で互いに素でなければinvalid_argument。
///
/// 注意: 入力を変更しません。ACLがあれば整数版はその公開APIを使用します。
///
/// 呼び分け: inv_mod(x,m) / inv_mod(Mint)。modint版は型の法を使います。0除算相当の逆元は作りません。
#ifndef AC_HAS_ACL
inline ll inv_mod(ll x, ll m) {
    assert(1 <= m);
    ll coefficient = 0;
    ll unused = 0;
    const ll g = extended_gcd(x, m, coefficient, unused);
    assert(g == 1);
    coefficient %= m;
    if (coefficient < 0) coefficient += m;
    return coefficient;
}
#endif

#ifdef AC_HAS_ACL
/// 値と法が互いに素な場合の乗法逆元を求める。
///
/// 引数 `x`: 逆元を求める値。
///
/// 引数 `m`: 1以上の法。gcd(x,m)=1が必要。
///
/// 戻り値: 整数版はllの非負代表値。Mint版は同じMint型。整数版の前提違反はassert対象、Mint版で互いに素でなければinvalid_argument。
///
/// 注意: 入力を変更しません。ACLがあれば整数版はその公開APIを使用します。
///
/// 呼び分け: inv_mod(x,m) / inv_mod(Mint)。modint版は型の法を使います。0除算相当の逆元は作りません。
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint inv_mod(Mint x) {
    if (gcd(static_cast<ll>(x.val()), static_cast<ll>(Mint::mod())) != 1) {
        throw invalid_argument("inv_mod: value and mod must be coprime");
    }
    return x.inv();
}
#endif

// @acf fn nCr — (ll n, ll r) -> ll / Mint — 組合せ（O(r)）

/// 重複なしの組合せの個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。r>nは0、r=0は1。通常版のll結果超過はoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版はnCr内部の縮約後の分母が法と互いに素である必要があり、不成立はinvalid_argument。
///
/// 呼び分け: nCr(n,r) / `nCr<Mint>`(n,r)。大量の同じ法のクエリにはComb系の前計算。
///
/// 例: `print(nCr(5,2),nCr<mint998244353>(5,2).val());`
inline ll nCr(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return 0;
    r = min(r, n - r);
    ll res = 1;
    for (ll i = 1; i <= r; ++i) {
        ll numerator = n - r + i;
        ll denominator = i;
        const ll g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;

        // C(k,i-1) * numerator / denominator は整数。先に割ることで
        // 乗算中間値も最終結果と同じ範囲に抑える。
        if (res % denominator != 0) {
            throw logic_error("nCr: internal division is not exact");
        }
        res /= denominator;
        if (numerator != 0 && res > numeric_limits<ll>::max() / numerator) {
            throw overflow_error("nCr: result does not fit in long long");
        }
        res *= numerator;
    }
    return res;
}

#ifdef AC_HAS_ACL
/// 重複なしの組合せの個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。r>nは0、r=0は1。通常版のll結果超過はoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版はnCr内部の縮約後の分母が法と互いに素である必要があり、不成立はinvalid_argument。
///
/// 呼び分け: nCr(n,r) / `nCr<Mint>`(n,r)。大量の同じ法のクエリにはComb系の前計算。
///
/// 例: `print(nCr(5,2),nCr<mint998244353>(5,2).val());`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint nCr(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return Mint(0);
    r = min(r, n - r);
    Mint result = 1;
    const ll mod = Mint::mod();
    for (ll i = 1; i <= r; ++i) {
        if (gcd(i, mod) != 1) {
            throw invalid_argument("nCr: r! must be invertible modulo Mint::mod()");
        }
        result *= Mint(n - r + i);
        result /= Mint(i);
    }
    return result;
}
#endif

// @acf fn nPr — (ll n, ll r) -> ll / Mint — 順列（O(r)）

/// 順列の個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。r>nは0、r=0は1。通常版のll結果超過はoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版は除算を使いません。
///
/// 呼び分け: nPr(n,r) / `nPr<Mint>`(n,r)。大量の同じ法のクエリにはComb系の前計算。
///
/// 例: `print(nPr(5,2),nPr<mint998244353>(5,2).val());`
inline ll nPr(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return 0;
    ll res = 1;
    for (ll i = 0; i < r; ++i) {
        const ll factor = n - i;
        if (factor != 0 && res > numeric_limits<ll>::max() / factor) {
            throw overflow_error("nPr: result does not fit in long long");
        }
        res *= factor;
    }
    return res;
}

#ifdef AC_HAS_ACL
/// 順列の個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。r>nは0、r=0は1。通常版のll結果超過はoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版は除算を使いません。
///
/// 呼び分け: nPr(n,r) / `nPr<Mint>`(n,r)。大量の同じ法のクエリにはComb系の前計算。
///
/// 例: `print(nPr(5,2),nPr<mint998244353>(5,2).val());`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint nPr(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return Mint(0);
    Mint result = 1;
    for (ll i = 0; i < r; ++i) result *= Mint(n - i);
    return result;
}
#endif

// @acf fn nHr — (ll n, ll r) -> ll / Mint — 重複組合せ H(n,r)=C(n+r-1,r)

/// 重複組合せの個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。n=0,r>0は0、r=0は1。通常版のll結果超過はoverflow_error。n+r-1のll超過もoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版はnCr内部の縮約後の分母が法と互いに素である必要があり、不成立はinvalid_argument。
///
/// 呼び分け: nHr(n,r) / `nHr<Mint>`(n,r)。H(n,r)=C(n+r-1,r)。
///
/// 例: `print(nHr(5,2),nHr<mint998244353>(5,2).val());`
inline ll nHr(ll n, ll r) {
    if (n < 0 || r < 0) return 0;
    if (r == 0) return 1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (r > numeric_limits<ll>::max() - (n - 1)) {
        throw overflow_error("nHr: n + r - 1 does not fit in long long");
    }
    return nCr((n - 1) + r, r);
}

#ifdef AC_HAS_ACL
/// 重複組合せの個数を求める。
///
/// 引数 `n`: 選べる種類/要素数。
///
/// 引数 `r`: 選ぶ個数。
///
/// 引数 `Mint`: 指定時は法の下の値。
///
/// 戻り値: 通常はll、Mint指定はMint。n/r<0は0。n=0,r>0は0、r=0は1。通常版のll結果超過はoverflow_error。n+r-1のll超過もoverflow_error。
///
/// 注意: 入力を変更せず個数分の積を計算。整数版は超過を検査。Mint版はnCr内部の縮約後の分母が法と互いに素である必要があり、不成立はinvalid_argument。
///
/// 呼び分け: nHr(n,r) / `nHr<Mint>`(n,r)。H(n,r)=C(n+r-1,r)。
///
/// 例: `print(nHr(5,2),nHr<mint998244353>(5,2).val());`
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint nHr(ll n, ll r) {
    if (n < 0 || r < 0) return Mint(0);
    if (r == 0) return Mint(1);
    if (n == 0) return Mint(0);
    if (n == 1) return Mint(1);
    if (r > numeric_limits<ll>::max() - (n - 1)) {
        throw overflow_error("nHr: n + r - 1 does not fit in long long");
    }
    return nCr<Mint>((n - 1) + r, r);
}
#endif
// 実行時mod指定の互換用クラス。新規コードは数字付きCombを推奨。
struct Comb {
    int mod = MOD1000000007;
    int max_n = -1;
    vector<ll> fac, ifac;
    /// 未初期化の組合せ前計算器を作る。
    Comb() = default;

    /// 法 `m`、最大 `n` までの組合せ前計算器を構築する。
    explicit Comb(int n, int m = MOD1000000007) { init(n, m); }

    /// 法 `m` で0から `n` までの階乗・逆階乗を前計算する。
    bool init(int n, int m = MOD1000000007) {
        if (n < 0) throw invalid_argument("Comb: max_n must be non-negative");
        if (m < 2 || !is_prime(m)) throw invalid_argument("Comb: mod must be prime");
        if (n >= m) throw invalid_argument("Comb: max_n must be smaller than mod");
        mod = m;
        max_n = n;
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) fac[i] = mul_mod(fac[i - 1], i, mod);
        ifac[n] = pow_mod(fac[n], mod - 2, mod);
        for (int i = n; i > 0; --i) ifac[i - 1] = mul_mod(ifac[i], i, mod);
        return true;
    }
    /// 前計算範囲内の添字か検査する内部メソッド。
    void require_index(ll n, const char* operation) const {
        if (n < 0 || n > max_n) {
            throw out_of_range(string("Comb::") + operation + ": n is outside precomputed range");
        }
    }
    /// `n! mod mod` を返す。
    ll fact(int n) const {
        require_index(n, "fact");
        return fac[n];
    }
    /// `(n!)^{-1} mod mod` を返す。
    ll inv_fact(int n) const {
        require_index(n, "inv_fact");
        return ifac[n];
    }
    /// `n` 個から `r` 個を選ぶ組合せを返す。
    ll C(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        require_index(n, "C");
        return mul_mod(mul_mod(fac[n], ifac[r], mod), ifac[n - r], mod);
    }
    /// `n` 個から `r` 個を選ぶ順列を返す。
    ll P(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        require_index(n, "P");
        return mul_mod(fac[n], ifac[n - r], mod);
    }
    /// 重複組合せ `H(n,r)=C(n+r-1,r)` を返す。
    ll H(ll n, ll r) const {
        if (n < 0 || r < 0) return 0;
        if (r == 0) return 1;
        if (n == 0) return 0;
        if (r > numeric_limits<ll>::max() - (n - 1)) {
            throw overflow_error("Comb::H: n + r - 1 does not fit in long long");
        }
        return C((n - 1) + r, r);
    }
    // カタラン数。前計算上限は 2*n 以上
    /// n番目のカタラン数を返す。`2*n` まで前計算済みである必要がある。
    ll catalan(ll n) const {
        if (n < 0) return 0;
        if (n > max_n / 2) {
            throw out_of_range("Comb::catalan: 2*n is outside precomputed range");
        }
        const ll twice_n = n * 2;
        ll result = C(twice_n, n) - C(twice_n, n + 1);
        if (result < 0) result += mod;
        return result;
    }
};

#ifdef AC_HAS_ACL
namespace ac_detail {
template <class Mint>
struct ModintComb {
    static_assert(io::is_modint_v<Mint>, "Comb requires an ACL modint type");

    int max_n = -1;
    vector<Mint> fac, ifac;

    /// 未初期化のmodint組合せ前計算器を作る。
    ModintComb() = default;

    /// `Mint::mod()` を法として `n` まで前計算する。
    explicit ModintComb(int n) { init(n); }

    /// `Mint::mod()` を法として0から `n` までの階乗を前計算する。
    bool init(int n) {
        const int mod = Mint::mod();
        if (n < 0) throw invalid_argument("Comb: max_n must be non-negative");
        if (mod < 2 || !is_prime(mod)) {
            throw invalid_argument("Comb: Mint::mod() must be prime");
        }
        if (n >= mod) {
            throw invalid_argument("Comb: max_n must be smaller than Mint::mod()");
        }

        max_n = n;
        fac.assign(n + 1, Mint(1));
        ifac.assign(n + 1, Mint(1));
        for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
        ifac[n] = fac[n].inv();
        for (int i = n; i > 0; --i) ifac[i - 1] = ifac[i] * i;
        return true;
    }

    /// 前計算範囲内の添字か検査する内部メソッド。
    void require_index(ll n, const char* operation) const {
        if (n < 0 || n > max_n) {
            throw out_of_range(string("Comb::") + operation +
                               ": n is outside precomputed range");
        }
    }

    /// `n!` を `Mint` で返す。
    Mint fact(int n) const {
        require_index(n, "fact");
        return fac[n];
    }
    /// `(n!)^{-1}` を `Mint` で返す。
    Mint inv_fact(int n) const {
        require_index(n, "inv_fact");
        return ifac[n];
    }
    /// `n` 個から `r` 個を選ぶ組合せを `Mint` で返す。
    Mint C(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return Mint(0);
        require_index(n, "C");
        return fac[n] * ifac[r] * ifac[n - r];
    }
    /// `n` 個から `r` 個を選ぶ順列を `Mint` で返す。
    Mint P(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return Mint(0);
        require_index(n, "P");
        return fac[n] * ifac[n - r];
    }
    /// 重複組合せ `H(n,r)` を `Mint` で返す。
    Mint H(ll n, ll r) const {
        if (n < 0 || r < 0) return Mint(0);
        if (r == 0) return Mint(1);
        if (n == 0) return Mint(0);
        if (r > numeric_limits<ll>::max() - (n - 1)) {
            throw overflow_error("Comb::H: n + r - 1 does not fit in long long");
        }
        return C((n - 1) + r, r);
    }
    /// n番目のカタラン数を `Mint` で返す。
    Mint catalan(ll n) const {
        if (n < 0) return Mint(0);
        if (n > max_n / 2) {
            throw out_of_range("Comb::catalan: 2*n is outside precomputed range");
        }
        const ll twice_n = n * 2;
        return C(twice_n, n) - C(twice_n, n + 1);
    }
};
}  // namespace ac_detail

// @acf using Comb1000000007 — mod 1,000,000,007 の推奨組合せクラス

/// 法 1000000007 での組合せ計算に必要な階乗と逆階乗の表を保持する前計算型です。同じ上限内の C・P・H・カタラン数を繰り返し求めるときに使います。
///
/// 注意: `max_n` は前計算上限、`fac[i]` は i!、`ifac[i]` はその逆元。`init(n)` は `0<=n<1000000007` で表を置き換えます。未初期化の `max_n` は -1。通常の問い合わせは前計算範囲内で使い、範囲外は `out_of_range`。`C`と`P`は`n<0`、`r<0`または`r>n`なら0を返します。`H(n,r)` は n+r-1、`catalan(n)` は2nが上限内に収まるよう前計算します。
using Comb1000000007 = ac_detail::ModintComb<mint1000000007>;
// @acf using Comb998244353 — mod 998,244,353 の推奨組合せクラス

/// 「998244353で割った余り」の組合せに使う。`Comb998244353 comb(max_n); comb.C(n,r)`
using Comb998244353 = ac_detail::ModintComb<mint998244353>;
#endif

// 競プロ向け動的行列。通常の環では演算子を使い、min-plus などは
// matrix_product / 4引数 matrix_power に演算を渡す。
/// 行と列に並んだ数値を保持し、線形変換・行列演算の入力を表す型です。行列積、累乗、連立方程式などへ、要素型を保ったまま渡せます。
template <class T>
struct Matrix {
    /// 行数。
    int rows = 0;
    /// 列数。
    int cols = 0;
    /// 行優先の要素列。
    vector<vector<T>> values;

    /// 0×0の空行列を構築する。
    Matrix() = default;

    /// `row_count`×`column_count` の行列を値 `value` で初期化する。
    Matrix(int row_count, int column_count, const T& value = T{})
        : rows(row_count), cols(column_count) {
        if (rows < 0 || cols < 0) {
            throw invalid_argument("Matrix: dimensions must be non-negative");
        }
        values.assign(static_cast<size_t>(rows),
                      vector<T>(static_cast<size_t>(cols), value));
    }

    /// 行列要素の2次元vectorから構築する。全行の長さは一致させる。
    explicit Matrix(vector<vector<T>> elements)
        : rows(static_cast<int>(elements.size())),
          cols(elements.empty() ? 0 : static_cast<int>(elements.front().size())),
          values(move(elements)) {
        if (!all_of(values.begin(), values.end(), [&](const auto& row) {
                return static_cast<int>(row.size()) == cols;
            })) {
            throw invalid_argument("Matrix: rows must have the same length");
        }
    }

    /// `{{...}, {...}}` 形式の初期化子から構築する。
    Matrix(initializer_list<initializer_list<T>> elements)
        : rows(static_cast<int>(elements.size())),
          cols(elements.size() == 0 ? 0
                                    : static_cast<int>(elements.begin()->size())) {
        values.reserve(static_cast<size_t>(rows));
        for (const auto& row : elements) {
            if (static_cast<int>(row.size()) != cols) {
                throw invalid_argument("Matrix: rows must have the same length");
            }
            values.emplace_back(row);
        }
    }

    /// `size`×`size` の単位行列を返す。
    static Matrix identity(int size) {
        if (size < 0) throw invalid_argument("Matrix::identity: size must be non-negative");
        Matrix result(size, size);
        for (int i = 0; i < size; ++i) result[i][i] = T{1};
        return result;
    }

    /// 行数を返す。
    int row_count() const { return rows; }

    /// 列数を返す。
    int column_count() const { return cols; }

    /// 行数または列数が0か判定する。
    bool empty() const { return rows == 0 || cols == 0; }

    /// 行数（外側vectorの要素数）を返す。
    size_t size() const { return values.size(); }

    /// 行 `row` を範囲チェックなしで返す。
    vector<T>& operator[](int row) { return values[static_cast<size_t>(row)]; }

    /// 行 `row` を範囲チェックなしでconst返却する。
    const vector<T>& operator[](int row) const {
        return values[static_cast<size_t>(row)];
    }

    /// 要素 `(row,col)` を範囲チェックなしで返す。
    decltype(auto) operator()(int row, int col) { return (*this)[row][col]; }

    /// 要素 `(row,col)` を範囲チェックなしでconst返却する。
    decltype(auto) operator()(int row, int col) const { return (*this)[row][col]; }

    /// 要素 `(row,col)` を範囲チェック付きで返す。
    decltype(auto) at(int row, int col) {
        return values.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
    }

    /// 要素 `(row,col)` を範囲チェック付きでconst返却する。
    decltype(auto) at(int row, int col) const {
        return values.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
    }

    /// 行を走査するmutable iteratorの先頭を返す。
    auto begin() { return values.begin(); }

    /// 行を走査するmutable iteratorの末尾を返す。
    auto end() { return values.end(); }

    /// 行を走査するconst iteratorの先頭を返す。
    auto begin() const { return values.begin(); }

    /// 行を走査するconst iteratorの末尾を返す。
    auto end() const { return values.end(); }

    /// 行と列を入れ替えた転置行列を返す。
    Matrix transposed() const {
        Matrix result(cols, rows);
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) result[col][row] = (*this)[row][col];
        return result;
    }

    /// 正方行列の対角成分の和を返す。
    T diagonal_sum() const {
        if (rows != cols) {
            throw invalid_argument("Matrix::diagonal_sum: matrix must be square");
        }
        T result{};
        for (int i = 0; i < rows; ++i) result += (*this)[i][i];
        return result;
    }

    /// 同じ形の行列を加算して自身を更新する。
    Matrix& operator+=(const Matrix& rhs) {
        require_same_shape(rhs, "operator+=");
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) (*this)[row][col] += rhs[row][col];
        return *this;
    }

    /// 同じ形の行列を減算して自身を更新する。
    Matrix& operator-=(const Matrix& rhs) {
        require_same_shape(rhs, "operator-=");
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) (*this)[row][col] -= rhs[row][col];
        return *this;
    }

    /// 行列積で自身を更新する。
    Matrix& operator*=(const Matrix& rhs) {
        *this = *this * rhs;
        return *this;
    }

    /// 全要素をスカラー倍して自身を更新する。
    Matrix& operator*=(const T& scalar) {
        for (auto& row : values)
            for (auto& value : row) value *= scalar;
        return *this;
    }

    /// 全要素をスカラーで割って自身を更新する。
    Matrix& operator/=(const T& scalar) {
        for (auto& row : values)
            for (auto& value : row) value /= scalar;
        return *this;
    }

    /// 全要素の符号を反転した行列を返す。
    Matrix operator-() const {
        Matrix result(*this);
        for (auto& row : result.values)
            for (auto& value : row) value = -value;
        return result;
    }

    /// 行列を非負整数 `exponent` 乗した結果を返す。
    Matrix pow(ull exponent) const {
        if (rows != cols) throw invalid_argument("Matrix::pow: matrix must be square");
        Matrix base(*this);
        Matrix result = identity(rows);
        while (exponent > 0) {
            if (exponent & 1ULL) result *= base;
            exponent >>= 1;
            if (exponent > 0) base *= base;
        }
        return result;
    }

    /// 2つの行列の和を返す。
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) { return lhs += rhs; }

    /// 2つの行列の差を返す。
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) { return lhs -= rhs; }

    /// 2つの行列の積を返す。
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
        if (lhs.cols != rhs.rows) {
            throw invalid_argument("Matrix::operator*: incompatible dimensions");
        }
        Matrix result(lhs.rows, rhs.cols);
        for (int row = 0; row < lhs.rows; ++row) {
            for (int mid = 0; mid < lhs.cols; ++mid) {
                const T left = lhs[row][mid];
                for (int col = 0; col < rhs.cols; ++col) {
                    result[row][col] += left * rhs[mid][col];
                }
            }
        }
        return result;
    }

    /// 行列とvectorの積を返す。
    friend vector<T> operator*(const Matrix& lhs, const vector<T>& rhs) {
        if (lhs.cols != static_cast<int>(rhs.size())) {
            throw invalid_argument("Matrix::operator*: incompatible vector size");
        }
        vector<T> result(static_cast<size_t>(lhs.rows), T{});
        for (int row = 0; row < lhs.rows; ++row)
            for (int col = 0; col < lhs.cols; ++col)
                result[static_cast<size_t>(row)] += lhs[row][col] * rhs[static_cast<size_t>(col)];
        return result;
    }

    /// 行列をスカラー倍した結果を返す。
    friend Matrix operator*(Matrix matrix, const T& scalar) { return matrix *= scalar; }

    /// スカラー倍の左右を入れ替えた結果を返す。
    friend Matrix operator*(const T& scalar, Matrix matrix) { return matrix *= scalar; }

    /// 行列をスカラーで割った結果を返す。
    friend Matrix operator/(Matrix matrix, const T& scalar) { return matrix /= scalar; }

    /// 形と全要素が一致するか判定する。
    friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
        return lhs.rows == rhs.rows && lhs.cols == rhs.cols && lhs.values == rhs.values;
    }

   private:
    /// 二項演算の前提となる行列形状の一致を検査する。
    void require_same_shape(const Matrix& rhs, const char* operation) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw invalid_argument(string("Matrix::") + operation +
                                   ": matrix shapes must match");
        }
    }
};

// @acf fn matrix_trace — (const Matrix<T>& matrix) -> T — 対角和（トレース）

/// 正方行列の対角成分の和を求める。
///
/// 引数 `matrix`: 正方行列。和がTの範囲に収まること。
///
/// 戻り値: T。0×0行列はT{}。非正方はinvalid_argument。
///
/// 注意: 入力を変更せず対角を走査。時間O(N)。
///
/// 呼び分け: traceはデバッグマクロ名なので、行列のトレースはmatrix_traceで呼びます。
template <class T>
T matrix_trace(const Matrix<T>& matrix) {
    return matrix.diagonal_sum();
}

// @acf fn matrix_product — (A,B[,zero,add,multiply]) -> Matrix<T> — 通常積または任意半環の行列積

/// 通常の積、または指定した半環の行列積を求める。
///
/// 引数 `lhs`: H×Kの左行列。
///
/// 引数 `rhs`: K×Wの右行列。
///
/// 引数 `zero`: 加算の単位元として初期化する値。省略時は `T{}`。
///
/// 引数 `add`: 部分積を足し合わせる操作。省略時は `{}（plus<T>）`。
///
/// 引数 `multiply`: 2要素の積の操作。省略時は `{}（multiplies<T>）`。
///
/// 戻り値: H×Wの`Matrix<T>`。lhs.cols!=rhs.rowsはinvalid_argument。K=0は全要素zero。操作と中間値がTで成立すること。
///
/// 注意: 入力を変更せず出力行列を確保。時間O(HKW)、出力メモリO(HW)（操作の費用は別）。callbackの副作用は利用者側の責任です。
///
/// 呼び分け: matrix_product(a,b) / matrix_product(a,b,zero,add,multiply)。min-plusなどではzeroと操作の整合性を呼び出し側が保証します。
template <class T, class Add = plus<T>, class Multiply = multiplies<T>>
Matrix<T> matrix_product(const Matrix<T>& lhs, const Matrix<T>& rhs,
                         T zero = T{}, Add add = {}, Multiply multiply = {}) {
    if (lhs.cols != rhs.rows) {
        throw invalid_argument("matrix_product: incompatible dimensions");
    }
    Matrix<T> result(lhs.rows, rhs.cols, zero);
    for (int row = 0; row < lhs.rows; ++row)
        for (int mid = 0; mid < lhs.cols; ++mid)
            for (int col = 0; col < rhs.cols; ++col)
                result[row][col] = add(result[row][col],
                                       multiply(lhs[row][mid], rhs[mid][col]));
    return result;
}

// @acf fn matrix_power — (Matrix<T> base, ull exponent) -> Matrix<T> — 行列累乗

/// 正方行列を非負整数乗する。
///
/// 引数 `base`: 累乗する正方行列のコピー。
///
/// 引数 `exponent`: 非負整数指数。負の値を渡すと符号なしへ変換されるので渡しません。
///
/// 引数 `identity`: baseと同じサイズの、multiplyに対する単位行列。
///
/// 引数 `multiply`: 行列2つを受け取り、その積の行列を返すcallable。
///
/// 戻り値: `Matrix<T>`。指数0は単位行列（4引数版は渡したidentity）。非正方やidentityのサイズ不一致はinvalid_argument。0×0にも対応。
///
/// 注意: 内部のbase/identityだけを更新。通常積は時間O(N^3 log exponent)、追加メモリO(N^2)。Tで中間積が表せること。
///
/// 呼び分け: matrix_power(base,exponent) / matrix_power(base,exponent,identity,multiply)。半環版は積に合うidentityを明示します。
template <class T>
Matrix<T> matrix_power(Matrix<T> base, ull exponent) {
    return base.pow(exponent);
}

// 任意半環向け。identity と積を渡せば、通常の +/* を持たない遷移にも使える。
/// 正方行列を非負整数乗する。
///
/// 引数 `base`: 累乗する正方行列のコピー。
///
/// 引数 `exponent`: 非負整数指数。負の値を渡すと符号なしへ変換されるので渡しません。
///
/// 引数 `identity`: baseと同じサイズの、multiplyに対する単位行列。
///
/// 引数 `multiply`: 行列2つを受け取り、その積の行列を返すcallable。
///
/// 戻り値: `Matrix<T>`。指数0は単位行列（4引数版は渡したidentity）。非正方やidentityのサイズ不一致はinvalid_argument。0×0にも対応。
///
/// 注意: 内部のbase/identityだけを更新。通常積は時間O(N^3 log exponent)、追加メモリO(N^2)。Tで中間積が表せること。
///
/// 呼び分け: matrix_power(base,exponent) / matrix_power(base,exponent,identity,multiply)。半環版は積に合うidentityを明示します。
template <class T, class Multiply>
Matrix<T> matrix_power(Matrix<T> base, ull exponent, Matrix<T> identity,
                       Multiply multiply) {
    if (base.rows != base.cols || identity.rows != base.rows ||
        identity.cols != base.cols) {
        throw invalid_argument("matrix_power: base and identity must be equally sized squares");
    }
    while (exponent > 0) {
        if (exponent & 1ULL) identity = multiply(identity, base);
        exponent >>= 1;
        if (exponent > 0) base = multiply(base, base);
    }
    return identity;
}

// @acf fn matrix_power_sum — (Matrix<T> base, ull exponent) -> Matrix<T> — I+A+...+A^(exponent-1)

/// IからA^(exponent-1)までの行列累乗和を求める。
///
/// 引数 `base`: 通常の加算/積が成立する正方行列のコピー。
///
/// 引数 `exponent`: 加える項数。指数exponent自身の項は含めません。
///
/// 戻り値: `Matrix<T>`。exponent=0は零行列、1は単位行列。非正方はinvalid_argument。
///
/// 注意: 入力を変更せず二倍化して計算。時間O(N^3 log exponent)、追加メモリO(N^2)。途中の和と積もTに収まること。
///
/// 呼び分け: 単一のA^nはmatrix_power。累乗和は項数を渡します。
template <class T>
Matrix<T> matrix_power_sum(Matrix<T> base, ull exponent) {
    if (base.rows != base.cols) {
        throw invalid_argument("matrix_power_sum: matrix must be square");
    }
    Matrix<T> result_power = Matrix<T>::identity(base.rows);
    Matrix<T> result_sum(base.rows, base.cols);
    Matrix<T> block_power = base;
    Matrix<T> block_sum = Matrix<T>::identity(base.rows);
    while (exponent > 0) {
        if (exponent & 1ULL) {
            result_sum += result_power * block_sum;
            result_power *= block_power;
        }
        exponent >>= 1;
        if (exponent > 0) {
            block_sum += block_power * block_sum;
            block_power *= block_power;
        }
    }
    return result_sum;
}

namespace ac_detail::matrix {
template <class T>
bool is_zero(const T& value, ld eps) {
    if constexpr (is_floating_point_v<remove_cv_t<T>>) {
        return fabsl(static_cast<ld>(value)) <= eps;
    } else {
        return value == T{};
    }
}

template <class T>
int pivot_row(const Matrix<T>& matrix, int first_row, int column, ld eps) {
    int pivot = -1;
    if constexpr (is_floating_point_v<remove_cv_t<T>>) {
        ld best = eps;
        for (int row = first_row; row < matrix.rows; ++row) {
            const ld magnitude = fabsl(static_cast<ld>(matrix[row][column]));
            if (magnitude > best) best = magnitude, pivot = row;
        }
    } else {
        for (int row = first_row; row < matrix.rows; ++row) {
            if (!is_zero(matrix[row][column], eps)) return row;
        }
    }
    return pivot;
}
}  // namespace ac_detail::matrix

/// 行列を行簡約階段形へ変形した結果と、独立な列の数・位置を保持する型です。階数や、拡大係数行列から解の情報を取り出すときに使います。
template <class T>
struct GaussJordanResult {
    /// 行簡約階段形へ変形した行列。
    Matrix<T> reduced;
    /// pivot の個数。
    int rank = 0;
    /// pivot 列を行順に並べたもの。
    vi pivot_columns;
};

// @acf fn gauss_jordan — (Matrix<T> matrix[,coefficient_columns,eps]) -> GaussJordanResult<T> — 掃き出し法

/// 体上の行列を掃き出し、階数とpivot列を求める。
///
/// 引数 `matrix`: 実数・modintなど除算が成立する体上の行列のコピー。整数型は拒否。
///
/// 引数 `coefficient_columns`: pivotを探す先頭の係数列数。0..matrix.cols。省略時は `-1（全列）`。
///
/// 引数 `eps`: 有限で非負の絶対ゼロ判定誤差。非実数の体では厳密な0判定。省略時は `EPS`。
///
/// 戻り値: `GaussJordanResult<T>`。reducedは簡約化した行列、rankはpivot数、pivot_columnsは0-indexのpivot列。0行/係数0列ならrank=0。負epsや範囲外の列数はinvalid_argument。
///
/// 注意: 元の行列は変更せずコピーを操作。全列へ行操作を適用し、coefficient_columnsはpivot探索だけを制限。実数は大きい絶対値のpivotを選びます。
///
/// 呼び分け: 拡大係数行列はcoefficient_columnsに変数数を指定。結果の意味・不変条件はGaussJordanResult。
template <class T>
GaussJordanResult<T> gauss_jordan(Matrix<T> matrix,
                                  int coefficient_columns = -1, ld eps = EPS) {
    static_assert(!is_integral_v<remove_cv_t<T>>,
                  "gauss_jordan requires floating point, modint, or another field type");
    if (eps < 0) throw invalid_argument("gauss_jordan: eps must be non-negative");
    if (coefficient_columns == -1) coefficient_columns = matrix.cols;
    if (coefficient_columns < 0 || coefficient_columns > matrix.cols) {
        throw invalid_argument("gauss_jordan: coefficient_columns is out of range");
    }

    GaussJordanResult<T> result{move(matrix), 0, {}};
    for (int col = 0;
         col < coefficient_columns && (result.rank) < result.reduced.rows;
         ++col) {
        const int pivot = ac_detail::matrix::pivot_row(
            result.reduced, result.rank, col, eps);
        if (pivot < 0) continue;
        swap(result.reduced[pivot], result.reduced[result.rank]);

        const T divisor = result.reduced[result.rank][col];
        for (int j = 0; j < result.reduced.cols; ++j)
            result.reduced[result.rank][j] /= divisor;

        for (int row = 0; row < result.reduced.rows; ++row) {
            if (row == result.rank) continue;
            const T factor = result.reduced[row][col];
            if (ac_detail::matrix::is_zero(factor, eps)) continue;
            for (int j = 0; j < result.reduced.cols; ++j) {
                result.reduced[row][j] -= factor * result.reduced[result.rank][j];
                if constexpr (is_floating_point_v<remove_cv_t<T>>) {
                    if (ac_detail::matrix::is_zero(result.reduced[row][j], eps))
                        result.reduced[row][j] = T{};
                }
            }
        }
        result.pivot_columns.push_back(col);
        ++result.rank;
    }
    return result;
}

// @acf fn matrix_rank — (Matrix<T> matrix[,eps]) -> int — 行列の階数

/// 行列を体上で見たときの階数を求める。
///
/// 引数 `matrix`: 行列のコピー。実数/体型、または符号付き整数。
///
/// 引数 `eps`: 有限で非負の絶対ゼロ判定誤差。非実数の体では厳密な0判定。省略時は `EPS`。
///
/// 戻り値: intのrank。空行/列は0。非整数版は負epsをinvalid_argumentで拒否。整数版ではepsを使用せず、無符号整数行列はコンパイル時に拒否。
///
/// 注意: 元の行列は変更せずコピーを操作。整数版は分数なしの消去で、途中の積と差がTの範囲に収まることが必要です。
///
/// 呼び分け: 非整数版はgauss_jordanのrank。整数版は有理数体上の階数を求めます。
template <class T>
int matrix_rank(Matrix<T> matrix, ld eps = EPS) {
    if constexpr (!is_integral_v<remove_cv_t<T>>) {
        return gauss_jordan(move(matrix), matrix.cols, eps).rank;
    } else {
        static_assert(is_signed_v<remove_cv_t<T>>,
                      "matrix_rank does not support unsigned integer matrices");
        int rank = 0;
        for (int col = 0; col < matrix.cols && rank < matrix.rows; ++col) {
            int pivot = rank;
            while (pivot < matrix.rows && matrix[pivot][col] == T{}) ++pivot;
            if (pivot == matrix.rows) continue;
            swap(matrix[pivot], matrix[rank]);
            for (int row = rank + 1; row < matrix.rows; ++row) {
                if (matrix[row][col] == T{}) continue;
                const T left = matrix[rank][col];
                const T right = matrix[row][col];
                for (int j = col + 1; j < matrix.cols; ++j)
                    matrix[row][j] = left * matrix[row][j] - right * matrix[rank][j];
                matrix[row][col] = T{};
            }
            ++rank;
        }
        return rank;
    }
}

// @acf fn matrix_determinant — (Matrix<T> matrix[,eps]) -> T — 正方行列の行列式

/// 正方行列の行列式を求める。
///
/// 引数 `matrix`: 正方行列のコピー。実数/体型、または符号付き整数。
///
/// 引数 `eps`: 有限で非負の絶対ゼロ判定誤差。非実数の体では厳密な0判定。省略時は `EPS`。
///
/// 戻り値: T。特異行列は0、0×0は1。非正方/負epsはinvalid_argument。無符号整数型は拒否。
///
/// 注意: コピーを消去。整数はBareiss法、体はGaussian elimination。時間O(N^3)。整数の中間積・差はTで表せる必要があり、超過検出は行いません。
///
/// 呼び分け: 整数版はepsを計算に使いませんが、負epsの引数検査は共通です。
template <class T>
T matrix_determinant(Matrix<T> matrix, ld eps = EPS) {
    if (matrix.rows != matrix.cols) {
        throw invalid_argument("matrix_determinant: matrix must be square");
    }
    if (eps < 0) throw invalid_argument("matrix_determinant: eps must be non-negative");
    const int n = matrix.rows;
    if (n == 0) return T{1};

    if constexpr (is_integral_v<remove_cv_t<T>>) {
        static_assert(is_signed_v<remove_cv_t<T>>,
                      "matrix_determinant does not support unsigned integer matrices");
        T previous = T{1};
        int sign = 1;
        for (int col = 0; col + 1 < n; ++col) {
            int pivot = col;
            while (pivot < n && matrix[pivot][col] == T{}) ++pivot;
            if (pivot == n) return T{};
            if (pivot != col) swap(matrix[pivot], matrix[col]), sign = -sign;
            const T pivot_value = matrix[col][col];
            for (int row = col + 1; row < n; ++row) {
                for (int j = col + 1; j < n; ++j) {
                    matrix[row][j] =
                        (matrix[row][j] * pivot_value -
                         matrix[row][col] * matrix[col][j]) /
                        previous;
                }
                matrix[row][col] = T{};
            }
            previous = pivot_value;
        }
        return sign == 1 ? matrix[n - 1][n - 1] : -matrix[n - 1][n - 1];
    } else {
        T determinant{1};
        for (int col = 0; col < n; ++col) {
            const int pivot = ac_detail::matrix::pivot_row(matrix, col, col, eps);
            if (pivot < 0) return T{};
            if (pivot != col) swap(matrix[pivot], matrix[col]), determinant = -determinant;
            const T pivot_value = matrix[col][col];
            determinant *= pivot_value;
            for (int row = col + 1; row < n; ++row) {
                const T factor = matrix[row][col] / pivot_value;
                for (int j = col + 1; j < n; ++j)
                    matrix[row][j] -= factor * matrix[col][j];
            }
        }
        return determinant;
    }
}

// @acf fn matrix_inverse — (Matrix<T> matrix[,eps]) -> optional<Matrix<T>> — 逆行列

/// 正則な体上の正方行列の逆行列を求める。
///
/// 引数 `matrix`: 実数・modintなど体の正方行列。整数型は拒否。
///
/// 引数 `eps`: 有限で非負の絶対ゼロ判定誤差。非実数の体では厳密な0判定。省略時は `EPS`。
///
/// 戻り値: `optional<Matrix<T>>`。正則なら逆行列、特異ならnullopt。0×0は0×0逆行列。非正方/負epsはinvalid_argument。
///
/// 注意: 入力を変更せず[I]との拡大係数行列を作って掃き出します。時間O(N^3)、追加メモリO(N^2)。
///
/// 呼び分け: 実数の正則性はepsによるpivot判定に依存します。体の型では必要な非零pivotが可逆であること。
template <class T>
optional<Matrix<T>> matrix_inverse(const Matrix<T>& matrix, ld eps = EPS) {
    static_assert(!is_integral_v<remove_cv_t<T>>,
                  "matrix_inverse requires floating point, modint, or another field type");
    if (matrix.rows != matrix.cols) {
        throw invalid_argument("matrix_inverse: matrix must be square");
    }
    const int n = matrix.rows;
    Matrix<T> augmented(n, n * 2);
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) augmented[row][col] = matrix[row][col];
        augmented[row][n + row] = T{1};
    }
    auto reduced = gauss_jordan(move(augmented), n, eps);
    if (reduced.rank != n) return nullopt;
    Matrix<T> inverse(n, n);
    for (int row = 0; row < n; ++row)
        for (int col = 0; col < n; ++col)
            inverse[row][col] = reduced.reduced[row][n + col];
    return inverse;
}

/// 連立一次方程式 Ax=b が解を持つかと、そのすべての解を表すための特殊解・零空間基底を保持する結果です。解なし・一意解・自由変数のある解を区別できます。
template <class T>
struct LinearSystemResult {
    /// Ax=b が少なくとも1つ解を持つか。
    bool consistent = false;
    /// 係数行列 A の階数（pivot数）。
    int rank = 0;
    /// 変数添字順の特殊解。自由変数は0。解なしなら空。
    vector<T> particular;
    /// 各`vector`が`Ax=0`の基底ベクトル。内側の添字は変数番号。
    vector<vector<T>> nullspace_basis;

    /// 連立方程式に少なくとも1つ解があるか判定する。
    bool has_solution() const { return consistent; }

    /// 解が一意か判定する。
    bool unique() const { return consistent && nullspace_basis.empty(); }
};

// @acf fn solve_linear_system — (Matrix<T> coefficients, vector<T> rhs[,eps]) -> LinearSystemResult<T> — 連立一次方程式

/// Ax=bの解の有無と、特解・斉次解の基底を求める。
///
/// 引数 `coefficients`: m行n列の係数行列。実数/体型、整数型は拒否。
///
/// 引数 `rhs`: m個の右辺。
///
/// 引数 `eps`: 有限で非負の絶対ゼロ判定誤差。非実数の体では厳密な0判定。省略時は `EPS`。
///
/// 戻り値: `LinearSystemResult<T>`。consistent=falseなら解なし。解ありは自由変数0のparticularとnullspace_basis。rhs長不一致/負epsはinvalid_argument。0式なら全変数が自由。
///
/// 注意: 係数と右辺を変更せず拡大係数行列を作って掃き出します。各解はparticularに基底の任意線形結合を足したもの。
///
/// 呼び分け: 結果の各メンバの意味・無効状態はLinearSystemResult。particularはconsistentを確認してから使用します。
template <class T>
LinearSystemResult<T> solve_linear_system(const Matrix<T>& coefficients,
                                          const vector<T>& rhs, ld eps = EPS) {
    static_assert(!is_integral_v<remove_cv_t<T>>,
                  "solve_linear_system requires floating point, modint, or another field type");
    if (coefficients.rows != static_cast<int>(rhs.size())) {
        throw invalid_argument("solve_linear_system: rhs size must equal row count");
    }
    const int variable_count = coefficients.cols;
    Matrix<T> augmented(coefficients.rows, variable_count + 1);
    for (int row = 0; row < coefficients.rows; ++row) {
        for (int col = 0; col < variable_count; ++col)
            augmented[row][col] = coefficients[row][col];
        augmented[row][variable_count] = rhs[static_cast<size_t>(row)];
    }

    auto reduced = gauss_jordan(move(augmented), variable_count, eps);
    for (int row = reduced.rank; row < reduced.reduced.rows; ++row) {
        if (!ac_detail::matrix::is_zero(reduced.reduced[row][variable_count], eps)) {
            return {false, reduced.rank, {}, {}};
        }
    }

    vector<T> particular(static_cast<size_t>(variable_count), T{});
    vector<char> is_pivot(static_cast<size_t>(variable_count), false);
    for (int row = 0; row < reduced.rank; ++row) {
        const int col = reduced.pivot_columns[static_cast<size_t>(row)];
        is_pivot[static_cast<size_t>(col)] = true;
        particular[static_cast<size_t>(col)] = reduced.reduced[row][variable_count];
    }

    vector<vector<T>> basis;
    for (int free_col = 0; free_col < variable_count; ++free_col) {
        if (is_pivot[static_cast<size_t>(free_col)]) continue;
        vector<T> direction(static_cast<size_t>(variable_count), T{});
        direction[static_cast<size_t>(free_col)] = T{1};
        for (int row = 0; row < reduced.rank; ++row) {
            const int pivot_col = reduced.pivot_columns[static_cast<size_t>(row)];
            direction[static_cast<size_t>(pivot_col)] = -reduced.reduced[row][free_col];
        }
        basis.push_back(move(direction));
    }
    return {true, reduced.rank, move(particular), move(basis)};
}

// @acf fn linear_recurrence_nth — (initial,coefficients,index) -> T — 定数係数線形漸化式の第 index 項

/// 定数係数線形漸化式の0始まり第index項を求める。
///
/// 引数 `initial`: a_0..a_(k-1)のk個の初期値。k>=1。
///
/// 引数 `coefficients`: c[0]a_(n-1)+...+c[k-1]a_(n-k)のk係数。Tへ構築可能。
///
/// 引数 `index`: 求める0始まりの項番号。
///
/// 戻り値: T。index<kなら対応する初期値。初期値が空、または係数と長さ不一致はinvalid_argument。
///
/// 注意: 入力を変更せずKitamasa法。時間O(k^2 log index)、追加メモリO(k)。整数Tの中間積や和が型の範囲外ならoverflow_error。最終値が範囲内でも中間値を検査する。係数の型変換は従来どおり。modintは型の法を使う。
///
/// 呼び分け: 全ての先頭項を生成するrecurrenceとは用途が異なります。遠い1項の計算に使用します。
///
/// 例: `print(linear_recurrence_nth(vll{0,1},vi{1,1},10));`
template <class T, class C>
T linear_recurrence_nth(const vector<T>& initial,
                        const vector<C>& coefficients, ull index) {
    if (initial.empty()) {
        throw invalid_argument("linear_recurrence_nth: initial must not be empty");
    }
    if (initial.size() != coefficients.size()) {
        throw invalid_argument(
            "linear_recurrence_nth: initial and coefficients must have the same size");
    }
    if (index < initial.size()) return initial[static_cast<size_t>(index)];

    const size_t order = initial.size();
    vector<T> recurrence_coefficients;
    recurrence_coefficients.reserve(order);
    for (const auto& coefficient : coefficients) {
        recurrence_coefficients.emplace_back(coefficient);
    }

    const auto accumulate_product = [](T& sum, const T& a, const T& b) {
        if constexpr (is_integral_v<T> && !is_same_v<T, bool>) {
            T product, total;
            if (__builtin_mul_overflow(a, b, &product) ||
                __builtin_add_overflow(sum, product, &total))
                throw overflow_error("linear_recurrence_nth: intermediate overflow");
            sum = total;
        } else {
            sum += a * b;
        }
    };
    const auto combine = [&](const vector<T>& lhs, const vector<T>& rhs) {
        vector<T> product(order * 2 - 1, T{});
        for (size_t i = 0; i < order; ++i) {
            for (size_t j = 0; j < order; ++j) {
                accumulate_product(product[i + j], lhs[i], rhs[j]);
            }
        }
        for (size_t degree = product.size(); degree-- > order;) {
            for (size_t lag = 0; lag < order; ++lag) {
                accumulate_product(product[degree - 1 - lag],
                                   product[degree], recurrence_coefficients[lag]);
            }
        }
        product.resize(order);
        return product;
    };

    vector<T> weights(order, T{});
    weights[0] = T{1};
    vector<T> power_of_x(order, T{});
    if (order == 1) power_of_x[0] = recurrence_coefficients[0];
    else power_of_x[1] = T{1};

    while (index > 0) {
        if (index & 1ULL) weights = combine(weights, power_of_x);
        index >>= 1;
        if (index > 0) power_of_x = combine(power_of_x, power_of_x);
    }

    T result{};
    for (size_t i = 0; i < order; ++i)
        accumulate_product(result, weights[i], initial[i]);
    return result;
}

inline ostream& operator<<(ostream& os, const Comb& comb) {
    return os << "Comb{mod=" << comb.mod
              << ", fac=" << comb.fac
              << ", ifac=" << comb.ifac << '}';
}
