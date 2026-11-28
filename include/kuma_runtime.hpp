#pragma once
// Library: https://github.com/facedolid/atcoder-headers
// Kuma generated-code support for GNU C++23.
// Keep this header in sync with the code emitted by tools/kuma/compiler.py.
#ifndef AC_NO_PRINT_MACROS
#define AC_NO_PRINT_MACROS
#endif
#include "ac.hpp"
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <span>
#include <string_view>
#include <utility>
#if __has_include(<ext/pb_ds/assoc_container.hpp>) && __has_include(<ext/pb_ds/tree_policy.hpp>)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define KUMA_HAS_PBDS 1
#endif

namespace kuma_rt {

inline std::vector<std::string> split_string_separator(std::string_view source,
                                                       std::string_view separator, bool trim_empty) {
    if (separator.empty()) throw std::invalid_argument("split: separator string must not be empty");
    std::vector<std::string> fields;
    std::size_t last_nonempty = 0;
    std::size_t start = 0;
    while (true) {
        const auto end = source.find(separator, start);
        const auto field = source.substr(start, end == std::string_view::npos ? end : end - start);
        if (!trim_empty || !fields.empty() || !field.empty()) fields.emplace_back(field);
        if (!field.empty()) last_nonempty = fields.size();
        if (end == std::string_view::npos) break;
        start = end + separator.size();
    }
    if (trim_empty) fields.resize(last_nonempty);
    return fields;
}

inline std::string join_strings(const std::vector<std::string>& parts, std::string_view separator) {
    std::string result;
    std::size_t size = 0;
    auto add = [&](std::size_t bytes) {
        if (bytes > result.max_size() - size) throw std::length_error("join: result too large");
        size += bytes;
    };
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) add(separator.size());
        add(parts[i].size());
    }
    result.reserve(size);
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) result.append(separator);
        result.append(parts[i]);
    }
    return result;
}

