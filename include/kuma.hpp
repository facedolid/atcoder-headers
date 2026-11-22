#pragma once
// Library: https://github.com/facedolid/atcoder-headers
// Kuma generated-code support for GNU C++23.
// Keep this header in sync with the code emitted by tools/kuma/compiler.py.
#ifndef AC_NO_PRINT_MACROS
#define AC_NO_PRINT_MACROS
#endif
#include "ac.hpp"
#include "ac_io.hpp"
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <utility>
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

template<class T>
std::set<T> read_set(int size) {
    std::set<T> result;
    for (int i = 0; i < size; ++i)
        ::ac_detail::collection::insert_checked(result, read<T>());
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

// Holds only the input length and choice count. Dereferencing yields an
// independent index row, so saved rows survive advancing the iterator.
class CombinationIndicesView {
    long long n_;
    long long k_;
public:
    using value_type = std::vector<long long>;
    using Sentinel = std::default_sentinel_t;
    CombinationIndicesView(std::size_t n, long long k) : n_(0), k_(k) {
        if (k < 0) throw std::invalid_argument("combination_indices: negative k");
        if (!std::in_range<long long>(n))
            throw std::length_error("combination_indices: vector length exceeds i64");
        n_ = static_cast<long long>(n);
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
        if (k_ > n_) return {n_, {}, true};
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
    explicit CombinationsView(const std::vector<T>& values, long long k)
        : indices_(values.size(), k),
          values_(k > 0 && std::cmp_less_equal(k, values.size()) ? values : value_type{}) {}
    explicit CombinationsView(std::vector<T>&& values, long long k)
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
decltype(auto) checked_index(C&& container, long long index) {
    const __int128 length = static_cast<__int128>(container.size());
    __int128 position = index;
    if (position < 0) position += length;
    if (position < 0 || position >= length) error("index out of range");
    return std::forward<C>(container)[static_cast<std::size_t>(position)];
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

template<class C, class K>
    requires ((is_vector<C>::value || std::same_as<C, std::string>)
              && (std::same_as<K, int> || std::same_as<K, long long>))
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

// One immutable snapshot for all windows. A row owns its share independently
// of the outer view; iterators borrow their respective view or row.
template<class C>
class WindowsView {
    std::shared_ptr<const C> values_;
    long long width_ = 0;
    std::optional<std::vector<std::size_t>> selected_;

    WindowsView(std::shared_ptr<const C> values, long long width)
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
    };

    using value_type = Row;

    template<class R, class K>
        requires (std::same_as<std::remove_cvref_t<R>, C>
                  && (std::same_as<K, int> || std::same_as<K, long long>))
    explicit WindowsView(R&& values, K width) {
        if (width <= 0) error("windows: width must be positive");
        width_ = width;
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
        auto operator*() const { return std::tuple<long long, decltype(*it)>{index, *it}; }
        Iterator& operator++() {
            ++it;
            if (it != end) {
                if (index == std::numeric_limits<long long>::max()) error("enumerate index overflows i64");
                ++index;
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

#define KUMA_CPP_INTEROP_ABI 1

// M6 helpers operate on ordinary C++ values. The native compiler resolves types.
// They do not claim M1-M5 borrow checking for arbitrary user-defined C++ code.
namespace kuma_rt {
template<class R>
using cpp_item = std::conditional_t<std::is_same_v<std::remove_cvref_t<R>, std::vector<bool>>,
    bool, std::remove_cvref_t<decltype(*std::begin(std::declval<const R&>()))>>;

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
// Immediate opt-in pipeline: own a source snapshot, allocate only final output,
// and do not advance past the last accepted item.
template<class T>
concept IterOwnedScalar = std::same_as<T, int> || std::same_as<T, long long>
    || std::same_as<T, double> || std::same_as<T, bool>
    || std::same_as<T, char> || std::same_as<T, std::string>;

template<IterOwnedScalar T, class P, class F, class K>
    requires ((std::same_as<K, int> || std::same_as<K, long long>)
              && IterOwnedScalar<std::remove_cvref_t<std::invoke_result_t<F&, const T&>>>)
auto cpp_iter_filter_map_take_collect(std::vector<T> source, P&& pred, F&& map, K count) {
    using Result = std::remove_cvref_t<std::invoke_result_t<F&, const T&>>;
    std::vector<Result> collected;
    if (count < 0) throw std::invalid_argument("iter.take: negative count");
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
template<bool IntegerLiteral = false, class R, class F, class Acc>
decltype(auto) cpp_member_reduce(R&& values, F&& callback, Acc&& initial) {
    if constexpr (requires { std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial)); })
        return std::forward<R>(values).reduce(std::forward<F>(callback), std::forward<Acc>(initial));
    else if constexpr (is_stock_windows_view<std::remove_cvref_t<R>>) {
        if constexpr (IntegerLiteral)
            return values.reduce_rows(std::forward<F>(callback), static_cast<long long>(initial));
        else
            return values.reduce_rows(std::forward<F>(callback), std::forward<Acc>(initial));
    } else
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
template<class C>
struct ChunksReceiver {
    const C& values;

    template<class K> requires (std::same_as<K, int> || std::same_as<K, long long>)
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

template<std::size_t Arity, bool IntegerLiteral, class R>
struct WindowsReduceReceiver {
    R&& values;
    template<class F, class Acc>
    auto reduce(F&& callback, Acc&& initial) const {
        if constexpr (IntegerLiteral)
            return values.template reduce_fields<Arity>(std::forward<F>(callback), static_cast<long long>(initial));
        else
            return values.template reduce_fields<Arity>(std::forward<F>(callback), std::forward<Acc>(initial));
    }
};

template<std::size_t Arity, bool IntegerLiteral, class R>
constexpr decltype(auto) windows_reduce_receiver(R&& values) noexcept {
    if constexpr (is_stock_windows_view<std::remove_cvref_t<R>>)
        return WindowsReduceReceiver<Arity, IntegerLiteral, R>{std::forward<R>(values)};
    else
        return std::forward<R>(values);  // Native member receives the original callback.
}

template<class Receiver, std::size_t Arity = 0>
struct WindowsReceiver {
    Receiver&& values;

    template<class K> requires (std::same_as<K, int> || std::same_as<K, long long>)
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

template<std::size_t Arity, bool Filter, bool Member, class R, class F>
decltype(auto) cpp_map_filter_tuple_arrow(R&& values, F&& callback) {
    if constexpr (Member && is_stock_windows_view<std::remove_cvref_t<R>>)
        return values.template row_fields<Arity, Filter>(std::forward<F>(callback));
    else if constexpr (Member && Filter && requires { std::forward<R>(values).filter(std::forward<F>(callback)); })
        return std::forward<R>(values).filter(std::forward<F>(callback));
    else if constexpr (Member && !Filter && requires { std::forward<R>(values).map(std::forward<F>(callback)); })
        return std::forward<R>(values).map(std::forward<F>(callback));
    else if constexpr (is_plain_vector<std::remove_cvref_t<R>>) {
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
    bool empty() const { return storage_.empty(); }
    long long size() const {
        if (std::cmp_greater(storage_.size(), std::numeric_limits<long long>::max()))
            throw std::length_error(Stack ? "stack: size too large" : "queue: size too large");
        return static_cast<long long>(storage_.size());
    }
    void push(long long value) { storage_.push_back(value); }
    long long front() const requires (!Stack) { return peek(); }
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
} // namespace kuma_rt
#endif
