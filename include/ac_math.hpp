#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include "ac.hpp"

// 数論・組合せ・行列。アルゴリズム本体はコアから移動し、仕様は維持する。

// @acf fn arith_sum — (a, d, n) -> ll / modint — 等差数列和
/// @acf-doc @brief 整数版は ll 超過を検出。第1引数が ACL modint なら巨大な項数も同じ法で返す
/// @acf-doc @code (ll a, ll d, ll n) -> ll / (Mint a, d, ll n) -> Mint
/// @acf-doc [function] arith_sum
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
/// @acf-doc @brief 整数版は超過を検出。第1引数が ACL modint なら除算なしの二分法で巨大な項数も同じ法で返す
/// @acf-doc @code (ll a, ll r, ll n) -> ll / (Mint a, r, ll n) -> Mint
/// @acf-doc [function] geom_sum
inline ll geom_sum(ll a, ll r, ll n) {
    if (n < 0) throw invalid_argument("geom_sum: n must be non-negative");
    if (n == 0 || a == 0) return 0;
    const i128 series = ac_detail::geom_pow_sum(r, n).first;
    return ac_detail::checked_ll(
        ac_detail::checked_mul_i128(a, series, "geom_sum"), "geom_sum");
}

#ifdef AC_HAS_ACL
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
/// @acf-doc @brief `n(n+1)(2n+1)/6`。整数版は ll 超過を検出。`sum_sq<Mint>(n)` は除算前に整数因子を約分して同じ法で返す
/// @acf-doc @code (ll n) -> ll / sum_sq<Mint>(ll n) -> Mint
/// @acf-doc [function] sum_sq
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
/// @acf-doc @brief `(n(n+1)/2)^2`。整数版は ll 超過を検出。`sum_cu<Mint>(n)` は2の逆元を使わず同じ法で返す
/// @acf-doc @code (ll n) -> ll / sum_cu<Mint>(ll n) -> Mint
/// @acf-doc [function] sum_cu
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
/// @acf-doc @brief 64bit 整数の素数判定。決定的 Miller-Rabin 法で O(log n) 回程度の mod 乗算。負数・0・1 は false
/// @acf-doc @code (integer n) -> bool
/// @acf-doc [function] is_prime
template <class T,
          enable_if_t<is_integral_v<remove_cv_t<T>> && (sizeof(T) <= 8), int> = 0>
inline bool is_prime(T n) {
    if constexpr (is_signed_v<T>) {
        if (n < 2) return false;
    }
    return ac_detail::prime_detail::is_prime_u64(static_cast<ull>(n));
}

