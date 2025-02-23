/// Copyright Mattias Lasersköld 2025

#pragma once

#include <array>
#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string_view>

template <size_t NumArgs>
struct FormatString {
    constexpr void place() {
        size_t count = 0;
        size_t prev = 0;
        for (auto i : std::ranges::iota_view{0uz, data.size()}) {
            auto c = data[i];
            if (c == '{') {
                parts.at(count).start = prev;
                parts.at(count).len = i - prev;
                ++count;
                prev = i + 2;
            }
        }

        // This runs at compile time so we just want the compiler to stop and
        // print a message
        if (count < NumArgs) {
            throw "to many argument to format or print function";
        }
        if (count > NumArgs) {
            throw "to few argument to format or print function";
        }

        parts.at(count).start = prev;
        parts.at(count).len = data.size() - prev;
    }

    template <size_t N>
    consteval FormatString(const char (&str)[N])
        : data{&str[0], N - 1} {
        place();
    }

    std::string_view data;

    struct Part {
        size_t start;
        size_t len;
    };

    std::array<Part, NumArgs + 1> parts;

    constexpr std::string_view at(size_t i) {
        auto part = parts.at(i);
        return data.substr(part.start, part.len);
    }

    template <typename... Args>
    void print(std::ostream &stream, Args &&...args) {
        size_t current = 0;
        ((stream << at(current) << args), ...);
        stream << at(parts.size() - 1);
    }
};

/// Print a string and insert the argument at the places where '{}' is found
template <typename... Args>
void print(FormatString<sizeof...(Args)> str, Args &&...args) {
    str.print(std::cout, args...);
}

/// Create a string from arguments with '{}' syntax
template <typename... Args>
std::string format(FormatString<sizeof...(Args)> str, Args &&...args) {
    auto ss = std::ostringstream{};
    str.print(ss, args...);
    return ss.str();
}
