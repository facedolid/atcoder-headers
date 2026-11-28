#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include "ac.hpp"

// @acf fn Fenwick — (int n) -> Fenwick<T> — 0-indexの一点加算・半開区間和

/// 更新される配列の区間和を保持する型です。add(i,x) で一点に加算し、sum(l,r) で [l,r) の総和を求めます。頻度表や更新のある累積和に使います。
///
/// 引数 `n / values`: 初期要素数 n>=0（全要素 T{}）、または初期配列。T は加算・減算を扱える型。
///
/// 戻り値: 構築される`Fenwick<T>`。`sum(right)`は`[0,right)`の和、`sum(left,right)`は`[left,right)`の和を`T`型で返す。空区間は`T{}`。`get(i)`は現在値。`lower_bound_prefix(target)`は累積値が`target`以上になる最初の要素の添字で、該当なしは`size()`、`target<=T{}`は0。
///
/// 注意: add / set は内部の配列集計を更新します。添字は 0-indexed。add/get/set は 0<=i<size()、sum は 0<=left<=right<=size()。lower_bound_prefix には非負の要素を登録して使います。初期配列はコピーして集計するため、元配列の変更とは同期しません。n=INT_MAXは内部添字の上限を超えるため、配列確保前にlength_errorを送出します。
///
/// 呼び分け: `Fenwick<T>`() は要素0、`Fenwick<T>`(n) は零初期化、`Fenwick<T>`(values) は配列から構築。sum(right) と sum(left,right) はどちらも右端を含みません。
template <class T>
class Fenwick {
    int n_ = 0;
    vector<T> bit_;

public:
    /// 要素数0のFenwick木を作る。
    Fenwick() = default;

    /// 要素数 `n` のFenwick木を構築する。
    explicit Fenwick(int n) : n_(n) {
        assert(n >= 0);
        if (n == numeric_limits<int>::max())
            throw length_error("Fenwick: size is too large for int indexing");
        bit_.assign(static_cast<size_t>(n) + 1, T{});
    }
    /// 初期配列 `values` の各値を登録したFenwick木を構築する。
    explicit Fenwick(const vector<T>& values) : Fenwick(sz(values)) {
        for (int i = 0; i < n_; ++i) add(i, values[i]);
    }

    /// 要素数を返す。
    int size() const { return n_; }

    /// 添字 `index` に `value` を加算する。
    void add(int index, const T& value) {
        assert(0 <= index && index < n_);
        for (unsigned i = static_cast<unsigned>(index) + 1;
             i <= static_cast<unsigned>(n_); i += i & -i) bit_[i] += value;
    }

    /// 半開区間 `[0,right)` の総和を返す。
    T sum(int right) const {
        assert(0 <= right && right <= n_);
        T result{};
        for (int i = right; i > 0; i -= i & -i) result += bit_[i];
        return result;
    }

    /// 半開区間 `[left,right)` の総和を返す。
    T sum(int left, int right) const {
        assert(0 <= left && left <= right && right <= n_);
        return sum(right) - sum(left);
    }

    /// 添字 `index` の現在値を返す。
    T get(int index) const {
        assert(0 <= index && index < n_);
        return sum(index, index + 1);
    }

    /// 添字 `index` の値を `value` に置き換える。
    void set(int index, const T& value) { add(index, value - get(index)); }

    /// 全要素の総和を返す。
    T total() const { return sum(n_); }

    // 非負値を登録したとき、prefix sum が target 以上になる最小の添字。
    // 存在しなければ size()、target<=0 なら 0 を返す。
    /// prefix sumが `target` 以上になる最小の境界を返す。
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

// @acf fn UnionFind — (int n) -> UnionFind — 経路圧縮とサイズ併合の素集合データ構造

/// 要素を互いに重ならないグループへ分け、併合と「同じグループか」の判定を管理する型です。辺を足した後の連結性や成分サイズを求めるときに使います。
///
/// 引数 `n`: 要素数 n>=0。構築時は各要素が別々のグループに属する。
///
/// 戻り値: 構築される UnionFind。unite(a,b) は新しく併合したとき true、既に同じなら false。same(a,b) は同じグループか、component_size(v) は所属グループの要素数、count() はグループ数。
///
/// 注意: unite は所属関係と成分数を更新します。find / same / component_size など非 const 操作は経路圧縮で内部表現も変えます。要素添字は 0<=v<n。併合の取り消しや成分分割はできません。空構築は count()==0。
///
/// 呼び分け: UnionFind() は要素0、UnionFind(n) は n 個の単独成分。構築してから unite / same を呼びます。
class UnionFind {
    vector<int> parent_or_size_;
    int components_ = 0;

