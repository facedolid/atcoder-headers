#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include "ac_math.hpp"
#include <bit>

// 定石データ構造・DP・文字列。包除等の計算で数論補助も使用する。

namespace ac_detail::algorithm {

template <class T>
struct min_op {
    /// @brief 2値の小さい方を返す。
    constexpr T operator()(const T& a, const T& b) const {
        return std::min(a, b);
    }
};

template <class R, class Compare>
vector<range_value_t<R>> sliding_window_extreme_impl(
    const R& values, int width, Compare compare) {
    using T = range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    if (width <= 0 || width > static_cast<int>(a.size())) return {};

    deque<int> candidates;
    vector<T> result;
    result.reserve(a.size() - width + 1);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        while (!candidates.empty() && candidates.front() <= i - width)
            candidates.pop_front();
        while (!candidates.empty() &&
               !compare(a[candidates.back()], a[i]))
            candidates.pop_back();
        candidates.push_back(i);
        if (i + 1 >= width) result.push_back(a[candidates.front()]);
    }
    return result;
}

inline void require_subset_transform_size(size_t size, const char* operation) {
    if (size == 0 || (size & (size - 1)) != 0) {
        throw invalid_argument(
            string(operation) + ": values.size() must be a positive power of two");
    }
}

template <class Set>
inline constexpr bool is_unordered_set_v = requires(const Set& values) {
    values.hash_function();
    values.key_eq();
};

template <class T>
inline constexpr bool is_set_container_v =
    requires(T& values) {
        typename T::key_type;
        typename T::value_type;
        values.insert(declval<typename T::value_type>());
    } && (!requires { typename T::mapped_type; }) &&
    (requires(const T& values) { values.key_comp(); } || is_unordered_set_v<T>);

template <class A, class B>
inline constexpr bool is_same_set_container_pair_v =
    is_same_v<remove_cvref_t<A>, remove_cvref_t<B>> &&
    is_set_container_v<remove_cvref_t<A>>;

template <class Value, ranges::input_range R>
set<Value> ordered_set_from(const R& values) {
    set<Value> result;
    for (const auto& value : values) result.insert(static_cast<Value>(value));
    return result;
}

template <size_t N>
vector<ll> bitset_indices(const bitset<N>& values) {
    vector<ll> result;
    for (size_t i = 0; i < N; ++i)
        if (values[i]) result.push_back(static_cast<ll>(i));
    return result;
}

template <class Set>
using set_count_map_t = unordered_map<typename Set::key_type, size_t,
                                      typename Set::hasher, typename Set::key_equal>;

template <class Set>
set_count_map_t<Set> set_counts(const Set& values) {
    set_count_map_t<Set> counts(0, values.hash_function(), values.key_eq());
    for (const auto& value : values) ++counts[value];
    return counts;
}

template <class Set>
Set empty_set_like(const Set& values) {
    Set result(values);
    result.clear();
    return result;
}

template <class Set>
Set set_from_counts(const Set& source, const set_count_map_t<Set>& counts) {
    Set result = empty_set_like(source);
    for (const auto& [key, count] : counts) {
        for (size_t i = 0; i < count; ++i) result.insert(key);
    }
    return result;
}

}  // namespace ac_detail::algorithm

// @acf fn set_union_of — (same set pair) -> Set / (range pair) -> set<T> / (bitset<N> pair) -> bitset<N> / (bitset<N>, integral range) -> set<T> — 集合の和集合
/// @acf-doc @brief 同じ型のset系は同型・同じ多重度規則で返す。一般rangeは重複を除いたset、bitsetは同じ幅どうしで演算でき、rangeとの混在時は立ちbit位置を要素として扱う
/// @acf-doc @code (same set container, same set container) -> Set
/// @acf-doc @code (input_range, input_range) -> set<common_type_t<range_value_t<A>, range_value_t<B>>>
/// @acf-doc @code (bitset<N>, bitset<N>) -> bitset<N>
/// @acf-doc @code (bitset<N>, integral input_range) / (integral input_range, bitset<N>) -> set<common_type_t<...>>
/// @acf-doc [function] set_union_of
template <class Set>
requires ac_detail::algorithm::is_set_container_v<Set>
Set set_union_of(const Set& a, const Set& b) {
    if constexpr (ac_detail::algorithm::is_unordered_set_v<Set>) {
        auto counts = ac_detail::algorithm::set_counts(a);
        const auto b_counts = ac_detail::algorithm::set_counts(b);
        for (const auto& [key, count] : b_counts) {
            auto& result_count = counts[key];
            result_count = max(result_count, count);
        }
        return ac_detail::algorithm::set_from_counts(a, counts);
    } else {
        Set result = ac_detail::algorithm::empty_set_like(a);
        std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                       std::inserter(result, result.end()), a.key_comp());
        return result;
    }
}

// @acf fn set_intersection_of — (same set pair) -> Set / (range pair) -> set<T> / (bitset<N> pair) -> bitset<N> / (bitset<N>, integral range) -> set<T> — 集合の積集合
/// @acf-doc @brief 同じ型のset系は同型・同じ多重度規則で返す。一般rangeは重複を除いたset、bitsetは同じ幅どうしで演算でき、rangeとの混在時は立ちbit位置を要素として扱う
/// @acf-doc @code (same set container, same set container) -> Set
/// @acf-doc @code (input_range, input_range) -> set<common_type_t<range_value_t<A>, range_value_t<B>>>
/// @acf-doc @code (bitset<N>, bitset<N>) -> bitset<N>
/// @acf-doc @code (bitset<N>, integral input_range) / (integral input_range, bitset<N>) -> set<common_type_t<...>>
/// @acf-doc [function] set_intersection_of
template <class Set>
requires ac_detail::algorithm::is_set_container_v<Set>
Set set_intersection_of(const Set& a, const Set& b) {
    if constexpr (ac_detail::algorithm::is_unordered_set_v<Set>) {
        auto counts = ac_detail::algorithm::set_counts(a);
        const auto b_counts = ac_detail::algorithm::set_counts(b);
        for (auto it = counts.begin(); it != counts.end();) {
            const auto found = b_counts.find(it->first);
            if (found == b_counts.end()) {
                it = counts.erase(it);
            } else {
                it->second = min(it->second, found->second);
                ++it;
            }
        }
        return ac_detail::algorithm::set_from_counts(a, counts);
    } else {
        Set result = ac_detail::algorithm::empty_set_like(a);
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                              std::inserter(result, result.end()), a.key_comp());
        return result;
    }
}

// @acf fn set_difference_of — (same set pair) -> Set / (range pair) -> set<T> / (bitset<N> pair) -> bitset<N> / (bitset<N>, integral range) -> set<T> — 集合の差集合
/// @acf-doc @brief `a \ b` を返す。同型set系は入力型を保ち、一般rangeは重複を除いた `set<common_type_t<...>>`、bitsetは同じ幅で返す
/// @acf-doc @code (same set container, same set container) -> Set
/// @acf-doc @code (input_range, input_range) -> set<common_type_t<range_value_t<A>, range_value_t<B>>>
/// @acf-doc @code (bitset<N>, bitset<N>) -> bitset<N>
/// @acf-doc @code (bitset<N>, integral input_range) / (integral input_range, bitset<N>) -> set<common_type_t<...>>
/// @acf-doc [function] set_difference_of
template <class Set>
requires ac_detail::algorithm::is_set_container_v<Set>
Set set_difference_of(const Set& a, const Set& b) {
    if constexpr (ac_detail::algorithm::is_unordered_set_v<Set>) {
        auto counts = ac_detail::algorithm::set_counts(a);
        const auto b_counts = ac_detail::algorithm::set_counts(b);
        for (auto& [key, count] : counts) {
            const auto found = b_counts.find(key);
            if (found != b_counts.end()) {
                count = count > found->second ? count - found->second : 0;
            }
        }
        return ac_detail::algorithm::set_from_counts(a, counts);
    } else {
        Set result = ac_detail::algorithm::empty_set_like(a);
        std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                            std::inserter(result, result.end()), a.key_comp());
        return result;
    }
}

// @acf fn set_symmetric_difference_of — (same set pair) -> Set / (range pair) -> set<T> / (bitset<N> pair) -> bitset<N> / (bitset<N>, integral range) -> set<T> — 集合の対称差
/// @acf-doc @brief 一方だけにある要素を返す。同型set系は入力型を保ち、一般rangeは重複を除いた `set<common_type_t<...>>`、bitsetは同じ幅で返す
/// @acf-doc @code (same set container, same set container) -> Set
/// @acf-doc @code (input_range, input_range) -> set<common_type_t<range_value_t<A>, range_value_t<B>>>
/// @acf-doc @code (bitset<N>, bitset<N>) -> bitset<N>
/// @acf-doc @code (bitset<N>, integral input_range) / (integral input_range, bitset<N>) -> set<common_type_t<...>>
/// @acf-doc [function] set_symmetric_difference_of
template <class Set>
requires ac_detail::algorithm::is_set_container_v<Set>
Set set_symmetric_difference_of(const Set& a, const Set& b) {
    if constexpr (ac_detail::algorithm::is_unordered_set_v<Set>) {
        auto counts = ac_detail::algorithm::set_counts(a);
        const auto b_counts = ac_detail::algorithm::set_counts(b);
        for (const auto& [key, count] : b_counts) {
            auto [it, inserted] = counts.emplace(key, count);
            if (!inserted) {
                it->second = it->second > count ? it->second - count : count - it->second;
            }
        }
        return ac_detail::algorithm::set_from_counts(a, counts);
    } else {
        Set result = ac_detail::algorithm::empty_set_like(a);
        std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(),
                                      std::inserter(result, result.end()), a.key_comp());
        return result;
    }
}

// @acf fn set_is_subset — (set pair) / (range pair) / (bitset<N> pair) / (bitset<N>, integral range) -> bool — 集合の包含判定
/// @acf-doc @brief `subset` が `superset` に含まれるか判定する。同型multiset系は多重度も比較し、一般rangeは重複を無視する
/// @acf-doc @code (set container, set container) -> bool
/// @acf-doc @code (input_range, input_range) -> bool
/// @acf-doc @code (bitset<N>, bitset<N>) -> bool
/// @acf-doc @code (bitset<N>, integral input_range) / (integral input_range, bitset<N>) -> bool
/// @acf-doc [function] set_is_subset
template <class Set>
requires ac_detail::algorithm::is_set_container_v<Set>
bool set_is_subset(const Set& subset, const Set& superset) {
    if constexpr (ac_detail::algorithm::is_unordered_set_v<Set>) {
        const auto subset_counts = ac_detail::algorithm::set_counts(subset);
        const auto superset_counts = ac_detail::algorithm::set_counts(superset);
        for (const auto& [key, count] : subset_counts) {
            const auto found = superset_counts.find(key);
            if (found == superset_counts.end() || found->second < count) return false;
        }
        return true;
    } else {
        return std::includes(superset.begin(), superset.end(), subset.begin(), subset.end(),
                             superset.key_comp());
    }
}

// General ranges use mathematical set semantics and return an ordered set.
template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B> &&
         (!ac_detail::algorithm::is_same_set_container_pair_v<A, B>)
auto set_union_of(const A& a, const B& b) {
    using T = common_type_t<ranges::range_value_t<A>, ranges::range_value_t<B>>;
    auto result = ac_detail::algorithm::ordered_set_from<T>(a);
    for (const auto& value : b) result.insert(static_cast<T>(value));
    return result;
}

template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B> &&
         (!ac_detail::algorithm::is_same_set_container_pair_v<A, B>)
auto set_intersection_of(const A& a, const B& b) {
    using T = common_type_t<ranges::range_value_t<A>, ranges::range_value_t<B>>;
    const auto left = ac_detail::algorithm::ordered_set_from<T>(a);
    const auto right = ac_detail::algorithm::ordered_set_from<T>(b);
    set<T> result;
    for (const auto& value : left)
        if (right.contains(value)) result.insert(value);
    return result;
}

template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B> &&
         (!ac_detail::algorithm::is_same_set_container_pair_v<A, B>)
auto set_difference_of(const A& a, const B& b) {
    using T = common_type_t<ranges::range_value_t<A>, ranges::range_value_t<B>>;
    auto result = ac_detail::algorithm::ordered_set_from<T>(a);
    const auto right = ac_detail::algorithm::ordered_set_from<T>(b);
    for (const auto& value : right) result.erase(value);
    return result;
}

template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B> &&
         (!ac_detail::algorithm::is_same_set_container_pair_v<A, B>)
auto set_symmetric_difference_of(const A& a, const B& b) {
    using T = common_type_t<ranges::range_value_t<A>, ranges::range_value_t<B>>;
    auto result = ac_detail::algorithm::ordered_set_from<T>(a);
    const auto right = ac_detail::algorithm::ordered_set_from<T>(b);
    for (const auto& value : right) {
        const auto found = result.find(value);
        if (found == result.end()) result.insert(value);
        else result.erase(found);
    }
    return result;
}

template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B> &&
         (!ac_detail::algorithm::is_same_set_container_pair_v<A, B>)
bool set_is_subset(const A& subset, const B& superset) {
    using T = common_type_t<ranges::range_value_t<A>, ranges::range_value_t<B>>;
    const auto left = ac_detail::algorithm::ordered_set_from<T>(subset);
    const auto right = ac_detail::algorithm::ordered_set_from<T>(superset);
    for (const auto& value : left)
        if (!right.contains(value)) return false;
    return true;
}

