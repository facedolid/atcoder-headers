#pragma once

// Geometry / trigonometry / affine transforms for ac.hpp.
// This header may be included alone or in any order with the other local headers.
#include "ac.hpp"

// LOCAL の core-only 構成でも、visual を後から include した順序を許容する。
#if defined(LOCAL) && !defined(AC_VISUAL_HPP)
#include "ac_visual.hpp"
#endif

// @acf using Point — Point<T,N> — 固定次元の点・ベクトル型
/// @acf-doc @brief `Point<T,N>` はT型のN次元座標。make_point、添字・x/y/z、構造化束縛、入出力、比較、算術演算に対応。2次元では右・左90度回転も提供
/// @acf-doc [type] Point
// 固定次元の点・ベクトル。解答側では make_point(...) の型推論を基本にする。
// Point と Vector は競プロでは同じ座標表現を共有し、意味は変数名とAPIで区別する。
template <class T, size_t N>
struct Point {
    static_assert(N > 0, "Point dimension must be positive");

    using value_type = T;
    static constexpr size_t dimension = N;

    array<T, N> coordinates{};

    /// @brief 全座標を値初期化した点を構築する。
    constexpr Point() = default;

    /// @brief 指定したN個の座標から点を構築する。
    template <class... U,
              enable_if_t<sizeof...(U) == N && (is_convertible_v<U, T> && ...), int> = 0>
    constexpr Point(U&&... values)
        : coordinates{static_cast<T>(std::forward<U>(values))...} {}

    /// @brief 同じ次元の別型の点を座標変換して構築する。
    template <class U>
    constexpr explicit Point(const Point<U, N>& other) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] = static_cast<T>(other[i]);
    }

    /// @brief `array` の各要素を座標変換して点を構築する。
    template <class U>
    constexpr explicit Point(const array<U, N>& values) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] = static_cast<T>(values[i]);
    }

    /// @brief N要素の `vector` から点を構築する。
    template <class U>
    explicit Point(const vector<U>& values) {
        assert(values.size() == N);
        for (size_t i = 0; i < N; ++i)
            coordinates[i] = static_cast<T>(values[i]);
    }

    /// @brief i番目の座標へ範囲チェックなしでアクセスする。
    constexpr T& operator[](size_t i) { return coordinates[i]; }

    /// @brief i番目の座標へ範囲チェックなしでconstアクセスする。
    constexpr const T& operator[](size_t i) const { return coordinates[i]; }

    /// @brief i番目の座標へ範囲チェック付きでアクセスする。
    constexpr T& at(size_t i) { return coordinates.at(i); }

    /// @brief i番目の座標へ範囲チェック付きでconstアクセスする。
    constexpr const T& at(size_t i) const { return coordinates.at(i); }

    /// @brief 座標配列の先頭ポインタを返す。
    constexpr T* data() { return coordinates.data(); }

    /// @brief 座標配列の先頭constポインタを返す。
    constexpr const T* data() const { return coordinates.data(); }

    /// @brief 次元数Nを返す。
    constexpr size_t size() const { return N; }

    /// @brief Pointは常に1個以上の座標を持つためfalseを返す。
    constexpr bool empty() const { return false; }

    /// @brief 先頭座標へのmutable iteratorを返す。
    constexpr auto begin() { return coordinates.begin(); }

    /// @brief 先頭座標へのconst iteratorを返す。
    constexpr auto begin() const { return coordinates.begin(); }

    /// @brief 末尾の次を指すmutable iteratorを返す。
    constexpr auto end() { return coordinates.end(); }

    /// @brief 末尾の次を指すconst iteratorを返す。
    constexpr auto end() const { return coordinates.end(); }

    /// @brief x座標（0番目の座標）へアクセスする。
    constexpr T& x() {
        static_assert(N >= 1, "x() requires at least one dimension");
        return coordinates[0];
    }
    /// @brief x座標へconstアクセスする。
    constexpr const T& x() const {
        static_assert(N >= 1, "x() requires at least one dimension");
        return coordinates[0];
    }
    /// @brief y座標（1番目の座標）へアクセスする。
    constexpr T& y() {
        static_assert(N >= 2, "y() requires at least two dimensions");
        return coordinates[1];
    }
    /// @brief y座標へconstアクセスする。
    constexpr const T& y() const {
        static_assert(N >= 2, "y() requires at least two dimensions");
        return coordinates[1];
    }
    /// @brief z座標（2番目の座標）へアクセスする。
    constexpr T& z() {
        static_assert(N >= 3, "z() requires at least three dimensions");
        return coordinates[2];
    }
    /// @brief z座標へconstアクセスする。
    constexpr const T& z() const {
        static_assert(N >= 3, "z() requires at least three dimensions");
        return coordinates[2];
    }

    /// @brief x右・y上の座標系で右へ90度回転した `(y,-x)` を返す。
    constexpr Point rot90_right() const requires(N == 2) {
        return {coordinates[1], -coordinates[0]};
    }

    /// @brief x右・y上の座標系で左へ90度回転した `(-y,x)` を返す。
    constexpr Point rot90_left() const requires(N == 2) {
        return {-coordinates[1], coordinates[0]};
    }

    /// @brief 対応する座標を加算して自身を更新する。
    template <class U>
    constexpr Point& operator+=(const Point<U, N>& other) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] += static_cast<T>(other[i]);
        return *this;
    }

    /// @brief 対応する座標を減算して自身を更新する。
    template <class U>
    constexpr Point& operator-=(const Point<U, N>& other) {
        for (size_t i = 0; i < N; ++i)
            coordinates[i] -= static_cast<T>(other[i]);
        return *this;
    }

    /// @brief 全座標をスカラー倍して自身を更新する。
    template <class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
    constexpr Point& operator*=(S scalar) {
        for (auto& value : coordinates)
            value = static_cast<T>(value * scalar);
        return *this;
    }

    /// @brief 全座標をスカラーで割って自身を更新する。
    template <class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
    constexpr Point& operator/=(S scalar) {
        assert(scalar != 0);
        for (auto& value : coordinates)
            value = static_cast<T>(value / scalar);
        return *this;
    }
};

template <class First, class... Rest>
Point(First, Rest...)
    -> Point<common_type_t<First, Rest...>, 1 + sizeof...(Rest)>;

template <class T>
using Point2 = Point<T, 2>;
template <class T>
using Point3 = Point<T, 3>;

// 短いエイリアスはコアの vi / pll / stdb と同じく小文字。
// @acf using p2i — Point2<int>
/// @acf-doc [type] p2i
using p2i = Point2<int>;
// @acf using p2ll — Point2<ll>
/// @acf-doc [type] p2ll
using p2ll = Point2<ll>;
// @acf using p2db — Point2<db>
/// @acf-doc [type] p2db
using p2db = Point2<db>;
// @acf using p2ld — Point2<ld>
/// @acf-doc [type] p2ld
using p2ld = Point2<ld>;
// @acf using p3i — Point3<int>
/// @acf-doc [type] p3i
using p3i = Point3<int>;
// @acf using p3ll — Point3<ll>
/// @acf-doc [type] p3ll
using p3ll = Point3<ll>;
// @acf using p3ld — Point3<ld>
/// @acf-doc [type] p3ld
using p3ld = Point3<ld>;
// @acf using vp2i — vector<p2i>
/// @acf-doc [type] vp2i
using vp2i = vector<p2i>;
// @acf using vp2ll — vector<p2ll>
/// @acf-doc [type] vp2ll
using vp2ll = vector<p2ll>;
// @acf using vp2ld — vector<p2ld>
/// @acf-doc [type] vp2ld
using vp2ld = vector<p2ld>;

template <size_t I, class T, size_t N>
constexpr T& get(Point<T, N>& point) {
    static_assert(I < N);
    return point[I];
}
template <size_t I, class T, size_t N>
constexpr const T& get(const Point<T, N>& point) {
    static_assert(I < N);
    return point[I];
}
template <size_t I, class T, size_t N>
constexpr T&& get(Point<T, N>&& point) {
    static_assert(I < N);
    return std::move(point[I]);
}

template <class A, class B, size_t N>
constexpr bool operator==(const Point<A, N>& a, const Point<B, N>& b) {
    for (size_t i = 0; i < N; ++i)
        if (a[i] != b[i]) return false;
    return true;
}
template <class A, class B, size_t N>
constexpr bool operator!=(const Point<A, N>& a, const Point<B, N>& b) {
    return !(a == b);
}
template <class A, class B, size_t N>
constexpr bool operator<(const Point<A, N>& a, const Point<B, N>& b) {
    using R = common_type_t<A, B>;
    for (size_t i = 0; i < N; ++i) {
        const R x = static_cast<R>(a[i]);
        const R y = static_cast<R>(b[i]);
        if (x != y) return x < y;
    }
    return false;
}
template <class A, class B, size_t N>
constexpr bool operator>(const Point<A, N>& a, const Point<B, N>& b) {
    return b < a;
}
template <class A, class B, size_t N>
constexpr bool operator<=(const Point<A, N>& a, const Point<B, N>& b) {
    return !(b < a);
}
template <class A, class B, size_t N>
constexpr bool operator>=(const Point<A, N>& a, const Point<B, N>& b) {
    return !(a < b);
}

template <class A, class B, size_t N>
constexpr auto operator+(const Point<A, N>& a, const Point<B, N>& b) {
    using R = common_type_t<A, B>;
    Point<R, N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(a[i]) + static_cast<R>(b[i]);
    return result;
}
template <class A, class B, size_t N>
constexpr auto operator-(const Point<A, N>& a, const Point<B, N>& b) {
    using R = common_type_t<A, B>;
    Point<R, N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(a[i]) - static_cast<R>(b[i]);
    return result;
}
template <class T, size_t N>
constexpr auto operator-(const Point<T, N>& point) {
    Point<T, N> result;
    for (size_t i = 0; i < N; ++i) result[i] = -point[i];
    return result;
}
template <class T, size_t N>
constexpr auto operator+(const Point<T, N>& point) {
    return point;
}
template <class T, size_t N, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Point<T, N>& point, S scalar) {
    using R = common_type_t<T, S>;
    Point<R, N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(point[i]) * static_cast<R>(scalar);
    return result;
}
template <class S, class T, size_t N, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Point<T, N>& point) {
    return point * scalar;
}
template <class T, size_t N, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Point<T, N>& point, S scalar) {
    assert(scalar != 0);
    using R = common_type_t<T, S>;
    Point<R, N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(point[i]) / static_cast<R>(scalar);
    return result;
}

// 幾何APIが受け取る既存コンテナにも、Pointと同じ座標演算を提供する。
// 2つの座標の * は内積と成分積の意味が衝突するため定義しない。
template <class A, class B, class C, class D>
constexpr auto operator+(const pair<A, B>& a, const pair<C, D>& b) {
    using X = common_type_t<A, C>;
    using Y = common_type_t<B, D>;
    return pair<X, Y>{static_cast<X>(a.first) + static_cast<X>(b.first),
                      static_cast<Y>(a.second) + static_cast<Y>(b.second)};
}
template <class A, class B, class C, class D>
constexpr auto operator-(const pair<A, B>& a, const pair<C, D>& b) {
    using X = common_type_t<A, C>;
    using Y = common_type_t<B, D>;
    return pair<X, Y>{static_cast<X>(a.first) - static_cast<X>(b.first),
                      static_cast<Y>(a.second) - static_cast<Y>(b.second)};
}
template <class A, class B>
constexpr auto operator+(const pair<A, B>& point) {
    return point;
}
template <class A, class B>
constexpr auto operator-(const pair<A, B>& point) {
    return pair<A, B>{-point.first, -point.second};
}
template <class A, class B, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const pair<A, B>& point, S scalar) {
    using X = common_type_t<A, S>;
    using Y = common_type_t<B, S>;
    return pair<X, Y>{static_cast<X>(point.first) * static_cast<X>(scalar),
                      static_cast<Y>(point.second) * static_cast<Y>(scalar)};
}
template <class S, class A, class B,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const pair<A, B>& point) {
    return point * scalar;
}
template <class A, class B, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const pair<A, B>& point, S scalar) {
    assert(scalar != 0);
    using X = common_type_t<A, S>;
    using Y = common_type_t<B, S>;
    return pair<X, Y>{static_cast<X>(point.first) / static_cast<X>(scalar),
                      static_cast<Y>(point.second) / static_cast<Y>(scalar)};
}
template <class A, class B, class C, class D>
constexpr pair<A, B>& operator+=(pair<A, B>& a, const pair<C, D>& b) {
    a.first += b.first;
    a.second += b.second;
    return a;
}
template <class A, class B, class C, class D>
constexpr pair<A, B>& operator-=(pair<A, B>& a, const pair<C, D>& b) {
    a.first -= b.first;
    a.second -= b.second;
    return a;
}
template <class A, class B, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr pair<A, B>& operator*=(pair<A, B>& point, S scalar) {
    point.first *= scalar;
    point.second *= scalar;
    return point;
}
template <class A, class B, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr pair<A, B>& operator/=(pair<A, B>& point, S scalar) {
    assert(scalar != 0);
    point.first /= scalar;
    point.second /= scalar;
    return point;
}

namespace ac_detail::geometry_operators {
template <class TA, class TB, size_t... I>
constexpr auto tuple_add(const TA& a, const TB& b, index_sequence<I...>) {
    return tuple<common_type_t<tuple_element_t<I, TA>,
                               tuple_element_t<I, TB>>...>{
        (get<I>(a) + get<I>(b))...};
}
template <class TA, class TB, size_t... I>
constexpr auto tuple_subtract(const TA& a, const TB& b,
                              index_sequence<I...>) {
    return tuple<common_type_t<tuple_element_t<I, TA>,
                               tuple_element_t<I, TB>>...>{
        (get<I>(a) - get<I>(b))...};
}
template <class Tuple, size_t... I>
constexpr auto tuple_negate(const Tuple& point, index_sequence<I...>) {
    return tuple<tuple_element_t<I, Tuple>...>{(-get<I>(point))...};
}
template <class Tuple, class S, size_t... I>
constexpr auto tuple_multiply(const Tuple& point, S scalar,
                              index_sequence<I...>) {
    return tuple<common_type_t<tuple_element_t<I, Tuple>, S>...>{
        (get<I>(point) * scalar)...};
}
template <class Tuple, class S, size_t... I>
constexpr auto tuple_divide(const Tuple& point, S scalar,
                            index_sequence<I...>) {
    return tuple<common_type_t<tuple_element_t<I, Tuple>, S>...>{
        (get<I>(point) / scalar)...};
}
template <class TA, class TB, size_t... I>
constexpr void tuple_add_assign(TA& a, const TB& b, index_sequence<I...>) {
    ((get<I>(a) += get<I>(b)), ...);
}
template <class TA, class TB, size_t... I>
constexpr void tuple_subtract_assign(TA& a, const TB& b,
                                     index_sequence<I...>) {
    ((get<I>(a) -= get<I>(b)), ...);
}
template <class Tuple, class S, size_t... I>
constexpr void tuple_multiply_assign(Tuple& point, S scalar,
                                     index_sequence<I...>) {
    ((get<I>(point) *= scalar), ...);
}
template <class Tuple, class S, size_t... I>
constexpr void tuple_divide_assign(Tuple& point, S scalar,
                                   index_sequence<I...>) {
    ((get<I>(point) /= scalar), ...);
}
}  // namespace ac_detail::geometry_operators

template <class... A, class... B,
          enable_if_t<sizeof...(A) == sizeof...(B), int> = 0>
constexpr auto operator+(const tuple<A...>& a, const tuple<B...>& b) {
    return ac_detail::geometry_operators::tuple_add(
        a, b, index_sequence_for<A...>{});
}
template <class... A, class... B,
          enable_if_t<sizeof...(A) == sizeof...(B), int> = 0>
constexpr auto operator-(const tuple<A...>& a, const tuple<B...>& b) {
    return ac_detail::geometry_operators::tuple_subtract(
        a, b, index_sequence_for<A...>{});
}
template <class... T>
constexpr auto operator+(const tuple<T...>& point) {
    return point;
}
template <class... T>
constexpr auto operator-(const tuple<T...>& point) {
    return ac_detail::geometry_operators::tuple_negate(
        point, index_sequence_for<T...>{});
}
template <class... T, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const tuple<T...>& point, S scalar) {
    return ac_detail::geometry_operators::tuple_multiply(
        point, scalar, index_sequence_for<T...>{});
}
template <class S, class... T,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const tuple<T...>& point) {
    return point * scalar;
}
template <class... T, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const tuple<T...>& point, S scalar) {
    assert(scalar != 0);
    return ac_detail::geometry_operators::tuple_divide(
        point, scalar, index_sequence_for<T...>{});
}
template <class... A, class... B,
          enable_if_t<sizeof...(A) == sizeof...(B), int> = 0>
constexpr tuple<A...>& operator+=(tuple<A...>& a, const tuple<B...>& b) {
    ac_detail::geometry_operators::tuple_add_assign(
        a, b, index_sequence_for<A...>{});
    return a;
}
template <class... A, class... B,
          enable_if_t<sizeof...(A) == sizeof...(B), int> = 0>
constexpr tuple<A...>& operator-=(tuple<A...>& a, const tuple<B...>& b) {
    ac_detail::geometry_operators::tuple_subtract_assign(
        a, b, index_sequence_for<A...>{});
    return a;
}
template <class... T, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr tuple<T...>& operator*=(tuple<T...>& point, S scalar) {
    ac_detail::geometry_operators::tuple_multiply_assign(
        point, scalar, index_sequence_for<T...>{});
    return point;
}
template <class... T, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr tuple<T...>& operator/=(tuple<T...>& point, S scalar) {
    assert(scalar != 0);
    ac_detail::geometry_operators::tuple_divide_assign(
        point, scalar, index_sequence_for<T...>{});
    return point;
}

template <class A, class B, size_t N>
constexpr auto operator+(const array<A, N>& a, const array<B, N>& b) {
    using R = common_type_t<A, B>;
    array<R, N> result{};
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(a[i]) + static_cast<R>(b[i]);
    return result;
}
template <class A, class B, size_t N>
constexpr auto operator-(const array<A, N>& a, const array<B, N>& b) {
    using R = common_type_t<A, B>;
    array<R, N> result{};
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(a[i]) - static_cast<R>(b[i]);
    return result;
}
template <class T, size_t N>
constexpr auto operator+(const array<T, N>& point) {
    return point;
}
template <class T, size_t N>
constexpr auto operator-(const array<T, N>& point) {
    array<T, N> result{};
    for (size_t i = 0; i < N; ++i) result[i] = -point[i];
    return result;
}
template <class T, size_t N, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const array<T, N>& point, S scalar) {
    using R = common_type_t<T, S>;
    array<R, N> result{};
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(point[i]) * static_cast<R>(scalar);
    return result;
}
template <class S, class T, size_t N,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const array<T, N>& point) {
    return point * scalar;
}
template <class T, size_t N, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const array<T, N>& point, S scalar) {
    assert(scalar != 0);
    using R = common_type_t<T, S>;
    array<R, N> result{};
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(point[i]) / static_cast<R>(scalar);
    return result;
}
template <class A, class B, size_t N>
constexpr array<A, N>& operator+=(array<A, N>& a, const array<B, N>& b) {
    for (size_t i = 0; i < N; ++i) a[i] += b[i];
    return a;
}
template <class A, class B, size_t N>
constexpr array<A, N>& operator-=(array<A, N>& a, const array<B, N>& b) {
    for (size_t i = 0; i < N; ++i) a[i] -= b[i];
    return a;
}
template <class T, size_t N, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr array<T, N>& operator*=(array<T, N>& point, S scalar) {
    for (auto& value : point) value *= scalar;
    return point;
}
template <class T, size_t N, class S,
          enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr array<T, N>& operator/=(array<T, N>& point, S scalar) {
    assert(scalar != 0);
    for (auto& value : point) value /= scalar;
    return point;
}

template <class A, class B>
auto operator+(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    using R = common_type_t<A, B>;
    vector<R> result(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        result[i] = static_cast<R>(a[i]) + static_cast<R>(b[i]);
    return result;
}
template <class A, class B>
auto operator-(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    using R = common_type_t<A, B>;
    vector<R> result(a.size());
    for (size_t i = 0; i < a.size(); ++i)
        result[i] = static_cast<R>(a[i]) - static_cast<R>(b[i]);
    return result;
}
template <class T>
auto operator+(const vector<T>& point) {
    return point;
}
template <class T>
auto operator-(const vector<T>& point) {
    vector<T> result(point.size());
    for (size_t i = 0; i < point.size(); ++i) result[i] = -point[i];
    return result;
}
template <class T, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
auto operator*(const vector<T>& point, S scalar) {
    using R = common_type_t<T, S>;
    vector<R> result(point.size());
    for (size_t i = 0; i < point.size(); ++i)
        result[i] = static_cast<R>(point[i]) * static_cast<R>(scalar);
    return result;
}
template <class S, class T, enable_if_t<is_arithmetic_v<S>, int> = 0>
auto operator*(S scalar, const vector<T>& point) {
    return point * scalar;
}
template <class T, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
auto operator/(const vector<T>& point, S scalar) {
    assert(scalar != 0);
    using R = common_type_t<T, S>;
    vector<R> result(point.size());
    for (size_t i = 0; i < point.size(); ++i)
        result[i] = static_cast<R>(point[i]) / static_cast<R>(scalar);
    return result;
}
template <class A, class B>
vector<A>& operator+=(vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i) a[i] += b[i];
    return a;
}
template <class A, class B>
vector<A>& operator-=(vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i) a[i] -= b[i];
    return a;
}
template <class T, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
vector<T>& operator*=(vector<T>& point, S scalar) {
    for (auto& value : point) value *= scalar;
    return point;
}
template <class T, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
vector<T>& operator/=(vector<T>& point, S scalar) {
    assert(scalar != 0);
    for (auto& value : point) value /= scalar;
    return point;
}