    /// 経路圧縮を伴わずに代表元を探す内部処理。
    int root_const(int x) const {
        assert(0 <= x && x < sz(parent_or_size_));
        while (parent_or_size_[x] >= 0) x = parent_or_size_[x];
        return x;
    }

public:
    /// 要素数0のUnion-Findを作る。
    UnionFind() = default;

    /// `n` 個の互いに異なる要素からUnion-Findを構築する。
    explicit UnionFind(int n)
        : parent_or_size_(n >= 0 ? n : 0, -1), components_(n >= 0 ? n : 0) {
        assert(n >= 0);
    }

    /// 要素数を返す。
    int size() const { return sz(parent_or_size_); }

    /// 要素 `x` の属する集合の代表元を返す。
    int find(int x) {
        assert(0 <= x && x < size());
        if (parent_or_size_[x] < 0) return x;
        return parent_or_size_[x] = find(parent_or_size_[x]);
    }

    /// `a` と `b` の集合を併合し、併合できたらtrueを返す。
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

    /// `a` と `b` が同じ集合に属するか判定する。
    bool same(int a, int b) { return find(a) == find(b); }

    /// `x` の属する集合の要素数を返す。
    int component_size(int x) { return -parent_or_size_[find(x)]; }

    /// 現在の集合数を返す。
    int count() const { return components_; }

    /// 各集合を頂点一覧として返す。
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

namespace ac_detail::algorithm {
template<class T>
T checked_segment_sum(T a, T b) {
    const i128 sum = static_cast<i128>(a) + b;
    if (sum < numeric_limits<T>::min() || sum > numeric_limits<T>::max())
        throw overflow_error("make_sum_segtree: sum does not fit the element type");
    return static_cast<T>(sum);
}

template<class T>
T checked_segment_gcd(T a, T b) {
    if (a < 0 || b < 0) throw invalid_argument("make_gcd_segtree: values must be nonnegative");
    return std::gcd(a, b);
}
}  // namespace ac_detail::algorithm

// @acf fn SegmentTree — (vector<T>, identity[, op]) -> SegmentTree<T> — 汎用の静的モノイド区間木

/// 結合演算で半開区間を集約し、1点更新する。
///
/// 引数 `n / values`: 非負の要素数の単位元配列、または初期配列。内部の2冪配列がintに収まるサイズ。
///
/// 引数 `identity`: 演算の両側の単位元。省略時は `T{}`。
///
/// 引数 `op`: 結合則を満たす二項演算。省略時は `Op{}（既定plus<T>）`。
///
/// 戻り値: `SegmentTree<T,Op>`。prod(l,r)は0-index半開区間の左からの集約、空区間はidentity。all_prod/get/setも利用可能。max_right/min_leftは条件を保つ端点を返します。
///
/// 注意: setは内部を変更しO(log n)。構築時間/メモリO(n)、prod/端点探索O(log n)、get/all_prodはO(1)（演算費用は別）。callbackは副作用なく安定した判定、identityでtrue、区間を伸ばすとtrue→falseの単調性が必要です。内部の2冪配列の要素数がintに収まらないサイズは、配列確保前にlength_errorを送出します。
///
/// 呼び分け: max_right(left,pred) / min_left(right,pred)。範囲とidentityの条件はassert、結合則や単調性は自動検証しません。
template <class T, class Op = plus<T>>
class SegmentTree {
    int n_ = 0, size_ = 1;
    T identity_;
    Op op_;
    vector<T> data_;