// A finite complement needs its universe; bitset<N> uses all N positions by default.
// @acf fn set_complement_of — (universe, subset) -> set<T> / same set container / (bitset<N>) -> bitset<N> / (bitset<N>, integral range) -> set<T> — 補集合
/// @acf-doc @brief 同型set系は入力型を保ち、一般rangeはuniverseからsubsetを重複なしで除く。bitsetはN bit全体または明示したuniverse内で補集合を取り、整数rangeとも混在できる
/// @acf-doc @code (universe range, subset range) -> set<common_type_t<...>> / same set container -> same set container / (bitset<N>) -> bitset<N> / (bitset<N>, integral range) / (integral range, bitset<N>) -> set<...>
/// @acf-doc [function] set_complement_of
template <ranges::input_range Universe, ranges::input_range Subset>
requires ranges::input_range<const Universe> && ranges::input_range<const Subset>
auto set_complement_of(const Universe& universe, const Subset& subset) {
    return set_difference_of(universe, subset);
}

template <size_t N>
bitset<N> set_complement_of(const bitset<N>& values) {
    return ~values;
}

template <size_t N>
bitset<N> set_complement_of(const bitset<N>& universe,
                            const bitset<N>& subset) {
    return universe & ~subset;
}

// @acf fn set_cartesian_product_of — (range, range) / (bitset, range) / (range, bitset) -> set<pair<T,U>> — 直積集合
/// @acf-doc @brief 2つのrangeから重複のない直積集合を作り、pairの辞書順で返す。bitsetは立っているbit位置の組としてrangeと混在できる
/// @acf-doc @code (range, range) -> set<pair<range_value_t<A>, range_value_t<B>>> / (bitset<N>, range) -> set<pair<ll, range_value_t<R>>> / (range, bitset<N>) -> set<pair<range_value_t<R>, ll>>
/// @acf-doc [function] set_cartesian_product_of
template <ranges::input_range A, ranges::input_range B>
requires ranges::input_range<const A> && ranges::input_range<const B>
auto set_cartesian_product_of(const A& a, const B& b) {
    using X = ranges::range_value_t<A>;
    using Y = ranges::range_value_t<B>;
    set<pair<X, Y>> result;
    for (const auto& x : a)
        for (const auto& y : b) result.emplace(x, y);
    return result;
}

template <size_t N, size_t M>
set<pair<size_t, size_t>> set_cartesian_product_of(const bitset<N>& a,
                                                    const bitset<M>& b) {
    set<pair<size_t, size_t>> result;
    for (size_t i = 0; i < N; ++i) {
        if (!a[i]) continue;
        for (size_t j = 0; j < M; ++j)
            if (b[j]) result.emplace(i, j);
    }
    return result;
}

template <size_t N>
bitset<N> set_union_of(const bitset<N>& a, const bitset<N>& b) {
    return a | b;
}

template <size_t N>
bitset<N> set_intersection_of(const bitset<N>& a, const bitset<N>& b) {
    return a & b;
}

template <size_t N>
bitset<N> set_difference_of(const bitset<N>& a, const bitset<N>& b) {
    return a & ~b;
}

template <size_t N>
bitset<N> set_symmetric_difference_of(const bitset<N>& a,
                                      const bitset<N>& b) {
    return a ^ b;
}

template <size_t N>
bool set_is_subset(const bitset<N>& subset, const bitset<N>& superset) {
    return (subset & superset) == subset;
}

