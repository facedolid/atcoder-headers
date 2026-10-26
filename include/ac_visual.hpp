#pragma once

// LOCAL 専用のターミナル可視化。通常提出時は no-op になる。
#define AC_VISUAL_HPP 1
#include "ac.hpp"

#ifdef LOCAL
namespace debug::graphical {
inline constexpr const char* kGlyphs[] = {"●", "◆", "■", "▲", "○", "★", "◇", "□"};
inline constexpr const char* kColors[] = {
    "\033[38;5;203m",  // rose
    "\033[38;5;81m",   // cyan
    "\033[38;5;220m",  // gold
    "\033[38;5;141m",  // violet
    "\033[38;5;118m",  // lime
    "\033[38;5;208m",  // orange
    "\033[38;5;51m",   // aqua
    "\033[38;5;213m",  // pink
};
inline constexpr const char* kReset = "\033[0m";
inline constexpr const char* kDim = "\033[2m";
inline constexpr const char* kAxisColor = "\033[38;5;245m";
inline constexpr int kDefaultWidth = 56;
inline constexpr int kDefaultHeight = 20;
inline constexpr long double kCharAspect = 0.5L;
// terminal cell width/height

inline constexpr long double kIsoCos = 0.8660254037844386L;
inline constexpr long double kIsoSin = 0.5L;
inline pair<long double, long double> project2(long double x, long double y) {
    return {x, y};
}
inline pair<long double, long double> project3(long double x, long double y,
                                              long double z) {
    return {(x - z) * kIsoCos, y + (x + z) * kIsoSin};
}

template <class A, class B>
pair<long double, long double> project_point(const pair<A, B>& p) {
    return project2(static_cast<long double>(p.first),
                    static_cast<long double>(p.second));
}
template <class... T>
pair<long double, long double> project_point(const tuple<T...>& p) {
    if constexpr (sizeof...(T) >= 3) {
        return project3(static_cast<long double>(get<0>(p)),
                        static_cast<long double>(get<1>(p)),
                        static_cast<long double>(get<2>(p)));
    } else {
        static_assert(sizeof...(T) >= 2);
        return project2(static_cast<long double>(get<0>(p)),
                        static_cast<long double>(get<1>(p)));
    }
}
template <class T, size_t N>
pair<long double, long double> project_point(const array<T, N>& p) {
    if constexpr (N >= 3) {
        return project3(static_cast<long double>(p[0]), static_cast<long double>(p[1]),
                        static_cast<long double>(p[2]));
    } else {
        static_assert(N >= 2);
        return project2(static_cast<long double>(p[0]), static_cast<long double>(p[1]));
    }
}
template <class T>
pair<long double, long double> project_point(const vector<T>& p) {
    if (p.size() >= 3)
        return project3(static_cast<long double>(p[0]), static_cast<long double>(p[1]),
                        static_cast<long double>(p[2]));
    assert(p.size() >= 2);
    return project2(static_cast<long double>(p[0]), static_cast<long double>(p[1]));
}

inline string paint(const char* color, const char* glyph) {
    return string(color) + glyph + kReset;
}
inline string paint_axis(string_view glyph) {
    return string(kAxisColor) + string(glyph) + kReset;
}
inline string format_tick(long double v) {
    ostringstream oss;
    oss << defaultfloat;
    if (fabsl(v) < 1e-12L) v = 0;
    if (fabsl(v) >= 1000 || (fabsl(v) > 0 && fabsl(v) < 0.01L)) {
        oss << scientific << setprecision(2) << static_cast<double>(v);
    } else {
        oss << fixed << setprecision(fabsl(v) >= 10 ? 1 : 2) << static_cast<double>(v);
        string s = oss.str();
        while (s.size() > 1 && s.back() == '0') s.pop_back();
        if (!s.empty() && s.back() == '.') s.pop_back();
        return s;
    }
    return oss.str();
}
// Braille dots for graphical_print: each cell ORs 2x4 dots (Unicode U+2800..).
// Not related to stack<char> / stc / STC.
inline string braille_utf8(uint8_t dots) {
    const char32_t cp = 0x2800u + dots;
    string s;
    s.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
    s.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
    s.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    return s;
}
inline constexpr uint8_t kBrailleBit[4][2] = {
    {0x01, 0x08},
    {0x02, 0x10},
    {0x04, 0x20},
    {0x40, 0x80},
};
// @acf fn braille_bit — (row,col) -> uint8_t — Braille セル内 (row0..3,col0..1) のビット
/// @acf-doc @brief `graphical_print` 用。1セルは 2×4 点。`braille_or` / `braille_char` と組み合わせ可
/// @acf-doc @code (int row, int col) -> uint8_t
/// @acf-doc [function] braille_bit
inline constexpr uint8_t braille_bit(int row, int col) {
    return kBrailleBit[row][col];
}
// @acf fn braille_or — (dots,row,col) -> uint8_t — 既存マスクに1点を OR
/// @acf-doc @brief 既存のドットマスクへ (row,col) を点灯して返す
/// @acf-doc @code (uint8_t dots, int row, int col) -> uint8_t
/// @acf-doc [function] braille_or
inline constexpr uint8_t braille_or(uint8_t dots, int row, int col) {
    return static_cast<uint8_t>(dots | braille_bit(row, col));
}
// @acf fn braille_char — (dots) -> string — ドットマスクを Braille 1文字（UTF-8）にする
/// @acf-doc @brief U+2800+dots を UTF-8 文字列で返す
/// @acf-doc @code (uint8_t dots) -> string
/// @acf-doc [function] braille_char
inline string braille_char(uint8_t dots) { return braille_utf8(dots); }
struct Cell {
    uint8_t dots = 0;
    int color_idx = -1;
    bool axis = false;
    const char* axis_glyph = " ";
    string overlay;
};
struct Canvas {
    int width = kDefaultWidth;
    int height = kDefaultHeight;
    vector<vector<Cell>> grid;
    long double min_x = 0, max_x = 1, min_y = 0, max_y = 1;
    bool has_bounds = false;
    vector<string> legend;

