#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>

int main() {
    std::string input(std::istreambuf_iterator<char>(std::cin), {});;
    std::vector<std::uint32_t> layout;
    std::uint32_t id{};
    for (std::size_t i{}; i != input.size() && std::isdigit(input[i]); ++i) {
        std::fill_n(std::back_inserter(layout), input[i] - '0', i & 1? ~std::uint32_t{}: id++);
    }
    auto front(layout.begin());
    auto back(layout.rbegin());
    for (;;) {
        front = std::find(front, layout.end(), ~std::uint32_t{});
        back  = std::find_if(back, layout.rend(), [](auto x) { return x != ~std::uint32_t{}; });
        if (front != layout.end() && back != layout.rend() && front < back.base()) {
           std::swap(*front, *back);
        }
        else
        {
           break;
        }
    }
    std::uint64_t checksum{};
    for (std::uint32_t pos{}; pos != layout.size(); ++pos) {
        if (layout[pos] != ~std::uint32_t{})
            checksum += layout[pos] * pos;
    }
    std::cout << checksum << "\n";
}
