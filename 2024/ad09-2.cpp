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
    auto print = [&layout]{ for (auto x: layout) std::cout << x << "\n"; }; 
    auto back(layout.rbegin());
    for (;;) {
        back  = std::find_if(back, layout.rend(), [](auto x) { return x != ~std::uint32_t{}; });
        if (back != layout.rend())
        {
            auto start = std::find_if(back, layout.rend(), [c=*back](auto x){ return x != c; });
            std::vector<std::uint32_t> search(start - back, ~std::uint32_t{});
            auto dest = std::search(layout.begin(), layout.end(), search.begin(), search.end());
            if (dest < start.base()) {
                while (back != start) {
                    std::swap(*dest++, *back++);
                }
            }
            back = start;
        }
        else
           break;
    }
    std::uint64_t checksum{};
    for (std::uint32_t pos{}; pos != layout.size(); ++pos) {
        if (layout[pos] != ~std::uint32_t{})
            checksum += layout[pos] * pos;
    }
    std::cout << checksum << "\n";
}
