#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <ranges>

template <char C>
std::istream& expect(std::istream& in) {
    int c{in.peek()};
    if (c != C) {
        in.setstate(std::ios_base::failbit);
    }
    else {
        std::cin.ignore();
    }
    return in;
}
constexpr auto pipe{expect<'|'>};
constexpr auto nl{expect<'\n'>};
constexpr auto comma{expect<','>};

bool is_before(auto &order, int b, int a) {
    auto const& after(order[b]);
    if (after.end() != std::find(after.begin(), after.end(), a)) {
        return true;
    }
    return false;
}

int main() {
    std::unordered_map<int, std::vector<int>> order;
    int before, after;
    std::cin >> std::noskipws;
    while (std::cin >> before >> pipe >> after >> nl)
    {
        order[before].push_back(after);
    }
    std::cin.clear();


    int sum{};
    while (!std::cin.eof()) {
        std::vector<int> vals;
        bool ok{true};
        for (int i; std::cin >> i; std::cin >> comma) {
            if (!vals.empty()) {
                ok = ok && is_before(order, vals.back(), i);
            }
            vals.push_back(i);
        }
        auto pred = [&order](int b, int a){ return is_before(order, b, a); };
        if (not vals.empty()) {
            if (!ok) {
                for (auto it(vals.begin()); vals.end() != (it = std::adjacent_find(vals.begin(), vals.end(), pred)); ) {
                    std::swap(it[0], it[1]);
                }
                sum += vals[vals.size() / 2];
            }
        }
        std::cin.clear();
        std::cin.ignore();
    }
    std::cout << sum << "\n";
}