// @acf fn prime_factorize — (integer n) -> vector<pair<ull,int>> — 64bit 整数を高速に素因数分解
/// @acf-doc @brief 決定的 Miller-Rabin 法と Pollard's Rho（Brent 法）で64bit整数を素因数分解し、`(素因数,指数)` を昇順で返す。`n<2` は空
/// @acf-doc @code (integer n) -> vector<pair<ull,int>>
/// @acf-doc [function] prime_factorize
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
/// @acf-doc @brief 線形篩で `n` 以下の各整数の最小素因数を作る。`spf[x]` は `x` の最小素因数で、`n<2` は空
/// @acf-doc @code (int n) -> vi
/// @acf-doc [function] smallest_prime_factor_sieve
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
/// @acf-doc @brief `smallest_prime_factor_sieve` の表を使って小さい整数を素因数分解し、`(素因数,指数)` を昇順で返す。`n<2` は空
/// @acf-doc @code (int n, vi smallest_prime_factors) -> vector<pair<ull,int>>
/// @acf-doc [function] prime_factorize_with_sieve
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
/// @acf-doc @brief 正の整数 n 以下で n と互いに素な整数の個数 `phi(n)` を返す。`n=1` は1、`n<1` は0。素因数分解後 O(素因数種類数)
/// @acf-doc @code (integer n) -> ull
/// @acf-doc [function] euler_phi
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
/// @acf-doc @brief 素数 `p` に対する `a` の乗法位数（最小の正整数 `k` で `a^k = 1 (mod p)`）。`p-1` の素因数分解を渡す。`p` は素数、`1 <= a < p`、因数分解は正確であることが前提
/// @acf-doc @code (ull a, ull p, vector<pair<ull,int>> factors_of_p_minus_one) -> ull
/// @acf-doc [function] multiplicative_order_prime
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
/// @acf-doc @brief 素数 `p` の原始根を一つ返す。`p-1` の素因数分解を渡し、`g^((p-1)/q) != 1 (mod p)` を全ての異なる素因数 `q` について確認する。`p=2` は1。`p` は素数、因数分解は正確であることが前提
/// @acf-doc @code (ull p, vector<pair<ull,int>> factors_of_p_minus_one) -> ull
/// @acf-doc [function] primitive_root_prime
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
/// @acf-doc @brief `prime_factorize` の結果または整数から、指数を全組合せして正の約数を昇順で返す。`n<1` は空。約数個数を `d` として分解後 O(d log d)
/// @acf-doc @code (integer n / vector<pair<ull,int>> factors) -> vull
/// @acf-doc [function] divisors
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
/// @acf-doc @brief `n=prod(p^e)` に対する `prod(e+1)`。整数または `prime_factorize` の結果を渡せる。`n<1` は 0
/// @acf-doc @code (integer n / vector<pair<ull,int>> factors) -> ull
/// @acf-doc [function] divisor_count
inline ull divisor_count(const vector<pair<ull, int>>& factors) {
    ull result = 1;
    for (auto [prime, exponent] : factors) {
        (void)prime;
        result *= static_cast<ull>(exponent + 1);
    }
    return result;
}

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
/// @acf-doc @brief `n=prod(p^e)` に対する `prod(1+p+...+p^e)`。通常は u128、`divisor_sum<Mint>(...)` は同じ法のmodintを返す
/// @acf-doc @code (integer n / factors) -> u128 / divisor_sum<Mint>(integer n / factors) -> Mint
/// @acf-doc [function] divisor_sum
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
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint divisor_sum(const vector<pair<ull, int>>& factors) {
    Mint result = 1;
    for (auto [prime, exponent] : factors) {
        result *= geom_sum(Mint(1), prime, static_cast<ll>(exponent) + 1);
    }
    return result;
}

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
/// @acf-doc @brief `[2,n]` の素数を昇順で返す。エラトステネスのふるい。`n<2` は空
/// @acf-doc @code (int n) -> vi
/// @acf-doc [function] primes_up_to
inline vi primes_up_to(int n) {
    return ac_detail::prime_detail::sieve(n);
}