// A bitset is interpreted as the set of its zero-based bit positions when mixed with a range.
template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_union_of(const bitset<N>& a, const R& b) {
    return set_union_of(ac_detail::algorithm::bitset_indices(a), b);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_union_of(const R& a, const bitset<N>& b) {
    return set_union_of(a, ac_detail::algorithm::bitset_indices(b));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_intersection_of(const bitset<N>& a, const R& b) {
    return set_intersection_of(ac_detail::algorithm::bitset_indices(a), b);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_intersection_of(const R& a, const bitset<N>& b) {
    return set_intersection_of(a, ac_detail::algorithm::bitset_indices(b));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_difference_of(const bitset<N>& a, const R& b) {
    return set_difference_of(ac_detail::algorithm::bitset_indices(a), b);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_difference_of(const R& a, const bitset<N>& b) {
    return set_difference_of(a, ac_detail::algorithm::bitset_indices(b));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_symmetric_difference_of(const bitset<N>& a, const R& b) {
    return set_symmetric_difference_of(ac_detail::algorithm::bitset_indices(a), b);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_symmetric_difference_of(const R& a, const bitset<N>& b) {
    return set_symmetric_difference_of(a, ac_detail::algorithm::bitset_indices(b));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
bool set_is_subset(const bitset<N>& subset, const R& superset) {
    return set_is_subset(ac_detail::algorithm::bitset_indices(subset), superset);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
bool set_is_subset(const R& subset, const bitset<N>& superset) {
    return set_is_subset(subset, ac_detail::algorithm::bitset_indices(superset));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_complement_of(const bitset<N>& universe, const R& subset) {
    return set_difference_of(ac_detail::algorithm::bitset_indices(universe), subset);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R> && is_integral_v<ranges::range_value_t<R>>
auto set_complement_of(const R& universe, const bitset<N>& subset) {
    return set_difference_of(universe, ac_detail::algorithm::bitset_indices(subset));
}

template <size_t N, ranges::input_range R>
requires ranges::input_range<const R>
auto set_cartesian_product_of(const bitset<N>& a, const R& b) {
    return set_cartesian_product_of(ac_detail::algorithm::bitset_indices(a), b);
}

template <ranges::input_range R, size_t N>
requires ranges::input_range<const R>
auto set_cartesian_product_of(const R& a, const bitset<N>& b) {
    return set_cartesian_product_of(a, ac_detail::algorithm::bitset_indices(b));
}

// @acf fn each_pair — (int n, F function) -> void — 0 <= i < j < n の各ペアを走査
/// @acf-doc @brief 添字の組を i 昇順、次に j 昇順で1回ずつ処理する。計算量 O(n^2)、追加メモリ O(1)
/// @acf-doc @code each_pair(int n, F function) -> void
/// @acf-doc [function] each_pair
template <class F>
constexpr void each_pair(int n, F&& function) {
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) std::invoke(function, i, j);
}

// @acf fn each_pair_eq — (int n, F function) -> void — 0 <= i <= j < n の各ペアを走査
/// @acf-doc @brief 対角要素を含む上三角のペアを i 昇順、次に j 昇順で処理する。計算量 O(n^2)、追加メモリ O(1)
/// @acf-doc @code each_pair_eq(int n, F function) -> void
/// @acf-doc [function] each_pair_eq
template <class F>
constexpr void each_pair_eq(int n, F&& function) {
    for (int i = 0; i < n; ++i)
        for (int j = i; j < n; ++j) std::invoke(function, i, j);
}

// @acf fn each_ordered_pair — (int n, F function) -> void — 相異なる順序付き添字ペアを走査
/// @acf-doc @brief 0 <= i,j < n かつ i != j のすべてを処理する。計算量 O(n^2)、追加メモリ O(1)
/// @acf-doc @code each_ordered_pair(int n, F function) -> void
/// @acf-doc [function] each_ordered_pair
template <class F>
constexpr void each_ordered_pair(int n, F&& function) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i != j) std::invoke(function, i, j);
}

// @acf fn all_pairs — (int n, Predicate predicate) -> bool — すべての i < j が条件を満たすか判定
/// @acf-doc @brief 最初の偽で終了する。ペアがない n <= 1 は true。計算量 O(n^2)、追加メモリ O(1)
/// @acf-doc @code all_pairs(int n, Predicate predicate) -> bool
/// @acf-doc [function] all_pairs
template <class Predicate>
constexpr bool all_pairs(int n, Predicate&& predicate) {
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (!std::invoke(predicate, i, j)) return false;
    return true;
}

// @acf fn any_pair — (int n, Predicate predicate) -> bool — 条件を満たす i < j があるか判定
/// @acf-doc @brief 最初の真で終了する。ペアがない n <= 1 は false。計算量 O(n^2)、追加メモリ O(1)
/// @acf-doc @code any_pair(int n, Predicate predicate) -> bool
/// @acf-doc [function] any_pair
template <class Predicate>
constexpr bool any_pair(int n, Predicate&& predicate) {
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (std::invoke(predicate, i, j)) return true;
    return false;
}

// @acf fn map_char — (char c, string_view from, string_view to) -> char — 対応表に従い1文字を変換
/// @acf-doc @brief from の最初の c の位置に対応する to の文字を返す。長さ不一致または c 不在は invalid_argument。割り当て・文字列コピーなし、計算量 O(from.size())
/// @acf-doc @code map_char(char c, string_view from, string_view to) -> char
/// @acf-doc [function] map_char
constexpr char map_char(char c, std::string_view from, std::string_view to) {
    if (from.size() != to.size())
        throw invalid_argument("map_char: from and to must have the same size");
    const size_t index = from.find(c);
    if (index == std::string_view::npos)
        throw invalid_argument("map_char: character is not in from");
    return to[index];
}

// @acf fn gcd_range — (range) -> ull — 整数列全体の最大公約数を非負で畳み込む
/// @acf-doc @brief 整数列を左からGCDへ畳み込む。負数は絶対値、0は単位元。空列は0
/// @acf-doc @code (range) -> ull
/// @acf-doc [function] gcd_range
template <class R>
ull gcd_range(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "gcd_range requires an integral range");
    static_assert(!is_same_v<T, bool>, "gcd_range does not accept bool ranges");
    static_assert(sizeof(T) <= sizeof(ull),
                  "gcd_range requires values no wider than unsigned long long");

    ull current = 0;
    for (const T& raw : values) {
        ull value;
        if constexpr (is_signed_v<T>) {
            const i128 signed_value = static_cast<i128>(raw);
            const i128 magnitude = signed_value < 0 ? -signed_value : signed_value;
            value = static_cast<ull>(magnitude);
        } else {
            value = static_cast<ull>(raw);
        }
        current = std::gcd(current, value);
    }
    return current;
}

// @acf fn lcm_bounded_range — (range, ll limit) -> optional<ll> — 正整数列のLCMを上限内で安全に畳み込む
/// @acf-doc @brief 正の整数列のLCMを左から畳み込む。途中または最終値が `limit` を超えると `nullopt`。空列のLCMは1
/// @acf-doc @code (range, ll limit) -> optional<ll>
/// @acf-doc [function] lcm_bounded_range
template <class R>
optional<ll> lcm_bounded_range(const R& values, ll limit) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>,
                  "lcm_bounded_range requires an integral range");
    if (limit < 0) {
        throw invalid_argument(
            "lcm_bounded_range: limit must be non-negative");
    }

    ll current = 1;
    for (const T& raw : values) {
        ll item;
        if constexpr (is_signed_v<T>) {
            const i128 value = static_cast<i128>(raw);
            if (value <= 0) {
                throw invalid_argument(
                    "lcm_bounded_range: values must be positive");
            }
            item = ac_detail::checked_ll(value, "lcm_bounded_range");
        } else {
            const u128 value = static_cast<u128>(raw);
            if (value == 0) {
                throw invalid_argument(
                    "lcm_bounded_range: values must be positive");
            }
            if (value > static_cast<u128>(numeric_limits<ll>::max())) {
                throw overflow_error(
                    "lcm_bounded_range: value does not fit in long long");
            }
            item = static_cast<ll>(value);
        }
        if (current > limit) return nullopt;
        const auto next = lcm_bounded(current, item, limit);
        if (!next) return nullopt;
        current = *next;
    }
    return current <= limit ? optional<ll>(current) : nullopt;
}

// @acf fn inclusion_exclusion_sum — (int n, callback term(mask) -> T) -> T — 非空部分集合の包除符号付き和
/// @acf-doc @brief `mask=1..(1<<n)-1` の項を、立っているビット数が奇数なら足し、偶数なら引く。交差部分の値はコールバックで計算する
/// @acf-doc @code (int n, F term) -> T
/// @acf-doc [function] inclusion_exclusion_sum
template <class T, class F>
T inclusion_exclusion_sum(int n, F&& term) {
    if (n < 0 || n > 20) {
        throw invalid_argument(
            "inclusion_exclusion_sum: n must be between 0 and 20");
    }
    T result{};
    const int subset_count = 1 << n;
    for (int mask = 1; mask < subset_count; ++mask) {
        const T value = invoke(term, mask);
        if (bit_count(static_cast<unsigned>(mask)) & 1)
            result += value;
        else
            result -= value;
    }
    return result;
}

// @acf fn subset_zeta_transform — (vector<T>& values) -> bool — 部分集合の包含和変換をin-placeで行う
/// @acf-doc @brief `values[mask]` を `sum_{sub subseteq mask} values[sub]` へ上書きする部分集合ゼータ変換。長さは正の2冪、計算量は O(K 2^K)。常に true
/// @acf-doc @code (vector<T>& values) -> bool
/// @acf-doc [function] subset_zeta_transform
template <class T>
bool subset_zeta_transform(vector<T>& values) {
    ac_detail::algorithm::require_subset_transform_size(
        values.size(), "subset_zeta_transform");
    for (size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (size_t mask = 0; mask < values.size(); ++mask) {
            if (mask & bit) values[mask] += values[mask ^ bit];
        }
    }
    return true;
}

// @acf fn subset_mobius_transform — (vector<T>& values) -> bool — 部分集合のメビウス逆変換をin-placeで行う
/// @acf-doc @brief 部分集合ゼータ変換の逆変換。`values[mask]` を `sum_{sub subseteq mask} (-1)^(|mask|-|sub|) values[sub]` へ戻す。長さは正の2冪、計算量は O(K 2^K)。常に true
/// @acf-doc @code (vector<T>& values) -> bool
/// @acf-doc [function] subset_mobius_transform
template <class T>
bool subset_mobius_transform(vector<T>& values) {
    ac_detail::algorithm::require_subset_transform_size(
        values.size(), "subset_mobius_transform");
    for (size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (size_t mask = 0; mask < values.size(); ++mask) {
            if (mask & bit) values[mask] -= values[mask ^ bit];
        }
    }
    return true;
}

// @acf fn superset_zeta_transform — (vector<T>& values) -> bool — 上位集合の包含和変換をin-placeで行う
/// @acf-doc @brief `values[mask]` を `sum_{super supseteq mask} values[super]` へ上書きする上位集合ゼータ変換。長さは正の2冪、計算量は O(K 2^K)。常に true
/// @acf-doc @code (vector<T>& values) -> bool
/// @acf-doc [function] superset_zeta_transform
template <class T>
bool superset_zeta_transform(vector<T>& values) {
    ac_detail::algorithm::require_subset_transform_size(
        values.size(), "superset_zeta_transform");
    for (size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (size_t mask = 0; mask < values.size(); ++mask) {
            if (!(mask & bit)) values[mask] += values[mask | bit];
        }
    }
    return true;
}

// @acf fn superset_mobius_transform — (vector<T>& values) -> bool — 上位集合のメビウス逆変換をin-placeで行う
/// @acf-doc @brief 上位集合ゼータ変換の逆変換。`values[mask]` を `sum_{super supseteq mask} (-1)^(|super|-|mask|) values[super]` へ戻す。長さは正の2冪、計算量は O(K 2^K)。常に true
/// @acf-doc @code (vector<T>& values) -> bool
/// @acf-doc [function] superset_mobius_transform
template <class T>
bool superset_mobius_transform(vector<T>& values) {
    ac_detail::algorithm::require_subset_transform_size(
        values.size(), "superset_mobius_transform");
    for (size_t bit = 1; bit < values.size(); bit <<= 1) {
        for (size_t mask = 0; mask < values.size(); ++mask) {
            if (!(mask & bit)) values[mask] -= values[mask | bit];
        }
    }
    return true;
}

// @acf fn count_multiples_union — (ll limit, range divisors) -> ll — いずれかの正の約数で割り切れる数を包除で数える
/// @acf-doc @brief `1..limit` のうち、`divisors` の少なくとも1つで割り切れる整数の個数を包除原理で数える。重複・冗長な約数を整理し、異なる約数は20個以下とする
/// @acf-doc [function] count_multiples_union
template <class R>
ll count_multiples_union(ll limit, const R& divisors) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "count_multiples_union requires an integral range");
    if (limit <= 0) return 0;

    vector<ll> candidates;
    for (const T& divisor : divisors) {
        const i128 d = static_cast<i128>(divisor);
        if (d <= 0) {
            throw invalid_argument(
                "count_multiples_union: divisors must be positive");
        }
        if (d <= static_cast<i128>(limit)) {
            candidates.push_back(static_cast<ll>(d));
        }
    }
    sort(ALL(candidates));
    candidates.erase(unique(ALL(candidates)), candidates.end());

    // If d is a multiple of a smaller retained divisor, its multiples are
    // already covered and it can be omitted from inclusion-exclusion.
    vector<ll> minimal;
    for (ll divisor : candidates) {
        bool redundant = false;
        for (ll smaller : minimal) {
            if (divisor % smaller == 0) {
                redundant = true;
                break;
            }
        }
        if (!redundant) minimal.push_back(divisor);
    }
    if (minimal.empty()) return 0;
    if (minimal.size() > 20) {
        throw invalid_argument(
            "count_multiples_union: at most 20 non-redundant divisors are supported");
    }

    const int m = static_cast<int>(minimal.size());
    const i128 result = inclusion_exclusion_sum<i128>(m, [&](int mask) -> i128 {
        ll lcm = 1;
        for (int i = 0; i < m; ++i) {
            if (!(mask & (1 << i))) continue;
            const auto next = lcm_bounded(lcm, minimal[i], limit);
            if (!next) return 0;
            lcm = *next;
        }
        return static_cast<i128>(limit / lcm);
    });
    assert(0 <= result && result <= static_cast<i128>(limit));
    return static_cast<ll>(result);
}

// @acf fn merge_intervals — (const pair range) -> vector<pair<T,T>> — 重なる区間を左端順に統合
/// @acf-doc @brief 同じ端点型のpair列をコピー・ソートし、重なる区間と同端点で接する区間を統合する。既定は閉 `[l,r]`、`<false>` は半開 `[l,r)`。入力は変更しない。整数隣接は統合しない。O(M log M)、追加メモリ O(M)
/// @acf-doc @code template <bool Closed=true, class R> auto merge_intervals(const R& intervals)
/// @acf-doc [function] merge_intervals
template <bool Closed = true, class R>
requires(ranges::input_range<const R> && pair_like<ranges::range_value_t<const R>>)
auto merge_intervals(const R& intervals) {
    using Pair = ranges::range_value_t<const R>;
    using T = decay_t<decltype(declval<Pair>().first)>;
    static_assert(is_same_v<T, decay_t<decltype(declval<Pair>().second)>>,
                  "merge_intervals requires the same endpoint type");
    vector<pair<T, T>> sorted;
    for (const auto& [l, r] : intervals) {
        if (r < l)
            throw invalid_argument("merge_intervals: l must not exceed r");
        if constexpr (!Closed) {
            if (l == r) continue;
        }
        sorted.emplace_back(l, r);
    }
    sort(ALL(sorted));
    vector<pair<T, T>> result;
    for (const auto& [l, r] : sorted) {
        if (result.empty() || result.back().second < l)
            result.emplace_back(l, r);
        else
            chmax(result.back().second, r);
    }
    return result;
}

// @acf fn CoordinateCompressor — (vector<T>) -> CoordinateCompressor<T> — 座標圧縮の値と順位を保持
/// @acf-doc @brief 座標圧縮後の値一覧と、元の値からの順位を同時に扱う
/// @acf-doc [function] CoordinateCompressor
template <class T>
class CoordinateCompressor {
    vector<T> values_;

public:
    /// @brief 空の座標圧縮器を作る。
    CoordinateCompressor() = default;

    /// @brief 値をソート・重複削除して座標圧縮器を構築する。
    explicit CoordinateCompressor(vector<T> values) : values_(std::move(values)) {
        sort(ALL(values_));
        values_.erase(unique(ALL(values_)), values_.end());
    }

    /// @brief 圧縮後の異なる値の個数を返す。
    int size() const { return static_cast<int>(values_.size()); }

    /// @brief `value` 以上の最初の圧縮順位を返す。値が存在しなくてもよい。
    int lower_bound(const T& value) const {
        return static_cast<int>(std::lower_bound(ALL(values_), value) -
                                values_.begin());
    }

    /// @brief 登録済みの `value` の圧縮順位を返す。
    int index(const T& value) const {
        auto it = std::lower_bound(ALL(values_), value);
        assert(it != values_.end() && *it == value);
        return static_cast<int>(it - values_.begin());
    }

    /// @brief 圧縮順位 `index` に対応する元の値を返す。
    const T& operator[](int index) const {
        assert(0 <= index && index < size());
        return values_[index];
    }

    /// @brief 圧縮後の値を昇順で保持するvectorを返す。
    const vector<T>& values() const { return values_; }
};

// @acf fn Imos — (int n) -> Imos<T> — 1次元の半開区間加算をまとめて反映
/// @acf-doc @brief `[l,r)` への加算を O(1) で登録し、`build()` で全要素を O(N) に復元
/// @acf-doc [function] Imos
template <class T>
class Imos {
    int n_ = 0;
    vector<T> difference_;

public:
    /// @brief 要素数0の差分配列を作る。
    Imos() = default;

    /// @brief 要素数 `n` の1次元いもす法を構築する。
    explicit Imos(int n)
        : n_(n), difference_(n >= 0 ? n + 1 : 0, T{}) {
        assert(n >= 0);
    }

    /// @brief 対象配列の要素数を返す。
    int size() const { return n_; }

    /// @brief 半開区間 `[l,r)` に `value` を加算登録する。
    void add(int l, int r, const T& value) {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return;
        difference_[l] += value;
        difference_[r] -= value;
    }

    /// @brief 登録した全区間を反映した配列を返す。
    vector<T> build() const {
        vector<T> result(n_);
        T current{};
        for (int i = 0; i < n_; ++i) {
            current += difference_[i];
            result[i] = current;
        }
        return result;
    }
};

// @acf fn Imos2D — (int h, int w) -> Imos2D<T> — 2次元の半開矩形加算をまとめて反映
/// @acf-doc @brief `[r1,r2)×[c1,c2)` への加算を登録し、`build()` でグリッドへ復元
/// @acf-doc [function] Imos2D
template <class T>
class Imos2D {
    int h_ = 0, w_ = 0;
    vector<vector<T>> difference_;

public:
    /// @brief 高さ・幅0の差分グリッドを作る。
    Imos2D() = default;

    /// @brief `h`×`w` の2次元いもす法を構築する。
    Imos2D(int h, int w)
        : h_(h),
          w_(w),
          difference_(h >= 0 && w >= 0 ? h + 1 : 0,
                      vector<T>(w >= 0 ? w + 1 : 0)) {
        assert(h >= 0 && w >= 0);
    }

    /// @brief `(高さ, 幅)` を返す。
    pair<int, int> size() const { return {h_, w_}; }

    /// @brief 半開矩形 `[r1,r2)×[c1,c2)` に `value` を加算登録する。
    void add(int r1, int c1, int r2, int c2, const T& value) {
        assert(0 <= r1 && r1 <= r2 && r2 <= h_);
        assert(0 <= c1 && c1 <= c2 && c2 <= w_);
        if (r1 == r2 || c1 == c2) return;
        difference_[r1][c1] += value;
        difference_[r2][c1] -= value;
        difference_[r1][c2] -= value;
        difference_[r2][c2] += value;
    }

    /// @brief 登録した全矩形を反映した2次元配列を返す。
    vector<vector<T>> build() const {
        auto result = difference_;
        for (int r = 0; r < h_; ++r)
            for (int c = 0; c < w_; ++c) {
                if (r > 0) result[r][c] += result[r - 1][c];
                if (c > 0) result[r][c] += result[r][c - 1];
                if (r > 0 && c > 0) result[r][c] -= result[r - 1][c - 1];
            }
        result.resize(h_);
        for (auto& row : result) row.resize(w_);
        return result;
    }
};

// @acf fn Fenwick — (int n) -> Fenwick<T> — 0-indexの一点加算・半開区間和
/// @acf-doc @brief 0-indexで `add(i,x)`、`sum(l,r)`（`[l,r)`）を O(log N) で処理する。nは0以上でINT_MAX未満。n=INT_MAXは確保前にlength_error。
/// @acf-doc [function] Fenwick
template <class T>
class Fenwick {
    int n_ = 0;
    vector<T> bit_;

public:
    /// @brief 要素数0のFenwick木を作る。
    Fenwick() = default;

    /// @brief 要素数 `n` のFenwick木を構築する。
    explicit Fenwick(int n) : n_(n) {
        assert(n >= 0);
        if (n == numeric_limits<int>::max())
            throw length_error("Fenwick: size is too large for int indexing");
        bit_.assign(static_cast<size_t>(n) + 1, T{});
    }
    /// @brief 初期配列 `values` の各値を登録したFenwick木を構築する。
    explicit Fenwick(const vector<T>& values) : Fenwick(sz(values)) {
        for (int i = 0; i < n_; ++i) add(i, values[i]);
    }

    /// @brief 要素数を返す。
    int size() const { return n_; }

    /// @brief 添字 `index` に `value` を加算する。
    void add(int index, const T& value) {
        assert(0 <= index && index < n_);
        for (unsigned i = static_cast<unsigned>(index) + 1;
             i <= static_cast<unsigned>(n_); i += i & -i) bit_[i] += value;
    }

    /// @brief 半開区間 `[0,right)` の総和を返す。
    T sum(int right) const {
        assert(0 <= right && right <= n_);
        T result{};
        for (int i = right; i > 0; i -= i & -i) result += bit_[i];
        return result;
    }

    /// @brief 半開区間 `[left,right)` の総和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        return sum(right) - sum(left);
    }

    /// @brief 添字 `index` の現在値を返す。
    T get(int index) const {
        assert(0 <= index && index < n_);
        return sum(index, index + 1);
    }

    /// @brief 添字 `index` の値を `value` に置き換える。
    void set(int index, const T& value) { add(index, value - get(index)); }

    /// @brief 全要素の総和を返す。
    T total() const { return sum(n_); }

    // 非負値を登録したとき、prefix sum が target 以上になる最小の添字。
    // 存在しなければ size()、target<=0 なら 0 を返す。
    /// @brief prefix sumが `target` 以上になる最小の境界を返す。
    int lower_bound_prefix(const T& target) const {
        if (!(T{} < target)) return 0;
        int index = 0;
        T accumulated{};
        int step = 1;
        while (step <= n_ / 2) step <<= 1;
        for (; step > 0; step >>= 1) {
            int next = index + step;
            if (next <= n_ && accumulated + bit_[next] < target) {
                index = next;
                accumulated += bit_[next];
            }
        }
        return index;
    }
};

// @acf fn RangeFenwick — (int n) -> RangeFenwick<T> — 0-indexの半開区間加算・区間和
/// @acf-doc @brief 数値型 `T` の `[l,r)` 加算と半開区間和を O(log N) で処理する。内部は2本のFenwick。nは0以上でINT_MAX未満。n=INT_MAXは確保前にlength_error。
/// @acf-doc @code (int n) -> RangeFenwick<T>
/// @acf-doc [function] RangeFenwick
template <class T>
class RangeFenwick {
    int n_ = 0;
    Fenwick<T> slope_;
    Fenwick<T> intercept_;

    /// @brief 区間加算の差分イベントを登録する内部処理。
    void add_event(int index, const T& slope, const T& intercept) {
        if (index == n_) return;
        slope_.add(index, slope);
        intercept_.add(index, intercept);
    }

    /// @brief `[0,right)` の総和を計算する内部処理。
    T prefix_sum(int right) const {
        const T width = static_cast<T>(right);
        return slope_.sum(right) * width + intercept_.sum(right);
    }

public:
    /// @brief 要素数0のRangeFenwickを作る。
    RangeFenwick() = default;

    /// @brief 要素数 `n` の区間加算Fenwick木を構築する。
    explicit RangeFenwick(int n) : n_(n), slope_(n), intercept_(n) {
        assert(n >= 0);
    }
    /// @brief 初期配列 `values` を登録したRangeFenwickを構築する。
    explicit RangeFenwick(const vector<T>& values) : RangeFenwick(sz(values)) {
        for (int i = 0; i < n_; ++i) add(i, i + 1, values[i]);
    }

    /// @brief 要素数を返す。
    int size() const { return n_; }

    /// @brief 半開区間 `[left,right)` に `value` を加算する。
    void add(int left, int right, const T& value) {
        assert(0 <= left && left <= right && right <= n_);
        if (left == right) return;
        const T l = static_cast<T>(left);
        const T r = static_cast<T>(right);
        add_event(left, value, -value * l);
        add_event(right, -value, value * r);
    }

    /// @brief 半開区間 `[0,right)` の総和を返す。
    T sum(int right) const {
        assert(0 <= right && right <= n_);
        return prefix_sum(right);
    }

    /// @brief 半開区間 `[left,right)` の総和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        return sum(right) - sum(left);
    }

    /// @brief 添字 `index` の現在値を返す。
    T get(int index) const {
        assert(0 <= index && index < n_);
        return sum(index, index + 1);
    }

    /// @brief 添字 `index` の値を `value` に置き換える。
    void set(int index, const T& value) {
        assert(0 <= index && index < n_);
        add(index, index + 1, value - get(index));
    }

    /// @brief 全要素の総和を返す。
    T total() const { return sum(n_); }
};

// @acf fn KSmallestSum — (int k) -> KSmallestSum<T=ll> — 追加・1個削除つきの小さい方からK個の和
/// @acf-doc @brief 重複を含む値を `insert`・`erase_one` で O(log N) で更新し、小さい方から `min(k,size())` 個の和を `sum()` で O(1) で返す。`k>=0`、和と更新途中の加減算が `T` に収まること
/// @acf-doc [function] KSmallestSum
template <class T = ll>
class KSmallestSum {
    size_t k_;
    multiset<T> small_, large_;
    T sum_{};

    // 全 small_ <= 全 large_ を保ったまま、small_ の個数だけを調整する。
    void rebalance() {
        while (small_.size() > k_) {
            auto it = std::prev(small_.end());
            sum_ -= *it;
            large_.insert(*it);
            small_.erase(it);
        }
        while (small_.size() < k_ && !large_.empty()) {
            auto it = large_.begin();
            sum_ += *it;
            small_.insert(*it);
            large_.erase(it);
        }
    }

public:
    /// @brief 小さい方から選ぶ個数を `k>=0` に固定して空の集合を作る。
    explicit KSmallestSum(int k) : k_(static_cast<size_t>(k)) {
        if (k < 0) throw invalid_argument("KSmallestSum: k must be non-negative");
    }

    /// @brief 重複を含めた全要素数を返す。
    size_t size() const { return small_.size() + large_.size(); }

    /// @brief 小さい方から `min(k,size())` 個の和を返す。空集合と `k=0` は0。
    T sum() const { return sum_; }

    /// @brief 小さい方から `k` 番目の値。`k=0` または要素数不足なら nullopt。
    optional<T> kth() const {
        if (k_ == 0 || small_.size() < k_) return nullopt;
        return *small_.rbegin();
    }

    /// @brief 値を1個追加する。重複・負数も扱う。
    void insert(const T& value) {
        if (!small_.empty() && !(*small_.rbegin() < value)) {
            small_.insert(value);
            sum_ += value;
        } else {
            large_.insert(value);
        }
        rebalance();
    }

    /// @brief 等しい値を1個だけ削除して true。不在なら変更せず false。
    bool erase_one(const T& value) {
        auto it = small_.find(value);
        if (it != small_.end()) {
            sum_ -= *it;
            small_.erase(it);
        } else {
            it = large_.find(value);
            if (it == large_.end()) return false;
            large_.erase(it);
        }
        rebalance();
        return true;
    }
};

// @acf fn UnionFind — (int n) -> UnionFind — 経路圧縮とサイズ併合の素集合データ構造
/// @acf-doc @brief `unite`・`same`・連結成分サイズをほぼ O(1)（償却）で扱う
/// @acf-doc [function] UnionFind
class UnionFind {
    vector<int> parent_or_size_;
    int components_ = 0;

    /// @brief 経路圧縮を伴わずに代表元を探す内部処理。
    int root_const(int x) const {
        assert(0 <= x && x < sz(parent_or_size_));
        while (parent_or_size_[x] >= 0) x = parent_or_size_[x];
        return x;
    }

public:
    /// @brief 要素数0のUnion-Findを作る。
    UnionFind() = default;

    /// @brief `n` 個の互いに異なる要素からUnion-Findを構築する。
    explicit UnionFind(int n)
        : parent_or_size_(n >= 0 ? n : 0, -1), components_(n >= 0 ? n : 0) {
        assert(n >= 0);
    }

    /// @brief 要素数を返す。
    int size() const { return sz(parent_or_size_); }

    /// @brief 要素 `x` の属する集合の代表元を返す。
    int find(int x) {
        assert(0 <= x && x < size());
        if (parent_or_size_[x] < 0) return x;
        return parent_or_size_[x] = find(parent_or_size_[x]);
    }

    /// @brief `a` と `b` の集合を併合し、併合できたらtrueを返す。
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (parent_or_size_[a] > parent_or_size_[b]) swap(a, b);
        parent_or_size_[a] += parent_or_size_[b];
        parent_or_size_[b] = a;
        --components_;
        return true;
    }

    /// @brief `a` と `b` が同じ集合に属するか判定する。
    bool same(int a, int b) { return find(a) == find(b); }

    /// @brief `x` の属する集合の要素数を返す。
    int component_size(int x) { return -parent_or_size_[find(x)]; }

    /// @brief 現在の集合数を返す。
    int count() const { return components_; }

    /// @brief 各集合を頂点一覧として返す。
    vector<vi> groups() const {
        vector<vi> result(size());
        for (int v = 0; v < size(); ++v) result[root_const(v)].push_back(v);
        result.erase(remove_if(ALL(result), [](const vi& group) {
                          return group.empty();
                      }),
                     result.end());
        return result;
    }
};