    /// @brief 端末セル数 `w`×`h` の描画キャンバスを構築する（最小8×6）。
    explicit Canvas(int w, int h) : width(max(w, 8)), height(max(h, 6)) {
        grid.assign(height, vector<Cell>(width));
    }

    /// @brief Brailleの横方向ピクセル数を返す。
    int px_w() const { return width * 2; }

    /// @brief Brailleの縦方向ピクセル数を返す。
    int px_h() const { return height * 4; }

    /// @brief 描画範囲へワールド座標の点を追加する。
    void expand(long double x, long double y) {
        if (!has_bounds) {
            min_x = max_x = x;
            min_y = max_y = y;
            has_bounds = true;
            return;
        }
        min_x = min(min_x, x);
        max_x = max(max_x, x);
        min_y = min(min_y, y);
        max_y = max(max_y, y);
    }

    /// @brief ワールド座標を端末セル座標へ変換する。
    pair<int, int> to_cell(long double x, long double y) const {
        auto [sx, sy] = to_subpixel(x, y);
        return {sx / 2, sy / 4};
    }

    /// @brief ワールド座標をBrailleサブピクセル座標へ変換する。
    pair<int, int> to_subpixel(long double x, long double y) const {
        long double dx = max_x - min_x;
        long double dy = max_y - min_y;
        if (dx < 1e-18L) dx = 1;
        if (dy < 1e-18L) dy = 1;
        const int sx =
            static_cast<int>(llround((x - min_x) / dx * (px_w() - 1)));
        const int sy =
            static_cast<int>(llround((1 - (y - min_y) / dy) * (px_h() - 1)));
        return {clamp(sx, 0, px_w() - 1), clamp(sy, 0, px_h() - 1)};
    }

    /// @brief 指定したサブピクセルを点灯する。
    void plot_subpixel(int sx, int sy, int color_idx) {
        if (sx < 0 || sy < 0 || sx >= px_w() || sy >= px_h()) return;
        Cell& cell = grid[sy / 4][sx / 2];
        cell.dots = static_cast<uint8_t>(cell.dots | kBrailleBit[sy % 4][sx % 2]);
        cell.color_idx = color_idx;
        cell.axis = false;
    }

    /// @brief ワールド座標の点を描画する。
    void plot_point(long double x, long double y, int color_idx) {
        auto [sx, sy] = to_subpixel(x, y);
        for (int dy = 0; dy < 2; ++dy)
            for (int dx = 0; dx < 2; ++dx) plot_subpixel(sx + dx, sy + dy, color_idx);
    }

    /// @brief 2点間をサブピクセルの線分として描画する。
    void draw_segment(long double x0, long double y0, long double x1, long double y1,
                      int color_idx) {
        auto [c0, r0] = to_subpixel(x0, y0);
        auto [c1, r1] = to_subpixel(x1, y1);
        int dc = abs(c1 - c0), dr = abs(r1 - r0);
        int sc = c0 < c1 ? 1 : -1;
        int sr = r0 < r1 ? 1 : -1;
        int err = dc - dr;
        for (;;) {
            plot_subpixel(c0, r0, color_idx);
            if (c0 == c1 && r0 == r1) break;
            const int e2 = 2 * err;
            if (e2 > -dr) {
                err -= dr;
                c0 += sc;
            }
            if (e2 < dc) {
                err += dc;
                r0 += sr;
            }
        }
    }

    /// @brief 空のセルに軸記号を設定する。図形があれば上書きしない。
    void set_axis_cell(int col, int row, const char* glyph) {
        if (0 <= col && col < width && 0 <= row && row < height) {
            Cell& cell = grid[row][col];
            if (cell.dots == 0 && !cell.axis) {
                cell.axis = true;
                cell.axis_glyph = glyph;
            }
        }
    }

    /// @brief セルへ軸記号を設定する。点がなければ既存の軸記号を上書きする。
    void force_axis_cell(int col, int row, const char* glyph) {
        if (0 <= col && col < width && 0 <= row && row < height) {
            Cell& cell = grid[row][col];
            if (cell.dots == 0) {
                cell.axis = true;
                cell.axis_glyph = glyph;
            }
        }
    }

    /// @brief 指定セルから凡例などのオーバーレイ文字列を描画する。
    void draw_overlay_text(int col, int row, string_view text) {
        if (row < 0 || row >= height) return;
        for (char ch : text) {
            if (0 <= col && col < width) grid[row][col].overlay.assign(1, ch);
            ++col;
        }
    }

