#pragma once
// Library: https://github.com/facedolid/atcoder-headers
// Kuma generated-code support for GNU C++23.
// Keep this header in sync with the code emitted by tools/kuma/compiler.py.
#ifndef AC_NO_PRINT_MACROS
#define AC_NO_PRINT_MACROS
#endif
#include "ac.hpp"
#include "ac_io.hpp"
#include <optional>
#include <span>
#include <string_view>
#if __has_include(<ext/pb_ds/assoc_container.hpp>) && __has_include(<ext/pb_ds/tree_policy.hpp>)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define KUMA_HAS_PBDS 1
#endif

namespace kuma_rt {
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

static_assert(std::numeric_limits<int>::digits == 31, "Kuma needs 32-bit int");
static_assert(std::numeric_limits<long long>::digits == 63, "Kuma needs 64-bit long long");

[[noreturn]] inline void error(const std::string& message) {
    throw std::runtime_error(message);
}

// The fallback keeps portability where GNU policy-based data structures are unavailable.
#ifndef KUMA_HAS_PBDS
template<class Key, class T> class SortedListFallbackTree {
    std::map<Key, T> values_;
public:
    using const_iterator = typename std::map<Key, T>::const_iterator;
    auto size() const { return values_.size(); }
    bool empty() const { return values_.empty(); }
    auto begin() const { return values_.begin(); }
    auto end() const { return values_.end(); }
    auto lower_bound(const Key& key) const { return values_.lower_bound(key); }
    void insert(const std::pair<Key, T>& item) { values_.insert(item); }
    void erase(const_iterator it) { values_.erase(it); }
    std::size_t order_of_key(const Key& key) const { return std::distance(values_.begin(), values_.lower_bound(key)); }
    const_iterator find_by_order(std::size_t i) const { auto it = values_.begin(); std::advance(it, i); return it; }
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
    template<class U> void add(U value) {
        T x = convert(value);
        if (next_ == std::numeric_limits<unsigned long long>::max()) error("SortedList insertion counter exhausted");
        K key = checked_ordered_value(key_(x));
        values_.insert({{std::move(key), next_++}, x});
    }
    template<class U> long long bisect_left(U x) const { return static_cast<long long>(values_.order_of_key({checked_ordered_value(key_(convert(x))), 0})); }
    template<class U> long long bisect_right(U x) const { return static_cast<long long>(values_.order_of_key({checked_ordered_value(key_(convert(x))), std::numeric_limits<unsigned long long>::max()})); }
    template<class U> T operator[](U i) const { return values_.find_by_order(index(integer(i)))->second; }
    template<class U> void remove(U value) {
        T x = convert(value);
        K key = checked_ordered_value(key_(x));
        auto it = values_.lower_bound({key, 0});
        for (; it != values_.end() && !(key < it->first.first) && !(it->first.first < key); ++it) {
            if (it->second == x) { values_.erase(it); return; }
        }
        error("SortedList value not found");
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

inline int dimension(long long n) {
    if (n < 0 || n > std::numeric_limits<int>::max()) {
        error("input dimension must be between 0 and INT_MAX");
    }
    return static_cast<int>(n);
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

template<class Item, class C> struct EnumerateView;
template<class Item, class... Cs> struct ZipView;
template<class T> struct is_kuma_view : std::false_type {};
template<class I, class C> struct is_kuma_view<EnumerateView<I, C>> : std::true_type {};
template<class I, class... Cs> struct is_kuma_view<ZipView<I, Cs...>> : std::true_type {};
template<class T> struct is_vector : std::false_type {};
template<class T, class A> struct is_vector<std::vector<T, A>> : std::true_type {};
template<class T> struct is_deque : std::false_type {};
template<class T, class A> struct is_deque<std::deque<T, A>> : std::true_type {};
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

template<class T>
T read() {
    if constexpr (requires(T& v) { kuma_fields(v); }) {
        T value{};
        std::apply([&](auto&... fields) {
            ((fields = read<std::remove_cvref_t<decltype(fields)>>()), ...);
        }, kuma_fields(value));
        return value;
    } else if constexpr (is_tuple<T>::value) {
        T value{};
        std::apply([&](auto&... fields) {
            ((fields = read<std::remove_cvref_t<decltype(fields)>>()), ...);
        }, value);
        return value;
    } else {
        T value = ::rd<T>();
        check_input();
        return value;
    }
}

template<class T>
std::vector<T> read_vec(long long n) {
    const int size = dimension(n);
    if (static_cast<std::size_t>(size) > std::vector<T>{}.max_size()) {
        error("input vector exceeds max_size");
    }
    if (size == 0) return {};
    std::vector<T> values;
    values.reserve(size);
    for (int i = 0; i < size; ++i) values.push_back(read<T>());
    return values;
}

template<class T>
std::vector<std::vector<T>> read_mat(long long h, long long w) {
    const int height = dimension(h), width = dimension(w);
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
    for (int i = 0; i < height; ++i) values.push_back(read_vec<T>(width));
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

template<class T> requires std::is_same_v<T, std::string>
std::string read_line_as() {
    return read_line();
}

inline void discard_line() {
    check_input();
    if (!std::cin.eof()) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        check_input();
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

template<class T>
void write_value(const T& value, const PrintOptions& options) {
    write_value_to(value, options, std::cout);
}

template<class... Ts>
void print(const PrintOptions& options, const Ts&... values) {
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

template<class... Ts>
void debug_print(const PrintOptions& options, const Ts&... values) {
    bool first = true;
    auto emit = [&](const auto& value) {
        if (!first) std::cerr << options.sep;
        first = false;
        write_value_to(value, options, std::cerr);
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
    return std::pair{std::begin(values) + start, std::begin(values) + stop};
}

template<class R, class F = std::less<>>
void sort_range(R& values, long long start, std::optional<long long> end, F compare = {}) {
    auto [first, last] = algorithm_bounds(values, start, end);
    std::sort(first, last, std::move(compare));
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
        long long value, stop, step;
        bool done;
        long long operator*() const { return value; }
        Iterator& operator++() {
            const __int128 next = static_cast<__int128>(value) + step;
            done = step > 0 ? next >= stop : next <= stop;
            if (!done) value = static_cast<long long>(next);
            return *this;
        }
        bool operator!=(const Iterator&) const { return !done; }
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
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>,
                  "Kuma range arguments must be integers (not bool)");
    const __int128 wide = value;
    if (wide < std::numeric_limits<long long>::min() || wide > std::numeric_limits<long long>::max())
        error("range argument exceeds signed 64-bit range");
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
decltype(auto) checked_index(C&& container, long long index) {
    const __int128 length = static_cast<__int128>(container.size());
    __int128 position = index;
    if (position < 0) position += length;
    if (position < 0 || position >= length) error("index out of range");
    return std::forward<C>(container)[static_cast<std::size_t>(position)];
}
// Eager adapters preserve left-to-right snapshot semantics in the compiler.
// Explicit element types materialize vector<bool> proxies and map const keys.
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
        decltype(std::declval<const C&>().begin()) it;
        decltype(std::declval<const C&>().end()) end;
        long long index;
        auto operator*() const { return std::tuple<long long, decltype(*it)>{index, *it}; }
        Iterator& operator++() {
            ++it;
            if (it != end) {
                if (index == std::numeric_limits<long long>::max()) error("enumerate index overflows i64");
                ++index;
            }
            return *this;
        }
        bool operator!=(Sentinel) const { return it != end; }
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
        bool operator!=(Sentinel) const {
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                const bool all = ((std::get<I>(iterators) != std::get<I>(ends)) && ...);
                if (!all && strict && ((std::get<I>(iterators) != std::get<I>(ends)) || ...))
                    error("zip strict: input lengths differ");
                return all;
            }(std::index_sequence_for<Cs...>{});
        }
    };
    Iterator begin() const {
        return std::apply([&](const auto&... inputs) {
            return Iterator{strict, std::tuple{inputs.begin()...}, std::tuple{inputs.end()...}};
        }, values);
    }
    Sentinel end() const { return {}; }
};

// Exact metadata checks for eager adapters before an allocation-free reduction.
// Wide arithmetic covers the entire signed-i64 range, including LL_MIN starts.
template<class C> __int128 sequence_length(const C& values) { return values.size(); }
inline __int128 sequence_length(const Range& values) {
    return static_cast<__int128>(values.last) - values.first;
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
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>,
                  "Kuma slice bounds must be integers (not bool)");
    const __int128 wide = value;
    if (wide < std::numeric_limits<long long>::min() ||
        wide > std::numeric_limits<long long>::max())
        error("slice bound exceeds signed 64-bit range");
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

#define KUMA_CPP_INTEROP_ABI 1

// M6 helpers operate on ordinary C++ values. The native compiler resolves types.
// They do not claim M1-M5 borrow checking for arbitrary user-defined C++ code.
namespace kuma_rt {
template<class R>
using cpp_item = std::conditional_t<std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>,
    bool, std::remove_cvref_t<decltype(*std::begin(std::declval<const R&>()))>>;

template<class R, class F, class P>
auto cpp_collect(const R& values, F transform, P predicate) {
    using Out = std::decay_t<decltype(transform(std::declval<const cpp_item<R>&>()))>;
    std::vector<Out> result;
    for (const auto& raw : values) {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>) {
            const bool item = raw;
            if (predicate(item)) result.push_back(transform(item));
        } else if (predicate(raw)) result.push_back(transform(raw));
    }
    return result;
}
template<class R, class F, class P>
auto cpp_flat_collect(const R& values, F transform, P predicate) {
    using Inner = std::decay_t<decltype(transform(std::declval<const cpp_item<R>&>()))>;
    using Out = typename Inner::value_type;
    std::vector<Out> result;
    for (const auto& raw : values) {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>) {
            const bool item = raw;
            if (!predicate(item)) continue;
            auto inner = transform(item);
            result.insert(result.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
        } else if (predicate(raw)) {
            auto inner = transform(raw);
            result.insert(result.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
        }
    }
    return result;
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
template<class R>
void cpp_reverse(R& values) { std::reverse(std::begin(values), std::end(values)); }
template<class R>
auto cpp_reversed(R values) { cpp_reverse(values); return values; }
template<class R>
auto cpp_enumerate(const R& values, long long start) { return enumerate<cpp_item<R>>(values, start); }
template<class R, class I> requires std::is_integral_v<std::remove_cvref_t<I>>
auto cpp_enumerate(const R& values, I&& start) {
    if constexpr (std::is_unsigned_v<std::remove_cvref_t<I>>) {
        const unsigned __int128 wide = start;
        if (wide > static_cast<unsigned __int128>(std::numeric_limits<long long>::max()))
            error("enumerate start exceeds signed 64-bit range");
        return enumerate<cpp_item<R>>(values, static_cast<long long>(wide));
    } else {
        const __int128 wide = start;
        if (wide < std::numeric_limits<long long>::min() || wide > std::numeric_limits<long long>::max())
            error("enumerate start exceeds signed 64-bit range");
        return enumerate<cpp_item<R>>(values, static_cast<long long>(wide));
    }
}
template<class... Rs>
auto cpp_zip(bool strict, const Rs&... values) { return zip<std::tuple<cpp_item<Rs>...>>(strict, values...); }
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
template<class R, class... As>
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
} // namespace kuma_rt