// @acf fn WeightedUnionFind — (int n) -> WeightedUnionFind<T> — 差分制約付きの素集合データ構造
/// @acf-doc @brief `value[y] - value[x]` の制約を併合し、成分内の差分をほぼ O(1)（償却）で扱う
/// @acf-doc [function] WeightedUnionFind
template <class T>
class WeightedUnionFind {
    vector<int> parent_or_size_;
    vector<T> weight_;
    int components_ = 0;

    /// @brief 経路圧縮しながら代表元を探す内部処理。
    int root(int x) {
        assert(0 <= x && x < size());
        if (parent_or_size_[x] < 0) return x;
        int parent = parent_or_size_[x];
        int result = root(parent);
        weight_[x] += weight_[parent];
        parent_or_size_[x] = result;
        return result;
    }

public:
    /// @brief 要素数0の重み付きUnion-Findを作る。
    WeightedUnionFind() = default;

    /// @brief `n` 個の要素から重み付きUnion-Findを構築する。
    explicit WeightedUnionFind(int n)
        : parent_or_size_(n >= 0 ? n : 0, -1),
          weight_(n >= 0 ? n : 0, T{}),
          components_(n >= 0 ? n : 0) {
        assert(n >= 0);
    }

    /// @brief 要素数を返す。
    int size() const { return sz(parent_or_size_); }

    /// @brief 要素 `x` の属する集合の代表元を返す。
    int find(int x) { return root(x); }

    /// @brief 要素 `x` の基準元からの相対ポテンシャルを返す。
    T potential(int x) {
        root(x);
        return weight_[x];
    }

    /// @brief `x` と `y` が同じ集合に属するか判定する。
    bool same(int x, int y) { return root(x) == root(y); }

    // Enforce value[y] - value[x] == difference.
    // Returns false only when the new constraint contradicts an existing one.
    /// @brief `value[y] - value[x] == difference` の制約を追加する。
    bool unite(int x, int y, T difference) {
        int rx = root(x);
        int ry = root(y);
        difference += weight_[x];
        difference -= weight_[y];
        if (rx == ry) return difference == T{};

        if (parent_or_size_[rx] > parent_or_size_[ry]) {
            swap(rx, ry);
            difference = T{} - difference;
        }
        parent_or_size_[rx] += parent_or_size_[ry];
        parent_or_size_[ry] = rx;
        weight_[ry] = difference;
        --components_;
        return true;
    }

    /// @brief 同じ集合に属する `x`,`y` の `value[y] - value[x]` を返す。
    T diff(int x, int y) {
        assert(same(x, y));
        return potential(y) - potential(x);
    }

    /// @brief `x` の属する集合の要素数を返す。
    int component_size(int x) {
        return -parent_or_size_[root(x)];
    }

    /// @brief 現在の集合数を返す。
    int count() const { return components_; }
};

// @acf fn SegmentTree — (vector<T>, identity[, op]) -> SegmentTree<T> — 汎用の静的モノイド区間木
/// @acf-doc @brief 半開区間の結合・一点更新を O(log N) で処理。`max_right` / `min_left` も提供。内部配列の要素数がintに収まらない場合は、確保前にlength_error。
/// @acf-doc [function] SegmentTree
template <class T, class Op = plus<T>>
class SegmentTree {
    int n_ = 0, size_ = 1;
    T identity_{};
    Op op_{};
    vector<T> data_{2, T{}};

    /// @brief 要素数・単位元・演算を指定して内部配列を初期化する。
    void init(int n, const T& identity, Op op) {
        assert(n >= 0);
        n_ = n;
        identity_ = identity;
        op_ = std::move(op);
        size_ = 1;
        while (size_ < n_) {
            if (size_ > numeric_limits<int>::max() / 4)
                throw length_error("SegmentTree: size is too large for int indexing");
            size_ <<= 1;
        }
        data_.assign(2 * size_, identity_);
    }

public:
    /// @brief 要素数0のSegmentTreeを作る。
    SegmentTree() = default;

    /// @brief 要素数 `n`、単位元 `identity` のSegmentTreeを構築する。
    explicit SegmentTree(int n, T identity = T{}, Op op = Op{}) {
        init(n, identity, std::move(op));
    }
    /// @brief 初期配列 `values` からSegmentTreeを構築する。
    explicit SegmentTree(const vector<T>& values, T identity = T{}, Op op = Op{}) {
        init(sz(values), identity, std::move(op));
        for (int i = 0; i < n_; ++i) data_[size_ + i] = values[i];
        for (int i = size_ - 1; i > 0; --i)
            data_[i] = op_(data_[i << 1], data_[i << 1 | 1]);
    }

    /// @brief 元配列の要素数を返す。
    int size() const { return n_; }

    /// @brief 添字 `index` の値を更新する。
    void set(int index, const T& value) {
        assert(0 <= index && index < n_);
        index += size_;
        data_[index] = value;
        while (index >>= 1)
            data_[index] = op_(data_[index << 1], data_[index << 1 | 1]);
    }

    /// @brief 添字 `index` の値を返す。
    T get(int index) const {
        assert(0 <= index && index < n_);
        return data_[size_ + index];
    }

    /// @brief 半開区間 `[left,right)` を演算 `op` で畳み込む。
    T prod(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        T left_result = identity_, right_result = identity_;
        left += size_;
        right += size_;
        while (left < right) {
            if (left & 1) left_result = op_(left_result, data_[left++]);
            if (right & 1) right_result = op_(data_[--right], right_result);
            left >>= 1;
            right >>= 1;
        }
        return op_(left_result, right_result);
    }

    /// @brief 全要素を演算 `op` で畳み込む。
    T all_prod() const { return data_[1]; }

    /// @brief `predicate` を保つ最長の左からの半開区間の右端を返す。
    template <class F>
    int max_right(int left, F predicate) const {
        assert(0 <= left && left <= n_ && predicate(identity_));
        if (left == n_) return n_;
        left += size_;
        T accumulated = identity_;
        do {
            while ((left & 1) == 0) left >>= 1;
            if (!predicate(op_(accumulated, data_[left]))) {
                while (left < size_) {
                    left <<= 1;
                    T candidate = op_(accumulated, data_[left]);
                    if (predicate(candidate)) {
                        accumulated = std::move(candidate);
                        ++left;
                    }
                }
                return left - size_;
            }
            accumulated = op_(accumulated, data_[left]);
            ++left;
        } while ((left & -left) != left);
        return n_;
    }

    /// @brief `predicate` を保つ最長の右からの半開区間の左端を返す。
    template <class F>
    int min_left(int right, F predicate) const {
        assert(0 <= right && right <= n_ && predicate(identity_));
        if (right == 0) return 0;
        right += size_;
        T accumulated = identity_;
        do {
            --right;
            while (right > 1 && (right & 1)) right >>= 1;
            if (!predicate(op_(data_[right], accumulated))) {
                while (right < size_) {
                    right = right * 2 + 1;
                    T candidate = op_(data_[right], accumulated);
                    if (predicate(candidate)) {
                        accumulated = std::move(candidate);
                        --right;
                    }
                }
                return right + 1 - size_;
            }
            accumulated = op_(data_[right], accumulated);
        } while ((right & -right) != right);
        return 0;
    }
};

