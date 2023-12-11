#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    auto print_map = [&map]{
        // std::copy(map.begin(), map.end(), std::ostream_iterator<std::string>(std::cout, "\n")); std::cout << "\n";
    };
    print_map();

    for (auto i(map.size()); i--; )
    {
        if (std::none_of(map[i].begin(), map[i].end(), [](char c){ return c == '#'; }))
        {
            map.insert(map.begin() + i, map[i]);
        }
    }
    print_map();
    for (auto i(map[0].size()); i--; )
    {
        if (std::none_of(map.begin(), map.end(), [i](auto const& l){ return l[i] == '#'; }))
        {
            for (auto& l: map)
            {
                l.insert(l.begin() + i, '.');
            }
        }
    }
    print_map();
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
    std::uint64_t sum{};
    for (auto[x0, y0]: galaxy)
    {
        for (auto[x1, y1]: galaxy)
        {
            auto distance(std::abs(x1 - x0) + std::abs(y1 - y0));
            //std::cout << "(" << x0 << ", " << y0 << ") - (" << x1 << ", " << y1 << ")="
            //          << distance << "\n";
            sum += distance;
        }
    }
    std::cout << (sum / 2) << "\n";
}