    /// @brief 現在の範囲に合わせて座標軸と目盛りを描画する。
    void draw_axes() {
        if (!has_bounds) return;
        optional<int> zero_col, zero_row;
        if (min_x <= 0 && 0 <= max_x) {
            zero_col = to_cell(0, (min_y + max_y) / 2).first;
            for (int row = 0; row < height; ++row) force_axis_cell(*zero_col, row, "│");
            force_axis_cell(*zero_col, 0, "↑");
        }
        if (min_y <= 0 && 0 <= max_y) {
            zero_row = to_cell((min_x + max_x) / 2, 0).second;
            for (int col = 0; col < width; ++col) {
                if (zero_col && col == *zero_col)
                    force_axis_cell(col, *zero_row, "┼");
                else
                    force_axis_cell(col, *zero_row, "─");
            }
            force_axis_cell(width - 1, *zero_row, "→");
        }
        for (int row = 0; row < height; row += 4)
            for (int col = 0; col < width; col += 8) set_axis_cell(col, row, "·");
    }

    /// @brief 描画範囲に余白を追加し、端末の縦横比に合わせる。
    void pad_bounds() {
        if (!has_bounds) {
            min_x = -1;
            max_x = 1;
            min_y = -1;
            max_y = 1;
            has_bounds = true;
        }
        long double dx = max_x - min_x;
        long double dy = max_y - min_y;
        if (dx < 1e-12L) dx = 1;
        if (dy < 1e-12L) dy = 1;
        min_x -= dx * 0.08L;
        max_x += dx * 0.08L;
        min_y -= dy * 0.08L;
        max_y += dy * 0.08L;
        dx = max_x - min_x;
        dy = max_y - min_y;
        const long double plot_aspect =
            (static_cast<long double>(width) * kCharAspect) /
            static_cast<long double>(height);
        const long double world_aspect = dx / dy;
        if (world_aspect < plot_aspect) {
            const long double target = dy * plot_aspect;
            const long double pad = (target - dx) / 2;
            min_x -= pad;
            max_x += pad;
        } else {
            const long double target = dx / plot_aspect;
            const long double pad = (target - dy) / 2;
            min_y -= pad;
            max_y += pad;
        }
    }

    /// @brief 1セルをBraille・軸記号・文字オーバーレイとして文字列化する。
    string render_cell(const Cell& cell) const {
        if (!cell.overlay.empty()) return cell.overlay;
        if (cell.dots != 0) {
            const char* color =
                cell.color_idx >= 0
                    ? kColors[static_cast<size_t>(cell.color_idx) %
                              (sizeof(kColors) / sizeof(kColors[0]))]
                    : kAxisColor;
            return string(color) + braille_utf8(cell.dots) + kReset;
        }
        if (cell.axis) return paint_axis(cell.axis_glyph);
        return " ";
    }