    static int checked_count(size_t n) {
        if (n > static_cast<size_t>(numeric_limits<int>::max()))
            throw length_error("SegmentTree: size is too large for int indexing");
        return static_cast<int>(n);
    }

    /// 指定済みの単位元で内部配列を初期化する。
    void init(int n) {
        assert(n >= 0);
        n_ = n;
        size_ = 1;
        while (size_ < n_) {
            if (size_ > numeric_limits<int>::max() / 4)
                throw length_error("SegmentTree: size is too large for int indexing");
            size_ <<= 1;
        }
        data_.assign(2 * size_, identity_);
    }

public:
    /// 要素数0の木を作る。TとOpがデフォルト構築できる場合のみ利用可能。
    SegmentTree() requires (is_default_constructible_v<T> && is_default_constructible_v<Op>)
        : SegmentTree(0, T{}, Op{}) {}

    /// n要素を単位元で初期化する。通常の初期値と単位元を混同しないこと。
    explicit SegmentTree(int n, T identity = T{}, Op op = Op{})
        : identity_(std::move(identity)), op_(std::move(op)) {
        init(n);
    }
    /// 配列をコピーして構築する。単位元と演算を渡せば非default構築型・capture付きラムダも使える。
    explicit SegmentTree(const vector<T>& values, T identity = T{}, Op op = Op{})
        : identity_(std::move(identity)), op_(std::move(op)) {
        init(checked_count(values.size()));
        for (int i = 0; i < n_; ++i) data_[size_ + i] = values[i];
        for (int i = size_ - 1; i > 0; --i)
            data_[i] = op_(data_[i << 1], data_[i << 1 | 1]);
    }

    /// 元配列の要素数を返す。
    int size() const { return n_; }

    /// 添字 `index` の値を更新する。
    void set(int index, const T& value) {
        assert(0 <= index && index < n_);
        index += size_;
        data_[index] = value;
        while (index >>= 1)
            data_[index] = op_(data_[index << 1], data_[index << 1 | 1]);
    }

    void set_checked(int index, T value)
        requires (((same_as<T, int> || same_as<T, ll>) && same_as<Op, T (*)(T, T)>)
                  || (same_as<Op, std::function<T(T, T)>> && std::is_nothrow_move_assignable_v<T>)) {
        assert(0 <= index && index < n_);
        if constexpr (same_as<T, int> || same_as<T, ll>) {
            using Pointer = T (*)(T, T);
            Pointer pointer;
            if constexpr (same_as<Op, Pointer>) pointer = op_;
            else {
                const auto* stored = op_.template target<Pointer>();
                pointer = stored ? *stored : nullptr;
            }
            if (pointer == &ac_detail::algorithm::checked_segment_gcd<T> && value < 0)
                throw invalid_argument("make_gcd_segtree: values must be nonnegative");
            if (pointer && pointer != &ac_detail::algorithm::checked_segment_sum<T>) {
                set(index, value);
                return;
            }
        }
        // Calculate all new sums before publishing the leaf or any ancestor.
        array<T, numeric_limits<int>::digits> sums;
        const int leaf = index + size_;
        int levels = 0;
        sums[levels++] = value;
        for (int node = leaf; node > 1; node >>= 1) {
            value = node & 1 ? op_(data_[node - 1], value) : op_(value, data_[node + 1]);
            sums[levels++] = value;
        }
        int node = leaf;
        for (int level = 0; level < levels; ++level, node >>= 1)
            data_[node] = std::move(sums[level]);
    }

    /// 添字 `index` の値を返す。
    T get(int index) const {
        assert(0 <= index && index < n_);
        return data_[size_ + index];
    }

    /// 半開区間 `[left,right)` を演算 `op` で畳み込む。
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

    /// 全要素を演算 `op` で畳み込む。
    T all_prod() const { return data_[1]; }

    /// `predicate` を保つ最長の左からの半開区間の右端を返す。
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

    /// `predicate` を保つ最長の右からの半開区間の左端を返す。
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