namespace std {
template <class T, size_t N>
struct tuple_size<::Point<T, N>> : integral_constant<size_t, N> {};
template <size_t I, class T, size_t N>
struct tuple_element<I, ::Point<T, N>> {
    static_assert(I < N);
    using type = T;
};
template <class T, size_t N>
struct hash<::Point<T, N>> {
    /// @brief 点の全座標からハッシュ値を計算する。
    size_t operator()(const ::Point<T, N>& point) const {
        size_t seed = 0;
        for (const auto& value : point) {
            const size_t h = hash<T>{}(value);
            seed ^= h + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
}  // namespace std


// @acf fn directions4 — (order="RLUD") -> array<Point2<int>,4> — 指定順の上下左右の移動ベクトル
/// @acf-doc @brief xは右が正、yは上が正。R=(1,0), L=(-1,0), U=(0,1), D=(0,-1)。orderは4文字の順列、不正ならinvalid_argument
/// @acf-doc [function] directions4
constexpr array<Point2<int>, 4> directions4(string_view order = "RLUD") {
    if (order.size() != 4)
        throw invalid_argument("directions4: order must be a permutation of RLUD");
    array<Point2<int>, 4> result{};
    unsigned seen = 0;
    for (size_t i = 0; i < order.size(); ++i) {
        unsigned bit;
        switch (order[i]) {
            case 'R': result[i] = Point2<int>{1, 0}; bit = 1; break;
            case 'L': result[i] = Point2<int>{-1, 0}; bit = 2; break;
            case 'U': result[i] = Point2<int>{0, 1}; bit = 4; break;
            case 'D': result[i] = Point2<int>{0, -1}; bit = 8; break;
            default:
                throw invalid_argument("directions4: order must be a permutation of RLUD");
        }
        if (seen & bit)
            throw invalid_argument("directions4: order must be a permutation of RLUD");
        seen |= bit;
    }
    return result;
}

// @acf fn direction_map4 — () -> unordered_map<char,Point2<int>> — 方向文字から移動ベクトルを引く
/// @acf-doc @brief R/L/U/Dをx右正・y上正のPoint2<int>へ対応させる。毎回独立したmapを返す
/// @acf-doc [function] direction_map4
inline unordered_map<char, Point2<int>> direction_map4() {
    const auto steps = directions4();
    return {{'R', steps[0]}, {'L', steps[1]}, {'U', steps[2]}, {'D', steps[3]}};
}

// @acf fn direction_map4_xy — () -> unordered_map<char,Point2<int>> — 方向文字から(x,y)の移動ベクトルを引く
/// @acf-doc @brief xは右が正、yは上が正。U=(0,1), D=(0,-1), L=(-1,0), R=(1,0)。direction_map4と同じで、毎回独立したmapを返す
/// @acf-doc [function] direction_map4_xy
inline unordered_map<char, Point2<int>> direction_map4_xy() {
    return direction_map4();
}

// @acf fn direction_map4_grid — () -> unordered_map<char,Point2<int>> — 方向文字から(行,列)の移動ベクトルを引く
/// @acf-doc @brief 行は下が正、列は右が正。U=(-1,0), D=(1,0), L=(0,-1), R=(0,1)。毎回独立したmapを返す
/// @acf-doc [function] direction_map4_grid
inline unordered_map<char, Point2<int>> direction_map4_grid() {
    return {{'R', {0, 1}}, {'L', {0, -1}}, {'U', {-1, 0}}, {'D', {1, 0}}};
}

namespace ac_detail::geometry {
template <class Tuple, size_t... I>
constexpr auto point_from_tuple(const Tuple& values, index_sequence<I...>) {
    using R = common_type_t<remove_cv_t<remove_reference_t<decltype(get<I>(values))>>...>;
    return Point<R, sizeof...(I)>{static_cast<R>(get<I>(values))...};
}
}  // namespace ac_detail::geometry

// @acf fn make_point — coordinates / pair / tuple / array -> Point — 座標から固定次元の点を作る
/// @acf-doc @brief `make_point(x,y,...)` は座標型をcommon_typeに揃えた `Point<T,N>` を返す。pair・tuple・array、`make_point<N>(vector)` からも変換できる
/// @acf-doc @code (coordinates...) / (container) -> Point<common_type, dimension>
/// @acf-doc [function] make_point
template <class First, class Second, class... Rest>
constexpr auto make_point(First&& first, Second&& second, Rest&&... rest) {
    using R = common_type_t<remove_cv_t<remove_reference_t<First>>,
                            remove_cv_t<remove_reference_t<Second>>,
                            remove_cv_t<remove_reference_t<Rest>>...>;
    return Point<R, 2 + sizeof...(Rest)>{
        static_cast<R>(std::forward<First>(first)),
        static_cast<R>(std::forward<Second>(second)),
        static_cast<R>(std::forward<Rest>(rest))...};
}

template <class T,
          enable_if_t<is_arithmetic_v<remove_cv_t<remove_reference_t<T>>>, int> = 0>
constexpr auto make_point(T&& value) {
    using R = remove_cv_t<remove_reference_t<T>>;
    return Point<R, 1>{static_cast<R>(std::forward<T>(value))};
}

template <class A, class B>
constexpr auto make_point(const pair<A, B>& values) {
    using R = common_type_t<A, B>;
    return Point<R, 2>{static_cast<R>(values.first), static_cast<R>(values.second)};
}

template <class... T>
constexpr auto make_point(const tuple<T...>& values) {
    static_assert(sizeof...(T) > 0, "make_point requires at least one coordinate");
    return ac_detail::geometry::point_from_tuple(values, index_sequence_for<T...>{});
}

template <class T, size_t N>
constexpr auto make_point(const array<T, N>& values) {
    static_assert(N > 0, "make_point requires at least one coordinate");
    return Point<T, N>{values};
}

template <class T, size_t N>
constexpr auto make_point(const Point<T, N>& point) {
    return point;
}

template <size_t N, class T>
auto make_point(const vector<T>& values) {
    return Point<T, N>{values};
}

// @acf fn point_cast — (Point<U,N>) -> Point<T,N> — 点の座標型を変換する
/// @acf-doc @brief `point_cast<T>(point)` は次元を保ったまま全座標をTへ変換する
/// @acf-doc @code <class T>(const Point<U,N>&) -> Point<T,N>
/// @acf-doc [function] point_cast
template <class T, class U, size_t N>
constexpr auto point_cast(const Point<U, N>& point) {
    return Point<T, N>{point};
}

// @acf fn manhattan_distance — (a,b) / coordinates -> common_type — 2点間のマンハッタン距離
/// @acf-doc @brief 2点のマンハッタン距離。Point・pair・tuple・vector・array、または2D・3Dの生座標に対応
/// @acf-doc @code (a, b) / (x1, y1, [z1,] x2, y2, [z2]) -> common_type
/// @acf-doc [function] manhattan_distance
template <class A, class B, class C, class D>
constexpr auto manhattan_distance(A r1, B c1, C r2, D c2) {
    using R = common_type_t<A, B, C, D>;
    const R dr = static_cast<R>(r1) - static_cast<R>(r2);
    const R dc = static_cast<R>(c1) - static_cast<R>(c2);
    return (dr < 0 ? -dr : dr) + (dc < 0 ? -dc : dc);
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto manhattan_distance(A x1, B y1, C z1, D x2, E y2, F z2) {
    using R = common_type_t<A, B, C, D, E, F>;
    const R dx = static_cast<R>(x1) - static_cast<R>(x2);
    const R dy = static_cast<R>(y1) - static_cast<R>(y2);
    const R dz = static_cast<R>(z1) - static_cast<R>(z2);
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy)
         + (dz < 0 ? -dz : dz);
}

template <class A, class B, class C, class D>
constexpr auto manhattan_distance(const pair<A, B>& a, const pair<C, D>& b) {
    return manhattan_distance(a.first, a.second, b.first, b.second);
}

template <class A, class B, size_t N>
constexpr auto manhattan_distance(const array<A, N>& a, const array<B, N>& b) {
    using R = common_type_t<A, B>;
    R result{};
    for (size_t i = 0; i < N; ++i) {
        const R d = static_cast<R>(a[i]) - static_cast<R>(b[i]);
        result += d < 0 ? -d : d;
    }
    return result;
}

template <class A, class B, size_t N>
constexpr auto manhattan_distance(const Point<A, N>& a, const Point<B, N>& b) {
    using R = common_type_t<A, B>;
    R result{};
    for (size_t i = 0; i < N; ++i) {
        const R d = static_cast<R>(a[i]) - static_cast<R>(b[i]);
        result += d < 0 ? -d : d;
    }
    return result;
}

template <class A, class B>
auto manhattan_distance(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    using R = common_type_t<A, B>;
    R result{};
    for (size_t i = 0; i < a.size(); ++i) {
        const R d = static_cast<R>(a[i]) - static_cast<R>(b[i]);
        result += d < 0 ? -d : d;
    }
    return result;
}

namespace ac_detail::geometry {
template <class... A, class... B, size_t... I>
constexpr auto manhattan_tuple(const tuple<A...>& a, const tuple<B...>& b,
                               index_sequence<I...>) {
    static_assert(sizeof...(A) == sizeof...(B));
    using R = common_type_t<A..., B...>;
    R result{};
    ((result += [&] {
        const R d = static_cast<R>(get<I>(a)) - static_cast<R>(get<I>(b));
        return d < 0 ? -d : d;
    }()), ...);
    return result;
}
}  // namespace ac_detail::geometry

template <class... A, class... B>
constexpr auto manhattan_distance(const tuple<A...>& a, const tuple<B...>& b) {
    static_assert(sizeof...(A) == sizeof...(B));
    static_assert(sizeof...(A) > 0);
    return ac_detail::geometry::manhattan_tuple(
        a, b, index_sequence_for<A...>{});
}

// @acf fn euclidean_distance_squared — (a,b) / coordinates -> product_result — 2点間の平方ユークリッド距離
/// @acf-doc @brief 2点の平方ユークリッド距離（sqrtなし）。整数は中間を広げ、戻りは `product_result_t`（`int`→`ll`、`ll`はそのまま）
/// @acf-doc @code (a, b) / (x1, y1, [z1,] x2, y2, [z2]) -> product_result
/// @acf-doc [function] euclidean_distance_squared
template <class A, class B, class C, class D>
constexpr auto euclidean_distance_squared(A r1, B c1, C r2, D c2) {
    using D0 = common_type_t<A, B, C, D>;
    const D0 dr = static_cast<D0>(r1) - static_cast<D0>(r2);
    const D0 dc = static_cast<D0>(c1) - static_cast<D0>(c2);
    return ac_detail::as_product_result<A, B, C, D>(
        wide_mul(dr, dr) + wide_mul(dc, dc));
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto euclidean_distance_squared(A x1, B y1, C z1,
                                          D x2, E y2, F z2) {
    using D0 = common_type_t<A, B, C, D, E, F>;
    const D0 dx = static_cast<D0>(x1) - static_cast<D0>(x2);
    const D0 dy = static_cast<D0>(y1) - static_cast<D0>(y2);
    const D0 dz = static_cast<D0>(z1) - static_cast<D0>(z2);
    return ac_detail::as_product_result<A, B, C, D, E, F>(
        wide_mul(dx, dx) + wide_mul(dy, dy) + wide_mul(dz, dz));
}

template <class A, class B>
constexpr auto euclidean_distance_squared(const A& a, const B& b);

// @acf fn make_vec — (from,to) / (x1,y1,x2,y2) / (x1,y1,z1,x2,y2,z2) -> 同形の差ベクトル — from→to
/// @acf-doc @brief `make_vec(from,to)` は `to-from`。Point・pair・tuple・vector・array対応。2Dは `x1,y1,x2,y2`、3Dは6引数の生座標版もあり
/// @acf-doc @code (from, to) / (x1,y1,x2,y2) / (x1,y1,z1,x2,y2,z2) -> same-shaped common_type
/// @acf-doc [function] make_vec
template <class A, class B, class C, class D>
constexpr auto make_vec(const pair<A, B>& from, const pair<C, D>& to) {
    using R = common_type_t<A, B, C, D>;
    return pair<R, R>{static_cast<R>(to.first) - static_cast<R>(from.first),
                      static_cast<R>(to.second) - static_cast<R>(from.second)};
}

template <class A, class B>
auto make_vec(const vector<A>& from, const vector<B>& to) {
    assert(from.size() == to.size());
    using R = common_type_t<A, B>;
    vector<R> result(from.size());
    for (size_t i = 0; i < from.size(); ++i)
        result[i] = static_cast<R>(to[i]) - static_cast<R>(from[i]);
    return result;
}

template <class A, class B, size_t N>
constexpr auto make_vec(const array<A, N>& from, const array<B, N>& to) {
    using R = common_type_t<A, B>;
    array<R, N> result{};
    for (size_t i = 0; i < N; ++i)
        result[i] = static_cast<R>(to[i]) - static_cast<R>(from[i]);
    return result;
}

template <class A, class B, size_t N>
constexpr auto make_vec(const Point<A, N>& from, const Point<B, N>& to) {
    return to - from;
}

namespace ac_detail::geometry {
template <class... A, class... B, size_t... I>
constexpr auto tuple_difference(const tuple<A...>& from, const tuple<B...>& to,
                                index_sequence<I...>) {
    return tuple<common_type_t<tuple_element_t<I, tuple<A...>>,
                               tuple_element_t<I, tuple<B...>>>...>{
        (static_cast<common_type_t<tuple_element_t<I, tuple<A...>>,
                                   tuple_element_t<I, tuple<B...>>>>(get<I>(to))
       - static_cast<common_type_t<tuple_element_t<I, tuple<A...>>,
                                   tuple_element_t<I, tuple<B...>>>>(get<I>(from)))...
    };
}
}  // namespace ac_detail::geometry

template <class... A, class... B>
constexpr auto make_vec(const tuple<A...>& from, const tuple<B...>& to) {
    static_assert(sizeof...(A) == sizeof...(B));
    return ac_detail::geometry::tuple_difference(
        from, to, index_sequence_for<A...>{});
}

template <class A, class B, class C, class D>
constexpr auto make_vec(A x1, B y1, C x2, D y2) {
    return make_vec(pair{x1, y1}, pair{x2, y2});
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto make_vec(A x1, B y1, C z1, D x2, E y2, F z2) {
    return make_vec(tuple{x1, y1, z1}, tuple{x2, y2, z2});
}

// @acf fn dot_product — (a,b) / coordinates -> product_result — 内積。Point・pair・tuple・vector・array・生座標対応
/// @acf-doc @brief 2ベクトルの内積。整数は中間を広げ、戻りは `product_result_t`。2Dは `x1,y1,x2,y2`、3Dは6引数の生座標版あり
/// @acf-doc @code (a, b) / (x1,y1,[z1,]x2,y2,[z2]) -> product_result
/// @acf-doc [function] dot_product
template <class A, class B, class C, class D>
constexpr auto dot_product(const pair<A, B>& a, const pair<C, D>& b) {
    return ac_detail::as_product_result<A, B, C, D>(
        wide_mul(a.first, b.first) + wide_mul(a.second, b.second));
}

template <class A, class B>
auto dot_product(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    using W = product_type_t<A, B>;
    W result{};
    for (size_t i = 0; i < a.size(); ++i)
        result += wide_mul(a[i], b[i]);
    return ac_detail::as_product_result<A, B>(result);
}

template <class A, class B, size_t N>
constexpr auto dot_product(const array<A, N>& a, const array<B, N>& b) {
    using W = product_type_t<A, B>;
    W result{};
    for (size_t i = 0; i < N; ++i)
        result += wide_mul(a[i], b[i]);
    return ac_detail::as_product_result<A, B>(result);
}

template <class A, class B, size_t N>
constexpr auto dot_product(const Point<A, N>& a, const Point<B, N>& b) {
    using W = product_type_t<A, B>;
    W result{};
    for (size_t i = 0; i < N; ++i)
        result += wide_mul(a[i], b[i]);
    return ac_detail::as_product_result<A, B>(result);
}

namespace ac_detail::geometry {
template <class... A, class... B, size_t... I>
constexpr auto dot_tuple(const tuple<A...>& a, const tuple<B...>& b,
                         index_sequence<I...>) {
    static_assert(sizeof...(A) == sizeof...(B));
    using W = product_type_t<A..., B...>;
    return ac_detail::as_product_result<A..., B...>(
        ((wide_mul(get<I>(a), get<I>(b))) + ... + W{}));
}
}  // namespace ac_detail::geometry

template <class... A, class... B>
constexpr auto dot_product(const tuple<A...>& a, const tuple<B...>& b) {
    static_assert(sizeof...(A) == sizeof...(B));
    static_assert(sizeof...(A) > 0);
    return ac_detail::geometry::dot_tuple(a, b, index_sequence_for<A...>{});
}

template <class A, class B, class C, class D>
constexpr auto dot_product(A x1, B y1, C x2, D y2) {
    return dot_product(pair{x1, y1}, pair{x2, y2});
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto dot_product(A x1, B y1, C z1, D x2, E y2, F z2) {
    return dot_product(tuple{x1, y1, z1}, tuple{x2, y2, z2});
}

namespace ac_detail::geometry {
template <class A, class B, class C, class D>
long double dot_ld(const pair<A, B>& a, const pair<C, D>& b) {
    return static_cast<long double>(a.first) * static_cast<long double>(b.first)
         + static_cast<long double>(a.second) * static_cast<long double>(b.second);
}

template <class A, class B>
long double dot_ld(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == b.size());
    long double result = 0;
    for (size_t i = 0; i < a.size(); ++i)
        result += static_cast<long double>(a[i]) * static_cast<long double>(b[i]);
    return result;
}

template <class A, class B, size_t N>
long double dot_ld(const array<A, N>& a, const array<B, N>& b) {
    long double result = 0;
    for (size_t i = 0; i < N; ++i)
        result += static_cast<long double>(a[i]) * static_cast<long double>(b[i]);
    return result;
}

template <class A, class B, size_t N>
long double dot_ld(const Point<A, N>& a, const Point<B, N>& b) {
    long double result = 0;
    for (size_t i = 0; i < N; ++i)
        result += static_cast<long double>(a[i]) * static_cast<long double>(b[i]);
    return result;
}


template <class... A, class... B, size_t... I>
long double dot_ld_tuple(const tuple<A...>& a, const tuple<B...>& b,
                         index_sequence<I...>) {
    static_assert(sizeof...(A) == sizeof...(B));
    return ((static_cast<long double>(get<I>(a))
             * static_cast<long double>(get<I>(b))) + ... + 0.0L);
}

template <class... A, class... B>
long double dot_ld(const tuple<A...>& a, const tuple<B...>& b) {
    return dot_ld_tuple(a, b, index_sequence_for<A...>{});
}
}  // namespace ac_detail::geometry
// @acf fn cross_product2d — (a,b) / (o,a,b) / coordinates -> product_result — 2次元外積（符号付き）
/// @acf-doc @brief 符号付き外積 `ax*by-ay*bx`。整数は中間を広げ、戻りは `product_result_t`。3点版は向き判定
/// @acf-doc @code (a, b) / (origin, a, b) / (ax,ay,bx,by) / (ox,oy,ax,ay,bx,by) -> product_result
/// @acf-doc [function] cross_product2d
template <class A, class B, class C, class D>
constexpr auto cross_product2d(const pair<A, B>& a, const pair<C, D>& b) {
    return ac_detail::as_product_result<A, B, C, D>(
        wide_mul(a.first, b.second) - wide_mul(a.second, b.first));
}

template <class A, class B>
auto cross_product2d(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == 2 && b.size() == 2);
    return ac_detail::as_product_result<A, B>(
        wide_mul(a[0], b[1]) - wide_mul(a[1], b[0]));
}

template <class A, class B>
constexpr auto cross_product2d(const array<A, 2>& a, const array<B, 2>& b) {
    return ac_detail::as_product_result<A, B>(
        wide_mul(a[0], b[1]) - wide_mul(a[1], b[0]));
}

template <class A, class B>
constexpr auto cross_product2d(const Point<A, 2>& a, const Point<B, 2>& b) {
    return ac_detail::as_product_result<A, B>(
        wide_mul(a[0], b[1]) - wide_mul(a[1], b[0]));
}

template <class A, class B, class C, class D>
constexpr auto cross_product2d(const tuple<A, B>& a, const tuple<C, D>& b) {
    return ac_detail::as_product_result<A, B, C, D>(
        wide_mul(get<0>(a), get<1>(b)) - wide_mul(get<1>(a), get<0>(b)));
}

template <class O, class A, class B>
constexpr auto cross_product2d(const O& origin, const A& a, const B& b) {
    return cross_product2d(make_vec(origin, a), make_vec(origin, b));
}

template <class A, class B, class C, class D>
constexpr auto cross_product2d(A ax, B ay, C bx, D by) {
    return cross_product2d(pair{ax, ay}, pair{bx, by});
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto cross_product2d(A ox, B oy, C ax, D ay, E bx, F by) {
    return cross_product2d(pair{ox, oy}, pair{ax, ay}, pair{bx, by});
}

// @acf fn cross_product3d — (a,b) / coordinates -> Point/tuple/vector/array — 3次元外積
/// @acf-doc @brief 3次元外積。整数は中間を広げ、成分の戻りは `product_result_t`。6引数版の戻りは tuple
/// @acf-doc @code (a, b) / (ax,ay,az,bx,by,bz) -> Point3
/// @acf-doc [function] cross_product3d
template <class A, class B>
auto cross_product3d(const vector<A>& a, const vector<B>& b) {
    assert(a.size() == 3 && b.size() == 3);
    using R = product_result_t<A, B>;
    return vector<R>{
        ac_detail::as_product_result<A, B>(
            wide_mul(a[1], b[2]) - wide_mul(a[2], b[1])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[2], b[0]) - wide_mul(a[0], b[2])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[0], b[1]) - wide_mul(a[1], b[0])),
    };
}

template <class A, class B>
constexpr auto cross_product3d(const array<A, 3>& a, const array<B, 3>& b) {
    using R = product_result_t<A, B>;
    return array<R, 3>{
        ac_detail::as_product_result<A, B>(
            wide_mul(a[1], b[2]) - wide_mul(a[2], b[1])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[2], b[0]) - wide_mul(a[0], b[2])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[0], b[1]) - wide_mul(a[1], b[0])),
    };
}

template <class A, class B>
constexpr auto cross_product3d(const Point<A, 3>& a, const Point<B, 3>& b) {
    using R = product_result_t<A, B>;
    return Point<R, 3>{
        ac_detail::as_product_result<A, B>(
            wide_mul(a[1], b[2]) - wide_mul(a[2], b[1])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[2], b[0]) - wide_mul(a[0], b[2])),
        ac_detail::as_product_result<A, B>(
            wide_mul(a[0], b[1]) - wide_mul(a[1], b[0])),
    };
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto cross_product3d(const tuple<A, B, C>& a,
                               const tuple<D, E, F>& b) {
    using R = product_result_t<A, B, C, D, E, F>;
    return tuple<R, R, R>{
        ac_detail::as_product_result<A, B, C, D, E, F>(
            wide_mul(get<1>(a), get<2>(b)) - wide_mul(get<2>(a), get<1>(b))),
        ac_detail::as_product_result<A, B, C, D, E, F>(
            wide_mul(get<2>(a), get<0>(b)) - wide_mul(get<0>(a), get<2>(b))),
        ac_detail::as_product_result<A, B, C, D, E, F>(
            wide_mul(get<0>(a), get<1>(b)) - wide_mul(get<1>(a), get<0>(b))),
    };
}

template <class A, class B, class C, class D, class E, class F>
constexpr auto cross_product3d(A ax, B ay, C az, D bx, E by, F bz) {
    return cross_product3d(tuple{ax, ay, az}, tuple{bx, by, bz});
}

// @acf fn vector_norm_squared — (v) / coordinates -> value_type — ベクトル長の二乗
/// @acf-doc @brief `dot_product(v,v)`。生座標は2D `(x,y)`、3D `(x,y,z)`
/// @acf-doc @code (v) / (x,y) / (x,y,z) -> value_type
/// @acf-doc [function] vector_norm_squared
template <class V>
constexpr auto vector_norm_squared(const V& v) {
    return dot_product(v, v);
}

template <class X, class Y>
constexpr auto vector_norm_squared(X x, Y y) {
    return vector_norm_squared(pair{x, y});
}

template <class X, class Y, class Z>
constexpr auto vector_norm_squared(X x, Y y, Z z) {
    return vector_norm_squared(tuple{x, y, z});
}

// @acf fn vector_norm — (v) / coordinates -> long double — ベクトル長
/// @acf-doc @brief ユークリッド長。生座標は2D `(x,y)`、3D `(x,y,z)`
/// @acf-doc @code (v) / (x,y) / (x,y,z) -> long double
/// @acf-doc [function] vector_norm
template <class V>
long double vector_norm(const V& v) {
    return sqrt(max(0.0L, ac_detail::geometry::dot_ld(v, v)));
}

template <class X, class Y>
long double vector_norm(X x, Y y) {
    return vector_norm(pair{x, y});
}

template <class X, class Y, class Z>
long double vector_norm(X x, Y y, Z z) {
    return vector_norm(tuple{x, y, z});
}

// @acf fn euclidean_distance — (a,b) / coordinates -> long double — 2点間のユークリッド距離
/// @acf-doc @brief `euclidean_distance` は2点間の距離をlong doubleで返す。点コンテナ2個、または2D・3Dの生座標で呼べる
/// @acf-doc @code (a, b) / (x1, y1, [z1,] x2, y2, [z2]) -> long double
/// @acf-doc [function] euclidean_distance
template <class A, class B>
long double euclidean_distance(const A& a, const B& b) {
    return vector_norm(make_vec(a, b));
}


template <class A, class B>
constexpr auto euclidean_distance_squared(const A& a, const B& b) {
    return vector_norm_squared(make_vec(a, b));
}

template <class A, class B, class C, class D>
long double euclidean_distance(A x1, B y1, C x2, D y2) {
    const long double dx = static_cast<long double>(x1) - static_cast<long double>(x2);
    const long double dy = static_cast<long double>(y1) - static_cast<long double>(y2);
    return hypot(dx, dy);
}

template <class A, class B, class C, class D, class E, class F>
long double euclidean_distance(A x1, B y1, C z1, D x2, E y2, F z2) {
    const long double dx = static_cast<long double>(x1) - static_cast<long double>(x2);
    const long double dy = static_cast<long double>(y1) - static_cast<long double>(y2);
    const long double dz = static_cast<long double>(z1) - static_cast<long double>(z2);
    return hypot(hypot(dx, dy), dz);
}

// @acf fn point_line_distance — (p,a,b) -> long double — 点 p と直線 ab の距離。2D/3D以上対応
/// @acf-doc @brief `point_line_distance(p,a,b)` は点 `p` と、2点 `a,b` を通る無限直線の距離。2D/3D以上に対応し、`a==b` なら点 `a` までの距離
/// @acf-doc @code (const Point& p, const Point& a, const Point& b) -> long double
/// @acf-doc [function] point_line_distance
template <class P, class A, class B>
long double point_line_distance(const P& p, const A& a, const B& b) {
    const auto ab = make_vec(a, b);
    const auto ap = make_vec(a, p);
    const long double ab2 = ac_detail::geometry::dot_ld(ab, ab);
    if (ab2 == 0) return euclidean_distance(p, a);
    const long double ap2 = ac_detail::geometry::dot_ld(ap, ap);
    const long double projection = ac_detail::geometry::dot_ld(ap, ab);
    return sqrt(max(0.0L, ap2 - projection * projection / ab2));
}


template <class X, class Y, class AX, class AY, class BX, class BY>
long double point_line_distance(X x, Y y, AX ax, AY ay, BX bx, BY by) {
    return point_line_distance(pair<X, Y>{x, y}, pair<AX, AY>{ax, ay},
                               pair<BX, BY>{bx, by});
}

template <class X, class Y, class Z,
          class AX, class AY, class AZ, class BX, class BY, class BZ>
long double point_line_distance(X x, Y y, Z z,
                                AX ax, AY ay, AZ az, BX bx, BY by, BZ bz) {
    return point_line_distance(array{static_cast<long double>(x),
                                     static_cast<long double>(y),
                                     static_cast<long double>(z)},
                               array{static_cast<long double>(ax),
                                     static_cast<long double>(ay),
                                     static_cast<long double>(az)},
                               array{static_cast<long double>(bx),
                                     static_cast<long double>(by),
                                     static_cast<long double>(bz)});
}

// @acf fn point_segment_distance — (p,a,b) -> long double — 点 p と線分 ab の最短距離。2D/3D以上対応
/// @acf-doc @brief `point_segment_distance(p,a,b)` は点 `p` と線分 `ab` の最短距離。垂線の足が線分外なら近い端点まで、線分上なら `point_line_distance` を返す。`a==b` にも対応
/// @acf-doc @code (const Point& p, const Point& a, const Point& b) -> long double
/// @acf-doc [function] point_segment_distance
template <class P, class A, class B>
long double point_segment_distance(const P& p, const A& a, const B& b) {
    const auto ab = make_vec(a, b);
    const auto ap = make_vec(a, p);
    if (ac_detail::geometry::dot_ld(ap, ab) <= 0) return euclidean_distance(p, a);
    const auto ba = make_vec(b, a);
    const auto bp = make_vec(b, p);
    if (ac_detail::geometry::dot_ld(bp, ba) <= 0) return euclidean_distance(p, b);
    return point_line_distance(p, a, b);
}


template <class X, class Y, class AX, class AY, class BX, class BY>
long double point_segment_distance(X x, Y y, AX ax, AY ay, BX bx, BY by) {
    return point_segment_distance(pair<X, Y>{x, y}, pair<AX, AY>{ax, ay},
                                  pair<BX, BY>{bx, by});
}

template <class X, class Y, class Z,
          class AX, class AY, class AZ, class BX, class BY, class BZ>
long double point_segment_distance(X x, Y y, Z z,
                                   AX ax, AY ay, AZ az, BX bx, BY by, BZ bz) {
    return point_segment_distance(array{static_cast<long double>(x),
                                        static_cast<long double>(y),
                                        static_cast<long double>(z)},
                                  array{static_cast<long double>(ax),
                                        static_cast<long double>(ay),
                                        static_cast<long double>(az)},
                                  array{static_cast<long double>(bx),
                                        static_cast<long double>(by),
                                        static_cast<long double>(bz)});
}

// 基本2D図形。解答側では型引数を書かず make_* で作る。
template <class A, class B = A>
struct Line {
    A a;
    B b;
};

// 2Dの直線を一般形 ax + by = c で表す。
template <class T>
struct LineEquation {
    T a;
    T b;
    T c;
};

template <class A, class B = A>
struct Segment {
    A a;
    B b;
};

template <class P, class Radius>
struct Circle {
    P center;
    Radius radius;
};

template <class A, class B = A>
struct Rectangle {
    A corner1;
    B corner2;
};

template <class P, class Side>
struct Square {
    P corner;
    Side side;
};

template <class A, class B = A, class C = A>
struct Triangle {
    A a;
    B b;
    C c;
};

template <class P>
struct Polygon {
    vector<P> vertices;
};

template <class P, class Radius>
struct Sphere {
    P center;
    Radius radius;
};

template <class A, class B = A>
struct Cuboid {
    A corner1;
    B corner2;
};

template <class A, class B, class C, class D>
constexpr bool operator==(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    return lhs.a == rhs.a && lhs.b == rhs.b;
}
template <class A, class B, class C, class D>
constexpr bool operator!=(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    return !(lhs == rhs);
}
template <class A, class B, class C, class D>
constexpr bool operator<(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    if (lhs.a < rhs.a) return true;
    if (rhs.a < lhs.a) return false;
    return lhs.b < rhs.b;
}
template <class A, class B, class C, class D>
constexpr bool operator>(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    return rhs < lhs;
}
template <class A, class B, class C, class D>
constexpr bool operator<=(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    return !(rhs < lhs);
}
template <class A, class B, class C, class D>
constexpr bool operator>=(const Line<A, B>& lhs, const Line<C, D>& rhs) {
    return !(lhs < rhs);
}
template <class A, class B, class C>
constexpr Line<A, B>& operator+=(Line<A, B>& lhs, const C& rhs) {
    lhs.a += rhs;
    lhs.b += rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr Line<A, B>& operator-=(Line<A, B>& lhs, const C& rhs) {
    lhs.a -= rhs;
    lhs.b -= rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr auto operator+(const Line<A, B>& lhs, const C& rhs) {
    return Line<decltype(lhs.a + rhs), decltype(lhs.b + rhs)>{lhs.a + rhs, lhs.b + rhs};
}
template <class A, class B, class C>
constexpr auto operator-(const Line<A, B>& lhs, const C& rhs) {
    return Line<decltype(lhs.a - rhs), decltype(lhs.b - rhs)>{lhs.a - rhs, lhs.b - rhs};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator*(const Line<A, B>& lhs, C scalar) {
    return Line<decltype(lhs.a * scalar), decltype(lhs.b * scalar)>{lhs.a * scalar,
                                                                 lhs.b * scalar};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator*(C scalar, const Line<A, B>& rhs) {
    return rhs * scalar;
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator/(const Line<A, B>& lhs, C scalar) {
    assert(scalar != 0);
    return Line<decltype(lhs.a / scalar), decltype(lhs.b / scalar)>{lhs.a / scalar,
                                                                  lhs.b / scalar};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr Line<A, B>& operator*=(Line<A, B>& lhs, C scalar) {
    lhs.a *= scalar;
    lhs.b *= scalar;
    return lhs;
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr Line<A, B>& operator/=(Line<A, B>& lhs, C scalar) {
    assert(scalar != 0);
    lhs.a /= scalar;
    lhs.b /= scalar;
    return lhs;
}

template <class A, class B, class C, class D>
constexpr bool operator==(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    return lhs.a == rhs.a && lhs.b == rhs.b;
}
template <class A, class B, class C, class D>
constexpr bool operator!=(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    return !(lhs == rhs);
}
template <class A, class B, class C, class D>
constexpr bool operator<(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    if (lhs.a < rhs.a) return true;
    if (rhs.a < lhs.a) return false;
    return lhs.b < rhs.b;
}
template <class A, class B, class C, class D>
constexpr bool operator>(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    return rhs < lhs;
}
template <class A, class B, class C, class D>
constexpr bool operator<=(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    return !(rhs < lhs);
}
template <class A, class B, class C, class D>
constexpr bool operator>=(const Segment<A, B>& lhs, const Segment<C, D>& rhs) {
    return !(lhs < rhs);
}
template <class A, class B, class C>
constexpr Segment<A, B>& operator+=(Segment<A, B>& lhs, const C& rhs) {
    lhs.a += rhs;
    lhs.b += rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr Segment<A, B>& operator-=(Segment<A, B>& lhs, const C& rhs) {
    lhs.a -= rhs;
    lhs.b -= rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr auto operator+(const Segment<A, B>& lhs, const C& rhs) {
    return Segment<decltype(lhs.a + rhs), decltype(lhs.b + rhs)>{lhs.a + rhs, lhs.b + rhs};
}
template <class A, class B, class C>
constexpr auto operator-(const Segment<A, B>& lhs, const C& rhs) {
    return Segment<decltype(lhs.a - rhs), decltype(lhs.b - rhs)>{lhs.a - rhs, lhs.b - rhs};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator*(const Segment<A, B>& lhs, C scalar) {
    return Segment<decltype(lhs.a * scalar), decltype(lhs.b * scalar)>{lhs.a * scalar,
                                                                     lhs.b * scalar};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator*(C scalar, const Segment<A, B>& rhs) {
    return rhs * scalar;
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr auto operator/(const Segment<A, B>& lhs, C scalar) {
    assert(scalar != 0);
    return Segment<decltype(lhs.a / scalar), decltype(lhs.b / scalar)>{lhs.a / scalar,
                                                                     lhs.b / scalar};
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr Segment<A, B>& operator*=(Segment<A, B>& lhs, C scalar) {
    lhs.a *= scalar;
    lhs.b *= scalar;
    return lhs;
}
template <class A, class B, class C, enable_if_t<is_arithmetic_v<C>, int> = 0>
constexpr Segment<A, B>& operator/=(Segment<A, B>& lhs, C scalar) {
    assert(scalar != 0);
    lhs.a /= scalar;
    lhs.b /= scalar;
    return lhs;
}

template <class T, class U>
constexpr bool operator==(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c;
}
template <class T, class U>
constexpr bool operator!=(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    return !(lhs == rhs);
}
template <class T, class U>
constexpr bool operator<(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    if (lhs.a < rhs.a) return true;
    if (rhs.a < lhs.a) return false;
    if (lhs.b < rhs.b) return true;
    if (rhs.b < lhs.b) return false;
    return lhs.c < rhs.c;
}
template <class T, class U>
constexpr bool operator>(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    return rhs < lhs;
}
template <class T, class U>
constexpr bool operator<=(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    return !(rhs < lhs);
}
template <class T, class U>
constexpr bool operator>=(const LineEquation<T>& lhs, const LineEquation<U>& rhs) {
    return !(lhs < rhs);
}

template <class P, class R, class C, class S>
constexpr bool operator==(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    return lhs.center == rhs.center && lhs.radius == rhs.radius;
}
template <class P, class R, class C, class S>
constexpr bool operator!=(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    return !(lhs == rhs);
}
template <class P, class R, class C, class S>
constexpr bool operator<(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    if (lhs.center < rhs.center) return true;
    if (rhs.center < lhs.center) return false;
    return lhs.radius < rhs.radius;
}
template <class P, class R, class C, class S>
constexpr bool operator>(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    return rhs < lhs;
}
template <class P, class R, class C, class S>
constexpr bool operator<=(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    return !(rhs < lhs);
}
template <class P, class R, class C, class S>
constexpr bool operator>=(const Circle<P, R>& lhs, const Circle<C, S>& rhs) {
    return !(lhs < rhs);
}
template <class P, class R, class V>
constexpr Circle<P, R>& operator+=(Circle<P, R>& lhs, const V& rhs) {
    lhs.center += rhs;
    return lhs;
}
template <class P, class R, class V>
constexpr Circle<P, R>& operator-=(Circle<P, R>& lhs, const V& rhs) {
    lhs.center -= rhs;
    return lhs;
}
template <class P, class R, class V>
constexpr auto operator+(const Circle<P, R>& lhs, const V& rhs) {
    return Circle<decltype(lhs.center + rhs), R>{lhs.center + rhs, lhs.radius};
}
template <class P, class R, class V>
constexpr auto operator-(const Circle<P, R>& lhs, const V& rhs) {
    return Circle<decltype(lhs.center - rhs), R>{lhs.center - rhs, lhs.radius};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Circle<P, R>& lhs, S scalar) {
    return Circle<decltype(lhs.center * scalar), decltype(lhs.radius * scalar)>{
        lhs.center * scalar, lhs.radius * scalar};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Circle<P, R>& rhs) {
    return rhs * scalar;
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Circle<P, R>& lhs, S scalar) {
    assert(scalar != 0);
    return Circle<decltype(lhs.center / scalar), decltype(lhs.radius / scalar)>{
        lhs.center / scalar, lhs.radius / scalar};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Circle<P, R>& operator*=(Circle<P, R>& lhs, S scalar) {
    lhs.center *= scalar;
    lhs.radius *= scalar;
    return lhs;
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Circle<P, R>& operator/=(Circle<P, R>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.center /= scalar;
    lhs.radius /= scalar;
    return lhs;
}

template <class A, class B, class C, class D>
constexpr bool operator==(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    return lhs.corner1 == rhs.corner1 && lhs.corner2 == rhs.corner2;
}
template <class A, class B, class C, class D>
constexpr bool operator!=(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    return !(lhs == rhs);
}
template <class A, class B, class C, class D>
constexpr bool operator<(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    if (lhs.corner1 < rhs.corner1) return true;
    if (rhs.corner1 < lhs.corner1) return false;
    return lhs.corner2 < rhs.corner2;
}
template <class A, class B, class C, class D>
constexpr bool operator>(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    return rhs < lhs;
}
template <class A, class B, class C, class D>
constexpr bool operator<=(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    return !(rhs < lhs);
}
template <class A, class B, class C, class D>
constexpr bool operator>=(const Rectangle<A, B>& lhs, const Rectangle<C, D>& rhs) {
    return !(lhs < rhs);
}
template <class A, class B, class C>
constexpr Rectangle<A, B>& operator+=(Rectangle<A, B>& lhs, const C& rhs) {
    lhs.corner1 += rhs;
    lhs.corner2 += rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr Rectangle<A, B>& operator-=(Rectangle<A, B>& lhs, const C& rhs) {
    lhs.corner1 -= rhs;
    lhs.corner2 -= rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr auto operator+(const Rectangle<A, B>& lhs, const C& rhs) {
    return Rectangle<decltype(lhs.corner1 + rhs), decltype(lhs.corner2 + rhs)>{
        lhs.corner1 + rhs, lhs.corner2 + rhs};
}
template <class A, class B, class C>
constexpr auto operator-(const Rectangle<A, B>& lhs, const C& rhs) {
    return Rectangle<decltype(lhs.corner1 - rhs), decltype(lhs.corner2 - rhs)>{
        lhs.corner1 - rhs, lhs.corner2 - rhs};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Rectangle<A, B>& lhs, S scalar) {
    return Rectangle<decltype(lhs.corner1 * scalar), decltype(lhs.corner2 * scalar)>{
        lhs.corner1 * scalar, lhs.corner2 * scalar};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Rectangle<A, B>& rhs) {
    return rhs * scalar;
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Rectangle<A, B>& lhs, S scalar) {
    assert(scalar != 0);
    return Rectangle<decltype(lhs.corner1 / scalar), decltype(lhs.corner2 / scalar)>{
        lhs.corner1 / scalar, lhs.corner2 / scalar};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Rectangle<A, B>& operator*=(Rectangle<A, B>& lhs, S scalar) {
    lhs.corner1 *= scalar;
    lhs.corner2 *= scalar;
    return lhs;
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Rectangle<A, B>& operator/=(Rectangle<A, B>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.corner1 /= scalar;
    lhs.corner2 /= scalar;
    return lhs;
}

template <class P, class Side, class C, class Side2>
constexpr bool operator==(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    return lhs.corner == rhs.corner && lhs.side == rhs.side;
}
template <class P, class Side, class C, class Side2>
constexpr bool operator!=(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    return !(lhs == rhs);
}
template <class P, class Side, class C, class Side2>
constexpr bool operator<(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    if (lhs.corner < rhs.corner) return true;
    if (rhs.corner < lhs.corner) return false;
    return lhs.side < rhs.side;
}
template <class P, class Side, class C, class Side2>
constexpr bool operator>(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    return rhs < lhs;
}
template <class P, class Side, class C, class Side2>
constexpr bool operator<=(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    return !(rhs < lhs);
}
template <class P, class Side, class C, class Side2>
constexpr bool operator>=(const Square<P, Side>& lhs, const Square<C, Side2>& rhs) {
    return !(lhs < rhs);
}
template <class P, class Side, class C>
constexpr Square<P, Side>& operator+=(Square<P, Side>& lhs, const C& rhs) {
    lhs.corner += rhs;
    return lhs;
}
template <class P, class Side, class C>
constexpr Square<P, Side>& operator-=(Square<P, Side>& lhs, const C& rhs) {
    lhs.corner -= rhs;
    return lhs;
}
template <class P, class Side, class C>
constexpr auto operator+(const Square<P, Side>& lhs, const C& rhs) {
    return Square<decltype(lhs.corner + rhs), Side>{lhs.corner + rhs, lhs.side};
}
template <class P, class Side, class C>
constexpr auto operator-(const Square<P, Side>& lhs, const C& rhs) {
    return Square<decltype(lhs.corner - rhs), Side>{lhs.corner - rhs, lhs.side};
}
template <class P, class Side, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Square<P, Side>& lhs, S scalar) {
    return Square<decltype(lhs.corner * scalar), decltype(lhs.side * scalar)>{
        lhs.corner * scalar, lhs.side * scalar};
}
template <class P, class Side, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Square<P, Side>& rhs) {
    return rhs * scalar;
}
template <class P, class Side, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Square<P, Side>& lhs, S scalar) {
    assert(scalar != 0);
    return Square<decltype(lhs.corner / scalar), decltype(lhs.side / scalar)>{
        lhs.corner / scalar, lhs.side / scalar};
}
template <class P, class Side, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Square<P, Side>& operator*=(Square<P, Side>& lhs, S scalar) {
    lhs.corner *= scalar;
    lhs.side *= scalar;
    return lhs;
}
template <class P, class Side, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Square<P, Side>& operator/=(Square<P, Side>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.corner /= scalar;
    lhs.side /= scalar;
    return lhs;
}

template <class A, class B, class C, class D, class E, class F>
constexpr bool operator==(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c;
}
template <class A, class B, class C, class D, class E, class F>
constexpr bool operator!=(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    return !(lhs == rhs);
}
template <class A, class B, class C, class D, class E, class F>
constexpr bool operator<(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    if (lhs.a < rhs.a) return true;
    if (rhs.a < lhs.a) return false;
    if (lhs.b < rhs.b) return true;
    if (rhs.b < lhs.b) return false;
    return lhs.c < rhs.c;
}
template <class A, class B, class C, class D, class E, class F>
constexpr bool operator>(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    return rhs < lhs;
}
template <class A, class B, class C, class D, class E, class F>
constexpr bool operator<=(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    return !(rhs < lhs);
}
template <class A, class B, class C, class D, class E, class F>
constexpr bool operator>=(const Triangle<A, B, C>& lhs, const Triangle<D, E, F>& rhs) {
    return !(lhs < rhs);
}
template <class A, class B, class C, class D, class E, class F>
constexpr Triangle<A, B, C>& operator+=(Triangle<A, B, C>& lhs, const D& rhs) {
    lhs.a += rhs;
    lhs.b += rhs;
    lhs.c += rhs;
    return lhs;
}
template <class A, class B, class C, class D, class E, class F>
constexpr Triangle<A, B, C>& operator-=(Triangle<A, B, C>& lhs, const D& rhs) {
    lhs.a -= rhs;
    lhs.b -= rhs;
    lhs.c -= rhs;
    return lhs;
}
template <class A, class B, class C, class D, class E, class F>
constexpr auto operator+(const Triangle<A, B, C>& lhs, const D& rhs) {
    return Triangle<decltype(lhs.a + rhs), decltype(lhs.b + rhs), decltype(lhs.c + rhs)>{
        lhs.a + rhs, lhs.b + rhs, lhs.c + rhs};
}
template <class A, class B, class C, class D, class E, class F>
constexpr auto operator-(const Triangle<A, B, C>& lhs, const D& rhs) {
    return Triangle<decltype(lhs.a - rhs), decltype(lhs.b - rhs), decltype(lhs.c - rhs)>{
        lhs.a - rhs, lhs.b - rhs, lhs.c - rhs};
}
template <class A, class B, class C, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Triangle<A, B, C>& lhs, S scalar) {
    return Triangle<decltype(lhs.a * scalar), decltype(lhs.b * scalar),
                    decltype(lhs.c * scalar)>{lhs.a * scalar, lhs.b * scalar,
                                              lhs.c * scalar};
}
template <class A, class B, class C, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Triangle<A, B, C>& rhs) {
    return rhs * scalar;
}
template <class A, class B, class C, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Triangle<A, B, C>& lhs, S scalar) {
    assert(scalar != 0);
    return Triangle<decltype(lhs.a / scalar), decltype(lhs.b / scalar),
                    decltype(lhs.c / scalar)>{lhs.a / scalar, lhs.b / scalar,
                                              lhs.c / scalar};
}
template <class A, class B, class C, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Triangle<A, B, C>& operator*=(Triangle<A, B, C>& lhs, S scalar) {
    lhs.a *= scalar;
    lhs.b *= scalar;
    lhs.c *= scalar;
    return lhs;
}
template <class A, class B, class C, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Triangle<A, B, C>& operator/=(Triangle<A, B, C>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.a /= scalar;
    lhs.b /= scalar;
    lhs.c /= scalar;
    return lhs;
}

template <class P, class Q>
constexpr bool operator==(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    if (lhs.vertices.size() != rhs.vertices.size()) return false;
    for (size_t i = 0; i < lhs.vertices.size(); ++i) {
        if (!(lhs.vertices[i] == rhs.vertices[i])) return false;
    }
    return true;
}
template <class P>
constexpr bool operator!=(const Polygon<P>& lhs, const Polygon<P>& rhs) {
    return lhs.vertices == rhs.vertices;
}
template <class P, class Q>
constexpr bool operator!=(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    return !(lhs == rhs);
}
template <class P, class Q>
constexpr bool operator<(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    const size_t min_size = min(lhs.vertices.size(), rhs.vertices.size());
    for (size_t i = 0; i < min_size; ++i) {
        if (lhs.vertices[i] < rhs.vertices[i]) return true;
        if (rhs.vertices[i] < lhs.vertices[i]) return false;
    }
    return lhs.vertices.size() < rhs.vertices.size();
}
template <class P, class Q>
constexpr bool operator>(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    return rhs < lhs;
}
template <class P, class Q>
constexpr bool operator<=(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    return !(rhs < lhs);
}
template <class P, class Q>
constexpr bool operator>=(const Polygon<P>& lhs, const Polygon<Q>& rhs) {
    return !(lhs < rhs);
}
template <class P, class V>
constexpr Polygon<P>& operator+=(Polygon<P>& lhs, const V& rhs) {
    for (auto& p : lhs.vertices) p += rhs;
    return lhs;
}
template <class P, class V>
constexpr Polygon<P>& operator-=(Polygon<P>& lhs, const V& rhs) {
    for (auto& p : lhs.vertices) p -= rhs;
    return lhs;
}
template <class P, class V>
constexpr auto operator+(const Polygon<P>& lhs, const V& rhs) {
    auto result = lhs;
    result += rhs;
    return result;
}
template <class P, class V>
constexpr auto operator-(const Polygon<P>& lhs, const V& rhs) {
    auto result = lhs;
    result -= rhs;
    return result;
}
template <class P, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Polygon<P>& lhs, S scalar) {
    using R = common_type_t<P, S>;
    Polygon<R> result;
    result.vertices.reserve(lhs.vertices.size());
    for (const auto& point : lhs.vertices) result.vertices.push_back(point * scalar);
    return result;
}
template <class P, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Polygon<P>& rhs) {
    return rhs * scalar;
}
template <class P, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Polygon<P>& lhs, S scalar) {
    assert(scalar != 0);
    using R = common_type_t<P, S>;
    Polygon<R> result;
    result.vertices.reserve(lhs.vertices.size());
    for (const auto& point : lhs.vertices) result.vertices.push_back(point / scalar);
    return result;
}
template <class P, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Polygon<P>& operator*=(Polygon<P>& lhs, S scalar) {
    for (auto& p : lhs.vertices) p *= scalar;
    return lhs;
}
template <class P, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Polygon<P>& operator/=(Polygon<P>& lhs, S scalar) {
    assert(scalar != 0);
    for (auto& p : lhs.vertices) p /= scalar;
    return lhs;
}

template <class P, class R, class C, class S>
constexpr bool operator==(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    return lhs.center == rhs.center && lhs.radius == rhs.radius;
}
template <class P, class R, class C, class S>
constexpr bool operator!=(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    return !(lhs == rhs);
}
template <class P, class R, class C, class S>
constexpr bool operator<(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    if (lhs.center < rhs.center) return true;
    if (rhs.center < lhs.center) return false;
    return lhs.radius < rhs.radius;
}
template <class P, class R, class C, class S>
constexpr bool operator>(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    return rhs < lhs;
}
template <class P, class R, class C, class S>
constexpr bool operator<=(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    return !(rhs < lhs);
}
template <class P, class R, class C, class S>
constexpr bool operator>=(const Sphere<P, R>& lhs, const Sphere<C, S>& rhs) {
    return !(lhs < rhs);
}
template <class P, class R, class V>
constexpr Sphere<P, R>& operator+=(Sphere<P, R>& lhs, const V& rhs) {
    lhs.center += rhs;
    return lhs;
}
template <class P, class R, class V>
constexpr Sphere<P, R>& operator-=(Sphere<P, R>& lhs, const V& rhs) {
    lhs.center -= rhs;
    return lhs;
}
template <class P, class R, class V>
constexpr auto operator+(const Sphere<P, R>& lhs, const V& rhs) {
    return Sphere<decltype(lhs.center + rhs), R>{lhs.center + rhs, lhs.radius};
}
template <class P, class R, class V>
constexpr auto operator-(const Sphere<P, R>& lhs, const V& rhs) {
    return Sphere<decltype(lhs.center - rhs), R>{lhs.center - rhs, lhs.radius};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Sphere<P, R>& lhs, S scalar) {
    return Sphere<decltype(lhs.center * scalar), decltype(lhs.radius * scalar)>{
        lhs.center * scalar, lhs.radius * scalar};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Sphere<P, R>& rhs) {
    return rhs * scalar;
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Sphere<P, R>& lhs, S scalar) {
    assert(scalar != 0);
    return Sphere<decltype(lhs.center / scalar), decltype(lhs.radius / scalar)>{
        lhs.center / scalar, lhs.radius / scalar};
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Sphere<P, R>& operator*=(Sphere<P, R>& lhs, S scalar) {
    lhs.center *= scalar;
    lhs.radius *= scalar;
    return lhs;
}
template <class P, class R, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Sphere<P, R>& operator/=(Sphere<P, R>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.center /= scalar;
    lhs.radius /= scalar;
    return lhs;
}

template <class A, class B, class C, class D>
constexpr bool operator==(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    return lhs.corner1 == rhs.corner1 && lhs.corner2 == rhs.corner2;
}
template <class A, class B, class C, class D>
constexpr bool operator!=(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    return !(lhs == rhs);
}
template <class A, class B, class C, class D>
constexpr bool operator<(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    if (lhs.corner1 < rhs.corner1) return true;
    if (rhs.corner1 < lhs.corner1) return false;
    return lhs.corner2 < rhs.corner2;
}
template <class A, class B, class C, class D>
constexpr bool operator>(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    return rhs < lhs;
}
template <class A, class B, class C, class D>
constexpr bool operator<=(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    return !(rhs < lhs);
}
template <class A, class B, class C, class D>
constexpr bool operator>=(const Cuboid<A, B>& lhs, const Cuboid<C, D>& rhs) {
    return !(lhs < rhs);
}
template <class A, class B, class C>
constexpr Cuboid<A, B>& operator+=(Cuboid<A, B>& lhs, const C& rhs) {
    lhs.corner1 += rhs;
    lhs.corner2 += rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr Cuboid<A, B>& operator-=(Cuboid<A, B>& lhs, const C& rhs) {
    lhs.corner1 -= rhs;
    lhs.corner2 -= rhs;
    return lhs;
}
template <class A, class B, class C>
constexpr auto operator+(const Cuboid<A, B>& lhs, const C& rhs) {
    return Cuboid<decltype(lhs.corner1 + rhs), decltype(lhs.corner2 + rhs)>{
        lhs.corner1 + rhs, lhs.corner2 + rhs};
}
template <class A, class B, class C>
constexpr auto operator-(const Cuboid<A, B>& lhs, const C& rhs) {
    return Cuboid<decltype(lhs.corner1 - rhs), decltype(lhs.corner2 - rhs)>{
        lhs.corner1 - rhs, lhs.corner2 - rhs};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(const Cuboid<A, B>& lhs, S scalar) {
    return Cuboid<decltype(lhs.corner1 * scalar), decltype(lhs.corner2 * scalar)>{
        lhs.corner1 * scalar, lhs.corner2 * scalar};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator*(S scalar, const Cuboid<A, B>& rhs) {
    return rhs * scalar;
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr auto operator/(const Cuboid<A, B>& lhs, S scalar) {
    assert(scalar != 0);
    return Cuboid<decltype(lhs.corner1 / scalar), decltype(lhs.corner2 / scalar)>{
        lhs.corner1 / scalar, lhs.corner2 / scalar};
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Cuboid<A, B>& operator*=(Cuboid<A, B>& lhs, S scalar) {
    lhs.corner1 *= scalar;
    lhs.corner2 *= scalar;
    return lhs;
}
template <class A, class B, class S, enable_if_t<is_arithmetic_v<S>, int> = 0>
constexpr Cuboid<A, B>& operator/=(Cuboid<A, B>& lhs, S scalar) {
    assert(scalar != 0);
    lhs.corner1 /= scalar;
    lhs.corner2 /= scalar;
    return lhs;
}
// @acf helper operator<< — 幾何図形（Line / LineEquation / Segment / Circle など）の構造化出力
/// @acf-doc @brief Line・LineEquation・Segment・Circle・Rectangle・Square・Triangle・Polygon・Sphere・Cuboid を構造化して出力。`print` からも使える。Point は従来どおり座標のみ（`x y`）
/// @acf-doc [helper] operator<<
template <class A, class B>
ostream& operator<<(ostream& os, const Line<A, B>& line) {
    return os << "Line(" << line.a << ", " << line.b << ')';
}
template <class T>
ostream& operator<<(ostream& os, const LineEquation<T>& line) {
    return os << "LineEquation(" << line.a << ", " << line.b << ", "
              << line.c << ')';
}
template <class A, class B>
ostream& operator<<(ostream& os, const Segment<A, B>& segment) {
    return os << "Segment(" << segment.a << ", " << segment.b << ')';
}
template <class P, class Radius>
ostream& operator<<(ostream& os, const Circle<P, Radius>& circle) {
    return os << "Circle(" << circle.center << ", " << circle.radius << ')';
}
template <class A, class B>
ostream& operator<<(ostream& os, const Rectangle<A, B>& rectangle) {
    return os << "Rectangle(" << rectangle.corner1 << ", " << rectangle.corner2 << ')';
}
template <class P, class Side>
ostream& operator<<(ostream& os, const Square<P, Side>& square) {
    return os << "Square(" << square.corner << ", " << square.side << ')';
}
template <class A, class B, class C>
ostream& operator<<(ostream& os, const Triangle<A, B, C>& triangle) {
    return os << "Triangle(" << triangle.a << ", " << triangle.b << ", " << triangle.c
              << ')';
}
template <class P>
ostream& operator<<(ostream& os, const Polygon<P>& polygon) {
    os << "Polygon(";
    bool first = true;
    for (const auto& vertex : polygon.vertices) {
        if (!first) os << "; ";
        first = false;
        os << vertex;
    }
    return os << ')';
}
template <class P, class Radius>
ostream& operator<<(ostream& os, const Sphere<P, Radius>& sphere) {
    return os << "Sphere(" << sphere.center << ", " << sphere.radius << ')';
}
template <class A, class B>
ostream& operator<<(ostream& os, const Cuboid<A, B>& cuboid) {
    return os << "Cuboid(" << cuboid.corner1 << ", " << cuboid.corner2 << ')';
}

// @acf fn make_line — (a,b) / (x1,y1,x2,y2) / (x1,y1,z1,x2,y2,z2) -> Line — 2点を通る無限直線
/// @acf-doc @brief 2点を通る無限直線。点コンテナ、2Dは4引数、3Dは6引数の生座標で渡せる
/// @acf-doc [function] make_line
template <class A, class B>
constexpr auto make_line(A a, B b) {
    return Line<A, B>{std::move(a), std::move(b)};
}

template <class X1, class Y1, class X2, class Y2>
constexpr auto make_line(X1 x1, Y1 y1, X2 x2, Y2 y2) {
    return make_line(pair<X1, Y1>{x1, y1}, pair<X2, Y2>{x2, y2});
}

template <class X1, class Y1, class Z1, class X2, class Y2, class Z2>
constexpr auto make_line(X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) {
    return make_line(tuple{x1, y1, z1}, tuple{x2, y2, z2});
}

// @acf fn make_line_equation — (a,b,c) -> LineEquation — ax+by=c の直線
/// @acf-doc @brief 一般形 `ax+by=c` の2D直線を作る。`a` と `b` は同時に0でない
/// @acf-doc [function] make_line_equation
template <class A, class B, class C>
constexpr auto make_line_equation(A a, B b, C c) {
    using T = common_type_t<A, B, C>;
    const T aa = static_cast<T>(a);
    const T bb = static_cast<T>(b);
    assert(aa != 0 || bb != 0);
    return LineEquation<T>{aa, bb, static_cast<T>(c)};
}

// @acf fn make_segment — (a,b) / (x1,y1,x2,y2) / (x1,y1,z1,x2,y2,z2) -> Segment — 2点を端点とする線分
/// @acf-doc @brief 2点を端点とする線分。点コンテナ、2Dは4引数、3Dは6引数の生座標で渡せる
/// @acf-doc [function] make_segment
template <class A, class B>
constexpr auto make_segment(A a, B b) {
    return Segment<A, B>{std::move(a), std::move(b)};
}

template <class X1, class Y1, class X2, class Y2>
constexpr auto make_segment(X1 x1, Y1 y1, X2 x2, Y2 y2) {
    return make_segment(pair<X1, Y1>{x1, y1}, pair<X2, Y2>{x2, y2});
}

template <class X1, class Y1, class Z1, class X2, class Y2, class Z2>
constexpr auto make_segment(X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) {
    return make_segment(tuple{x1, y1, z1}, tuple{x2, y2, z2});
}

// @acf fn make_circle — (center,r) / (x,y,r) -> Circle — 中心と半径から円を作る
/// @acf-doc @brief `make_circle(center,r)` は中心と半径から円を作る。3引数版は `x,y,r`。半径は0以上
/// @acf-doc [function] make_circle
template <class P, class Radius>
constexpr auto make_circle(P center, Radius radius) {
    assert(radius >= 0);
    return Circle<P, Radius>{std::move(center), radius};
}

template <class X, class Y, class Radius>
constexpr auto make_circle(X x, Y y, Radius radius) {
    return make_circle(pair<X, Y>{x, y}, radius);
}

// @acf fn make_rectangle — (corner1,corner2) / (x1,y1,x2,y2) -> Rectangle — 座標軸に平行な長方形
/// @acf-doc @brief `make_rectangle(a,b)` は対角の2点から座標軸に平行な長方形を作る。点の順序は自由
/// @acf-doc [function] make_rectangle
template <class A, class B>
constexpr auto make_rectangle(A corner1, B corner2) {
    return Rectangle<A, B>{std::move(corner1), std::move(corner2)};
}

template <class X1, class Y1, class X2, class Y2>
constexpr auto make_rectangle(X1 x1, Y1 y1, X2 x2, Y2 y2) {
    return make_rectangle(pair<X1, Y1>{x1, y1}, pair<X2, Y2>{x2, y2});
}

// @acf fn make_square — (corner,side) / (x,y,side) -> Square — 座標軸に平行な正方形
/// @acf-doc @brief `make_square(corner,side)` は左下の角と辺長から座標軸に平行な正方形を作る。辺長は0以上
/// @acf-doc [function] make_square
template <class P, class Side>
constexpr auto make_square(P corner, Side side) {
    assert(side >= 0);
    return Square<P, Side>{std::move(corner), side};
}

template <class X, class Y, class Side>
constexpr auto make_square(X x, Y y, Side side) {
    return make_square(pair<X, Y>{x, y}, side);
}

// @acf fn make_triangle — (a,b,c) / (x1,y1,x2,y2,x3,y3) -> Triangle — 3頂点から三角形を作る
/// @acf-doc @brief `make_triangle(a,b,c)` は3頂点から三角形を作る。点は Point・pair・tuple・vector・array、または6個の座標で渡せる
/// @acf-doc [function] make_triangle
template <class A, class B, class C>
constexpr auto make_triangle(A a, B b, C c) {
    return Triangle<A, B, C>{std::move(a), std::move(b), std::move(c)};
}

template <class X1, class Y1, class X2, class Y2, class X3, class Y3>
constexpr auto make_triangle(X1 x1, Y1 y1, X2 x2, Y2 y2, X3 x3, Y3 y3) {
    return make_triangle(pair<X1, Y1>{x1, y1}, pair<X2, Y2>{x2, y2},
                         pair<X3, Y3>{x3, y3});
}

// @acf fn make_polygon — (vector<Point>) -> Polygon — 頂点列から多角形を作る
/// @acf-doc @brief `make_polygon(points)` は周回順の頂点列から多角形を作る。凸・凹のどちらにも対応
/// @acf-doc [function] make_polygon
template <class P>
auto make_polygon(vector<P> points) {
    return Polygon<P>{std::move(points)};
}

// @acf fn make_sphere — (center,r) / (x,y,z,r) -> Sphere — 中心と半径から球を作る
/// @acf-doc @brief `make_sphere(center,r)` は3次元の球を作る。半径は0以上
/// @acf-doc [function] make_sphere
template <class P, class Radius>
constexpr auto make_sphere(P center, Radius radius) {
    assert(radius >= 0);
    return Sphere<P, Radius>{std::move(center), radius};
}

template <class X, class Y, class Z, class Radius>
constexpr auto make_sphere(X x, Y y, Z z, Radius radius) {
    return make_sphere(Point3<common_type_t<X, Y, Z>>{x, y, z}, radius);
}

// @acf fn make_cuboid — (corner1,corner2) / (x1,y1,z1,x2,y2,z2) -> Cuboid — 対角2点から直方体を作る
/// @acf-doc @brief `make_cuboid(a,b)` は対角の2点から座標軸に平行な直方体を作る。点の順序は自由
/// @acf-doc [function] make_cuboid
template <class A, class B>
constexpr auto make_cuboid(A corner1, B corner2) {
    return Cuboid<A, B>{std::move(corner1), std::move(corner2)};
}

template <class X1, class Y1, class Z1, class X2, class Y2, class Z2>
constexpr auto make_cuboid(X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) {
    using R = common_type_t<X1, Y1, Z1, X2, Y2, Z2>;
    return make_cuboid(Point3<R>{x1, y1, z1}, Point3<R>{x2, y2, z2});
}

// ---------------------------------------------------------------------------
// Geometry input — using → in_* → UPPER(name[, n])（コア入力と同じ三段）
// フラット数値（構造化 print 形式ではない）。Point は range の in でも可。
// ---------------------------------------------------------------------------

// @acf using linell — Line<pll>
/// @acf-doc [type] linell
using linell = Line<pll>;
// @acf using segll — Segment<pll>
/// @acf-doc [type] segll
using segll = Segment<pll>;
// @acf using cirll — Circle<pll,ll>
/// @acf-doc [type] cirll
using cirll = Circle<pll, ll>;
// @acf using rectll — Rectangle<pll>
/// @acf-doc [type] rectll
using rectll = Rectangle<pll>;
// @acf using sqll — Square<pll,ll>
/// @acf-doc [type] sqll
using sqll = Square<pll, ll>;
// @acf using trill — Triangle<pll>
/// @acf-doc [type] trill
using trill = Triangle<pll>;
// @acf using polyll — Polygon<p2ll>
/// @acf-doc [type] polyll
using polyll = Polygon<p2ll>;
// @acf using sphll — Sphere<p3ll,ll>
/// @acf-doc [type] sphll
using sphll = Sphere<p3ll, ll>;
// @acf using cubll — Cuboid<p3ll>
/// @acf-doc [type] cubll
using cubll = Cuboid<p3ll>;

// @acf helper operator>> — 幾何図形のフラット入力
/// @acf-doc @brief Line/Segment は2点、Circle は中心+半径、Rectangle は対角2点、Square は角+辺、Triangle は3点、Polygon は事前サイズの頂点列、Sphere は中心+半径、Cuboid は対角2点。`in` / `rd` / `in_*` からも使える
/// @acf-doc [helper] operator>>
template <class A, class B>
istream& operator>>(istream& is, Line<A, B>& line) {
    return is >> line.a >> line.b;
}
template <class T>
istream& operator>>(istream& is, LineEquation<T>& line) {
    return is >> line.a >> line.b >> line.c;
}
template <class A, class B>
istream& operator>>(istream& is, Segment<A, B>& segment) {
    return is >> segment.a >> segment.b;
}
template <class P, class Radius>
istream& operator>>(istream& is, Circle<P, Radius>& circle) {
    return is >> circle.center >> circle.radius;
}
template <class A, class B>
istream& operator>>(istream& is, Rectangle<A, B>& rectangle) {
    return is >> rectangle.corner1 >> rectangle.corner2;
}
template <class P, class Side>
istream& operator>>(istream& is, Square<P, Side>& square) {
    return is >> square.corner >> square.side;
}
template <class A, class B, class C>
istream& operator>>(istream& is, Triangle<A, B, C>& triangle) {
    return is >> triangle.a >> triangle.b >> triangle.c;
}
template <class P>
istream& operator>>(istream& is, Polygon<P>& polygon) {
    for (auto& vertex : polygon.vertices) is >> vertex;
    return is;
}
template <class P, class Radius>
istream& operator>>(istream& is, Sphere<P, Radius>& sphere) {
    return is >> sphere.center >> sphere.radius;
}
template <class A, class B>
istream& operator>>(istream& is, Cuboid<A, B>& cuboid) {
    return is >> cuboid.corner1 >> cuboid.corner2;
}

namespace ac_detail::geometry {
template <class T, size_t N>
constexpr long double x_ld(const Point<T, N>& point) {
    static_assert(N >= 2, "2D geometry requires at least two dimensions");
    return static_cast<long double>(point[0]);
}
template <class T, size_t N>
constexpr long double y_ld(const Point<T, N>& point) {
    static_assert(N >= 2, "2D geometry requires at least two dimensions");
    return static_cast<long double>(point[1]);
}
template <class T, size_t N>
constexpr long double z_ld(const Point<T, N>& point) {
    static_assert(N >= 3, "3D geometry requires at least three dimensions");
    return static_cast<long double>(point[2]);
}
template <class A, class B>
constexpr long double x_ld(const pair<A, B>& p) {
    return static_cast<long double>(p.first);
}
template <class A, class B>
constexpr long double y_ld(const pair<A, B>& p) {
    return static_cast<long double>(p.second);
}
template <class... T>
constexpr long double x_ld(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 2);
    return static_cast<long double>(get<0>(p));
}
template <class... T>
constexpr long double y_ld(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 2);
    return static_cast<long double>(get<1>(p));
}
template <class... T>
constexpr long double z_ld(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 3);
    return static_cast<long double>(get<2>(p));
}
template <class T, size_t N>
constexpr long double x_ld(const array<T, N>& p) {
    static_assert(N >= 2);
    return static_cast<long double>(p[0]);
}
template <class T, size_t N>
constexpr long double y_ld(const array<T, N>& p) {
    static_assert(N >= 2);
    return static_cast<long double>(p[1]);
}
template <class T, size_t N>
constexpr long double z_ld(const array<T, N>& p) {
    static_assert(N >= 3);
    return static_cast<long double>(p[2]);
}
template <class T>
long double x_ld(const vector<T>& p) {
    assert(p.size() >= 2);
    return static_cast<long double>(p[0]);
}
template <class T>
long double y_ld(const vector<T>& p) {
    assert(p.size() >= 2);
    return static_cast<long double>(p[1]);
}
template <class T>
long double z_ld(const vector<T>& p) {
    assert(p.size() >= 3);
    return static_cast<long double>(p[2]);
}

template <class P>
constexpr auto point2_ld(const P& p) {
    return Point2<long double>{x_ld(p), y_ld(p)};
}

template <class P>
constexpr auto point3_ld(const P& p) {
    return Point3<long double>{x_ld(p), y_ld(p), z_ld(p)};
}

inline long double cross2d_ld(const Point2<long double>& a,
                              const Point2<long double>& b) {
    return a.x() * b.y() - a.y() * b.x();
}

inline long double cross2d_ld(const Point2<long double>& o,
                              const Point2<long double>& a,
                              const Point2<long double>& b) {
    return cross2d_ld(a - o, b - o);
}

template <class A, class B>
long double distance2d(const A& a, const B& b) {
    return hypot(x_ld(a) - x_ld(b), y_ld(a) - y_ld(b));
}

template <class P, class A, class B>
long double segment_distance2d(const P& p, const A& a, const B& b) {
    return point_segment_distance(x_ld(p), y_ld(p), x_ld(a), y_ld(a),
                                  x_ld(b), y_ld(b));
}

inline long double tolerance(long double scale) {
    return EPS * max(1.0L, scale);
}

template <class P, class A, class B>
bool on_segment2d(const P& p, const A& a, const B& b);

template <class T, size_t N>
constexpr auto x_val(const Point<T, N>& point) {
    static_assert(N >= 2, "2D geometry requires at least two dimensions");
    return point[0];
}
template <class T, size_t N>
constexpr auto y_val(const Point<T, N>& point) {
    static_assert(N >= 2, "2D geometry requires at least two dimensions");
    return point[1];
}
template <class A, class B>
constexpr auto x_val(const pair<A, B>& p) {
    return p.first;
}
template <class A, class B>
constexpr auto y_val(const pair<A, B>& p) {
    return p.second;
}
template <class T, size_t N>
constexpr auto z_val(const Point<T, N>& point) {
    static_assert(N >= 3, "3D geometry requires at least three dimensions");
    return point[2];
}
template <class... T>
constexpr auto z_val(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 3);
    return get<2>(p);
}
template <class T, size_t N>
constexpr auto z_val(const array<T, N>& p) {
    static_assert(N >= 3);
    return p[2];
}
template <class T>
auto z_val(const vector<T>& p) {
    assert(p.size() >= 3);
    return p[2];
}
template <class... T>
constexpr auto x_val(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 2);
    return get<0>(p);
}
template <class... T>
constexpr auto y_val(const tuple<T...>& p) {
    static_assert(sizeof...(T) >= 2);
    return get<1>(p);
}
template <class T, size_t N>
constexpr auto x_val(const array<T, N>& p) {
    static_assert(N >= 2);
    return p[0];
}
template <class T, size_t N>
constexpr auto y_val(const array<T, N>& p) {
    static_assert(N >= 2);
    return p[1];
}
template <class T>
auto x_val(const vector<T>& p) {
    assert(p.size() >= 2);
    return p[0];
}
template <class T>
auto y_val(const vector<T>& p) {
    assert(p.size() >= 2);
    return p[1];
}

template <class P>
struct integral_point2 : false_type {};
template <class A, class B>
struct integral_point2<pair<A, B>>
    : bool_constant<is_integral_v<remove_cv_t<A>>
                    && is_integral_v<remove_cv_t<B>>> {};
template <class... T>
struct integral_point2<tuple<T...>>
    : bool_constant<(sizeof...(T) >= 2)
                    && (is_integral_v<remove_cv_t<T>> && ...)> {};
template <class T, size_t N>
struct integral_point2<Point<T, N>>
    : bool_constant<(N >= 2) && is_integral_v<remove_cv_t<T>>> {};
template <class T, size_t N>
struct integral_point2<array<T, N>>
    : bool_constant<(N >= 2) && is_integral_v<remove_cv_t<T>>> {};
template <class T>
struct integral_point2<vector<T>>
    : bool_constant<is_integral_v<remove_cv_t<T>>> {};

template <class... Ps>
constexpr bool integral_points_v =
    (integral_point2<remove_cv_t<remove_reference_t<Ps>>>::value && ...);

template <class A, class B>
constexpr auto cross_wide(const A& u, const B& v) {
    return wide_mul(x_val(u), y_val(v)) - wide_mul(y_val(u), x_val(v));
}

template <class A, class B>
constexpr auto dot_wide(const A& u, const B& v) {
    return wide_mul(x_val(u), x_val(v)) + wide_mul(y_val(u), y_val(v));
}

template <class A, class B, class C>
constexpr int orient_exact(const A& a, const B& b, const C& c) {
    return sgn(wide_mul(x_val(b) - x_val(a), y_val(c) - y_val(a))
             - wide_mul(y_val(b) - y_val(a), x_val(c) - x_val(a)));
}

template <class A, class B>
constexpr bool same_point_exact(const A& a, const B& b) {
    return x_val(a) == x_val(b) && y_val(a) == y_val(b);
}

template <class P, class A, class B>
constexpr bool on_segment_exact(const P& p, const A& a, const B& b) {
    if (orient_exact(a, b, p) != 0) return false;
    return min(x_val(a), x_val(b)) <= x_val(p)
        && x_val(p) <= max(x_val(a), x_val(b))
        && min(y_val(a), y_val(b)) <= y_val(p)
        && y_val(p) <= max(y_val(a), y_val(b));
}

template <class A, class B, class C, class D>
constexpr bool bbox_overlap_exact(const A& a, const B& b,
                                  const C& c, const D& d) {
    return max(min(x_val(a), x_val(b)), min(x_val(c), x_val(d)))
            <= min(max(x_val(a), x_val(b)), max(x_val(c), x_val(d)))
        && max(min(y_val(a), y_val(b)), min(y_val(c), y_val(d)))
            <= min(max(y_val(a), y_val(b)), max(y_val(c), y_val(d)));
}

template <class A, class B, class C, class D>
constexpr bool segments_intersect_exact(const A& a, const B& b,
                                        const C& c, const D& d) {
    const int o1 = orient_exact(a, b, c);
    const int o2 = orient_exact(a, b, d);
    const int o3 = orient_exact(c, d, a);
    const int o4 = orient_exact(c, d, b);
    if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0)
        return bbox_overlap_exact(a, b, c, d);
    return o1 * o2 <= 0 && o3 * o4 <= 0;
}

template <class A, class B, class C, class D>
constexpr auto direction_cross_exact(const A& a, const B& b,
                                     const C& c, const D& d) {
    return wide_mul(x_val(b) - x_val(a), y_val(d) - y_val(c))
         - wide_mul(y_val(b) - y_val(a), x_val(d) - x_val(c));
}

template <class A, class B, class C, class D>
constexpr bool lines_coincident_exact(const A& a, const B& b,
                                      const C& c, const D& d) {
    const bool ab_deg = same_point_exact(a, b);
    const bool cd_deg = same_point_exact(c, d);
    if (ab_deg && cd_deg) return same_point_exact(a, c);
    if (ab_deg) return orient_exact(c, d, a) == 0;
    if (cd_deg) return orient_exact(a, b, c) == 0;
    return direction_cross_exact(a, b, c, d) == 0
        && orient_exact(a, b, c) == 0;
}

template <class A, class B, class C, class D>
constexpr bool lines_intersect_exact(const A& a, const B& b,
                                     const C& c, const D& d) {
    if (lines_coincident_exact(a, b, c, d)) return true;
    if (same_point_exact(a, b) || same_point_exact(c, d)) return false;
    return direction_cross_exact(a, b, c, d) != 0;
}

// Line vs segment: segment endpoints must not lie strictly on the same side.
template <class A, class B, class C, class D>
constexpr bool line_segment_intersect_exact(const A& la, const B& lb,
                                            const C& sa, const D& sb) {
    if (same_point_exact(la, lb)) return on_segment_exact(la, sa, sb);
    return orient_exact(la, lb, sa) * orient_exact(la, lb, sb) <= 0;
}

template <class A, class B, class P>
constexpr bool on_line_exact(const A& a, const B& b, const P& p) {
    if (same_point_exact(a, b)) return same_point_exact(a, p);
    return orient_exact(a, b, p) == 0;
}

template <class P, class A, class B>
constexpr bool in_rectangle_exact(const P& p, const A& a, const B& b) {
    return min(x_val(a), x_val(b)) <= x_val(p)
        && x_val(p) <= max(x_val(a), x_val(b))
        && min(y_val(a), y_val(b)) <= y_val(p)
        && y_val(p) <= max(y_val(a), y_val(b));
}

template <class A, class B, class C, class D>
constexpr bool rectangles_intersect_exact(const A& a, const B& b,
                                          const C& c, const D& d) {
    return bbox_overlap_exact(a, b, c, d);
}

template <class P>
struct integral_point3 : false_type {};
template <class... T>
struct integral_point3<tuple<T...>>
    : bool_constant<(sizeof...(T) >= 3)
                    && (is_integral_v<remove_cv_t<T>> && ...)> {};
template <class T, size_t N>
struct integral_point3<Point<T, N>>
    : bool_constant<(N >= 3) && is_integral_v<remove_cv_t<T>>> {};
template <class T, size_t N>
struct integral_point3<array<T, N>>
    : bool_constant<(N >= 3) && is_integral_v<remove_cv_t<T>>> {};
template <class T>
struct integral_point3<vector<T>>
    : bool_constant<is_integral_v<remove_cv_t<T>>> {};

template <class... Ps>
constexpr bool integral_points3_v =
    (integral_point3<remove_cv_t<remove_reference_t<Ps>>>::value && ...);

template <class P, class A, class B>
constexpr bool in_cuboid_exact(const P& p, const A& a, const B& b) {
    return min(x_val(a), x_val(b)) <= x_val(p)
        && x_val(p) <= max(x_val(a), x_val(b))
        && min(y_val(a), y_val(b)) <= y_val(p)
        && y_val(p) <= max(y_val(a), y_val(b))
        && min(z_val(a), z_val(b)) <= z_val(p)
        && z_val(p) <= max(z_val(a), z_val(b));
}

template <class C, class R, class P>
constexpr bool circle_contains_exact(const C& center, R radius, const P& p) {
    const auto dx = x_val(p) - x_val(center);
    const auto dy = y_val(p) - y_val(center);
    return wide_mul(dx, dx) + wide_mul(dy, dy) <= wide_mul(radius, radius);
}

template <class Poly, class Q>
bool point_in_polygon_exact(const Poly& shape, const Q& p) {
    const size_t n = shape.vertices.size();
    if (n == 0) return false;
    bool inside = false;
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const auto& a = shape.vertices[j];
        const auto& b = shape.vertices[i];
        if (on_segment_exact(p, a, b)) return true;
        const auto ay = y_val(a), by = y_val(b), py = y_val(p);
        if ((ay > py) == (by > py)) continue;
        const auto dy = by - ay;
        const auto lhs = wide_mul(x_val(p) - x_val(a), dy);
        const auto rhs = wide_mul(x_val(b) - x_val(a), py - ay);
        if (dy > 0 ? lhs < rhs : lhs > rhs) inside = !inside;
    }
    return inside;
}

template <class A, class B, class C, class P>
constexpr bool triangle_contains_exact(const A& a, const B& b, const C& c,
                                       const P& p) {
    const int o1 = orient_exact(a, b, p);
    const int o2 = orient_exact(b, c, p);
    const int o3 = orient_exact(c, a, p);
    const bool has_pos = o1 > 0 || o2 > 0 || o3 > 0;
    const bool has_neg = o1 < 0 || o2 < 0 || o3 < 0;
    return !(has_pos && has_neg);
}

template <class P, class A, class B>
bool on_segment2d(const P& p, const A& a, const B& b) {
    if constexpr (integral_points_v<P, A, B>) {
        return on_segment_exact(p, a, b);
    } else {
        const long double scale = 1 + fabsl(x_ld(a)) + fabsl(y_ld(a))
                                    + fabsl(x_ld(b)) + fabsl(y_ld(b));
        return segment_distance2d(p, a, b) <= tolerance(scale);
    }
}

template <class P, class A, class B>
bool in_rectangle(const P& p, const A& a, const B& b) {
    if constexpr (integral_points_v<P, A, B>) {
        return in_rectangle_exact(p, a, b);
    } else {
        const long double x = x_ld(p), y = y_ld(p);
        const long double x1 = x_ld(a), x2 = x_ld(b);
        const long double y1 = y_ld(a), y2 = y_ld(b);
        const long double eps = tolerance(1 + fabsl(x1) + fabsl(x2)
                                           + fabsl(y1) + fabsl(y2));
        return min(x1, x2) - eps <= x && x <= max(x1, x2) + eps
            && min(y1, y2) - eps <= y && y <= max(y1, y2) + eps;
    }
}
}  // namespace ac_detail::geometry

// @acf fn to_line — (LineEquation) -> Line<Point2<ld>> — 係数形式の直線を2点形式へ変換
/// @acf-doc @brief `LineEquation<T>` の `ax+by=c` を `Line<Point2<ld>>` へ変換する。`a,b` は同時に0でない
/// @acf-doc [function] to_line
template <class T>
auto to_line(const LineEquation<T>& equation) {
    assert(equation.a != 0 || equation.b != 0);
    const long double a = static_cast<long double>(equation.a);
    const long double b = static_cast<long double>(equation.b);
    const long double c = static_cast<long double>(equation.c);
    if (equation.b != 0) {
        return make_line(Point2<long double>{0, c / b},
                         Point2<long double>{1, (c - a) / b});
    }
    return make_line(Point2<long double>{c / a, 0},
                     Point2<long double>{c / a, 1});
}

// @acf fn to_line_equation — (Line) -> LineEquation<ld> — 2点形式の直線を係数形式へ変換
/// @acf-doc @brief 2点形式の `Line` を一般形 `ax+by=c` の `LineEquation<ld>` へ変換する。2点は異なる必要がある
/// @acf-doc [function] to_line_equation
template <class A, class B>
auto to_line_equation(const Line<A, B>& line) {
    using namespace ac_detail::geometry;
    const auto first = point2_ld(line.a);
    const auto second = point2_ld(line.b);
    const auto direction = second - first;
    const long double scale = 1 + vector_norm(first) + vector_norm(second);
    assert(dot_product(direction, direction) > tolerance(scale * scale));

    const long double a = first.y() - second.y();
    const long double b = second.x() - first.x();
    const long double c = a * first.x() + b * first.y();
    return LineEquation<long double>{a, b, c};
}

// @acf fn to_rectangle — (Square) -> Rectangle — 正方形を長方形へ変換する
/// @acf-doc @brief 座標軸に平行な正方形を、同じ領域の `Rectangle` にする
/// @acf-doc [function] to_rectangle
template <class P, class S>
auto to_rectangle(const Square<P, S>& shape) {
    using namespace ac_detail::geometry;
    const Point2<long double> opposite{
        x_ld(shape.corner) + static_cast<long double>(shape.side),
        y_ld(shape.corner) + static_cast<long double>(shape.side)};
    return make_rectangle(point2_ld(shape.corner), opposite);
}

// @acf fn to_polygon — (Rectangle/Square/Triangle/Polygon) -> Polygon<Point2<ld>> — 塗りつぶし図形を多角形へ
/// @acf-doc @brief 長方形・正方形・三角形・多角形を `Polygon<Point2<ld>>` にする。軸平行長方形は反時計回り
/// @acf-doc [function] to_polygon
template <class A, class B>
auto to_polygon(const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    const long double x1 = min(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double x2 = max(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double y1 = min(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double y2 = max(y_ld(shape.corner1), y_ld(shape.corner2));
    return make_polygon(vector<Point2<long double>>{
        {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}});
}

template <class P, class S>
auto to_polygon(const Square<P, S>& shape) {
    return to_polygon(to_rectangle(shape));
}

template <class A, class B, class C>
auto to_polygon(const Triangle<A, B, C>& shape) {
    using namespace ac_detail::geometry;
    return make_polygon(vector<Point2<long double>>{
        point2_ld(shape.a), point2_ld(shape.b), point2_ld(shape.c)});
}

template <class P>
auto to_polygon(const Polygon<P>& shape) {
    using namespace ac_detail::geometry;
    vector<Point2<long double>> vertices;
    vertices.reserve(shape.vertices.size());
    for (const auto& vertex : shape.vertices) vertices.push_back(point2_ld(vertex));
    return make_polygon(std::move(vertices));
}

// @acf fn segment_length — (Segment) -> long double — 線分の長さ
/// @acf-doc @brief 線分の端点間距離を返す。変数名 `length` との衝突を避ける
/// @acf-doc [function] segment_length
template <class A, class B>
long double segment_length(const Segment<A, B>& shape) {
    return euclidean_distance(shape.a, shape.b);
}

// @acf fn bounding_rectangle — (shape) -> Rectangle<Point2<ld>,Point2<ld>> — 軸平行な外接長方形
/// @acf-doc @brief 2D図形を囲む座標軸に平行な最小長方形を返す
/// @acf-doc [function] bounding_rectangle
template <class A, class B>
auto bounding_rectangle(const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    const long double x1 = min(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double x2 = max(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double y1 = min(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double y2 = max(y_ld(shape.corner1), y_ld(shape.corner2));
    return make_rectangle(Point2<long double>{x1, y1}, Point2<long double>{x2, y2});
}

template <class P, class S>
auto bounding_rectangle(const Square<P, S>& shape) {
    return to_rectangle(shape);
}

template <class P>
auto bounding_rectangle(const Polygon<P>& shape) {
    using namespace ac_detail::geometry;
    assert(!shape.vertices.empty());
    long double x1 = x_ld(shape.vertices[0]), x2 = x1;
    long double y1 = y_ld(shape.vertices[0]), y2 = y1;
    for (const auto& vertex : shape.vertices) {
        x1 = min(x1, x_ld(vertex));
        x2 = max(x2, x_ld(vertex));
        y1 = min(y1, y_ld(vertex));
        y2 = max(y2, y_ld(vertex));
    }
    return make_rectangle(Point2<long double>{x1, y1}, Point2<long double>{x2, y2});
}

template <class A, class B, class C>
auto bounding_rectangle(const Triangle<A, B, C>& shape) {
    return bounding_rectangle(to_polygon(shape));
}

template <class P, class R>
auto bounding_rectangle(const Circle<P, R>& shape) {
    using namespace ac_detail::geometry;
    const long double radius = static_cast<long double>(shape.radius);
    const auto center = point2_ld(shape.center);
    return make_rectangle(Point2<long double>{center.x() - radius, center.y() - radius},
                          Point2<long double>{center.x() + radius, center.y() + radius});
}

template <class A, class B>
auto bounding_rectangle(const Segment<A, B>& shape) {
    using namespace ac_detail::geometry;
    return make_rectangle(Point2<long double>{min(x_ld(shape.a), x_ld(shape.b)),
                                              min(y_ld(shape.a), y_ld(shape.b))},
                          Point2<long double>{max(x_ld(shape.a), x_ld(shape.b)),
                                              max(y_ld(shape.a), y_ld(shape.b))});
}

// @acf fn centroid — (shape) -> Point2<ld> — 2D図形の重心
/// @acf-doc @brief 円は中心、長方形・正方形は対角線の交点、三角形・多角形は面積重心
/// @acf-doc [function] centroid
template <class P, class R>
auto centroid(const Circle<P, R>& shape) {
    return ac_detail::geometry::point2_ld(shape.center);
}

template <class A, class B>
auto centroid(const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    return Point2<long double>{
        (x_ld(shape.corner1) + x_ld(shape.corner2)) / 2,
        (y_ld(shape.corner1) + y_ld(shape.corner2)) / 2};
}

template <class P, class S>
auto centroid(const Square<P, S>& shape) {
    return centroid(to_rectangle(shape));
}

template <class A, class B, class C>
auto centroid(const Triangle<A, B, C>& shape) {
    using namespace ac_detail::geometry;
    return Point2<long double>{
        (x_ld(shape.a) + x_ld(shape.b) + x_ld(shape.c)) / 3,
        (y_ld(shape.a) + y_ld(shape.b) + y_ld(shape.c)) / 3};
}

template <class P>
auto centroid(const Polygon<P>& shape) {
    using namespace ac_detail::geometry;
    const size_t n = shape.vertices.size();
    assert(n > 0);
    long double twice_area = 0, cx = 0, cy = 0;
    for (size_t i = 0; i < n; ++i) {
        const auto& a = shape.vertices[i];
        const auto& b = shape.vertices[(i + 1) % n];
        const long double cross = x_ld(a) * y_ld(b) - y_ld(a) * x_ld(b);
        twice_area += cross;
        cx += (x_ld(a) + x_ld(b)) * cross;
        cy += (y_ld(a) + y_ld(b)) * cross;
    }
    if (fabsl(twice_area) <= tolerance(1)) {
        long double sx = 0, sy = 0;
        for (const auto& vertex : shape.vertices) {
            sx += x_ld(vertex);
            sy += y_ld(vertex);
        }
        return Point2<long double>{sx / n, sy / n};
    }
    return Point2<long double>{cx / (3 * twice_area), cy / (3 * twice_area)};
}

enum class Ccw {
    Clockwise = -1,
    OnSegment = 0,
    CounterClockwise = 1,
    OnlineBack = 2,
    OnlineFront = -2,
};

// @acf fn ccw — (a,b,p) / (ax,ay,bx,by,px,py) -> Ccw — 有向線分abに対する点pの位置を5分類する
/// @acf-doc @brief 反時計回り・時計回り・後方・前方・線分上を `Ccw` で返す。整数座標は厳密、実数はEPS。6引数の生座標版あり
/// @acf-doc [function] ccw
template <class A, class B, class P>
Ccw ccw(const A& a_value, const B& b_value, const P& p_value) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, P>) {
        const int side = orient_exact(a_value, b_value, p_value);
        if (side > 0) return Ccw::CounterClockwise;
        if (side < 0) return Ccw::Clockwise;
        const auto abx = x_val(b_value) - x_val(a_value);
        const auto aby = y_val(b_value) - y_val(a_value);
        const auto apx = x_val(p_value) - x_val(a_value);
        const auto apy = y_val(p_value) - y_val(a_value);
        if (wide_mul(abx, apx) + wide_mul(aby, apy) < 0) return Ccw::OnlineBack;
        if (wide_mul(apx, apx) + wide_mul(apy, apy)
                > wide_mul(abx, abx) + wide_mul(aby, aby))
            return Ccw::OnlineFront;
        return Ccw::OnSegment;
    } else {
        const auto a = point2_ld(a_value);
        const auto b = point2_ld(b_value);
        const auto p = point2_ld(p_value);
        const auto ab = b - a;
        const auto ap = p - a;
        const long double scale = 1 + vector_norm(ab) + vector_norm(ap);
        const long double eps = tolerance(scale * scale);
        const long double cross = cross2d_ld(ab, ap);
        if (cross > eps) return Ccw::CounterClockwise;
        if (cross < -eps) return Ccw::Clockwise;
        if (dot_product(ab, ap) < -eps) return Ccw::OnlineBack;
        if (vector_norm_squared(ab) + eps < vector_norm_squared(ap))
            return Ccw::OnlineFront;
        return Ccw::OnSegment;
    }
}

template <class A, class B, class C, class D, class E, class F>
Ccw ccw(A ax, B ay, C bx, D by, E px, F py) {
    return ccw(pair{ax, ay}, pair{bx, by}, pair{px, py});
}

// @acf fn projection_point — (p,a,b) / (p,line) / coordinates -> Point2<ld> — 点を直線へ正射影する
/// @acf-doc @brief 点pから直線abへ下ろした垂線の足。a==bならaを返す。6引数の生座標版あり
/// @acf-doc [function] projection_point
template <class P, class A, class B>
auto projection_point(const P& p_value, const A& a_value, const B& b_value) {
    using namespace ac_detail::geometry;
    const auto p = point2_ld(p_value);
    const auto a = point2_ld(a_value);
    const auto b = point2_ld(b_value);
    const auto ab = b - a;
    const long double length2 = dot_product(ab, ab);
    if (length2 == 0) return a;
    return a + ab * (dot_product(p - a, ab) / length2);
}

template <class P, class A, class B>
auto projection_point(const P& p, const Line<A, B>& line) {
    return projection_point(p, line.a, line.b);
}

template <class A, class B, class C, class D, class E, class F>
auto projection_point(A px, B py, C ax, D ay, E bx, F by) {
    return projection_point(pair{px, py}, pair{ax, ay}, pair{bx, by});
}

// @acf fn reflection_point — (p,a,b) / (p,line) / coordinates -> Point2<ld> — 点を直線に関して反転する
/// @acf-doc @brief 点pを直線abに関して線対称移動した点を返す。6引数の生座標版あり
/// @acf-doc [function] reflection_point
template <class P, class A, class B>
auto reflection_point(const P& p_value, const A& a, const B& b) {
    const auto p = ac_detail::geometry::point2_ld(p_value);
    return projection_point(p, a, b) * 2 - p;
}

template <class P, class A, class B>
auto reflection_point(const P& p, const Line<A, B>& line) {
    return reflection_point(p, line.a, line.b);
}

template <class A, class B, class C, class D, class E, class F>
auto reflection_point(A px, B py, C ax, D ay, E bx, F by) {
    return reflection_point(pair{px, py}, pair{ax, ay}, pair{bx, by});
}

namespace ac_detail::geometry {
inline bool same_point2d(const Point2<long double>& a,
                         const Point2<long double>& b) {
    const long double scale = 1 + vector_norm(a) + vector_norm(b);
    return euclidean_distance(a, b) <= tolerance(scale);
}

inline void push_unique(vector<Point2<long double>>& points,
                        const Point2<long double>& point) {
    for (const auto& current : points)
        if (same_point2d(current, point)) return;
    points.push_back(point);
}

inline long double parameter_eps() {
    return max(1e-9L, tolerance(1));
}

inline bool in_unit_interval(long double t, long double eps = parameter_eps()) {
    return t >= -eps && t <= 1 + eps;
}

template <class A, class B, class C, class D>
bool lines_coincident(const A& a_value, const B& b_value,
                      const C& c_value, const D& d_value) {
    if constexpr (integral_points_v<A, B, C, D>) {
        return lines_coincident_exact(a_value, b_value, c_value, d_value);
    } else {
        const auto a = point2_ld(a_value), b = point2_ld(b_value);
        const auto c = point2_ld(c_value), d = point2_ld(d_value);
        const auto r = b - a, s = d - c;
        const long double scale = 1 + vector_norm(r) + vector_norm(s);
        const long double eps = tolerance(scale * scale);
        const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
        if (r2 <= eps && s2 <= eps) return same_point2d(a, c);
        if (r2 <= eps) return point_line_distance(a, c, d) <= tolerance(scale);
        if (s2 <= eps) return point_line_distance(c, a, b) <= tolerance(scale);
        return fabsl(cross2d_ld(r, s)) <= eps
            && fabsl(cross2d_ld(c - a, r)) <= eps;
    }
}

template <class A, class B, class C, class D>
bool segments_intersect_ld(const A& a_value, const B& b_value,
                           const C& c_value, const D& d_value) {
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value), d = point2_ld(d_value);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
    if (r2 <= eps && s2 <= eps) return same_point2d(a, c);
    if (r2 <= eps) return on_segment2d(a, c, d);
    if (s2 <= eps) return on_segment2d(c, a, b);
    const long double denominator = cross2d_ld(r, s);
    if (fabsl(denominator) > eps) {
        const long double t = cross2d_ld(c - a, s) / denominator;
        const long double u = cross2d_ld(c - a, r) / denominator;
        return in_unit_interval(t) && in_unit_interval(u);
    }
    if (!lines_coincident(a_value, b_value, c_value, d_value)) return false;
    return on_segment2d(a, c, d) || on_segment2d(b, c, d)
        || on_segment2d(c, a, b) || on_segment2d(d, a, b);
}

template <class A, class B, class C, class D>
bool line_segment_intersect_ld(const A& a_value, const B& b_value,
                               const C& c_value, const D& d_value) {
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value), d = point2_ld(d_value);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
    if (r2 <= eps) return on_segment2d(a, c, d);
    if (s2 <= eps) return point_line_distance(c, a, b) <= tolerance(scale);
    if (lines_coincident(a_value, b_value, c_value, d_value)) return true;
    const long double denominator = cross2d_ld(r, s);
    if (fabsl(denominator) <= eps) return false;
    const long double u = cross2d_ld(c - a, r) / denominator;
    return in_unit_interval(u);
}

template <class A, class B, class C, class D>
bool lines_intersect_ld(const A& a_value, const B& b_value,
                        const C& c_value, const D& d_value) {
    if (lines_coincident(a_value, b_value, c_value, d_value)) return true;
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value), d = point2_ld(d_value);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    return fabsl(cross2d_ld(r, s)) > eps;
}

template <class A, class B, class C, class D>
vector<Point2<long double>> line_intersection_points(
        const A& a_value, const B& b_value,
        const C& c_value, const D& d_value) {
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value), d = point2_ld(d_value);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
    if (r2 <= eps && s2 <= eps)
        return same_point2d(a, c) ? vector<Point2<long double>>{a}
                                  : vector<Point2<long double>>{};
    if (r2 <= eps)
        return point_line_distance(a, c, d) <= tolerance(scale)
            ? vector<Point2<long double>>{a} : vector<Point2<long double>>{};
    if (s2 <= eps)
        return point_line_distance(c, a, b) <= tolerance(scale)
            ? vector<Point2<long double>>{c} : vector<Point2<long double>>{};
    const long double denominator = cross2d_ld(r, s);
    if (fabsl(denominator) <= eps) return {};
    const long double t = cross2d_ld(c - a, s) / denominator;
    return {a + r * t};
}
}  // namespace ac_detail::geometry

// @acf fn intersection_points — (shape1,shape2) -> vector<Point2<ld>> — 交点を列挙する
/// @acf-doc @brief 直線・`LineEquation`・線分・円の交点を重複なしで返す。完全一致で交点が無限個なら空配列
/// @acf-doc [function] intersection_points
template <class A, class B, class C, class D>
auto intersection_points(const Line<A, B>& first,
                         const Line<C, D>& second) {
    return ac_detail::geometry::line_intersection_points(
        first.a, first.b, second.a, second.b);
}

template <class T, class U>
auto intersection_points(const LineEquation<T>& first,
                         const LineEquation<U>& second) {
    const long double a1 = static_cast<long double>(first.a);
    const long double b1 = static_cast<long double>(first.b);
    const long double c1 = static_cast<long double>(first.c);
    const long double a2 = static_cast<long double>(second.a);
    const long double b2 = static_cast<long double>(second.b);
    const long double c2 = static_cast<long double>(second.c);

    long double det;
    if constexpr (is_integral_v<T> && is_integral_v<U>) {
        const i128 exact_det = static_cast<i128>(first.a) * second.b
                             - static_cast<i128>(second.a) * first.b;
        if (exact_det == 0) return vector<Point2<long double>>{};
        det = static_cast<long double>(exact_det);
    } else {
        det = a1 * b2 - a2 * b1;
        const long double scale = 1 + fabsl(a1) + fabsl(b1)
                                    + fabsl(a2) + fabsl(b2);
        if (fabsl(det) <= ac_detail::geometry::tolerance(scale * scale))
            return vector<Point2<long double>>{};
    }

    return vector<Point2<long double>>{
        Point2<long double>{(c1 * b2 - c2 * b1) / det,
                            (a1 * c2 - a2 * c1) / det}};
}

template <class A, class B, class C, class D>
auto intersection_points(const Segment<A, B>& first,
                         const Segment<C, D>& second) {
    using namespace ac_detail::geometry;
    vector<Point2<long double>> result;
    const auto a = point2_ld(first.a), b = point2_ld(first.b);
    const auto c = point2_ld(second.a), d = point2_ld(second.b);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
    if (r2 <= eps && s2 <= eps) {
        if (same_point2d(a, c)) result.push_back(a);
        return result;
    }
    if (r2 <= eps) {
        if (on_segment2d(a, second.a, second.b)) result.push_back(a);
        return result;
    }
    if (s2 <= eps) {
        if (on_segment2d(c, first.a, first.b)) result.push_back(c);
        return result;
    }
    const long double denominator = cross2d_ld(r, s);
    if (fabsl(denominator) > eps) {
        const long double t = cross2d_ld(c - a, s) / denominator;
        const long double u = cross2d_ld(c - a, r) / denominator;
        if (in_unit_interval(t) && in_unit_interval(u))
            result.push_back(a + r * t);
        return result;
    }
    if (!lines_coincident(first.a, first.b, second.a, second.b)) return result;
    for (const auto& point : {a, b, c, d})
        if (on_segment2d(point, first.a, first.b)
                && on_segment2d(point, second.a, second.b))
            push_unique(result, point);
    if (result.size() == 2) {
        const auto direction = b - a;
        const bool use_x = fabsl(direction.x()) >= fabsl(direction.y());
        sort(result.begin(), result.end(), [&](const auto& x, const auto& y) {
            return use_x ? x.x() < y.x() : x.y() < y.y();
        });
    }
    return result;
}

template <class A, class B, class C, class D>
auto intersection_points(const Line<A, B>& line,
                         const Segment<C, D>& segment) {
    using namespace ac_detail::geometry;
    if (lines_coincident(line.a, line.b, segment.a, segment.b)) {
        vector<Point2<long double>> result;
        push_unique(result, point2_ld(segment.a));
        push_unique(result, point2_ld(segment.b));
        return result;
    }
    const auto a = point2_ld(line.a), b = point2_ld(line.b);
    const auto c = point2_ld(segment.a), d = point2_ld(segment.b);
    const auto r = b - a, s = d - c;
    const long double scale = 1 + vector_norm(r) + vector_norm(s);
    const long double eps = tolerance(scale * scale);
    const long double r2 = dot_product(r, r), s2 = dot_product(s, s);
    vector<Point2<long double>> result;
    if (r2 <= eps) {
        if (on_segment2d(a, segment.a, segment.b)) result.push_back(a);
        return result;
    }
    if (s2 <= eps) {
        if (point_line_distance(c, a, b) <= tolerance(scale))
            result.push_back(c);
        return result;
    }
    const long double denominator = cross2d_ld(r, s);
    if (fabsl(denominator) <= eps) return result;
    const long double t = cross2d_ld(c - a, s) / denominator;
    const long double u = cross2d_ld(c - a, r) / denominator;
    if (in_unit_interval(u)) result.push_back(a + r * t);
    return result;
}

template <class A, class B, class C, class D>
auto intersection_points(const Segment<A, B>& segment,
                         const Line<C, D>& line) {
    return intersection_points(line, segment);
}

template <class C, class R, class A, class B>
auto intersection_points(const Circle<C, R>& circle,
                         const Line<A, B>& line) {
    using namespace ac_detail::geometry;
    vector<Point2<long double>> result;
    const auto center = point2_ld(circle.center);
    const auto a = point2_ld(line.a), b = point2_ld(line.b);
    const auto direction = b - a;
    const long double direction2 = dot_product(direction, direction);
    const long double radius = static_cast<long double>(circle.radius);
    const long double eps = tolerance(1 + radius + vector_norm(center));
    const long double squared_eps = tolerance(1 + radius * radius);
    if (direction2 == 0) {
        if (fabsl(euclidean_distance(center, a) - radius) <= eps)
            result.push_back(a);
        return result;
    }
    const auto foot = projection_point(center, a, b);
    const long double distance = euclidean_distance(center, foot);
    if (distance > radius + eps) return result;
    if (fabsl(distance - radius) <= eps) {
        result.push_back(foot);
        return result;
    }
    const long double offset2 = radius * radius - distance * distance;
    if (offset2 < -squared_eps) return result;
    const long double offset = sqrt(max(0.0L, offset2));
    const auto unit = direction / sqrt(direction2);
    result.push_back(foot - unit * offset);
    result.push_back(foot + unit * offset);
    return result;
}

template <class A, class B, class C, class R>
auto intersection_points(const Line<A, B>& line,
                         const Circle<C, R>& circle) {
    return intersection_points(circle, line);
}

template <class C, class R, class A, class B>
auto intersection_points(const Circle<C, R>& circle,
                         const Segment<A, B>& segment) {
    using namespace ac_detail::geometry;
    auto result = intersection_points(circle, make_line(segment.a, segment.b));
    result.erase(remove_if(result.begin(), result.end(), [&](const auto& p) {
        return !on_segment2d(p, segment.a, segment.b);
    }), result.end());
    return result;
}

template <class A, class B, class C, class R>
auto intersection_points(const Segment<A, B>& segment,
                         const Circle<C, R>& circle) {
    return intersection_points(circle, segment);
}

template <class C1, class R1, class C2, class R2>
auto intersection_points(const Circle<C1, R1>& first,
                         const Circle<C2, R2>& second) {
    using namespace ac_detail::geometry;
    vector<Point2<long double>> result;
    const auto a = point2_ld(first.center), b = point2_ld(second.center);
    const long double r1 = static_cast<long double>(first.radius);
    const long double r2 = static_cast<long double>(second.radius);
    const auto delta = b - a;
    const long double d = vector_norm(delta);
    const long double eps = tolerance(1 + r1 + r2 + d);
    const long double squared_eps = tolerance(1 + r1 * r1 + r2 * r2 + d * d);
    if (d <= eps) return result;
    if (d > r1 + r2 + eps || d < fabsl(r1 - r2) - eps) return result;
    const long double along = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    const long double height2 = r1 * r1 - along * along;
    if (height2 < -squared_eps) return result;
    const auto unit = delta / d;
    const auto base = a + unit * along;
    if (height2 <= squared_eps) {
        result.push_back(base);
        return result;
    }
    const long double height = sqrt(max(0.0L, height2));
    const Point2<long double> normal{-unit.y(), unit.x()};
    result.push_back(base + normal * height);
    result.push_back(base - normal * height);
    return result;
}

namespace ac_detail::geometry {
template <class Poly, class Other>
auto polygon_boundary_intersections(const Poly& poly, const Other& other) {
    vector<Point2<long double>> result;
    const size_t n = poly.vertices.size();
    if (n < 2) return result;
    for (size_t i = 0; i < n; ++i) {
        const auto edge = make_segment(poly.vertices[i], poly.vertices[(i + 1) % n]);
        for (const auto& point : intersection_points(edge, other))
            push_unique(result, point);
    }
    return result;
}
}  // namespace ac_detail::geometry

template <class P, class A, class B>
auto intersection_points(const Polygon<P>& polygon, const Line<A, B>& line) {
    return ac_detail::geometry::polygon_boundary_intersections(polygon, line);
}

template <class A, class B, class P>
auto intersection_points(const Line<A, B>& line, const Polygon<P>& polygon) {
    return intersection_points(polygon, line);
}

template <class P, class A, class B>
auto intersection_points(const Polygon<P>& polygon, const Segment<A, B>& segment) {
    return ac_detail::geometry::polygon_boundary_intersections(polygon, segment);
}

template <class A, class B, class P>
auto intersection_points(const Segment<A, B>& segment, const Polygon<P>& polygon) {
    return intersection_points(polygon, segment);
}

template <class P, class C, class R>
auto intersection_points(const Polygon<P>& polygon, const Circle<C, R>& circle) {
    return ac_detail::geometry::polygon_boundary_intersections(polygon, circle);
}

template <class C, class R, class P>
auto intersection_points(const Circle<C, R>& circle, const Polygon<P>& polygon) {
    return intersection_points(polygon, circle);
}

template <class A, class B, class C, class D>
auto intersection_points(const Rectangle<A, B>& rectangle, const Line<C, D>& line) {
    return intersection_points(to_polygon(rectangle), line);
}

template <class C, class D, class A, class B>
auto intersection_points(const Line<C, D>& line, const Rectangle<A, B>& rectangle) {
    return intersection_points(rectangle, line);
}

template <class A, class B, class C, class D>
auto intersection_points(const Rectangle<A, B>& rectangle, const Segment<C, D>& segment) {
    return intersection_points(to_polygon(rectangle), segment);
}

template <class C, class D, class A, class B>
auto intersection_points(const Segment<C, D>& segment, const Rectangle<A, B>& rectangle) {
    return intersection_points(rectangle, segment);
}

template <class A, class B, class C, class R>
auto intersection_points(const Rectangle<A, B>& rectangle, const Circle<C, R>& circle) {
    return intersection_points(to_polygon(rectangle), circle);
}

template <class C, class R, class A, class B>
auto intersection_points(const Circle<C, R>& circle, const Rectangle<A, B>& rectangle) {
    return intersection_points(rectangle, circle);
}

template <class P, class S, class A, class B>
auto intersection_points(const Square<P, S>& square, const Line<A, B>& line) {
    return intersection_points(to_polygon(square), line);
}

template <class A, class B, class P, class S>
auto intersection_points(const Line<A, B>& line, const Square<P, S>& square) {
    return intersection_points(square, line);
}

template <class P, class S, class A, class B>
auto intersection_points(const Square<P, S>& square, const Segment<A, B>& segment) {
    return intersection_points(to_polygon(square), segment);
}

template <class A, class B, class P, class S>
auto intersection_points(const Segment<A, B>& segment, const Square<P, S>& square) {
    return intersection_points(square, segment);
}

template <class P, class S, class C, class R>
auto intersection_points(const Square<P, S>& square, const Circle<C, R>& circle) {
    return intersection_points(to_polygon(square), circle);
}

template <class C, class R, class P, class S>
auto intersection_points(const Circle<C, R>& circle, const Square<P, S>& square) {
    return intersection_points(square, circle);
}

template <class A, class B, class C, class D, class E>
auto intersection_points(const Triangle<A, B, C>& triangle, const Line<D, E>& line) {
    return intersection_points(to_polygon(triangle), line);
}

template <class D, class E, class A, class B, class C>
auto intersection_points(const Line<D, E>& line, const Triangle<A, B, C>& triangle) {
    return intersection_points(triangle, line);
}

template <class A, class B, class C, class D, class E>
auto intersection_points(const Triangle<A, B, C>& triangle, const Segment<D, E>& segment) {
    return intersection_points(to_polygon(triangle), segment);
}

template <class D, class E, class A, class B, class C>
auto intersection_points(const Segment<D, E>& segment, const Triangle<A, B, C>& triangle) {
    return intersection_points(triangle, segment);
}

template <class A, class B, class C, class Cent, class R>
auto intersection_points(const Triangle<A, B, C>& triangle, const Circle<Cent, R>& circle) {
    return intersection_points(to_polygon(triangle), circle);
}

template <class Cent, class R, class A, class B, class C>
auto intersection_points(const Circle<Cent, R>& circle, const Triangle<A, B, C>& triangle) {
    return intersection_points(triangle, circle);
}

// @acf fn intersects — (shape1,shape2) -> bool — 2D・3D図形の交差を判定する
/// @acf-doc @brief 直線・線分・円・多角形など2D図形と球・直方体が共有部分を持つか判定する。完全一致や内包もtrue。線分・直線の整数座標は厳密判定
/// @acf-doc [function] intersects
template <class A, class B, class C, class D>
bool intersects(const Line<A, B>& first, const Line<C, D>& second) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, C, D>)
        return lines_intersect_exact(first.a, first.b, second.a, second.b);
    else
        return lines_intersect_ld(first.a, first.b, second.a, second.b);
}

template <class A, class B, class C, class D>
bool intersects(const Segment<A, B>& first, const Segment<C, D>& second) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, C, D>)
        return segments_intersect_exact(first.a, first.b, second.a, second.b);
    else
        return segments_intersect_ld(first.a, first.b, second.a, second.b);
}

template <class A, class B, class C, class D>
bool intersects(const Line<A, B>& line, const Segment<C, D>& segment) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, C, D>)
        return line_segment_intersect_exact(line.a, line.b, segment.a, segment.b);
    else
        return line_segment_intersect_ld(line.a, line.b, segment.a, segment.b);
}

template <class A, class B, class C, class D>
bool intersects(const Segment<A, B>& segment, const Line<C, D>& line) {
    return intersects(line, segment);
}

template <class C, class R, class A, class B>
bool intersects(const Circle<C, R>& circle, const Line<A, B>& line) {
    return !intersection_points(circle, line).empty();
}

template <class A, class B, class C, class R>
bool intersects(const Line<A, B>& line, const Circle<C, R>& circle) {
    return intersects(circle, line);
}

template <class C, class R, class A, class B>
bool intersects(const Circle<C, R>& circle, const Segment<A, B>& segment) {
    const long double radius = static_cast<long double>(circle.radius);
    const long double eps = ac_detail::geometry::tolerance(radius);
    return !intersection_points(circle, segment).empty()
        || ac_detail::geometry::distance2d(circle.center, segment.a) <= radius + eps
        || ac_detail::geometry::distance2d(circle.center, segment.b) <= radius + eps;
}

template <class A, class B, class C, class R>
bool intersects(const Segment<A, B>& segment, const Circle<C, R>& circle) {
    return intersects(circle, segment);
}

template <class C1, class R1, class C2, class R2>
bool intersects(const Circle<C1, R1>& first,
                const Circle<C2, R2>& second) {
    using namespace ac_detail::geometry;
    const long double d = distance2d(first.center, second.center);
    const long double r1 = static_cast<long double>(first.radius);
    const long double r2 = static_cast<long double>(second.radius);
    const long double eps = tolerance(1 + d + r1 + r2);
    return d <= r1 + r2 + eps;
}

// @acf fn convex_hull — (points,include_collinear=false) -> Polygon<P> — 凸包を反時計回りで作る
/// @acf-doc @brief 点集合の凸包。重複点を除き、最小点から反時計回りに頂点を返す。整数座標は厳密な向き判定
/// @acf-doc [function] convex_hull
template <class P>
auto convex_hull(vector<P> points, bool include_collinear = false) {
    using namespace ac_detail::geometry;
    if constexpr (integral_point2<remove_cv_t<P>>::value) {
        sort(points.begin(), points.end(), [](const P& a, const P& b) {
            if (x_val(a) != x_val(b)) return x_val(a) < x_val(b);
            return y_val(a) < y_val(b);
        });
        points.erase(unique(points.begin(), points.end(),
                            [](const P& a, const P& b) {
                                return same_point_exact(a, b);
                            }),
                     points.end());
    } else {
        sort(points.begin(), points.end(), [](const P& a, const P& b) {
            const long double ax = x_ld(a), bx = x_ld(b);
            if (ax != bx) return ax < bx;
            return y_ld(a) < y_ld(b);
        });
        points.erase(unique(points.begin(), points.end(), [](const P& a, const P& b) {
            return x_ld(a) == x_ld(b) && y_ld(a) == y_ld(b);
        }), points.end());
    }
    if (points.size() <= 2) return Polygon<P>{std::move(points)};

    const auto turn = [&](const P& a, const P& b, const P& c) {
        if constexpr (integral_point2<remove_cv_t<P>>::value) {
            return orient_exact(a, b, c);
        } else {
            const auto pa = point2_ld(a), pb = point2_ld(b), pc = point2_ld(c);
            const long double value = cross2d_ld(pa, pb, pc);
            const long double scale = 1 + vector_norm(pb - pa) + vector_norm(pc - pa);
            const long double eps = tolerance(scale * scale);
            if (value > eps) return 1;
            if (value < -eps) return -1;
            return 0;
        }
    };

    bool all_collinear = true;
    for (size_t i = 2; i < points.size(); ++i)
        if (turn(points[0], points[1], points[i]) != 0) {
            all_collinear = false;
            break;
        }
    if (all_collinear) {
        if (include_collinear) return Polygon<P>{std::move(points)};
        return Polygon<P>{{points.front(), points.back()}};
    }

    vector<P> lower, upper;
    for (const auto& point : points) {
        while (lower.size() >= 2) {
            const int direction = turn(lower[lower.size() - 2], lower.back(), point);
            if (direction > 0 || (include_collinear && direction == 0)) break;
            lower.pop_back();
        }
        lower.push_back(point);
    }
    for (auto it = points.rbegin(); it != points.rend(); ++it) {
        while (upper.size() >= 2) {
            const int direction = turn(upper[upper.size() - 2], upper.back(), *it);
            if (direction > 0 || (include_collinear && direction == 0)) break;
            upper.pop_back();
        }
        upper.push_back(*it);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return Polygon<P>{std::move(lower)};
}

struct ClosestPairResult {
    long double distance = numeric_limits<long double>::infinity();
    size_t first_index = numeric_limits<size_t>::max();
    size_t second_index = numeric_limits<size_t>::max();

    /// @brief 2点以上の入力から有効な最近点対が得られたか判定する。
    constexpr bool valid() const {
        return first_index != numeric_limits<size_t>::max();
    }
};

inline ostream& operator<<(ostream& os, const ClosestPairResult& result) {
    return os << "ClosestPairResult{distance=" << result.distance
              << ", first_index=" << result.first_index
              << ", second_index=" << result.second_index
              << ", valid=" << (result.valid() ? "true" : "false") << '}';
}

// @acf fn closest_pair — (points) -> ClosestPairResult — 最近点対をO(N log N)で求める
/// @acf-doc @brief 最短距離と元配列での2添字を返す。2点未満ではvalid()==false
/// @acf-doc [function] closest_pair
template <class P>
ClosestPairResult closest_pair(const vector<P>& points) {
    using namespace ac_detail::geometry;
    struct Item {
        long double x;
        long double y;
        size_t index;
    };
    vector<Item> sorted;
    sorted.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        sorted.push_back({x_ld(points[i]), y_ld(points[i]), i});
    sort(sorted.begin(), sorted.end(), [](const Item& a, const Item& b) {
        return tie(a.x, a.y, a.index) < tie(b.x, b.y, b.index);
    });

    ClosestPairResult result;
    set<tuple<long double, long double, size_t>> active;
    size_t left = 0;
    for (const auto& current : sorted) {
        while (left < sorted.size()
               && current.x - sorted[left].x > result.distance) {
            active.erase({sorted[left].y, sorted[left].x, sorted[left].index});
            ++left;
        }
        auto first = result.valid()
            ? active.lower_bound({current.y - result.distance,
                                  -numeric_limits<long double>::infinity(), 0})
            : active.begin();
        auto last = result.valid()
            ? active.upper_bound({current.y + result.distance,
                                  numeric_limits<long double>::infinity(),
                                  numeric_limits<size_t>::max()})
            : active.end();
        for (auto it = first; it != last; ++it) {
            const auto [y, x, index] = *it;
            const long double distance = hypot(current.x - x, current.y - y);
            if (distance < result.distance) {
                result.distance = distance;
                result.first_index = index;
                result.second_index = current.index;
                if (distance == 0) return result;
            }
        }
        active.insert({current.y, current.x, current.index});
    }
    return result;
}

// @acf fn tangent_points — (circle,p) -> vector<Point2<ld>> — 点から円へ引いた接線の接点を返す
/// @acf-doc @brief 円外の点からは2点、円上からは1点、円内からは0点を返す
/// @acf-doc [function] tangent_points
template <class C, class R, class P>
auto tangent_points(const Circle<C, R>& circle, const P& point_value) {
    using namespace ac_detail::geometry;
    vector<Point2<long double>> result;
    const auto center = point2_ld(circle.center);
    const auto point = point2_ld(point_value);
    const auto delta = point - center;
    const long double distance2 = dot_product(delta, delta);
    const long double radius = static_cast<long double>(circle.radius);
    const long double radius2 = radius * radius;
    const long double eps = tolerance(1 + radius2 + distance2);
    if (radius == 0) {
        result.push_back(center);
        return result;
    }
    if (distance2 < radius2 - eps) return result;
    if (fabsl(distance2 - radius2) <= eps) {
        result.push_back(point);
        return result;
    }
    const auto base = center + delta * (radius2 / distance2);
    const long double factor = radius * sqrt(max(0.0L, distance2 - radius2))
                             / distance2;
    const Point2<long double> normal{-delta.y(), delta.x()};
    result.push_back(base + normal * factor);
    result.push_back(base - normal * factor);
    return result;
}

// @acf fn common_tangents — (circle1,circle2) -> vector<Line<Point2<ld>>> — 2円の共通接線を返す
/// @acf-doc @brief 2円の共通接線を0〜4本返す。同心円や同一円では空配列
/// @acf-doc [function] common_tangents
template <class C1, class R1, class C2, class R2>
auto common_tangents(const Circle<C1, R1>& first,
                     const Circle<C2, R2>& second) {
    using namespace ac_detail::geometry;
    vector<Line<Point2<long double>>> result;
    const auto a = point2_ld(first.center), b = point2_ld(second.center);
    const auto delta = b - a;
    const long double distance2 = dot_product(delta, delta);
    const long double r1 = static_cast<long double>(first.radius);
    const long double r2 = static_cast<long double>(second.radius);
    const long double eps = tolerance(1 + distance2 + r1 * r1 + r2 * r2);
    if (distance2 <= eps) return result;
    for (int side : {-1, 1}) {
        const long double radius_delta = r1 - side * r2;
        const long double height2 = distance2 - radius_delta * radius_delta;
        if (height2 < -eps) continue;
        const long double height = sqrt(max(0.0L, height2));
        const int direction_count = height2 <= eps ? 1 : 2;
        for (int i = 0; i < direction_count; ++i) {
            const long double sign = i == 0 ? 1.0L : -1.0L;
            const long double nx = (delta.x() * radius_delta
                                   + delta.y() * height * sign) / distance2;
            const long double ny = (delta.y() * radius_delta
                                   - delta.x() * height * sign) / distance2;
            const Point2<long double> normal{nx, ny};
            const auto p1 = a + normal * r1;
            const auto p2 = b + normal * (side * r2);
            const Point2<long double> tangent_direction{-normal.y(), normal.x()};
            result.push_back(make_line(
                p1, same_point2d(p1, p2) ? p1 + tangent_direction : p2));
        }
    }
    return result;
}

// @acf fn make_circumcircle — (a,b,c) / (x1,y1,x2,y2,x3,y3) -> Circle<Point2<ld>,ld> — 3点の外接円を作る
/// @acf-doc @brief 一直線上でない3点を通る円を返す。6引数の生座標版あり
/// @acf-doc [function] make_circumcircle
template <class A, class B, class C>
auto make_circumcircle(const A& a_value, const B& b_value, const C& c_value) {
    using namespace ac_detail::geometry;
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value);
    const long double denominator = 2 * cross2d_ld(a, b, c);
    assert(fabsl(denominator) > tolerance(
        1 + vector_norm(b - a) * vector_norm(c - a)));
    const long double aa = dot_product(a, a);
    const long double bb = dot_product(b, b);
    const long double cc = dot_product(c, c);
    const Point2<long double> center{
        (aa * (b.y() - c.y()) + bb * (c.y() - a.y())
             + cc * (a.y() - b.y())) / denominator,
        (aa * (c.x() - b.x()) + bb * (a.x() - c.x())
             + cc * (b.x() - a.x())) / denominator};
    return make_circle(center, euclidean_distance(center, a));
}

template <class A, class B, class C, class D, class E, class F>
auto make_circumcircle(A x1, B y1, C x2, D y2, E x3, F y3) {
    return make_circumcircle(pair{x1, y1}, pair{x2, y2}, pair{x3, y3});
}

// @acf fn make_incircle — (a,b,c) / (x1,y1,x2,y2,x3,y3) -> Circle<Point2<ld>,ld> — 三角形の内接円を作る
/// @acf-doc @brief 一直線上でない3点から三角形の内接円を返す。6引数の生座標版あり
/// @acf-doc [function] make_incircle
template <class A, class B, class C>
auto make_incircle(const A& a_value, const B& b_value, const C& c_value) {
    using namespace ac_detail::geometry;
    const auto a = point2_ld(a_value), b = point2_ld(b_value);
    const auto c = point2_ld(c_value);
    const long double opposite_a = euclidean_distance(b, c);
    const long double opposite_b = euclidean_distance(c, a);
    const long double opposite_c = euclidean_distance(a, b);
    const long double perimeter = opposite_a + opposite_b + opposite_c;
    const long double twice_area = fabsl(cross2d_ld(a, b, c));
    assert(perimeter > 0 && twice_area > tolerance(perimeter * perimeter));
    const auto center = (a * opposite_a + b * opposite_b + c * opposite_c)
                      / perimeter;
    return make_circle(center, twice_area / perimeter);
}

template <class A, class B, class C, class D, class E, class F>
auto make_incircle(A x1, B y1, C x2, D y2, E x3, F y3) {
    return make_incircle(pair{x1, y1}, pair{x2, y2}, pair{x3, y3});
}

struct Affine2D {
    array<array<long double, 3>, 3> matrix{};
};

inline ostream& operator<<(ostream& os, const Affine2D& transform) {
    return os << "Affine2D{matrix=" << transform.matrix << '}';
}

// @acf fn identity_transform — () -> Affine2D — 何も変えない2Dアフィン変換
/// @acf-doc @brief 単位変換を返す。変換の合成を始める初期値にも使える
/// @acf-doc [function] identity_transform
constexpr Affine2D identity_transform() {
    Affine2D result{};
    result.matrix[0] = {1, 0, 0};
    result.matrix[1] = {0, 1, 0};
    result.matrix[2] = {0, 0, 1};
    return result;
}

constexpr Affine2D operator*(const Affine2D& left, const Affine2D& right) {
    Affine2D result{};
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            for (size_t k = 0; k < 3; ++k)
                result.matrix[i][j] += left.matrix[i][k] * right.matrix[k][j];
    return result;
}

constexpr Affine2D& operator*=(Affine2D& left, const Affine2D& right) {
    return left = left * right;
}

// @acf fn translation_transform — (dx,dy) / (delta) -> Affine2D — 平行移動行列を作る
/// @acf-doc @brief x方向dx、y方向dyだけ動かす変換を返す
/// @acf-doc [function] translation_transform
template <class X, class Y>
constexpr Affine2D translation_transform(X dx, Y dy) {
    auto result = identity_transform();
    result.matrix[0][2] = static_cast<long double>(dx);
    result.matrix[1][2] = static_cast<long double>(dy);
    return result;
}

template <class P>
constexpr Affine2D translation_transform(const P& delta) {
    return translation_transform(ac_detail::geometry::x_ld(delta),
                                 ac_detail::geometry::y_ld(delta));
}

// --- trigonometry (geometry extension): rad は sinl/cosl、度は *_deg ---

// @acf fn to_rad — (deg) -> ld — 度数法をラジアンへ変換
/// @acf-doc @brief `deg * PI / 180`。幾何・`rotation_transform` はラジアン前提
/// @acf-doc @code (T deg) -> ld
/// @acf-doc [function] to_rad
template <class T>
constexpr ld to_rad(T deg) {
    return static_cast<ld>(deg) * PI / 180.0L;
}

// @acf fn to_deg — (rad) -> ld — ラジアンを度数法へ変換
/// @acf-doc @brief `rad * 180 / PI`
/// @acf-doc @code (T rad) -> ld
/// @acf-doc [function] to_deg
template <class T>
constexpr ld to_deg(T rad) {
    return static_cast<ld>(rad) * 180.0L / PI;
}

// @acf fn sin_deg — (deg) -> ld — 正弦（引数は度数）
/// @acf-doc @brief `sinl(to_rad(deg))`
/// @acf-doc [function] sin_deg
template <class T>
ld sin_deg(T deg) {
    return sinl(to_rad(deg));
}

// @acf fn cos_deg — (deg) -> ld — 余弦（引数は度数）
/// @acf-doc @brief `cosl(to_rad(deg))`
/// @acf-doc [function] cos_deg
template <class T>
ld cos_deg(T deg) {
    return cosl(to_rad(deg));
}

// @acf fn tan_deg — (deg) -> ld — 正接（引数は度数）
/// @acf-doc @brief `tanl(to_rad(deg))`
/// @acf-doc [function] tan_deg
template <class T>
ld tan_deg(T deg) {
    return tanl(to_rad(deg));
}

// @acf fn asin_deg — (x) -> ld — 逆正弦（戻り値は度数）
/// @acf-doc @brief `to_deg(asinl(x))`。定義域 `[-1,1]`、値域約 `[-90,90]`
/// @acf-doc [function] asin_deg
template <class T>
ld asin_deg(T x) {
    return to_deg(asinl(static_cast<ld>(x)));
}

// @acf fn acos_deg — (x) -> ld — 逆余弦（戻り値は度数）
/// @acf-doc @brief `to_deg(acosl(x))`。定義域 `[-1,1]`、値域約 `[0,180]`
/// @acf-doc [function] acos_deg
template <class T>
ld acos_deg(T x) {
    return to_deg(acosl(static_cast<ld>(x)));
}

// @acf fn atan_deg — (x) -> ld — 逆正接（戻り値は度数）
/// @acf-doc @brief `to_deg(atanl(x))`。値域約 `(-90,90)`
/// @acf-doc [function] atan_deg
template <class T>
ld atan_deg(T x) {
    return to_deg(atanl(static_cast<ld>(x)));
}

// @acf fn atan2_deg — (y,x) -> ld — 2引数逆正接（戻り値は度数）
/// @acf-doc @brief `to_deg(atan2l(y,x))`。値域約 `(-180,180]`
/// @acf-doc [function] atan2_deg
template <class Y, class X>
ld atan2_deg(Y y, X x) {
    return to_deg(atan2l(static_cast<ld>(y), static_cast<ld>(x)));
}

// @acf fn argument — (p) / (x,y) -> ld — 偏角（ラジアン、atan2）
/// @acf-doc @brief 点またはベクトルの偏角。`atan2(y,x)`、値域 `(-PI,PI]`
/// @acf-doc [function] argument
template <class P>
ld argument(const P& p) {
    return atan2l(ac_detail::geometry::y_ld(p), ac_detail::geometry::x_ld(p));
}

template <class X, class Y>
ld argument(X x, Y y) {
    return atan2l(static_cast<ld>(y), static_cast<ld>(x));
}

// @acf fn argument_deg — (p) / (x,y) -> ld — 偏角（度数）
/// @acf-doc @brief `to_deg(argument(...))`
/// @acf-doc [function] argument_deg
template <class P>
ld argument_deg(const P& p) {
    return to_deg(argument(p));
}

template <class X, class Y>
ld argument_deg(X x, Y y) {
    return to_deg(argument(x, y));
}

// @acf fn oriented_angle — (u,v) / (ux,uy,vx,vy) -> ld — u から v への向き付き角（ラジアン）
/// @acf-doc @brief `atan2(cross,dot)`。値域 `(-PI,PI]`。反時計が正。4引数の生座標版あり
/// @acf-doc [function] oriented_angle
template <class U, class V>
ld oriented_angle(const U& u, const V& v) {
    const ld cross = static_cast<ld>(cross_product2d(u, v));
    const ld dot = static_cast<ld>(dot_product(u, v));
    return atan2l(cross, dot);
}

template <class A, class B, class C, class D>
ld oriented_angle(A ux, B uy, C vx, D vy) {
    return oriented_angle(pair{ux, uy}, pair{vx, vy});
}

// @acf fn oriented_angle_deg — (u,v) / (ux,uy,vx,vy) -> ld — u から v への向き付き角（度数）
/// @acf-doc @brief `to_deg(oriented_angle(...))`
/// @acf-doc [function] oriented_angle_deg
template <class U, class V>
ld oriented_angle_deg(const U& u, const V& v) {
    return to_deg(oriented_angle(u, v));
}

template <class A, class B, class C, class D>
ld oriented_angle_deg(A ux, B uy, C vx, D vy) {
    return to_deg(oriented_angle(ux, uy, vx, vy));
}

// @acf fn angle_between — (u,v) / (ux,uy,vx,vy) -> ld — 2ベクトルの夾角（ラジアン、[0,PI]）
/// @acf-doc @brief `abs(oriented_angle(...))`。4引数の生座標版あり
/// @acf-doc [function] angle_between
template <class U, class V>
ld angle_between(const U& u, const V& v) {
    return fabsl(oriented_angle(u, v));
}

template <class A, class B, class C, class D>
ld angle_between(A ux, B uy, C vx, D vy) {
    return fabsl(oriented_angle(ux, uy, vx, vy));
}

// @acf fn angle_between_deg — (u,v) / (ux,uy,vx,vy) -> ld — 2ベクトルの夾角（度数、[0,180]）
/// @acf-doc @brief `to_deg(angle_between(...))`
/// @acf-doc [function] angle_between_deg
template <class U, class V>
ld angle_between_deg(const U& u, const V& v) {
    return to_deg(angle_between(u, v));
}

template <class A, class B, class C, class D>
ld angle_between_deg(A ux, B uy, C vx, D vy) {
    return to_deg(angle_between(ux, uy, vx, vy));
}

// @acf fn polar_point — (r,theta) -> Point2<ld> — 極座標（ラジアン）から点を作る
/// @acf-doc @brief `(r cos θ, r sin θ)`。θ はラジアン
/// @acf-doc [function] polar_point
template <class R, class Theta>
Point2<ld> polar_point(R r, Theta theta) {
    const ld radius = static_cast<ld>(r);
    const ld ang = static_cast<ld>(theta);
    return Point2<ld>{radius * cosl(ang), radius * sinl(ang)};
}

// @acf fn polar_point_deg — (r,deg) -> Point2<ld> — 極座標（度数）から点を作る
/// @acf-doc @brief `polar_point(r, to_rad(deg))`
/// @acf-doc [function] polar_point_deg
template <class R, class Deg>
Point2<ld> polar_point_deg(R r, Deg deg) {
    return polar_point(r, to_rad(deg));
}

// @acf fn to_polar — (p) / (x,y) -> pair<ld,ld> — 直交→極（r, θラジアン）
/// @acf-doc @brief `{vector_norm, argument}`。原点は `{0,0}`
/// @acf-doc [function] to_polar
template <class P>
pair<ld, ld> to_polar(const P& p) {
    const ld x = ac_detail::geometry::x_ld(p);
    const ld y = ac_detail::geometry::y_ld(p);
    return {hypotl(x, y), atan2l(y, x)};
}

template <class X, class Y>
pair<ld, ld> to_polar(X x, Y y) {
    const ld xx = static_cast<ld>(x);
    const ld yy = static_cast<ld>(y);
    return {hypotl(xx, yy), atan2l(yy, xx)};
}

// @acf fn to_polar_deg — (p) / (x,y) -> pair<ld,ld> — 直交→極（r, θ度数）
/// @acf-doc @brief `{r, to_deg(θ)}`
/// @acf-doc [function] to_polar_deg
template <class P>
pair<ld, ld> to_polar_deg(const P& p) {
    auto [r, theta] = to_polar(p);
    return {r, to_deg(theta)};
}

template <class X, class Y>
pair<ld, ld> to_polar_deg(X x, Y y) {
    auto [r, theta] = to_polar(x, y);
    return {r, to_deg(theta)};
}

// @acf fn law_of_cosines_side — (a,b,C) -> ld — 余弦定理で対辺 c（Cはラジアン）
/// @acf-doc @brief `c² = a²+b²-2ab cos C`
/// @acf-doc [function] law_of_cosines_side
template <class A, class B, class C>
ld law_of_cosines_side(A a, B b, C C_rad) {
    const ld aa = static_cast<ld>(a);
    const ld bb = static_cast<ld>(b);
    const ld ang = static_cast<ld>(C_rad);
    return sqrtl(aa * aa + bb * bb - 2.0L * aa * bb * cosl(ang));
}

// @acf fn law_of_cosines_side_deg — (a,b,C_deg) -> ld — 余弦定理で対辺 c（Cは度数）
/// @acf-doc @brief `law_of_cosines_side(a,b,to_rad(C_deg))`
/// @acf-doc [function] law_of_cosines_side_deg
template <class A, class B, class C>
ld law_of_cosines_side_deg(A a, B b, C C_deg) {
    return law_of_cosines_side(a, b, to_rad(C_deg));
}

// @acf fn law_of_cosines_angle — (a,b,c) -> ld — 余弦定理で角 C（ラジアン）
/// @acf-doc @brief `cos C = (a²+b²-c²)/(2ab)`。戻り値は `[0,PI]`
/// @acf-doc [function] law_of_cosines_angle
template <class A, class B, class C>
ld law_of_cosines_angle(A a, B b, C c) {
    const ld aa = static_cast<ld>(a);
    const ld bb = static_cast<ld>(b);
    const ld cc = static_cast<ld>(c);
    const ld denom = 2.0L * aa * bb;
    assert(denom != 0);
    ld cos_c = (aa * aa + bb * bb - cc * cc) / denom;
    if (cos_c > 1.0L) cos_c = 1.0L;
    if (cos_c < -1.0L) cos_c = -1.0L;
    return acosl(cos_c);
}

// @acf fn law_of_cosines_angle_deg — (a,b,c) -> ld — 余弦定理で角 C（度数）
/// @acf-doc @brief `to_deg(law_of_cosines_angle(a,b,c))`
/// @acf-doc [function] law_of_cosines_angle_deg
template <class A, class B, class C>
ld law_of_cosines_angle_deg(A a, B b, C c) {
    return to_deg(law_of_cosines_angle(a, b, c));
}

// @acf fn law_of_sines_side — (a,A,B) -> ld — 正弦定理で辺 b（角はラジアン）
/// @acf-doc @brief `b = a * sin(B) / sin(A)`。`sin(A)≠0` を前提
/// @acf-doc [function] law_of_sines_side
template <class Side, class AngA, class AngB>
ld law_of_sines_side(Side a, AngA A_rad, AngB B_rad) {
    const ld sin_a = sinl(static_cast<ld>(A_rad));
    assert(fabsl(sin_a) > EPS);
    return static_cast<ld>(a) * sinl(static_cast<ld>(B_rad)) / sin_a;
}

// @acf fn law_of_sines_side_deg — (a,A_deg,B_deg) -> ld — 正弦定理で辺 b（角は度数）
/// @acf-doc @brief `law_of_sines_side(a,to_rad(A),to_rad(B))`
/// @acf-doc [function] law_of_sines_side_deg
template <class Side, class AngA, class AngB>
ld law_of_sines_side_deg(Side a, AngA A_deg, AngB B_deg) {
    return law_of_sines_side(a, to_rad(A_deg), to_rad(B_deg));
}

// @acf fn law_of_sines_angle — (a,b,A) -> ld — 正弦定理で角 B（ラジアン、鋭角側）
/// @acf-doc @brief `B = asin(b sin A / a)`。主値（鋭角側）。鈍角は `PI-B` を別途検討
/// @acf-doc [function] law_of_sines_angle
template <class SideA, class SideB, class AngA>
ld law_of_sines_angle(SideA a, SideB b, AngA A_rad) {
    const ld aa = static_cast<ld>(a);
    assert(fabsl(aa) > EPS);
    ld sin_b = static_cast<ld>(b) * sinl(static_cast<ld>(A_rad)) / aa;
    if (sin_b > 1.0L) sin_b = 1.0L;
    if (sin_b < -1.0L) sin_b = -1.0L;
    return asinl(sin_b);
}

// @acf fn law_of_sines_angle_deg — (a,b,A_deg) -> ld — 正弦定理で角 B（度数、鋭角側）
/// @acf-doc @brief `to_deg(law_of_sines_angle(a,b,to_rad(A_deg)))`
/// @acf-doc [function] law_of_sines_angle_deg
template <class SideA, class SideB, class AngA>
ld law_of_sines_angle_deg(SideA a, SideB b, AngA A_deg) {
    return to_deg(law_of_sines_angle(a, b, to_rad(A_deg)));
}

// @acf fn sin_sum — (a,b) -> ld — 加法定理 sin(a+b)（ラジアン、公式評価）
/// @acf-doc @brief `sin a cos b + cos a sin b`。`sinl(a+b)` と誤差経路は異なる場合あり
/// @acf-doc [function] sin_sum
template <class A, class B>
ld sin_sum(A a, B b) {
    const ld aa = static_cast<ld>(a), bb = static_cast<ld>(b);
    return sinl(aa) * cosl(bb) + cosl(aa) * sinl(bb);
}

// @acf fn sin_sum_deg — (a,b) -> ld — 加法定理 sin(a+b)（度数入力）
/// @acf-doc @brief `sin_sum(to_rad(a),to_rad(b))`
/// @acf-doc [function] sin_sum_deg
template <class A, class B>
ld sin_sum_deg(A a, B b) {
    return sin_sum(to_rad(a), to_rad(b));
}

// @acf fn sin_diff — (a,b) -> ld — 減法定理 sin(a-b)（ラジアン、公式評価）
/// @acf-doc @brief `sin a cos b - cos a sin b`
/// @acf-doc [function] sin_diff
template <class A, class B>
ld sin_diff(A a, B b) {
    const ld aa = static_cast<ld>(a), bb = static_cast<ld>(b);
    return sinl(aa) * cosl(bb) - cosl(aa) * sinl(bb);
}

// @acf fn sin_diff_deg — (a,b) -> ld — 減法定理 sin(a-b)（度数入力）
/// @acf-doc @brief `sin_diff(to_rad(a),to_rad(b))`
/// @acf-doc [function] sin_diff_deg
template <class A, class B>
ld sin_diff_deg(A a, B b) {
    return sin_diff(to_rad(a), to_rad(b));
}

// @acf fn cos_sum — (a,b) -> ld — 加法定理 cos(a+b)（ラジアン、公式評価）
/// @acf-doc @brief `cos a cos b - sin a sin b`
/// @acf-doc [function] cos_sum
template <class A, class B>
ld cos_sum(A a, B b) {
    const ld aa = static_cast<ld>(a), bb = static_cast<ld>(b);
    return cosl(aa) * cosl(bb) - sinl(aa) * sinl(bb);
}

// @acf fn cos_sum_deg — (a,b) -> ld — 加法定理 cos(a+b)（度数入力）
/// @acf-doc @brief `cos_sum(to_rad(a),to_rad(b))`
/// @acf-doc [function] cos_sum_deg
template <class A, class B>
ld cos_sum_deg(A a, B b) {
    return cos_sum(to_rad(a), to_rad(b));
}

// @acf fn cos_diff — (a,b) -> ld — 減法定理 cos(a-b)（ラジアン、公式評価）
/// @acf-doc @brief `cos a cos b + sin a sin b`
/// @acf-doc [function] cos_diff
template <class A, class B>
ld cos_diff(A a, B b) {
    const ld aa = static_cast<ld>(a), bb = static_cast<ld>(b);
    return cosl(aa) * cosl(bb) + sinl(aa) * sinl(bb);
}

// @acf fn cos_diff_deg — (a,b) -> ld — 減法定理 cos(a-b)（度数入力）
/// @acf-doc @brief `cos_diff(to_rad(a),to_rad(b))`
/// @acf-doc [function] cos_diff_deg
template <class A, class B>
ld cos_diff_deg(A a, B b) {
    return cos_diff(to_rad(a), to_rad(b));
}

// @acf fn tan_sum — (a,b) -> ld — 加法定理 tan(a+b)（ラジアン、公式評価）
/// @acf-doc @brief `(tan a + tan b)/(1 - tan a tan b)`
/// @acf-doc [function] tan_sum
template <class A, class B>
ld tan_sum(A a, B b) {
    const ld ta = tanl(static_cast<ld>(a));
    const ld tb = tanl(static_cast<ld>(b));
    return (ta + tb) / (1.0L - ta * tb);
}

// @acf fn tan_sum_deg — (a,b) -> ld — 加法定理 tan(a+b)（度数入力）
/// @acf-doc @brief `tan_sum(to_rad(a),to_rad(b))`
/// @acf-doc [function] tan_sum_deg
template <class A, class B>
ld tan_sum_deg(A a, B b) {
    return tan_sum(to_rad(a), to_rad(b));
}

// @acf fn tan_diff — (a,b) -> ld — 減法定理 tan(a-b)（ラジアン、公式評価）
/// @acf-doc @brief `(tan a - tan b)/(1 + tan a tan b)`
/// @acf-doc [function] tan_diff
template <class A, class B>
ld tan_diff(A a, B b) {
    const ld ta = tanl(static_cast<ld>(a));
    const ld tb = tanl(static_cast<ld>(b));
    return (ta - tb) / (1.0L + ta * tb);
}

// @acf fn tan_diff_deg — (a,b) -> ld — 減法定理 tan(a-b)（度数入力）
/// @acf-doc @brief `tan_diff(to_rad(a),to_rad(b))`
/// @acf-doc [function] tan_diff_deg
template <class A, class B>
ld tan_diff_deg(A a, B b) {
    return tan_diff(to_rad(a), to_rad(b));
}

// @acf fn sin_double — (a) -> ld — 倍角 sin(2a)（ラジアン、公式評価）
/// @acf-doc @brief `2 sin a cos a`
/// @acf-doc [function] sin_double
template <class A>
ld sin_double(A a) {
    const ld aa = static_cast<ld>(a);
    return 2.0L * sinl(aa) * cosl(aa);
}

// @acf fn sin_double_deg — (a) -> ld — 倍角 sin(2a)（度数入力）
/// @acf-doc @brief `sin_double(to_rad(a))`
/// @acf-doc [function] sin_double_deg
template <class A>
ld sin_double_deg(A a) {
    return sin_double(to_rad(a));
}

// @acf fn cos_double — (a) -> ld — 倍角 cos(2a)（ラジアン、公式評価）
/// @acf-doc @brief `cos²a - sin²a`
/// @acf-doc [function] cos_double
template <class A>
ld cos_double(A a) {
    const ld aa = static_cast<ld>(a);
    const ld c = cosl(aa), s = sinl(aa);
    return c * c - s * s;
}

// @acf fn cos_double_deg — (a) -> ld — 倍角 cos(2a)（度数入力）
/// @acf-doc @brief `cos_double(to_rad(a))`
/// @acf-doc [function] cos_double_deg
template <class A>
ld cos_double_deg(A a) {
    return cos_double(to_rad(a));
}

// @acf fn tan_double — (a) -> ld — 倍角 tan(2a)（ラジアン、公式評価）
/// @acf-doc @brief `2 tan a / (1 - tan²a)`
/// @acf-doc [function] tan_double
template <class A>
ld tan_double(A a) {
    const ld t = tanl(static_cast<ld>(a));
    return 2.0L * t / (1.0L - t * t);
}

// @acf fn tan_double_deg — (a) -> ld — 倍角 tan(2a)（度数入力）
/// @acf-doc @brief `tan_double(to_rad(a))`
/// @acf-doc [function] tan_double_deg
template <class A>
ld tan_double_deg(A a) {
    return tan_double(to_rad(a));
}

// @acf fn sin_half — (a) -> ld — 半角 |sin(a/2)|（ラジアン、非負）
/// @acf-doc @brief `sqrt((1-cos a)/2)`。非負の主値
/// @acf-doc [function] sin_half
template <class A>
ld sin_half(A a) {
    return sqrtl((1.0L - cosl(static_cast<ld>(a))) / 2.0L);
}

// @acf fn sin_half_deg — (a) -> ld — 半角 |sin(a/2)|（度数入力、非負）
/// @acf-doc @brief `sin_half(to_rad(a))`
/// @acf-doc [function] sin_half_deg
template <class A>
ld sin_half_deg(A a) {
    return sin_half(to_rad(a));
}

// @acf fn cos_half — (a) -> ld — 半角 |cos(a/2)|（ラジアン、非負）
/// @acf-doc @brief `sqrt((1+cos a)/2)`。非負の主値
/// @acf-doc [function] cos_half
template <class A>
ld cos_half(A a) {
    return sqrtl((1.0L + cosl(static_cast<ld>(a))) / 2.0L);
}

// @acf fn cos_half_deg — (a) -> ld — 半角 |cos(a/2)|（度数入力、非負）
/// @acf-doc @brief `cos_half(to_rad(a))`
/// @acf-doc [function] cos_half_deg
template <class A>
ld cos_half_deg(A a) {
    return cos_half(to_rad(a));
}

// @acf fn tan_half — (a) -> ld — 半角 tan(a/2)（ラジアン、公式評価）
/// @acf-doc @brief `sin a / (1 + cos a)`（安定形）。`1+cos a≈0` のときは別式を検討
/// @acf-doc [function] tan_half
template <class A>
ld tan_half(A a) {
    const ld aa = static_cast<ld>(a);
    const ld c = cosl(aa);
    const ld denom = 1.0L + c;
    if (fabsl(denom) > EPS) return sinl(aa) / denom;
    return (1.0L - c) / sinl(aa);
}

// @acf fn tan_half_deg — (a) -> ld — 半角 tan(a/2)（度数入力）
/// @acf-doc @brief `tan_half(to_rad(a))`
/// @acf-doc [function] tan_half_deg
template <class A>
ld tan_half_deg(A a) {
    return tan_half(to_rad(a));
}

// @acf fn rotation_transform — (radians[,origin]) -> Affine2D — 回転行列を作る
/// @acf-doc @brief 反時計回りの回転。角度はラジアン。origin省略時は原点中心。度数は `rotation_transform_deg` か `to_rad`
/// @acf-doc [function] rotation_transform
inline Affine2D rotation_transform(long double radians) {
    const long double cosine = cosl(radians);
    const long double sine = sinl(radians);
    Affine2D result{};
    result.matrix[0] = {cosine, -sine, 0};
    result.matrix[1] = {sine, cosine, 0};
    result.matrix[2] = {0, 0, 1};
    return result;
}

template <class P>
Affine2D rotation_transform(long double radians, const P& origin) {
    using namespace ac_detail::geometry;
    return translation_transform(x_ld(origin), y_ld(origin))
         * rotation_transform(radians)
         * translation_transform(-x_ld(origin), -y_ld(origin));
}

// @acf fn rotation_transform_deg — (deg[,origin]) -> Affine2D — 度数で回転行列を作る
/// @acf-doc @brief `rotation_transform(to_rad(deg)[,origin])`
/// @acf-doc [function] rotation_transform_deg
template <class Deg>
Affine2D rotation_transform_deg(Deg deg) {
    return rotation_transform(to_rad(deg));
}

template <class Deg, class P>
Affine2D rotation_transform_deg(Deg deg, const P& origin) {
    return rotation_transform(to_rad(deg), origin);
}

// @acf fn scaling_transform — (sx,sy[,origin]) -> Affine2D — 拡大縮小行列を作る
/// @acf-doc @brief x方向sx、y方向syに拡大縮小する。origin省略時は原点中心
/// @acf-doc [function] scaling_transform
template <class X, class Y>
constexpr Affine2D scaling_transform(X sx, Y sy) {
    Affine2D result{};
    result.matrix[0] = {static_cast<long double>(sx), 0, 0};
    result.matrix[1] = {0, static_cast<long double>(sy), 0};
    result.matrix[2] = {0, 0, 1};
    return result;
}

template <class X, class Y, class P>
Affine2D scaling_transform(X sx, Y sy, const P& origin) {
    using namespace ac_detail::geometry;
    return translation_transform(x_ld(origin), y_ld(origin))
         * scaling_transform(sx, sy)
         * translation_transform(-x_ld(origin), -y_ld(origin));
}

// @acf fn shear_transform — (shx,shy) -> Affine2D — せん断行列を作る
/// @acf-doc @brief `(x,y)` を `(x+shx*y, shy*x+y)` へ写す
/// @acf-doc [function] shear_transform
template <class X, class Y>
constexpr Affine2D shear_transform(X shx, Y shy) {
    Affine2D result{};
    result.matrix[0] = {1, static_cast<long double>(shx), 0};
    result.matrix[1] = {static_cast<long double>(shy), 1, 0};
    result.matrix[2] = {0, 0, 1};
    return result;
}

// @acf fn apply_transform — (transform,point/shape) -> transformed value — アフィン変換を適用する
/// @acf-doc @brief 点、直線、線分、三角形、多角形へ2Dアフィン変換を適用する
/// @acf-doc [function] apply_transform
template <class P>
auto apply_transform(const Affine2D& transform, const P& point) {
    using namespace ac_detail::geometry;
    const long double x = x_ld(point), y = y_ld(point);
    const long double tx = transform.matrix[0][0] * x
                         + transform.matrix[0][1] * y
                         + transform.matrix[0][2];
    const long double ty = transform.matrix[1][0] * x
                         + transform.matrix[1][1] * y
                         + transform.matrix[1][2];
    const long double tw = transform.matrix[2][0] * x
                         + transform.matrix[2][1] * y
                         + transform.matrix[2][2];
    assert(tw != 0);
    return Point2<long double>{tx / tw, ty / tw};
}

template <class A, class B>
auto apply_transform(const Affine2D& transform, const Line<A, B>& shape) {
    return make_line(apply_transform(transform, shape.a),
                     apply_transform(transform, shape.b));
}

template <class A, class B>
auto apply_transform(const Affine2D& transform, const Segment<A, B>& shape) {
    return make_segment(apply_transform(transform, shape.a),
                        apply_transform(transform, shape.b));
}

template <class A, class B, class C>
auto apply_transform(const Affine2D& transform, const Triangle<A, B, C>& shape) {
    return make_triangle(apply_transform(transform, shape.a),
                         apply_transform(transform, shape.b),
                         apply_transform(transform, shape.c));
}

template <class P>
auto apply_transform(const Affine2D& transform, const Polygon<P>& shape) {
    vector<Point2<long double>> points;
    points.reserve(shape.vertices.size());
    for (const auto& point : shape.vertices)
        points.push_back(apply_transform(transform, point));
    return make_polygon(std::move(points));
}

template <class A, class B>
auto apply_transform(const Affine2D& transform, const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    const auto a = point2_ld(shape.corner1), c = point2_ld(shape.corner2);
    const Point2<long double> b{c.x(), a.y()}, d{a.x(), c.y()};
    return apply_transform(transform, make_polygon(vector{a, b, c, d}));
}

template <class P, class S>
auto apply_transform(const Affine2D& transform, const Square<P, S>& shape) {
    using namespace ac_detail::geometry;
    const auto a = point2_ld(shape.corner);
    const long double side = static_cast<long double>(shape.side);
    return apply_transform(transform, make_polygon(vector{
        a, Point2<long double>{a.x() + side, a.y()},
        Point2<long double>{a.x() + side, a.y() + side},
        Point2<long double>{a.x(), a.y() + side}}));
}

// @acf fn inverse_transform — (Affine2D) -> Affine2D — アフィン変換の逆変換を返す
/// @acf-doc @brief 逆行列を返す。行列式0の変換には使えない
/// @acf-doc [function] inverse_transform
inline Affine2D inverse_transform(const Affine2D& transform) {
    const auto& m = transform.matrix;
    const long double determinant =
          m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
        - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
        + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    assert(fabsl(determinant) > 1e-18L);
    Affine2D result{};
    result.matrix[0] = {
        (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / determinant,
        (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / determinant,
        (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / determinant};
    result.matrix[1] = {
        (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / determinant,
        (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / determinant,
        (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / determinant};
    result.matrix[2] = {
        (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / determinant,
        (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / determinant,
        (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / determinant};
    return result;
}

// @acf fn area — (shape) -> long double — 2D図形の面積（直線・線分は0）
/// @acf-doc @brief `area(shape)` は基本2D図形の面積を返す。直線・線分は0。多角形は凸・凹と頂点の周回方向を問わない
/// @acf-doc [function] area
template <class A, class B>
long double area(const Line<A, B>&) {
    return 0;
}

template <class A, class B>
long double area(const Segment<A, B>&) {
    return 0;
}

template <class P, class R>
long double area(const Circle<P, R>& shape) {
    const long double r = static_cast<long double>(shape.radius);
    return acosl(-1.0L) * r * r;
}

template <class A, class B>
long double area(const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    return fabsl(x_ld(shape.corner1) - x_ld(shape.corner2))
         * fabsl(y_ld(shape.corner1) - y_ld(shape.corner2));
}

template <class P, class S>
long double area(const Square<P, S>& shape) {
    const long double side = static_cast<long double>(shape.side);
    return side * side;
}

template <class A, class B, class C>
long double area(const Triangle<A, B, C>& shape) {
    using namespace ac_detail::geometry;
    return fabsl((x_ld(shape.b) - x_ld(shape.a)) * (y_ld(shape.c) - y_ld(shape.a))
               - (y_ld(shape.b) - y_ld(shape.a)) * (x_ld(shape.c) - x_ld(shape.a))) / 2;
}

template <class P>
long double area(const Polygon<P>& shape) {
    using namespace ac_detail::geometry;
    long double twice = 0;
    const size_t n = shape.vertices.size();
    if (n < 3) return 0;
    for (size_t i = 0; i < n; ++i) {
        const auto& a = shape.vertices[i];
        const auto& b = shape.vertices[(i + 1) % n];
        twice += x_ld(a) * y_ld(b) - y_ld(a) * x_ld(b);
    }
    return fabsl(twice) / 2;
}

// @acf fn perimeter — (shape) -> long double — 2D図形の周長（線分は長さ）
/// @acf-doc @brief `perimeter(shape)` は基本2D図形の周長を返す。線分は端点間距離
/// @acf-doc [function] perimeter
template <class A, class B>
long double perimeter(const Segment<A, B>& shape) {
    return segment_length(shape);
}

template <class P, class R>
long double perimeter(const Circle<P, R>& shape) {
    return 2 * acosl(-1.0L) * static_cast<long double>(shape.radius);
}

template <class A, class B>
long double perimeter(const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    return 2 * (fabsl(x_ld(shape.corner1) - x_ld(shape.corner2))
              + fabsl(y_ld(shape.corner1) - y_ld(shape.corner2)));
}

template <class P, class S>
long double perimeter(const Square<P, S>& shape) {
    return 4 * static_cast<long double>(shape.side);
}

template <class A, class B, class C>
long double perimeter(const Triangle<A, B, C>& shape) {
    using namespace ac_detail::geometry;
    return distance2d(shape.a, shape.b) + distance2d(shape.b, shape.c)
         + distance2d(shape.c, shape.a);
}

template <class P>
long double perimeter(const Polygon<P>& shape) {
    using namespace ac_detail::geometry;
    long double result = 0;
    const size_t n = shape.vertices.size();
    if (n < 2) return 0;
    for (size_t i = 0; i < n; ++i)
        result += distance2d(shape.vertices[i], shape.vertices[(i + 1) % n]);
    return result;
}

// @acf fn surface_area — (Sphere/Cuboid) -> long double — 3次元図形の表面積
/// @acf-doc @brief 球または座標軸に平行な直方体の表面積を返す
/// @acf-doc [function] surface_area
template <class P, class R>
long double surface_area(const Sphere<P, R>& shape) {
    const long double radius = static_cast<long double>(shape.radius);
    return 4 * acosl(-1.0L) * radius * radius;
}

template <class A, class B>
long double surface_area(const Cuboid<A, B>& shape) {
    using namespace ac_detail::geometry;
    const long double dx = fabsl(x_ld(shape.corner1) - x_ld(shape.corner2));
    const long double dy = fabsl(y_ld(shape.corner1) - y_ld(shape.corner2));
    const long double dz = fabsl(z_ld(shape.corner1) - z_ld(shape.corner2));
    return 2 * (dx * dy + dy * dz + dz * dx);
}

// @acf fn volume — (Sphere/Cuboid) -> long double — 3次元図形の体積
/// @acf-doc @brief 球または座標軸に平行な直方体の体積を返す
/// @acf-doc [function] volume
template <class P, class R>
long double volume(const Sphere<P, R>& shape) {
    const long double radius = static_cast<long double>(shape.radius);
    return 4 * acosl(-1.0L) * radius * radius * radius / 3;
}

template <class A, class B>
long double volume(const Cuboid<A, B>& shape) {
    using namespace ac_detail::geometry;
    return fabsl(x_ld(shape.corner1) - x_ld(shape.corner2))
         * fabsl(y_ld(shape.corner1) - y_ld(shape.corner2))
         * fabsl(z_ld(shape.corner1) - z_ld(shape.corner2));
}

// @acf fn contains_point — (shape,p) -> bool — 2D・3D図形の境界を含む点包含判定
/// @acf-doc @brief `contains_point(shape,p)` は点が図形の内部または境界上ならtrue。整数座標の線分・直線・多角形・三角形・長方形・円（整数半径）は厳密判定
/// @acf-doc [function] contains_point
template <class P, class R, class Q>
bool contains_point(const Circle<P, R>& shape, const Q& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_point2<remove_cv_t<remove_reference_t<P>>>::value
                  && is_integral_v<remove_cv_t<R>>
                  && integral_point2<remove_cv_t<remove_reference_t<Q>>>::value) {
        return circle_contains_exact(shape.center, shape.radius, p);
    } else {
        const long double radius = static_cast<long double>(shape.radius);
        return distance2d(shape.center, p) <= radius + tolerance(radius);
    }
}

template <class A, class B, class P>
bool contains_point(const Rectangle<A, B>& shape, const P& p) {
    return ac_detail::geometry::in_rectangle(p, shape.corner1, shape.corner2);
}

template <class Q, class S, class P>
bool contains_point(const Square<Q, S>& shape, const P& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_point2<remove_cv_t<remove_reference_t<Q>>>::value
                  && is_integral_v<remove_cv_t<S>>
                  && integral_point2<remove_cv_t<remove_reference_t<P>>>::value) {
        using T = common_type_t<decay_t<decltype(x_val(shape.corner))>,
                                remove_cv_t<S>>;
        const pair<T, T> opposite{
            static_cast<T>(x_val(shape.corner)) + static_cast<T>(shape.side),
            static_cast<T>(y_val(shape.corner)) + static_cast<T>(shape.side)};
        return in_rectangle_exact(p, shape.corner, opposite);
    } else {
        const pair<long double, long double> opposite{
            x_ld(shape.corner) + static_cast<long double>(shape.side),
            y_ld(shape.corner) + static_cast<long double>(shape.side)};
        return in_rectangle(p, shape.corner, opposite);
    }
}

template <class A, class B, class P>
bool contains_point(const Segment<A, B>& shape, const P& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, P>)
        return on_segment_exact(p, shape.a, shape.b);
    else
        return on_segment2d(p, shape.a, shape.b);
}

template <class A, class B, class P>
bool contains_point(const Line<A, B>& shape, const P& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, P>) {
        return on_line_exact(shape.a, shape.b, p);
    } else {
        const long double scale = 1 + distance2d(shape.a, shape.b);
        return point_line_distance(x_ld(p), y_ld(p), x_ld(shape.a), y_ld(shape.a),
                                   x_ld(shape.b), y_ld(shape.b)) <= tolerance(scale);
    }
}

template <class P, class Q>
bool contains_point(const Polygon<P>& shape, const Q& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_point2<remove_cv_t<P>>::value
                  && integral_point2<remove_cv_t<remove_reference_t<Q>>>::value) {
        return point_in_polygon_exact(shape, p);
    } else {
        const size_t n = shape.vertices.size();
        if (n == 0) return false;
        bool inside = false;
        const long double x = x_ld(p), y = y_ld(p);
        for (size_t i = 0, j = n - 1; i < n; j = i++) {
            const auto& a = shape.vertices[j];
            const auto& b = shape.vertices[i];
            if (on_segment2d(p, a, b)) return true;
            const long double ay = y_ld(a), by = y_ld(b);
            if ((ay > y) != (by > y)) {
                const long double crossing = x_ld(a)
                    + (x_ld(b) - x_ld(a)) * (y - ay) / (by - ay);
                if (x < crossing) inside = !inside;
            }
        }
        return inside;
    }
}

template <class A, class B, class C, class P>
bool contains_point(const Triangle<A, B, C>& shape, const P& p) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, C, P>) {
        return triangle_contains_exact(shape.a, shape.b, shape.c, p);
    } else {
        Polygon<pair<long double, long double>> polygon{{
            {x_ld(shape.a), y_ld(shape.a)},
            {x_ld(shape.b), y_ld(shape.b)},
            {x_ld(shape.c), y_ld(shape.c)},
        }};
        const pair<long double, long double> q{x_ld(p), y_ld(p)};
        return contains_point(polygon, q);
    }
}

template <class C, class R, class P>
bool contains_point(const Sphere<C, R>& shape, const P& point) {
    const long double radius = static_cast<long double>(shape.radius);
    return euclidean_distance(ac_detail::geometry::point3_ld(shape.center),
                              ac_detail::geometry::point3_ld(point))
        <= radius + ac_detail::geometry::tolerance(radius);
}

template <class A, class B, class P>
bool contains_point(const Cuboid<A, B>& shape, const P& point) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points3_v<A, B, P>) {
        return in_cuboid_exact(point, shape.corner1, shape.corner2);
    } else {
        const long double x = x_ld(point), y = y_ld(point), z = z_ld(point);
        const long double eps = tolerance(
            1 + fabsl(x_ld(shape.corner1)) + fabsl(x_ld(shape.corner2))
              + fabsl(y_ld(shape.corner1)) + fabsl(y_ld(shape.corner2))
              + fabsl(z_ld(shape.corner1)) + fabsl(z_ld(shape.corner2)));
        return min(x_ld(shape.corner1), x_ld(shape.corner2)) - eps <= x
            && x <= max(x_ld(shape.corner1), x_ld(shape.corner2)) + eps
            && min(y_ld(shape.corner1), y_ld(shape.corner2)) - eps <= y
            && y <= max(y_ld(shape.corner1), y_ld(shape.corner2)) + eps
            && min(z_ld(shape.corner1), z_ld(shape.corner2)) - eps <= z
            && z <= max(z_ld(shape.corner1), z_ld(shape.corner2)) + eps;
    }
}

// @acf fn point_shape_distance — (p,shape) -> long double — 点と2D・3D図形の最短距離
/// @acf-doc @brief `point_shape_distance(p,shape)` は点と図形の最短距離。面・体積を持つ図形では内部・境界なら0
/// @acf-doc [function] point_shape_distance
template <class P, class A, class B>
long double point_shape_distance(const P& p, const Line<A, B>& shape) {
    using namespace ac_detail::geometry;
    return point_line_distance(x_ld(p), y_ld(p), x_ld(shape.a), y_ld(shape.a),
                               x_ld(shape.b), y_ld(shape.b));
}

template <class P, class A, class B>
long double point_shape_distance(const P& p, const Segment<A, B>& shape) {
    return ac_detail::geometry::segment_distance2d(p, shape.a, shape.b);
}

template <class P, class C, class R>
long double point_shape_distance(const P& p, const Circle<C, R>& shape) {
    return max(0.0L, ac_detail::geometry::distance2d(p, shape.center)
                     - static_cast<long double>(shape.radius));
}

template <class P, class A, class B>
long double point_shape_distance(const P& p, const Rectangle<A, B>& shape) {
    using namespace ac_detail::geometry;
    const long double x = x_ld(p), y = y_ld(p);
    const long double left = min(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double right = max(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double bottom = min(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double top = max(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double dx = max({left - x, 0.0L, x - right});
    const long double dy = max({bottom - y, 0.0L, y - top});
    return hypot(dx, dy);
}

template <class P, class Q, class S>
long double point_shape_distance(const P& p, const Square<Q, S>& shape) {
    using namespace ac_detail::geometry;
    const pair<long double, long double> opposite{
        x_ld(shape.corner) + static_cast<long double>(shape.side),
        y_ld(shape.corner) + static_cast<long double>(shape.side)};
    return point_shape_distance(
        p, Rectangle<Q, pair<long double, long double>>{shape.corner, opposite});
}

template <class P, class Q>
long double point_shape_distance(const P& p, const Polygon<Q>& shape) {
    using namespace ac_detail::geometry;
    if (contains_point(shape, p)) return 0;
    if (shape.vertices.empty()) return numeric_limits<long double>::infinity();
    long double result = numeric_limits<long double>::infinity();
    for (size_t i = 0; i < shape.vertices.size(); ++i)
        result = min(result, segment_distance2d(
            p, shape.vertices[i], shape.vertices[(i + 1) % shape.vertices.size()]));
    return result;
}

template <class P, class A, class B, class C>
long double point_shape_distance(const P& p, const Triangle<A, B, C>& shape) {
    if (contains_point(shape, p)) return 0;
    return min({
        ac_detail::geometry::segment_distance2d(p, shape.a, shape.b),
        ac_detail::geometry::segment_distance2d(p, shape.b, shape.c),
        ac_detail::geometry::segment_distance2d(p, shape.c, shape.a),
    });
}

template <class P, class C, class R>
long double point_shape_distance(const P& point, const Sphere<C, R>& shape) {
    return max(0.0L,
        euclidean_distance(ac_detail::geometry::point3_ld(point),
                           ac_detail::geometry::point3_ld(shape.center))
        - static_cast<long double>(shape.radius));
}

template <class P, class A, class B>
long double point_shape_distance(const P& point, const Cuboid<A, B>& shape) {
    using namespace ac_detail::geometry;
    const long double x = x_ld(point), y = y_ld(point), z = z_ld(point);
    const long double left = min(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double right = max(x_ld(shape.corner1), x_ld(shape.corner2));
    const long double bottom = min(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double top = max(y_ld(shape.corner1), y_ld(shape.corner2));
    const long double back = min(z_ld(shape.corner1), z_ld(shape.corner2));
    const long double front = max(z_ld(shape.corner1), z_ld(shape.corner2));
    const long double dx = max({left - x, 0.0L, x - right});
    const long double dy = max({bottom - y, 0.0L, y - top});
    const long double dz = max({back - z, 0.0L, z - front});
    return hypot(hypot(dx, dy), dz);
}

template <class A, class B, class C, class D>
bool intersects(const Rectangle<A, B>& first, const Rectangle<C, D>& second) {
    using namespace ac_detail::geometry;
    if constexpr (integral_points_v<A, B, C, D>) {
        return rectangles_intersect_exact(
            first.corner1, first.corner2, second.corner1, second.corner2);
    } else {
        const long double eps = tolerance(1
            + fabsl(x_ld(first.corner1)) + fabsl(x_ld(first.corner2))
            + fabsl(y_ld(first.corner1)) + fabsl(y_ld(first.corner2))
            + fabsl(x_ld(second.corner1)) + fabsl(x_ld(second.corner2))
            + fabsl(y_ld(second.corner1)) + fabsl(y_ld(second.corner2)));
        return max(min(x_ld(first.corner1), x_ld(first.corner2)),
                   min(x_ld(second.corner1), x_ld(second.corner2)))
                <= min(max(x_ld(first.corner1), x_ld(first.corner2)),
                       max(x_ld(second.corner1), x_ld(second.corner2))) + eps
            && max(min(y_ld(first.corner1), y_ld(first.corner2)),
                   min(y_ld(second.corner1), y_ld(second.corner2)))
                <= min(max(y_ld(first.corner1), y_ld(first.corner2)),
                       max(y_ld(second.corner1), y_ld(second.corner2))) + eps;
    }
}

template <class P, class S, class A, class B>
bool intersects(const Square<P, S>& square, const Rectangle<A, B>& rectangle) {
    return intersects(to_rectangle(square), rectangle);
}

template <class A, class B, class P, class S>
bool intersects(const Rectangle<A, B>& rectangle, const Square<P, S>& square) {
    return intersects(square, rectangle);
}

template <class P1, class S1, class P2, class S2>
bool intersects(const Square<P1, S1>& first, const Square<P2, S2>& second) {
    return intersects(to_rectangle(first), to_rectangle(second));
}

template <class P, class A, class B>
bool intersects(const Polygon<P>& polygon, const Segment<A, B>& segment) {
    if (contains_point(polygon, segment.a) || contains_point(polygon, segment.b))
        return true;
    return !intersection_points(polygon, segment).empty();
}

template <class A, class B, class P>
bool intersects(const Segment<A, B>& segment, const Polygon<P>& polygon) {
    return intersects(polygon, segment);
}

template <class P, class A, class B>
bool intersects(const Polygon<P>& polygon, const Line<A, B>& line) {
    return !intersection_points(polygon, line).empty();
}

template <class A, class B, class P>
bool intersects(const Line<A, B>& line, const Polygon<P>& polygon) {
    return intersects(polygon, line);
}

template <class P, class C, class R>
bool intersects(const Circle<C, R>& circle, const Polygon<P>& polygon) {
    const long double radius = static_cast<long double>(circle.radius);
    return point_shape_distance(circle.center, polygon)
        <= radius + ac_detail::geometry::tolerance(1 + radius);
}

template <class P, class C, class R>
bool intersects(const Polygon<P>& polygon, const Circle<C, R>& circle) {
    return intersects(circle, polygon);
}

template <class P1, class P2>
bool intersects(const Polygon<P1>& first, const Polygon<P2>& second) {
    for (const auto& vertex : first.vertices)
        if (contains_point(second, vertex)) return true;
    for (const auto& vertex : second.vertices)
        if (contains_point(first, vertex)) return true;
    const size_t n = first.vertices.size(), m = second.vertices.size();
    for (size_t i = 0; i < n; ++i) {
        const auto edge1 = make_segment(first.vertices[i],
                                        first.vertices[(i + 1) % n]);
        for (size_t j = 0; j < m; ++j) {
            const auto edge2 = make_segment(second.vertices[j],
                                            second.vertices[(j + 1) % m]);
            if (intersects(edge1, edge2)) return true;
        }
    }
    return false;
}

template <class A, class B, class C, class D>
bool intersects(const Rectangle<A, B>& rectangle, const Segment<C, D>& segment) {
    return intersects(to_polygon(rectangle), segment);
}

template <class C, class D, class A, class B>
bool intersects(const Segment<C, D>& segment, const Rectangle<A, B>& rectangle) {
    return intersects(rectangle, segment);
}

template <class A, class B, class C, class D>
bool intersects(const Rectangle<A, B>& rectangle, const Line<C, D>& line) {
    return intersects(to_polygon(rectangle), line);
}

template <class C, class D, class A, class B>
bool intersects(const Line<C, D>& line, const Rectangle<A, B>& rectangle) {
    return intersects(rectangle, line);
}

template <class C, class R, class A, class B>
bool intersects(const Circle<C, R>& circle, const Rectangle<A, B>& rectangle) {
    const long double radius = static_cast<long double>(circle.radius);
    return point_shape_distance(circle.center, rectangle)
        <= radius + ac_detail::geometry::tolerance(1 + radius);
}

template <class A, class B, class C, class R>
bool intersects(const Rectangle<A, B>& rectangle, const Circle<C, R>& circle) {
    return intersects(circle, rectangle);
}

template <class P, class S, class A, class B>
bool intersects(const Square<P, S>& square, const Segment<A, B>& segment) {
    return intersects(to_polygon(square), segment);
}

template <class A, class B, class P, class S>
bool intersects(const Segment<A, B>& segment, const Square<P, S>& square) {
    return intersects(square, segment);
}

template <class P, class S, class A, class B>
bool intersects(const Square<P, S>& square, const Line<A, B>& line) {
    return intersects(to_polygon(square), line);
}

template <class A, class B, class P, class S>
bool intersects(const Line<A, B>& line, const Square<P, S>& square) {
    return intersects(square, line);
}

template <class C, class R, class P, class S>
bool intersects(const Circle<C, R>& circle, const Square<P, S>& square) {
    return intersects(circle, to_rectangle(square));
}

template <class P, class S, class C, class R>
bool intersects(const Square<P, S>& square, const Circle<C, R>& circle) {
    return intersects(circle, square);
}

template <class A, class B, class C, class D, class E>
bool intersects(const Triangle<A, B, C>& triangle, const Segment<D, E>& segment) {
    return intersects(to_polygon(triangle), segment);
}

template <class D, class E, class A, class B, class C>
bool intersects(const Segment<D, E>& segment, const Triangle<A, B, C>& triangle) {
    return intersects(triangle, segment);
}

template <class A, class B, class C, class D, class E>
bool intersects(const Triangle<A, B, C>& triangle, const Line<D, E>& line) {
    return intersects(to_polygon(triangle), line);
}

template <class D, class E, class A, class B, class C>
bool intersects(const Line<D, E>& line, const Triangle<A, B, C>& triangle) {
    return intersects(triangle, line);
}

template <class Cent, class R, class A, class B, class C>
bool intersects(const Circle<Cent, R>& circle, const Triangle<A, B, C>& triangle) {
    return intersects(circle, to_polygon(triangle));
}

template <class A, class B, class C, class Cent, class R>
bool intersects(const Triangle<A, B, C>& triangle, const Circle<Cent, R>& circle) {
    return intersects(circle, triangle);
}

template <class A, class B, class C, class P>
bool intersects(const Triangle<A, B, C>& triangle, const Polygon<P>& polygon) {
    return intersects(to_polygon(triangle), polygon);
}

template <class P, class A, class B, class C>
bool intersects(const Polygon<P>& polygon, const Triangle<A, B, C>& triangle) {
    return intersects(triangle, polygon);
}

template <class A1, class B1, class C1, class A2, class B2, class C2>
bool intersects(const Triangle<A1, B1, C1>& first,
                const Triangle<A2, B2, C2>& second) {
    return intersects(to_polygon(first), to_polygon(second));
}

template <class A, class B, class C, class D, class E>
bool intersects(const Triangle<A, B, C>& triangle, const Rectangle<D, E>& rectangle) {
    return intersects(to_polygon(triangle), to_polygon(rectangle));
}

template <class D, class E, class A, class B, class C>
bool intersects(const Rectangle<D, E>& rectangle, const Triangle<A, B, C>& triangle) {
    return intersects(triangle, rectangle);
}

template <class A, class B, class P>
bool intersects(const Rectangle<A, B>& rectangle, const Polygon<P>& polygon) {
    return intersects(to_polygon(rectangle), polygon);
}

template <class P, class A, class B>
bool intersects(const Polygon<P>& polygon, const Rectangle<A, B>& rectangle) {
    return intersects(rectangle, polygon);
}

template <class P, class S, class Q>
bool intersects(const Square<P, S>& square, const Polygon<Q>& polygon) {
    return intersects(to_polygon(square), polygon);
}

template <class Q, class P, class S>
bool intersects(const Polygon<Q>& polygon, const Square<P, S>& square) {
    return intersects(square, polygon);
}

// @acf fn shape_distance — (shape1,shape2) -> long double — 2図形の最短距離
/// @acf-doc @brief 2つの2D図形の最短距離。共有点があれば0。面を持つ図形は塗りつぶしとして扱う
/// @acf-doc [function] shape_distance
template <class A, class B, class C, class D>
long double shape_distance(const Segment<A, B>& first, const Segment<C, D>& second) {
    if (intersects(first, second)) return 0;
    return min({
        point_segment_distance(first.a, second.a, second.b),
        point_segment_distance(first.b, second.a, second.b),
        point_segment_distance(second.a, first.a, first.b),
        point_segment_distance(second.b, first.a, first.b),
    });
}

template <class A, class B, class C, class D>
long double shape_distance(const Line<A, B>& line, const Segment<C, D>& segment) {
    if (intersects(line, segment)) return 0;
    return min(point_line_distance(segment.a, line.a, line.b),
               point_line_distance(segment.b, line.a, line.b));
}

template <class A, class B, class C, class D>
long double shape_distance(const Segment<A, B>& segment, const Line<C, D>& line) {
    return shape_distance(line, segment);
}

template <class A, class B, class C, class D>
long double shape_distance(const Line<A, B>& first, const Line<C, D>& second) {
    if (intersects(first, second)) return 0;
    return point_line_distance(first.a, second.a, second.b);
}

template <class C1, class R1, class C2, class R2>
long double shape_distance(const Circle<C1, R1>& first, const Circle<C2, R2>& second) {
    const long double d = ac_detail::geometry::distance2d(first.center, second.center);
    const long double r1 = static_cast<long double>(first.radius);
    const long double r2 = static_cast<long double>(second.radius);
    return max(0.0L, d - r1 - r2);
}

template <class C, class R, class A, class B>
long double shape_distance(const Circle<C, R>& circle, const Line<A, B>& line) {
    return max(0.0L, point_line_distance(circle.center, line.a, line.b)
                     - static_cast<long double>(circle.radius));
}

template <class A, class B, class C, class R>
long double shape_distance(const Line<A, B>& line, const Circle<C, R>& circle) {
    return shape_distance(circle, line);
}

template <class C, class R, class A, class B>
long double shape_distance(const Circle<C, R>& circle, const Segment<A, B>& segment) {
    return max(0.0L, point_segment_distance(circle.center, segment.a, segment.b)
                     - static_cast<long double>(circle.radius));
}

template <class A, class B, class C, class R>
long double shape_distance(const Segment<A, B>& segment, const Circle<C, R>& circle) {
    return shape_distance(circle, segment);
}

template <class C, class R, class P>
long double shape_distance(const Circle<C, R>& circle, const Polygon<P>& polygon) {
    return max(0.0L, point_shape_distance(circle.center, polygon)
                     - static_cast<long double>(circle.radius));
}

template <class P, class C, class R>
long double shape_distance(const Polygon<P>& polygon, const Circle<C, R>& circle) {
    return shape_distance(circle, polygon);
}

template <class C, class R, class A, class B>
long double shape_distance(const Circle<C, R>& circle, const Rectangle<A, B>& rectangle) {
    return max(0.0L, point_shape_distance(circle.center, rectangle)
                     - static_cast<long double>(circle.radius));
}

template <class A, class B, class C, class R>
long double shape_distance(const Rectangle<A, B>& rectangle, const Circle<C, R>& circle) {
    return shape_distance(circle, rectangle);
}

template <class C, class R, class A, class B, class D>
long double shape_distance(const Circle<C, R>& circle, const Triangle<A, B, D>& triangle) {
    return max(0.0L, point_shape_distance(circle.center, triangle)
                     - static_cast<long double>(circle.radius));
}

template <class A, class B, class D, class C, class R>
long double shape_distance(const Triangle<A, B, D>& triangle, const Circle<C, R>& circle) {
    return shape_distance(circle, triangle);
}

template <class C, class R, class P, class S>
long double shape_distance(const Circle<C, R>& circle, const Square<P, S>& square) {
    return shape_distance(circle, to_rectangle(square));
}

template <class P, class S, class C, class R>
long double shape_distance(const Square<P, S>& square, const Circle<C, R>& circle) {
    return shape_distance(circle, square);
}

template <class A, class B, class C, class D>
long double shape_distance(const Rectangle<A, B>& first, const Rectangle<C, D>& second) {
    if (intersects(first, second)) return 0;
    using namespace ac_detail::geometry;
    const long double left1 = min(x_ld(first.corner1), x_ld(first.corner2));
    const long double right1 = max(x_ld(first.corner1), x_ld(first.corner2));
    const long double bottom1 = min(y_ld(first.corner1), y_ld(first.corner2));
    const long double top1 = max(y_ld(first.corner1), y_ld(first.corner2));
    const long double left2 = min(x_ld(second.corner1), x_ld(second.corner2));
    const long double right2 = max(x_ld(second.corner1), x_ld(second.corner2));
    const long double bottom2 = min(y_ld(second.corner1), y_ld(second.corner2));
    const long double top2 = max(y_ld(second.corner1), y_ld(second.corner2));
    const long double dx = max({left1 - right2, left2 - right1, 0.0L});
    const long double dy = max({bottom1 - top2, bottom2 - top1, 0.0L});
    return hypot(dx, dy);
}

template <class P1, class S1, class P2, class S2>
long double shape_distance(const Square<P1, S1>& first, const Square<P2, S2>& second) {
    return shape_distance(to_rectangle(first), to_rectangle(second));
}

template <class P, class S, class A, class B>
long double shape_distance(const Square<P, S>& square, const Rectangle<A, B>& rectangle) {
    return shape_distance(to_rectangle(square), rectangle);
}

template <class A, class B, class P, class S>
long double shape_distance(const Rectangle<A, B>& rectangle, const Square<P, S>& square) {
    return shape_distance(square, rectangle);
}

template <class P, class S, class Q>
long double shape_distance(const Square<P, S>& square, const Polygon<Q>& polygon) {
    return shape_distance(to_polygon(square), polygon);
}

template <class Q, class P, class S>
long double shape_distance(const Polygon<Q>& polygon, const Square<P, S>& square) {
    return shape_distance(square, polygon);
}

template <class P, class S, class A, class B>
long double shape_distance(const Square<P, S>& square, const Segment<A, B>& segment) {
    return shape_distance(segment, to_polygon(square));
}

template <class A, class B, class P, class S>
long double shape_distance(const Segment<A, B>& segment, const Square<P, S>& square) {
    return shape_distance(square, segment);
}

template <class P1, class P2>
long double shape_distance(const Polygon<P1>& first, const Polygon<P2>& second) {
    if (intersects(first, second)) return 0;
    const size_t n = first.vertices.size(), m = second.vertices.size();
    if (n == 0 || m == 0) return numeric_limits<long double>::infinity();
    long double result = numeric_limits<long double>::infinity();
    for (size_t i = 0; i < n; ++i) {
        const auto edge1 = make_segment(first.vertices[i],
                                        first.vertices[(i + 1) % n]);
        for (size_t j = 0; j < m; ++j) {
            const auto edge2 = make_segment(second.vertices[j],
                                            second.vertices[(j + 1) % m]);
            result = min(result, shape_distance(edge1, edge2));
        }
    }
    return result;
}

template <class A, class B, class P>
long double shape_distance(const Segment<A, B>& segment, const Polygon<P>& polygon) {
    if (intersects(segment, polygon)) return 0;
    if (polygon.vertices.empty()) return numeric_limits<long double>::infinity();
    long double result = numeric_limits<long double>::infinity();
    for (size_t i = 0; i < polygon.vertices.size(); ++i)
        result = min(result, shape_distance(
            segment,
            make_segment(polygon.vertices[i],
                         polygon.vertices[(i + 1) % polygon.vertices.size()])));
    return result;
}

template <class P, class A, class B>
long double shape_distance(const Polygon<P>& polygon, const Segment<A, B>& segment) {
    return shape_distance(segment, polygon);
}

template <class A, class B, class C, class D>
long double shape_distance(const Segment<A, B>& segment, const Rectangle<C, D>& rectangle) {
    return shape_distance(segment, to_polygon(rectangle));
}

template <class C, class D, class A, class B>
long double shape_distance(const Rectangle<C, D>& rectangle, const Segment<A, B>& segment) {
    return shape_distance(segment, rectangle);
}

template <class A, class B, class C, class D, class E>
long double shape_distance(const Segment<A, B>& segment, const Triangle<C, D, E>& triangle) {
    return shape_distance(segment, to_polygon(triangle));
}

template <class C, class D, class E, class A, class B>
long double shape_distance(const Triangle<C, D, E>& triangle, const Segment<A, B>& segment) {
    return shape_distance(segment, triangle);
}

template <class A, class B, class P>
long double shape_distance(const Rectangle<A, B>& rectangle, const Polygon<P>& polygon) {
    return shape_distance(to_polygon(rectangle), polygon);
}

template <class P, class A, class B>
long double shape_distance(const Polygon<P>& polygon, const Rectangle<A, B>& rectangle) {
    return shape_distance(rectangle, polygon);
}

template <class A, class B, class C, class D, class E>
long double shape_distance(const Triangle<A, B, C>& triangle, const Rectangle<D, E>& rectangle) {
    return shape_distance(to_polygon(triangle), to_polygon(rectangle));
}

template <class D, class E, class A, class B, class C>
long double shape_distance(const Rectangle<D, E>& rectangle, const Triangle<A, B, C>& triangle) {
    return shape_distance(triangle, rectangle);
}

template <class A1, class B1, class C1, class A2, class B2, class C2>
long double shape_distance(const Triangle<A1, B1, C1>& first,
                           const Triangle<A2, B2, C2>& second) {
    return shape_distance(to_polygon(first), to_polygon(second));
}

template <class A, class B, class C, class P>
long double shape_distance(const Triangle<A, B, C>& triangle, const Polygon<P>& polygon) {
    return shape_distance(to_polygon(triangle), polygon);
}

template <class P, class A, class B, class C>
long double shape_distance(const Polygon<P>& polygon, const Triangle<A, B, C>& triangle) {
    return shape_distance(triangle, polygon);
}

template <class C1, class R1, class C2, class R2>
bool intersects(const Sphere<C1, R1>& first,
                const Sphere<C2, R2>& second) {
    const long double distance = euclidean_distance(
        ac_detail::geometry::point3_ld(first.center),
        ac_detail::geometry::point3_ld(second.center));
    const long double r1 = static_cast<long double>(first.radius);
    const long double r2 = static_cast<long double>(second.radius);
    const long double eps = ac_detail::geometry::tolerance(1 + distance + r1 + r2);
    return distance <= r1 + r2 + eps;
}

template <class A, class B, class C, class D>
bool intersects(const Cuboid<A, B>& first, const Cuboid<C, D>& second) {
    using namespace ac_detail::geometry;
    const long double eps = tolerance(1
        + fabsl(x_ld(first.corner1)) + fabsl(x_ld(first.corner2))
        + fabsl(y_ld(first.corner1)) + fabsl(y_ld(first.corner2))
        + fabsl(z_ld(first.corner1)) + fabsl(z_ld(first.corner2))
        + fabsl(x_ld(second.corner1)) + fabsl(x_ld(second.corner2))
        + fabsl(y_ld(second.corner1)) + fabsl(y_ld(second.corner2))
        + fabsl(z_ld(second.corner1)) + fabsl(z_ld(second.corner2)));
    return max(min(x_ld(first.corner1), x_ld(first.corner2)),
               min(x_ld(second.corner1), x_ld(second.corner2)))
            <= min(max(x_ld(first.corner1), x_ld(first.corner2)),
                   max(x_ld(second.corner1), x_ld(second.corner2))) + eps
        && max(min(y_ld(first.corner1), y_ld(first.corner2)),
               min(y_ld(second.corner1), y_ld(second.corner2)))
            <= min(max(y_ld(first.corner1), y_ld(first.corner2)),
                   max(y_ld(second.corner1), y_ld(second.corner2))) + eps
        && max(min(z_ld(first.corner1), z_ld(first.corner2)),
               min(z_ld(second.corner1), z_ld(second.corner2)))
            <= min(max(z_ld(first.corner1), z_ld(first.corner2)),
                   max(z_ld(second.corner1), z_ld(second.corner2))) + eps;
}

template <class C, class R, class A, class B>
bool intersects(const Sphere<C, R>& sphere, const Cuboid<A, B>& cuboid) {
    return point_shape_distance(sphere.center, cuboid)
        <= static_cast<long double>(sphere.radius)
         + ac_detail::geometry::tolerance(static_cast<long double>(sphere.radius));
}

template <class A, class B, class C, class R>
bool intersects(const Cuboid<A, B>& cuboid, const Sphere<C, R>& sphere) {
    return intersects(sphere, cuboid);
}

#if defined(LOCAL) && defined(AC_VISUAL_HPP)
namespace debug::graphical {
template <class T, size_t N>
pair<long double, long double> project_point(const Point<T, N>& p) {
    using namespace ac_detail::geometry;
    if constexpr (N >= 3) {
        return project3(x_ld(p), y_ld(p), z_ld(p));
    } else {
        return project2(x_ld(p), y_ld(p));
    }
}
inline void expand_xyz(Canvas& canvas, long double x, long double y, long double z) {
    auto [sx, sy] = project3(x, y, z);
    canvas.expand(sx, sy);
}
template <class T, size_t N>
void expand_object(Canvas& canvas, const Point<T, N>& point) {
    // Do not call expand_point_like: it is defined in ac.hpp before Point's
    // project_point overload exists, so two-phase lookup would miss it.
    auto [sx, sy] = project_point(point);
    canvas.expand(sx, sy);
}
template <class A, class B>
void expand_object(Canvas& canvas, const Line<A, B>& line) {
    expand_object(canvas, line.a);
    expand_object(canvas, line.b);
}
template <class A, class B>
void expand_object(Canvas& canvas, const Segment<A, B>& segment) {
    expand_object(canvas, segment.a);
    expand_object(canvas, segment.b);
}
template <class P, class Radius>
void expand_object(Canvas& canvas, const Circle<P, Radius>& circle) {
    using namespace ac_detail::geometry;
    const long double cx = x_ld(circle.center), cy = y_ld(circle.center);
    const long double r = static_cast<long double>(circle.radius);
    expand_xy(canvas, cx - r, cy - r);
    expand_xy(canvas, cx + r, cy + r);
}
template <class A, class B>
void expand_object(Canvas& canvas, const Rectangle<A, B>& rectangle) {
    using namespace ac_detail::geometry;
    expand_xy(canvas, min(x_ld(rectangle.corner1), x_ld(rectangle.corner2)),
              min(y_ld(rectangle.corner1), y_ld(rectangle.corner2)));
    expand_xy(canvas, max(x_ld(rectangle.corner1), x_ld(rectangle.corner2)),
              max(y_ld(rectangle.corner1), y_ld(rectangle.corner2)));
}
template <class P, class Side>
void expand_object(Canvas& canvas, const Square<P, Side>& square) {
    using namespace ac_detail::geometry;
    const long double s = static_cast<long double>(square.side);
    expand_xy(canvas, x_ld(square.corner), y_ld(square.corner));
    expand_xy(canvas, x_ld(square.corner) + s, y_ld(square.corner) + s);
}
template <class A, class B, class C>
void expand_object(Canvas& canvas, const Triangle<A, B, C>& triangle) {
    expand_object(canvas, triangle.a);
    expand_object(canvas, triangle.b);
    expand_object(canvas, triangle.c);
}
template <class P>
void expand_object(Canvas& canvas, const Polygon<P>& polygon) {
    for (const auto& v : polygon.vertices) expand_object(canvas, v);
}
template <class P, class Radius>
void expand_object(Canvas& canvas, const Sphere<P, Radius>& sphere) {
    using namespace ac_detail::geometry;
    const long double r = static_cast<long double>(sphere.radius);
    const long double cx = x_ld(sphere.center), cy = y_ld(sphere.center),
                      cz = z_ld(sphere.center);
    for (int i = 0; i < 24; ++i) {
        const long double t = 2 * PI * i / 24;
        expand_xyz(canvas, cx + r * cosl(t), cy + r * sinl(t), cz);
        expand_xyz(canvas, cx + r * cosl(t), cy, cz + r * sinl(t));
        expand_xyz(canvas, cx, cy + r * cosl(t), cz + r * sinl(t));
    }
}
template <class A, class B>
void expand_object(Canvas& canvas, const Cuboid<A, B>& cuboid) {
    using namespace ac_detail::geometry;
    const long double x0 = min(x_ld(cuboid.corner1), x_ld(cuboid.corner2));
    const long double x1 = max(x_ld(cuboid.corner1), x_ld(cuboid.corner2));
    const long double y0 = min(y_ld(cuboid.corner1), y_ld(cuboid.corner2));
    const long double y1 = max(y_ld(cuboid.corner1), y_ld(cuboid.corner2));
    const long double z0 = min(z_ld(cuboid.corner1), z_ld(cuboid.corner2));
    const long double z1 = max(z_ld(cuboid.corner1), z_ld(cuboid.corner2));
    for (long double x : {x0, x1})
        for (long double y : {y0, y1})
            for (long double z : {z0, z1}) expand_xyz(canvas, x, y, z);
}
template <class T, size_t N>
void draw_object(Canvas& canvas, const Point<T, N>& point, int color_idx) {
    auto [sx, sy] = project_point(point);
    canvas.plot_point(sx, sy, color_idx);
}
template <class A, class B>
void draw_object(Canvas& canvas, const Segment<A, B>& segment, int color_idx) {
    auto [x0, y0] = ::debug::graphical::project_point(segment.a);
    auto [x1, y1] = ::debug::graphical::project_point(segment.b);
    canvas.draw_segment(x0, y0, x1, y1, color_idx);
}
template <class A, class B>
void draw_object(Canvas& canvas, const Line<A, B>& line, int color_idx) {
    auto [x0, y0] = ::debug::graphical::project_point(line.a);
    auto [x1, y1] = ::debug::graphical::project_point(line.b);
    long double dx = x1 - x0, dy = y1 - y0;
    const long double len = sqrtl(dx * dx + dy * dy);
    if (len < 1e-18L) {
        draw_object(canvas, line.a, color_idx);
        return;
    }
    dx /= len;
    dy /= len;
    const long double span =
        hypotl(canvas.max_x - canvas.min_x, canvas.max_y - canvas.min_y) * 2;
    canvas.draw_segment(x0 - dx * span, y0 - dy * span, x0 + dx * span,
                        y0 + dy * span, color_idx);
}
template <class P, class Radius>
void draw_object(Canvas& canvas, const Circle<P, Radius>& circle, int color_idx) {
    using namespace ac_detail::geometry;
    const long double cx = x_ld(circle.center), cy = y_ld(circle.center);
    const long double r = static_cast<long double>(circle.radius);
    constexpr int samples = 160;
    long double px = cx + r, py = cy;
    for (int i = 1; i <= samples; ++i) {
        const long double t = 2 * PI * i / samples;
        const long double nx = cx + r * cosl(t);
        const long double ny = cy + r * sinl(t);
        canvas.draw_segment(px, py, nx, ny, color_idx);
        px = nx;
        py = ny;
    }
}
template <class A, class B>
void draw_object(Canvas& canvas, const Rectangle<A, B>& rectangle, int color_idx) {
    using namespace ac_detail::geometry;
    const long double x0 = min(x_ld(rectangle.corner1), x_ld(rectangle.corner2));
    const long double x1 = max(x_ld(rectangle.corner1), x_ld(rectangle.corner2));
    const long double y0 = min(y_ld(rectangle.corner1), y_ld(rectangle.corner2));
    const long double y1 = max(y_ld(rectangle.corner1), y_ld(rectangle.corner2));
    canvas.draw_segment(x0, y0, x1, y0, color_idx);
    canvas.draw_segment(x1, y0, x1, y1, color_idx);
    canvas.draw_segment(x1, y1, x0, y1, color_idx);
    canvas.draw_segment(x0, y1, x0, y0, color_idx);
}
template <class P, class Side>
void draw_object(Canvas& canvas, const Square<P, Side>& square, int color_idx) {
    using namespace ac_detail::geometry;
    const long double x0 = x_ld(square.corner), y0 = y_ld(square.corner);
    const long double s = static_cast<long double>(square.side);
    draw_object(canvas, make_rectangle(pair{x0, y0}, pair{x0 + s, y0 + s}), color_idx);
}
template <class A, class B, class C>
void draw_object(Canvas& canvas, const Triangle<A, B, C>& triangle, int color_idx) {
    auto [ax, ay] = ::debug::graphical::project_point(triangle.a);
    auto [bx, by] = ::debug::graphical::project_point(triangle.b);
    auto [cx, cy] = ::debug::graphical::project_point(triangle.c);
    canvas.draw_segment(ax, ay, bx, by, color_idx);
    canvas.draw_segment(bx, by, cx, cy, color_idx);
    canvas.draw_segment(cx, cy, ax, ay, color_idx);
}
template <class P>
void draw_object(Canvas& canvas, const Polygon<P>& polygon, int color_idx) {
    if (polygon.vertices.empty()) return;
    for (size_t i = 0; i < polygon.vertices.size(); ++i) {
        const auto& a = polygon.vertices[i];
        const auto& b = polygon.vertices[(i + 1) % polygon.vertices.size()];
        auto [x0, y0] = ::debug::graphical::project_point(a);
        auto [x1, y1] = ::debug::graphical::project_point(b);
        canvas.draw_segment(x0, y0, x1, y1, color_idx);
    }
}
template <class P, class Radius>
void draw_object(Canvas& canvas, const Sphere<P, Radius>& sphere, int color_idx) {
    using namespace ac_detail::geometry;
    const long double r = static_cast<long double>(sphere.radius);
    const long double cx = x_ld(sphere.center), cy = y_ld(sphere.center),
                      cz = z_ld(sphere.center);
    auto ring = [&](auto sample) {
        constexpr int n = 64;
        auto [px, py] = sample(0);
        for (int i = 1; i <= n; ++i) {
            auto [nx, ny] = sample(i);
            canvas.draw_segment(px, py, nx, ny, color_idx);
            px = nx;
            py = ny;
        }
    };
    ring([&](int i) {
        const long double t = 2 * PI * i / 64;
        return project3(cx + r * cosl(t), cy + r * sinl(t), cz);
    });
    ring([&](int i) {
        const long double t = 2 * PI * i / 64;
        return project3(cx + r * cosl(t), cy, cz + r * sinl(t));
    });
    ring([&](int i) {
        const long double t = 2 * PI * i / 64;
        return project3(cx, cy + r * cosl(t), cz + r * sinl(t));
    });
}
template <class A, class B>
void draw_object(Canvas& canvas, const Cuboid<A, B>& cuboid, int color_idx) {
    using namespace ac_detail::geometry;
    const long double x0 = min(x_ld(cuboid.corner1), x_ld(cuboid.corner2));
    const long double x1 = max(x_ld(cuboid.corner1), x_ld(cuboid.corner2));
    const long double y0 = min(y_ld(cuboid.corner1), y_ld(cuboid.corner2));
    const long double y1 = max(y_ld(cuboid.corner1), y_ld(cuboid.corner2));
    const long double z0 = min(z_ld(cuboid.corner1), z_ld(cuboid.corner2));
    const long double z1 = max(z_ld(cuboid.corner1), z_ld(cuboid.corner2));
    array<array<long double, 3>, 8> corners{};
    int idx = 0;
    for (long double x : {x0, x1})
        for (long double y : {y0, y1})
            for (long double z : {z0, z1}) corners[idx++] = {x, y, z};
    auto edge = [&](int i, int j) {
        auto [ax, ay] = project3(corners[i][0], corners[i][1], corners[i][2]);
        auto [bx, by] = project3(corners[j][0], corners[j][1], corners[j][2]);
        canvas.draw_segment(ax, ay, bx, by, color_idx);
    };
    edge(0, 1);
    edge(0, 2);
    edge(0, 4);
    edge(1, 3);
    edge(1, 5);
    edge(2, 3);
    edge(2, 6);
    edge(3, 7);
    edge(4, 5);
    edge(4, 6);
    edge(5, 7);
    edge(6, 7);
}
}  // namespace debug::graphical
#endif