// @acf fn SparseTable — (vector<T>[, op]) -> SparseTable<T> — 更新なしの静的区間畳み込み
/// @acf-doc @brief 構築 O(N log N)、一般の結合演算は query O(log N)、冪等演算は idempotent_query O(1)
/// @acf-doc [function] SparseTable
template <class T, class Op = ac_detail::algorithm::min_op<T>>
class SparseTable {
    vector<vector<T>> table_;
    vector<int> logarithm_;
    Op op_{};

public:
    /// @brief 空のSparseTableを作る。
    SparseTable() = default;

    /// @brief 初期配列 `values` からSparseTableを構築する。
    explicit SparseTable(const vector<T>& values, Op op = Op{})
        : op_(std::move(op)) {
        build(values);
    }

    /// @brief 初期配列から区間畳み込み表を再構築する。
    void build(const vector<T>& values) {
        int n = sz(values);
        logarithm_.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) logarithm_[i] = logarithm_[i / 2] + 1;
        table_.clear();
        if (n == 0) return;
        table_.push_back(values);
        for (int k = 1; (1 << k) <= n; ++k) {
            int length = n - (1 << k) + 1;
            table_.emplace_back(length);
            for (int i = 0; i < length; ++i)
                table_[k][i] = op_(table_[k - 1][i],
                                   table_[k - 1][i + (1 << (k - 1))]);
        }
    }

    /// @brief 元配列の要素数を返す。
    int size() const { return table_.empty() ? 0 : sz(table_[0]); }

    /// @brief 半開区間 `[left,right)` を一般の演算で畳み込む。
    T query(int left, int right) const {
        assert(0 <= left && left < right && right <= size());
        int length = right - left;
        int first = logarithm_[length];
        T result = table_[first][left];
        left += 1 << first;
        length -= 1 << first;
        for (int k = logarithm_[length]; k >= 0; --k) {
            if ((1 << k) > length) continue;
            result = op_(result, table_[k][left]);
            left += 1 << k;
            length -= 1 << k;
        }
        return result;
    }

    /// @brief 冪等演算の半開区間 `[left,right)` をO(1)で畳み込む。
    T idempotent_query(int left, int right) const {
        assert(0 <= left && left < right && right <= size());
        int k = logarithm_[right - left];
        return op_(table_[k][left], table_[k][right - (1 << k)]);
    }
};

// @acf fn count_subarrays_with_sum — (range, ll target) -> ll — 和がtargetになる非空部分配列の個数
/// @acf-doc @brief prefix sum `p` ごとに過去の `p-target` の個数を数え、和が `target` になる非空部分配列を期待 O(N) で求める。累積値は `ll` に収まる必要がある
/// @acf-doc @code (range, ll target) -> ll
/// @acf-doc [function] count_subarrays_with_sum
template <class R>
ll count_subarrays_with_sum(const R& values, ll target) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "count_subarrays_with_sum requires an integral range");
    unordered_map<ll, ll> frequency;
    frequency.emplace(0, 1);
    ll prefix = 0;
    ll result = 0;
    for (const T& value : values) {
        prefix += static_cast<ll>(value);
        auto it = frequency.find(prefix - target);
        if (it != frequency.end()) result += it->second;
        ++frequency[prefix];
    }
    return result;
}

// @acf fn count_equal_pairs — (range) -> ll — 同じ値の非空ペアの個数を期待O(N)で数える
/// @acf-doc @brief 左から過去の同値要素の個数を足し、`i<j` かつ `a[i]==a[j]` の組を期待 O(N) で数える。要素は `ll` に収まる必要がある
/// @acf-doc [function] count_equal_pairs
template <class R>
ll count_equal_pairs(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "count_equal_pairs requires an integral range");
    unordered_map<ll, ll> frequency;
    ll result = 0;
    for (const T& value : values) {
        const ll x = static_cast<ll>(value);
        result += frequency[x];
        ++frequency[x];
    }
    return result;
}

// @acf fn count_pairs_with_sum — (range, ll target) -> ll — 和がtargetになる非空ペアの個数を期待O(N)で数える
/// @acf-doc @brief 左から過去の `target-x` の個数を足し、`i<j` かつ `a[i]+a[j]==target` の組を期待 O(N) で数える。要素と和は `ll` に収まる必要がある
/// @acf-doc [function] count_pairs_with_sum
template <class R>
ll count_pairs_with_sum(const R& values, ll target) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "count_pairs_with_sum requires an integral range");
    unordered_map<ll, ll> frequency;
    ll result = 0;
    for (const T& value : values) {
        const ll x = static_cast<ll>(value);
        const i128 complement = static_cast<i128>(target) - static_cast<i128>(x);
        if (complement >= static_cast<i128>(numeric_limits<ll>::min()) &&
            complement <= static_cast<i128>(numeric_limits<ll>::max())) {
            auto it = frequency.find(static_cast<ll>(complement));
            if (it != frequency.end()) result += it->second;
        }
        ++frequency[x];
    }
    return result;
}

// Exact DP strategies. A forced strategy still obeys the resource budgets.
enum class DpMethod { Auto, Dense, Value, Enumerate, MeetInMiddle };
struct DpOptions {
    DpMethod method;
    size_t memory_limit_bytes;
    ull work_limit;
    DpOptions(DpMethod method = DpMethod::Auto,
              size_t memory_limit_bytes = 128ULL * 1024 * 1024,
              ull work_limit = 200'000'000)
        : method(method), memory_limit_bytes(memory_limit_bytes), work_limit(work_limit) {}
};

namespace ac_detail::dp {
inline void validate(DpOptions o, bool value_axis) {
    switch (o.method) {
        case DpMethod::Auto: case DpMethod::Dense:
        case DpMethod::Enumerate: case DpMethod::MeetInMiddle: return;
        case DpMethod::Value: if (value_axis) return; break;
    }
    throw invalid_argument("DP: unsupported method for this function");
}
inline void require_budget(i128 bytes, i128 work, DpOptions o) {
    if (bytes > static_cast<i128>(o.memory_limit_bytes) ||
        work > static_cast<i128>(o.work_limit))
        throw length_error("DP: no exact method fits the memory/work limits");
}
template <class T>
void append(vector<T>& items, T item, DpOptions o) {
    // Allow for geometric growth, including the old buffer during reallocation.
    require_budget((static_cast<i128>(items.size()) + 1) * sizeof(T) * 4,
                   static_cast<i128>(items.size()) + 1, o);
    items.push_back(std::move(item));
}
struct Planner {
    DpOptions options;
    i128 input_bytes, best_work = -1, best_bytes = 0;
    DpMethod best = DpMethod::Auto;
    void offer(DpMethod method, i128 bytes, i128 work) {
        if (options.method != DpMethod::Auto && options.method != method) return;
        bytes += input_bytes;
        if (bytes > static_cast<i128>(options.memory_limit_bytes) ||
            work > static_cast<i128>(options.work_limit)) return;
        if (best_work < 0 || work < best_work || (work == best_work && bytes < best_bytes)) {
            best = method; best_work = work; best_bytes = bytes;
        }
    }
    DpMethod finish() const {
        if (best == DpMethod::Auto)
            throw length_error("DP: no exact method fits the memory/work limits");
        return best;
    }
};
inline i128 states(size_t n) {
    return n < 63 ? (i128(1) << n) : -1;
}
inline void offer_enumeration(Planner& p, size_t n, size_t item_bytes) {
    const i128 all = states(n), left = states(n / 2), right = states(n - n / 2);
    if (all >= 0) p.offer(DpMethod::Enumerate, i128(n + 1) * 128, 2 * all);
    if (left >= 0 && right >= 0)
        p.offer(DpMethod::MeetInMiddle, (left + right) * item_bytes,
                (left + right) * (n - n / 2 + 2));
}

inline ll subset_max(const vector<ll>& items, ll limit, DpOptions o) {
    validate(o, false);
    if (items.empty() || limit == 0) return 0;
    const i128 words = static_cast<i128>(limit) / 64 + 1;
    Planner plan{o, static_cast<i128>(items.capacity()) * sizeof(ll)};
    plan.offer(DpMethod::Dense, words * sizeof(ull), (items.size() + i128(1)) * words);
    offer_enumeration(plan, items.size(), sizeof(ll));
    const auto method = plan.finish();
    if (method == DpMethod::Dense) {
        vector<ull> bits(static_cast<size_t>(words), 0);
        bits[0] = 1;
        for (ll x : items) {
            const size_t whole = static_cast<size_t>(x / 64);
            const unsigned shift = static_cast<unsigned>(x % 64);
            for (size_t j = bits.size(); j-- > whole;) {
                ull added = bits[j - whole] << shift;
                if (shift && j > whole) added |= bits[j - whole - 1] >> (64 - shift);
                bits[j] |= added;
            }
        }
        const unsigned last = static_cast<unsigned>(limit % 64);
        if (last != 63) bits.back() &= (ull(1) << (last + 1)) - 1;
        for (size_t j = bits.size(); j-- > 0;)
            if (bits[j]) return static_cast<ll>(j * 64 + 63 - countl_zero(bits[j]));
        return 0;
    }
    ll best = 0;
    if (method == DpMethod::Enumerate) {
        auto visit = [&](auto&& self, size_t i, ll sum) -> void {
            if (i == items.size()) { best = max(best, sum); return; }
            self(self, i + 1, sum);
            if (items[i] <= limit - sum) self(self, i + 1, sum + items[i]);
        };
        visit(visit, 0, 0);
        return best;
    }
    const size_t middle = items.size() / 2;
    auto enumerate_half = [&](size_t first, size_t last) {
        vector<ll> sums;
        sums.reserve(static_cast<size_t>(states(last - first)));
        sums.push_back(0);
        for (size_t i = first; i < last; ++i) {
            const size_t old = sums.size();
            for (size_t j = 0; j < old; ++j)
                if (items[i] <= limit - sums[j]) sums.push_back(sums[j] + items[i]);
        }
        return sums;
    };
    auto left = enumerate_half(0, middle);
    auto right = enumerate_half(middle, items.size());
    sort(right.begin(), right.end());
    for (ll x : left) {
        auto it = upper_bound(right.begin(), right.end(), limit - x);
        best = max(best, x + *prev(it));
    }
    return best;
}

using Item = pair<i128, i128>; // non-negative weight/gain and value/cost
// cover=false: maximize value with weight <= bound.
// cover=true: minimize cost with gain >= bound.
inline optional<i128> optimize(const vector<Item>& items, ll bound, bool cover, DpOptions o) {
    validate(o, true);
    i128 total_weight = 0, total_value = 0;
    for (auto [w, v] : items) {
        total_weight = checked_add_i128(total_weight, w, "DP");
        total_value = checked_add_i128(total_value, v, "DP");
    }
    if (cover && bound == 0) return i128(0);
    if (cover && total_weight < bound) return nullopt;
    if (items.empty()) return i128(0);
    const i128 width = cover ? i128(bound) : min(i128(bound), total_weight);
    Planner plan{o, static_cast<i128>(items.capacity()) * sizeof(Item)};
    // Check indexing before narrowing; memory/work products remain in i128.
    if (width < static_cast<i128>(numeric_limits<size_t>::max() / sizeof(i128)))
        plan.offer(DpMethod::Dense, (width + 1) * sizeof(i128), (width + 1) * items.size());
    if (total_value < static_cast<i128>(numeric_limits<size_t>::max() / sizeof(i128)))
        plan.offer(DpMethod::Value, (total_value + 1) * sizeof(i128), (total_value + 1) * items.size());
    offer_enumeration(plan, items.size(), sizeof(Item));
    const auto method = plan.finish();
    if (method == DpMethod::Dense) {
        const size_t cap = static_cast<size_t>(width);
        vector<i128> dp(cap + 1, cover ? -1 : 0);
        dp[0] = 0;
        for (auto [w, v] : items) {
            const size_t delta = static_cast<size_t>(min(w, width));
            for (size_t j = cap + 1; j-- > 0;) {
                if (cover) {
                    if (dp[j] < 0) continue;
                    const size_t to = j + min(delta, cap - j);
                    const i128 candidate = checked_add_i128(dp[j], v, "min_cost_to_reach");
                    if (dp[to] < 0 || candidate < dp[to]) dp[to] = candidate;
                } else if (w <= static_cast<i128>(j)) {
                    dp[j] = max(dp[j], checked_add_i128(dp[j - delta], v, "knapsack01_max_value"));
                }
            }
        }
        if (cover && dp.back() < 0) return nullopt;
        return dp.back();
    }
    if (method == DpMethod::Value) {
        const size_t cap = static_cast<size_t>(total_value);
        vector<i128> dp(cap + 1, -1);
        dp[0] = 0;
        for (auto [w, v] : items) {
            const size_t delta = static_cast<size_t>(v);
            for (size_t j = cap + 1; j-- > delta;) {
                if (dp[j - delta] < 0) continue;
                i128 candidate = checked_add_i128(dp[j - delta], w, "DP");
                if (cover) dp[j] = max(dp[j], min(i128(bound), candidate));
                else if (candidate <= bound && (dp[j] < 0 || candidate < dp[j])) dp[j] = candidate;
            }
        }
        if (cover) {
            for (size_t j = 0; j <= cap; ++j) if (dp[j] >= bound) return i128(j);
            return nullopt;
        }
        for (size_t j = cap + 1; j-- > 0;) if (dp[j] >= 0) return i128(j);
    }
    optional<i128> best = cover ? nullopt : optional<i128>(0);
    auto accept = [&](i128 w, i128 v) {
        if (cover ? w >= bound : w <= bound)
            if (!best || (cover ? v < *best : v > *best)) best = v;
    };
    if (method == DpMethod::Enumerate) {
        auto visit = [&](auto&& self, size_t i, i128 w, i128 v) -> void {
            if (i == items.size()) { accept(w, v); return; }
            self(self, i + 1, w, v);
            auto [dw, dv] = items[i];
            if (cover || dw <= i128(bound) - w)
                self(self, i + 1, cover ? min(i128(bound), w + dw) : w + dw, v + dv);
        };
        visit(visit, 0, 0, 0);
        return best;
    }
    const size_t middle = items.size() / 2;
    auto enumerate_half = [&](size_t first, size_t last) {
        vector<Item> sums;
        sums.reserve(static_cast<size_t>(states(last - first)));
        sums.emplace_back(0, 0);
        for (size_t i = first; i < last; ++i) {
            auto [w, v] = items[i];
            const size_t old = sums.size();
            for (size_t j = 0; j < old; ++j)
                if (cover || w <= i128(bound) - sums[j].first)
                    sums.emplace_back(cover ? min(i128(bound), sums[j].first + w) : sums[j].first + w,
                                      sums[j].second + v);
        }
        return sums;
    };
    auto left = enumerate_half(0, middle);
    auto right = enumerate_half(middle, items.size());
    sort(right.begin(), right.end());
    if (cover) {
        for (size_t j = right.size() - 1; j > 0; --j)
            right[j - 1].second = min(right[j - 1].second, right[j].second);
        for (auto [w, v] : left) {
            auto it = lower_bound(right.begin(), right.end(), Item{max(i128(0), i128(bound) - w), -1});
            if (it != right.end()) accept(i128(bound), v + it->second);
        }
    } else {
        for (size_t j = 1; j < right.size(); ++j)
            right[j].second = max(right[j].second, right[j - 1].second);
        for (auto [w, v] : left) {
            auto it = upper_bound(right.begin(), right.end(), i128(bound) - w,
                                  [](i128 x, const Item& item) { return x < item.first; });
            if (it != right.begin()) accept(i128(bound), v + prev(it)->second);
        }
    }
    return best;
}
} // namespace ac_detail::dp