inline std::string join_strings(const std::vector<std::string>& parts, char separator) {
    return join_strings(parts, std::string_view(&separator, 1));
}
// These helpers deliberately do not change the raw ACL overloads.
template<class Acl = void>
inline std::vector<int> (suffix_array_bytes)(const std::string& text) {
#ifdef AC_HAS_ACL
    // ACL's SA-IS allocates an index map of size n + 1 using int arithmetic.
    if (text.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("suffix_array_bytes: input too large");
    std::vector<int> bytes;
    bytes.reserve(text.size());
    for (unsigned char byte : text) bytes.push_back(byte);
    return atcoder::suffix_array(bytes, 255);
#else
    static_assert(!std::is_same_v<Acl, Acl>, "Kuma suffix_array_bytes requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class Acl = void>
inline std::vector<int> (z_algorithm_bytes)(const std::string& text) {
#ifdef AC_HAS_ACL
    if (text.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("z_algorithm_bytes: input too large");
    // Z only checks equality, so signed and unsigned bytes give the same matches.
    return atcoder::z_algorithm(text);
#else
    static_assert(!std::is_same_v<Acl, Acl>, "Kuma z_algorithm_bytes requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class Acl = void>
inline std::vector<int> (lcp_array_bytes)(const std::string& text, const std::vector<int>& suffixes) {
#ifdef AC_HAS_ACL
    if (text.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("lcp_array_bytes: input too large");
    if (suffixes.size() != text.size())
        throw std::invalid_argument("lcp_array_bytes: suffix array size differs from string length");
    if (text.empty()) return {};
    const int n = static_cast<int>(text.size());
    {
        std::vector<int> rank(n, -1);
        for (int i = 0; i < n; ++i) {
            const int offset = suffixes[i];
            if (offset < 0 || offset >= n || rank[offset] != -1)
                throw std::invalid_argument("lcp_array_bytes: suffix array must be a permutation of byte offsets");
            rank[offset] = i;
        }
        // After the first byte ties, lexicographic order is the order of the
        // shorter suffixes. Their ranks must agree with every adjacent pair.
        const auto key = [&](int i) {
            return std::pair{static_cast<unsigned char>(text[i]), i + 1 < n ? rank[i + 1] : -1};
        };
        for (int i = 1; i < n; ++i)
            if (!(key(suffixes[i - 1]) < key(suffixes[i])))
                throw std::invalid_argument("lcp_array_bytes: suffix array is not sorted for this string");
    }
    // ACL compares bytes only for equality and does not need unsigned rank values.
    return atcoder::lcp_array(text, suffixes);
#else
    static_assert(!std::is_same_v<Acl, Acl>, "Kuma lcp_array_bytes requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class T>
std::vector<int> suffix_array_ints(const std::vector<T>& values, std::optional<long long> upper = std::nullopt) {
#ifdef AC_HAS_ACL
    if (values.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("suffix_array: input too large");
    if (!upper) return ::atcoder::suffix_array(values);
    if (*upper < 0 || *upper >= std::numeric_limits<int>::max())
        throw std::out_of_range("suffix_array: upper must be in [0, INT_MAX)");
    for (const auto value : values) {
        if (value < 0 || value > *upper)
            throw std::out_of_range("suffix_array: element must be in [0, upper]");
    }
    if constexpr (std::same_as<T, int>) return ::atcoder::suffix_array(values, static_cast<int>(*upper));
    else return ::atcoder::suffix_array(std::vector<int>(values.begin(), values.end()), static_cast<int>(*upper));
#else
    static_assert(!std::is_same_v<T, T>, "Kuma suffix_array requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class T>
std::vector<int> z_algorithm_ints(const std::vector<T>& values) {
#ifdef AC_HAS_ACL
    if (values.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("z_algorithm: input too large");
    return ::atcoder::z_algorithm(values);
#else
    static_assert(!std::is_same_v<T, T>, "Kuma z_algorithm requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class T>
std::vector<int> lcp_array_ints(const std::vector<T>& values, const std::vector<int>& suffixes) {
#ifdef AC_HAS_ACL
    if (values.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("lcp_array: input too large");
    if (suffixes.size() != values.size())
        throw std::invalid_argument("lcp_array: suffix array size differs from input length");
    if (values.empty()) return {};
    const int size = static_cast<int>(values.size());
    {
        std::vector<int> ranks(size, -1);
        for (int index = 0; index < size; ++index) {
            const int offset = suffixes[index];
            if (offset < 0 || offset >= size || ranks[offset] != -1)
                throw std::invalid_argument("lcp_array: suffix array must be a permutation of input offsets");
            ranks[offset] = index;
        }
        const auto key = [&](int offset) {
            return std::pair{values[offset], offset + 1 < size ? ranks[offset + 1] : -1};
        };
        for (int index = 1; index < size; ++index)
            if (!(key(suffixes[index - 1]) < key(suffixes[index])))
                throw std::invalid_argument("lcp_array: suffix array is not sorted for this input");
    }
    return ::atcoder::lcp_array(values, suffixes);
#else
    static_assert(!std::is_same_v<T, T>, "Kuma lcp_array requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class A, class B>
    requires ((std::same_as<A, int> || std::same_as<A, long long>) &&
              (std::same_as<B, int> || std::same_as<B, long long>))
inline std::vector<long long> checked_convolution_ll(const std::vector<A>& a, const std::vector<B>& b) {
#ifdef AC_HAS_ACL
    if (a.empty() || b.empty()) return {};
    constexpr std::size_t max_length = std::size_t{1} << 24;
    if (a.size() > max_length || b.size() > max_length + 1 - a.size())
        throw std::length_error("convolution_ll: output length must be at most 2^24");

    using Wide = unsigned __int128;
    using U64 = unsigned long long;
    const auto magnitudes = [](const auto& values) {
        Wide sum = 0;  // at most 2^24 terms, each at most 2^63
        U64 maximum = 0;
        for (auto x : values) {
            const U64 bits = static_cast<U64>(x);
            const U64 magnitude = x < 0 ? U64{0} - bits : bits;
            sum += magnitude;
            maximum = std::max(maximum, magnitude);
        }
        return std::pair{sum, maximum};
    };
    const auto [sum_a, max_a] = magnitudes(a);
    const auto [sum_b, max_b] = magnitudes(b);
    if (max_a == 0 || max_b == 0) return std::vector<long long>(a.size() + b.size() - 1, 0);
    constexpr U64 limit = std::numeric_limits<long long>::max();
    // Both sum(A)*max(B) and sum(B)*max(A) bound each coefficient's magnitude.
    // Divide before comparing so the bound itself cannot overflow.
    if (sum_a > limit / max_b && sum_b > limit / max_a)
        throw std::overflow_error("convolution_ll: cannot certify i64 coefficients");

    const auto coefficients = []<class T>(const std::vector<T>& values) -> decltype(auto) {
        if constexpr (std::same_as<T, long long>) return (values);
        else return std::vector<long long>(values.begin(), values.end());
    };
    return ::atcoder::convolution_ll(coefficients(a), coefficients(b));
#else
    static_assert(!std::is_same_v<A, A>, "Kuma convolution_ll requires AtCoder Library (ACL)");
    return {};
#endif
}

#ifdef AC_HAS_ACL
inline bool convolution_length_fits(std::size_t left, std::size_t right, std::size_t limit) {
    return left <= limit && right <= limit + 1 - left;
}

template<class A, class B>
std::vector<long long> convolution_mod_crt(const std::vector<A>& left, const std::vector<B>& right,
                                           long long modulus) {
    if (modulus <= 0 || modulus > std::numeric_limits<int>::max())
        throw std::out_of_range("convolution_mod: modulus must be a positive i32");
    if (left.empty() || right.empty()) return {};
    constexpr std::size_t limit = std::size_t{1} << 24;
    if (!convolution_length_fits(left.size(), right.size(), limit))
        throw std::length_error("convolution_mod: output length exceeds 2^24 for this modulus");
    const auto length = left.size() + right.size() - 1;
    if (modulus == 1) return std::vector<long long>(length);
    const auto residues = [modulus]<class T>(const std::vector<T>& values) {
        std::vector<int> result;
        result.reserve(values.size());
        for (const auto& value : values) {
            if constexpr (std::integral<T>) {
                const auto remainder = static_cast<long long>(value) % modulus;
                result.push_back(static_cast<int>(remainder < 0 ? remainder + modulus : remainder));
            } else result.push_back(value.val());
        }
        return result;
    };
    const auto a = residues(left), b = residues(right);
    std::vector<long long> result(length);
    if (std::min(a.size(), b.size()) <= 60) {
        for (std::size_t i = 0; i < a.size(); ++i)
            for (std::size_t j = 0; j < b.size(); ++j)
                result[i + j] = (result[i + j] + 1LL * a[i] * b[j]) % modulus;
        return result;
    }

    constexpr long long p = 754974721, q = 167772161, r = 469762049;
    constexpr long long pq = p * q;
    // At most 2^23 summands, each the product of two residues below 2^31.
    static_assert(static_cast<__int128>(pq) * r > (static_cast<__int128>(1) << 85));
    constexpr auto inv_p = atcoder::internal::inv_gcd(p, q).second;
    constexpr auto inv_pq = atcoder::internal::inv_gcd(pq, r).second;
    const auto x = atcoder::convolution<p>(a, b);
    const auto y = atcoder::convolution<q>(a, b);
    const auto z = atcoder::convolution<r>(a, b);
    for (std::size_t i = 0; i < length; ++i) {
        const auto low = x[i] + p * (((y[i] + q - x[i] % q) * inv_p) % q);
        const auto high = ((z[i] + r - low % r) * inv_pq) % r;
        result[i] = static_cast<long long>((low + static_cast<__int128>(pq) * high) % modulus);
    }
    return result;
}
#endif

template<class A, class B>
std::vector<long long> checked_convolution_mod_runtime(const std::vector<A>& left,
                                                       const std::vector<B>& right, long long modulus) {
#ifdef AC_HAS_ACL
    return convolution_mod_crt(left, right, modulus);
#else
    static_assert(!std::is_same_v<A, A>, "Kuma convolution_mod requires AtCoder Library (ACL)");
    return {};
#endif
}

template<unsigned Mod, class A, class B>
std::vector<long long> checked_convolution_mod(const std::vector<A>& left, const std::vector<B>& right) {
#ifdef AC_HAS_ACL
    if (left.empty() || right.empty()) return {};
    if constexpr (Mod <= 2'000'000'000 && atcoder::internal::is_prime<Mod>) {
        constexpr auto limit = std::size_t{1} << std::countr_zero(Mod - 1);
        if (convolution_length_fits(left.size(), right.size(), limit)) {
            const auto as_i64 = []<class T>(const std::vector<T>& values) -> decltype(auto) {
                if constexpr (std::same_as<T, long long>) return (values);
                else return std::vector<long long>(values.begin(), values.end());
            };
            return ::atcoder::convolution<Mod>(as_i64(left), as_i64(right));
        }
    }
    return convolution_mod_crt(left, right, Mod);
#else
    static_assert(Mod == 0, "Kuma convolution_mod requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class Mint>
std::vector<Mint> checked_convolution_mod_mint(const std::vector<Mint>& left,
                                               const std::vector<Mint>& right) {
#ifdef AC_HAS_ACL
    if (left.empty() || right.empty()) return {};
    if constexpr (atcoder::internal::is_static_modint<Mint>::value) {
        constexpr unsigned modulus = Mint::mod();
        if constexpr (modulus <= 2'000'000'000 && atcoder::internal::is_prime<modulus>) {
            constexpr auto limit = std::size_t{1} << std::countr_zero(modulus - 1);
            if (convolution_length_fits(left.size(), right.size(), limit))
                return ::atcoder::convolution(left, right);
        }
    }
    const auto residues = convolution_mod_crt(left, right, Mint{}.mod());
    std::vector<Mint> result;
    result.reserve(residues.size());
    for (auto value : residues) result.emplace_back(value);
    return result;
#else
    static_assert(!std::is_same_v<Mint, Mint>, "Kuma convolution_mod requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class Mint>
std::vector<Mint> checked_convolution998_mint(const std::vector<Mint>& left,
                                              const std::vector<Mint>& right) {
#ifdef AC_HAS_ACL
    if (left.empty() || right.empty()) return {};
    constexpr std::size_t limit = std::size_t{1} << 23;
    if (left.size() > limit || right.size() > limit + 1 - left.size())
        throw std::length_error("convolution998: output length must be at most 2^23");
    return ::atcoder::convolution(left, right);
#else
    static_assert(!std::is_same_v<Mint, Mint>, "Kuma convolution998 requires AtCoder Library (ACL)");
    return {};
#endif
}

template<class R, class M>
inline std::pair<long long, long long> checked_crt(const std::vector<R>& residues,
                                                   const std::vector<M>& moduli) {
#ifdef AC_HAS_ACL
    if (residues.size() != moduli.size())
        throw std::invalid_argument("crt: arrays must have the same length");
    if (residues.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::length_error("crt: arrays are too long");
    long long period = 1;
    for (long long modulus : moduli) {
        if (modulus <= 0) throw std::invalid_argument("crt: moduli must be positive");
        const long long factor = modulus / std::gcd(period, modulus);
        if (period > std::numeric_limits<long long>::max() / factor)
            throw std::overflow_error("crt: least common multiple does not fit in i64");
        period *= factor;
    }
    const auto as_i64 = []<class T>(const std::vector<T>& values) -> decltype(auto) {
        if constexpr (std::same_as<T, long long>) return (values);
        else return std::vector<long long>(values.begin(), values.end());
    };
    return ::atcoder::crt(as_i64(residues), as_i64(moduli));
#else
    static_assert(!std::is_same_v<R, R>, "Kuma crt requires AtCoder Library (ACL)");
    return {};
#endif
}

#ifdef AC_HAS_ACL
class CheckedSccGraph {
    int vertices_;
    int edges_ = 0;
    mutable atcoder::scc_graph graph_;

    static int checked_vertices(long long count) {
        if (count < 0 || count >= std::numeric_limits<int>::max())
            throw std::length_error("scc_graph: invalid vertex count");
        return static_cast<int>(count);
    }

public:
    explicit CheckedSccGraph(long long count) : vertices_(checked_vertices(count)), graph_(vertices_) {}

    int size() const { return vertices_; }

    void add_edge(long long from, long long to) {
        if (from < 0 || from >= vertices_ || to < 0 || to >= vertices_)
            throw std::out_of_range("scc_graph: invalid vertex");
        if (edges_ == std::numeric_limits<int>::max())
            throw std::length_error("scc_graph: too many edges");
        graph_.add_edge(static_cast<int>(from), static_cast<int>(to));
        ++edges_;
    }

    std::vector<std::vector<int>> scc() const { return graph_.scc(); }
};

class CheckedTwoSat {
    int variables_;
    mutable atcoder::two_sat solver_;  // The read-only ACL answer() is not const-qualified.
    int clauses_ = 0;
    bool answer_ready_ = false;
    bool intact_ = true;

    static int checked_size(long long n) {
        // The ACL implication graph has 2*n vertices and a CSR start array of size 2*n+1.
        if (n < 0 || n > (std::numeric_limits<int>::max() - 1) / 2)
            throw std::out_of_range("two_sat: invalid variable count");
        return static_cast<int>(n);
    }
    void require_intact() const {
        if (!intact_) throw std::logic_error("two_sat: previous clause insertion failed");
    }

public:
    explicit CheckedTwoSat(long long n) : variables_(checked_size(n)), solver_(variables_) {}
    void add_clause(long long i, bool f, long long j, bool g) {
        require_intact();
        if (i < 0 || i >= variables_ || j < 0 || j >= variables_)
            throw std::out_of_range("two_sat: variable index out of range");
        // Each clause creates two edges, counted by int CSR offsets.
        if (clauses_ == std::numeric_limits<int>::max() / 2)
            throw std::length_error("two_sat: too many clauses");
        answer_ready_ = false;
        intact_ = false;  // An allocation error can interrupt ACL after only one of two edge insertions.
        solver_.add_clause(static_cast<int>(i), f, static_cast<int>(j), g);
        ++clauses_;
        intact_ = true;
    }
    bool satisfiable() {
        require_intact();
        answer_ready_ = false;
        return answer_ready_ = solver_.satisfiable();
    }
    std::vector<bool> answer() const {
        if (!answer_ready_) throw std::logic_error("two_sat: answer requires a current satisfiable result");
        return solver_.answer();
    }
};
#endif

template<class T>
T checked_prefix3d_value(i128 value) {
    if (value < std::numeric_limits<T>::min() || value > std::numeric_limits<T>::max())
        throw std::overflow_error("prefix3d: result does not fit the element type");
    return static_cast<T>(value);
}

template<class T>
std::vector<std::vector<std::vector<T>>> checked_prefix_sum3d(
        const std::vector<std::vector<std::vector<T>>>& volume) {
    if (!ac_detail::grid::rectangular3d(volume))
        throw std::invalid_argument("prefix_sum3d: planes and rows must have equal dimensions");
    if constexpr (std::floating_point<T>) {
        return ::prefix_sum3d(volume);
    } else {
        const auto depth = volume.size();
        const auto height = depth == 0 ? 0 : volume.front().size();
        const auto width = height == 0 ? 0 : volume.front().front().size();
        std::vector<std::vector<std::vector<T>>> result(
            depth + 1, std::vector<std::vector<T>>(height + 1, std::vector<T>(width + 1)));
        std::vector<std::vector<i128>> depth_totals(height, std::vector<i128>(width));
        for (std::size_t plane = 0; plane < depth; ++plane) {
            std::vector<i128> row_totals(width);
            for (std::size_t row = 0; row < height; ++row) {
                i128 total = 0;
                for (std::size_t column = 0; column < width; ++column) {
                    depth_totals[row][column] += volume[plane][row][column];
                    row_totals[column] += depth_totals[row][column];
                    total += row_totals[column];
                    result[plane + 1][row + 1][column + 1] = checked_prefix3d_value<T>(total);
                }
            }
        }
        return result;
    }
}

template<class T>
T checked_range_sum3d(const std::vector<std::vector<std::vector<T>>>& prefix,
                      ll front, ll top, ll left, ll back, ll bottom, ll right) {
    if (prefix.empty() || prefix.front().empty() || prefix.front().front().empty()
            || !ac_detail::grid::rectangular3d(prefix))
        throw std::invalid_argument("range_sum: invalid 3D prefix shape");
    const auto check_border = [](T value) {
        if (value != T{}) throw std::invalid_argument("range_sum: prefix border must be zero");
    };
    for (std::size_t plane = 0; plane < prefix.size(); ++plane)
        for (std::size_t row = 0; row < prefix[plane].size(); ++row) {
            const auto& values = prefix[plane][row];
            if (plane == 0 || row == 0) {
                for (auto value : values) check_border(value);
            } else {
                check_border(values.front());
            }
        }
    if (front < 0 || top < 0 || left < 0 || front > back || top > bottom || left > right
            || static_cast<unsigned long long>(back) >= prefix.size()
            || static_cast<unsigned long long>(bottom) >= prefix.front().size()
            || static_cast<unsigned long long>(right) >= prefix.front().front().size())
        throw std::out_of_range("range_sum: invalid cuboid bounds");
    if (front == back || top == bottom || left == right) return T{};
    if constexpr (std::floating_point<T>) {
        return ::range_sum(prefix, front, top, left, back, bottom, right);
    } else {
        const auto at = [&](ll plane, ll row, ll column) -> i128 { return prefix[plane][row][column]; };
        const i128 sum = at(back, bottom, right) - at(front, bottom, right)
                       - at(back, top, right) - at(back, bottom, left)
                       + at(front, top, right) + at(front, bottom, left)
                       + at(back, top, left) - at(front, top, left);
        return checked_prefix3d_value<T>(sum);
    }
}

template<class T>
class Prefix2D {
    static_assert(std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, double>,
                  "Prefix2D supports int, long long or double");
    std::vector<std::vector<T>> table_{{T{}}};
public:
    Prefix2D() = default;
    explicit Prefix2D(const std::vector<std::vector<T>>& grid) {
        const auto width = grid.empty() ? 0 : grid.front().size();
        for (const auto& row : grid)
            if (row.size() != width) throw std::invalid_argument("prefix2d: rows must have equal length");
        table_ = ::prefix_sum2d(grid);
    }
    // Report the input shape, not the padded prefix table.
    friend std::ostream& operator<<(std::ostream& output, const Prefix2D& prefix) {
        const auto& table = prefix.table_;
        const auto rows = table.empty() ? 0 : table.size() - 1;
        const auto cols = table.empty() || table.front().empty() ? 0 : table.front().size() - 1;
        return output << "Prefix2D(rows=" << rows << ", cols=" << cols << ')';
    }
    T sum(long long r1, long long c1, long long r2, long long c2) const {
        if (table_.empty() || table_.front().empty() || r1 < 0 || c1 < 0 || r1 > r2 || c1 > c2 ||
            static_cast<unsigned long long>(r2) >= table_.size() ||
            static_cast<unsigned long long>(c2) >= table_.front().size())
            throw std::out_of_range("prefix2d: invalid rectangle bounds");
        if (r1 == r2 || c1 == c2) return T{};
        return table_[r2][c2] - table_[r1][c2] - table_[r2][c1] + table_[r1][c1];
    }
};
template<class T>
Prefix2D<T> prefix2d(const std::vector<std::vector<T>>& grid) { return Prefix2D<T>(grid); }
template<class T>
T range_sum(const Prefix2D<T>& prefix, long long r1, long long c1, long long r2, long long c2) {
    return prefix.sum(r1, c1, r2, c2);
}

class DynamicBitset {
    std::vector<std::uint64_t> words_;
    long long width_ = 0;

    static std::size_t word_count(long long width) {
        if (width < 0) throw std::invalid_argument("dynamic bitset: width must not be negative");
        const auto need = static_cast<unsigned long long>(width / 64 + (width % 64 != 0));
        if (need > std::numeric_limits<std::size_t>::max())
            throw std::length_error("dynamic bitset width exceeds representable storage");
        return static_cast<std::size_t>(need);
    }

    static void check_index(long long i, long long width) {
        if (i < 0 || i >= width) throw std::out_of_range("dynamic bitset: index out of range");
    }

    void mask_tail() {
        if (width_ == 0 || words_.empty()) return;
        const unsigned rem = static_cast<unsigned>(width_ % 64);
        if (rem == 0) return;
        words_.back() &= (std::uint64_t{1} << rem) - 1;
    }

public:
    explicit DynamicBitset(long long width) : words_(word_count(width), 0), width_(width) {}
    DynamicBitset(const DynamicBitset&) = default;
    DynamicBitset& operator=(const DynamicBitset&) = default;
    DynamicBitset(DynamicBitset&& other) noexcept
        : words_(std::move(other.words_)), width_(std::exchange(other.width_, 0)) {
        other.words_.clear();
    }
    DynamicBitset& operator=(DynamicBitset&& other) noexcept {
        if (this != &other) {
            words_ = std::move(other.words_);
            width_ = std::exchange(other.width_, 0);
            other.words_.clear();
        }
        return *this;
    }

    long long size() const { return width_; }

    long long count() const {
        long long total = 0;
        for (std::uint64_t word : words_) total += std::popcount(word);
        return total;
    }

    void set(long long i, bool value = true) {
        check_index(i, width_);
        const std::size_t word = static_cast<std::size_t>(i / 64);
        const unsigned bit = static_cast<unsigned>(i % 64);
        const std::uint64_t mask = std::uint64_t{1} << bit;
        if (value)
            words_[word] |= mask;
        else
            words_[word] &= ~mask;
    }

    bool test(long long i) const {
        check_index(i, width_);
        const std::size_t word = static_cast<std::size_t>(i / 64);
        const unsigned bit = static_cast<unsigned>(i % 64);
        return (words_[word] >> bit) & 1;
    }

    // Display the logical width and the low 64 bits, most significant first.
    friend std::ostream& operator<<(std::ostream& output, const DynamicBitset& bits) {
        const auto shown = std::min(bits.width_, 64LL);
        output << "DynamicBitset(" << bits.width_ << ", 0b";
        if (shown < bits.width_) output << "...";
        for (auto i = shown; i > 0; --i)
            output.put(bits.test(i - 1) ? '1' : '0');
        return output << ')';
    }

    // OR with a left shift of this bitset into itself (bits shifted past the width are dropped).
    void or_shift_left(long long shift) {
        if (shift < 0) throw std::invalid_argument("dynamic bitset: shift must not be negative");
        if (shift == 0 || shift >= width_ || words_.empty()) return;

        const std::size_t n = words_.size();
        const std::size_t word_shift = static_cast<std::size_t>(shift / 64);
        const unsigned bit_shift = static_cast<unsigned>(shift % 64);
        const unsigned bit_carry = 64u - bit_shift;

        for (std::size_t i = n; i-- > word_shift; ) {
            std::uint64_t part = words_[i - word_shift] << bit_shift;
            if (bit_shift != 0 && i > word_shift) part |= words_[i - word_shift - 1] >> bit_carry;
            words_[i] |= part;
        }
        mask_tail();
    }
};

static_assert(std::numeric_limits<int>::digits == 31, "Kuma needs 32-bit int");
static_assert(std::numeric_limits<long long>::digits == 63, "Kuma needs 64-bit long long");

[[noreturn]] inline void error(const std::string& message) {
    throw std::runtime_error(message);
}

// A counted AVL tree supplies portable order statistics where PBDS is absent.
#ifndef KUMA_HAS_PBDS
template<class Key, class T> class SortedListFallbackTree {
    using Entry = std::pair<const Key, T>;
    struct Node {
        Entry item;
        Node* parent;
        std::unique_ptr<Node> left, right;
        std::size_t size = 1;
        int height = 1;
        Node(const Key& key, const T& value, Node* p) : item(key, value), parent(p) {}
    };
    using Link = std::unique_ptr<Node>;
    Link root_;

    static std::size_t size(const Link& node) { return node ? node->size : 0; }
    static int height(const Link& node) { return node ? node->height : 0; }
    static void set_parent(const Link& node, Node* parent) {
        if (node) node->parent = parent;
    }
    static void refresh(Node& node) {
        node.size = 1 + size(node.left) + size(node.right);
        node.height = 1 + std::max(height(node.left), height(node.right));
    }
    static void rotate_left(Link& link) {
        auto top = std::move(link->right);
        top->parent = link->parent;
        link->right = std::move(top->left);
        set_parent(link->right, link.get());
        link->parent = top.get();
        refresh(*link);
        top->left = std::move(link);
        refresh(*top);
        link = std::move(top);
    }
    static void rotate_right(Link& link) {
        auto top = std::move(link->left);
        top->parent = link->parent;
        link->left = std::move(top->right);
        set_parent(link->left, link.get());
        link->parent = top.get();
        refresh(*link);
        top->right = std::move(link);
        refresh(*top);
        link = std::move(top);
    }
    static void balance(Link& link) {
        if (!link) return;
        refresh(*link);
        if (height(link->left) - height(link->right) > 1) {
            if (height(link->left->left) < height(link->left->right)) rotate_left(link->left);
            rotate_right(link);
        } else if (height(link->right) - height(link->left) > 1) {
            if (height(link->right->right) < height(link->right->left)) rotate_right(link->right);
            rotate_left(link);
        }
    }
    static void insert_at(Link& link, Node* parent, const std::pair<Key, T>& item) {
        if (!link) {
            link = std::make_unique<Node>(item.first, item.second, parent);
            return;
        }
        auto less = std::less<Key>{};
        if (less(item.first, link->item.first)) insert_at(link->left, link.get(), item);
        else if (less(link->item.first, item.first)) insert_at(link->right, link.get(), item);
        else return;
        balance(link);
    }
    static Link extract_min(Link& link) {
        if (link->left) {
            auto min = extract_min(link->left);
            balance(link);
            return min;
        }
        auto min = std::move(link);
        link = std::move(min->right);
        set_parent(link, min->parent);
        return min;
    }
    static void erase_at(Link& link) {
        auto old = std::move(link);
        if (!old->left) link = std::move(old->right);
        else if (!old->right) link = std::move(old->left);
        else {
            // Move the successor node itself, never the stored key/value.
            link = extract_min(old->right);
            link->left = std::move(old->left);
            link->right = std::move(old->right);
            set_parent(link->left, link.get());
            set_parent(link->right, link.get());
        }
        set_parent(link, old->parent);
        balance(link);
    }
    Link& link_of(Node* node) {
        auto parent = node->parent;
        if (!parent) return root_;
        return parent->left.get() == node ? parent->left : parent->right;
    }
    static Link clone(const Link& from, Node* parent) {
        if (!from) return {};
        auto node = std::make_unique<Node>(from->item.first, from->item.second, parent);
        node->left = clone(from->left, node.get());
        node->right = clone(from->right, node.get());
        refresh(*node);
        return node;
    }
    static Node* first(Node* node) {
        while (node && node->left) node = node->left.get();
        return node;
    }
public:
    class const_iterator {
        Node* node_ = nullptr;
        friend class SortedListFallbackTree;
        explicit const_iterator(Node* node) : node_(node) {}
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Entry;
        using difference_type = std::ptrdiff_t;
        using pointer = const Entry*;
        using reference = const Entry&;
        const_iterator() = default;
        bool operator==(const const_iterator&) const = default;
        reference operator*() const { return node_->item; }
        pointer operator->() const { return &node_->item; }
        const_iterator& operator++() {
            if (node_->right) {
                node_ = first(node_->right.get());
            } else {
                auto node = node_;
                auto parent = node->parent;
                while (parent && parent->right.get() == node) {
                    node = parent;
                    parent = node->parent;
                }
                node_ = parent;
            }
            return *this;
        }
        const_iterator operator++(int) { auto old = *this; ++*this; return old; }
    };

    SortedListFallbackTree() = default;
    SortedListFallbackTree(const SortedListFallbackTree& other) : root_(clone(other.root_, nullptr)) {}
    SortedListFallbackTree& operator=(const SortedListFallbackTree& other) {
        if (this != &other) root_ = clone(other.root_, nullptr);
        return *this;
    }
    SortedListFallbackTree(SortedListFallbackTree&&) noexcept = default;
    SortedListFallbackTree& operator=(SortedListFallbackTree&&) noexcept = default;

    std::size_t size() const { return size(root_); }
    bool empty() const { return !root_; }
    const_iterator begin() const { return const_iterator(first(root_.get())); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator lower_bound(const Key& key) const {
        auto node = root_.get();
        Node* best = nullptr;
        auto less = std::less<Key>{};
        while (node) {
            if (less(node->item.first, key)) node = node->right.get();
            else { best = node; node = node->left.get(); }
        }
        return const_iterator(best);
    }
    void insert(const std::pair<Key, T>& item) { insert_at(root_, nullptr, item); }
    void erase(const_iterator it) {
        auto parent = it.node_->parent;
        erase_at(link_of(it.node_));
        while (parent) {
            auto upper = parent->parent;
            balance(link_of(parent));
            parent = upper;
        }
    }
    std::size_t order_of_key(const Key& key) const {
        std::size_t rank = 0;
        auto node = root_.get();
        auto less = std::less<Key>{};
        while (node) {
            if (less(node->item.first, key)) {
                rank += 1 + size(node->left);
                node = node->right.get();
            } else node = node->left.get();
        }
        return rank;
    }
    const_iterator find_by_order(std::size_t i) const {
        auto node = root_.get();
        while (node) {
            const auto partial = size(node->left);
            if (i == partial) return const_iterator(node);
            if (i < partial) node = node->left.get();
            else { i -= partial + 1; node = node->right.get(); }
        }
        return end();
    }
};
#endif

template<class T, class = void> struct KumaOrderedValue : std::bool_constant<std::is_arithmetic_v<T> || std::is_same_v<T, std::string>> {};
template<class... T> struct KumaOrderedValue<std::tuple<T...>, void> : std::bool_constant<((!std::is_reference_v<T> && KumaOrderedValue<std::remove_cv_t<T>>::value) && ...)> {};
template<class A, class B> struct KumaOrderedValue<std::pair<A, B>, void> : std::bool_constant<!std::is_reference_v<A> && !std::is_reference_v<B> && KumaOrderedValue<std::remove_cv_t<A>>::value && KumaOrderedValue<std::remove_cv_t<B>>::value> {};
template<class T> struct KumaOrderedValue<T, std::void_t<typename T::kuma_gen_owned_record,
    typename T::kuma_gen_value_types, decltype(std::declval<const T&>().kuma_gen_value_fields())>> :
    KumaOrderedValue<typename T::kuma_gen_value_types> {};
// Capture only recursively checked owning values; the resulting copy has no borrowed fields.
template<class T> auto sorted_key_capture(const T& value) {
    using U = std::remove_cvref_t<T>;
    static_assert(KumaOrderedValue<U>::value,
                  "SortedList key captures must be supported owning ordered values");
    return U(value);
}

template<class T> struct KumaTupleValue : std::false_type {};
template<class... T> struct KumaTupleValue<std::tuple<T...>> : std::true_type {};
template<class A, class B> struct KumaTupleValue<std::pair<A, B>> : std::true_type {};

// Validate nested floating-point values before they participate in an ordering.
template<class V> V checked_ordered_value(V value) {
    ::ac_detail::validate_ordered_finite(value);
    return value;
}

template<class T>
void write_sorted_list_item(std::ostream& output, const T& value);

// Ordered multiset with indexed access; elements and copies are owned.
template<class T = long long, class K = T>
class SortedList {
    static_assert(KumaOrderedValue<T>::value, "SortedList value type is not supported");
    static_assert(KumaOrderedValue<K>::value, "SortedList key type is not supported");
    using Key = std::pair<K, unsigned long long>;
#ifdef KUMA_HAS_PBDS
    using Tree = __gnu_pbds::tree<Key, T, std::less<Key>,
        __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
#else
    using Tree = SortedListFallbackTree<Key, T>;
#endif
    Tree values_;
    std::function<K(const T&)> key_;
    unsigned long long next_ = 0;
    template<class U> static T convert(U value) {
        if constexpr (std::is_same_v<T, bool>) {
            static_assert(std::is_same_v<U, bool>, "SortedList requires bool values");
            return value;
        } else if constexpr (std::is_integral_v<T>) {
            static_assert(std::is_integral_v<U> && !std::is_same_v<U, bool>, "SortedList requires integer values");
            if constexpr (std::is_same_v<T, U>) return value;
            else {
                if (!std::in_range<T>(value)) error("SortedList integer out of range");
                return static_cast<T>(value);
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            static_assert(std::is_arithmetic_v<U> && !std::is_same_v<U, bool>, "SortedList requires numeric values");
            return checked_ordered_value(static_cast<T>(value));
        } else if constexpr (std::is_same_v<T, std::string>) {
            static_assert(std::is_convertible_v<U, std::string>, "SortedList requires string values");
            return std::string(value);
        } else {
            static_assert(std::is_same_v<T, U>, "SortedList tuple types must match");
            return checked_ordered_value(value);
        }
    }
    template<class U> static long long integer(U value) {
        static_assert(std::is_integral_v<U> && !std::is_same_v<U, bool>, "SortedList index must be integer");
        if (!std::in_range<long long>(value)) error("SortedList integer out of range");
        return static_cast<long long>(value);
    }
    std::size_t index(long long i) const {
        if (i < 0) i += static_cast<long long>(values_.size());
        if (i < 0 || static_cast<unsigned long long>(i) >= values_.size()) error("SortedList index out of range");
        return static_cast<std::size_t>(i);
    }
    bool erase_first(const T& x) {
        K key = checked_ordered_value(key_(x));
        auto it = values_.lower_bound({key, 0});
        for (; it != values_.end() && !(key < it->first.first) && !(it->first.first < key); ++it) {
            if (it->second == x) { values_.erase(it); return true; }
        }
        return false;
    }
public:
    SortedList() requires std::is_same_v<T, K> : key_([](const T& value) { return value; }) {}
    template<class U> explicit SortedList(const std::vector<U>& input) requires std::is_same_v<T, K> : SortedList() { for (std::size_t i = 0; i < input.size(); ++i) add(static_cast<U>(input[i])); }
    template<class U, class F> SortedList(const std::vector<U>& input, F key) : key_(std::move(key)) { for (std::size_t i = 0; i < input.size(); ++i) add(static_cast<U>(input[i])); }
    std::size_t size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    std::vector<T> snapshot() const {
        std::vector<T> result;
        result.reserve(values_.size());
        for (const auto& item : values_) result.push_back(item.second);
        return result;
    }
    // Preview stored values in index order without calling the key or copying values.
    friend std::ostream& operator<<(std::ostream& output, const SortedList& list) {
        const auto shown = std::min<std::size_t>(list.size(), 100);
        auto it = list.values_.begin();
        output.put('[');
        for (std::size_t i = 0; i < shown; ++i, ++it) {
            if (i > 0) output << ", ";
            write_sorted_list_item(output, it->second);
        }
        if (shown < list.size()) output << ", ...";
        return output << ']';
    }
    template<class U> void add(U value) {
        T x = convert(value);
        if (next_ == std::numeric_limits<unsigned long long>::max()) error("SortedList insertion counter exhausted");
        K key = checked_ordered_value(key_(x));
        values_.insert({{std::move(key), next_++}, x});
    }
    template<class U> long long bisect_left(U x) const { return static_cast<long long>(values_.order_of_key({checked_ordered_value(key_(convert(x))), 0})); }
    template<class U> long long bisect_right(U x) const { return static_cast<long long>(values_.order_of_key({checked_ordered_value(key_(convert(x))), std::numeric_limits<unsigned long long>::max()})); }
    template<class U> T operator[](U i) const { return values_.find_by_order(index(integer(i)))->second; }
    template<class U> bool discard(U value) { return erase_first(convert(value)); }
    template<class U> void remove(U value) {
        if (!erase_first(convert(value))) error("SortedList value not found");
    }
    template<std::integral U> requires (!std::same_as<U, bool>)
    std::optional<T> try_kth(U i) const {
        if constexpr (std::is_signed_v<U>) {
            if (i < 0) return std::nullopt;
        }
        if constexpr (std::numeric_limits<U>::digits > std::numeric_limits<std::size_t>::digits) {
            if (i > static_cast<U>(std::numeric_limits<std::size_t>::max())) return std::nullopt;
        }
        const auto k = static_cast<std::size_t>(i);
        if (k >= values_.size()) return std::nullopt;
        return values_.find_by_order(k)->second;
    }
    T pop() { return pop(-1); }
    template<class U> T pop(U i) {
        auto it = values_.find_by_order(index(integer(i)));
        T value = it->second;
        values_.erase(it);
        return value;
    }
};
template<class T> SortedList(const std::vector<T>&) -> SortedList<T>;
template<class T, class F> SortedList(const std::vector<T>&, F) -> SortedList<T, std::remove_cvref_t<std::invoke_result_t<F, const T&>>>;

template<class T, class R, class P>
auto sorted_named_key(const std::vector<T>& values, R (*function)(P)) {
    constexpr bool matching = std::is_same_v<T, std::remove_cvref_t<P>>;
    constexpr bool valid_result = KumaOrderedValue<R>::value;
    static_assert(matching, "Kuma named key parameter must match the SortedList element type");
    static_assert(valid_result, "Kuma named key return type must be a supported owning ordered value");
    if constexpr (matching && valid_result) return SortedList<T, R>(values, function);
    else return SortedList<T>{};
}

// Mixed Kuma for-loops snapshot SortedList; other ranges retain C++ semantics.
template<class T, class K> std::vector<T> mixed_iteration_range(const SortedList<T,K>& values) { return values.snapshot(); }
template<class T, class K> std::vector<T> mixed_iteration_range(SortedList<T,K>& values) { return values.snapshot(); }
template<class T, class K> std::vector<T> mixed_iteration_range(SortedList<T,K>&& values) { return values.snapshot(); }
template<class C> decltype(auto) mixed_iteration_range(C&& values) {
    if constexpr (std::is_lvalue_reference_v<C>) return (values);
    else return std::remove_cvref_t<C>(std::forward<C>(values));
}

inline void init() {
    ::fast_io();
    std::cin >> std::noboolalpha;
}

inline void check_input() {
    if (std::cin.fail() || std::cin.bad()) {
        error("input failed: invalid value, out-of-range value, or unexpected EOF");
    }
}

template<class T>
void read_index1_into(T& target) {
    constexpr bool supported = std::same_as<T, int> || std::same_as<T, long long>;
    static_assert(supported, "read : index1 requires a writable int or long long");
    if constexpr (supported) {
        long long raw = 0;
        std::cin >> raw;
        check_input();
        if (raw <= 0) error("read : index1 requires a positive integer");
        const long long value = raw - 1;
        if (value > std::numeric_limits<T>::max()) {
            error("read : index1 result is out of range for the target");
        }
        target = static_cast<T>(value);
    }
}

inline int dimension(long long n) {
    if (n < 0 || n > std::numeric_limits<int>::max()) {
        error("input dimension must be between 0 and INT_MAX");
    }
    return static_cast<int>(n);
}

// Mixed shaped input validates the original type and width before narrowing.
template<class I>
int checked_input_dimension(const I& n) {
    if constexpr ((!std::integral<I> && !std::same_as<I, __int128>
                   && !std::same_as<I, unsigned __int128>) || std::same_as<I, bool>) {
        error("input dimension must have an integer type other than bool");
    } else {
        if constexpr (std::numeric_limits<I>::is_signed) {
            if (n < 0) error("input dimension must be between 0 and INT_MAX");
        }
        if constexpr (std::numeric_limits<I>::digits > std::numeric_limits<int>::digits) {
            if (n > static_cast<I>(std::numeric_limits<int>::max()))
                error("input dimension must be between 0 and INT_MAX");
        }
        return static_cast<int>(n);
    }
}

template<class H, class W>
std::pair<int, int> checked_input_shape(const std::pair<H, W>& shape) {
    return {checked_input_dimension(shape.first), checked_input_dimension(shape.second)};
}

template<class T, class U>
T numeric_cast(U value) {
    static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<U>,
                  "numeric_cast requires numeric scalar types");
    if constexpr (std::is_floating_point_v<U>) {
        if (!std::isfinite(value)) error("numeric conversion requires a finite value");
        if constexpr (std::is_integral_v<T>) {
            const U truncated = std::trunc(value);
            // The exclusive power-of-two bound avoids rounding LL_MAX up to 2^63.
            const U upper = std::ldexp(U{1}, std::numeric_limits<T>::digits);
            const U lower = std::is_signed_v<T> ? -upper : U{0};
            if (truncated < lower || truncated >= upper) {
                error("numeric conversion is outside the integer range");
            }
            return static_cast<T>(truncated);
        } else {
            const long double wide = static_cast<long double>(value);
            if (wide < std::numeric_limits<T>::lowest() || wide > std::numeric_limits<T>::max()) {
                error("numeric conversion is outside the floating-point range");
            }
        }
    } else if constexpr (std::is_integral_v<T>) {
        if (!std::in_range<T>(value)) error("numeric conversion is outside the integer range");
    }
    return static_cast<T>(value);
}

template<std::integral I>
int checked_radix(I base) {
    if (base != 0 && (base < 2 || base > 36))
        throw std::invalid_argument("stoi/stoll: base must be 0 or 2..36");
    return static_cast<int>(base);
}

enum class StringStripSide { Left, Right, Both };

inline std::string string_strip(std::string_view value, StringStripSide side) {
    constexpr std::string_view spaces = " \t\r\n\v\f";
    if (side != StringStripSide::Right) {
        const auto first = value.find_first_not_of(spaces);
        if (first == std::string_view::npos) return {};
        value.remove_prefix(first);
    }
    if (side != StringStripSide::Left) {
        const auto last = value.find_last_not_of(spaces);
        if (last == std::string_view::npos) return {};
        value.remove_suffix(value.size() - 1 - last);
    }
    return std::string(value);
}

inline std::string (replace_all)(std::string_view source, std::string_view needle,
                               std::string_view replacement) {
    if (needle.empty()) throw std::invalid_argument("replace_all: empty needle");
    std::string result;
    result.reserve(source.size());
    std::size_t cursor = 0;
    for (auto hit = source.find(needle); hit != std::string_view::npos;
         hit = source.find(needle, cursor)) {
        result.append(source.substr(cursor, hit - cursor));
        result.append(replacement);
        cursor = hit + needle.size();
    }
    result.append(source.substr(cursor));
    return result;
}

inline long long (count_substr)(std::string_view source, std::string_view needle) {
    if (needle.empty()) throw std::invalid_argument("count_substr: empty needle");
    std::size_t count = 0;
    for (auto hit = source.find(needle); hit != std::string_view::npos;
         hit = source.find(needle, hit + needle.size())) ++count;
    if (!std::in_range<long long>(count))
        throw std::length_error("count_substr: result exceeds i64");
    return static_cast<long long>(count);
}

inline std::tuple<std::string, std::string, std::string>
(partition_text)(std::string_view source, std::string_view separator) {
    if (separator.empty()) throw std::invalid_argument("partition_text: empty separator");
    const auto hit = source.find(separator);
    if (hit == std::string_view::npos) return {std::string(source), {}, {}};
    return {std::string(source.substr(0, hit)), std::string(separator),
            std::string(source.substr(hit + separator.size()))};
}

enum class StringPadSide { Start, End, ZFill };

template<std::integral I, class Fill>
requires (std::same_as<Fill, char> && !std::same_as<I, bool> && !std::same_as<I, char> &&
          !std::same_as<I, wchar_t> && !std::same_as<I, char8_t> &&
          !std::same_as<I, char16_t> && !std::same_as<I, char32_t>)
std::string string_pad(std::string value, I width, Fill fill, StringPadSide side) {
    if constexpr (std::is_signed_v<I>) {
        if (width < 0) return value;
    }
    if (!std::in_range<std::size_t>(width))
        throw std::length_error("string pad width overflows size_t");
    const std::size_t target = static_cast<std::size_t>(width);
    const std::size_t current = value.size();
    if (target <= current) return value;
    if (target > value.max_size())
        throw std::length_error("string pad width exceeds string max_size");
    const std::size_t pad = target - current;
    if (side == StringPadSide::ZFill) {
        if (!value.empty() && (value.front() == '+' || value.front() == '-'))
            value.insert(static_cast<std::size_t>(1), pad, '0');
        else
            value.insert(0, pad, '0');
        return value;
    }
    if (side == StringPadSide::Start) {
        value.insert(0, pad, fill);
        return value;
    }
    value.append(pad, fill);
    return value;
}

template<std::integral I>
requires (!std::same_as<I, bool> && !std::same_as<I, char> && !std::same_as<I, signed char> &&
          !std::same_as<I, unsigned char> && !std::same_as<I, wchar_t> && !std::same_as<I, char8_t> &&
          !std::same_as<I, char16_t> && !std::same_as<I, char32_t>)
std::string string_repeat(const std::string& value, I count) {
    if constexpr (std::is_signed_v<I>) {
        if (count <= 0) return {};
    } else if (count == 0) {
        return {};
    }
    if (value.empty()) return {};
    if (!std::in_range<std::size_t>(count))
        throw std::length_error("string repeat count overflows size_t");
    const std::size_t times = static_cast<std::size_t>(count);
    const std::size_t piece = value.size();
    if (times > value.max_size() / piece)
        throw std::length_error("string repeat exceeds string max_size");
    std::string result;
    result.reserve(piece * times);
    for (std::size_t i = 0; i < times; ++i)
        result += value;
    return result;
}

template<class T>
inline constexpr bool kuma_arithmetic_scalar = std::is_arithmetic_v<T> &&
    !std::same_as<T, bool> && !std::same_as<T, char> && !std::same_as<T, signed char> &&
    !std::same_as<T, unsigned char> && !std::same_as<T, wchar_t> && !std::same_as<T, char8_t> &&
    !std::same_as<T, char16_t> && !std::same_as<T, char32_t>;

template<class R> struct kuma_numeric_vector : std::false_type {};
template<class T>
struct kuma_numeric_vector<std::vector<T>> : std::bool_constant<kuma_arithmetic_scalar<T>> {};

template<class L, class R>
void kuma_vector_same_size(const L& left, const R& right) {
    if (left.size() != right.size())
        throw std::invalid_argument("vector operands must have the same length");
}

template<class A, class S>
void kuma_vector_divisor_ok(const std::vector<A>& values, S scalar) {
    using R = std::common_type_t<A, S>;
    const auto divisor = static_cast<R>(scalar);
    if (divisor == R{0}) throw std::domain_error("division by zero");
    if constexpr (std::signed_integral<R>) {
        if (divisor == R{-1}) {
            for (const A& value : values) {
                if (static_cast<R>(value) == (std::numeric_limits<R>::min)())
                    throw std::overflow_error("vector division overflows");
            }
        }
    }
}

template<char Op, class A, class B>
auto kuma_vector_pair(const std::vector<A>& left, const std::vector<B>& right) {
    kuma_vector_same_size(left, right);
    using R = std::common_type_t<A, B>;
    std::vector<R> out(left.size());
    for (std::size_t i = 0; i < left.size(); ++i) {
        if constexpr (Op == '+') out[i] = static_cast<R>(left[i]) + static_cast<R>(right[i]);
        else out[i] = static_cast<R>(left[i]) - static_cast<R>(right[i]);
    }
    return out;
}
template<class L, class R>
auto kuma_vector_add(const L& left, const R& right) { return kuma_vector_pair<'+'>(left, right); }
template<class L, class R>
auto kuma_vector_sub(const L& left, const R& right) { return kuma_vector_pair<'-'>(left, right); }

template<char Op, class A, class S>
auto kuma_vector_scale(const std::vector<A>& values, S scalar) {
    if constexpr (Op == '/') kuma_vector_divisor_ok(values, scalar);
    using R = std::common_type_t<A, S>;
    std::vector<R> out(values.size());
    for (std::size_t i = 0; i < values.size(); ++i) {
        if constexpr (Op == '*') out[i] = static_cast<R>(values[i]) * static_cast<R>(scalar);
        else out[i] = static_cast<R>(values[i]) / static_cast<R>(scalar);
    }
    return out;
}
template<class V, class S>
auto kuma_vector_mul(const V& values, S scalar) { return kuma_vector_scale<'*'>(values, scalar); }
template<class S, class V>
auto kuma_vector_rmul(S scalar, const V& values) { return kuma_vector_mul(values, scalar); }
template<class V, class S>
auto kuma_vector_div(const V& values, S scalar) { return kuma_vector_scale<'/'>(values, scalar); }

template<class L, class R>
void kuma_vector_iadd(L& left, const R& right) {
    kuma_vector_same_size(left, right);
    for (std::size_t i = 0; i < left.size(); ++i) left[i] += right[i];
}
template<class L, class R>
void kuma_vector_isub(L& left, const R& right) {
    kuma_vector_same_size(left, right);
    for (std::size_t i = 0; i < left.size(); ++i) left[i] -= right[i];
}
template<class L, class S>
void kuma_vector_imul(L& left, S scalar) {
    for (auto& value : left) value *= scalar;
}
template<class L, class S>
void kuma_vector_idiv(L& left, S scalar) {
    kuma_vector_divisor_ok(left, scalar);
    for (auto& value : left) value /= scalar;
}

template<class A, class B>
auto vector_dot(const std::vector<A>& left, const std::vector<B>& right) {
    kuma_vector_same_size(left, right);
    using R = std::common_type_t<A, B>;
    R out{};
    for (std::size_t i = 0; i < left.size(); ++i)
        out += static_cast<R>(left[i]) * static_cast<R>(right[i]);
    return out;
}

template<class A, class B>
auto vector_mul_each(const std::vector<A>& left, const std::vector<B>& right) {
    kuma_vector_same_size(left, right);
    using R = std::common_type_t<A, B>;
    std::vector<R> out(left.size());
    for (std::size_t i = 0; i < left.size(); ++i)
        out[i] = static_cast<R>(left[i]) * static_cast<R>(right[i]);
    return out;
}

template<class T>
auto vector_concat(const std::vector<T>& left, const std::vector<T>& right) {
    std::vector<T> out;
    if (right.size() > out.max_size() - left.size())
        throw std::length_error("vector concat exceeds max_size");
    out.reserve(left.size() + right.size());
    out.insert(out.end(), left.begin(), left.end());
    out.insert(out.end(), right.begin(), right.end());
    return out;
}

template<class T, std::integral I>
auto vector_repeat(const std::vector<T>& values, I count) {
    std::vector<T> out;
    if (count <= 0 || values.empty()) return out;
    if (!std::in_range<std::size_t>(count) || static_cast<std::size_t>(count) > out.max_size() / values.size())
        throw std::length_error("vector repeat exceeds max_size");
    const auto times = static_cast<std::size_t>(count);
    out.reserve(values.size() * times);
    for (std::size_t i = 0; i < times; ++i)
        out.insert(out.end(), values.begin(), values.end());
    return out;
}

template<class> inline constexpr bool is_plain_vector = false;
template<class T> inline constexpr bool is_plain_vector<std::vector<T>> = true;

// Compare cells directly: no substring copies and no hash collisions.
template<class Row>
    requires (std::is_same_v<Row, std::string> || is_plain_vector<Row>) &&
             requires(const Row& a, const Row& b) {
        { a[0] == b[0] } -> std::convertible_to<bool>;
    }
std::pair<int, int> find_subgrid(const std::vector<Row>& grid, const std::vector<Row>& pattern) {
    if (!::ac_detail::grid::rectangular(grid) || !::ac_detail::grid::rectangular(pattern))
        throw std::invalid_argument("find_subgrid: rows must have equal length");
    const auto height = grid.size(), h = pattern.size();
    if (h == 0 || h > height) return {-1, -1};
    const auto width = grid.front().size(), w = pattern.front().size();
    if (w == 0 || w > width) return {-1, -1};
    const auto limit = static_cast<std::size_t>(std::numeric_limits<int>::max());
    if (height > limit || width > limit)
        throw std::length_error("find_subgrid: coordinates must fit i32");
    for (std::size_t i = 0; i <= height - h; ++i) {
        for (std::size_t j = 0; j <= width - w; ++j) {
            bool matches = true;
            for (std::size_t r = 0; r < h && matches; ++r) {
                for (std::size_t c = 0; c < w; ++c) {
                    if (!(grid[i + r][j + c] == pattern[r][c])) {
                        matches = false;
                        break;
                    }
                }
            }
            if (matches) return {static_cast<int>(i), static_cast<int>(j)};
        }
    }
    return {-1, -1};
}

// Numeric key projections keep a single owned key; never sort or copy the range.
template<class T>
concept ExtremaNumber = std::same_as<std::remove_cvref_t<T>, int>
    || std::same_as<std::remove_cvref_t<T>, long long>
    || std::same_as<std::remove_cvref_t<T>, double>;

template<class T>
concept ExtremaScalar = std::same_as<T, std::remove_cvref_t<T>>
    && (ExtremaNumber<T> || std::same_as<T, bool>
        || std::same_as<T, char> || std::same_as<T, std::string>);

// Implicit assignment for a result that is not a compile-time constant.
template<class Result, class Acc>
inline constexpr bool implicit_fold_assignment = std::same_as<Result, Acc>
    || (std::same_as<Result, int> && std::same_as<Acc, long long>)
    || ((std::same_as<Result, int> || std::same_as<Result, long long>) && std::same_as<Acc, double>);

template<class T> inline constexpr bool is_extrema_pair = false;
template<ExtremaScalar T, ExtremaScalar U>
inline constexpr bool is_extrema_pair<std::pair<T, U>> = true;

template<class Key, class T>
concept ExtremaProjection = requires(Key& key, const T& item) {
    requires ExtremaNumber<decltype(std::invoke(key, item))>;
};

// Precondition: nonempty. The public-facing wrappers define their own empty behavior.
template<bool Max, class T, ExtremaProjection<T> Key>
std::size_t projected_extreme_position(const std::vector<T>& values, Key& key, const char* nan_error) {
    auto project = [&](const T& item) {
        auto value = std::invoke(key, item);
        if constexpr (std::same_as<decltype(value), double>) {
            if (std::isnan(value)) throw std::domain_error(nan_error);
        }
        return value;
    };
    auto best_key = project(values.front());
    std::size_t best = 0;
    for (std::size_t i = 1; i < values.size(); ++i) {
        const auto candidate = project(values[i]);
        if (Max ? best_key < candidate : candidate < best_key) {
            best_key = candidate;
            best = i;
        }
    }
    return best;
}

template<bool Max, ExtremaNumber T, ExtremaProjection<T> Key>
int projected_extreme_index(const std::vector<T>& values, Key&& key) {
    if (values.empty()) return 0;
    if (!std::in_range<int>(values.size() - 1))
        throw std::length_error("argmin/argmax: index does not fit i32");
    return static_cast<int>(projected_extreme_position<Max>(values, key, "argmin/argmax: key must not be NaN"));
}

template<bool Max, class T, ExtremaProjection<T> Key>
requires (ExtremaScalar<T> || is_extrema_pair<T>)
T projected_extreme_value(const std::vector<T>& values, Key&& key) {
    if (values.empty()) throw std::out_of_range("min_by/max_by: empty array");
    return values[projected_extreme_position<Max>(values, key, "min_by/max_by: key must not be NaN")];
}

template<class Item, class C> struct EnumerateView;
template<class Item, class... Cs> struct ZipView;
template<class T> struct is_kuma_view : std::false_type {};
template<class I, class C> struct is_kuma_view<EnumerateView<I, C>> : std::true_type {};
template<class I, class... Cs> struct is_kuma_view<ZipView<I, Cs...>> : std::true_type {};
template<class T> struct SetBitsView;
template<class T> struct SubmasksView;
template<class T> struct is_kuma_view<SetBitsView<T>> : std::true_type {};
template<class T> struct is_kuma_view<SubmasksView<T>> : std::true_type {};
template<class T> struct is_vector : std::false_type {};
template<class T, class A> struct is_vector<std::vector<T, A>> : std::true_type {};
template<class T> struct is_deque : std::false_type {};
template<class T, class A> struct is_deque<std::deque<T, A>> : std::true_type {};
template<class T> struct is_fixed_array : std::false_type {};
template<class T, std::size_t N> struct is_fixed_array<std::array<T, N>> : std::true_type {};
template<class T> struct is_cpp_sequence : std::false_type {};
template<class T, std::size_t N> struct is_cpp_sequence<std::array<T, N>> : std::true_type {};
template<class T, std::size_t N> struct is_cpp_sequence<std::span<T, N>> : std::true_type {};
template<class T, class A> struct is_cpp_sequence<std::list<T, A>> : std::true_type {};
template<class T, class A> struct is_cpp_sequence<std::forward_list<T, A>> : std::true_type {};
// No primary definition: mixed C++ input inference is available only for an
// actual std::vector, never an arbitrary type with a value_type member.
template<class T> struct vector_input_element;
template<class T, class A> struct vector_input_element<std::vector<T, A>> { using type = T; };
template<class T> struct is_tuple : std::false_type {};
template<class... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};
template<class A, class B> struct is_tuple<std::pair<A, B>> : std::true_type {};
template<class T> struct is_associative : std::false_type {};
template<class... Ts> struct is_associative<std::set<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::multiset<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::map<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::multimap<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::unordered_set<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::unordered_multiset<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::unordered_map<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::unordered_multimap<Ts...>> : std::true_type {};

template<class Storage>
class CheckedRangeStorage {
    std::function<void()> validate_;
    Storage storage_;

    template<class... Args>
    static Storage initialize(const std::function<void()>& validate, Args&&... args) {
        if (validate) validate();
        return Storage(std::forward<Args>(args)...);
    }

public:
    template<class... Args>
    requires std::is_constructible_v<Storage, Args&&...>
    explicit CheckedRangeStorage(Args&&... args) : storage_(std::forward<Args>(args)...) {}

    template<class... Args>
    CheckedRangeStorage(std::in_place_t, std::function<void()> validate, Args&&... args)
        : validate_(std::move(validate)), storage_(initialize(validate_, std::forward<Args>(args)...)) {}

    template<class Values>
    static CheckedRangeStorage minimum(const Values& values) {
        return CheckedRangeStorage(Storage::minimum(values));
    }

    Storage& checked_range() {
        if (validate_) validate_();
        return storage_;
    }
    const Storage& checked_range() const {
        if (validate_) validate_();
        return storage_;
    }

    template<class Values>
    void build(const Values& values) { checked_range().build(values); }
};

#ifdef AC_HAS_ACL
// Keep checked IDs separate from ACL's native dynamic_modint<ID> state.
template<int Id>
class DynamicModint {
    inline static atcoder::internal::barrett reduction_{998244353};
    inline static std::uint64_t generation_ = 0;
    inline static const char* modulus_lock_operation_ = nullptr;
    unsigned int value_ = 0;
    std::uint64_t created_ = generation_;

    void require_current() const {
        if (created_ != generation_)
            throw std::domain_error("dynamic_modint: value belongs to a previous modulus");
    }

    static DynamicModint from_residue(unsigned int value) {
        DynamicModint result;
        result.value_ = value;
        return result;
    }

    template<class Mint>
    friend Mint checked_mint_raw(long long value);

public:
    class ModulusLock {
        const char* previous_ = modulus_lock_operation_;

    public:
        explicit ModulusLock(const char* operation = "a lazy segment tree operation") {
            modulus_lock_operation_ = operation;
        }
        ModulusLock(const ModulusLock&) = delete;
        ModulusLock& operator=(const ModulusLock&) = delete;
        ~ModulusLock() { modulus_lock_operation_ = previous_; }
    };

    DynamicModint() = default;
    explicit DynamicModint(long long value) {
        const auto modulus = static_cast<long long>(reduction_.umod());
        const auto residue = value % modulus;
        value_ = static_cast<unsigned int>(residue < 0 ? residue + modulus : residue);
    }

    template<std::unsigned_integral T>
    explicit DynamicModint(T value) : value_(static_cast<unsigned int>(value % reduction_.umod())) {}

    static void set_mod(long long modulus) {
        if (modulus < 1 || modulus > std::numeric_limits<int>::max())
            throw std::out_of_range("dynamic_modint: modulus must be a positive i32");
        if (modulus == reduction_.umod()) return;
        if (modulus_lock_operation_)
            throw std::logic_error(std::string("dynamic_modint: cannot change a modulus during ") + modulus_lock_operation_);
        if (generation_ == std::numeric_limits<std::uint64_t>::max())
            throw std::overflow_error("dynamic_modint: too many modulus changes");
        reduction_ = atcoder::internal::barrett(static_cast<unsigned int>(modulus));
        ++generation_;
    }

    int val() const { require_current(); return static_cast<int>(value_); }
    int mod() const { require_current(); return static_cast<int>(reduction_.umod()); }

    DynamicModint operator+() const { require_current(); return *this; }
    DynamicModint operator-() const {
        require_current();
        return from_residue(value_ == 0 ? 0 : reduction_.umod() - value_);
    }

    DynamicModint& operator++() {
        require_current();
        if (++value_ == reduction_.umod()) value_ = 0;
        return *this;
    }

    DynamicModint& operator--() {
        require_current();
        if (value_ == 0) value_ = reduction_.umod();
        --value_;
        return *this;
    }

    DynamicModint operator++(int) {
        auto previous = *this;
        ++*this;
        return previous;
    }

    DynamicModint operator--(int) {
        auto previous = *this;
        --*this;
        return previous;
    }

    DynamicModint inv() const {
        require_current();
        const auto [gcd, inverse] = atcoder::internal::inv_gcd(value_, reduction_.umod());
        if (gcd != 1) throw std::domain_error("dynamic_modint: value has no multiplicative inverse");
        return from_residue(static_cast<unsigned int>(inverse));
    }

    DynamicModint pow(long long exponent) const {
        require_current();
        if (exponent < 0) throw std::invalid_argument("dynamic_modint.pow: negative exponent");
        auto base = value_;
        unsigned int result = 1 % reduction_.umod();
        while (exponent > 0) {
            if (exponent & 1) result = reduction_.mul(result, base);
            base = reduction_.mul(base, base);
            exponent >>= 1;
        }
        return from_residue(result);
    }

    DynamicModint& operator+=(const DynamicModint& rhs) {
        require_current();
        rhs.require_current();
        value_ += rhs.value_;
        if (value_ >= reduction_.umod()) value_ -= reduction_.umod();
        return *this;
    }
    DynamicModint& operator-=(const DynamicModint& rhs) {
        require_current();
        rhs.require_current();
        value_ += reduction_.umod() - rhs.value_;
        if (value_ >= reduction_.umod()) value_ -= reduction_.umod();
        return *this;
    }
    DynamicModint& operator*=(const DynamicModint& rhs) {
        require_current();
        rhs.require_current();
        value_ = reduction_.mul(value_, rhs.value_);
        return *this;
    }
    DynamicModint& operator/=(const DynamicModint& rhs) { return *this *= rhs.inv(); }
    friend DynamicModint operator+(DynamicModint a, const DynamicModint& b) { return a += b; }
    friend DynamicModint operator-(DynamicModint a, const DynamicModint& b) { return a -= b; }
    friend DynamicModint operator*(DynamicModint a, const DynamicModint& b) { return a *= b; }
    friend DynamicModint operator/(DynamicModint a, const DynamicModint& b) { return a /= b; }
    friend bool operator==(const DynamicModint& a, const DynamicModint& b) { return a.val() == b.val(); }
};

template<class T> struct is_checked_dynamic_modint : std::false_type {};
template<int Id> struct is_checked_dynamic_modint<DynamicModint<Id>> : std::true_type {};

template<class Mint>
Mint checked_mint_raw(long long value) {
    if (value < 0 || value >= Mint{}.mod())
        throw std::out_of_range("mint_raw: value must be in [0, mod)");
    if constexpr (is_checked_dynamic_modint<Mint>::value)
        return Mint::from_residue(static_cast<unsigned int>(value));
    else
        return Mint::raw(static_cast<int>(value));
}

template<class T> void validate_current_modints(const T& value);

template<class T>
void validate_current_modints(const std::optional<T>& value) {
    if (value) validate_current_modints(*value);
}

template<class T>
void validate_current_modints(const T& value) {
    if constexpr (is_checked_dynamic_modint<T>::value) {
        (void)value.val();
    } else if constexpr (is_tuple<T>::value) {
        std::apply([](const auto&... items) { (validate_current_modints(items), ...); }, value);
    } else if constexpr (requires { value.kuma_gen_value_fields(); }) {
        validate_current_modints(value.kuma_gen_value_fields());
    } else if constexpr (ac_detail::io::is_range_v<T>) {
        for (const auto& item : value) validate_current_modints(item);
    }
}

template<class Mint, class Storage>
class ModulusBound {
    Mint generation_{};
    Storage storage_;

    template<class... Args>
    static Storage initialize(Args&&... args) {
        (validate_current_modints(args), ...);
        return Storage(std::forward<Args>(args)...);
    }

public:
    ModulusBound() = default;

    template<class... Args>
    requires (sizeof...(Args) > 0 && std::is_constructible_v<Storage, Args&&...>)
    explicit ModulusBound(Args&&... args) : storage_(initialize(std::forward<Args>(args)...)) {}

    Storage& current() { (void)generation_.val(); return storage_; }
    const Storage& current() const { (void)generation_.val(); return storage_; }

    // Used for SparseTable, whose build keeps an operation but no stored identity.
    void rebuild_without_identity(const std::vector<Mint>& values) {
        validate_current_modints(values);
        storage_.build(values);
        generation_ = Mint{};
    }
};

template<class Storage, class... Mints>
class LockedModulusStorage {
    using Locks = std::tuple<typename Mints::ModulusLock...>;
    std::tuple<Mints...> generations_{};
    Storage storage_;

    template<class Arg>
    static Storage initialize(Arg&& arg) {
        const Locks locks;
        validate_current_modints(arg);
        return Storage(std::forward<Arg>(arg));
    }

    template<class Self, class Function>
    static auto evaluate(Self& self, Function&& operation) {
        validate_current_modints(self.generations_);
        const Locks locks;
        return std::forward<Function>(operation)(self.storage_);
    }

public:
    template<class Arg>
    requires std::is_constructible_v<Storage, Arg&&>
    explicit LockedModulusStorage(Arg&& arg) : storage_(initialize(std::forward<Arg>(arg))) {}

    template<class Function>
    auto with_current(Function&& operation) {
        return evaluate(*this, std::forward<Function>(operation));
    }
    template<class Function>
    auto with_current(Function&& operation) const {
        return evaluate(*this, std::forward<Function>(operation));
    }
};
#endif

#ifdef AC_HAS_ACL
template<class T>
inline constexpr bool mixed_aggregate_dynamic_element = is_checked_dynamic_modint<T>::value;
#else
template<class T>
inline constexpr bool mixed_aggregate_dynamic_element = false;
#endif

template<class T>
void validate_mixed_aggregate_element(const T& value) {
#ifdef AC_HAS_ACL
    validate_current_modints(value);
#else
    (void)value;
#endif
}

// The mixed Kuma member forms retain the checked key/value contract without
// changing the more general C++ free functions.
template<class T> struct MixedAggregateKey : KumaOrderedValue<T> {};
template<class T>
struct MixedAggregateKey<::ac::descending_integral_key<T>> : std::true_type {};
template<class... Ts>
struct MixedAggregateKey<std::tuple<Ts...>>
    : std::bool_constant<((!std::is_reference_v<Ts> && MixedAggregateKey<std::remove_cv_t<Ts>>::value) && ...)> {};
template<class A, class B>
struct MixedAggregateKey<std::pair<A, B>> : MixedAggregateKey<std::tuple<A, B>> {};

template<class R, class Key>
auto mixed_count_by(const R& values, Key&& key) {
    auto checked_key = [&](const auto& item) {
        validate_mixed_aggregate_element(item);
        auto result = std::invoke(key, item);
        static_assert(MixedAggregateKey<decltype(result)>::value,
                      "count_by requires an owning ordered key");
        validate_mixed_aggregate_element(item);
        return result;
    };
    return ::ac_detail::count_by_impl(values, checked_key);
}

template<class R, class Key, class Value>
auto mixed_sum_by_key(const R& values, Key&& key, Value&& project) {
    auto checked_key = [&](const auto& item) {
        validate_mixed_aggregate_element(item);
        auto result = std::invoke(key, item);
        static_assert(MixedAggregateKey<decltype(result)>::value,
                      "sum_by_key requires an owning ordered key");
        validate_mixed_aggregate_element(item);
        return result;
    };
    auto checked_project = [&](const auto& item) {
        validate_mixed_aggregate_element(item);
        auto result = std::invoke(project, item);
        static_assert(std::same_as<decltype(result), int> ||
                      std::same_as<decltype(result), long long> ||
                      std::same_as<decltype(result), double>,
                      "sum_by_key requires i32, i64, or f64 values");
        validate_mixed_aggregate_element(item);
        return result;
    };
    return ::ac_detail::sum_by_key_impl(values, checked_key, checked_project);
}

template<class T>
T read() {
    if constexpr (requires(T& v) { kuma_fields(v); }) {
        T value{};
        std::apply([&](auto&... fields) {
            ((fields = read<std::remove_cvref_t<decltype(fields)>>()), ...);
        }, kuma_fields(value));
        return value;
    } else if constexpr (is_fixed_array<T>::value) {
        T result{};
        for (auto& value : result) value = read<typename T::value_type>();
        return result;
    } else if constexpr (is_tuple<T>::value) {
        T value{};
        std::apply([&](auto&... fields) {
            ((fields = read<std::remove_cvref_t<decltype(fields)>>()), ...);
        }, value);
        return value;
#ifdef AC_HAS_ACL
    } else if constexpr (atcoder::internal::is_static_modint<T>::value || is_checked_dynamic_modint<T>::value) {
        return T(read<long long>());
#endif
    } else {
        T value = ::rd<T>();
        check_input();
        return value;
    }
}

template<class T>
void validate_axis_keys(const T& values) {
    if constexpr (ac_detail::io::is_range_v<T>) {
        for (const auto& value : values) validate_axis_keys(value);
    } else {
        ::ac_detail::validate_ordered_finite(values);
    }
}

template<class R>
void scan_kuma_sequence(R& target) {
    using Element = typename R::value_type;
    for (auto&& element : target) {
        if constexpr (is_vector<Element>::value || is_deque<Element>::value)
            scan_kuma_sequence(element);
        else
            element = read<Element>();
    }
}

template<std::size_t Axis, class R>
auto checked_axis_sum(const R& values) {
    if (!::ac_detail::nested_sum::rectangular(values))
        throw std::invalid_argument("axis_sum: expected a rectangular array");
    return ::ac_detail::nested_sum::axis_sum_impl<Axis, true>(values);
}

template<std::size_t Axis, bool Maximum, class R>
auto checked_axis_extreme(const R& values) {
    if (!::ac_detail::nested_sum::rectangular(values))
        throw std::invalid_argument("axis_min/axis_max: expected a rectangular array");
    validate_axis_keys(values);
    if constexpr (Maximum) return ::axis_max<Axis>(values);
    else return ::axis_min<Axis>(values);
}

// Give the owned Kuma views a forward iterator for standard range algorithms.
// A copied cursor keeps its position; a dereference always yields an owning value.
template<class T, class R>
class ForwardValues {
    const R* values_;
    using Cursor = decltype(std::begin(std::declval<const R&>()));
    using End = decltype(std::end(std::declval<const R&>()));

public:
    explicit ForwardValues(const R& values) : values_(&values) {}

    struct Iterator {
        using iterator_concept = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        const R* source = nullptr;
        Cursor cursor{};
        std::uint64_t position = 0;

        T operator*() const { return T(*cursor); }
        Iterator& operator++() {
            ++cursor;
            ++position;
            return *this;
        }
        Iterator operator++(int) {
            auto previous = *this;
            ++*this;
            return previous;
        }
        bool operator==(const Iterator& other) const {
            return source == other.source && position == other.position;
        }
        friend difference_type operator-(const Iterator& left, const Iterator& right) {
            const auto delta = static_cast<__int128>(left.position) - right.position;
            if (delta < std::numeric_limits<difference_type>::min()
                    || delta > std::numeric_limits<difference_type>::max())
                throw std::overflow_error("range index exceeds iterator difference range");
            return static_cast<difference_type>(delta);
        }
    };

    struct Sentinel {
        End end{};
        friend bool operator==(const Iterator& iterator, const Sentinel& sentinel) {
            return iterator.cursor == sentinel.end;
        }
    };

    Iterator begin() const { return {values_, std::begin(*values_), 0}; }
    Sentinel end() const { return {std::end(*values_)}; }
};

template<class T, class R>
auto forward_values(const R& values) { return ForwardValues<T, R>(values); }

template<bool Last, class Range, class Predicate, class Projection = std::identity>
int checked_view_index(const Range& values, Predicate predicate, Projection projection = {}) {
    std::size_t index = 0;
    std::optional<std::size_t> match;
    for (const auto& item : values) {
        if (std::invoke(predicate, std::invoke(projection, item))) {
            if constexpr (!Last) return ::ac_detail::checked_range_index(index);
            match = index;
        }
        ++index;
    }
    return ::ac_detail::checked_range_index(match.value_or(index));
}

template<class Range, class Predicate = std::equal_to<>, class Projection = std::identity>
int checked_view_adjacent_index(const Range& values, Predicate predicate = {}, Projection projection = {}) {
    auto left = std::begin(values);
    const auto end = std::end(values);
    if (left == end) return 0;
    auto right = left;
    ++right;
    std::size_t index = 0;
    for (; right != end; ++left, ++right, ++index) {
        const auto a = std::invoke(projection, *left);
        const auto b = std::invoke(projection, *right);
        if (std::invoke(predicate, a, b)) return ::ac_detail::checked_range_index(index);
    }
    return ::ac_detail::checked_range_index(index + 1);
}

enum class AdjacentKind { All, Any, Filter };

template<class T, std::size_t Width, AdjacentKind Kind, class R, class Pred>
auto checked_view_adjacent(const R& values, Pred predicate) {
    auto first = std::begin(values);
    const auto last = std::end(values);
    if constexpr (Kind == AdjacentKind::Filter) {
        std::vector<T> result;
        auto it = first;
        for (std::size_t i = 1; i < Width && it != last; ++i, ++it)
            result.emplace_back(*it);
        ::ac_detail::adjacent::for_each_window<Width>(first, last, [&](const auto& window) {
            if (::ac_detail::adjacent::matches(window, predicate)) result.emplace_back(*window.back());
            return true;
        });
        return result;
    } else {
        constexpr bool all = Kind == AdjacentKind::All;
        bool result = all;
        ::ac_detail::adjacent::for_each_window<Width>(first, last, [&](const auto& window) {
            if (::ac_detail::adjacent::matches(window, predicate) == all) return true;
            result = !all;
            return false;
        });
        return result;
    }
}

template<class T, bool Minimum, bool Maximum, class R>
auto checked_view_extrema(const R& values) {
    std::optional<T> low, high;
    for (const auto& element : values) {
        T value(element);
        ::ac_detail::validate_ordered_finite(value);
        if constexpr (Minimum) {
            if (!low || value < *low) low.emplace(value);
        }
        if constexpr (Maximum) {
            if (!high || *high < value) high.emplace(std::move(value));
        }
    }
    if constexpr (Minimum && Maximum) {
        if (!low) return std::optional<std::pair<T, T>>{};
        return std::optional<std::pair<T, T>>(std::in_place, std::move(*low), std::move(*high));
    } else if constexpr (Minimum) return low;
    else return high;
}

template<bool Flip, class R>
auto checked_swap_key_value(const R& values) {
    using Item = std::ranges::range_value_t<R>;
    if constexpr (Flip) {
        using Key = std::remove_cvref_t<decltype(std::get<1>(std::declval<Item>()))>;
        using Mapped = std::remove_cvref_t<decltype(std::get<0>(std::declval<Item>()))>;
        std::map<Key, Mapped> result;
        for (const auto& item : values) {
            const auto& key = std::get<1>(item);
            ::ac_detail::validate_ordered_finite(key);
            if (!result.try_emplace(key, std::get<0>(item)).second)
                throw std::invalid_argument("swap_key_value: duplicated values are not invertible");
        }
        return result;
    } else {
        if (values.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
            throw std::length_error("swap_key_value: input too large for i32 indices");
        std::map<Item, int> result;
        int index = 0;
        for (const auto& key : values) {
            ::ac_detail::validate_ordered_finite(key);
            if (!result.try_emplace(key, index++).second)
                throw std::invalid_argument("swap_key_value: duplicated values are not invertible");
        }
        return result;
    }
}

template<class I>
std::size_t checked_vector_index(I index, std::size_t size, bool allow_end,
                                 const char* bounds_error = "vector index out of range") {
    if constexpr ((!std::integral<I> && !std::same_as<I, __int128>
                   && !std::same_as<I, unsigned __int128>) || std::same_as<I, bool>) {
        throw std::invalid_argument("vector index must have an integer type other than bool");
    } else {
        if constexpr (std::numeric_limits<I>::is_signed) {
            if (index < 0) throw std::out_of_range(bounds_error);
        }
        if constexpr (std::numeric_limits<I>::digits > std::numeric_limits<std::size_t>::digits) {
            if (index > static_cast<I>(std::numeric_limits<std::size_t>::max()))
                throw std::out_of_range(bounds_error);
        }
        const auto offset = static_cast<std::size_t>(index);
        if (offset > size || (!allow_end && offset == size))
            throw std::out_of_range(bounds_error);
        return offset;
    }
}

template<class T, class I>
void vector_insert_at(std::vector<T>& values, I index, T&& item) {
    const auto offset = checked_vector_index(index, values.size(), true);
    values.insert(values.begin() + static_cast<typename std::vector<T>::difference_type>(offset),
                  std::move(item));
}

template<class T, class I>
void vector_erase_at(std::vector<T>& values, I index) {
    const auto offset = checked_vector_index(index, values.size(), false);
    values.erase(values.begin() + static_cast<typename std::vector<T>::difference_type>(offset));
}

template<class T, class L, class R>
void vector_erase_range(std::vector<T>& values, L left, R right) {
    const auto size = values.size();
    const auto first = checked_vector_index(left, size, true);
    const auto last = checked_vector_index(right, size, true);
    if (first > last) throw std::out_of_range("vector erase_range endpoints are reversed");
    if (first == last) return;
    using Difference = typename std::vector<T>::difference_type;
    values.erase(values.begin() + static_cast<Difference>(first),
                 values.begin() + static_cast<Difference>(last));
}

template<class T, class I>
T vector_pop_at(std::vector<T>& values, I index) {
    const auto offset = checked_vector_index(index, values.size(), false);
    T item = values[offset];
    values.erase(values.begin() + static_cast<typename std::vector<T>::difference_type>(offset));
    return item;
}

template<class R, class I>
auto sequence_index_iterator(R& values, I index, bool allow_end) {
    const auto offset = checked_vector_index(index, values.size(), allow_end, "sequence index out of range");
    using Difference = typename R::difference_type;
    if (offset > static_cast<std::size_t>(std::numeric_limits<Difference>::max()))
        throw std::out_of_range("sequence index out of range");
    return std::next(values.begin(), static_cast<Difference>(offset));
}

template<class Target, class Expected, class Value>
constexpr decltype(auto) sequence_literal_numeric(Value&& value) {
    if constexpr (std::same_as<Target, Expected>) return static_cast<Expected>(value);
    else return std::forward<Value>(value);
}

template<class T> inline constexpr bool is_index_edit_sequence = false;
template<class T, class A> inline constexpr bool is_index_edit_sequence<std::vector<T, A>> = true;
template<class T, class A> inline constexpr bool is_index_edit_sequence<std::deque<T, A>> = true;
template<class T, class A> inline constexpr bool is_index_edit_sequence<std::list<T, A>> = true;
template<class T, class A> inline constexpr bool is_index_edit_sequence<std::basic_string<char, T, A>> = true;

template<class T> inline constexpr bool is_range_push_adaptor = false;
template<class T, class C> inline constexpr bool is_range_push_adaptor<std::queue<T, C>> = true;
template<class T, class C> inline constexpr bool is_range_push_adaptor<std::stack<T, C>> = true;
template<class T, class C, class Compare>
inline constexpr bool is_range_push_adaptor<std::priority_queue<T, C, Compare>> = true;

template<class R, class I>
void sequence_insert_at(R& values, I index, typename R::value_type&& item) {
    values.insert(sequence_index_iterator(values, index, true), std::move(item));
}

template<class R, class I>
void sequence_erase_at(R& values, I index) {
    values.erase(sequence_index_iterator(values, index, false));
}

template<class R, class L, class U>
void sequence_erase_range(R& values, L left, U right) {
    const auto first_index = checked_vector_index(left, values.size(), true, "sequence index out of range");
    const auto last_index = checked_vector_index(right, values.size(), true, "sequence index out of range");
    if (first_index > last_index) throw std::out_of_range("sequence erase_range endpoints are reversed");
    if (first_index == last_index) return;
    auto first = sequence_index_iterator(values, first_index, true);
    auto last = sequence_index_iterator(values, last_index, true);
    values.erase(first, last);
}

template<class R, class I>
typename R::value_type sequence_pop_at(R& values, I index) {
    auto at = sequence_index_iterator(values, index, false);
    typename R::value_type item = *at;
    values.erase(at);
    return item;
}

template<class T, class Less = std::less<>>
void list_merge(std::list<T>& target, std::list<T>& source, Less less = {}) {
    if (&target == &source) return;
    if (source.size() > target.max_size() - target.size())
        throw std::length_error("merge: combined size exceeds list max_size");
    if constexpr (std::is_same_v<Less, std::less<>>) {
        for (const auto& value : target) ::ac_detail::validate_ordered_finite(value);
        for (const auto& value : source) ::ac_detail::validate_ordered_finite(value);
    }
    const auto compare = std::ref(less);
    if (!std::is_sorted(target.cbegin(), target.cend(), compare)
            || !std::is_sorted(source.cbegin(), source.cend(), compare))
        throw std::invalid_argument("merge: both lists must be sorted");
    target.merge(source, compare);
}

template<class T>
void list_splice(std::list<T>& target, long long position, std::list<T>& source,
                 std::optional<long long> left = {}, std::optional<long long> right = {}) {
    const auto pos = checked_vector_index(position, target.size(), true, "splice: destination out of range");
    const bool same = &target == &source;
    const auto first = left ? checked_vector_index(*left, source.size(), right.has_value(),
                                                   "splice: source index out of range") : 0;
    const auto last = right ? checked_vector_index(*right, source.size(), true, "splice: source index out of range")
                            : left ? first + 1 : source.size();
    if (first > last) throw std::out_of_range("splice: source endpoints are reversed");
    if (same && !left) throw std::invalid_argument("splice: moving all nodes requires distinct lists");
    if (same && right && first <= pos && pos < last)
        throw std::invalid_argument("splice: destination lies in the moved range");
    if (!same && last - first > target.max_size() - target.size())
        throw std::length_error("splice: combined size exceeds list max_size");
    if (first == last) return;
    using Difference = typename std::list<T>::difference_type;
    const auto destination = std::next(target.cbegin(), static_cast<Difference>(pos));
    if (!left) target.splice(destination, source);
    else {
        const auto begin = std::next(source.cbegin(), static_cast<Difference>(first));
        if (right) target.splice(destination, source, begin, std::next(begin, static_cast<Difference>(last - first)));
        else target.splice(destination, source, begin);
    }
}

template<class T>
std::set<T> read_set(int size) {
    std::set<T> result;
    for (int i = 0; i < size; ++i)
        ::ac_detail::collection::insert_checked(result, read<T>());
    return result;
}

template<class C, class R>
C associative_from_rows(const R& rows) {
    C result;
    for (const auto& row : rows) {
        if constexpr (requires { typename C::mapped_type; }) {
            ::ac_detail::validate_ordered_finite(std::get<0>(row));
            if constexpr (requires { result.insert_or_assign(std::get<0>(row), std::get<1>(row)); })
                result.insert_or_assign(std::get<0>(row), std::get<1>(row));
            else
                result.emplace(std::get<0>(row), std::get<1>(row));
        } else {
            ::ac_detail::validate_ordered_finite(row);
            result.emplace(row);
        }
    }
    return result;
}

template<class Map>
bool dict_try_emplace_default(Map& target, typename Map::key_type&& key) {
    using Mapped = typename Map::mapped_type;
    if constexpr (std::is_default_constructible_v<Mapped>) {
        return target.try_emplace(std::move(key)).second;
    } else {
        static_assert(std::is_default_constructible_v<Mapped>,
                      "try_emplace: a value is required for this mapped type");
        return false;
    }
}

template<class Entry, class Container>
std::optional<Entry> associative_extract(Container& source, const typename Container::key_type& key) {
    if constexpr (std::is_nothrow_move_constructible_v<Entry>) {
        auto node = source.extract(key);
        if (node.empty()) return std::nullopt;
        if constexpr (requires { typename Container::mapped_type; })
            return std::optional<Entry>(std::in_place, std::move(node.key()), std::move(node.mapped()));
        else
            return std::optional<Entry>(std::in_place, std::move(node.value()));
    } else {
        const auto position = [&] {
            if constexpr (requires { source.key_comp(); }) {
                const auto [first, last] = source.equal_range(key);
                return first == last ? source.end() : first;
            } else {
                return source.find(key);
            }
        }();
        if (position == source.end()) return std::nullopt;
        // The return object is constructed before local objects are destroyed.
        // If copying throws, leave the entry in its original container.
        struct EraseAfterCopy {
            Container& source;
            typename Container::iterator position;
            int exceptions = std::uncaught_exceptions();

            ~EraseAfterCopy() {
                if (std::uncaught_exceptions() == exceptions) source.erase(position);
            }
        } erase{source, position};
        return std::optional<Entry>(std::in_place, *position);
    }
}

template<class C>
C read_associative(long long n) {
    const int size = dimension(n);
    C result;
    for (int i = 0; i < size; ++i) {
        auto key = read<typename C::key_type>();
        ::ac_detail::validate_ordered_finite(key);
        if constexpr (requires { typename C::mapped_type; }) {
            auto value = read<typename C::mapped_type>();
            if constexpr (requires { result.insert_or_assign(std::move(key), std::move(value)); })
                result.insert_or_assign(std::move(key), std::move(value));
            else
                result.emplace(std::move(key), std::move(value));
        } else {
            result.emplace(std::move(key));
        }
    }
    return result;
}

template<class T>
struct SetPriorityOrder {
    bool min_first = false;
    bool operator()(const T& a, const T& b) const { return min_first ? b < a : a < b; }
};

template<class T>
using SetPriorityQueue = ::setpq<T, SetPriorityOrder<T>>;

template<class T>
SetPriorityQueue<T> make_setpq(const std::vector<T>& values, bool min_first) {
    SetPriorityQueue<T> result(SetPriorityOrder<T>{min_first});
    for (const auto& value : values) {
        ::ac_detail::validate_ordered_finite(value);
        result.push(value);
    }
    return result;
}

template<class T>
SetPriorityQueue<T> read_setpq(long long count, bool min_first) {
    const int size = dimension(count);
    SetPriorityQueue<T> result(SetPriorityOrder<T>{min_first});
    for (int i = 0; i < size; ++i) {
        auto value = read<T>();
        ::ac_detail::validate_ordered_finite(value);
        result.push(std::move(value));
    }
    return result;
}

template<class Heap>
Heap make_checked_heap(std::vector<typename Heap::value_type> values) {
    for (const auto& value : values) ::ac_detail::validate_ordered_finite(value);
    return Heap(typename Heap::value_compare{}, std::move(values));
}

template<class C, bool CheckFinite = false>
C read_container(long long n) {
    const int size = dimension(n);
    C result;
    using T = typename C::value_type;
    for (int i = 0; i < size; ++i) {
        if constexpr (requires(C& container, T value) { container.push_back(std::move(value)); })
            result.push_back(read<T>());
        else if constexpr (CheckFinite) {
            auto value = read<T>();
            ::ac_detail::validate_ordered_finite(value);
            result.push(std::move(value));
        } else
            result.push(read<T>());
    }
    return result;
}

template<class K, class V>
std::map<K, V> read_dict(long long n) {
    const int size = dimension(n);
    std::map<K, V> result;
    for (int i = 0; i < size; ++i) {
        auto key = read<K>();
        ::ac_detail::validate_ordered_finite(key);
        auto value = read<V>();
        result.insert_or_assign(std::move(key), std::move(value));
    }
    return result;
}

template<class T>
std::set<T> set_from_vector(const std::vector<T>& values) {
    std::set<T> result;
    for (const auto& item : values)
        ::ac_detail::collection::insert_checked(result, item);
    return result;
}

template<class T>
std::vector<T> read_vec(long long n, long long base = 0) {
    const int padding = base > 0 ? dimension(base) : 0;
    const int size = dimension(static_cast<long long>(dimension(n)) + padding);
    if (static_cast<std::size_t>(size) > std::vector<T>{}.max_size()) {
        error("input vector exceeds max_size");
    }
    if (size == 0) return {};
    std::vector<T> values;
    values.reserve(size);
    values.resize(padding);
    for (int i = padding; i < size; ++i) values.push_back(read<T>());
    return values;
}

template<class T>
std::vector<std::vector<T>> read_mat(long long h, long long w, long long base = 0) {
    const int padding = base > 0 ? dimension(base) : 0;
    const int height = dimension(static_cast<long long>(dimension(h)) + padding);
    const int width = dimension(static_cast<long long>(dimension(w)) + padding);
    const auto rows = static_cast<std::size_t>(height);
    const auto cols = static_cast<std::size_t>(width);
    const auto element_limit = std::vector<T>{}.max_size();
    if (rows > std::vector<std::vector<T>>{}.max_size() || cols > element_limit) {
        error("input matrix dimension exceeds max_size");
    }
    if (cols != 0 && rows > std::numeric_limits<std::size_t>::max() / cols) {
        error("input matrix size overflows size_t");
    }
    if (rows * cols > element_limit) error("input matrix exceeds max_size");
    // in_mat constructs an inner row even when height is zero; avoid that allocation.
    if (height == 0 || width == 0) return std::vector<std::vector<T>>(rows);
    std::vector<std::vector<T>> values;
    values.reserve(height);
    for (int i = 0; i < padding; ++i) values.emplace_back(cols);
    for (int i = padding; i < height; ++i) values.push_back(read_vec<T>(w, padding));
    return values;
}

template<class T>
std::vector<std::vector<std::vector<T>>> read_mat3(long long n, long long m,
                                                  long long k, long long base = 0) {
    using Row = std::vector<T>;
    using Plane = std::vector<Row>;
    using Volume = std::vector<Plane>;
    const auto padding = base > 0 ? static_cast<std::size_t>(dimension(base)) : 0;
    const auto size_with_padding = [padding](long long extent) {
        const auto size = static_cast<long long>(dimension(extent)) + static_cast<long long>(padding);
        return static_cast<std::size_t>(dimension(size));
    };
    const auto depth = size_with_padding(n), height = size_with_padding(m), width = size_with_padding(k);
    const auto rows = static_cast<__int128>(depth) * height;
    if (depth > Volume{}.max_size() || height > Plane{}.max_size() || width > Row{}.max_size()
        || rows > Plane{}.max_size() || rows * width > Row{}.max_size()) {
        error("input 3D array exceeds max_size");
    }

    Volume values(depth);
    for (std::size_t i = 0; i < depth; ++i) {
        auto& plane = values[i];
        plane.resize(height);
        for (std::size_t j = 0; j < height; ++j) {
            auto& row = plane[j];
            if (i < padding || j < padding) {
                row.resize(width);
                continue;
            }
            row.reserve(width);
            row.resize(padding);
            while (row.size() < width) row.push_back(read<T>());
        }
    }
    return values;
}

template<class T>
std::vector<T> read_all() {
    std::vector<T> result;
    while (true) {
        std::cin >> std::ws;
        if (std::cin.bad()) error("input failed");
        if (std::cin.eof()) break;
        check_input();
        result.push_back(read<T>());
    }
    return result;
}

inline std::string read_line() {
    std::string value;
    std::getline(std::cin, value);
    check_input();
    if (!value.empty() && value.back() == '\r') value.pop_back();
    return value;
}

inline std::string read_line_after_whitespace() {
    std::cin >> std::ws;
    return read_line();
}

template<class T> requires std::is_same_v<T, std::string>
std::string read_line_as() {
    return read_line();
}

inline std::vector<std::string> read_all_lines() {
    std::vector<std::string> result;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        check_input();
        result.push_back(std::move(line));
    }
    if (std::cin.bad()) error("input failed");
    if (!std::cin.eof()) check_input();
    return result;
}

template<class T> requires std::is_same_v<T, std::string>
std::vector<std::string> read_all_lines_as() {
    return read_all_lines();
}

inline void discard_line() {
    check_input();
    if (!std::cin.eof()) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        check_input();
    }
}

// Match the generated declaration itself, not C++ classes that inherit it.
template<class T>
concept GeneratedRecord = requires(const T& value) {
    requires std::same_as<typename T::kuma_gen_owned_record, void>;
    typename T::kuma_gen_value_types;
    { &T::kuma_gen_value_fields } ->
        std::same_as<decltype(value.kuma_gen_value_fields()) (T::*)() const>;
};

// One conversion puts this fallback below native insertion overloads,
// including user-defined operator templates with exact stream parameters.
struct RecordOutputFallback {
    std::ostream& stream;
    RecordOutputFallback(std::ostream& output) : stream(output) {}
};

template<class T> struct RecordOptional : std::false_type {};
template<class T> struct RecordOptional<std::optional<T>> : std::true_type {};

// Native range/adaptor insertion templates do not constrain their elements.
// Check the formatter's leaves before advertising an ostream overload.
template<class T, class... Seen>
consteval bool record_output_supported() {
    using U = std::remove_cvref_t<T>;
    if constexpr ((std::same_as<U, Seen> || ...)) {
        return false;
    } else if constexpr (GeneratedRecord<U>) {
        return record_output_supported<decltype(std::declval<const U&>().kuma_gen_value_fields()), Seen..., U>();
    } else if constexpr (is_tuple<U>::value) {
        return []<std::size_t... I>(std::index_sequence<I...>) {
            return (record_output_supported<std::tuple_element_t<I, U>, Seen..., U>() && ...);
        }(std::make_index_sequence<std::tuple_size_v<U>>{});
    } else if constexpr (::ac_detail::io::is_range_v<U>) {
        return record_output_supported<decltype(*std::begin(std::declval<const U&>())), Seen..., U>();
    } else if constexpr (RecordOptional<U>::value || requires { typename U::container_type; typename U::value_type; }) {
        return record_output_supported<typename U::value_type, Seen..., U>() &&
            requires(std::ostream& out, const U& leaf) { out << leaf; };
    } else {
        return requires(std::ostream& out, const U& leaf) { out << leaf; };
    }
}

struct PrintOptions {
    std::string sep = " ";
    std::string row_sep = "\n";
    std::string end = "\n";
    bool flush = false;
};

template<class T>
void write_value_to(const T& value, const PrintOptions& options, std::ostream& output) {
    using U = std::remove_cvref_t<T>;
    if constexpr (is_vector<U>::value || is_deque<U>::value || is_cpp_sequence<U>::value || is_associative<U>::value || is_kuma_view<U>::value) {
        using E = typename U::value_type;
        const auto& separator = (is_vector<E>::value || is_deque<E>::value || is_cpp_sequence<E>::value || is_associative<E>::value || is_tuple<E>::value || requires(const E& v) { kuma_fields(v); })
            ? options.row_sep : options.sep;
        bool first = true;
        for (const auto& item : value) {
            if (!first && !std::is_same_v<E, char>) output << separator;
            first = false;
            if constexpr (is_vector<U>::value && std::is_same_v<E, bool>) write_value_to(static_cast<bool>(item), options, output);
            else write_value_to(item, options, output);
        }
    } else if constexpr (requires { kuma_fields(value); }) {
        write_value_to(kuma_fields(value), options, output);
    } else if constexpr (is_tuple<U>::value) {
        bool first = true;
        std::apply([&](const auto&... fields) {
            auto emit = [&](const auto& field) {
                if (!first) output << options.sep;
                first = false;
                write_value_to(field, options, output);
            };
            (emit(fields), ...);
        }, value);
    } else if constexpr (std::is_same_v<U, bool> || std::is_same_v<U, std::vector<bool>::reference>) {
        output << (static_cast<bool>(value) ? "true" : "false");
    } else if constexpr (std::is_floating_point_v<U>) {
        const auto previous_precision = output.precision();
        try {
            output << std::setprecision(std::numeric_limits<U>::max_digits10) << value;
        } catch (...) {
            output.precision(previous_precision);
            throw;
        }
        output.precision(previous_precision);
    } else {
        output << value;
    }
}

// Keep each composite element distinct; the regular writer flattens tuples.
template<class T>
void write_sorted_list_item(std::ostream& output, const T& value) {
    if constexpr (requires { value.kuma_gen_value_fields(); }) {
        write_sorted_list_item(output, value.kuma_gen_value_fields());
    } else if constexpr (KumaTupleValue<std::remove_cvref_t<T>>::value) {
        output.put('(');
        bool first = true;
        std::apply([&](const auto&... fields) {
            auto emit = [&](const auto& field) {
                if (!first) output << ", ";
                first = false;
                write_sorted_list_item(output, field);
            };
            (emit(fields), ...);
        }, value);
        output.put(')');
    } else {
        write_value_to(value, PrintOptions{}, output);
    }
}

template<class T>
void write_value(const T& value, const PrintOptions& options) {
    write_value_to(value, options, std::cout);
}

template<class... Ts>
void (print)(const PrintOptions& options, const Ts&... values) {
    bool first = true;
    auto emit = [&](const auto& value) {
        if (!first) std::cout << options.sep;
        first = false;
        write_value(value, options);
    };
    (emit(values), ...);
    std::cout << options.end;
    if (options.flush) std::cout.flush();
}

struct Range;
class CombinationIndicesView;
template<class T> class CombinationsView;

// Only preview exact Kuma ranges that can be restarted without consuming input.
template<class T> inline constexpr bool debug_preview_range = false;
template<> inline constexpr bool debug_preview_range<Range> = true;
template<> inline constexpr bool debug_preview_range<CombinationIndicesView> = true;
template<class T> inline constexpr bool debug_preview_range<CombinationsView<T>> = true;
template<class T> inline constexpr bool debug_preview_range<SubmasksView<T>> = true;

template<class T>
void debug_write_value(const T& value, const PrintOptions& options) {
    if constexpr (debug_preview_range<T>) {
        constexpr std::size_t max_elements = 100; // Same element limit as trace.
        auto it = value.begin();
        const auto end = value.end();
        using Item = std::remove_cvref_t<decltype(*it)>;
        // Match the existing Kuma writer for submasks and stock streaming for
        // the other exact views (including the formatting of combination rows).
        auto separate = [&] {
            if constexpr (is_kuma_view<T>::value) std::cerr << options.sep;
            else std::cerr << ac_detail::io::natural_range_sep_v<Item>;
        };
        std::size_t count = 0;
        for (; it != end && count < max_elements; ++it, ++count) {
            if (count != 0) separate();
            if constexpr (is_kuma_view<T>::value) write_value_to(*it, options, std::cerr);
            else ac_detail::output::write_item(std::cerr, *it);
        }
        if (it != end) {
            separate();
            std::cerr << "...";
        }
    } else {
        write_value_to(value, options, std::cerr);
    }
}

template<class... Ts>
void debug_print(const PrintOptions& options, const Ts&... values) {
    bool first = true;
    auto emit = [&](const auto& value) {
        if (!first) std::cerr << options.sep;
        first = false;
        debug_write_value(value, options);
    };
    (emit(values), ...);
    std::cerr << options.end;
    if (options.flush) std::cerr.flush();
}

template<class T>
auto sum(const std::vector<T>& values) {
    static_assert(std::is_arithmetic_v<T>, "sum requires a one-dimensional numeric vector");
    using Result = std::conditional_t<std::is_integral_v<T>, std::common_type_t<long long, T>, T>;
    Result result{};
    for (const auto& value : values) result += static_cast<Result>(value);
    return result;
}

template<class T, class F>
auto map(const std::vector<T>& values, F callback) {
    // The generated callback takes owned values; map_vec materializes its result.
    return ::map_vec(values, std::move(callback));
}

template<class T, class F>
std::vector<T> filter(const std::vector<T>& values, F callback) {
    // Evaluate every predicate now, retaining the input order and no callback.
    return ::filter_vec(values, std::move(callback));
}

template<class R, class Key, class Compare>
auto argsort_by(const R& values, Key key, Compare compare) {
    using KeyValue = std::decay_t<std::invoke_result_t<Key&, std::ranges::range_reference_t<const R>>>;
    std::vector<KeyValue> keys;
    keys.reserve(::ac_detail::checked_range_index(values.size()));
    for (const auto& value : values) {
        auto projected = std::invoke(key, value);
        ::ac_detail::validate_ordered_finite(projected);
        keys.push_back(std::move(projected));
    }
    return ::argsort(keys, std::move(compare));
}

template<class T>
void sort(std::vector<T>& values) {
    std::sort(values.begin(), values.end());
}

template<class T>
long long algorithm_index(T value) {
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>,
                  "algorithm start/end must be integer indices");
    return numeric_cast<long long>(value);
}

template<class R>
auto algorithm_bounds(R& values, long long start, std::optional<long long> end) {
    const auto size = numeric_cast<long long>(std::size(values));
    const auto stop = end.value_or(size);
    if (start < 0 || stop < start || stop > size)
        error("algorithm range requires 0 <= start <= end <= size");
    const auto first = std::begin(values);
    using Difference = std::iter_difference_t<decltype(first)>;
    const auto left = std::next(first, static_cast<Difference>(start));
    return std::pair{left, std::next(left, static_cast<Difference>(stop - start))};
}

template<class R, class F = std::less<>>
void sort_range(R& values, long long start, std::optional<long long> end, F compare = {}) {
    auto [first, last] = algorithm_bounds(values, start, end);
    std::sort(first, last, std::move(compare));
}

template<bool Stable, class R, class Validate, class Compare, class Projection = std::identity>
void sequence_sort(R& values, long long start, std::optional<long long> end, Validate validate,
                   Compare compare, Projection project = {}) {
    auto [first, last] = algorithm_bounds(values, start, end);
    if constexpr (!std::same_as<Validate, std::nullptr_t>)
        for (auto it = first; it != last; ++it) validate(*it);
    const auto less = [&](const auto& left, const auto& right) {
        return std::invoke(compare, std::invoke(project, left), std::invoke(project, right));
    };
    if constexpr (!std::ranges::random_access_range<R>) values.sort(less);
    else if constexpr (Stable) std::stable_sort(first, last, less);
    else std::sort(first, last, less);
}

template<class R>
auto sequence_sort_copy(R&& values) {
    if constexpr (std::same_as<std::remove_cvref_t<R>, std::string_view>)
        return std::string(values);
    else
        return std::remove_cvref_t<R>(std::forward<R>(values));
}

template<class T, std::size_t N>
auto sequence_sort_copy(std::span<T, N> values) {
    return std::vector<std::remove_cv_t<T>>(values.begin(), values.end());
}

template<bool Stable, class R, class Validate, class Compare, class Projection>
void sequence_sort_mixed(R& values, long long start, std::optional<long long> end,
                         Validate validate, Compare compare, Projection project) {
    sequence_sort<Stable>(values, start, end, std::move(validate), std::move(compare), std::move(project));
}

template<bool Stable, class T, class A, class Validate, class Compare, class Projection>
void sequence_sort_mixed(std::forward_list<T, A>& values, long long start, std::optional<long long> end,
                         Validate validate, Compare compare, Projection project) {
    if (start != 0 || end) error("list sort requires the whole sequence");
    if constexpr (!std::same_as<Validate, std::nullptr_t>)
        for (const auto& item : values) validate(item);
    values.sort([&](const auto& left, const auto& right) {
        return std::invoke(compare, std::invoke(project, left), std::invoke(project, right));
    });
}

template<class R>
void reverse_range(R& values, long long start, std::optional<long long> end) {
    auto [first, last] = algorithm_bounds(values, start, end);
    std::reverse(first, last);
}

template<class T>
std::vector<T> sorted(std::vector<T> values) {
    kuma_rt::sort(values);
    return values;
}

template<class T>
void reverse(std::vector<T>& values) {
    std::reverse(values.begin(), values.end());
}

template<class T>
std::vector<T> reversed(std::vector<T> values) {
    kuma_rt::reverse(values);
    return values;
}

struct Range {
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        long long value = 0, stop = 0, step = 1;
        bool done = true;
        long long operator*() const { return value; }
        Iterator& operator++() {
            const __int128 next = static_cast<__int128>(value) + step;
            done = step > 0 ? next >= stop : next <= stop;
            if (!done) value = static_cast<long long>(next);
            return *this;
        }
        Iterator operator++(int) {
            auto previous = *this;
            ++*this;
            return previous;
        }
        bool operator==(const Iterator& other) const {
            if (done || other.done) return done && other.done;
            return value == other.value && stop == other.stop && step == other.step;
        }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };
    long long first, last, step = 1;
    Iterator begin() const { return {first, last, step, step > 0 ? first >= last : first <= last}; }
    Iterator end() const { return {last, last, step, true}; }
};

inline Range range(long long begin, long long end, long long step = 1) {
    if (step == 0) error("range step must not be zero");
    return {begin, end, step};
}
inline Range range(long long end) { return range(0, end); }

enum class BoundKind { Lower, Upper, Equal };

template<class R, class Search>
auto checked_view_binary_search(const R& values, Search search) {
    const auto first = std::begin(values);
    const auto end = std::end(values);
    std::uint64_t count = 0;
    if constexpr (std::same_as<R, Range>) {
        if (first != end) {
            const __int128 difference = static_cast<__int128>(values.last) - values.first;
            const __int128 width = difference > 0 ? difference : -difference;
            const __int128 stride = values.step > 0 ? values.step : -static_cast<__int128>(values.step);
            count = static_cast<std::uint64_t>((width - 1) / stride + 1);
        }
    } else {
        for (auto it = first; it != end; ++it) ++count;
    }

    const auto advance = [](auto& it, std::uint64_t offset) {
        if constexpr (std::same_as<R, Range>) {
            const __int128 value = static_cast<__int128>(it.value) + static_cast<__int128>(it.step) * offset;
            it.done = it.step > 0 ? value >= it.stop : value <= it.stop;
            if (!it.done) it.value = static_cast<long long>(value);
        } else {
            for (; offset != 0; --offset) ++it;
        }
    };
    const auto boundary = [&](auto precedes) {
        auto position = first;
        auto remaining = count;
        std::uint64_t index = 0;
        while (remaining != 0) {
            const auto half = remaining / 2;
            auto middle = position;
            advance(middle, half);
            if (precedes(*middle)) {
                position = middle;
                ++position;
                index += half + 1;
                remaining -= half + 1;
            } else {
                remaining = half;
            }
        }
        return std::pair{position, index};
    };
    return search(boundary);
}

template<BoundKind Kind, class R, class T, class Compare = std::less<>, class Projection = std::identity>
auto checked_view_bound(const R& values, const T& target, Compare comp = {}, Projection projection = {}) {
    return checked_view_binary_search(values, [&](const auto& boundary) {
        const auto less_than = [&](const auto& item) { return std::invoke(comp, std::invoke(projection, item), target); };
        const auto at_most = [&](const auto& item) { return !std::invoke(comp, target, std::invoke(projection, item)); };
        if constexpr (Kind == BoundKind::Lower) return ::ac_detail::checked_range_index(boundary(less_than).second);
        else if constexpr (Kind == BoundKind::Upper) return ::ac_detail::checked_range_index(boundary(at_most).second);
        else {
            const int lower = ::ac_detail::checked_range_index(boundary(less_than).second);
            return std::pair{lower, ::ac_detail::checked_range_index(boundary(at_most).second)};
        }
    });
}

template<class R, class T, class Compare = std::less<>, class Projection = std::identity>
bool checked_view_contains(const R& values, const T& target, Compare comp = {}, Projection projection = {}) {
    return checked_view_binary_search(values, [&](const auto& boundary) {
        const auto lower = boundary([&](const auto& item) {
            return std::invoke(comp, std::invoke(projection, item), target);
        }).first;
        return lower != std::end(values) && !std::invoke(comp, target, std::invoke(projection, *lower));
    });
}

template<class R, class Predicate, class Projection = std::identity>
int checked_view_partition(const R& values, Predicate predicate, Projection projection = {}) {
    return checked_view_binary_search(values, [&](const auto& boundary) {
        const auto found = boundary([&](const auto& item) { return std::invoke(predicate, std::invoke(projection, item)); });
        return ::ac_detail::checked_range_index(found.second);
    });
}

template<class T>
concept kuma_mask_integral = std::integral<T> && !std::is_same_v<T, bool> &&
    !std::is_same_v<T, char> && !std::is_same_v<T, signed char> && !std::is_same_v<T, unsigned char> &&
    !std::is_same_v<T, wchar_t> && !std::is_same_v<T, char8_t> && !std::is_same_v<T, char16_t> &&
    !std::is_same_v<T, char32_t> && sizeof(T) <= 8;

template<class T>
void mask_reject_negative(T mask) {
    static_assert(kuma_mask_integral<T>);
    if constexpr (std::is_signed_v<T>) {
        if (mask < 0) error("integer mask must not be negative");
    }
}

template<kuma_mask_integral T, kuma_mask_integral I>
bool mask_has_bit(T mask, I index) {
    mask_reject_negative(mask);
    using U = std::make_unsigned_t<T>;
    constexpr int width = std::numeric_limits<U>::digits;
    // Check before narrowing, including unsigned C++ indices wider than int.
    if (!std::in_range<int>(index) || index < 0 || index >= width)
        throw std::out_of_range("has_bit: bit position out of range");
    return ((static_cast<U>(mask) >> static_cast<int>(index)) & U{1}) != 0;
}

template<class T>
int mask_bit_count(T mask) {
    static_assert(kuma_mask_integral<T>);
    mask_reject_negative(mask);
    using U = std::make_unsigned_t<T>;
    return std::popcount(static_cast<U>(mask));
}

template<class T>
struct SetBitsView {
    static_assert(kuma_mask_integral<T>);
    using value_type = int;
    using Sentinel = std::default_sentinel_t;
    T mask;
    explicit SetBitsView(T value) : mask(value) { mask_reject_negative(mask); }
    struct Iterator {
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::input_iterator_tag;
        std::make_unsigned_t<T> bits;
        bool done = false;
        int operator*() const { return std::countr_zero(bits); }
        Iterator& operator++() {
            bits &= bits - 1;
            done = bits == 0;
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator==(Sentinel) const { return done; }
    };
    Iterator begin() const {
        using U = std::make_unsigned_t<T>;
        const U bits = static_cast<U>(mask);
        return {bits, bits == 0};
    }
    Sentinel end() const { return {}; }
};

template<class T>
struct SubmasksView {
    static_assert(kuma_mask_integral<T>);
    using value_type = T;
    using Sentinel = std::default_sentinel_t;
    T mask;
    explicit SubmasksView(T value) : mask(value) { mask_reject_negative(mask); }
    struct Iterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::input_iterator_tag;
        T sub;
        T mask;
        bool done = false;
        T operator*() const { return sub; }
        Iterator& operator++() {
            if (sub == T{0}) {
                done = true;
                return *this;
            }
            using U = std::make_unsigned_t<T>;
            sub = static_cast<T>((static_cast<U>(sub) - 1) & static_cast<U>(mask));
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator==(Sentinel) const { return done; }
    };
    Iterator begin() const { return {mask, mask, false}; }
    Sentinel end() const { return {}; }
};

template<class T>
concept CollectionInteger = std::same_as<T, int> || std::same_as<T, long long>
    || std::same_as<T, std::size_t>;

// Holds only the input length and choice count. Dereferencing yields an
// independent index row, so saved rows survive advancing the iterator.
class CombinationIndicesView {
    long long n_;
    unsigned long long k_;
public:
    using value_type = std::vector<long long>;
    using Sentinel = std::default_sentinel_t;
    template<CollectionInteger K>
    CombinationIndicesView(std::size_t n, K k) : n_(0), k_(0) {
        if (std::cmp_less(k, 0)) throw std::invalid_argument("combination_indices: negative k");
        if (!std::in_range<long long>(n))
            throw std::length_error("combination_indices: vector length exceeds i64");
        n_ = static_cast<long long>(n);
        k_ = static_cast<unsigned long long>(k);
    }
    struct Iterator {
        using value_type = std::vector<long long>;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::input_iterator_tag;
        long long n;
        value_type row;
        bool done;
        value_type operator*() const { return row; }
        Iterator& operator++() {
            for (std::size_t i = row.size(); i != 0; ) {
                --i;
                if (row[i] != n - static_cast<long long>(row.size() - i)) {
                    ++row[i];
                    for (std::size_t j = i + 1; j < row.size(); ++j)
                        row[j] = row[j - 1] + 1;
                    return *this;
                }
            }
            done = true;
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator==(Sentinel) const { return done; }
    };
    Iterator begin() const {
        if (std::cmp_greater(k_, n_)) return {n_, {}, true};
        value_type row(static_cast<std::size_t>(k_));
        std::iota(row.begin(), row.end(), 0LL);
        return {n_, std::move(row), false};
    }
    Sentinel end() const { return {}; }
};

// Own the source snapshot, not all combinations. Iterators borrow the view;
// each emitted row owns its values and may outlive both iterator and view.
template<class T>
class CombinationsView {
    CombinationIndicesView indices_;
    std::vector<T> values_;
public:
    using value_type = std::vector<T>;
    using Sentinel = std::default_sentinel_t;
    template<CollectionInteger K>
    explicit CombinationsView(const std::vector<T>& values, K k)
        : indices_(values.size(), k),
          values_(k > 0 && std::cmp_less_equal(k, values.size()) ? values : value_type{}) {}
    template<CollectionInteger K>
    explicit CombinationsView(std::vector<T>&& values, K k)
        : indices_(values.size(), k),
          values_(k > 0 && std::cmp_less_equal(k, values.size()) ? std::move(values) : value_type{}) {}
    CombinationsView(const CombinationsView&) = default;
    CombinationsView& operator=(const CombinationsView&) = default;
    // Reset the old index range along with the source values, including when k == 0.
    CombinationsView(CombinationsView&& other) noexcept
        : indices_(std::exchange(other.indices_, CombinationIndicesView(0, 1))),
          values_(std::move(other.values_)) {}
    CombinationsView& operator=(CombinationsView&& other) noexcept {
        if (this != &other) {
            indices_ = std::exchange(other.indices_, CombinationIndicesView(0, 1));
            values_ = std::move(other.values_);
        }
        return *this;
    }
    struct Iterator {
        using value_type = std::vector<T>;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::input_iterator_tag;
        CombinationIndicesView::Iterator indices;
        const std::vector<T>* values;
        value_type operator*() const {
            value_type result;
            result.reserve(indices.row.size());
            for (const long long i : indices.row)
                result.push_back((*values)[static_cast<std::size_t>(i)]);
            return result;
        }
        Iterator& operator++() { ++indices; return *this; }
        void operator++(int) { ++*this; }
        bool operator==(Sentinel end) const { return indices == end; }
    };
    Iterator begin() const & { return {indices_.begin(), &values_}; }
    Iterator begin() const && = delete;
    Sentinel end() const { return {}; }
};

// Keep non-vector calls native, including bit-field arguments and native
// overload resolution. Only vectors need dispatch through a fallback adaptor.
template<class R>
struct CombinationReceiver {
    R&& values;

    template<class K>
    decltype(auto) (combination_indices)(K&& k) {
        if constexpr (requires { (std::forward<R>(values).combination_indices)(std::forward<K>(k)); })
            return (std::forward<R>(values).combination_indices)(std::forward<K>(k));
        else if constexpr (CollectionInteger<std::remove_cvref_t<K>>)
            return CombinationIndicesView(std::forward<R>(values).size(), k);
        else
            return (std::forward<R>(values).combination_indices)(std::forward<K>(k));
    }

    template<class K>
    decltype(auto) (combinations)(K&& k) {
        if constexpr (requires { (std::forward<R>(values).combinations)(std::forward<K>(k)); }) {
            return (std::forward<R>(values).combinations)(std::forward<K>(k));
        } else if constexpr (CollectionInteger<std::remove_cvref_t<K>>) {
            using Elem = typename std::remove_cvref_t<R>::value_type;
            if constexpr (std::same_as<Elem, int> || std::same_as<Elem, long long> ||
                          std::same_as<Elem, double> || std::same_as<Elem, bool> ||
                          std::same_as<Elem, char> || std::same_as<Elem, std::string> ||
                          std::same_as<Elem, std::pair<long long, long long>> ||
                          std::same_as<Elem, std::pair<char, long long>>)
                return CombinationsView<Elem>(std::forward<R>(values), k);
            else
                return (std::forward<R>(values).combinations)(std::forward<K>(k));
        } else {
            return (std::forward<R>(values).combinations)(std::forward<K>(k));
        }
    }
};

template<class R>
constexpr decltype(auto) combination_receiver(R&& values) noexcept {
    if constexpr (is_plain_vector<std::remove_cvref_t<R>>)
        return CombinationReceiver<R>{std::forward<R>(values)};
    else
        return std::forward<R>(values);
}

template<class T>
SetBitsView<T> mask_set_bits(T mask) {
    return SetBitsView<T>(mask);
}

template<class T>
SubmasksView<T> mask_submasks(T mask) {
    return SubmasksView<T>(mask);
}

inline long long sum(const Range& values) {
    const __int128 first = values.first, stop = values.last, step = values.step;
    if (step == 0) error("range step must not be zero");
    if (step > 0 ? first >= stop : first <= stop) return 0;
    const __int128 distance = step > 0 ? stop - first : first - stop;
    const __int128 stride = step > 0 ? step : -step;
    const __int128 count = (distance - 1) / stride + 1;
    const __int128 last = first + (count - 1) * step;
    // A signed-64-bit arithmetic progression's exact sum fits signed 128-bit.
    const __int128 total = count % 2 == 0 ? (count / 2) * (first + last)
                                         : count * ((first + last) / 2);
    if (total < std::numeric_limits<long long>::min() ||
        total > std::numeric_limits<long long>::max())
        error("range sum exceeds signed 64-bit range");
    return static_cast<long long>(total);
}
inline long long cpp_sum(const Range& values) { return sum(values); }

template<class T>
long long range_bound(T value) {
    // Some strict-dialect standard libraries exclude the compiler's 128-bit types from these traits.
    static_assert((std::is_integral_v<T> || std::is_same_v<T, __int128>
                   || std::is_same_v<T, unsigned __int128>) && !std::is_same_v<T, bool>,
                  "Kuma range arguments must be integers (not bool)");
    constexpr auto maximum = std::numeric_limits<long long>::max();
    if constexpr (std::is_signed_v<T> || std::is_same_v<T, __int128>) {
        const auto wide = static_cast<__int128>(value);
        if (wide < std::numeric_limits<long long>::min() || wide > maximum)
            error("range argument exceeds signed 64-bit range");
    } else {
        if (static_cast<unsigned __int128>(value) > static_cast<unsigned __int128>(maximum))
            error("range argument exceeds signed 64-bit range");
    }
    return static_cast<long long>(value);
}
// Braced arguments are evaluated left-to-right, exactly once in mixed code.
inline Range range_args(std::initializer_list<long long> args) {
    auto p = args.begin();
    if (args.size() == 1) return range(p[0]);
    if (args.size() == 2) return range(p[0], p[1]);
    if (args.size() == 3) return range(p[0], p[1], p[2]);
    error("range needs one to three arguments");
}

inline long long count(long long n) {
    if (n < 0) error("repeat count must not be negative");
    return n;
}

template<class C>
decltype(auto) checked_index(C&& container, __int128 index) {
    const __int128 length = static_cast<__int128>(container.size());
    __int128 position = index;
    if (position < 0) position += length;
    if (position < 0 || position >= length) error("index out of range");
    return std::forward<C>(container)[static_cast<std::size_t>(position)];
}
template<class R>
struct AtWrapReceiver {
    R&& source;
    using Item = typename std::remove_cvref_t<R>::value_type;

    template<CollectionInteger I>
    Item at_wrap(I index) const {
        static_assert(ExtremaScalar<Item> || is_extrema_pair<Item>,
                      "at_wrap: expected an owning scalar or flat pair element");
        return static_cast<Item>(checked_index(source, index));
    }
};

template<class R>
constexpr decltype(auto) at_wrap_receiver(R&& source) noexcept {
    using C = std::remove_cvref_t<R>;
    if constexpr ((is_plain_vector<C> || is_deque<C>::value || std::same_as<C, std::string>)
                  && !std::is_volatile_v<std::remove_reference_t<R>>)
        return AtWrapReceiver<R>{std::forward<R>(source)};
    else
        return std::forward<R>(source);  // Native member keeps its exact signature and return type.
}

// Used only within a loop that holds the source alive.
template<class C, bool PairRow = false>
class BorrowedPairwiseView {
    const C* values_;
public:
    explicit BorrowedPairwiseView(const C& values) : values_(&values) {}
    struct Iterator {
        const C* values;
        std::size_t index;
        auto operator*() const {
            using Ref = typename C::const_reference;
            using Row = std::conditional_t<PairRow, std::pair<Ref, Ref>, std::tuple<Ref, Ref>>;
            return Row((*values)[index], (*values)[index + 1]);
        }
        Iterator& operator++() { ++index; return *this; }
        bool operator==(const Iterator&) const = default;
    };
    Iterator begin() const { return {values_, 0}; }
    Iterator end() const { return {values_, values_->empty() ? 0 : values_->size() - 1}; }
};

template<class T> inline constexpr bool is_borrowed_pairwise_view = false;
template<class C, bool PairRow>
inline constexpr bool is_borrowed_pairwise_view<BorrowedPairwiseView<C, PairRow>> = true;

template<bool FlatPair, bool PairRow = false, class R>
decltype(auto) cpp_member_pairwise_view(R&& values) {
    using C = std::remove_cvref_t<R>;
    if constexpr (requires { std::forward<R>(values).pairwise_view(); }) {
        return std::forward<R>(values).pairwise_view();
    } else if constexpr ((is_plain_vector<C> || std::same_as<C, std::string>)
                         && !std::is_volatile_v<std::remove_reference_t<R>>) {
        static_assert(FlatPair, "pairwise_view: use for (x, y) in a.pairwise_view()");
        return BorrowedPairwiseView<C, PairRow>{values};
    } else {
        return std::forward<R>(values).pairwise_view();
    }
}

// Eager adapters preserve left-to-right snapshot semantics in the compiler.
// Explicit element types materialize vector<bool> proxies and map const keys.
template<class C> requires (is_vector<C>::value || std::same_as<C, std::string>)
auto pairwise_copy(const C& values) {
    using Item = typename C::value_type;
    std::vector<std::tuple<Item, Item>> result;
    const auto n = values.size();
    if (n < 2) return result;
    result.reserve(n - 1);
    for (std::size_t i = 1; i < n; ++i)
        result.emplace_back(values[i - 1], values[i]);
    return result;
}

template<class C, CollectionInteger K>
    requires (is_vector<C>::value || std::same_as<C, std::string>)
auto chunks_copy(const C& values, K width) {
    using Chunk = std::conditional_t<std::same_as<C, std::string>,
                                     std::string, std::vector<typename C::value_type>>;
    if (width <= 0) error("chunks: width must be positive");
    std::vector<Chunk> result;
    const auto n = values.size();
    if (n == 0) return result;
    const auto k = std::cmp_greater_equal(width, n) ? n : static_cast<std::size_t>(width);
    result.reserve(n / k + (n % k != 0));
    for (std::size_t i = 0; i < n;) {
        const auto next = i + std::min(k, n - i);
        result.emplace_back(values.begin() + i, values.begin() + next);
        i = next;
    }
    return result;
}

template<class K>
concept WindowsWidth = std::same_as<K, int> || std::same_as<K, long> || std::same_as<K, long long>
    || std::same_as<K, unsigned int> || std::same_as<K, unsigned long> || std::same_as<K, unsigned long long>;

// One immutable snapshot for all windows. A row owns its share independently
// of the outer view; iterators borrow their respective view or row.
template<class C>
class WindowsView {
    std::shared_ptr<const C> values_;
    unsigned long long width_ = 0;
    std::optional<std::vector<std::size_t>> selected_;

    WindowsView(std::shared_ptr<const C> values, unsigned long long width)
        : values_(std::move(values)), width_(width), selected_(std::in_place) {}
    std::size_t offset(std::size_t i) const { return selected_ ? (*selected_)[i] : i; }

public:
    class Row {
        friend class WindowsView;
        std::shared_ptr<const C> values_;
        std::size_t first_;
        std::size_t width_;
        Row(const std::shared_ptr<const C>& values, std::size_t first, std::size_t width)
            : values_(values), first_(first), width_(width) {}
    public:
        using value_type = typename C::value_type;
        using kuma_window_source = C;
        std::size_t size() const { return values_ ? width_ : 0; }
        // A const std::vector<bool> returns bool rather than a proxy here.
        typename C::const_reference operator[](std::size_t i) const & {
            return (*values_)[first_ + i];
        }
        value_type operator[](std::size_t i) const && {
            return (*values_)[first_ + i];
        }
        typename C::const_iterator begin() const & {
            return values_ ? values_->cbegin() + first_ : typename C::const_iterator{};
        }
        auto begin() const && = delete;
        typename C::const_iterator end() const & {
            return values_ ? values_->cbegin() + first_ + width_ : typename C::const_iterator{};
        }
        auto end() const && = delete;

        friend std::ostream& operator<<(std::ostream& out, const Row& row)
            requires std::same_as<C, std::string> {
            if (!row.values_) return out;
            return out << std::string_view(*row.values_).substr(row.first_, row.width_);
        }
        // ac.hpp's native print uses ADL before its generic range formatter.
        friend void write_item(std::ostream& out, const Row& row)
            requires std::same_as<C, std::string> {
            out << row;
        }
    };

    using value_type = Row;

    template<class R, WindowsWidth K>
        requires std::same_as<std::remove_cvref_t<R>, C>
    explicit WindowsView(R&& values, K width) {
        if (width <= 0) error("windows: width must be positive");
        width_ = static_cast<unsigned long long>(width);
        if (std::cmp_greater(width, values.size())) return;
        values_ = std::make_shared<const C>(std::forward<R>(values));
    }

    std::size_t size() const {
        if (selected_) return selected_->size();
        return values_ ? values_->size() - static_cast<std::size_t>(width_) + 1 : 0;
    }
    Row operator[](std::size_t i) const {
        return Row(values_, offset(i), static_cast<std::size_t>(width_));
    }

    struct Iterator {
        using value_type = Row;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::input_iterator_tag;
        const WindowsView* view;
        std::size_t index;
        Row operator*() const { return (*view)[index]; }
        Iterator& operator++() { ++index; return *this; }
        void operator++(int) { ++*this; }
        bool operator==(std::default_sentinel_t) const { return index == view->size(); }
    };
    Iterator begin() const & { return {this, 0}; }
    Iterator begin() const && = delete;
    std::default_sentinel_t end() const { return {}; }

    template<class F>
    auto map(F&& callback) const {
        using Out = std::remove_cvref_t<std::invoke_result_t<F&, const Row&>>;
        std::vector<Out> result;
        result.reserve(size());
        for (const auto& row : *this) result.emplace_back(std::invoke(callback, row));
        return result;
    }

    template<class F>
    WindowsView filter(F&& callback) const {
        WindowsView result(values_, width_);
        for (std::size_t i = 0; i < size(); ++i) {
            if (const auto row = (*this)[i]; std::invoke(callback, row))
                result.selected_->push_back(offset(i));
        }
        return result;
    }

    template<class F, class Acc>
    Acc reduce_rows(F callback, Acc initial) const {
        using Result = std::invoke_result_t<F&, Acc&&, const Row&>;
        static_assert(implicit_fold_assignment<Result, Acc>,
                      "windows.reduce: callback result must match the seed type or widen implicitly");
        if constexpr (implicit_fold_assignment<Result, Acc>)
            for (const auto& row : *this)
                initial = static_cast<Acc>(std::invoke(callback, std::move(initial), row));
        return initial;
    }

    template<std::size_t Arity>
    void validate_binding_count() const {
        if (!std::cmp_equal(Arity, width_)) error("windows: width must match binding count");
    }

    template<std::size_t Arity, class F, class Acc>
    Acc reduce_fields(F&& callback, Acc initial) const {
        if (!std::cmp_equal(Arity, width_)) error("windows: width must match callback arity");
        for (const auto& row : *this) {
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                using Result = decltype(std::invoke(callback, std::move(initial), row[I]...));
                static_assert(implicit_fold_assignment<Result, Acc>,
                              "windows.reduce: callback result must match the seed type or widen implicitly");
                if constexpr (implicit_fold_assignment<Result, Acc>)
                    initial = static_cast<Acc>(std::invoke(callback, std::move(initial), row[I]...));
            }(std::make_index_sequence<Arity>{});
        }
        return initial;
    }

    template<std::size_t Arity, bool Filter, class F>
    auto row_fields(F&& callback) const {
        if (!std::cmp_equal(Arity, width_)) error("windows: width must match callback arity");
        auto expand = [&callback](const Row& row) {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                return std::invoke(callback, row[I]...);
            }(std::make_index_sequence<Arity>{});
        };
        if constexpr (Filter) return filter(expand);
        else return map(expand);
    }
};

template<class T> inline constexpr bool is_stock_windows_view = false;
template<class C> inline constexpr bool is_stock_windows_view<WindowsView<C>> = true;

template<std::size_t Arity, class R>
decltype(auto) validate_window_binding_range(R&& range) {
    if constexpr (is_stock_windows_view<std::remove_cvref_t<R>>)
        range.template validate_binding_count<Arity>();
    return std::forward<R>(range);
}

template<class Row>
concept StockWindowRow = requires { typename Row::kuma_window_source; }
    && std::same_as<Row, typename WindowsView<typename Row::kuma_window_source>::Row>;

template<std::size_t Arity, class K>
K checked_window_binding_width(K width) {
    if (width <= 0) error("windows: width must be positive");
    if (!std::cmp_equal(width, Arity)) error("windows: width must match binding count");
    return width;
}

template<std::size_t Arity, class Row>
void check_window_binding(const Row& row) {
    if constexpr (StockWindowRow<Row>) {
        if (row.size() != Arity) error("windows: width must match binding count");
    }
}

template<std::size_t Arity, class Row>
decltype(auto) window_binding_values(Row&& row) {
    if constexpr (StockWindowRow<std::remove_cvref_t<Row>>) {
        check_window_binding<Arity>(row);
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple<std::remove_cvref_t<decltype(row[I])>...>{row[I]...};
        }(std::make_index_sequence<Arity>{});
    } else {
        // Native rows keep normal C++ structured binding, including
        // aggregate or tuple-like types that do not use std::get.
        return std::forward<Row>(row);
    }
}

template<std::size_t I, class Row>
decltype(auto) window_binding_element(Row& row) {
    if constexpr (StockWindowRow<std::remove_cvref_t<Row>>) return row[I];
    else return std::get<I>(row);
}

template<class Item, class C>
auto enumerate(const C& values, long long start) {
    std::vector<std::tuple<long long, Item>> result;
    auto it = std::begin(values);
    const auto end = std::end(values);
    long long index = start;
    while (it != end) {
        result.emplace_back(index, *it);
        ++it;
        if (it != end) {
            if (index == std::numeric_limits<long long>::max()) error("enumerate index overflows i64");
            ++index;
        }
    }
    return result;
}

template<class Item, class... Cs>
auto zip(bool strict, const Cs&... values) {
    std::vector<Item> result;
    auto iterators = std::tuple{std::begin(values)...};
    auto ends = std::tuple{std::end(values)...};
    [&]<std::size_t... I>(std::index_sequence<I...>) {
        while (((std::get<I>(iterators) != std::get<I>(ends)) && ...)) {
            result.emplace_back((*std::get<I>(iterators))...);
            (++std::get<I>(iterators), ...);
        }
        if (strict && ((std::get<I>(iterators) != std::get<I>(ends)) || ...))
            error("zip strict: input lengths differ");
    }(std::index_sequence_for<Cs...>{});
    return result;
}

// These views own snapshots; unlike eager adapters they do not allocate output rows.
template<class Item, class C>
struct EnumerateView {
    using value_type = std::tuple<long long, Item>;
    C values;
    long long start;
    struct Sentinel {};
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        decltype(std::declval<const C&>().begin()) it;
        decltype(std::declval<const C&>().end()) end;
        long long index;
        bool index_overflow = false;
        auto operator*() const {
            // An enclosing zip may advance to a row it never reads.
            if (index_overflow) error("enumerate index overflows i64");
            return std::tuple<long long, decltype(*it)>{index, *it};
        }
        Iterator& operator++() {
            ++it;
            if (it != end) {
                if (index == std::numeric_limits<long long>::max()) index_overflow = true;
                else ++index;
            }
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator!=(Sentinel) const { return it != end; }
        bool operator==(Sentinel sentinel) const { return !operator!=(sentinel); }
        friend bool operator==(Sentinel sentinel, const Iterator& it) { return it.operator==(sentinel); }
        friend bool operator!=(Sentinel sentinel, const Iterator& it) { return it.operator!=(sentinel); }
    };
    Iterator begin() const { return {values.begin(), values.end(), start}; }
    Sentinel end() const { return {}; }
};

template<class Item, class... Cs>
struct ZipView {
    using value_type = Item;
    bool strict;
    std::tuple<Cs...> values;
    ZipView(bool check, Cs... inputs) : strict(check), values(std::move(inputs)...) {}
    struct Sentinel {};
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        bool strict;
        std::tuple<decltype(std::declval<const Cs&>().begin())...> iterators;
        std::tuple<decltype(std::declval<const Cs&>().end())...> ends;
        auto operator*() const {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                return std::tuple<decltype(*std::get<I>(iterators))...>{(*std::get<I>(iterators))...};
            }(std::index_sequence_for<Cs...>{});
        }
        Iterator& operator++() {
            std::apply([](auto&... it) { (++it, ...); }, iterators);
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator!=(Sentinel) const {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                const bool all = ((std::get<I>(iterators) != std::get<I>(ends)) && ...);
                if (!all && strict && ((std::get<I>(iterators) != std::get<I>(ends)) || ...))
                    error("zip strict: input lengths differ");
                return all;
            }(std::index_sequence_for<Cs...>{});
        }
        bool operator==(Sentinel sentinel) const { return !operator!=(sentinel); }
        friend bool operator==(Sentinel sentinel, const Iterator& it) { return it.operator==(sentinel); }
        friend bool operator!=(Sentinel sentinel, const Iterator& it) { return it.operator!=(sentinel); }
    };
    Iterator begin() const {
        return std::apply([&](const auto&... inputs) {
            return Iterator{strict, std::tuple{inputs.begin()...}, std::tuple{inputs.end()...}};
        }, values);
    }
    Sentinel end() const { return {}; }
};

template<class Item, class... Cs>
struct ProductView {
    using value_type = Item;
    std::tuple<Cs...> inputs;

    explicit ProductView(Cs... values) : inputs(std::move(values)...) {}

    struct Sentinel {};
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        using Positions = std::tuple<decltype(std::begin(std::declval<const Cs&>()))...>;
        using Ends = std::tuple<decltype(std::end(std::declval<const Cs&>()))...>;
        Positions begins, current;
        Ends ends;
        bool done;

        Item operator*() const {
            return std::apply([](const auto&... position) { return Item{*position...}; }, current);
        }

        template<std::size_t N>
        void advance() {
            auto& position = std::get<N - 1>(current);
            if (++position != std::get<N - 1>(ends)) return;
            if constexpr (N > 1) {
                position = std::get<N - 1>(begins);
                advance<N - 1>();
            } else {
                done = true;
            }
        }

        Iterator& operator++() {
            if constexpr (sizeof...(Cs) == 0) done = true;
            else advance<sizeof...(Cs)>();
            return *this;
        }
        void operator++(int) { ++*this; }
        bool operator==(Sentinel) const { return done; }
    };

    Iterator begin() const {
        return std::apply([](const auto&... values) {
            auto begins = std::tuple{std::begin(values)...};
            auto ends = std::tuple{std::end(values)...};
            const bool empty = [&]<std::size_t... I>(std::index_sequence<I...>) {
                return ((std::get<I>(begins) == std::get<I>(ends)) || ...);
            }(std::index_sequence_for<Cs...>{});
            return Iterator{begins, begins, std::move(ends), empty};
        }, inputs);
    }
    Sentinel end() const { return {}; }
};

template<class R, class... Args>
class RecursiveSelf {
    const void* object_;
    R (*invoke_)(const void*, Args...);

public:
    template<class Self>
    static RecursiveSelf bind(Self& self) {
        return RecursiveSelf{std::addressof(self), [](const void* address, Args... args) -> R {
            // Recover the exact cv-qualification of the callable passed to bind.
            return (*static_cast<Self*>(const_cast<void*>(address)))(std::move(args)...);
        }};
    }

    R operator()(Args... args) const { return invoke_(object_, std::move(args)...); }

private:
    RecursiveSelf(const void* object, R (*invoke)(const void*, Args...))
        : object_(object), invoke_(invoke) {}
};

template<class R, class... Args>
struct RecursiveBody {
    std::function<R(RecursiveSelf<R, Args...>, Args...)> body;

    template<class Self>
    R operator()(Self& self, Args... args) const {
        return body(RecursiveSelf<R, Args...>::bind(self), std::move(args)...);
    }
};

template<class R, class... Args>
using Recursive = ::ac_detail::recursive_callable<RecursiveBody<R, Args...>>;

template<class R, class... Args>
using Memoized = ::ac_detail::memoized<RecursiveBody<R, Args...>, R(Args...)>;

// Exact metadata checks for eager adapters before an allocation-free reduction.
// Wide arithmetic covers the entire signed-i64 range, including LL_MIN starts.
template<class C> __int128 sequence_length(const C& values) { return values.size(); }
inline __int128 sequence_length(const Range& values) {
    const __int128 first = values.first, last = values.last, step = values.step;
    if (step == 0) error("range step must not be zero");
    const __int128 distance = step > 0 ? last - first : first - last;
    if (distance <= 0) return 0;
    const __int128 stride = step > 0 ? step : -step;
    return (distance - 1) / stride + 1;
}

template<class Destination, class Source, class Project = std::identity>
Destination sequence_range_copy(const Destination& destination, const Source& source,
                                std::size_t limit, const char* bounds_error, Project project = {}) {
    Destination copy(destination.get_allocator());
    if constexpr (std::same_as<Source, Range> || requires { source.size(); }) {
        const auto count = sequence_length(source);
        if (count > limit) throw std::length_error(bounds_error);
        if constexpr (requires { copy.reserve(std::size_t{}); })
            copy.reserve(static_cast<std::size_t>(count));
    }
    for (const auto& item : source) {
        if (copy.size() == limit) throw std::length_error(bounds_error);
        decltype(auto) value = project(item);
#ifdef AC_HAS_ACL
        validate_current_modints(value);
#endif
        if constexpr (requires { copy.emplace_back(std::forward<decltype(value)>(value)); })
            copy.emplace_back(std::forward<decltype(value)>(value));
        else
            copy.push_back(std::forward<decltype(value)>(value));
    }
    return copy;
}

template<class T>
void validate_ordered_range_value(const T& value) {
    if constexpr (RecordOptional<T>::value) {
        if (value) validate_ordered_range_value(*value);
    } else if constexpr (is_tuple<T>::value) {
        std::apply([](const auto&... fields) { (validate_ordered_range_value(fields), ...); }, value);
    } else if constexpr (GeneratedRecord<T>) {
        validate_ordered_range_value(value.kuma_gen_value_fields());
    } else if constexpr (is_index_edit_sequence<T> || is_fixed_array<T>::value || is_associative<T>::value) {
        if constexpr (!std::same_as<typename T::value_type, char>)
            for (const auto& item : value) validate_ordered_range_value(item);
    } else {
        ::ac_detail::validate_ordered_finite(value);
    }
}

template<class Adaptor, class Source>
void adaptor_push_range(Adaptor& target, const Source& source) {
    using Item = typename Adaptor::value_type;
    constexpr bool heap = requires { typename Adaptor::value_compare; };
    const std::vector<Item> empty;
    auto limit = empty.max_size();
    if constexpr (requires { typename Adaptor::container_type; }) {
        if constexpr (heap && std::same_as<typename Adaptor::container_type, std::vector<Item>>)
            limit -= target.size();
    }
    const auto check = [](const auto& value) -> decltype(auto) {
        if constexpr (heap && !std::same_as<std::remove_cvref_t<decltype(value)>, Item>) {
            Item converted(value);
            validate_ordered_range_value(converted);
            return converted;
        } else {
            if constexpr (heap) validate_ordered_range_value(value);
            return value;
        }
    };
    auto entries = sequence_range_copy(empty, source, limit, "push_range: result exceeds max_size", check);
    if (entries.empty()) return;
    const auto input = std::ranges::subrange(std::make_move_iterator(entries.begin()),
                                             std::make_move_iterator(entries.end()));
    if constexpr (requires { target.push_range(input); }) {
        target.push_range(input);
    } else {
        for (auto&& value : entries) target.push(std::move(value));
    }
}

template<class Target, class Source, class CheckKey>
void associative_insert_range(Target& target, const Source& source, CheckKey check_key) {
    using Key = typename Target::key_type;
    const auto own = [&](const auto& item) {
        if constexpr (requires { typename Target::mapped_type; }) {
            using Row = std::remove_cvref_t<decltype(item)>;
            static_assert(requires {
                requires is_tuple<Row>::value;
                requires std::tuple_size<Row>::value == 2;
            }, "insert_range: dictionary rows must be pairs or two-element tuples");
            Key key(std::get<0>(item));
            check_key(key);
            return std::pair<Key, typename Target::mapped_type>(std::move(key), std::get<1>(item));
        } else {
            Key key(item);
            check_key(key);
            return key;
        }
    };
    const std::vector<decltype(own(*std::begin(source)))> empty;
    auto entries = sequence_range_copy(empty, source, empty.max_size(),
                                       "insert_range: source exceeds max_size", own);
    for (auto&& entry : entries) {
        if constexpr (requires { target.try_emplace(std::move(entry.first), std::move(entry.second)); })
            target.try_emplace(std::move(entry.first), std::move(entry.second));
        else
            target.emplace(std::move(entry));
    }
}

template<class Target, class Source>
void associative_insert_range(Target& target, const Source& source) {
    associative_insert_range(target, source, [](const auto& key) { validate_ordered_range_value(key); });
}

template<class Destination, class Source>
void sequence_assign_range(Destination& destination, const Source& source) {
    auto replacement = sequence_range_copy(destination, source, destination.max_size(),
                                           "assign_range: result exceeds max_size");
    destination.swap(replacement);
}

template<class Destination, class Index, class Source>
void sequence_insert_range(Destination& destination, Index index, const Source& source,
                           const char* bounds_error) {
    const auto size = destination.size();
    const auto offset = checked_vector_index(index, size, true, "insert_range: index out of range");
    auto copy = sequence_range_copy(destination, source, destination.max_size() - size, bounds_error);
    if (copy.empty()) return;
    if constexpr (requires { destination.insert(offset, copy); }) {
        destination.insert(offset, copy);
    } else {
        const auto where = offset == size ? destination.end() : sequence_index_iterator(destination, offset, true);
        if constexpr (requires { destination.splice(where, copy); }) {
            destination.splice(where, copy);
        } else {
            destination.insert(where, std::make_move_iterator(copy.begin()), std::make_move_iterator(copy.end()));
        }
    }
}

template<class Traits, class Allocator, class Left, class Right, class Source>
void sequence_replace_with_range(std::basic_string<char, Traits, Allocator>& text,
                                 Left left, Right right, const Source& source) {
    const auto size = text.size();
    constexpr auto bounds_error = "replace_with_range: index out of range";
    const auto first = checked_vector_index(left, size, true, bounds_error);
    const auto last = checked_vector_index(right, size, true, bounds_error);
    if (last < first) throw std::out_of_range("replace_with_range: endpoints are reversed");
    const auto removed = last - first;
    const auto limit = text.max_size() - (size - removed);
    const auto replacement = sequence_range_copy(text, source, limit, "replace_with_range: result exceeds max_size");
    text.replace(first, removed, replacement);
}

template<class C>
decltype(auto) sequence_algorithm_range(const C& input) {
    if constexpr (std::same_as<C, Range>) return forward_values<long long>(input);
    else return (input);
}

template<class C, class View, class Compare, class Projection>
bool sequence_is_sorted(const C& input, const View& view, Compare& compare, Projection& key) {
    if constexpr (std::same_as<C, Range> && std::same_as<Compare, std::less<>>
                  && std::same_as<Projection, std::identity>) {
        return input.step > 0 || sequence_length(input) <= 1;
    } else {
        return std::ranges::is_sorted(view, std::ref(compare), std::ref(key));
    }
}

template<class R, class S, class Compare = std::less<>,
         class Proj1 = std::identity, class Proj2 = std::identity>
bool sequence_includes(const R& values, const S& subset, Compare compare = {},
                       Proj1 first = {}, Proj2 second = {}) {
    auto&& left = sequence_algorithm_range(values);
    auto&& right = sequence_algorithm_range(subset);
    if (!sequence_is_sorted(values, left, compare, first) || !sequence_is_sorted(subset, right, compare, second))
        throw std::invalid_argument("includes: both inputs must be sorted");
    return ::includes(left, right, std::ref(compare), std::ref(first), std::ref(second));
}

template<class T, class R, class S, class Compare = std::less<>,
         class Proj1 = std::identity, class Proj2 = std::identity>
std::vector<T> sequence_merge(const R& values, const S& other, Compare compare = {},
                              Proj1 first = {}, Proj2 second = {}) {
    std::vector<T> result;
    __int128 count = 0;
    if constexpr ((std::same_as<R, Range> || requires { values.size(); })
                  && (std::same_as<S, Range> || requires { other.size(); })) {
        count = sequence_length(values) + sequence_length(other);
        if (count > result.max_size()) throw std::length_error("merge: result exceeds max_size");
    }
    auto&& left = sequence_algorithm_range(values);
    auto&& right = sequence_algorithm_range(other);
    if (!sequence_is_sorted(values, left, compare, first) || !sequence_is_sorted(other, right, compare, second))
        throw std::invalid_argument("merge: both inputs must be sorted");
    if (count != 0) result.reserve(static_cast<std::size_t>(count));
    ::merge(left, right, std::back_inserter(result), std::ref(compare), std::ref(first), std::ref(second));
    return result;
}

enum class SequenceSetOperation { Union, Intersection, Difference, SymmetricDifference };

template<SequenceSetOperation Operation, class T, class R, class S, class Compare = std::less<>,
         class Proj1 = std::identity, class Proj2 = std::identity>
std::vector<T> sequence_set_operation(const R& values, const S& other, Compare compare = {},
                                      Proj1 first = {}, Proj2 second = {}) {
    auto&& left = sequence_algorithm_range(values);
    auto&& right = sequence_algorithm_range(other);
    if (!sequence_is_sorted(values, left, compare, first) || !sequence_is_sorted(other, right, compare, second))
        throw std::invalid_argument("set operation: both inputs must be sorted");
    std::vector<T> result;
    if constexpr (Operation == SequenceSetOperation::Union) {
        ::set_union(left, right, std::back_inserter(result),
                    std::ref(compare), std::ref(first), std::ref(second));
    } else if constexpr (Operation == SequenceSetOperation::SymmetricDifference) {
        ::set_symmetric_difference(left, right, std::back_inserter(result),
                                   std::ref(compare), std::ref(first), std::ref(second));
    } else {
        // Only left elements are emitted, even with differently typed inputs.
        // There is no iterator result that would need advancing to either end.
        auto a = std::begin(left);
        auto b = std::begin(right);
        const auto left_end = std::end(left);
        const auto right_end = std::end(right);
        while (a != left_end && b != right_end) {
            auto&& value = *a;
            auto&& candidate = *b;
            decltype(auto) x = std::invoke(first, value);
            decltype(auto) y = std::invoke(second, candidate);
            if (std::invoke(compare, x, y)) {
                if constexpr (Operation == SequenceSetOperation::Difference) result.emplace_back(value);
                ++a;
            } else if (std::invoke(compare, y, x)) {
                ++b;
            } else {
                if constexpr (Operation == SequenceSetOperation::Intersection) result.emplace_back(value);
                ++a;
                ++b;
            }
        }
        if constexpr (Operation == SequenceSetOperation::Difference)
            for (; a != left_end; ++a) result.emplace_back(*a);
    }
    return result;
}

template<class R, class Validate, class Compare, class Projection = std::identity>
void sequence_inplace_merge(R& values, long long index, Validate validate,
                            Compare compare, Projection key = {}) {
    const auto offset = checked_vector_index(index, values.size(), true, "inplace_merge: middle out of range");
    const auto first = std::begin(values);
    const auto last = std::end(values);
    const auto middle = std::next(first, static_cast<std::iter_difference_t<decltype(std::begin(values))>>(offset));
    if constexpr (!std::same_as<Validate, std::nullptr_t>)
        for (const auto& value : values) validate(value);
    if (!std::ranges::is_sorted(first, middle, std::ref(compare), std::ref(key))
            || !std::ranges::is_sorted(middle, last, std::ref(compare), std::ref(key)))
        throw std::invalid_argument("inplace_merge: both parts must be sorted");
    ::inplace_merge(values, middle, std::ref(compare), std::ref(key));
}

template<class R, class Validate, class Compare, class Projection = std::identity>
void sequence_sort_heap(R& values, Validate validate, Compare compare, Projection project = {}) {
    if constexpr (!std::same_as<Validate, std::nullptr_t>)
        for (const auto& value : values) validate(value);
    if (!::is_heap(values, std::ref(compare), std::ref(project)))
        throw std::invalid_argument("sort_heap: input must be a heap");
    ::sort_heap(values, std::ref(compare), std::ref(project));
}

template<bool Push, class R, class Validate, class Compare, class Projection = std::identity>
void sequence_update_heap(R& values, Validate validate, Compare compare, Projection project = {}) {
    const auto size = values.size();
    if (size == 0)
        throw std::invalid_argument(Push ? "push_heap: empty sequence" : "pop_heap: empty sequence");

    const auto first = std::begin(values);
    const auto position = [&](std::size_t index) {
        return first + static_cast<std::iter_difference_t<decltype(first)>>(index);
    };
    const auto below = [&](std::size_t left, std::size_t right) {
        if constexpr (!std::same_as<Validate, std::nullptr_t>) {
            validate(*position(left));
            validate(*position(right));
        }
        return std::invoke(compare, std::invoke(project, *position(left)),
                           std::invoke(project, *position(right)));
    };

    const auto last = size - 1;
    if constexpr (!std::same_as<Validate, std::nullptr_t>) validate(*position(last));
    if constexpr (Push) {
        auto target = last;
        while (target > 0 && below((target - 1) / 2, last)) target = (target - 1) / 2;
        for (auto child = last; child != target;) {
            const auto parent = (child - 1) / 2;
            std::ranges::iter_swap(position(parent), position(child));
            child = parent;
        }
    } else {
        if (last == 0) return;
        if constexpr (!std::same_as<Validate, std::nullptr_t>) validate(*first);
        std::size_t target = 0;
        while (target < last / 2) {
            auto child = 2 * target + 1;
            if (child + 1 < last && below(child, child + 1)) ++child;
            if (!below(last, child)) break;
            target = child;
        }
        // Rotate the path from its bottom, carrying the displaced value in the last slot.
        const auto tail = position(last);
        while (true) {
            std::ranges::iter_swap(tail, position(target));
            if (target == 0) break;
            target = (target - 1) / 2;
        }
    }
}

template<bool Nth, class R, class Validate, class Compare, class Projection = std::identity>
void sequence_select(R& values, long long position, Validate validate,
                     Compare compare, Projection project = {}) {
    const auto offset = checked_vector_index(position, values.size(), true,
        Nth ? "nth_element: index out of range" : "partial_sort: count out of range");
    if (Nth ? offset == values.size() : offset == 0) return;
    if constexpr (!std::same_as<Validate, std::nullptr_t>)
        for (const auto& value : values) validate(value);
    const auto first = std::begin(values);
    const auto middle = first + static_cast<std::iter_difference_t<decltype(first)>>(offset);
    if constexpr (Nth) ::nth_element(values, middle, std::ref(compare), std::ref(project));
    else ::partial_sort(values, middle, std::ref(compare), std::ref(project));
}

template<class T, class R, class Validate, class Compare, class Projection = std::identity>
std::vector<T> sequence_partial_sort_copy(const R& values, long long count, Validate validate,
                                          Compare compare, Projection project = {}) {
    if (count < 0) throw std::invalid_argument("partial_sort_copy: count must be nonnegative");
    if (count == 0) return {};
    if constexpr (requires { values.snapshot(); }) {
        return sequence_partial_sort_copy<T>(values.snapshot(), count, validate, compare, project);
    } else {
        std::vector<T> result;
        for (const auto& value : values) {
            if constexpr (!std::same_as<Validate, std::nullptr_t>) validate(value);
            if (std::cmp_less(result.size(), count)) {
                result.emplace_back(value);
                std::ranges::push_heap(result, std::ref(compare), std::ref(project));
            } else if (std::invoke(compare, std::invoke(project, value), std::invoke(project, result.front()))) {
                std::ranges::pop_heap(result, std::ref(compare), std::ref(project));
                result.back() = value;
                std::ranges::push_heap(result, std::ref(compare), std::ref(project));
            }
        }
        std::ranges::sort_heap(result, std::ref(compare), std::ref(project));
        return result;
    }
}

template<bool Stable, class R, class Pred, class Proj = std::identity>
long long sequence_partition(R& values, Pred pred, Proj project = {}) {
    const auto tail = [&] {
        if constexpr (Stable) return ::stable_partition(values, std::move(pred), std::move(project));
        else return ::partition(values, std::move(pred), std::move(project));
    }();
    return static_cast<long long>(std::ranges::distance(std::begin(values), tail.begin()));
}

template<class T, class R, class Pred, class Proj = std::identity>
std::pair<std::vector<T>, std::vector<T>> sequence_partition_copy(const R& values, Pred pred, Proj project = {}) {
    std::pair<std::vector<T>, std::vector<T>> parts;
    ::partition_copy(values, std::back_inserter(parts.first), std::back_inserter(parts.second),
                     std::move(pred), std::move(project));
    return parts;
}

template<class R>
long long sequence_rotate(R& values, long long index) {
    const auto size = values.size();
    const auto offset = checked_vector_index(index, size, true, "rotate: middle out of range");
    const auto first = std::begin(values);
    const auto middle = std::next(first, static_cast<std::iter_difference_t<decltype(first)>>(offset));
    ::rotate(values, middle);
    return static_cast<long long>(size - offset);
}

template<class T, class R>
std::vector<T> sequence_rotate_copy(const R& values, long long index) {
    if (index < 0) throw std::out_of_range("rotate_copy: middle out of range");
    std::vector<T> result;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (index > size) throw std::out_of_range("rotate_copy: middle out of range");
        if (size > result.max_size()) throw std::length_error("rotate_copy: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(size));
    }
    auto&& input = sequence_algorithm_range(values);
    auto middle = std::begin(input);
    using Difference = std::iter_difference_t<decltype(middle)>;
    if (!std::in_range<Difference>(index)
            || std::ranges::advance(middle, static_cast<Difference>(index), std::end(input)) != 0)
        throw std::out_of_range("rotate_copy: middle out of range");
    ::rotate_copy(input, middle, std::back_inserter(result));
    return result;
}

template<class T, class R>
std::vector<T> sequence_reverse_copy(const R& values) {
    std::vector<T> result;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (size > result.max_size()) throw std::length_error("reverse_copy: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(size));
    }
    if constexpr (std::ranges::bidirectional_range<const R>) {
        ::reverse_copy(values, std::back_inserter(result));
    } else {
        for (const auto& value : values) result.emplace_back(value);
        std::reverse(result.begin(), result.end());
    }
    return result;
}

template<class R, class T>
void sequence_fill(R& values, const T& value) {
    if constexpr (requires { values.column_count(); }) {
        if (value.size() != static_cast<std::size_t>(values.column_count()))
            throw std::invalid_argument("fill: row width must match matrix columns");
    }
    ::fill(values, value);
}

template<class R, class Generator>
void sequence_generate(R& values, Generator generate_value) {
    if constexpr (requires { values.column_count(); }) {
        ::generate(values, [&] {
            auto row = std::invoke(generate_value);
            if (row.size() != static_cast<std::size_t>(values.column_count()))
                throw std::invalid_argument("generate: row width must match matrix columns");
            return row;
        });
    } else {
        ::generate(values, std::move(generate_value));
    }
}

template<class R, class Start>
void sequence_iota(R& values, Start start) {
    using T = typename R::value_type;
    if constexpr (std::integral<T>) {
        const auto size = values.size();
        if (size == 0) return;
        __int128 next = start;
        const auto last = next + static_cast<__int128>(size) - 1;
        if (next < std::numeric_limits<T>::min() || last > std::numeric_limits<T>::max())
            throw std::overflow_error("iota: values do not fit the element type");
        for (auto&& value : values) value = static_cast<T>(next++);
#ifdef AC_HAS_ACL
    } else {
        validate_current_modints(start);
        const T one(1);
        for (auto& value : values) {
            value = start;
            start += one;
        }
#endif
    }
}

template<class T, class R, class Function, class Projection = std::identity>
std::vector<T> sequence_transform(const R& values, Function operation, Projection project = {}) {
    std::vector<T> result;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (size > result.max_size()) throw std::length_error("transform: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(size));
    }
    auto&& input = sequence_algorithm_range(values);
    ::transform(input, std::back_inserter(result), std::move(operation), std::move(project));
    return result;
}

template<class T, class R>
std::vector<T> sequence_copy(const R& values) {
    std::vector<T> result;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (size > result.max_size()) throw std::length_error("copy: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(size));
    }
    auto&& input = sequence_algorithm_range(values);
    ::copy(input, std::back_inserter(result));
    return result;
}

template<class T, class R>
std::vector<T> sequence_sample(const R& values, long long requested, std::mt19937_64& engine) {
    if (requested < 0) throw std::invalid_argument("sample: count must be nonnegative");
    std::vector<T> result;
    if (requested == 0) return result;
    if constexpr (std::same_as<R, Range>) {
        // Sample positions without walking a potentially enormous integer range.
        const auto size = static_cast<std::uint64_t>(sequence_length(values));
        const auto count = std::min(size, static_cast<std::uint64_t>(requested));
        if (count == size) return sequence_copy<T>(values);
        if (count > result.max_size()) throw std::length_error("sample: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(count));
        std::set<std::uint64_t> positions;
        for (auto j = size - count; j < size; ++j) {
            const auto i = std::uniform_int_distribution<std::uint64_t>(0, j)(engine);
            if (!positions.insert(i).second) positions.insert(j);
        }
        for (const auto i : positions)
            result.emplace_back(static_cast<long long>(static_cast<__int128>(values.first)
                                                       + static_cast<__int128>(values.step) * i));
    } else {
        if constexpr (requires { values.size(); }) {
            const auto size = sequence_length(values);
            if (size <= requested) return sequence_copy<T>(values);
            if (std::cmp_greater(requested, result.max_size()))
                throw std::length_error("sample: result exceeds max_size");
            result.reserve(static_cast<std::size_t>(requested));
        }
        ::sample(values, std::back_inserter(result), requested, engine);
    }
    return result;
}

template<bool Right, class T, class R, class Function>
auto sequence_fold_order(const R& values, Function consume) {
    if constexpr (!Right) {
        return consume(std::begin(values), std::end(values));
    } else if constexpr (std::same_as<R, Range>) {
        const auto count = static_cast<std::uint64_t>(sequence_length(values));
        auto reversed = std::views::iota(std::uint64_t{0}, count) | std::views::reverse
            | std::views::transform([&](std::uint64_t i) {
                return static_cast<long long>(static_cast<__int128>(values.first)
                                             + static_cast<__int128>(values.step) * i);
            });
        return consume(reversed.begin(), reversed.end());
    } else if constexpr (std::ranges::bidirectional_range<const R> && std::ranges::common_range<const R>) {
        return consume(std::make_reverse_iterator(std::end(values)),
                       std::make_reverse_iterator(std::begin(values)));
    } else {
        const auto copy = sequence_copy<T>(values);
        return consume(copy.rbegin(), copy.rend());
    }
}

template<bool Right, bool Seeded, bool WithIndex, class T, class U,
         class R, class Seed, class First, class Step = std::nullptr_t>
auto sequence_fold(const R& values, std::optional<Seed> seed, First first, Step step = nullptr) {
    if constexpr (WithIndex && (std::same_as<R, Range> || requires { values.size(); })) {
        if (sequence_length(values) > std::numeric_limits<long long>::max())
            throw std::overflow_error("fold: position does not fit i64");
    }
    return sequence_fold_order<Right, T>(values, [&](auto it, auto end) {
        long long position = 0;
        const auto advance = [&] {
            if constexpr (WithIndex) {
                if (position == std::numeric_limits<long long>::max())
                    throw std::overflow_error("fold: position does not fit i64");
                ++position;
            }
            ++it;
        };
        const auto apply = [&](auto& operation, auto&& acc) -> U {
            if constexpr (Right) return std::invoke(operation, *it, std::move(acc));
            else return std::invoke(operation, std::move(acc), *it);
        };
        if constexpr (!Seeded) {
            if (it != end) {
                seed.emplace(*it);
                advance();
            }
        }
        std::optional<U> result;
        if (seed) {
            if (it == end) {
                result.emplace(std::move(*seed));
            } else {
                result.emplace(apply(first, *seed));
                advance();
                for (; it != end; advance()) {
                    if constexpr (std::same_as<Step, std::nullptr_t>) *result = apply(first, *result);
                    else *result = apply(step, *result);
                }
            }
        }
        if constexpr (WithIndex && Seeded) return std::pair{position, std::move(*result)};
        else if constexpr (WithIndex) return std::pair{position, std::move(result)};
        else if constexpr (Seeded) return std::move(*result);
        else return result;
    });
}

template<class T, class R, class Pred, class Proj = std::identity>
std::vector<T> sequence_copy_if(const R& values, Pred predicate, Proj project = {}) {
    std::vector<T> result;
    auto&& input = sequence_algorithm_range(values);
    ::copy_if(input, std::back_inserter(result), std::move(predicate), std::move(project));
    return result;
}

template<bool Backward, class R, class S>
long long sequence_copy_to(const R& values, S& destination, long long index) {
    const auto offset = checked_vector_index(index, destination.size(), true, "copy: destination out of range");
    const auto capacity = Backward ? offset : destination.size() - offset;
    auto&& input = sequence_algorithm_range(values);
    std::size_t count = 0;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (size > capacity) throw std::out_of_range("copy: destination interval is too small");
        count = static_cast<std::size_t>(size);
    } else {
        const auto end = std::end(input);
        for (auto it = std::begin(input); it != end; ++it) {
            if (count == capacity) throw std::out_of_range("copy: destination interval is too small");
            ++count;
        }
    }
    const auto boundary = static_cast<long long>(Backward ? offset - count : offset + count);
    if (count == 0) return boundary;
    if constexpr (std::same_as<R, S>) {
        if (&values == &destination) return boundary;
    }
    if constexpr (requires { destination.column_count(); }) {
        for (const auto& row : input)
            if (row.size() != static_cast<std::size_t>(destination.column_count()))
                throw std::invalid_argument("copy: row width must match destination columns");
    }
    using Difference = std::iter_difference_t<decltype(std::begin(destination))>;
    if constexpr (Backward && std::ranges::bidirectional_range<decltype(input)>) {
        ::copy_backward(input, std::next(std::begin(destination), static_cast<Difference>(offset)));
    } else {
        const auto start = Backward ? offset - count : offset;
        ::copy(input, std::next(std::begin(destination), static_cast<Difference>(start)));
    }
    return boundary;
}

template<bool Backward, class R>
long long sequence_copy_within(R& values, long long left, long long right, long long index) {
    const auto size = values.size();
    const auto first = checked_vector_index(left, size, true, "copy: source out of range");
    const auto last = checked_vector_index(right, size, true, "copy: source out of range");
    const auto offset = checked_vector_index(index, size, true, "copy: destination out of range");
    if (first > last) throw std::out_of_range("copy: source endpoints are reversed");
    const auto count = last - first;
    if (count > (Backward ? offset : size - offset))
        throw std::out_of_range("copy: destination interval is too small");
    const auto start = Backward ? offset - count : offset;
    const auto boundary = static_cast<long long>(Backward ? start : offset + count);
    if (count == 0 || start == first) return boundary;
    if (Backward ? start < first && offset > first : start > first && start < last)
        throw std::invalid_argument("copy: overlap requires the opposite copy direction");
    const auto begin = std::begin(values);
    using Difference = std::iter_difference_t<decltype(begin)>;
    const auto range = std::ranges::subrange(std::next(begin, static_cast<Difference>(first)),
                                            std::next(begin, static_cast<Difference>(last)));
    const auto out = std::next(begin, static_cast<Difference>(offset));
    if constexpr (Backward) ::copy_backward(range, out);
    else ::copy(range, out);
    return boundary;
}

template<class Container>
void swap_container(Container& left, Container& right) {
    static_assert(noexcept(left.swap(right)), "swap: container contents must be swappable without throwing");
    if (std::addressof(left) != std::addressof(right)) left.swap(right);
}

template<class R, class S>
long long sequence_swap_ranges(R& left, S& right) {
    const auto count = std::min(left.size(), right.size());
    if (count == 0) return 0;
    if constexpr (std::same_as<R, S>) {
        if (&left == &right) return static_cast<long long>(count);
    }
    const auto check_width = [count]<class Destination, class Source>(
                                 const Destination& destination, const Source& source) {
        if constexpr (requires { destination.column_count(); }) {
            const auto width = static_cast<std::size_t>(destination.column_count());
            if constexpr (requires { source.column_count(); }) {
                if (width != static_cast<std::size_t>(source.column_count()))
                    throw std::invalid_argument("swap_ranges: row widths must match matrix columns");
            } else {
                auto row = std::begin(source);
                for (std::size_t i = 0; i < count; ++i, ++row)
                    if ((*row).size() != width)
                        throw std::invalid_argument("swap_ranges: row widths must match matrix columns");
            }
        }
    };
    check_width(left, right);
    check_width(right, left);
    ::swap_ranges(left, right);
    return static_cast<long long>(count);
}

template<class T, class R, class S, class Function, class Proj1 = std::identity, class Proj2 = std::identity>
std::vector<T> sequence_transform_binary(const R& left, const S& right, Function operation,
                                         Proj1 first = {}, Proj2 second = {}) {
    constexpr bool left_sized = std::same_as<R, Range> || requires { left.size(); };
    constexpr bool right_sized = std::same_as<S, Range> || requires { right.size(); };
    std::vector<T> result;
    std::optional<__int128> remaining;
    if constexpr (left_sized) remaining = sequence_length(left);
    if constexpr (right_sized) {
        const auto size = sequence_length(right);
        remaining = remaining ? std::min(*remaining, size) : size;
    }
    if (remaining == 0) return result;
    if constexpr (left_sized && right_sized) {
        if (*remaining > result.max_size()) throw std::length_error("transform: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(*remaining));
    }
    auto a = std::begin(left);
    auto b = std::begin(right);
    const auto left_end = std::end(left);
    const auto right_end = std::end(right);
    while (a != left_end && b != right_end) {
        auto&& value1 = *a;
        auto&& value2 = *b;
        decltype(auto) x = std::invoke(first, value1);
        decltype(auto) y = std::invoke(second, value2);
        result.emplace_back(std::invoke(operation, std::forward<decltype(x)>(x), std::forward<decltype(y)>(y)));
        // At a known shorter boundary, even advancing the other view can check its tail.
        if (remaining && --*remaining == 0) break;
        ++a;
        ++b;
    }
    return result;
}

template<class T, class R, class Pred, class Proj = std::identity>
std::vector<T> sequence_replace_copy(const R& values, Pred pred, const T& replacement, Proj project = {}) {
    std::vector<T> result;
    if constexpr (std::same_as<R, Range> || requires { values.size(); }) {
        const auto size = sequence_length(values);
        if (size > result.max_size()) throw std::length_error("replace_copy: result exceeds max_size");
        result.reserve(static_cast<std::size_t>(size));
    }
    auto&& input = sequence_algorithm_range(values);
    ::replace_copy_if(input, std::back_inserter(result), std::move(pred), replacement, std::move(project));
    return result;
}

template<bool PreserveShape, class R, class Pred, class Proj = std::identity>
long long sequence_remove(R& values, Pred pred, Proj project = {}) {
    if constexpr (PreserveShape) {
        // Swapping whole rows keeps the shape valid, including the unused tail.
        auto out = std::begin(values);
        const auto end = std::end(values);
        long long kept = 0;
        for (auto it = out; it != end; ++it) {
            if (std::invoke(pred, std::invoke(project, *it))) continue;
            if (out != it) std::ranges::iter_swap(out, it);
            ++out;
            ++kept;
        }
        return kept;
    } else {
        const auto tail = ::remove_if(values, std::move(pred), std::move(project));
        return static_cast<long long>(std::ranges::distance(std::begin(values), tail.begin()));
    }
}

template<class T, class R, class Pred, class Proj = std::identity>
std::vector<T> sequence_remove_copy(const R& values, Pred pred, Proj project = {}) {
    std::vector<T> result;
    auto&& input = sequence_algorithm_range(values);
    ::remove_copy_if(input, std::back_inserter(result), std::move(pred), std::move(project));
    return result;
}

template<bool PreserveShape, class R, class Pred = std::ranges::equal_to, class Proj = std::identity>
long long sequence_unique(R& values, Pred pred = {}, Proj project = {}) {
    auto last_kept = std::begin(values);
    const auto end = std::end(values);
    if (last_kept == end) return 0;
    long long kept = 1;
    for (auto it = std::next(last_kept); it != end; ++it) {
        if (std::invoke(pred, std::invoke(project, *last_kept), std::invoke(project, *it))) continue;
        ++last_kept;
        if (last_kept != it) {
            if constexpr (PreserveShape) std::ranges::iter_swap(last_kept, it);
            else *last_kept = std::ranges::iter_move(it);
        }
        ++kept;
    }
    return kept;
}

template<class T, class R, class Pred = std::ranges::equal_to, class Proj = std::identity>
std::vector<T> sequence_unique_copy(const R& values, Pred pred = {}, Proj project = {}) {
    std::vector<T> result;
    for (const auto& value : values)
        if (result.empty() || !std::invoke(pred, std::invoke(project, std::as_const(result).back()),
                                           std::invoke(project, value)))
            result.emplace_back(value);
    return result;
}

template<class R, class S, class Pred = std::ranges::equal_to,
         class Proj1 = std::identity, class Proj2 = std::identity>
bool sequence_contains_subrange(const R& values, const S& pattern, Pred pred = {}, Proj1 first = {}, Proj2 second = {}) {
    if (std::ranges::begin(pattern) == std::ranges::end(pattern)) return true;
    return !std::ranges::search(values, pattern, std::move(pred), std::move(first), std::move(second)).empty();
}

template<class R, class S, class Pred = std::ranges::equal_to,
         class Proj1 = std::identity, class Proj2 = std::identity>
bool sequence_starts_with(const R& values, const S& prefix, Pred pred = {}, Proj1 first = {}, Proj2 second = {}) {
    auto part = std::begin(prefix);
    const auto part_end = std::end(prefix);
    if (part == part_end) return true;
    auto current = std::begin(values);
    const auto end = std::end(values);
    for (;;) {
        if (current == end || !std::invoke(pred, std::invoke(first, *current), std::invoke(second, *part)))
            return false;
        if (++part == part_end) return true;
        ++current;
    }
}

template<class R, class S, class Pred = std::ranges::equal_to,
         class Proj1 = std::identity, class Proj2 = std::identity>
bool sequence_ends_with(const R& values, const S& suffix, Pred pred = {}, Proj1 first = {}, Proj2 second = {}) {
    auto tail = std::begin(suffix);
    const auto tail_end = std::end(suffix);
    if (tail == tail_end) return true;

    const auto length = []<class C>(const C& input) -> __int128 {
        if constexpr (std::same_as<C, Range> || requires { input.size(); }) {
            return sequence_length(input);
        } else {
            __int128 count = 0;
            const auto end = std::end(input);
            for (auto it = std::begin(input); it != end; ++it) ++count;
            return count;
        }
    };
    auto current = std::begin(values);
    if (current == std::end(values)) return false;
    const auto size = length(values);
    const auto skip = size - length(suffix);
    if (skip < 0) return false;
    if constexpr (std::same_as<R, Range>) {
        current.value = static_cast<long long>(static_cast<__int128>(values.first) + values.step * skip);
    } else if constexpr (std::random_access_iterator<decltype(current)>) {
        current += static_cast<std::iter_difference_t<decltype(current)>>(skip);
    } else {
        for (auto i = skip; i > 0; --i) ++current;
    }
    for (; tail != tail_end; ++tail, ++current)
        if (!std::invoke(pred, std::invoke(first, *current), std::invoke(second, *tail))) return false;
    return true;
}

template<class T, class U, class L, class R>
auto zip_longest(const L& left, const R& right) {
    std::vector<std::pair<std::optional<T>, std::optional<U>>> result;
    const auto check_length = [&result]<class C>(const C& input) {
        if constexpr (std::is_same_v<C, Range> || requires { input.size(); }) {
            if (sequence_length(input) > static_cast<__int128>(result.max_size()))
                throw std::length_error("zip_longest: result is too large");
        }
    };
    check_length(left);
    check_length(right);

    auto i = std::begin(left);
    auto j = std::begin(right);
    const auto left_end = std::end(left);
    const auto right_end = std::end(right);
    while (i != left_end || j != right_end) {
        auto& row = result.emplace_back();
        if (i != left_end) {
            row.first.emplace(*i);
            ++i;
        }
        if (j != right_end) {
            row.second.emplace(*j);
            ++j;
        }
    }
    return result;
}
template<class C>
void check_eager_enumerate(const C& values, long long start) {
    if (sequence_length(values) > static_cast<__int128>(std::numeric_limits<long long>::max()) - start + 1)
        error("enumerate index overflows i64");
}
template<class C, class... Cs>
void check_eager_zip(bool strict, const C& first, const Cs&... rest) {
    if (strict && ((sequence_length(first) != sequence_length(rest)) || ...))
        error("zip strict: input lengths differ");
}

// Compiler-only immediate borrows. Input lifetimes cover the entire reduction;
// no borrowed view can be stored or returned from Kuma source.
template<class Item, class C>
auto borrow_enumerate(const C& values, long long start) {
    return EnumerateView<Item, const C&>{values, start};
}
template<class Item, class... Cs>
auto borrow_zip(bool strict, const Cs&... values) {
    return ZipView<Item, const Cs&...>{strict, values...};
}

template<class C>
void pop_back(C& values) {
    if (values.empty()) error("pop_back on an empty container");
    values.pop_back();
}

template<class K, class V>
V dict_get(const std::map<K, V>& values, const K& key, const V& fallback) {
    const auto it = values.find(key);
    return it == values.end() ? fallback : it->second;
}

template<class> inline constexpr bool is_std_map = false;
template<class K, class V, class Compare, class Allocator>
inline constexpr bool is_std_map<std::map<K, V, Compare, Allocator>> = true;

template<class T>
concept DictSpreadScalar = std::same_as<T, int> || std::same_as<T, long long> ||
    std::same_as<T, double> || std::same_as<T, bool> || std::same_as<T, char> ||
    std::same_as<T, std::string>;

template<class> inline constexpr bool is_spread_dict = false;
template<DictSpreadScalar K, DictSpreadScalar V>
inline constexpr bool is_spread_dict<std::map<K, V>> = true;

template<class To, class From>
To dict_spread_value(From&& value) {
    using Input = std::remove_cvref_t<From>;
    if constexpr (std::same_as<To, Input> || (std::same_as<To, long long> && std::same_as<Input, int>))
        return static_cast<To>(std::forward<From>(value));
    else
        static_assert(false, "dictionary entry type must match the dictionary (i32 to i64 is allowed)");
}

template<class Dest, class Source>
void dict_spread_into(Dest& dest, const Source& source) {
    if constexpr (is_spread_dict<Dest> && std::same_as<Dest, Source>) {
        for (const auto& [key, value] : source) {
            ::ac_detail::validate_ordered_finite(key);
            dest.insert_or_assign(key, value);
        }
    } else {
        static_assert(false, "dictionary spread requires matching standard dictionaries with basic key and value types");
    }
}

template<class Map>
struct DictGetReceiver {
    const Map& values;

    template<class Key, class Default>
    auto (get)(const Key& key, Default&& fallback) const -> typename Map::mapped_type {
        return ::get_or<Map, Key, Default>(values, key, std::forward<Default>(fallback));
    }
};

// Adapt only actual std::map specializations, not derived or map-like types.
// For native members, leave the arguments in the caller's full expression.
template<class Receiver>
constexpr decltype(auto) dict_get_receiver(Receiver&& receiver) noexcept {
    using Map = std::remove_cvref_t<Receiver>;
    if constexpr (is_std_map<Map> && !std::is_volatile_v<std::remove_reference_t<Receiver>>)
        return DictGetReceiver<Map>{receiver};
    else
        return std::forward<Receiver>(receiver);
}

// Slices always own their result. Wide arithmetic handles LL_MIN steps/bounds
// without negating LL_MIN or overflowing the final loop increment.
struct SliceBounds { __int128 start, stop, step, count; };
inline SliceBounds slice_bounds(std::size_t length, std::optional<long long> first,
                                std::optional<long long> last, std::optional<long long> stride) {
    using Wide = __int128;
    const Wide n = static_cast<Wide>(length);
    const Wide step = stride.value_or(1);
    if (step == 0) error("slice step must not be zero");
    auto normalize = [=](std::optional<long long> bound, bool is_start) -> Wide {
        if (!bound) return step > 0 ? (is_start ? 0 : n) : (is_start ? n - 1 : -1);
        Wide index = *bound;
        if (index < 0) index += n;
        const Wide low = step > 0 ? 0 : -1;
        const Wide high = step > 0 ? n : n - 1;
        return std::min(std::max(index, low), high);
    };
    const Wide start = normalize(first, true), stop = normalize(last, false);
    const Wide count = step > 0 ? (start < stop ? 1 + (stop - start - 1) / step : 0)
                               : (start > stop ? 1 + (start - stop - 1) / (-step) : 0);
    return {start, stop, step, count};
}

template<class C>
C slice(const C& values, std::optional<long long> first,
        std::optional<long long> last, std::optional<long long> stride) {
    using Wide = __int128;
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    C result;
    result.reserve(static_cast<std::size_t>(count));
    for (Wide index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

template<class T, std::size_t N>
std::vector<std::remove_cv_t<T>> slice(const T (&values)[N], std::optional<long long> first,
                                      std::optional<long long> last, std::optional<long long> stride) {
    const auto [start, stop, step, count] = slice_bounds(N, first, last, stride);
    std::vector<std::remove_cv_t<T>> result;
    result.reserve(static_cast<std::size_t>(count));
    for (__int128 index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

inline std::string slice(const std::string_view& values, std::optional<long long> first,
                         std::optional<long long> last, std::optional<long long> stride) {
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    std::string result;
    result.reserve(static_cast<std::size_t>(count));
    for (__int128 index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

template<class T, std::size_t N>
std::vector<std::remove_cv_t<T>> slice(const std::span<T, N>& values, std::optional<long long> first,
                                      std::optional<long long> last, std::optional<long long> stride) {
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    std::vector<std::remove_cv_t<T>> result;
    result.reserve(static_cast<std::size_t>(count));
    for (__int128 index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

template<class T, class A>
std::deque<T, A> slice(const std::deque<T, A>& values, std::optional<long long> first,
                       std::optional<long long> last, std::optional<long long> stride) {
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    std::deque<T, A> result(values.get_allocator());
    for (__int128 index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

template<class T, std::size_t N>
std::vector<T> slice(const std::array<T, N>& values, std::optional<long long> first,
                     std::optional<long long> last, std::optional<long long> stride) {
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    std::vector<T> result;
    result.reserve(static_cast<std::size_t>(count));
    for (__int128 index = start; step > 0 ? index < stop : index > stop; index += step)
        result.push_back(values[static_cast<std::size_t>(index)]);
    return result;
}

template<class T, class K>
std::vector<T> slice(const SortedList<T,K>& values, std::optional<long long> first,
                                    std::optional<long long> last, std::optional<long long> stride) {
    return slice(values.snapshot(), first, last, stride);
}

// Braced initialization sequences source and bounds left-to-right. The source
// reference is used only during this full expression, including temporary input.
template<class C>
struct CppSliceRequest {
    const C& values;
    std::optional<long long> first, last, stride;
};
template<class C>
CppSliceRequest(const C&, std::optional<long long>, std::optional<long long>,
                std::optional<long long>) -> CppSliceRequest<C>;

template<class T>
std::optional<long long> cpp_slice_bound(T value) {
    static_assert((std::is_integral_v<T> || std::is_same_v<T, __int128> ||
                   std::is_same_v<T, unsigned __int128>) && !std::is_same_v<T, bool>,
                  "Kuma slice bounds must be integers (not bool)");
    using Limits = std::numeric_limits<long long>;
    // Compare in the original type: even signed __int128 cannot represent
    // every unsigned __int128 bound.
    if constexpr (std::numeric_limits<T>::digits > Limits::digits) {
        if (value > static_cast<T>(Limits::max()))
            error("slice bound exceeds signed 64-bit range");
        if constexpr (std::numeric_limits<T>::is_signed) {
            if (value < static_cast<T>(Limits::min()))
                error("slice bound exceeds signed 64-bit range");
        }
    }
    return static_cast<long long>(value);
}
template<class C>
auto cpp_slice(CppSliceRequest<C> request) {
    return slice(request.values, request.first, request.last, request.stride);
}

// Build a replacement before swapping: overlap and failed copies do not leave
// the target partially modified. Step-one replacement may change the length.
template<class C>
void assign_slice(C& values, std::optional<long long> first, std::optional<long long> last,
                  std::optional<long long> stride, C replacement) {
    const auto [start, stop, step, count] = slice_bounds(values.size(), first, last, stride);
    if (step == 1) {
        const auto final_size = static_cast<__int128>(values.size()) - count + replacement.size();
        if (final_size > values.max_size()) error("slice assignment exceeds max_size");
        C result;
        result.reserve(static_cast<std::size_t>(final_size));
        const auto begin = static_cast<std::size_t>(start);
        const auto end = static_cast<std::size_t>(start + count);
        result.insert(result.end(), values.begin(), values.begin() + begin);
        result.insert(result.end(), replacement.begin(), replacement.end());
        result.insert(result.end(), values.begin() + end, values.end());
        values.swap(result);
    } else {
        if (count != static_cast<__int128>(replacement.size())) error("extended slice assignment length mismatch");
        C result = values;
        __int128 position = start;
        for (std::size_t i = 0; i < replacement.size(); ++i, position += step)
            result[static_cast<std::size_t>(position)] = replacement[i];
        values.swap(result);
    }
}

template<class Item, class C, class F, class Acc>
Acc fold(const C& values, F callback, Acc initial) {
    for (const auto& value : values)
        initial = callback(std::move(initial), Item(value));
    return initial;
}

template<class Item, class C, class F>
bool any_matching(const C& values, F predicate) {
    for (const auto& value : values) if (predicate(Item(value))) return true;
    return false;
}

template<class Item, class C, class F>
bool all_matching(const C& values, F predicate) {
    for (const auto& value : values) if (!predicate(Item(value))) return false;
    return true;
}

template<class Item, class C, class F>
long long count_matching(const C& values, F predicate) {
    long long result = 0;
    for (const auto& value : values) if (predicate(Item(value))) {
        if (result == std::numeric_limits<long long>::max()) error("count overflows i64");
        ++result;
    }
    return result;
}

template<class T, class F>
void sort(std::vector<T>& values, F compare) {
    std::sort(values.begin(), values.end(), std::move(compare));
}

template<class K, class V, class C>
std::map<K, V> dict_from_rows(const C& rows) {
    std::map<K, V> result;
    for (const auto& row : rows) result.insert_or_assign(std::get<0>(row), std::get<1>(row));
    return result;
}
}  // namespace kuma_rt

// Generated records are emitted in the global namespace. ADL also finds
// this when the native range writer or trace visits a nested record.
template<kuma_rt::GeneratedRecord T> requires (kuma_rt::record_output_supported<T>())
std::ostream& operator<<(kuma_rt::RecordOutputFallback output, const T& value) {
    kuma_rt::write_value_to(value.kuma_gen_value_fields(), kuma_rt::PrintOptions{}, output.stream);
    return output.stream;
}

// M6 helpers operate on ordinary C++ values. The native compiler resolves types.
// They do not claim M1-M5 borrow checking for arbitrary user-defined C++ code.
namespace kuma_rt {
template<class R>
using cpp_item = std::conditional_t<std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>,
    bool, std::remove_cvref_t<decltype(*std::begin(std::declval<const R&>()))>>;

template<class T> inline constexpr bool is_owned_iteration_view = false;
template<class Item, class... Cs>
inline constexpr bool is_owned_iteration_view<ZipView<Item, Cs...>> = true;
template<class Item, class C>
inline constexpr bool is_owned_iteration_view<EnumerateView<Item, C>> = true;

// Check nested values as well as the outer range. A vector of string_views
// or a tuple containing a reference is not an owning snapshot.
template<class T> struct CppViewOwns : std::bool_constant<std::is_arithmetic_v<T>> {};
template<> struct CppViewOwns<std::string> : std::true_type {};
template<> struct CppViewOwns<Range> : std::true_type {};
template<class T> struct CppViewOwns<const T> : CppViewOwns<T> {};
template<class T> struct CppViewOwns<std::vector<T>> : CppViewOwns<T> {};
template<class T, std::size_t N> struct CppViewOwns<std::array<T, N>> : CppViewOwns<T> {};
template<class K> struct CppViewOwns<std::set<K>> : CppViewOwns<K> {};
template<class K, class V>
struct CppViewOwns<std::map<K, V>> : std::conjunction<CppViewOwns<K>, CppViewOwns<V>> {};
template<class A, class B>
struct CppViewOwns<std::pair<A, B>> : std::conjunction<CppViewOwns<A>, CppViewOwns<B>> {};
template<class... Ts>
struct CppViewOwns<std::tuple<Ts...>> : std::conjunction<CppViewOwns<Ts>...> {};
template<class Item, class... Cs>
struct CppViewOwns<ZipView<Item, Cs...>> : std::conjunction<CppViewOwns<Cs>...> {};
template<class Item, class C> struct CppViewOwns<EnumerateView<Item, C>> : CppViewOwns<C> {};

template<class R>
concept CppViewSource = CppViewOwns<R>::value && requires (const R& r) { r.begin(); r.end(); };

// Dereference may produce tuples of references (including nested tuples from
// nested views). Preserve these while reading, but never let an eager Kuma
// collection store reference-bearing rows from a temporary/saved view.
template<class T> struct MixedSortedByView : std::false_type {};
template<class Item, class C> struct MixedSortedByView<EnumerateView<Item, C>> : std::true_type {};
template<class Item, class... Cs> struct MixedSortedByView<ZipView<Item, Cs...>> : std::true_type {};

template<class T> struct CppViewValue;
template<class T> using cpp_view_value_t = typename CppViewValue<std::remove_cvref_t<T>>::type;
template<class R, class T> using cpp_view_result_t = std::conditional_t<
    is_owned_iteration_view<std::remove_cvref_t<R>>, cpp_view_value_t<T>, T>;
template<class T> struct CppViewValue { using type = T; };
template<class... Ts>
struct CppViewValue<std::tuple<Ts...>> { using type = std::tuple<cpp_view_value_t<Ts>...>; };
template<class A, class B>
struct CppViewValue<std::pair<A, B>> { using type = std::pair<cpp_view_value_t<A>, cpp_view_value_t<B>>; };
template<class T>
struct CppViewValue<std::vector<T>> { using type = std::vector<cpp_view_value_t<T>>; };

template<class R, class T>
decltype(auto) cpp_materialize_view_result(T&& value) {
    using Input = std::remove_cvref_t<T>;
    using Output = cpp_view_value_t<T>;
    if constexpr (!is_owned_iteration_view<std::remove_cvref_t<R>> || std::is_same_v<Input, Output>) {
        return std::forward<T>(value);
    } else if constexpr (is_plain_vector<Input>) {
        Output result;
        result.reserve(value.size());
        for (auto&& item : value)
            result.emplace_back(cpp_materialize_view_result<R>(std::forward<decltype(item)>(item)));
        return result;
    } else {
        return std::apply([](auto&&... item) {
            return Output{cpp_materialize_view_result<R>(std::forward<decltype(item)>(item))...};
        }, std::forward<T>(value));
    }
}

template<class F, class T>
auto mixed_lazy_map_value(F& callback, const T& value) {
    using Result = std::remove_cvref_t<std::invoke_result_t<F&, const T&>>;
    if constexpr (ExtremaScalar<Result>) {
        return std::invoke(callback, value);
    } else {
        static_assert(ExtremaScalar<Result>,
                      "mixed lazy.map: expected int, long long, double, bool, char or std::string");
        return 0;  // Avoid cascading invalid variable/array declarations.
    }
}

template<class Acc, class F, class T>
void mixed_lazy_fold_step(Acc& acc, F& callback, const T& value) {
    static_assert(ExtremaScalar<Acc>,
                  "mixed lazy.fold: expected int, long long, double, bool, char or std::string seed");
    if constexpr (ExtremaScalar<Acc>) {
        using Result = std::invoke_result_t<F&, const Acc&, const T&>;
        static_assert(implicit_fold_assignment<Result, Acc>,
                      "mixed lazy.fold: callback result must match the seed type or widen implicitly");
        if constexpr (implicit_fold_assignment<Result, Acc>)
            acc = static_cast<Acc>(std::invoke(callback, std::as_const(acc), value));
    }
}

template<class F, class T>
auto mixed_lazy_extreme_key(F& project, const char* nan_error, const T& item) {
    if constexpr (ExtremaProjection<F, T>) {
        auto key = std::invoke(project, item);
        if constexpr (std::same_as<decltype(key), double>) {
            if (std::isnan(key)) throw std::domain_error(nan_error);
        }
        return key;
    } else {
        static_assert(ExtremaProjection<F, T>,
                      "mixed lazy.min_by/max_by: expected int, long long or double key");
        return 0;
    }
}

template<class R, class F, class P>
auto cpp_collect(const R& values, F transform, P predicate) {
    using Mapped = std::decay_t<decltype(transform(std::declval<const cpp_item<R>&>()))>;
    using Out = cpp_view_result_t<R, Mapped>;
    static_assert(!is_owned_iteration_view<R> || CppViewOwns<Out>::value,
                  "mixed owned view collection: expected owning result values");
    std::vector<Out> result;
    for (const auto& raw : values) {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>) {
            const bool item = raw;
            if (predicate(item)) result.push_back(transform(item));
        } else if (predicate(raw)) result.emplace_back(cpp_materialize_view_result<R>(transform(raw)));
    }
    return result;
}
template<class R, class F, class P>
auto cpp_flat_collect(const R& values, F transform, P predicate) {
    using Inner = std::decay_t<decltype(transform(std::declval<const cpp_item<R>&>()))>;
    using Out = cpp_view_result_t<R, typename Inner::value_type>;
    static_assert(!is_owned_iteration_view<R> || CppViewOwns<Out>::value,
                  "mixed owned view collection: expected owning result values");
    std::vector<Out> result;
    for (const auto& raw : values) {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>) {
            const bool item = raw;
            if (!predicate(item)) continue;
            auto inner = transform(item);
            result.insert(result.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
        } else if (predicate(raw)) {
            auto inner = transform(raw);
            if constexpr (!std::is_same_v<Out, typename Inner::value_type>) {
                for (auto&& item : inner)
                    result.emplace_back(cpp_materialize_view_result<R>(std::move(item)));
            } else result.insert(result.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
        }
    }
    return result;
}
// Immediate opt-in pipeline: own a source snapshot, allocate only final output,
// and do not advance past the last accepted item.
template<class T>
concept IterOwnedScalar = std::same_as<T, int> || std::same_as<T, long long>
    || std::same_as<T, double> || std::same_as<T, bool>
    || std::same_as<T, char> || std::same_as<T, std::string>;

template<IterOwnedScalar T, class P, class F, CollectionInteger K>
    requires IterOwnedScalar<std::remove_cvref_t<std::invoke_result_t<F&, const T&>>>
auto cpp_iter_filter_map_take_collect(std::vector<T> source, P&& pred, F&& map, K count) {
    using Result = std::remove_cvref_t<std::invoke_result_t<F&, const T&>>;
    std::vector<Result> collected;
    if (std::cmp_less(count, 0)) throw std::invalid_argument("iter.take: negative count");
    if (count == 0) return collected;
    for (const T& item : source) {
        if (!std::invoke(pred, item)) continue;
        collected.emplace_back(std::invoke(map, item));
        if (--count == 0) break;
    }
    return collected;
}

template<class R, class F>
auto cpp_map(const R& values, F callback) {
    return cpp_collect(values, std::move(callback), [](const auto&) { return true; });
}
template<class R, class F>
auto cpp_filter(const R& values, F callback) {
    return cpp_collect(values, [](const auto& x) { return x; }, std::move(callback));
}
template<class R>
auto cpp_sum(const R& values) {
    using E = cpp_item<R>;
    static_assert(std::is_arithmetic_v<E>, "Kuma sum requires numeric elements");
    using Out = std::conditional_t<std::is_integral_v<E>, std::common_type_t<long long, E>, E>;
    Out result{};
    for (const auto& item : values) result += item;
    return result;
}
template<class R>
long long cpp_len(const R& values) { return numeric_cast<long long>(std::size(values)); }
// Own a Kuma literal without dropping embedded NULs or keeping the C++ terminator.
template<std::size_t N>
std::string cpp_reduce_string_literal(const char (&literal)[N]) {
    return std::string(literal, N - 1);
}
template<class R, class F, class A>
auto cpp_reduce(const R& values, F callback, A initial) {
    for (const auto& item : values) initial = callback(std::move(initial), item);
    return initial;
}
template<class R, class F>
bool cpp_any(const R& values, F predicate) {
    for (const auto& item : values) if (predicate(item)) return true;
    return false;
}
template<class R, class F>
bool cpp_all(const R& values, F predicate) {
    for (const auto& item : values) if (!predicate(item)) return false;
    return true;
}
template<class R> bool cpp_any(const R& values) {
    return cpp_any(values, [](const auto& x) { return static_cast<bool>(x); });
}
template<class R> bool cpp_all(const R& values) {
    return cpp_all(values, [](const auto& x) { return static_cast<bool>(x); });
}
template<class R, class... Args>
void cpp_sort(R& values, Args... args) { std::sort(std::begin(values), std::end(values), std::move(args)...); }
template<class R, class... Args>
auto cpp_sorted(R values, Args... args) { cpp_sort(values, std::move(args)...); return values; }
template<class... Args>
std::string cpp_sorted(std::string_view values, Args... args) {
    std::string result(values);
    cpp_sort(result, std::move(args)...);
    return result;
}
template<class T, class A, class... Args>
auto cpp_sorted(std::list<T, A> values, Args... args) {
    values.sort(std::move(args)...);
    return values;
}
template<class T, class A, class... Args>
auto cpp_sorted(std::forward_list<T, A> values, Args... args) {
    values.sort(std::move(args)...);
    return values;
}
template<class R>
void cpp_reverse(R& values) { std::reverse(std::begin(values), std::end(values)); }
template<class T, class A>
void cpp_reverse(std::forward_list<T, A>& values) { values.reverse(); }
template<class R>
auto cpp_reversed(R values) { cpp_reverse(values); return values; }
inline std::string cpp_reversed(std::string_view values) {
    std::string result(values);
    cpp_reverse(result);
    return result;
}
template<class... Args>
std::vector<long long> cpp_sorted(Range values, Args&&... args) {
    auto vec = cpp_map(values, [](long long x) { return x; });
    cpp_sort(vec, std::forward<Args>(args)...);
    return vec;
}
inline std::vector<long long> cpp_reversed(Range values) {
    auto vec = cpp_map(values, [](long long x) { return x; });
    cpp_reverse(vec);
    return vec;
}
template<class R>
auto cpp_enumerate(const R& values, long long start) { return enumerate<cpp_view_result_t<R, cpp_item<R>>>(values, start); }
template<class R, class I> requires std::is_integral_v<std::remove_cvref_t<I>>
auto cpp_enumerate(const R& values, I&& start) {
    if constexpr (std::is_unsigned_v<std::remove_cvref_t<I>>) {
        const unsigned __int128 wide = start;
        if (wide > static_cast<unsigned __int128>(std::numeric_limits<long long>::max()))
            error("enumerate start exceeds signed 64-bit range");
        return enumerate<cpp_view_result_t<R, cpp_item<R>>>(values, static_cast<long long>(wide));
    } else {
        const __int128 wide = start;
        if (wide < std::numeric_limits<long long>::min() || wide > std::numeric_limits<long long>::max())
            error("enumerate start exceeds signed 64-bit range");
        return enumerate<cpp_view_result_t<R, cpp_item<R>>>(values, static_cast<long long>(wide));
    }
}
template<class... Rs>
auto cpp_zip(bool strict, const Rs&... values) { return zip<std::tuple<cpp_view_result_t<Rs, cpp_item<Rs>>...>>(strict, values...); }

template<class R>
auto cpp_view_snapshot(R&& source) {
    using RangeType = std::remove_cvref_t<R>;
    static_assert(CppViewSource<RangeType>,
                  "mixed owned view: expected a stock range of owning values (vector, array, string, set, map, Kuma range or owned view)");
    if constexpr (CppViewSource<RangeType>)
        return RangeType(std::forward<R>(source));
    else return Range{};  // Avoid secondary diagnostics for an invalid range.
}

template<class T>
bool cpp_view_strict(T value) {
    static_assert(std::is_same_v<T, bool>, "zip_view strict must be bool");
    if constexpr (std::is_same_v<T, bool>) return value;
    else return false;
}

template<class T>
long long cpp_view_start(T value) {
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>,
                  "enumerate_view start must be an integer, not bool");
    if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_unsigned_v<T>) {
            if (static_cast<unsigned __int128>(value) >
                static_cast<unsigned __int128>(std::numeric_limits<long long>::max()))
                error("enumerate start exceeds signed 64-bit range");
        } else if (static_cast<__int128>(value) < std::numeric_limits<long long>::min()
                   || static_cast<__int128>(value) > std::numeric_limits<long long>::max()) {
            error("enumerate start exceeds signed 64-bit range");
        }
        return static_cast<long long>(value);
    } else return 0;
}

template<class... Rs>
struct CppZipViewRequest { std::tuple<Rs...> sources; bool strict; };
template<class... Rs>
auto cpp_zip_view(CppZipViewRequest<Rs...>&& request) {
    using Row = std::tuple<cpp_view_value_t<cpp_item<Rs>>...>;
    return std::apply([&](auto&&... source) {
        return ZipView<Row, Rs...>{request.strict, std::move(source)...};
    }, std::move(request.sources));
}

template<class R>
struct CppEnumerateViewRequest { R source; long long start; };
template<class R>
auto cpp_enumerate_view(CppEnumerateViewRequest<R>&& request) {
    return EnumerateView<cpp_view_value_t<cpp_item<R>>, R>{std::move(request.source), request.start};
}
} // namespace kuma_rt

namespace kuma_rt {
template<class R, class... As>
decltype(auto) cpp_member_map(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).map(std::forward<As>(args)...); })
        return std::forward<R>(values).map(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_map(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_filter(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).filter(std::forward<As>(args)...); })
        return std::forward<R>(values).filter(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_filter(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_sum(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).sum(std::forward<As>(args)...); })
        return std::forward<R>(values).sum(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_sum(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_sort(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).sort(std::forward<As>(args)...); })
        return std::forward<R>(values).sort(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_sort(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_sorted(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).sorted(std::forward<As>(args)...); })
        return std::forward<R>(values).sorted(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_sorted(std::forward<R>(values), std::forward<As>(args)...);
}
template<bool IntegerLiteral = false, bool StringLiteral = false, class R, class F, class Acc>
decltype(auto) cpp_member_reduce(R&& values, F&& callback, Acc&& initial) {
    if constexpr (requires { std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial)); })
        return std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial));
    else if constexpr (is_stock_windows_view<std::remove_cvref_t<R>>) {
        if constexpr (IntegerLiteral)
            return values.reduce_rows(std::forward<F>(callback), static_cast<long long>(initial));
        else if constexpr (StringLiteral)
            return values.reduce_rows(std::forward<F>(callback), kuma_rt::cpp_reduce_string_literal(initial));
        else
            return values.reduce_rows(std::forward<F>(callback), std::forward<Acc>(initial));
    } else if constexpr (StringLiteral)
        return kuma_rt::cpp_reduce(std::forward<R>(values), std::forward<F>(callback), kuma_rt::cpp_reduce_string_literal(initial));
    else
        return kuma_rt::cpp_reduce(std::forward<R>(values), std::forward<F>(callback), std::forward<Acc>(initial));
}

template<class R, class... As> requires (sizeof...(As) != 2)
decltype(auto) cpp_member_reduce(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).reduce(std::forward<As>(args)...); })
        return std::forward<R>(values).reduce(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_reduce(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_reverse(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).reverse(std::forward<As>(args)...); })
        return std::forward<R>(values).reverse(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_reverse(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_reversed(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).reversed(std::forward<As>(args)...); })
        return std::forward<R>(values).reversed(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_reversed(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_len(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).len(std::forward<As>(args)...); })
        return std::forward<R>(values).len(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_len(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_any(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).any(std::forward<As>(args)...); })
        return std::forward<R>(values).any(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_any(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_some(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).some(std::forward<As>(args)...); })
        return std::forward<R>(values).some(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_any(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_all(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).all(std::forward<As>(args)...); })
        return std::forward<R>(values).all(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_all(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_every(R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).every(std::forward<As>(args)...); })
        return std::forward<R>(values).every(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_all(std::forward<R>(values), std::forward<As>(args)...);
}
template<class R, class... As>
decltype(auto) cpp_member_zip(bool strict, R&& values, As&&... args) {
    if constexpr (requires { std::forward<R>(values).zip(std::forward<As>(args)...); })
        return std::forward<R>(values).zip(std::forward<As>(args)...);
    else
        return kuma_rt::cpp_zip(strict, std::forward<R>(values), std::forward<As>(args)...);
}
// Adapt only stock sequences without a native count member. Keep the call
// itself outside the adapter so native braced arguments and temporaries work.
template<class C>
struct CountReceiver {
    const C& values;
    using Item = typename C::value_type;

    template<class T>
    long long count(T&& argument) const {
        using Arg = std::remove_cvref_t<T>;
        if constexpr (std::is_invocable_v<T&, Item>) {
            using Result = std::remove_cvref_t<std::invoke_result_t<T&, Item>>;
            constexpr bool valid = std::same_as<Result, bool> || CollectionInteger<Result>
                || std::same_as<Result, double>;
            static_assert(valid, "count: predicate must return bool or a number");
            if constexpr (valid) {
                return kuma_rt::count_matching<Item>(values, [&](Item item) {
                    return static_cast<bool>(std::invoke(argument, std::move(item)));
                });
            } else {
                return 0;
            }
        } else if constexpr (CollectionInteger<Item> && CollectionInteger<Arg>) {
            return kuma_rt::count_matching<Item>(values, [&](Item item) {
                return std::cmp_equal(item, argument);
            });
        } else {
            constexpr bool valid =
                std::same_as<Item, Arg> ||
                (std::same_as<Item, double> && CollectionInteger<Arg>) ||
                (std::same_as<Item, bool> && std::same_as<Arg, std::vector<bool>::reference>) ||
                (std::same_as<Item, std::string> && std::is_convertible_v<T&&, Item>);
            static_assert(valid, "count: expected an element value or a unary predicate");
            if constexpr (valid) {
                const Item needle(std::forward<T>(argument));
                return kuma_rt::count_matching<Item>(values, [&](Item item) {
                    return item == needle;
                });
            } else {
                return 0;
            }
        }
    }
};

template<class R>
constexpr decltype(auto) count_receiver(R&& receiver) noexcept {
    using C = std::remove_cvref_t<R>;
    if constexpr ((is_plain_vector<C> || is_deque<C>::value || std::same_as<C, std::string>)
                  && !std::is_volatile_v<std::remove_reference_t<R>>)
        return CountReceiver<C>{receiver};
    else
        return std::forward<R>(receiver);
}

template<class C>
struct ContainsReceiver {
    const C& values;

    template<class Key>
    bool (contains)(const Key& key) const {
        return std::find(values.begin(), values.end(), key) != values.end();
    }
};

// Only actual stock sequences need a fallback. Leave native member
// arguments (including braces and bit-fields) in the caller's full expression.
template<class R>
constexpr decltype(auto) contains_receiver(R&& values) noexcept {
    using C = std::remove_cvref_t<R>;
    if constexpr ((is_vector<C>::value || is_deque<C>::value)
                  && !std::is_volatile_v<std::remove_reference_t<R>>)
        return ContainsReceiver<C>{values};
    else
        return std::forward<R>(values);
}

template<class C>
struct ChunksReceiver {
    const C& values;

    template<CollectionInteger K>
    auto chunks(K width) const { return kuma_rt::chunks_copy(values, width); }
};

// Leave native member arguments in the caller's full expression.
template<class Receiver>
constexpr decltype(auto) chunks_receiver(Receiver&& receiver) noexcept {
    using C = std::remove_cvref_t<Receiver>;
    if constexpr ((is_vector<C>::value || std::same_as<C, std::string>)
                  && !std::is_volatile_v<std::remove_reference_t<Receiver>>)
        return ChunksReceiver<C>{receiver};
    else
        return std::forward<Receiver>(receiver);
}

template<std::size_t Arity, bool IntegerLiteral, bool StringLiteral, class R>
struct WindowsReduceReceiver {
    R&& values;
    template<class F, class Acc>
    auto reduce(F&& callback, Acc&& initial) const {
        if constexpr (IntegerLiteral)
            return values.template reduce_fields<Arity>(std::forward<F>(callback), static_cast<long long>(initial));
        else if constexpr (StringLiteral)
            return values.template reduce_fields<Arity>(std::forward<F>(callback), kuma_rt::cpp_reduce_string_literal(initial));
        else
            return values.template reduce_fields<Arity>(std::forward<F>(callback), std::forward<Acc>(initial));
    }
};

template<std::size_t Arity, bool IntegerLiteral, bool StringLiteral = false, class R>
constexpr decltype(auto) windows_reduce_receiver(R&& values) noexcept {
    if constexpr (is_stock_windows_view<std::remove_cvref_t<R>>)
        return WindowsReduceReceiver<Arity, IntegerLiteral, StringLiteral, R>{std::forward<R>(values)};
    else
        return std::forward<R>(values);  // Native member receives the original callback.
}

template<class Receiver, std::size_t Arity = 0>
struct WindowsReceiver {
    Receiver&& values;

    template<WindowsWidth K>
    auto windows(K width) const {
        if constexpr (Arity != 0) width = checked_window_binding_width<Arity>(width);
        return WindowsView<std::remove_cvref_t<Receiver>>(std::forward<Receiver>(values), width);
    }
};

template<class Receiver>
constexpr decltype(auto) windows_receiver(Receiver&& receiver) noexcept {
    using C = std::remove_cvref_t<Receiver>;
    // An allocator tied to an external resource cannot guarantee row ownership.
    constexpr bool owning_vector = is_vector<C>::value && requires {
        requires std::same_as<C, std::vector<typename C::value_type>>;
    };
    if constexpr ((owning_vector || std::same_as<C, std::string>)
                  && !std::is_volatile_v<std::remove_reference_t<Receiver>>)
        return WindowsReceiver<Receiver>{std::forward<Receiver>(receiver)};
    else
        return std::forward<Receiver>(receiver);
}

template<std::size_t Arity, long long Width, class Receiver>
constexpr decltype(auto) windows_binding_receiver(Receiver&& receiver) noexcept {
    if constexpr (std::same_as<
            decltype(windows_receiver(std::forward<Receiver>(receiver))), WindowsReceiver<Receiver>>) {
        if constexpr (Width > 0) {
            static_assert(std::cmp_equal(Arity, Width), "windows: width must match binding count");
            return windows_receiver(std::forward<Receiver>(receiver));
        } else {
            return WindowsReceiver<Receiver, Arity>{std::forward<Receiver>(receiver)};
        }
    } else {
        return std::forward<Receiver>(receiver);
    }
}

template<class T> inline constexpr bool is_stock_tuple_or_pair = false;
template<class... T> inline constexpr bool is_stock_tuple_or_pair<std::tuple<T...>> = true;
template<class T, class U> inline constexpr bool is_stock_tuple_or_pair<std::pair<T, U>> = true;

template<std::size_t I, class T, class Index>
decltype(auto) cpp_lazy_view_index(T&& value, Index index) {
    if constexpr (is_stock_tuple_or_pair<std::remove_cvref_t<T>>)
        return std::get<I>(std::forward<T>(value));
    else return std::forward<T>(value)[index];
}


template<long long I, class Receiver>
struct TupleGetReceiver {
    Receiver&& value;

    template<class Key>
    constexpr decltype(auto) (get)(Key&& key) const {
        using Tuple = std::remove_cvref_t<Receiver>;
        if constexpr (requires { (std::forward<Receiver>(value).get)(std::forward<Key>(key)); }) {
            return (std::forward<Receiver>(value).get)(std::forward<Key>(key));
        } else if constexpr (I < 0) {
            static_assert(I >= 0, "tuple/pair get: expected a nonnegative integer literal");
        } else if constexpr (!std::cmp_less(I, std::tuple_size_v<Tuple>)) {
            static_assert(std::cmp_less(I, std::tuple_size_v<Tuple>), "tuple/pair get: index out of range");
        } else if constexpr (std::is_lvalue_reference_v<Receiver>) {
            return std::get<I>(value);
        } else {
            using Element = std::remove_cvref_t<decltype(std::get<I>(std::forward<Receiver>(value)))>;
            return Element(std::get<I>(std::forward<Receiver>(value)));
        }
    }
};

template<long long I, class Receiver>
constexpr decltype(auto) tuple_get_receiver(Receiver&& value) noexcept {
    if constexpr (is_stock_tuple_or_pair<std::remove_cvref_t<Receiver>>
                  && !std::is_volatile_v<std::remove_reference_t<Receiver>>) {
        return TupleGetReceiver<I, Receiver>{std::forward<Receiver>(value)};
    } else {
        return std::forward<Receiver>(value);
    }
}

template<std::size_t Arity, class T>
constexpr bool arrow_tuple_arity() {
    if constexpr (is_stock_tuple_or_pair<T>) return std::tuple_size_v<T> == Arity;
    return false;
}

template<class Row>
auto copy_lazy_adjacent_row(const Row& row) {
    static_assert(arrow_tuple_arity<2, Row>(), "mixed lazy pairwise: expected an adjacent pair");
    if constexpr (arrow_tuple_arity<2, Row>()) {
        using A = std::remove_cvref_t<decltype(std::get<0>(row))>;
        using B = std::remove_cvref_t<decltype(std::get<1>(row))>;
        static_assert(ExtremaScalar<A> && ExtremaScalar<B>,
                      "mixed lazy pairwise: expected two owning scalar values");
        if constexpr (ExtremaScalar<A> && ExtremaScalar<B>)
            return std::tuple<A, B>{std::get<0>(row), std::get<1>(row)};
        else return std::tuple<int, int>{};
    } else return std::tuple<int, int>{};  // Keep diagnostics on the assertions above.
}

template<std::size_t Arity, bool Member, bool IntegerLiteral, bool StringLiteral = false,
         class R, class F, class Acc>
decltype(auto) cpp_reduce_tuple_arrow(R&& values, F&& callback, Acc&& initial) {
    if constexpr (Member && requires { std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial)); }) {
        return std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial));
    } else if constexpr (Member && is_stock_windows_view<std::remove_cvref_t<R>>) {
        return windows_reduce_receiver<Arity, IntegerLiteral, StringLiteral>(std::forward<R>(values))
            .reduce(std::forward<F>(callback), std::forward<Acc>(initial));
    } else {
        using Row = cpp_item<R>;
        constexpr bool matches = arrow_tuple_arity<Arity, Row>();
        static_assert(matches, "reduce arrow: expected a pair or tuple matching the parameters after the accumulator");
        if constexpr (matches) {
            auto apply = [&callback](auto&& acc, const Row& row) {
                return std::apply([&](const auto&... parts) {
                    return std::invoke(callback, std::forward<decltype(acc)>(acc), parts...);
                }, row);
            };
            if constexpr (StringLiteral)
                return cpp_reduce(values, apply, cpp_reduce_string_literal(initial));
            else
                return cpp_reduce(values, apply, std::forward<Acc>(initial));
        }
    }
}

template<std::size_t Arity, bool Filter, bool Member, class R, class F>
decltype(auto) cpp_map_filter_tuple_arrow(R&& values, F&& callback) {
    if constexpr (Member && is_stock_windows_view<std::remove_cvref_t<R>>)
        return values.template row_fields<Arity, Filter>(std::forward<F>(callback));
    else if constexpr (Member && Filter && requires { std::forward<R>(values).filter(std::forward<F>(callback)); })
        return std::forward<R>(values).filter(std::forward<F>(callback));
    else if constexpr (Member && !Filter && requires { std::forward<R>(values).map(std::forward<F>(callback)); })
        return std::forward<R>(values).map(std::forward<F>(callback));
    else if constexpr (is_plain_vector<std::remove_cvref_t<R>> || is_std_map<std::remove_cvref_t<R>>) {
        using Elem = typename std::remove_cvref_t<R>::value_type;
        if constexpr (arrow_tuple_arity<Arity, Elem>()) {
            auto apply = [&callback](const Elem& row) { return std::apply(callback, row); };
            if constexpr (Filter) return cpp_filter(values, apply);
            else return cpp_map(values, apply);
        } else {
            static_assert(arrow_tuple_arity<Arity, Elem>(), "map/filter arrow: expected a pair or tuple with matching arity");
        }
    } else if constexpr (Filter) {
        return cpp_filter(values, std::forward<F>(callback));
    } else {
        return cpp_map(values, std::forward<F>(callback));
    }
}

template<class T> inline constexpr bool is_two_tuple = false;
template<class A, class B> inline constexpr bool is_two_tuple<std::tuple<A, B>> = true;

template<std::size_t I, class T>
decltype(auto) tuple_alias(T& value) {
    if constexpr (I == 0 && requires { value.first; }) return (value.first);
    else if constexpr (I == 1 && requires { value.second; }) return (value.second);
    else return std::get<I>(value);
}

template<class C, class I, class F>
long long expand_at(const C& seq, I center, F&& pred) {
    static_assert(std::is_integral_v<I> && !std::same_as<I, bool>,
                  "Kuma expand_at: center must be an integer");
    if constexpr (std::is_integral_v<I> && !std::same_as<I, bool>) {
        using Item = typename C::value_type;
        static_assert(std::same_as<std::invoke_result_t<F&, const Item&, const Item&>, bool>,
                      "Kuma expand_at: predicate must take two elements and return bool");
        if constexpr (std::is_signed_v<I>) if (center < 0) return 0;
        const auto i = static_cast<std::make_unsigned_t<I>>(center);
        const auto n = seq.size();
        if (i >= n) return 0;
        // These are actual indices, not distances added to the center.
        // In particular, i + 1 fits even when n is the largest size_t.
        std::size_t left = static_cast<std::size_t>(i), right = left + 1;
        long long matched = 0;
        while (left != 0 && right < n) {
            // Materialize only vector<bool> proxy results, not normal values.
            const Item& a = seq[--left];
            const Item& b = seq[right++];
            if (!std::invoke(pred, a, b)) break;
            ++matched;
        }
        return matched;
    } else {
        return 0;
    }
}

template<class R, class I, class F>
decltype(auto) cpp_member_expand_at(R&& seq, I&& center, F&& pred) {
    using C = std::remove_cvref_t<R>;
    if constexpr (requires { std::forward<R>(seq).expand_at(std::forward<I>(center), std::forward<F>(pred)); }) {
        return std::forward<R>(seq).expand_at(std::forward<I>(center), std::forward<F>(pred));
    } else if constexpr (is_plain_vector<C> || is_fixed_array<C>::value || std::same_as<C, std::string>) {
        return kuma_rt::expand_at(seq, std::forward<I>(center), std::forward<F>(pred));
    } else {
        return std::forward<R>(seq).expand_at(std::forward<I>(center), std::forward<F>(pred));
    }
}

template<class R>
constexpr decltype(auto) cpp_member_pairwise(R&& values) {
    if constexpr (requires { std::forward<R>(values).pairwise(); })
        return std::forward<R>(values).pairwise();
    else
        return kuma_rt::pairwise_copy(std::forward<R>(values));
}
template<class R>
decltype(auto) cpp_member_enumerate(R&& values) {
    if constexpr (requires { std::forward<R>(values).enumerate(); })
        return std::forward<R>(values).enumerate();
    else
        return kuma_rt::cpp_enumerate(std::forward<R>(values), 0LL);
}
template<class R, class A0, class... Rest>
decltype(auto) cpp_member_enumerate(R&& values, A0&& a0, Rest&&... rest) {
    if constexpr (requires { std::forward<R>(values).enumerate(std::forward<A0>(a0), std::forward<Rest>(rest)...); })
        return std::forward<R>(values).enumerate(std::forward<A0>(a0), std::forward<Rest>(rest)...);
    else if constexpr (sizeof...(Rest) == 0)
        return kuma_rt::cpp_enumerate(std::forward<R>(values), std::forward<A0>(a0));
    else
        static_assert(sizeof...(Rest) == 0, "Kuma enumerate fallback accepts at most one start value");
}
} // namespace kuma_rt

namespace kuma_rt {
template <class Rank>
concept kth_rank = std::integral<Rank> && !std::same_as<Rank, bool>;

template <kth_rank Rank>
std::size_t kth_offset(std::size_t size, Rank k) {
    if (k <= 0 || static_cast<std::make_unsigned_t<Rank>>(k) > size)
        throw std::out_of_range("kth selection: rank must be in [1, size]");
    return static_cast<std::size_t>(k) - 1;
}

template <class K>
consteval bool kth_owned_key() {
    if constexpr (std::is_reference_v<K>) return false;
    else if constexpr (std::is_arithmetic_v<K> || std::same_as<std::remove_cv_t<K>, std::string>) return true;
    else if constexpr (::ac_detail::io::is_tuple_v<K> || ::ac_detail::io::is_pair_v<K>) {
        return []<std::size_t... I>(std::index_sequence<I...>) {
            return (kth_owned_key<std::tuple_element_t<I, K>>() && ...);
        }(std::make_index_sequence<std::tuple_size_v<K>>{});
    } else return false;
}

template <bool Largest, class T, class Alloc, kth_rank Rank, class Key>
T kth_by(const std::vector<T, Alloc>& values, Rank k, Key&& key) {
    const auto offset = ::kuma_rt::kth_offset(values.size(), k);
    using K = std::decay_t<std::invoke_result_t<Key&, const T&>>;
    static_assert(kth_owned_key<K>(), "kth selection requires scalar or owning tuple/pair keys");
    std::vector<K> keys;
    std::vector<std::size_t> order;
    keys.reserve(values.size());
    order.reserve(values.size());
    for (std::size_t i = 0; i < values.size(); ++i) {
        const T& row = values[i];
        auto&& projected = std::invoke(key, row);
        ::ac_detail::validate_ordered_finite(projected);
        keys.emplace_back(std::forward<decltype(projected)>(projected));
        order.push_back(i);
    }
    const auto nth = order.begin() + offset;
    // Reorder only indices: owned keys can have const components, and
    // moving a large compound key is unnecessary during selection.
    std::nth_element(order.begin(), nth, order.end(), [&](std::size_t a, std::size_t b) {
        const auto& lhs = std::as_const(keys)[a];
        const auto& rhs = std::as_const(keys)[b];
        if constexpr (Largest) {
            if (rhs < lhs) return true;
            if (lhs < rhs) return false;
        } else {
            if (lhs < rhs) return true;
            if (rhs < lhs) return false;
        }
        return a < b;
    });
    return values[*nth];
}

// kth_smallest — (vector, k) -> T — 1始まりで k 番目に小さい要素
/// 重複を数え、元の配列を変更しない。平均O(N)時間、追加O(N)領域。範囲外は out_of_range。
template <class T, class Alloc, kth_rank Rank>
T kth_smallest(const std::vector<T, Alloc>& values, Rank k) {
    return ::kuma_rt::kth_by<false>(values, k, std::identity{});
}

// kth_largest — (vector, k) -> T — 1始まりで k 番目に大きい要素
/// 重複を数え、元の配列を変更しない。平均O(N)時間、追加O(N)領域。範囲外は out_of_range。
template <class T, class Alloc, kth_rank Rank>
T kth_largest(const std::vector<T, Alloc>& values, Rank k) {
    return ::kuma_rt::kth_by<true>(values, k, std::identity{});
}

// kth_smallest_by — (vector, k, key) -> T — キー昇順で k 番目の元要素
/// keyは入力順に1回ずつ。キーが等しい場合も入力順。平均O(N)時間、追加O(N)領域。
template <class T, class Alloc, kth_rank Rank, class Key>
T kth_smallest_by(const std::vector<T, Alloc>& values, Rank k, Key&& key) {
    return ::kuma_rt::kth_by<false>(values, k, std::forward<Key>(key));
}

// kth_largest_by — (vector, k, key) -> T — キー降順で k 番目の元要素
/// keyは入力順に1回ずつ。キーが等しい場合も入力順。平均O(N)時間、追加O(N)領域。
template <class T, class Alloc, kth_rank Rank, class Key>
T kth_largest_by(const std::vector<T, Alloc>& values, Rank k, Key&& key) {
    return ::kuma_rt::kth_by<true>(values, k, std::forward<Key>(key));
}
} // namespace kuma_rt

// Included only for a source with a checked adaptor declaration, so ordinary
// C++ source macros cannot interfere with these new names.
#ifdef kuma_gen_enable_mixed_adaptors
namespace kuma_rt {
// Explicit standard queue and stack types are not changed.
template <bool Stack>
class MixedAdaptor {
    std::deque<long long> storage_;
    void check_nonempty() const {
        if (storage_.empty()) throw std::out_of_range(Stack ? "stack: empty" : "queue: empty");
    }
    long long peek() const {
        check_nonempty();
        if constexpr (Stack) return storage_.back();
        else return storage_.front();
    }
    void remove() {
        if constexpr (Stack) storage_.pop_back();
        else storage_.pop_front();
    }
public:
    using value_type = long long;
    bool empty() const { return storage_.empty(); }
    long long size() const {
        if (std::cmp_greater(storage_.size(), std::numeric_limits<long long>::max()))
            throw std::length_error(Stack ? "stack: size too large" : "queue: size too large");
        return static_cast<long long>(storage_.size());
    }
    void push(long long value) { storage_.push_back(value); }
    long long front() const requires (!Stack) { return peek(); }
    long long back() const requires (!Stack) {
        check_nonempty();
        return storage_.back();
    }
    long long top() const requires Stack { return peek(); }
    void pop() {
        check_nonempty();
        remove();
    }
    long long popv() {
        const auto value = peek();
        remove();
        return value;
    }
    friend std::ostream& operator<<(std::ostream& os, const MixedAdaptor& adaptor) {
        // Match the stock trace display limit without depending on its macros.
        constexpr std::size_t limit = 100;
        const auto n = adaptor.storage_.size();
        os << (Stack ? "stack[" : "queue[");
        for (std::size_t i = 0; i < n && i < limit; ++i) {
            if (i != 0) os << ", ";
            os << adaptor.storage_[Stack ? n - 1 - i : i];
        }
        if (n > limit) os << ", ...";
        return os << ']';
    }
};
using MixedQueue = MixedAdaptor<false>;
using MixedStack = MixedAdaptor<true>;
template<bool Stack> inline constexpr bool is_range_push_adaptor<MixedAdaptor<Stack>> = true;
} // namespace kuma_rt
#endif

#ifdef AC_HAS_ACL
namespace ac_detail::io {
template<int Id>
struct is_modint<kuma_rt::DynamicModint<Id>> : std::true_type {};
}
#endif
