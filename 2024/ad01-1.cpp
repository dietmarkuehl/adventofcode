#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>

int main() {
    std::vector<std::int64_t> l0, l1;
    for (int v0, v1; std::cin >> v0 >> v1; ) {
        l0.push_back(v0);
        l1.push_back(v1);
    }
    std::ranges::sort(l0);
    std::ranges::sort(l1);
    std::int64_t sum{};
    for (std::size_t i{}; i != l0.size(); ++i) {
        sum += std::abs(l0[i] - l1[i]);
    }
    std::cout << sum << "\n";
}
