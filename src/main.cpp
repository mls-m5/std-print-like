#include <array>
#include <iostream>
#include <ranges>
#include <string_view>

template <int NumArgs>
struct FormatString {
    constexpr void place() {
        int count = 0;
        int prev = 0;
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

    template <int N>
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

    constexpr std::string_view at(int i) {
        auto part = parts.at(i);
        return data.substr(part.start, part.len);
    }

    template <typename... Args>
    void print(Args &&...args) {
        int current = 0;

        ((std::cout << at(current) << args), ...);

        std::cout << at(parts.size() - 1);
    }
};

/// Print a string and insert the argument at the places where '{}' is found
template <typename... Args>
void print(FormatString<sizeof...(Args)> str, Args &&...args) {
    str.print(args...);
}

int main(int argc, char *argv[]) {
    print("hello {} you\n", "there");

    return 0;
}
