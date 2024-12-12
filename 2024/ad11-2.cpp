#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>

int digits(std::uint64_t d) {
    if (10u <= d && d < 100u)
        return 1;
    if (1000u <= d && d < 10000u)
        return 2;
    if (100000u <= d && d < 1000000u)
        return 3;
    if (10000000u <= d && d < 100000000u)
        return 4;
    if (1000000000u <= d && d < 10000000000u)
        return 5;
    if (100000000000u <= d && d < 1000000000000u)
        return 6;
    if (10000000000000u <= d && d < 100000000000000u)
        return 7;
    if (1000000000000000u <= d && d < 10000000000000000u)
        return 8;
    if (100000000000000000u <= d)
        std::cout << "need digits!\n";
    return 0;
}

std::uint64_t pow10(int p) {
    std::uint64_t rc[]{0u, 10u, 100u, 1000u, 10000u, 100000u, 1000000u, 10000000u, 100000000u };
    return rc[p];
}

    std::uint64_t compute(auto &cache, int i, std::uint64_t d){
        std::uint64_t dummy{};
        if (i == 0)
            return 1ull;
        if (d < cache.size() && cache[d][i]) {
            return cache[d][i];
        }
        else if (d == 0) {
            return (d < cache.size()? cache[d][i]: dummy) = compute(cache, i - 1, 1u);
        }
        else if (int digs = digits(d)) {
            return (d < cache.size()? cache[d][i]: dummy) = compute(cache, i - 1, d/pow10(digs)) + compute(cache, i - 1, d%pow10(digs));
        }
        else {
            return (d < cache.size()? cache[d][i]: dummy) = compute(cache, i - 1, 2024u * d);
        }
    }

int main() {
    std::vector<std::uint64_t> data{std::istream_iterator<std::uint64_t>(std::cin), {}};
    std::vector<std::array<std::uint64_t, 75>> cache(512);

    std::uint64_t sum{};
    for (auto d: data) {
        sum += compute(cache, 75, d);
    }

    std::cout << sum << "\n";
}
