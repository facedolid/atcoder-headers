#pragma once
// Library: https://github.com/facedolid/atcoder-headers

// Opt-in advanced data structures. No dependency on other ac headers.
// Algorithmic choices informed by qandori/cp-library (CC0).
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

// @acf fn DisjointSparseTable — (vector<T>, identity[, op]) -> DisjointSparseTable<T,Op> — 更新なしの一般区間集約
/// 結合的な演算で、更新のない配列の半開区間[l,r)を集約します。
/// 引数valuesは初期配列、identityは両側単位元、opは順序を保つ二項演算です。
/// prod(l,r)は区間の集約値を返し、空区間ではidentityを返します。queryはprodと同じです。
/// build(values)で配列を再構築できます。元配列は変更しません。点更新はできません。
/// 構築時間・空間O(N log N)、問い合わせO(1)回の演算。演算・要素コピーの費用は別です。
/// 0<=l<=r<=size()が必要です。結合則・単位元の条件は自動検証しません。
template <class T, class Op = std::plus<T>>
class DisjointSparseTable {
    int n_ = 0;
    T identity_;
    Op op_;
    std::vector<T> values_;
    std::vector<std::vector<T>> table_;

public:
    DisjointSparseTable()
        requires (std::is_default_constructible_v<T> && std::is_default_constructible_v<Op>)
        : identity_{}, op_{} {}

    explicit DisjointSparseTable(const std::vector<T>& values, T identity = T{}, Op op = Op{})
        : identity_(std::move(identity)), op_(std::move(op)) { build(values); }

    void build(const std::vector<T>& values) {
        if (values.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
            throw std::length_error("DisjointSparseTable: size exceeds int");
        const std::size_t n = values.size();
        std::vector<T> next_values(values);
        std::vector<std::vector<T>> next_table;
        if (n > 1) next_table.reserve(std::bit_width(static_cast<unsigned>(n - 1)));
        for (std::size_t half = 1; half < n; half <<= 1) {
            next_table.push_back(values);
            auto& row = next_table.back();
            const std::size_t block = half << 1;
            for (std::size_t start = 0; start < n; start += block) {
                const std::size_t mid = std::min(start + half, n);
                const std::size_t end = std::min(start + block, n);
                if (mid == end) continue;
                for (std::size_t i = mid - 1; i > start; --i)
                    row[i - 1] = op_(values[i - 1], row[i]);
                for (std::size_t i = mid + 1; i < end; ++i)
                    row[i] = op_(row[i - 1], values[i]);
            }
        }
        values_.swap(next_values);
        table_.swap(next_table);
        n_ = static_cast<int>(n);
    }

    int size() const { return n_; }
    bool empty() const { return n_ == 0; }
    T get(int i) const {
        assert(0 <= i && i < n_);
        return values_[i];
    }
    T prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= n_);
        if (l == r) return identity_;
        if (r - l == 1) return values_[l];
        const unsigned level = std::bit_width(static_cast<unsigned>(l ^ (r - 1))) - 1;
        return op_(table_[level][l], table_[level][r - 1]);
    }
    T query(int l, int r) const { return prod(l, r); }
    T all_prod() const { return prod(0, n_); }
};

// @acf fn SlidingWindowAggregation — (identity[, op]) -> SlidingWindowAggregation<T,Op> — キュー全体のモノイド集約
/// 末尾へ追加し先頭から削除する列について、順番どおりの集約値を保持します。
/// push_back(x)、pop_front()は償却O(1)、prod()・front()・back()はO(1)回の操作です。
/// opは結合則を満たし、identityは両側単位元である必要があります。非可換演算にも対応します。
/// 空のprod()はidentityです。空のfront/back/pop_frontは呼び出せません。
/// Tのコピー・移動・演算コストは別です。push_backなどは列を変更します。
template <class T, class Op = std::plus<T>>
class SlidingWindowAggregation {
    struct Entry {
        T value;
        T product;
    };
    T identity_;
    Op op_;
    std::vector<Entry> front_, back_;

    void transfer() {
        while (!back_.empty()) {
            T value = std::move(back_.back().value);
            back_.pop_back();
            T product = front_.empty() ? value : op_(value, front_.back().product);
            front_.push_back({std::move(value), std::move(product)});
        }
    }

public:
    SlidingWindowAggregation()
        requires (std::is_default_constructible_v<T> && std::is_default_constructible_v<Op>)
        : identity_{}, op_{} {}

    explicit SlidingWindowAggregation(T identity, Op op = Op{})
        : identity_(std::move(identity)), op_(std::move(op)) {}

    explicit SlidingWindowAggregation(const std::vector<T>& values, T identity = T{}, Op op = Op{})
        : identity_(std::move(identity)), op_(std::move(op)) {
        back_.reserve(values.size());
        for (const T& value : values) push_back(value);
    }

    std::size_t size() const { return front_.size() + back_.size(); }
    bool empty() const { return front_.empty() && back_.empty(); }
    void clear() { front_.clear(); back_.clear(); }
    void reserve(std::size_t n) { front_.reserve(n); back_.reserve(n); }

