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

namespace kuma_rt {
static_assert(std::numeric_limits<int>::digits == 31, "Kuma needs 32-bit int");
static_assert(std::numeric_limits<long long>::digits == 63, "Kuma needs 64-bit long long");

[[noreturn]] inline void error(const std::string& message) {
    throw std::runtime_error(message);
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
template<class T> struct is_tuple : std::false_type {};
template<class... Ts> struct is_tuple<std::tuple<Ts...>> : std::true_type {};
template<class A, class B> struct is_tuple<std::pair<A, B>> : std::true_type {};
template<class T> struct is_associative : std::false_type {};
template<class... Ts> struct is_associative<std::set<Ts...>> : std::true_type {};
template<class... Ts> struct is_associative<std::map<Ts...>> : std::true_type {};

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
    if constexpr (is_vector<U>::value || is_associative<U>::value || is_kuma_view<U>::value) {
        using E = typename U::value_type;
        const auto& separator = (is_vector<E>::value || is_tuple<E>::value || requires(const E& v) { kuma_fields(v); })
            ? options.row_sep : options.sep;
        bool first = true;
        for (const auto& item : value) {
            if (!first && !std::is_same_v<E, char>) output << separator;
            first = false;
            write_value_to(item, options, output);
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
        output << std::setprecision(std::numeric_limits<U>::max_digits10) << value;
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
        long long value;
        long long operator*() const { return value; }
        Iterator& operator++() { ++value; return *this; }
        bool operator!=(const Iterator& other) const { return value != other.value; }
    };
    long long first, last;
    Iterator begin() const { return {first}; }
    Iterator end() const { return {last}; }
};

inline Range range(long long begin, long long end) {
    return {begin < end ? begin : end, end};
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
    auto it = values.begin();
    const auto end = values.end();
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
    auto iterators = std::tuple{values.begin()...};
    auto ends = std::tuple{values.end()...};
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
C cpp_slice(CppSliceRequest<C> request) {
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
using cpp_item = std::remove_cvref_t<decltype(*std::begin(std::declval<const R&>()))>;

template<class R, class F, class P>
auto cpp_collect(const R& values, F transform, P predicate) {
    using Out = std::decay_t<decltype(transform(*std::begin(values)))>;
    std::vector<Out> result;
    for (const auto& item : values)
        if (predicate(item)) result.push_back(transform(item));
    return result;
}
template<class R, class F, class P>
auto cpp_flat_collect(const R& values, F transform, P predicate) {
    using Inner = std::decay_t<decltype(transform(*std::begin(values)))>;
    using Out = typename Inner::value_type;
    std::vector<Out> result;
    for (const auto& item : values) if (predicate(item)) {
        auto inner = transform(item);
        result.insert(result.end(), std::make_move_iterator(inner.begin()), std::make_move_iterator(inner.end()));
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