// @acf fn subset_sum_max_leq — (range, ll limit, DpOptions options={}) -> ll — 上限以下の部分和の最大値
/// @acf-doc @brief 非負整数の最大部分和。既定Autoはbitset DP・全探索・半分全列挙から予算内の方式を選ぶ。全方式で厳密解を返し、適用範囲外はlength_error
/// @acf-doc @code (range, ll limit, DpOptions options={}) -> ll
/// @acf-doc [function] subset_sum_max_leq
template <class R>
ll subset_sum_max_leq(const R& values, ll limit, DpOptions options = {}) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "subset_sum_max_leq requires an integral range");
    if (limit < 0) throw invalid_argument("subset_sum_max_leq: limit must be non-negative");
    ac_detail::dp::validate(options, false);
    vector<ll> items;
    i128 total = 0;
    for (const T& value : values) {
        const i128 x = static_cast<i128>(value);
        if (x < 0) throw invalid_argument("subset_sum_max_leq: values must be non-negative");
        const ll item = ac_detail::checked_ll(x, "subset_sum_max_leq");
        if (item > 0 && item <= limit) {
            ac_detail::dp::append(items, item, options);
            total = min(i128(limit), total + item);
        }
    }
    return ac_detail::dp::subset_max(items, static_cast<ll>(total), options);
}

// @acf fn subset_sum_possible — (range, ll target, DpOptions options={}) -> bool — 非負整数の0-1部分和判定
/// @acf-doc @brief 既定Autoで厳密な部分和判定。target<0はfalse。手動方式・資源上限はDpOptionsで指定
/// @acf-doc @code (range, ll target, DpOptions options={}) -> bool
/// @acf-doc [function] subset_sum_possible
template <class R>
bool subset_sum_possible(const R& values, ll target, DpOptions options = {}) {
    if (target < 0) return false;
    return subset_sum_max_leq(values, target, options) == target;
}

// @acf fn bounded_subset_sum_possible — (values, counts, ll target, DpOptions options={}) -> bool — 個数制限付き部分和判定
/// @acf-doc @brief 個数を二進分割し、Autoで部分和を判定する。値・個数は非負、target<0はfalse。中間状態と計算量をDpOptionsの予算内に制限
/// @acf-doc @code (values, counts, ll target, DpOptions options={}) -> bool
/// @acf-doc [function] bounded_subset_sum_possible
template <class VR, class CR>
bool bounded_subset_sum_possible(const VR& values, const CR& counts, ll target, DpOptions options = {}) {
    using V = ac_detail::algorithm::range_value_t<VR>;
    using C = ac_detail::algorithm::range_value_t<CR>;
    static_assert(is_integral_v<V> && is_integral_v<C>, "bounded_subset_sum_possible requires integral ranges");
    if (target < 0) return false;
    ac_detail::dp::validate(options, false);
    vector<ll> items;
    i128 total = 0;
    auto it = begin(values); auto jt = begin(counts);
    for (; it != end(values) && jt != end(counts); ++it, ++jt) {
        const i128 x = static_cast<i128>(*it), count = static_cast<i128>(*jt);
        if (x < 0 || count < 0) throw invalid_argument("bounded_subset_sum_possible: values/counts must be non-negative");
        if (x == 0 || x > target) continue;
        i128 remaining = min(count, i128(target) / x);
        for (i128 block = 1; remaining > 0; block *= 2) {
            const i128 take = min(block, remaining);
            ac_detail::dp::append(items, static_cast<ll>(x * take), options);
            total = min(i128(target), total + x * take);
            remaining -= take;
        }
    }
    if (it != end(values) || jt != end(counts))
        throw invalid_argument("bounded_subset_sum_possible: values and counts must have the same length");
    if (total < target) return false;
    return ac_detail::dp::subset_max(items, target, options) == target;
}

// @acf fn knapsack01_max_value — (weights, values, ll capacity, DpOptions options={}) -> ll — 0-1ナップサックの最大価値
/// @acf-doc @brief Autoで容量DP・価値DP・全探索・半分全列挙を選ぶ。重さは非負、負の価値は選ばない。ゼロ重みも各1回。答えのll超過はoverflow_error
/// @acf-doc @code (weights, values, ll capacity, DpOptions options={}) -> ll
/// @acf-doc [function] knapsack01_max_value
template <class WR, class VR>
ll knapsack01_max_value(const WR& weights, const VR& values, ll capacity, DpOptions options = {}) {
    using W = ac_detail::algorithm::range_value_t<WR>;
    using V = ac_detail::algorithm::range_value_t<VR>;
    static_assert(is_integral_v<W> && is_integral_v<V>, "knapsack01_max_value requires integral ranges");
    if (capacity < 0) throw invalid_argument("knapsack01_max_value: capacity must be non-negative");
    ac_detail::dp::validate(options, true);
    vector<ac_detail::dp::Item> items;
    auto it = begin(weights); auto jt = begin(values);
    for (; it != end(weights) && jt != end(values); ++it, ++jt) {
        const i128 w = static_cast<i128>(*it), v = static_cast<i128>(*jt);
        if (w < 0) throw invalid_argument("knapsack01_max_value: weights must be non-negative");
        if (w <= capacity && v > 0) ac_detail::dp::append(items, ac_detail::dp::Item{w, v}, options);
    }
    if (it != end(weights) || jt != end(values))
        throw invalid_argument("knapsack01_max_value: weights and values must have the same length");
    return ac_detail::checked_ll(*ac_detail::dp::optimize(items, capacity, false, options), "knapsack01_max_value");
}

// @acf fn min_cost_to_reach — (gains, costs, ll target, DpOptions options={}) -> optional<ll> — 0-1最小コスト閾値到達
/// @acf-doc @brief Autoで利益DP・費用DP・全探索・半分全列挙を選ぶ。利益・費用は非負、到達不能はnullopt。手動指定でも予算を検査
/// @acf-doc @code (gains, costs, ll target, DpOptions options={}) -> optional<ll>
/// @acf-doc [function] min_cost_to_reach
template <class GR, class CR>
optional<ll> min_cost_to_reach(const GR& gains, const CR& costs, ll target, DpOptions options = {}) {
    using G = ac_detail::algorithm::range_value_t<GR>;
    using C = ac_detail::algorithm::range_value_t<CR>;
    static_assert(is_integral_v<G> && is_integral_v<C>, "min_cost_to_reach requires integral ranges");
    if (target < 0) throw invalid_argument("min_cost_to_reach: target must be non-negative");
    ac_detail::dp::validate(options, true);
    vector<ac_detail::dp::Item> items;
    auto it = begin(gains); auto jt = begin(costs);
    for (; it != end(gains) && jt != end(costs); ++it, ++jt) {
        const i128 gain = static_cast<i128>(*it), cost = static_cast<i128>(*jt);
        if (gain < 0 || cost < 0) throw invalid_argument("min_cost_to_reach: gains/costs must be non-negative");
        if (gain > 0 && target > 0)
            ac_detail::dp::append(items, ac_detail::dp::Item{min(i128(target), gain), cost}, options);
    }
    if (it != end(gains) || jt != end(costs))
        throw invalid_argument("min_cost_to_reach: gains and costs must have the same length");
    const auto result = ac_detail::dp::optimize(items, target, true, options);
    if (!result) return nullopt;
    return ac_detail::checked_ll(*result, "min_cost_to_reach");
}

// @acf fn min_partition_max_sum — (range, DpOptions options={}) -> ll — 2群分割の大きい和の最小値
/// @acf-doc @brief 合計の半分以下の最大部分和をAutoで求め、全体から引く。intを超える合計にも対応。答えのll超過はoverflow_error
/// @acf-doc @code (range, DpOptions options={}) -> ll
/// @acf-doc [function] min_partition_max_sum
template <class R>
ll min_partition_max_sum(const R& values, DpOptions options = {}) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "min_partition_max_sum requires an integral range");
    ac_detail::dp::validate(options, false);
    vector<ll> items;
    i128 total = 0;
    for (const T& value : values) {
        const i128 x = static_cast<i128>(value);
        if (x < 0) throw invalid_argument("min_partition_max_sum: values must be non-negative");
        const ll item = ac_detail::checked_ll(x, "min_partition_max_sum");
        total = ac_detail::checked_add_i128(total, x, "min_partition_max_sum");
        if (item > 0) ac_detail::dp::append(items, item, options);
    }
    const ll half = ac_detail::checked_ll(total / 2, "min_partition_max_sum");
    items.erase(remove_if(items.begin(), items.end(), [&](ll x) { return x > half; }), items.end());
    return ac_detail::checked_ll(total - ac_detail::dp::subset_max(items, half, options), "min_partition_max_sum");
}

// @acf fn min_k_span — (range, int k) -> optional<ll> — K個を含む最小1次元区間の幅
/// @acf-doc @brief 整数列から `k` 個を選んだときの最小の `max-min` を O(N log N) で求める。要素数不足や `k<=0` は nullopt
/// @acf-doc @code (range, int k) -> optional<ll>
/// @acf-doc [function] min_k_span
template <class R>
optional<ll> min_k_span(const R& values, int k) {
    using T = ac_detail::algorithm::range_value_t<R>;
    static_assert(is_integral_v<T>, "min_k_span requires an integral range");
    static_assert(!is_same_v<remove_cv_t<T>, bool>,
                  "min_k_span does not accept bool ranges");

    vector<T> sorted(begin(values), end(values));
    if (k <= 0 || k > static_cast<int>(sorted.size())) return nullopt;
    sort(sorted.begin(), sorted.end());

    i128 answer = static_cast<i128>(sorted[k - 1]) - static_cast<i128>(sorted[0]);
    for (int right = k; right < static_cast<int>(sorted.size()); ++right) {
        const i128 width = static_cast<i128>(sorted[right])
                         - static_cast<i128>(sorted[right - k + 1]);
        if (width < answer) answer = width;
    }
    return ac_detail::checked_ll(answer, "min_k_span");
}

// @acf fn SubsetSumCounter — (int target, size_t memory_limit_bytes=128MiB) -> SubsetSumCounter<Mint> — 追加削除つき部分和の通り数
/// @acf-doc @brief 加減算可能な `Mint` 上で値の追加・削除を O(target) で処理し、部分和の通り数を返す。追加時は降順、削除時は昇順に更新する
/// @acf-doc @code (int target, size_t memory_limit_bytes=128MiB) -> SubsetSumCounter<Mint>
/// @acf-doc [function] SubsetSumCounter
template <class Mint>
class SubsetSumCounter {
    int target_ = 0;
    vector<Mint> ways_;
    vector<int> multiplicity_;

    /// @brief `value` が対象範囲に入っていることを確認する。
    void check_value(int value) const {
        if (value <= 0 || value > target_)
            throw invalid_argument("SubsetSumCounter: value must be in [1, target]");
    }

public:
    /// @brief 目標和 `target` 以下の部分和カウンタを構築する。
    explicit SubsetSumCounter(int target, size_t memory_limit_bytes = 128ULL * 1024 * 1024) : target_(target) {
        if (target < 0) throw invalid_argument("SubsetSumCounter: target must be non-negative");
        if (target == numeric_limits<int>::max())
            throw length_error("SubsetSumCounter: target is too large for int indexing");
        const size_t cells = static_cast<size_t>(target) + 1;
        if (i128(cells) * (sizeof(Mint) + sizeof(int)) > i128(memory_limit_bytes))
            throw length_error("SubsetSumCounter: memory limit exceeded");
        ways_.assign(cells, Mint{});
        multiplicity_.assign(cells, 0);
        ways_[0] = 1;
    }

