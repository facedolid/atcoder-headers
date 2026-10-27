#pragma once
// Library: https://github.com/facedolid/atcoder-headers

#include <array>
#include <bitset>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace debug {

inline constexpr std::size_t TRACE_MAX_ELEMENTS = 100;

#ifdef TRACE_COLOR
inline constexpr const char* TRACE_COLOR_BEGIN = "\033[36m";
inline constexpr const char* TRACE_COLOR_END = "\033[0m";
#else
inline constexpr const char* TRACE_COLOR_BEGIN = "";
inline constexpr const char* TRACE_COLOR_END = "";
#endif

/// @brief ファイルパスから最後のファイル名部分を返す。
inline const char* trace_basename(const char* path) {
    const char* base = path;
    for (const char* p = path; *p != '\0'; ++p) {
        if (*p == '/' || *p == '\\') {
            base = p + 1;
        }
    }
    return base;
}

/// @brief `trace` の引数文字列を括弧・引用符を考慮して名前へ分割する。
inline std::vector<std::string> trace_split_names(const char* names, bool use_angle = true) {
    std::vector<std::string> result;
    if (names == nullptr || names[0] == '\0') {
        return result;
    }

    std::string current;
    int paren = 0;
    int bracket = 0;
    int brace = 0;
    int angle = 0;
    bool in_string = false;
    bool in_char = false;

    auto push_current = [&]() {
        std::size_t start = 0;
        while (start < current.size() && current[start] == ' ') {
            ++start;
        }
        std::size_t end = current.size();
        while (end > start && current[end - 1] == ' ') {
            --end;
        }
        if (start < end) {
            result.emplace_back(current.substr(start, end - start));
        }
        current.clear();
    };

    for (const char* p = names; *p != '\0'; ++p) {
        const char c = *p;

        if (in_string) {
            current.push_back(c);
            if (c == '\\' && *(p + 1) != '\0') {
                current.push_back(*(++p));
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }

        if (in_char) {
            current.push_back(c);
            if (c == '\\' && *(p + 1) != '\0') {
                current.push_back(*(++p));
            } else if (c == '\'') {
                in_char = false;
            }
            continue;
        }

        if (c == '"') {
            in_string = true;
            current.push_back(c);
            continue;
        }
        if (c == '\'') {
            in_char = true;
            current.push_back(c);
            continue;
        }

        switch (c) {
            case '(':
                ++paren;
                break;
            case ')':
                --paren;
                break;
            case '[':
                ++bracket;
                break;
            case ']':
                --bracket;
                break;
            case '{':
                ++brace;
                break;
            case '}':
                --brace;
                break;
            case '<':
                if (use_angle) ++angle;
                break;
            case '>':
                if (use_angle) --angle;
                break;
            case ',':
                if (paren == 0 && bracket == 0 && brace == 0 && angle == 0) {
                    push_current();
                    continue;
                }
                break;
            default:
                break;
        }

        current.push_back(c);
    }

    push_current();
    return result;
}

template <typename T>
concept StringLike = std::same_as<std::remove_cvref_t<T>, std::string> ||
                     std::same_as<std::remove_cvref_t<T>, std::string_view>;

template <typename T>
concept CStringLike = std::same_as<std::remove_cvref_t<T>, const char*> ||
                      std::same_as<std::remove_cvref_t<T>, char*>;

template <typename T>
struct is_std_pair : std::false_type {};

template <typename T, typename U>
struct is_std_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
struct is_std_tuple : std::false_type {};

template <typename... Ts>
struct is_std_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T>
struct is_std_bitset : std::false_type {};

template <std::size_t N>
struct is_std_bitset<std::bitset<N>> : std::true_type {};

template <typename T>
struct is_std_optional : std::false_type {};

template <typename T>
struct is_std_optional<std::optional<T>> : std::true_type {};

template <typename T>
struct is_std_variant : std::false_type {};

template <typename... Ts>
struct is_std_variant<std::variant<Ts...>> : std::true_type {};

template <typename T>
struct is_std_queue : std::false_type {};

template <typename T, typename Container>
struct is_std_queue<std::queue<T, Container>> : std::true_type {};

template <typename T>
struct is_std_stack : std::false_type {};

template <typename T, typename Container>
struct is_std_stack<std::stack<T, Container>> : std::true_type {};

template <typename T>
struct is_std_priority_queue : std::false_type {};

template <typename T, typename Container, typename Compare>
struct is_std_priority_queue<std::priority_queue<T, Container, Compare>> : std::true_type {};

template <typename T>
concept Iterable = requires(T& value) {
    { std::begin(value) } -> std::input_or_output_iterator;
    { std::end(value) } -> std::sentinel_for<decltype(std::begin(value))>;
} && !StringLike<T> && !CStringLike<T> && !is_std_pair<std::remove_cvref_t<T>>::value &&
   !is_std_tuple<std::remove_cvref_t<T>>::value &&
   !is_std_bitset<std::remove_cvref_t<T>>::value &&
   !is_std_optional<std::remove_cvref_t<T>>::value &&
   !is_std_variant<std::remove_cvref_t<T>>::value &&
   !is_std_queue<std::remove_cvref_t<T>>::value &&
   !is_std_stack<std::remove_cvref_t<T>>::value &&
   !is_std_priority_queue<std::remove_cvref_t<T>>::value;

template <typename T>
concept MapLike = Iterable<T> && requires {
    typename T::key_type;
    typename T::mapped_type;
};

template <typename T>
concept Streamable = requires(std::ostream& os, T& value) {
    { os << value } -> std::same_as<std::ostream&>;
} && !std::is_same_v<std::remove_cvref_t<T>, std::nullptr_t> &&
   !std::is_same_v<std::remove_cvref_t<T>, bool> &&
   !std::is_same_v<std::remove_cvref_t<T>, char> &&
   !std::is_same_v<std::remove_cvref_t<T>, signed char> &&
   !std::is_same_v<std::remove_cvref_t<T>, unsigned char> &&
   !StringLike<T> && !CStringLike<T> && !is_std_pair<std::remove_cvref_t<T>>::value &&
   !is_std_tuple<std::remove_cvref_t<T>>::value &&
   !is_std_bitset<std::remove_cvref_t<T>>::value &&
   !is_std_optional<std::remove_cvref_t<T>>::value &&
   !is_std_variant<std::remove_cvref_t<T>>::value &&
   !is_std_queue<std::remove_cvref_t<T>>::value &&
   !is_std_stack<std::remove_cvref_t<T>>::value &&
   !is_std_priority_queue<std::remove_cvref_t<T>>::value &&
   !Iterable<T>;

template <typename T>
/// @brief 値をトレース出力へ表示する。
void trace_print(std::ostream& os, T&& value);

template <typename T>
/// @brief iterableの要素を上限付きで区切り表示する。
void trace_print_delimited(std::ostream& os, T&& value) {
    bool first = true;
    std::size_t count = 0;
    for (auto it = std::begin(value); it != std::end(value); ++it) {
        if (count >= TRACE_MAX_ELEMENTS) {
            os << ", ...";
            break;
        }
        if (!first) {
            os << ", ";
        }
        trace_print(os, *it);
        first = false;
        ++count;
    }
}

template <typename T>
/// @brief map風コンテナのキーと値を区切り表示する。
void trace_print_map_like(std::ostream& os, T&& value) {
    bool first = true;
    std::size_t count = 0;
    for (auto it = std::begin(value); it != std::end(value); ++it) {
        if (count >= TRACE_MAX_ELEMENTS) {
            os << ", ...";
            break;
        }
        if (!first) {
            os << ", ";
        }
        os << '(';
        trace_print(os, it->first);
        os << ", ";
        trace_print(os, it->second);
        os << ')';
        first = false;
        ++count;
    }
}

template <std::size_t... Is, typename Tuple>
/// @brief tupleの指定された要素列を表示する内部処理。
void trace_print_tuple_impl(std::ostream& os, Tuple&& tuple, std::index_sequence<Is...>) {
    bool first = true;
    auto print_elem = [&](auto&& elem) {
        if (!first) {
            os << ", ";
        }
        trace_print(os, std::forward<decltype(elem)>(elem));
        first = false;
    };
    (print_elem(std::get<Is>(tuple)), ...);
}

template <typename T>
/// @brief 型に応じた形式で値をトレース出力へ表示する。
void trace_print(std::ostream& os, T&& value) {
    using U = std::remove_cvref_t<T>;

    if constexpr (std::is_same_v<U, std::nullptr_t>) {
        os << "nullptr";
    } else if constexpr (std::is_same_v<U, bool>) {
        os << (value ? "true" : "false");
    } else if constexpr (std::is_same_v<U, char>) {
        if (value == 0) {
            os << "false";
        } else if (value == 1) {
            os << "true";
        } else {
            os << '\'' << value << '\'';
        }
    } else if constexpr (std::is_same_v<U, signed char>) {
        os << static_cast<int>(value);
    } else if constexpr (std::is_same_v<U, unsigned char>) {
        os << static_cast<unsigned>(value);
    } else if constexpr (StringLike<T>) {
        os << '"' << value << '"';
    } else if constexpr (CStringLike<T>) {
        if (value == nullptr) {
            os << "nullptr";
        } else {
            os << '"' << value << '"';
        }
    } else if constexpr (is_std_optional<U>::value) {
        if (value.has_value()) {
            os << "optional(";
            trace_print(os, *value);
            os << ')';
        } else {
            os << "nullopt";
        }
    } else if constexpr (is_std_variant<U>::value) {
        os << "variant[" << value.index() << "](";
        std::visit([&](const auto& elem) { trace_print(os, elem); }, value);
        os << ')';
    } else if constexpr (is_std_bitset<U>::value) {
        os << value;
    } else if constexpr (is_std_queue<U>::value) {
        U copy = value;
        os << "queue[";
        bool first = true;
        std::size_t count = 0;
        while (!copy.empty()) {
            if (count >= TRACE_MAX_ELEMENTS) {
                os << ", ...";
                break;
            }
            if (!first) {
                os << ", ";
            }
            trace_print(os, copy.front());
            copy.pop();
            first = false;
            ++count;
        }
        os << ']';
    } else if constexpr (is_std_stack<U>::value) {
        U copy = value;
        std::vector<typename U::value_type> elems;
        while (!copy.empty()) {
            elems.push_back(copy.top());
            copy.pop();
        }
        os << "stack[";
        trace_print_delimited(os, elems);
        os << ']';
    } else if constexpr (is_std_priority_queue<U>::value) {
        U copy = value;
        std::vector<typename U::value_type> elems;
        while (!copy.empty()) {
            elems.push_back(copy.top());
            copy.pop();
        }
        os << "priority_queue[";
        trace_print_delimited(os, elems);
        os << ']';
    } else if constexpr (is_std_pair<U>::value) {
        os << '(';
        trace_print(os, value.first);
        os << ", ";
        trace_print(os, value.second);
        os << ')';
    } else if constexpr (is_std_tuple<U>::value) {
        os << '(';
        trace_print_tuple_impl(
            os, value, std::make_index_sequence<std::tuple_size_v<U>>{});
        os << ')';
    } else if constexpr (MapLike<T>) {
        os << '[';
        trace_print_map_like(os, value);
        os << ']';
    } else if constexpr (Iterable<T>) {
        os << '[';
        trace_print_delimited(os, value);
        os << ']';
    } else if constexpr (Streamable<T>) {
        os << value;
    } else {
        os << value;
    }
}

template <std::size_t I, typename T>
/// @brief 名前付き引数を1件表示する内部処理。
void trace_print_entry(std::ostream& os, const std::vector<std::string>& names,
                       T&& value) {
    if (I > 0) {
        os << " | ";
    }
    if (I < names.size()) {
        os << names[I] << " = ";
    } else {
        os << "arg" << I << " = ";
    }
    trace_print(os, std::forward<T>(value));
}

template <std::size_t... Is, typename... Args>
/// @brief 複数のトレース引数をまとめて表示する内部処理。
void trace_print_all(std::ostream& os, const std::vector<std::string>& names,
                     std::index_sequence<Is...>, Args&&... args) {
    (trace_print_entry<Is>(os, names, std::forward<Args>(args)), ...);
}

template <typename... Args>
/// @brief ファイル名・行番号付きでトレースを出力する。
void trace_impl(const char* file, int line, const char* names, Args&&... args) {
    // 引数数と分割数を照合する。マクロ展開や比較式とテンプレートの混在は曖昧さが残る。
    auto name_list = trace_split_names(names, false);
    if (name_list.size() != sizeof...(Args)) name_list = trace_split_names(names);
    std::ostringstream body;
    trace_print_all(body, name_list, std::index_sequence_for<Args...>{},
                    std::forward<Args>(args)...);

    std::cerr << TRACE_COLOR_BEGIN << "[TRACE " << trace_basename(file) << ':'
            << line << ']' << TRACE_COLOR_END << ' ' << body.str() << '\n';
}

template <typename Container, typename Index>
/// @brief 添字と、その添字で参照したコンテナ要素を表示する。
void trace_print_index(std::ostream& os, std::string_view container_name,
                       std::string_view index_name, const Container& container,
                       const Index& index) {
    os << "  " << index_name << " = ";
    trace_print(os, index);

    if constexpr (std::integral<std::remove_cvref_t<Index>> &&
                  !std::same_as<std::remove_cvref_t<Index>, bool> &&
                  requires { std::size(container); container[index]; }) {
        bool in_range = true;
        if constexpr (std::signed_integral<std::remove_cvref_t<Index>>) {
            in_range = index >= 0;
        }
        if (in_range) {
            using UnsignedIndex = std::make_unsigned_t<std::remove_cvref_t<Index>>;
            in_range = static_cast<UnsignedIndex>(index) < std::size(container);
        }

        os << " -> " << container_name << '[' << index_name << "] = ";
        if (in_range) {
            trace_print(os, container[index]);
        } else {
            os << "<out of range; size=" << std::size(container) << '>';
        }
    } else {
        os << " -> <添字表示非対応>";
    }
    os << '\n';
}

template <std::size_t... Is, typename Container, typename... Indices>
/// @brief 複数の添字について `trace_print_index` を呼び出す内部処理。
void trace_print_indices(std::ostream& os, std::string_view container_name,
                         const std::vector<std::string>& index_names,
                         const Container& container, std::index_sequence<Is...>,
                         const Indices&... indices) {
    (trace_print_index(
         os, container_name,
         Is < index_names.size() ? std::string_view(index_names[Is])
                                 : std::string_view("index"),
         container, indices),
     ...);
}

template <typename Expr, typename Container, typename... Indices>
/// @brief 式とコンテナ添字の値をファイル名・行番号付きで出力する。
void trace_indexed_impl(const char* file, int line, const char* expression_name,
                        Expr&& expression_value, const char* container_name,
                        const Container& container, const char* index_names,
                        const Indices&... indices) {
    std::cerr << TRACE_COLOR_BEGIN << "[TRACE " << trace_basename(file) << ':'
              << line << ']' << TRACE_COLOR_END << ' ' << expression_name << " = ";
    trace_print(std::cerr, std::forward<Expr>(expression_value));
    std::cerr << '\n';

    auto names = trace_split_names(index_names, false);
    if (names.size() != sizeof...(Indices)) names = trace_split_names(index_names);
    trace_print_indices(std::cerr, container_name, names, container,
                        std::index_sequence_for<Indices...>{}, indices...);
}

}  // namespace debug

#define trace(...) \
    debug::trace_impl(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
#define trace_idx(expr, container, ...)                                      \
    debug::trace_indexed_impl(__FILE__, __LINE__, #expr, (expr), #container, \
                              (container), #__VA_ARGS__, __VA_ARGS__)
