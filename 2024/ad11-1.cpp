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
    return 0;
}

std::uint64_t pow10(int p) {
    std::uint64_t rc[]{0u, 10u, 100u, 1000u, 10000u, 100000u, 1000000u, 10000000u, 100000000u };
    return rc[p];
}

int main() {
    std::vector<std::uint64_t> data{std::istream_iterator<std::uint64_t>(std::cin), {}};
    auto print{[&data]{ std::copy(data.begin(), data.end(), std::ostream_iterator<std::uint64_t>(std::cout, " ")); std::cout << "\n"; }};

    for (int i{}; i < 25; ++i) {
        std::vector<std::uint64_t> tmp;
        for (auto d: data) {
            if (d == 0u) {
                tmp.push_back(1u);
            }
            else if (int digs = digits(d)) {
                tmp.push_back(d / pow10(digs));
                tmp.push_back(d % pow10(digs));
            }
            else {
                tmp.push_back(d * 2024);
            }
        }
        data.swap(tmp);
        //print();
    }

    //print();
    std::cout << data.size() << "\n";
}