    /// @brief 値 `value` を1個追加する。
    void add(int value) {
        check_value(value);
        if (multiplicity_[value] == numeric_limits<int>::max())
            throw overflow_error("SubsetSumCounter: multiplicity exceeds int range");
        for (int sum = target_; sum >= value; --sum) ways_[sum] += ways_[sum - value];
        ++multiplicity_[value];
    }

    /// @brief 値 `value` を1個削除する。
    void erase(int value) {
        check_value(value);
        if (multiplicity_[value] == 0)
            throw invalid_argument("SubsetSumCounter: cannot erase a value that is not present");
        for (int sum = value; sum <= target_; ++sum) ways_[sum] -= ways_[sum - value];
        --multiplicity_[value];
    }

    /// @brief 和 `sum` を作る部分集合の通り数を返す。
    Mint count(int sum) const {
        if (sum < 0 || sum > target_)
            throw invalid_argument("SubsetSumCounter: sum must be in [0, target]");
        return ways_[sum];
    }

    /// @brief 扱える最大の和を返す。
    int target() const { return target_; }
};

// @acf fn inversion_count — (range) -> ll — 配列の転倒数を O(N log N) で数える
/// @acf-doc @brief `i<j` かつ `a[i]>a[j]` となる組の個数。重複値は転倒に数えない
/// @acf-doc [function] inversion_count
template <class R>
ll inversion_count(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    CoordinateCompressor<T> compressor(a);
    Fenwick<ll> bit(compressor.size());
    ll result = 0;
    for (auto it = a.rbegin(); it != a.rend(); ++it) {
        int index = compressor.index(*it);
        result += bit.sum(index);
        bit.add(index, 1);
    }
    return result;
}

// @acf fn lis_length — (range[, strict]) -> int — 最長増加部分列の長さ
/// @acf-doc @brief `strict=true` なら狭義、`false` なら広義のLIS長を O(N log N) で求める
/// @acf-doc [function] lis_length
template <class R>
int lis_length(const R& values, bool strict = true) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> tails;
    for (const T& value : values) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), value)
                         : upper_bound(tails.begin(), tails.end(), value);
        if (it == tails.end()) tails.push_back(value);
        else *it = value;
    }
    return sz(tails);
}

// @acf fn lis_indices — (range[, strict]) -> vi — LISの添字を1つ復元
/// @acf-doc @brief LISの長さだけでなく、元配列上の添字列を O(N log N) で復元
/// @acf-doc [function] lis_indices
template <class R>
vi lis_indices(const R& values, bool strict = true) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    vector<T> tails;
    vi tail_indices, previous(a.size(), -1);
    for (int i = 0; i < sz(a); ++i) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), a[i])
                         : upper_bound(tails.begin(), tails.end(), a[i]);
        int pos = static_cast<int>(it - tails.begin());
        if (pos > 0) previous[i] = tail_indices[pos - 1];
        if (it == tails.end()) {
            tails.push_back(a[i]);
            tail_indices.push_back(i);
        } else {
            *it = a[i];
            tail_indices[pos] = i;
        }
    }
    vi result;
    if (tail_indices.empty()) return result;
    for (int i = tail_indices.back(); i != -1; i = previous[i])
        result.push_back(i);
    reverse(ALL(result));
    return result;
}

// @acf fn lis_member_indices — (range[, strict]) -> vi — いずれかのLISに属する添字
/// @acf-doc @brief 最長増加部分列の少なくとも1つに含められる元配列の添字をすべて O(N log N) で返す。`strict=false` は広義単調
/// @acf-doc [function] lis_member_indices
template <class R>
vi lis_member_indices(const R& values, bool strict = true) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    vector<int> left(a.size()), right(a.size());
    vector<T> increasing_tails, decreasing_tails;
    const auto descending = [](const T& lhs, const T& rhs) { return rhs < lhs; };

    for (int i = 0; i < sz(a); ++i) {
        auto it = strict ? lower_bound(increasing_tails.begin(), increasing_tails.end(), a[i])
                         : upper_bound(increasing_tails.begin(), increasing_tails.end(), a[i]);
        left[i] = static_cast<int>(it - increasing_tails.begin()) + 1;
        if (it == increasing_tails.end()) increasing_tails.push_back(a[i]);
        else *it = a[i];
    }
    for (int i = sz(a) - 1; i >= 0; --i) {
        auto it = strict
                      ? lower_bound(decreasing_tails.begin(), decreasing_tails.end(), a[i], descending)
                      : upper_bound(decreasing_tails.begin(), decreasing_tails.end(), a[i], descending);
        right[i] = static_cast<int>(it - decreasing_tails.begin()) + 1;
        if (it == decreasing_tails.end()) decreasing_tails.push_back(a[i]);
        else *it = a[i];
    }

    const int length = sz(increasing_tails);
    vi result;
    for (int i = 0; i < sz(a); ++i) {
        if (left[i] + right[i] - 1 == length) result.push_back(i);
    }
    return result;
}

// @acf fn prefix_lis_query — (values, rights, limits) -> vi — 接頭辞・上限制約つきLISクエリ
/// @acf-doc @brief 各クエリ `(right, limit)` について、先頭 `right` 個から値が `limit` 以下の厳密増加部分列の最大長を O((N+Q) log N + Q log Q) で返す。3列のクエリ数は同じである必要がある
/// @acf-doc @code (values, rights, limits) -> vi
/// @acf-doc [function] prefix_lis_query
template <class VR, class RR, class XR>
vi prefix_lis_query(const VR& values, const RR& rights, const XR& limits) {
    using V = ac_detail::algorithm::range_value_t<VR>;
    using R = ac_detail::algorithm::range_value_t<RR>;
    using X = ac_detail::algorithm::range_value_t<XR>;
    static_assert(is_integral_v<V>, "prefix_lis_query requires integral values");
    static_assert(is_integral_v<R>, "prefix_lis_query requires integral prefix lengths");
    static_assert(is_integral_v<X>, "prefix_lis_query requires integral limits");

    vector<i128> a;
    for (const V& value : values) a.push_back(static_cast<i128>(value));
    if (a.size() > static_cast<size_t>(numeric_limits<int>::max())) {
        throw invalid_argument("prefix_lis_query: values are too long");
    }
    const int n = static_cast<int>(a.size());

    struct Query {
        int right;
        i128 limit;
        size_t index;
    };
    vector<Query> queries;
    auto right_it = begin(rights);
    auto limit_it = begin(limits);
    const auto right_end = end(rights);
    const auto limit_end = end(limits);
    while (right_it != right_end && limit_it != limit_end) {
        const i128 right = static_cast<i128>(*right_it);
        if (right < 0 || right > static_cast<i128>(n)) {
            throw invalid_argument("prefix_lis_query: prefix lengths must be in [0, N]");
        }
        queries.push_back({static_cast<int>(right), static_cast<i128>(*limit_it), queries.size()});
        ++right_it;
        ++limit_it;
    }
    if (right_it != right_end || limit_it != limit_end) {
        throw invalid_argument("prefix_lis_query: rights and limits must have the same length");
    }

    vector<i128> coordinates = a;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    auto max_op = [](int lhs, int rhs) { return max(lhs, rhs); };
    SegmentTree<int, decltype(max_op)> segment(
        static_cast<int>(coordinates.size()), 0, max_op);

    vector<size_t> order(queries.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](size_t lhs, size_t rhs) {
        return queries[lhs].right < queries[rhs].right;
    });

    vi result(queries.size(), 0);
    int processed = 0;
    for (const size_t query_index : order) {
        while (processed < queries[query_index].right) {
            const int position = static_cast<int>(
                lower_bound(coordinates.begin(), coordinates.end(), a[processed]) -
                coordinates.begin());
            const int candidate = segment.prod(0, position) + 1;
            segment.set(position, max(segment.get(position), candidate));
            ++processed;
        }
        const int last = static_cast<int>(
            upper_bound(coordinates.begin(), coordinates.end(), queries[query_index].limit) -
            coordinates.begin());
        result[queries[query_index].index] = segment.prod(0, last);
    }
    return result;
}

// @acf fn sliding_window_min — (range, width) -> vector<T> — スライド区間の最小値
/// @acf-doc @brief 幅 `width` の各半開窓の最小値を償却 O(N) で返す
/// @acf-doc [function] sliding_window_min
template <class R>
vector<ac_detail::algorithm::range_value_t<R>> sliding_window_min(
    const R& values, int width) {
    using T = ac_detail::algorithm::range_value_t<R>;
    return ac_detail::algorithm::sliding_window_extreme_impl(
        values, width, less<T>{});
}

// @acf fn sliding_window_max — (range, width) -> vector<T> — スライド区間の最大値
/// @acf-doc @brief 幅 `width` の各半開窓の最大値を償却 O(N) で返す
/// @acf-doc [function] sliding_window_max
template <class R>
vector<ac_detail::algorithm::range_value_t<R>> sliding_window_max(
    const R& values, int width) {
    using T = ac_detail::algorithm::range_value_t<R>;
    return ac_detail::algorithm::sliding_window_extreme_impl(
        values, width, greater<T>{});
}

