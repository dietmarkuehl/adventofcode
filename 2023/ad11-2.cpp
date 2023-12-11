#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};

    std::vector<std::pair<int, int>> galaxy;
    for (unsigned y{}; y != map.size(); ++y)
    {
        for (unsigned x{}; x != map[y].size(); ++x)
        {
            if (map[y][x] == '#')
            {
                galaxy.emplace_back(x, y);
            }
        }
    }

    int expansion{1'000'000};

    for (int i(map.size()); i--; )
    {
        if (std::none_of(map[i].begin(), map[i].end(), [](char c){ return c == '#'; }))
        {
            for (auto&[x, y]: galaxy)
            {
                if (i < y) {
                    y += expansion - 1;
                }
            }
        }
    }

    for (int i(map[0].size()); i--; )
    {
        if (std::none_of(map.begin(), map.end(), [i](auto const& l){ return l[i] == '#'; }))
        {
            for (auto&[x, y]: galaxy)
            {
                if (i < x) {
                    x += expansion - 1;
                }
            }
        }
    }

    std::uint64_t sum{};
    for (auto[x0, y0]: galaxy)
    {
        for (auto[x1, y1]: galaxy)
        {
            auto distance(std::abs(x1 - x0) + std::abs(y1 - y0));
            sum += distance;
        }
    }
    std::cout << (sum / 2) << "\n";
}
