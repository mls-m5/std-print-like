#include <algorithm>
#include <array>
#include <format>
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
        parts.at(count).start = prev;
        parts.at(count).len = data.size() - prev;
    }

    template <int N>
    constexpr FormatString(const char (&str)[N])
        // constexpr FormatString(const char str[N])
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

template <typename... Args>
void print(FormatString<sizeof...(Args)> str, Args &&...args) {
    // for (auto &part : str.parts) {
    //     std::cout << part.start << " -> " << part.len << " = '"
    //               << str.data.substr(part.start, part.len) << "'\n";
    // }

    str.print(args...);
}

int main(int argc, char *argv[]) {
    print("hello {} you\n", "there");

    auto str = std::format("hello {}", "there");

    return 0;
}