// @acf fn mobius_sieve — (int n) -> vi — n 以下のメビウス関数をふるいで列挙
/// @acf-doc @brief `mu[0]=0` とし、`mu[1..n]` をメビウス関数の定義どおりに返す。`n<1` は空。線形篩で O(n)
/// @acf-doc @code (int n) -> vi
/// @acf-doc [function] mobius_sieve
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
/// @acf-doc @brief `phi[0]=0` とし、`phi[1..n]` をオイラーのトーシェント関数として返す。`n<1` は空。線形篩で O(n)
/// @acf-doc @code (int n) -> vi
/// @acf-doc [function] totient_sieve
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
/// @acf-doc @brief 閉区間 `[left,right]` の素数を昇順で返す。区間ふるい。空区間や `right<2` は空
/// @acf-doc @code (ll left, ll right) -> vll
/// @acf-doc [function] primes_in_range
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
/// @acf-doc @brief 正の整数 a,b のLCMを、limit 以下のときだけ返す。積が上限を超えると nullopt。積の前に gcd で割る
/// @acf-doc @code (ll a, ll b, ll limit) -> optional<ll>
/// @acf-doc [function] lcm_bounded
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
/// @acf-doc @brief 法を取らない整数の累乗を繰り返し二乗法で返す。`exp==0` は1。結果が `ll` に収まらなければ例外
/// @acf-doc @code (ll base, ll exp) -> ll
/// @acf-doc [function] ipow
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
/// @acf-doc @brief `x >= 0, n >= 1`。浮動小数点を使わず `floor(x^(1/n))` を返す
/// @acf-doc @code (ll x, ll n) -> ll
/// @acf-doc [function] nth_root
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
/// @acf-doc @brief `x >= 0, n >= 1`。完全な n 乗ならその根、そうでなければ `-1` を返す
/// @acf-doc @code (ll x, ll n) -> ll
/// @acf-doc [function] exact_nth_root
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
/// @acf-doc @brief 整数版は法を第3引数に渡す。modint版 `pow_mod(x,n)` は型の法を使う。どちらも繰り返し二乗法で n>=0
/// @acf-doc @code (ll x, ll n, int m) -> ll / (Mint x, ll n) -> Mint
/// @acf-doc [function] pow_mod
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
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint pow_mod(Mint x, ll n) {
    if (n < 0) throw invalid_argument("pow_mod: n must be non-negative");
    return x.pow(n);
}
#endif

// @acf fn extended_gcd — (ll a, ll b, ll& x, ll& y) -> ll — ax+by=gcd(a,b) の係数も返す拡張ユークリッド互除法
/// @acf-doc @brief `a*x + b*y = gcd(a,b)` を満たす係数 `x,y` と、非負の最大公約数を返す。負数も扱い、`(0,0)` は `0` とする
/// @acf-doc @code (ll a, ll b, ll& x, ll& y) -> ll
/// @acf-doc [function] extended_gcd
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
/// @acf-doc @brief mod m での逆元。整数版は法を第2引数に渡す。modint版 `inv_mod(x)` は型の法を使う。どちらも値と法が互いに素であること
/// @acf-doc @code (ll x, ll m) -> ll / (Mint x) -> Mint
/// @acf-doc [function] inv_mod
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
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint inv_mod(Mint x) {
    if (gcd(static_cast<ll>(x.val()), static_cast<ll>(Mint::mod())) != 1) {
        throw invalid_argument("inv_mod: value and mod must be coprime");
    }
    return x.inv();
}
#endif

// @acf fn nCr — (ll n, ll r) -> ll / Mint — 組合せ（O(r)）
/// @acf-doc @brief 整数版は約分後に乗算し ll 超過を検出。`nCr<Mint>(n,r)` はmodintで返し、`r!` と法が互いに素である範囲に対応
/// @acf-doc @code (ll n, ll r) -> ll / nCr<Mint>(ll n, ll r) -> Mint
/// @acf-doc [function] nCr
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
/// @acf-doc @brief 順列 nPr = n!/(n-r)!。整数版は ll 超過を検出。`nPr<Mint>(n,r)` は除算なしで同じ法の値を返す
/// @acf-doc @code (ll n, ll r) -> ll / nPr<Mint>(ll n, ll r) -> Mint
/// @acf-doc [function] nPr
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
template <class Mint, enable_if_t<ac_detail::io::is_modint_v<Mint>, int> = 0>
Mint nPr(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return Mint(0);
    Mint result = 1;
    for (ll i = 0; i < r; ++i) result *= Mint(n - i);
    return result;
}
#endif

