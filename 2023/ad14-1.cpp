#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map;
    for (std::string line; std::getline(std::cin, line); )
    {
         map.push_back(line);
    }

    std::uint64_t sum{};
    for (unsigned c{}; c != map[0].size(); ++c)
    {
        std::size_t weight{map.size()};
        std::size_t row_weight{map.size()};
        for (auto const& r: map)
        {
            --row_weight;
            switch (r[c])
            {
            default: std::cout << "unexpected char '" << r[c] << "'\n"; break;
            case '.': break;
            case 'O': sum += weight--; break;
            case '#': weight = row_weight; break;
            }
        }
    }
    std::cout << sum << "\n";
}