    void push_back(T value) {
        T product = back_.empty() ? value : op_(back_.back().product, value);
        back_.push_back({std::move(value), std::move(product)});
    }
    void pop_front() {
        assert(!empty());
        if (front_.empty()) transfer();
        front_.pop_back();
    }
    const T& front() const {
        assert(!empty());
        return front_.empty() ? back_.front().value : front_.back().value;
    }
    const T& back() const {
        assert(!empty());
        return back_.empty() ? front_.front().value : back_.back().value;
    }
    T prod() const {
        if (front_.empty() && back_.empty()) return identity_;
        if (front_.empty()) return back_.back().product;
        if (back_.empty()) return front_.back().product;
        return op_(front_.back().product, back_.back().product);
    }
};

// @acf fn IntervalSet — () -> IntervalSet<T> — 半開区間の集合・mex・次/前の整数
/// 整数集合を交わらない半開区間[l,r)として管理します。隣接区間は自動で結合します。
/// insert(l,r)、erase(l,r)は実際に増減した整数の個数(uint64_t)を返します。
/// contains(x)・mex(x)・next(x)・prev(x)は区間数Kに対してO(log K)です。
/// 座標Tはbool以外の64bit以下の整数型です。区間はl<=r、空区間は変更なしです。
/// Tの最大値は右端や検索位置には使えますが、集合の要素としては格納できません。
/// insert/eraseはO(log K + M log K)、Mは操作で触れる区間数です。
/// sizeは登録した整数の個数をuint64_tで返します。next/prevの不在はnulloptです。
template <class T = long long>
class IntervalSet {
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool> && sizeof(T) <= 8,
                  "IntervalSet<T> requires an integer type up to 64 bits");
    std::map<T, T> intervals_;
    std::uint64_t count_ = 0;

    static std::uint64_t length(T l, T r) {
        using U = std::make_unsigned_t<T>;
        return static_cast<std::uint64_t>(static_cast<U>(static_cast<U>(r) - static_cast<U>(l)));
    }

public:
    using const_iterator = typename std::map<T, T>::const_iterator;

    bool empty() const { return intervals_.empty(); }
    std::uint64_t size() const { return count_; }
    std::size_t interval_count() const { return intervals_.size(); }
    const_iterator begin() const { return intervals_.begin(); }
    const_iterator end() const { return intervals_.end(); }
    void clear() { intervals_.clear(); count_ = 0; }

    std::uint64_t insert(T l, T r) {
        if (r < l) throw std::invalid_argument("IntervalSet: l must not exceed r");
        if (l == r) return 0;
        auto it = intervals_.lower_bound(l);
        if (it != intervals_.begin() && std::prev(it)->second >= l) --it;
        T left = l, right = r;
        std::uint64_t covered = 0;
        while (it != intervals_.end() && it->first <= right) {
            left = std::min(left, it->first);
            right = std::max(right, it->second);
            covered += length(it->first, it->second);
            it = intervals_.erase(it);
        }
        const auto added = length(left, right) - covered;
        intervals_.emplace_hint(it, left, right);
        count_ += added;
        return added;
    }

    std::uint64_t erase(T l, T r) {
        if (r < l) throw std::invalid_argument("IntervalSet: l must not exceed r");
        if (l == r) return 0;
        auto it = intervals_.upper_bound(l);
        if (it != intervals_.begin()) --it;
        std::uint64_t removed = 0;
        while (it != intervals_.end() && it->first < r) {
            const T a = it->first, b = it->second;
            if (b <= l) { ++it; continue; }
            removed += length(std::max(a, l), std::min(b, r));
            if (a < l && r < b) {
                it->second = l;
                intervals_.emplace(r, b);
                break;
            }
            if (a < l) {
                it->second = l;
                ++it;
                continue;
            }
            if (r < b) {
                it = intervals_.erase(it);
                intervals_.emplace_hint(it, r, b);
                break;
            }
            it = intervals_.erase(it);
        }
        count_ -= removed;
        return removed;
    }

    bool contains(T x) const {
        auto it = intervals_.upper_bound(x);
        return it != intervals_.begin() && x < std::prev(it)->second;
    }
    bool contains(T l, T r) const {
        if (r < l) throw std::invalid_argument("IntervalSet: l must not exceed r");
        if (l == r) return true;
        auto it = intervals_.upper_bound(l);
        return it != intervals_.begin() && std::prev(it)->second >= r;
    }
    bool intersects(T l, T r) const {
        if (r < l) throw std::invalid_argument("IntervalSet: l must not exceed r");
        if (l == r) return false;
        auto it = intervals_.lower_bound(l);
        if (it != intervals_.begin() && std::prev(it)->second > l) return true;
        return it != intervals_.end() && it->first < r;
    }
    T mex(T x = T{}) const {
        auto it = intervals_.upper_bound(x);
        return it != intervals_.begin() && x < std::prev(it)->second ? std::prev(it)->second : x;
    }
    std::optional<T> next(T x) const {
        auto it = intervals_.upper_bound(x);
        if (it != intervals_.begin() && x < std::prev(it)->second) return x;
        it = intervals_.lower_bound(x);
        if (it == intervals_.end()) return std::nullopt;
        return it->first;
    }
    std::optional<T> prev(T x) const {
        auto it = intervals_.upper_bound(x);
        if (it == intervals_.begin()) return std::nullopt;
        --it;
        return x < it->second ? x : static_cast<T>(it->second - 1);
    }
};