// @acf fn nHr — (ll n, ll r) -> ll / Mint — 重複組合せ H(n,r)=C(n+r-1,r)
/// @acf-doc @brief 重複組合せ。整数版は ll 超過を検出。`nHr<Mint>(n,r)` は `nCr<Mint>` と同じ逆元条件で同じ法の値を返す
/// @acf-doc @code (ll n, ll r) -> ll / nHr<Mint>(ll n, ll r) -> Mint
/// @acf-doc [function] nHr
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
    /// @brief 未初期化の組合せ前計算器を作る。
    Comb() = default;

    /// @brief 法 `m`、最大 `n` までの組合せ前計算器を構築する。
    explicit Comb(int n, int m = MOD1000000007) { init(n, m); }

    /// @brief 法 `m` で0から `n` までの階乗・逆階乗を前計算する。
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
    /// @brief 前計算範囲内の添字か検査する内部メソッド。
    void require_index(ll n, const char* operation) const {
        if (n < 0 || n > max_n) {
            throw out_of_range(string("Comb::") + operation + ": n is outside precomputed range");
        }
    }
    /// @brief `n! mod mod` を返す。
    ll fact(int n) const {
        require_index(n, "fact");
        return fac[n];
    }
    /// @brief `(n!)^{-1} mod mod` を返す。
    ll inv_fact(int n) const {
        require_index(n, "inv_fact");
        return ifac[n];
    }
    /// @brief `n` 個から `r` 個を選ぶ組合せを返す。
    ll C(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        require_index(n, "C");
        return mul_mod(mul_mod(fac[n], ifac[r], mod), ifac[n - r], mod);
    }
    /// @brief `n` 個から `r` 個を選ぶ順列を返す。
    ll P(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return 0;
        require_index(n, "P");
        return mul_mod(fac[n], ifac[n - r], mod);
    }
    /// @brief 重複組合せ `H(n,r)=C(n+r-1,r)` を返す。
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
    /// @brief n番目のカタラン数を返す。`2*n` まで前計算済みである必要がある。
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

    /// @brief 未初期化のmodint組合せ前計算器を作る。
    ModintComb() = default;

    /// @brief `Mint::mod()` を法として `n` まで前計算する。
    explicit ModintComb(int n) { init(n); }

    /// @brief `Mint::mod()` を法として0から `n` までの階乗を前計算する。
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

    /// @brief 前計算範囲内の添字か検査する内部メソッド。
    void require_index(ll n, const char* operation) const {
        if (n < 0 || n > max_n) {
            throw out_of_range(string("Comb::") + operation +
                               ": n is outside precomputed range");
        }
    }

    /// @brief `n!` を `Mint` で返す。
    Mint fact(int n) const {
        require_index(n, "fact");
        return fac[n];
    }
    /// @brief `(n!)^{-1}` を `Mint` で返す。
    Mint inv_fact(int n) const {
        require_index(n, "inv_fact");
        return ifac[n];
    }
    /// @brief `n` 個から `r` 個を選ぶ組合せを `Mint` で返す。
    Mint C(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return Mint(0);
        require_index(n, "C");
        return fac[n] * ifac[r] * ifac[n - r];
    }
    /// @brief `n` 個から `r` 個を選ぶ順列を `Mint` で返す。
    Mint P(ll n, ll r) const {
        if (n < 0 || r < 0 || n < r) return Mint(0);
        require_index(n, "P");
        return fac[n] * ifac[n - r];
    }
    /// @brief 重複組合せ `H(n,r)` を `Mint` で返す。
    Mint H(ll n, ll r) const {
        if (n < 0 || r < 0) return Mint(0);
        if (r == 0) return Mint(1);
        if (n == 0) return Mint(0);
        if (r > numeric_limits<ll>::max() - (n - 1)) {
            throw overflow_error("Comb::H: n + r - 1 does not fit in long long");
        }
        return C((n - 1) + r, r);
    }
    /// @brief n番目のカタラン数を `Mint` で返す。
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
/// @acf-doc @brief 「1000000007で割った余り」の組合せに使う。`Comb1000000007 comb(max_n); comb.C(n,r)`
/// @acf-doc [type] Comb1000000007
using Comb1000000007 = ac_detail::ModintComb<mint1000000007>;
// @acf using Comb998244353 — mod 998,244,353 の推奨組合せクラス
/// @acf-doc @brief 「998244353で割った余り」の組合せに使う。`Comb998244353 comb(max_n); comb.C(n,r)`
/// @acf-doc [type] Comb998244353
using Comb998244353 = ac_detail::ModintComb<mint998244353>;
#endif

