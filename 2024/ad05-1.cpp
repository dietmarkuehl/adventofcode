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
#if 0
    for (auto bb: after) {
        if (is_before(order, bb, a)) {
            return true;
        }
    }
#endif
    return false;
}

int main() {
    std::unordered_map<int, std::vector<int>> order;
    int before, after;
    std::cin >> std::noskipws;
    while (std::cin >> before >> pipe >> after >> nl)
    {
        //std::cout << "before=" << before << " after=" << after << "\n";
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
            //std::cout << i << " ";
        }
        if (not vals.empty()) {
            //std::cout << " -> " << (ok? "OK": "broken") << " " << vals[vals.size() / 2] << "\n";
            if (ok)
                sum += vals[vals.size() / 2];
        }
        std::cin.clear();
        std::cin.ignore();
    }
    std::cout << sum << "\n";
}
