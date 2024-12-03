#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <cinttypes>

int main() {
    std::ostringstream out;
    out << std::cin.rdbuf();
    std::string s(out.str());
    auto it(s.begin());
    std::string mul("mul(");
    std::uint64_t sum{};
    while ((it = std::search(it, s.end(), mul.begin(), mul.end())) != s.end())
    {
        it += 4;
        auto n1(it);
        auto e1(n1);
        for (int i{}; e1 != s.end() && i < 3 && std::isdigit(*e1); ++i)
            ++e1;
        if (e1 == s.end() || *e1 != ',')
            continue;
        auto n2(e1 + 1);
        auto e2(n2);
        for (int i{}; e2 != s.end() && i < 3 && std::isdigit(*e2); ++i)
            ++e2;
        if (e2 == s.end() || *e2 != ')')
            continue;
        sum += std::atoi(std::string(n1, e1).c_str()) * std::atoi(std::string(n2, e2).c_str());
    }
    std::cout << sum << "\n";
}