// 競プロ向け動的行列。通常の環では演算子を使い、min-plus などは
// matrix_product / 4引数 matrix_power に演算を渡す。
template <class T>
struct Matrix {
    int rows = 0;
    int cols = 0;
    vector<vector<T>> values;

    /// @brief 0×0の空行列を構築する。
    Matrix() = default;

    /// @brief `row_count`×`column_count` の行列を値 `value` で初期化する。
    Matrix(int row_count, int column_count, const T& value = T{})
        : rows(row_count), cols(column_count) {
        if (rows < 0 || cols < 0) {
            throw invalid_argument("Matrix: dimensions must be non-negative");
        }
        values.assign(static_cast<size_t>(rows),
                      vector<T>(static_cast<size_t>(cols), value));
    }

    /// @brief 行列要素の2次元vectorから構築する。全行の長さは一致させる。
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

    /// @brief `{{...}, {...}}` 形式の初期化子から構築する。
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

    /// @brief `size`×`size` の単位行列を返す。
    static Matrix identity(int size) {
        if (size < 0) throw invalid_argument("Matrix::identity: size must be non-negative");
        Matrix result(size, size);
        for (int i = 0; i < size; ++i) result[i][i] = T{1};
        return result;
    }

    /// @brief 行数を返す。
    int row_count() const { return rows; }

    /// @brief 列数を返す。
    int column_count() const { return cols; }

    /// @brief 行数または列数が0か判定する。
    bool empty() const { return rows == 0 || cols == 0; }

    /// @brief 行数（外側vectorの要素数）を返す。
    size_t size() const { return values.size(); }

    /// @brief 行 `row` を範囲チェックなしで返す。
    vector<T>& operator[](int row) { return values[static_cast<size_t>(row)]; }

    /// @brief 行 `row` を範囲チェックなしでconst返却する。
    const vector<T>& operator[](int row) const {
        return values[static_cast<size_t>(row)];
    }

    /// @brief 要素 `(row,col)` を範囲チェックなしで返す。
    decltype(auto) operator()(int row, int col) { return (*this)[row][col]; }

    /// @brief 要素 `(row,col)` を範囲チェックなしでconst返却する。
    decltype(auto) operator()(int row, int col) const { return (*this)[row][col]; }

    /// @brief 要素 `(row,col)` を範囲チェック付きで返す。
    decltype(auto) at(int row, int col) {
        return values.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
    }

    /// @brief 要素 `(row,col)` を範囲チェック付きでconst返却する。
    decltype(auto) at(int row, int col) const {
        return values.at(static_cast<size_t>(row)).at(static_cast<size_t>(col));
    }

    /// @brief 行を走査するmutable iteratorの先頭を返す。
    auto begin() { return values.begin(); }

    /// @brief 行を走査するmutable iteratorの末尾を返す。
    auto end() { return values.end(); }

    /// @brief 行を走査するconst iteratorの先頭を返す。
    auto begin() const { return values.begin(); }

    /// @brief 行を走査するconst iteratorの末尾を返す。
    auto end() const { return values.end(); }