    /// @brief キャンバスを軸・凡例付きでストリームへ出力する。
    void write(ostream& os) const {
        const int label_w = 8;
        auto repeat_glyph = [](string_view glyph, int n) {
            string out;
            out.reserve(glyph.size() * static_cast<size_t>(max(n, 0)));
            for (int i = 0; i < n; ++i) out += glyph;
            return out;
        };
        auto y_label = [&](int row) -> string {
            if (min_y <= 0 && 0 <= max_y) {
                const int zero_row = to_cell((min_x + max_x) / 2, 0).second;
                if (row == zero_row) return "0";
            }
            if (!(row == 0 || row == height - 1)) return {};
            const long double t =
                height == 1 ? 0
                            : static_cast<long double>(height - 1 - row) / (height - 1);
            return format_tick(min_y + t * (max_y - min_y));
        };

        os << kDim << "  graphical_print  "
           << "x∈[" << format_tick(min_x) << ", " << format_tick(max_x) << "]  "
           << "y∈[" << format_tick(min_y) << ", " << format_tick(max_y) << "]"
           << kReset << '\n';

        os << string(label_w, ' ') << paint_axis("╭")
           << paint_axis(repeat_glyph("─", width)) << paint_axis("╮") << ' '
           << paint_axis("y↑") << '\n';

        for (int row = 0; row < height; ++row) {
            string yl = y_label(row);
            if (static_cast<int>(yl.size()) > label_w - 1) yl = yl.substr(0, label_w - 1);
            os << kDim << setw(label_w - 1) << yl << kReset << ' ' << paint_axis("│");
            for (int col = 0; col < width; ++col) os << render_cell(grid[row][col]);
            os << paint_axis("│") << '\n';
        }

        os << string(label_w, ' ') << paint_axis("╰")
           << paint_axis(repeat_glyph("─", width)) << paint_axis("╯") << ' '
           << paint_axis("→x") << '\n';

        string xline(label_w + 1 + width, ' ');
        auto place_xtick = [&](long double x) {
            auto [col, _] = to_cell(x, min_y);
            string lab = format_tick(x);
            int start = label_w + 1 + col - static_cast<int>(lab.size()) / 2;
            start = max(label_w + 1,
                        min(start, label_w + width - static_cast<int>(lab.size()) + 1));
            for (size_t i = 0; i < lab.size() &&
                               start + static_cast<int>(i) < static_cast<int>(xline.size());
                 ++i)
                xline[start + static_cast<int>(i)] = lab[i];
        };
        place_xtick(min_x);
        place_xtick((min_x + max_x) / 2);
        place_xtick(max_x);
        if (min_x < 0 && 0 < max_x) place_xtick(0);
        os << kDim << xline << kReset << '\n';

        if (!legend.empty()) {
            os << kDim << "  legend" << kReset << '\n';
            for (const auto& line : legend) os << "  " << line << '\n';
        }
    }
};
inline void expand_xy(Canvas& canvas, long double x, long double y) {
    canvas.expand(x, y);
}
template <class P>
void expand_point_like(Canvas& canvas, const P& point) {
    auto [sx, sy] = ::debug::graphical::project_point(point);
    canvas.expand(sx, sy);
}
template <class P>
void draw_point_like(Canvas& canvas, const P& point, int color_idx) {
    auto [sx, sy] = ::debug::graphical::project_point(point);
    canvas.plot_point(sx, sy, color_idx);
}
template <class A, class B>
void expand_object(Canvas& canvas, const pair<A, B>& point) {
    expand_point_like(canvas, point);
}
template <class... T>
void expand_object(Canvas& canvas, const tuple<T...>& point) {
    expand_point_like(canvas, point);
}
template <class T>
long double to_plot_ld(const T& v) {
    return static_cast<long double>(v);
}
template <class Seq>
void expand_stem_seq(Canvas& canvas, const Seq& seq) {
    if (std::empty(seq)) {
        canvas.expand(0, 0);
        canvas.expand(1, 1);
        return;
    }
    size_t i = 0;
    for (const auto& v : seq) {
        const long double y = to_plot_ld(v);
        canvas.expand(static_cast<long double>(i), 0);
        canvas.expand(static_cast<long double>(i), y);
        ++i;
    }
    if (i == 1) canvas.expand(1, 0);
}
template <class Seq>
void draw_stem_seq(Canvas& canvas, const Seq& seq, int color_idx) {
    size_t i = 0;
    for (const auto& v : seq) {
        const long double x = static_cast<long double>(i);
        const long double y = to_plot_ld(v);
        canvas.draw_segment(x, 0, x, y, color_idx);
        canvas.plot_point(x, y, color_idx);
        ++i;
    }
}
template <class Mat>
void expand_heatmap(Canvas& canvas, const Mat& mat) {
    size_t rows = 0, cols = 0;
    for (const auto& row : mat) {
        ++rows;
        cols = max(cols, static_cast<size_t>(std::size(row)));
    }
    if (rows == 0 || cols == 0) {
        canvas.expand(0, 0);
        canvas.expand(1, 1);
        return;
    }
    canvas.expand(0, 0);
    canvas.expand(static_cast<long double>(cols) - 1,
                  static_cast<long double>(rows) - 1);
}
template <class Mat>
void draw_heatmap(Canvas& canvas, const Mat& mat, int color_idx,
                  long double x_offset = 0) {
    long double lo = numeric_limits<long double>::infinity();
    long double hi = -numeric_limits<long double>::infinity();
    size_t rows = 0, cols = 0;
    for (const auto& row : mat) {
        ++rows;
        cols = max(cols, static_cast<size_t>(std::size(row)));
        for (const auto& v : row) {
            const long double x = to_plot_ld(v);
            lo = min(lo, x);
            hi = max(hi, x);
        }
    }
    if (rows == 0 || cols == 0) return;
    if (!isfinite(lo) || !isfinite(hi) || fabsl(hi - lo) < 1e-18L) {
        lo = 0;
        hi = 1;
    }
    constexpr size_t n_c = sizeof(kColors) / sizeof(kColors[0]);
    size_t r = 0;
    for (const auto& row : mat) {
        size_t c = 0;
        for (const auto& v : row) {
            const long double t = (to_plot_ld(v) - lo) / (hi - lo);
            const int shade =
                static_cast<int>((color_idx + static_cast<int>(t * (n_c - 1))) % n_c);
            const long double x = x_offset + static_cast<long double>(c);
            const long double y = static_cast<long double>(rows - 1 - r);
            auto [sx, sy] = canvas.to_subpixel(x, y);
            const int base_c = sx / 2, base_r = sy / 4;
            for (int ly = 0; ly < 4; ++ly)
                for (int lx = 0; lx < 2; ++lx)
                    canvas.plot_subpixel(base_c * 2 + lx, base_r * 4 + ly, shade);
            ++c;
        }
        ++r;
    }
}
template <class Volume>
void expand_volume_layers(Canvas& canvas, const Volume& volume) {
    size_t xoff = 0;
    constexpr size_t gap = 1;
    bool any = false;
    for (const auto& layer : volume) {
        size_t rows = 0, cols = 0;
        for (const auto& row : layer) {
            ++rows;
            cols = max(cols, static_cast<size_t>(std::size(row)));
        }
        if (rows == 0 || cols == 0) continue;
        any = true;
        canvas.expand(static_cast<long double>(xoff), 0);
        canvas.expand(static_cast<long double>(xoff + cols - 1),
                      static_cast<long double>(rows - 1));
        xoff += cols + gap;
    }
    if (!any) {
        canvas.expand(0, 0);
        canvas.expand(1, 1);
    }
}
template <class Volume>
void draw_volume_layers(Canvas& canvas, const Volume& volume, int color_idx) {
    size_t xoff = 0;
    constexpr size_t gap = 1;
    size_t layer_i = 0;
    constexpr size_t n_c = sizeof(kColors) / sizeof(kColors[0]);
    for (const auto& layer : volume) {
        size_t cols = 0;
        for (const auto& row : layer)
            cols = max(cols, static_cast<size_t>(std::size(row)));
        if (cols == 0) {
            ++layer_i;
            continue;
        }
        const int layer_color =
            static_cast<int>((static_cast<size_t>(color_idx) + layer_i) % n_c);
        draw_heatmap(canvas, layer, layer_color, static_cast<long double>(xoff));
        xoff += cols + gap;
        ++layer_i;
    }
}
template <class T, class = void>
struct is_iterable : false_type {};
template <class T>
struct is_iterable<T, void_t<decltype(std::begin(declval<const T&>())),
                             decltype(std::end(declval<const T&>()))>> : true_type {};
template <class T, class = void>
struct iter_value {
    using type = void;
};
template <class T>
struct iter_value<T, enable_if_t<is_iterable<T>::value>> {
    using type = remove_cv_t<remove_reference_t<decltype(*std::begin(declval<const T&>()))>>;
};
template <class T>
using iter_value_t = typename iter_value<T>::type;
template <class T>
inline constexpr bool is_arith_seq_v =
    is_iterable<T>::value && is_arithmetic_v<iter_value_t<T>>;
template <class T>
inline constexpr bool is_arith_matrix_v =
    is_iterable<T>::value && is_arith_seq_v<iter_value_t<T>>;
template <class T>
inline constexpr bool is_arith_volume_v =
    is_iterable<T>::value && is_arith_matrix_v<iter_value_t<T>>;

template <class Seq>
void expand_seq_or_nested(Canvas& canvas, const Seq& seq) {
    using V = remove_cv_t<remove_reference_t<decltype(*std::begin(seq))>>;
    if constexpr (is_arithmetic_v<V>) {
        expand_stem_seq(canvas, seq);
    } else if constexpr (is_arith_volume_v<Seq>) {
        expand_volume_layers(canvas, seq);
    } else if constexpr (is_arith_matrix_v<Seq>) {
        expand_heatmap(canvas, seq);
    } else {
        for (const auto& elem : seq) expand_object(canvas, elem);
    }
}
template <class Seq>
void draw_seq_or_nested(Canvas& canvas, const Seq& seq, int color_idx) {
    using V = remove_cv_t<remove_reference_t<decltype(*std::begin(seq))>>;
    if constexpr (is_arithmetic_v<V>) {
        draw_stem_seq(canvas, seq, color_idx);
    } else if constexpr (is_arith_volume_v<Seq>) {
        draw_volume_layers(canvas, seq, color_idx);
    } else if constexpr (is_arith_matrix_v<Seq>) {
        draw_heatmap(canvas, seq, color_idx);
    } else {
        for (const auto& elem : seq) draw_object(canvas, elem, color_idx);
    }
}
template <class Set>
void expand_set_like(Canvas& canvas, const Set& s) {
    using V = typename Set::value_type;
    if constexpr (is_arithmetic_v<V>) {
        if (s.empty()) {
            canvas.expand(0, 0);
            return;
        }
        for (const auto& v : s) {
            canvas.expand(to_plot_ld(v), 0);
            canvas.expand(to_plot_ld(v), 1);
        }
    } else {
        for (const auto& elem : s) expand_object(canvas, elem);
    }
}
template <class Set>
void draw_set_like(Canvas& canvas, const Set& s, int color_idx) {
    using V = typename Set::value_type;
    if constexpr (is_arithmetic_v<V>) {
        for (const auto& v : s) {
            const long double x = to_plot_ld(v);
            canvas.draw_segment(x, 0, x, 1, color_idx);
            canvas.plot_point(x, 1, color_idx);
        }
    } else {
        for (const auto& elem : s) draw_object(canvas, elem, color_idx);
    }
}
template <class MultiSet>
void expand_mset_like(Canvas& canvas, const MultiSet& s) {
    using V = typename MultiSet::value_type;
    if constexpr (is_arithmetic_v<V>) {
        if (s.empty()) {
            canvas.expand(0, 0);
            return;
        }
        map<V, int> freq;
        for (const auto& v : s) ++freq[v];
        for (const auto& [v, f] : freq) {
            canvas.expand(to_plot_ld(v), 0);
            canvas.expand(to_plot_ld(v), to_plot_ld(f));
        }
    } else {
        for (const auto& elem : s) expand_object(canvas, elem);
    }
}
template <class MultiSet>
void draw_mset_like(Canvas& canvas, const MultiSet& s, int color_idx) {
    using V = typename MultiSet::value_type;
    if constexpr (is_arithmetic_v<V>) {
        map<V, int> freq;
        for (const auto& v : s) ++freq[v];
        for (const auto& [v, f] : freq) {
            const long double x = to_plot_ld(v);
            canvas.draw_segment(x, 0, x, to_plot_ld(f), color_idx);
            canvas.plot_point(x, to_plot_ld(f), color_idx);
        }
    } else {
        for (const auto& elem : s) draw_object(canvas, elem, color_idx);
    }
}
template <class Map>
void expand_map_like(Canvas& canvas, const Map& mp) {
    using K = typename Map::key_type;
    using V = typename Map::mapped_type;
    if constexpr (is_arithmetic_v<K> && is_arithmetic_v<V>) {
        if (mp.empty()) {
            canvas.expand(0, 0);
            return;
        }
        for (const auto& [k, v] : mp) {
            canvas.expand(to_plot_ld(k), 0);
            canvas.expand(to_plot_ld(k), to_plot_ld(v));
        }
    } else {
        size_t i = 0;
        for (const auto& [k, v] : mp) {
            (void)k;
            if constexpr (is_arithmetic_v<V>) {
                canvas.expand(static_cast<long double>(i), 0);
                canvas.expand(static_cast<long double>(i), to_plot_ld(v));
            } else {
                expand_object(canvas, v);
            }
            ++i;
        }
    }
}
template <class Map>
void draw_map_like(Canvas& canvas, const Map& mp, int color_idx) {
    using K = typename Map::key_type;
    using V = typename Map::mapped_type;
    if constexpr (is_arithmetic_v<K> && is_arithmetic_v<V>) {
        for (const auto& [k, v] : mp) {
            const long double x = to_plot_ld(k);
            const long double y = to_plot_ld(v);
            canvas.draw_segment(x, 0, x, y, color_idx);
            canvas.plot_point(x, y, color_idx);
        }
    } else if constexpr (is_arithmetic_v<V>) {
        size_t i = 0;
        for (const auto& [k, v] : mp) {
            (void)k;
            const long double x = static_cast<long double>(i);
            const long double y = to_plot_ld(v);
            canvas.draw_segment(x, 0, x, y, color_idx);
            canvas.plot_point(x, y, color_idx);
            ++i;
        }
    } else {
        for (const auto& [k, v] : mp) {
            (void)k;
            draw_object(canvas, v, color_idx);
        }
    }
}
template <class T, size_t N>
void expand_object(Canvas& canvas, const array<T, N>& a) {
    if constexpr (is_arithmetic_v<T> && (N == 2 || N == 3)) {
        expand_point_like(canvas, a);
    } else if constexpr (is_arithmetic_v<T>) {
        expand_stem_seq(canvas, a);
    } else {
        for (const auto& elem : a) expand_object(canvas, elem);
    }
}
template <class T>
void expand_object(Canvas& canvas, const vector<T>& seq) {
    expand_seq_or_nested(canvas, seq);
}
template <class T>
void expand_object(Canvas& canvas, const vector<vector<T>>& mat) {
    if constexpr (is_arithmetic_v<T>) {
        expand_heatmap(canvas, mat);
    } else {
        for (const auto& row : mat) expand_object(canvas, row);
    }
}
template <class T>
void expand_object(Canvas& canvas, const vector<vector<vector<T>>>& volume) {
    if constexpr (is_arithmetic_v<T>) {
        expand_volume_layers(canvas, volume);
    } else {
        for (const auto& layer : volume) expand_object(canvas, layer);
    }
}
template <class T>
void expand_object(Canvas& canvas, const deque<T>& seq) {
    expand_seq_or_nested(canvas, seq);
}
template <class T>
void expand_object(Canvas& canvas, const list<T>& seq) {
    expand_seq_or_nested(canvas, seq);
}
template <class T, class Comp, class Alloc>
void expand_object(Canvas& canvas, const set<T, Comp, Alloc>& s) {
    expand_set_like(canvas, s);
}
template <class T, class Hash, class Eq, class Alloc>
void expand_object(Canvas& canvas, const unordered_set<T, Hash, Eq, Alloc>& s) {
    expand_set_like(canvas, s);
}
template <class T, class Comp, class Alloc>
void expand_object(Canvas& canvas, const multiset<T, Comp, Alloc>& s) {
    expand_mset_like(canvas, s);
}
template <class T, class Hash, class Eq, class Alloc>
void expand_object(Canvas& canvas, const unordered_multiset<T, Hash, Eq, Alloc>& s) {
    expand_mset_like(canvas, s);
}
template <class K, class V, class Comp, class Alloc>
void expand_object(Canvas& canvas, const map<K, V, Comp, Alloc>& mp) {
    expand_map_like(canvas, mp);
}
template <class K, class V, class Comp, class Alloc>
void expand_object(Canvas& canvas, const multimap<K, V, Comp, Alloc>& mp) {
    expand_map_like(canvas, mp);
}
template <class K, class V, class Hash, class Eq, class Alloc>
void expand_object(Canvas& canvas, const unordered_map<K, V, Hash, Eq, Alloc>& mp) {
    expand_map_like(canvas, mp);
}
template <class K, class V, class Hash, class Eq, class Alloc>
void expand_object(Canvas& canvas,
                   const unordered_multimap<K, V, Hash, Eq, Alloc>& mp) {
    expand_map_like(canvas, mp);
}
template <class T, class C>
void expand_object(Canvas& canvas, stack<T, C> st) {
    vector<T> vals;
    while (!st.empty()) {
        vals.push_back(st.top());
        st.pop();
    }
    reverse(vals.begin(), vals.end());
    expand_object(canvas, vals);
}
template <class T, class C>
void expand_object(Canvas& canvas, queue<T, C> q) {
    vector<T> vals;
    while (!q.empty()) {
        vals.push_back(q.front());
        q.pop();
    }
    expand_object(canvas, vals);
}
template <class T, class C, class Comp>
void expand_object(Canvas& canvas, priority_queue<T, C, Comp> pq) {
    vector<T> vals;
    while (!pq.empty()) {
        vals.push_back(pq.top());
        pq.pop();
    }
    expand_object(canvas, vals);
}
template <size_t N>
void expand_object(Canvas& canvas, const bitset<N>& bs) {
    vector<int> bits(N);
    for (size_t i = 0; i < N; ++i) bits[i] = bs[i];
    expand_stem_seq(canvas, bits);
}
inline void expand_object(Canvas& canvas, const string& s) {
    expand_stem_seq(canvas, s);
}
inline void expand_object(Canvas& canvas, string_view s) {
    expand_stem_seq(canvas, s);
}
template <class T>
void expand_object(Canvas& canvas, const optional<T>& o) {
    if (o) expand_object(canvas, *o);
    else {
        canvas.expand(0, 0);
        canvas.expand(1, 1);
    }
}
template <class T, enable_if_t<is_arithmetic_v<T>, int> = 0>
void expand_object(Canvas& canvas, const T& v) {
    canvas.expand(0, 0);
    canvas.expand(0, to_plot_ld(v));
    if (fabsl(to_plot_ld(v)) < 1e-18L) canvas.expand(1, 1);
    else canvas.expand(1, 0);
}
template <class T>
void expand_object(Canvas& canvas, const forward_list<T>& seq) {
    expand_seq_or_nested(canvas, seq);
}
#if __has_include(<span>)
template <class T, size_t Extent>
void expand_object(Canvas& canvas, span<T, Extent> seq) {
    expand_seq_or_nested(canvas, seq);
}
#endif
template <class A, class B>
void draw_object(Canvas& canvas, const pair<A, B>& point, int color_idx) {
    draw_point_like(canvas, point, color_idx);
}
template <class... T>
void draw_object(Canvas& canvas, const tuple<T...>& point, int color_idx) {
    draw_point_like(canvas, point, color_idx);
}
template <class T, size_t N>
void draw_object(Canvas& canvas, const array<T, N>& a, int color_idx) {
    if constexpr (is_arithmetic_v<T> && (N == 2 || N == 3)) {
        draw_point_like(canvas, a, color_idx);
    } else if constexpr (is_arithmetic_v<T>) {
        draw_stem_seq(canvas, a, color_idx);
    } else {
        for (const auto& elem : a) draw_object(canvas, elem, color_idx);
    }
}
template <class T>
void draw_object(Canvas& canvas, const vector<T>& seq, int color_idx) {
    draw_seq_or_nested(canvas, seq, color_idx);
}
template <class T>
void draw_object(Canvas& canvas, const vector<vector<T>>& mat, int color_idx) {
    if constexpr (is_arithmetic_v<T>) {
        draw_heatmap(canvas, mat, color_idx);
    } else {
        for (const auto& row : mat) draw_object(canvas, row, color_idx);
    }
}
template <class T>
void draw_object(Canvas& canvas, const vector<vector<vector<T>>>& volume,
                 int color_idx) {
    if constexpr (is_arithmetic_v<T>) {
        draw_volume_layers(canvas, volume, color_idx);
    } else {
        for (const auto& layer : volume) draw_object(canvas, layer, color_idx);
    }
}
template <class T>
void draw_object(Canvas& canvas, const deque<T>& seq, int color_idx) {
    draw_seq_or_nested(canvas, seq, color_idx);
}
template <class T>
void draw_object(Canvas& canvas, const list<T>& seq, int color_idx) {
    draw_seq_or_nested(canvas, seq, color_idx);
}
template <class T, class Comp, class Alloc>
void draw_object(Canvas& canvas, const set<T, Comp, Alloc>& s, int color_idx) {
    draw_set_like(canvas, s, color_idx);
}
template <class T, class Hash, class Eq, class Alloc>
void draw_object(Canvas& canvas, const unordered_set<T, Hash, Eq, Alloc>& s,
                 int color_idx) {
    draw_set_like(canvas, s, color_idx);
}
template <class T, class Comp, class Alloc>
void draw_object(Canvas& canvas, const multiset<T, Comp, Alloc>& s, int color_idx) {
    draw_mset_like(canvas, s, color_idx);
}
template <class T, class Hash, class Eq, class Alloc>
void draw_object(Canvas& canvas, const unordered_multiset<T, Hash, Eq, Alloc>& s,
                 int color_idx) {
    draw_mset_like(canvas, s, color_idx);
}
template <class K, class V, class Comp, class Alloc>
void draw_object(Canvas& canvas, const map<K, V, Comp, Alloc>& mp, int color_idx) {
    draw_map_like(canvas, mp, color_idx);
}
template <class K, class V, class Comp, class Alloc>
void draw_object(Canvas& canvas, const multimap<K, V, Comp, Alloc>& mp,
                 int color_idx) {
    draw_map_like(canvas, mp, color_idx);
}
template <class K, class V, class Hash, class Eq, class Alloc>
void draw_object(Canvas& canvas, const unordered_map<K, V, Hash, Eq, Alloc>& mp,
                 int color_idx) {
    draw_map_like(canvas, mp, color_idx);
}
template <class K, class V, class Hash, class Eq, class Alloc>
void draw_object(Canvas& canvas,
                 const unordered_multimap<K, V, Hash, Eq, Alloc>& mp, int color_idx) {
    draw_map_like(canvas, mp, color_idx);
}
template <class T, class C>
void draw_object(Canvas& canvas, stack<T, C> st, int color_idx) {
    vector<T> vals;
    while (!st.empty()) {
        vals.push_back(st.top());
        st.pop();
    }
    reverse(vals.begin(), vals.end());
    draw_object(canvas, vals, color_idx);
}
template <class T, class C>
void draw_object(Canvas& canvas, queue<T, C> q, int color_idx) {
    vector<T> vals;
    while (!q.empty()) {
        vals.push_back(q.front());
        q.pop();
    }
    draw_object(canvas, vals, color_idx);
}
template <class T, class C, class Comp>
void draw_object(Canvas& canvas, priority_queue<T, C, Comp> pq, int color_idx) {
    vector<T> vals;
    while (!pq.empty()) {
        vals.push_back(pq.top());
        pq.pop();
    }
    draw_object(canvas, vals, color_idx);
}
template <size_t N>
void draw_object(Canvas& canvas, const bitset<N>& bs, int color_idx) {
    vector<int> bits(N);
    for (size_t i = 0; i < N; ++i) bits[i] = bs[i];
    draw_stem_seq(canvas, bits, color_idx);
}
inline void draw_object(Canvas& canvas, const string& s, int color_idx) {
    draw_stem_seq(canvas, s, color_idx);
}
inline void draw_object(Canvas& canvas, string_view s, int color_idx) {
    draw_stem_seq(canvas, s, color_idx);
}
template <class T>
void draw_object(Canvas& canvas, const optional<T>& o, int color_idx) {
    if (o) draw_object(canvas, *o, color_idx);
}
template <class T, enable_if_t<is_arithmetic_v<T>, int> = 0>
void draw_object(Canvas& canvas, const T& v, int color_idx) {
    const long double y = to_plot_ld(v);
    canvas.draw_segment(0, 0, 0, y, color_idx);
    canvas.plot_point(0, y, color_idx);
}
template <class T>
void draw_object(Canvas& canvas, const forward_list<T>& seq, int color_idx) {
    draw_seq_or_nested(canvas, seq, color_idx);
}
#if __has_include(<span>)
template <class T, size_t Extent>
void draw_object(Canvas& canvas, span<T, Extent> seq, int color_idx) {
    draw_seq_or_nested(canvas, seq, color_idx);
}
#endif
template <class T>
string summarize(const T& value) {
    ostringstream oss;
    oss << value;
    string s = oss.str();
    if (s.size() > 72) s = s.substr(0, 69) + "...";
    return s;
}
template <class... Shapes>
void render(int width, int height, ostream& os, const Shapes&... shapes) {
    Canvas canvas(width, height);
    size_t mark_i = 0;
    (expand_object(canvas, shapes), ...);
    canvas.pad_bounds();
    canvas.draw_axes();
    (
        [&] {
            constexpr size_t n_g = sizeof(kGlyphs) / sizeof(kGlyphs[0]);
            constexpr size_t n_c = sizeof(kColors) / sizeof(kColors[0]);
            const int color_idx = static_cast<int>(mark_i % n_c);
            const char* glyph = kGlyphs[mark_i % n_g];
            const char* color = kColors[static_cast<size_t>(color_idx)];
            ostringstream leg;
            leg << paint(color, glyph) << ' ' << kDim << '#' << mark_i << kReset << ' '
                << summarize(shapes);
            canvas.legend.push_back(leg.str());
            draw_object(canvas, shapes, color_idx);
            ++mark_i;
        }(),
        ...);
    canvas.write(os);
    os.flush();
}
}  // namespace debug::graphical

