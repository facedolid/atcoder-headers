#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Immutable snapshot for zero-based order statistics in a half-open range.
template<class T, bool Weighted = false>
requires (std::same_as<T, int> || std::same_as<T, long long>)
class WaveletMatrix {
    using Wide = __int128;
    struct NoSums {};
    int size_;
    std::vector<T> values_;
    std::vector<std::vector<int>> zero_prefix_;
    [[no_unique_address]] std::conditional_t<Weighted, std::vector<std::vector<Wide>>, NoSums> zero_sums_;

    template<class I>
    static constexpr bool is_index = std::same_as<I, int> || std::same_as<I, long long>;

    static int checked_size(std::size_t size) {
        if (size >= (std::size_t{1} << 30))
            throw std::length_error(Weighted ? "weighted_wavelet_matrix: too many elements"
                                             : "wavelet_matrix: too many elements");
        return static_cast<int>(size);
    }

public:
    explicit WaveletMatrix(const std::vector<T>& values)
        : size_(checked_size(values.size())), values_(values) {
        std::sort(values_.begin(), values_.end());
        values_.erase(std::unique(values_.begin(), values_.end()), values_.end());
        if (values_.size() < 2) return;

        const int levels = std::bit_width(static_cast<unsigned>(values_.size() - 1));
        zero_prefix_.reserve(levels);
        if constexpr (Weighted) zero_sums_.reserve(levels);
        std::vector<int> ranks(size_), next(size_);
        for (int i = 0; i < size_; ++i)
            ranks[i] = static_cast<int>(std::lower_bound(values_.begin(), values_.end(), values[i]) - values_.begin());

        for (int bit = levels - 1; bit >= 0; --bit) {
            const int mask = 1 << bit;
            auto& prefix = zero_prefix_.emplace_back(size_ + 1, 0);
            for (int i = 0; i < size_; ++i)
                prefix[i + 1] = prefix[i] + ((ranks[i] & mask) == 0);
            if constexpr (Weighted) {
                auto& sums = zero_sums_.emplace_back(size_ + 1, 0);
                for (int i = 0; i < size_; ++i)
                    sums[i + 1] = sums[i] + ((ranks[i] & mask) ? Wide{0} : values_[ranks[i]]);
            }
            int zero = 0, one = prefix.back();
            for (int rank : ranks)
                next[(rank & mask) ? one++ : zero++] = rank;
            ranks.swap(next);
        }
    }

    int size() const { return size_; }

    template<class L, class R, class K>
    requires (is_index<L> && is_index<R> && is_index<K>)
    T kth(L l, R r, K k) const {
        if (l < 0 || l > r || r > size_)
            throw std::out_of_range(Weighted ? "weighted_wavelet_matrix: invalid interval"
                                             : "wavelet_matrix: invalid interval");
        if (k < 0 || k >= r - l)
            throw std::out_of_range(Weighted ? "weighted_wavelet_matrix: invalid rank"
                                             : "wavelet_matrix: invalid rank");

        int left = static_cast<int>(l), right = static_cast<int>(r);
        int remaining = static_cast<int>(k), rank = 0;
        for (const auto& prefix : zero_prefix_) {
            const int zl = prefix[left], zr = prefix[right];
            rank <<= 1;
            if (remaining < zr - zl) {
                left = zl;
                right = zr;
            } else {
                remaining -= zr - zl;
                rank |= 1;
                left = prefix.back() + (left - zl);
                right = prefix.back() + (right - zr);
            }
        }
        return values_[rank];
    }

    template<class L, class R, class X>
    requires (Weighted && is_index<L> && is_index<R> && is_index<X>)
    long long sum_le(L l, R r, X x) const {
        if (l < 0 || l > r || r > size_)
            throw std::out_of_range("weighted_wavelet_matrix: invalid interval");
        if (l == r) return 0;

        const auto upper = std::upper_bound(values_.begin(), values_.end(), x);
        if (upper == values_.begin()) return 0;
        const int rank = static_cast<int>(upper - values_.begin() - 1);

        int left = static_cast<int>(l), right = static_cast<int>(r);
        Wide sum = 0;
        const int levels = static_cast<int>(zero_prefix_.size());
        for (int level = 0; level < levels; ++level) {
            const auto& prefix = zero_prefix_[level];
            const int zl = prefix[left], zr = prefix[right];
            if (rank & (1 << (levels - level - 1))) {
                sum += zero_sums_[level][right] - zero_sums_[level][left];
                left = prefix.back() + (left - zl);
                right = prefix.back() + (right - zr);
            } else {
                left = zl;
                right = zr;
            }
        }
        sum += static_cast<Wide>(values_[rank]) * (right - left);
        if (sum < std::numeric_limits<long long>::min()
                || sum > std::numeric_limits<long long>::max())
            throw std::overflow_error("weighted_wavelet_matrix: sum does not fit i64");
        return static_cast<long long>(sum);
    }
};

template<class T>
using WeightedWaveletMatrix = WaveletMatrix<T, true>;