    /// @brief 行と列を入れ替えた転置行列を返す。
    Matrix transposed() const {
        Matrix result(cols, rows);
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) result[col][row] = (*this)[row][col];
        return result;
    }

    /// @brief 正方行列の対角成分の和を返す。
    T diagonal_sum() const {
        if (rows != cols) {
            throw invalid_argument("Matrix::diagonal_sum: matrix must be square");
        }
        T result{};
        for (int i = 0; i < rows; ++i) result += (*this)[i][i];
        return result;
    }

    /// @brief 同じ形の行列を加算して自身を更新する。
    Matrix& operator+=(const Matrix& rhs) {
        require_same_shape(rhs, "operator+=");
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) (*this)[row][col] += rhs[row][col];
        return *this;
    }

    /// @brief 同じ形の行列を減算して自身を更新する。
    Matrix& operator-=(const Matrix& rhs) {
        require_same_shape(rhs, "operator-=");
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col) (*this)[row][col] -= rhs[row][col];
        return *this;
    }

    /// @brief 行列積で自身を更新する。
    Matrix& operator*=(const Matrix& rhs) {
        *this = *this * rhs;
        return *this;
    }

    /// @brief 全要素をスカラー倍して自身を更新する。
    Matrix& operator*=(const T& scalar) {
        for (auto& row : values)
            for (auto& value : row) value *= scalar;
        return *this;
    }

    /// @brief 全要素をスカラーで割って自身を更新する。
    Matrix& operator/=(const T& scalar) {
        for (auto& row : values)
            for (auto& value : row) value /= scalar;
        return *this;
    }

    /// @brief 全要素の符号を反転した行列を返す。
    Matrix operator-() const {
        Matrix result(*this);
        for (auto& row : result.values)
            for (auto& value : row) value = -value;
        return result;
    }

    /// @brief 行列を非負整数 `exponent` 乗した結果を返す。
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

    /// @brief 2つの行列の和を返す。
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) { return lhs += rhs; }

    /// @brief 2つの行列の差を返す。
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) { return lhs -= rhs; }

    /// @brief 2つの行列の積を返す。
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

    /// @brief 行列とvectorの積を返す。
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

    /// @brief 行列をスカラー倍した結果を返す。
    friend Matrix operator*(Matrix matrix, const T& scalar) { return matrix *= scalar; }

    /// @brief スカラー倍の左右を入れ替えた結果を返す。
    friend Matrix operator*(const T& scalar, Matrix matrix) { return matrix *= scalar; }

    /// @brief 行列をスカラーで割った結果を返す。
    friend Matrix operator/(Matrix matrix, const T& scalar) { return matrix /= scalar; }

    /// @brief 形と全要素が一致するか判定する。
    friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
        return lhs.rows == rhs.rows && lhs.cols == rhs.cols && lhs.values == rhs.values;
    }

   private:
    /// @brief 二項演算の前提となる行列形状の一致を検査する。
    void require_same_shape(const Matrix& rhs, const char* operation) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw invalid_argument(string("Matrix::") + operation +
                                   ": matrix shapes must match");
        }
    }
};

// @acf fn matrix_trace — (const Matrix<T>& matrix) -> T — 対角和（トレース）
/// @acf-doc @brief 正方行列の対角成分の和。`trace` はデバッグマクロ名なので衝突しない `matrix_trace` を使う。O(N)
/// @acf-doc @code (const Matrix<T>& matrix) -> T
/// @acf-doc [function] matrix_trace
template <class T>
T matrix_trace(const Matrix<T>& matrix) {
    return matrix.diagonal_sum();
}

// @acf fn matrix_product — (A,B[,zero,add,multiply]) -> Matrix<T> — 通常積または任意半環の行列積
/// @acf-doc @brief 通常は `matrix_product(a,b)`。`zero, add, multiply` を渡すと min-plus・max-plus・bool など任意半環の積になる。O(HWK)
/// @acf-doc @code (const Matrix<T>& a, const Matrix<T>& b, T zero=T{}, Add add={}, Multiply multiply={}) -> Matrix<T>
/// @acf-doc [function] matrix_product
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
/// @acf-doc @brief 正方行列を繰り返し二乗法で `exponent` 乗する。0乗は単位行列。O(N^3 log exponent)
/// @acf-doc @code (Matrix<T> base, ull exponent) -> Matrix<T>
/// @acf-doc [function] matrix_power
template <class T>
Matrix<T> matrix_power(Matrix<T> base, ull exponent) {
    return base.pow(exponent);
}