// @acf fn next_greater_indices — (range) -> vi — 右側の次のより大きい要素の添字
/// @acf-doc @brief 各位置から右へ見て最初に現れる「より大きい値」の添字を O(N) で返す
/// @acf-doc [function] next_greater_indices
template <class R>
vi next_greater_indices(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    vi result(sz(a), sz(a)), stack;
    for (int i = 0; i < sz(a); ++i) {
        while (!stack.empty() && a[stack.back()] < a[i]) {
            result[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    return result;
}

// @acf fn next_smaller_indices — (range) -> vi — 右側の次のより小さい要素の添字
/// @acf-doc @brief 各位置から右へ見て最初に現れる「より小さい値」の添字を O(N) で返す
/// @acf-doc [function] next_smaller_indices
template <class R>
vi next_smaller_indices(const R& values) {
    using T = ac_detail::algorithm::range_value_t<R>;
    vector<T> a(std::begin(values), std::end(values));
    vi result(sz(a), sz(a)), stack;
    for (int i = 0; i < sz(a); ++i) {
        while (!stack.empty() && a[stack.back()] > a[i]) {
            result[stack.back()] = i;
            stack.pop_back();
        }
        stack.push_back(i);
    }
    return result;
}

// @acf fn prefix_function — (string_view) -> vi — KMPの接頭辞関数
/// @acf-doc @brief `s[0..i]` の真の接頭辞でもある接尾辞の最大長を各位置で求める
/// @acf-doc [function] prefix_function
inline vi prefix_function(string_view s) {
    vi result(s.size());
    for (int i = 1; i < sz(s); ++i) {
        int j = result[i - 1];
        while (j > 0 && s[i] != s[j]) j = result[j - 1];
        if (s[i] == s[j]) ++j;
        result[i] = j;
    }
    return result;
}

// @acf fn z_function — (string_view) -> vi — 文字列の各位置からのLCP長
/// @acf-doc @brief `z[0]=|s|`、`z[i]` は `s` と `s[i..]` の最長共通接頭辞長
/// @acf-doc [function] z_function
inline vi z_function(string_view s) {
    int n = sz(s);
    vi result(n);
    if (n == 0) return result;
    result[0] = n;
    int left = 0, right = 0;
    for (int i = 1; i < n; ++i) {
        if (i < right) result[i] = min(right - i, result[i - left]);
        while (i + result[i] < n && s[result[i]] == s[i + result[i]])
            ++result[i];
        if (i + result[i] > right) {
            left = i;
            right = i + result[i];
        }
    }
    return result;
}

// @acf fn kmp_search — (text, pattern) -> vi — KMPでパターンの出現位置を列挙
/// @acf-doc @brief 文字列 `text` 中の `pattern` の開始位置を重複込みで O(|text|+|pattern|) に列挙
/// @acf-doc [function] kmp_search
inline vi kmp_search(string_view text, string_view pattern) {
    if (pattern.empty()) return {};
    vi prefix = prefix_function(pattern), result;
    int matched = 0;
    for (int i = 0; i < sz(text); ++i) {
        while (matched > 0 && text[i] != pattern[matched])
            matched = prefix[matched - 1];
        if (text[i] == pattern[matched]) ++matched;
        if (matched == sz(pattern)) {
            result.push_back(i - sz(pattern) + 1);
            matched = prefix[matched - 1];
        }
    }
    return result;
}

// @acf fn lcs_length — (string_view s, string_view t, DpOptions options={}) -> int — 2文字列の最長共通部分列長
/// @acf-doc @brief 2つの文字列の最長共通部分列（LCS）の長さを1次元DPで返す。時間 O(|s||t|)、追加メモリ O(min(|s|,|t|))
/// @acf-doc @code (string_view s, string_view t, DpOptions options={}) -> int
/// @acf-doc [function] lcs_length
inline int lcs_length(string_view s, string_view t, DpOptions options = {}) {
    if (options.method != DpMethod::Auto && options.method != DpMethod::Dense)
        throw invalid_argument("lcs_length: only Auto/Dense is supported");
    if (min(s.size(), t.size()) > static_cast<size_t>(numeric_limits<int>::max()))
        throw overflow_error("lcs_length: result may exceed int range");
    if (s.empty() || t.empty()) return 0;
    ac_detail::dp::require_budget((i128(min(s.size(), t.size())) + 1) * sizeof(int),
                                 i128(s.size()) * t.size(), options);
    if (s.size() < t.size()) swap(s, t);
    vi dp(t.size() + 1);
    for (char x : s) {
        int diagonal = 0;
        for (size_t j = 1; j <= t.size(); ++j) {
            const int above = dp[j];
            if (x == t[j - 1]) dp[j] = diagonal + 1;
            else dp[j] = max(dp[j], dp[j - 1]);
            diagonal = above;
        }
    }
    return dp.back();
}

// @acf fn edit_distance — (string_view s, string_view t, DpOptions options={}) -> int — 2文字列の編集距離
/// @acf-doc @brief 挿入・削除・置換を各1回とする2文字列のLevenshtein距離を1次元DPで返す。時間 O(|s||t|)、追加メモリ O(min(|s|,|t|))
/// @acf-doc @code (string_view s, string_view t, DpOptions options={}) -> int
/// @acf-doc [function] edit_distance
inline int edit_distance(string_view s, string_view t, DpOptions options = {}) {
    if (options.method != DpMethod::Auto && options.method != DpMethod::Dense)
        throw invalid_argument("edit_distance: only Auto/Dense is supported");
    if (max(s.size(), t.size()) > static_cast<size_t>(numeric_limits<int>::max()))
        throw overflow_error("edit_distance: result may exceed int range");
    if (s.empty() || t.empty()) return static_cast<int>(max(s.size(), t.size()));
    ac_detail::dp::require_budget((i128(min(s.size(), t.size())) + 1) * sizeof(int),
                                 i128(s.size()) * t.size(), options);
    if (s.size() < t.size()) swap(s, t);
    vi dp(t.size() + 1);
    iota(dp.begin(), dp.end(), 0);
    for (size_t i = 1; i <= s.size(); ++i) {
        int diagonal = dp[0];
        dp[0] = static_cast<int>(i);
        for (size_t j = 1; j <= t.size(); ++j) {
            const int above = dp[j];
            if (s[i - 1] == t[j - 1]) {
                dp[j] = diagonal;
            } else {
                dp[j] = 1 + min({diagonal, above, dp[j - 1]});
            }
            diagonal = above;
        }
    }
    return dp.back();
}

// @acf fn Trie — () -> Trie — 文字列の挿入・完全一致・接頭辞個数を扱うTrie
/// @acf-doc @brief charのバイト列を扱い、空文字・重複挿入・1個削除・完全一致・接頭辞検索に対応。各操作はO(|s| log Σ)、Σは最大256の文字種数
/// @acf-doc [function] Trie
class Trie {
    struct Node {
        map<char, int> next;
        int pass = 0;
        int terminal = 0;
    };

    vector<Node> nodes_{1};
    int words_ = 0;

    /// @brief 文字列が対応する状態を探し、なければ `-1` を返す。
    int node_for(string_view s) const {
        int v = 0;
        for (char c : s) {
            auto it = nodes_[v].next.find(c);
            if (it == nodes_[v].next.end()) return -1;
            v = it->second;
        }
        return v;
    }

public:
    /// @brief 空のTrieを構築する。
    Trie() = default;

    /// @brief 登録語をすべて削除して空のTrieに戻す。
    void clear() {
        nodes_.assign(1, Node{});
        words_ = 0;
    }

    /// @brief 文字列 `s` を1個登録する。
    void insert(string_view s) {
        int v = 0;
        ++nodes_[v].pass;
        for (char c : s) {
            auto [it, inserted] = nodes_[v].next.emplace(c, sz(nodes_));
            int next = it->second;
            if (inserted) nodes_.emplace_back();
            v = next;
            ++nodes_[v].pass;
        }
        ++nodes_[v].terminal;
        ++words_;
    }

    /// @brief 文字列 `s` を1個削除し、存在しなければ `false` を返す。
    bool erase(string_view s) {
        int v = node_for(s);
        if (v < 0 || nodes_[v].terminal == 0) return false;
        v = 0;
        --nodes_[v].pass;
        for (char c : s) {
            v = nodes_[v].next.at(c);
            --nodes_[v].pass;
        }
        --nodes_[v].terminal;
        --words_;
        return true;
    }

    /// @brief 文字列 `s` が1個以上登録されているか判定する。
    bool contains(string_view s) const {
        int v = node_for(s);
        return v >= 0 && nodes_[v].terminal > 0;
    }

    /// @brief 文字列 `s` の登録個数を返す。
    int count(string_view s) const {
        int v = node_for(s);
        return v < 0 ? 0 : nodes_[v].terminal;
    }

    /// @brief `prefix` で始まる登録語の個数を返す。
    int prefix_count(string_view prefix) const {
        int v = node_for(prefix);
        return v < 0 ? 0 : nodes_[v].pass;
    }

    /// @brief prefixで始まる登録語があるか判定する。空prefixは登録語があればtrue。
    bool starts_with(string_view prefix) const { return prefix_count(prefix) > 0; }

    /// @brief 登録語がないか判定する。
    bool empty() const { return words_ == 0; }

    /// @brief 重複と空文字を含む登録語の総数を返す。
    int size() const { return words_; }
};

// @acf fn AhoCorasick — (string alphabet="abcdefghijklmnopqrstuvwxyz") -> AhoCorasick — 複数パターンの失敗リンク付き文字列オートマトン
/// @acf-doc @brief 辞書語を登録し、失敗リンク・出力リンクを持つ遷移を O(Σ|pattern|·|alphabet|) で構築する。照合は O(|text|+出力数)
/// @acf-doc @code (string alphabet="abcdefghijklmnopqrstuvwxyz") -> AhoCorasick
/// @acf-doc [function] AhoCorasick
class AhoCorasick {
    struct Node {
        vector<int> next;
        vi terminal_ids;
        int failure = 0;
        int output_link = 0;
        int output_count = 0;

        /// @brief 指定したアルファベット数の空状態を構築する。
        explicit Node(int alphabet_size) : next(alphabet_size, -1) {}
    };

    int alphabet_size_;
    array<int, 256> code_{};
    vector<Node> nodes_;
    vector<int> pattern_nodes_;
    bool built_ = false;

    /// @brief 構築済みであることを確認する内部処理。
    void require_built() const { assert(built_); }

    /// @brief 文字をアルファベットの添字へ変換する内部処理。
    int code(char c) const {
        return code_[static_cast<unsigned char>(c)];
    }

    /// @brief 状態番号が有効であることを確認する内部処理。
    int checked_state(int state) const {
        assert(0 <= state && state < static_cast<int>(nodes_.size()));
        return state;
    }

public:
    /// @brief 指定したアルファベットで空のオートマトンを構築する。
    explicit AhoCorasick(string alphabet = "abcdefghijklmnopqrstuvwxyz")
        : alphabet_size_(static_cast<int>(alphabet.size())) {
        if (alphabet.empty())
            throw invalid_argument("AhoCorasick: alphabet must not be empty");
        code_.fill(-1);
        for (int i = 0; i < alphabet_size_; ++i) {
            const unsigned char c = static_cast<unsigned char>(alphabet[i]);
            if (code_[c] != -1)
                throw invalid_argument("AhoCorasick: alphabet has duplicate characters");
            code_[c] = i;
        }
        nodes_.emplace_back(alphabet_size_);
    }

    /// @brief パターンを登録し、登録順のパターンIDを返す。
    int add(string_view pattern) {
        if (built_)
            throw invalid_argument("AhoCorasick: add must precede build");
        if (pattern.empty())
            throw invalid_argument("AhoCorasick: empty patterns are not supported");
        for (char c : pattern)
            if (code(c) < 0)
                throw invalid_argument("AhoCorasick: pattern has a character outside alphabet");

        int state = 0;
        for (char c : pattern) {
            const int index = code(c);
            if (nodes_[state].next[index] == -1) {
                nodes_[state].next[index] = static_cast<int>(nodes_.size());
                nodes_.emplace_back(alphabet_size_);
            }
            state = nodes_[state].next[index];
        }
        const int pattern_id = static_cast<int>(pattern_nodes_.size());
        pattern_nodes_.push_back(state);
        nodes_[state].terminal_ids.push_back(pattern_id);
        return pattern_id;
    }

    /// @brief BFS順に失敗リンクと未接続文字の遷移を構築する。
    void build() {
        if (built_) return;
        queue<int> pending;
        for (int c = 0; c < alphabet_size_; ++c) {
            int child = nodes_[0].next[c];
            if (child == -1) {
                nodes_[0].next[c] = 0;
            } else {
                nodes_[child].failure = 0;
                pending.push(child);
            }
        }
        while (!pending.empty()) {
            const int state = pending.front();
            pending.pop();
            const int failure = nodes_[state].failure;
            nodes_[state].output_link =
                !nodes_[failure].terminal_ids.empty()
                    ? failure
                    : nodes_[failure].output_link;
            nodes_[state].output_count =
                static_cast<int>(nodes_[state].terminal_ids.size()) +
                nodes_[failure].output_count;
            for (int c = 0; c < alphabet_size_; ++c) {
                int child = nodes_[state].next[c];
                if (child == -1) {
                    nodes_[state].next[c] = nodes_[failure].next[c];
                } else {
                    nodes_[child].failure = nodes_[failure].next[c];
                    pending.push(child);
                }
            }
        }
        built_ = true;
    }

    /// @brief オートマトンの状態数を返す。
    int size() const { return static_cast<int>(nodes_.size()); }

    /// @brief 登録済みパターン数を返す。
    int pattern_count() const {
        return static_cast<int>(pattern_nodes_.size());
    }

    /// @brief 状態 `state` から文字 `c` を読んだ遷移先を返す。
    int step(int state, char c) const {
        require_built();
        checked_state(state);
        const int index = code(c);
        if (index < 0)
            throw invalid_argument("AhoCorasick: text has a character outside alphabet");
        return nodes_[state].next[index];
    }

    /// @brief 状態 `state` の失敗リンク先を返す。
    int failure_link(int state) const {
        require_built();
        return nodes_[checked_state(state)].failure;
    }

    /// @brief failure chain上で最も近い出力状態を返す。
    int output_link(int state) const {
        require_built();
        return nodes_[checked_state(state)].output_link;
    }

    /// @brief 状態 `state` で終わる登録パターンIDの一覧を返す。
    const vi& terminal_ids(int state) const {
        require_built();
        return nodes_[checked_state(state)].terminal_ids;
    }

    /// @brief 状態 `state` を末尾とするパターン総数を返す。
    int output_count(int state) const {
        require_built();
        return nodes_[checked_state(state)].output_count;
    }

    /// @brief `text` 中の各登録パターンの出現回数を返す。
    vi match_counts(string_view text) const {
        require_built();
        vi result(pattern_count(), 0);
        int state = 0;
        for (char c : text) {
            state = step(state, c);
            for (int output = state; output != 0;
                 output = nodes_[output].output_link) {
                for (int pattern_id : nodes_[output].terminal_ids)
                    ++result[pattern_id];
            }
        }
        return result;
    }
};

// @acf fn RollingHash — (string_view[, ull base]) -> RollingHash — 文字列の半開区間を61bitハッシュで比較
/// @acf-doc @brief 文字列を前処理 O(N)、半開区間ハッシュを O(1) で取得する。ハッシュ一致は確率的な判定
/// @acf-doc @code (string_view[, ull base]) -> RollingHash
/// @acf-doc [function] RollingHash
class RollingHash {
    static constexpr ull modulus_ = (1ULL << 61) - 1;

    ull base_;
    vector<ull> hash_;
    vector<ull> powers_;

    /// @brief 61bit法による剰余加算を行う内部処理。
    static ull add_mod(ull left, ull right) {
        ull result = left + right;
        if (result >= modulus_) result -= modulus_;
        return result;
    }

    /// @brief 61bit法による剰余減算を行う内部処理。
    static ull sub_mod(ull left, ull right) {
        return left >= right ? left - right : modulus_ - (right - left);
    }

    /// @brief 61bit法による剰余乗算を行う内部処理。
    static ull mul_mod(ull left, ull right) {
        const u128 product = static_cast<u128>(left) * right;
        ull result = static_cast<ull>(product >> 61) +
                     static_cast<ull>(product & modulus_);
        result = (result >> 61) + (result & modulus_);
        if (result >= modulus_) result -= modulus_;
        return result;
    }

public:
    static constexpr ull modulus = modulus_;
    static constexpr ull default_base = 1'000'000'007ULL;

    /// @brief `value` を前処理し、区間ハッシュを取得できるようにする。
    explicit RollingHash(string_view value, ull base = default_base)
        : base_(base), hash_(value.size() + 1), powers_(value.size() + 1, 1) {
        assert(2 <= base_ && base_ < modulus_);
        for (int i = 0; i < sz(value); ++i) {
            const ull code = static_cast<unsigned char>(value[i]) + 1;
            hash_[i + 1] = add_mod(mul_mod(hash_[i], base_), code);
            powers_[i + 1] = mul_mod(powers_[i], base_);
        }
    }

    /// @brief 前処理した文字列の長さを返す。
    int size() const { return sz(hash_) - 1; }

    /// @brief 半開区間 `[left,right)` のハッシュ値を返す。
    ull get(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return sub_mod(hash_[right], mul_mod(hash_[left], powers_[right - left]));
    }

    /// @brief 文字列全体のハッシュ値を返す。
    ull whole() const { return get(0, size()); }

    /// @brief 2つの半開区間が同じ文字列かハッシュで比較する。
    bool equal(int left, int right, const RollingHash& other,
               int other_left, int other_right) const {
        assert(0 <= left && left <= right && right <= size());
        assert(0 <= other_left && other_left <= other_right &&
               other_right <= other.size());
        return right - left == other_right - other_left &&
               base_ == other.base_ && get(left, right) == other.get(other_left, other_right);
    }
};