using debug::graphical::braille_bit;
using debug::graphical::braille_char;
using debug::graphical::braille_or;

namespace ac_detail::graphical_print_detail {
template <class...>
inline constexpr bool size_prefix_v = false;
template <class A, class B, class... Rest>
inline constexpr bool size_prefix_v<A, B, Rest...> =
    is_integral_v<A> && is_integral_v<B> && (sizeof...(Rest) >= 1);
}  // namespace ac_detail::graphical_print_detail

template <class A, class B, class... Rest,
          enable_if_t<is_integral_v<A> && is_integral_v<B> && (sizeof...(Rest) >= 1),
                      int> = 0>
void graphical_print(A width, B height, const Rest&... rest) {
    debug::graphical::render(static_cast<int>(width), static_cast<int>(height), cerr,
                             rest...);
}

template <class... Shapes,
          enable_if_t<(sizeof...(Shapes) >= 1) &&
                          !ac_detail::graphical_print_detail::size_prefix_v<Shapes...>,
                      int> = 0>
void graphical_print(const Shapes&... shapes) {
    debug::graphical::render(debug::graphical::kDefaultWidth,
                             debug::graphical::kDefaultHeight, cerr, shapes...);
}
#else
#ifndef graphical_print
#define graphical_print(...) ((void)0)
#endif
#ifndef braille_char
#define braille_char(...) (::std::string{})
#endif
#ifndef braille_bit
#define braille_bit(...) (0)
#endif
#ifndef braille_or
#define braille_or(dots, ...) (dots)
#endif
#endif