// 任意半環向け。identity と積を渡せば、通常の +/* を持たない遷移にも使える。
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
/// @acf-doc @brief `I + A + ... + A^(exponent-1)` を二倍化で返す。exponent=0 は零行列。O(N^3 log exponent)
/// @acf-doc @code (Matrix<T> base, ull exponent) -> Matrix<T>
/// @acf-doc [function] matrix_power_sum
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

template <class T>
struct GaussJordanResult {
    Matrix<T> reduced;
    int rank = 0;
    vi pivot_columns;
};

// @acf fn gauss_jordan — (Matrix<T> matrix[,coefficient_columns,eps]) -> GaussJordanResult<T> — 掃き出し法
/// @acf-doc @brief 実数または modint 行列を行簡約階段形にし、階数と pivot 列を返す。拡大係数行列では coefficient_columns に変数列数を渡す
/// @acf-doc @code (Matrix<T> matrix, int coefficient_columns=-1, ld eps=EPS) -> GaussJordanResult<T>
/// @acf-doc [function] gauss_jordan
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
/// @acf-doc @brief 行列の rank。実数は部分 pivot 付き掃き出し、modint は体上、符号付き整数は分数なし消去で有理数体上の rank を求める
/// @acf-doc @code (Matrix<T> matrix, ld eps=EPS) -> int
/// @acf-doc [function] matrix_rank
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
/// @acf-doc @brief 実数・modint は Gaussian elimination、符号付き整数は Bareiss 法で determinant を返す。O(N^3)
/// @acf-doc @code (Matrix<T> matrix, ld eps=EPS) -> T
/// @acf-doc [function] matrix_determinant
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
/// @acf-doc @brief 実数または modint の正方行列を掃き出し、正則なら逆行列、特異なら nullopt。O(N^3)
/// @acf-doc @code (Matrix<T> matrix, ld eps=EPS) -> optional<Matrix<T>>
/// @acf-doc [function] matrix_inverse
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

template <class T>
struct LinearSystemResult {
    bool consistent = false;
    int rank = 0;
    vector<T> particular;
    vector<vector<T>> nullspace_basis;

    /// @brief 連立方程式に少なくとも1つ解があるか判定する。
    bool has_solution() const { return consistent; }

    /// @brief 解が一意か判定する。
    bool unique() const { return consistent && nullspace_basis.empty(); }
};

// @acf fn solve_linear_system — (Matrix<T> coefficients, vector<T> rhs[,eps]) -> LinearSystemResult<T> — 連立一次方程式
/// @acf-doc @brief `Ax=b` を実数または modint の体上で解く。解なし判定、自由変数を0とした particular、斉次解空間の basis を返す
/// @acf-doc @code (const Matrix<T>& coefficients, const vector<T>& rhs, ld eps=EPS) -> LinearSystemResult<T>
/// @acf-doc [function] solve_linear_system
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
/// @acf-doc @brief `a_n=c[0]a_(n-1)+...+c[k-1]a_(n-k)` の0始まり第 `index` 項。Kitamasa法で O(k^2 log index)、整数・modint対応
/// @acf-doc @code (const vector<T>& initial, const vector<C>& coefficients, ull index) -> T
/// @acf-doc [function] linear_recurrence_nth
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

    const auto combine = [&](const vector<T>& lhs, const vector<T>& rhs) {
        vector<T> product(order * 2 - 1, T{});
        for (size_t i = 0; i < order; ++i) {
            for (size_t j = 0; j < order; ++j) {
                product[i + j] += lhs[i] * rhs[j];
            }
        }
        for (size_t degree = product.size(); degree-- > order;) {
            for (size_t lag = 0; lag < order; ++lag) {
                product[degree - 1 - lag] +=
                    product[degree] * recurrence_coefficients[lag];
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
    for (size_t i = 0; i < order; ++i) result += weights[i] * initial[i];
    return result;
}

inline ostream& operator<<(ostream& os, const Comb& comb) {
    return os << "Comb{mod=" << comb.mod
              << ", fac=" << comb.fac
              << ", ifac=" << comb.ifac << '}';
}
