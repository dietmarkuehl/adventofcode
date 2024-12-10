#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct coord {
   int x;
   int y;
   bool operator== (coord const&) const = default;
};
int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    std::unordered_map<char, std::vector<coord>> antennas;
    for (std::size_t y{}; y != map.size(); ++y)
    {
        for (std::size_t x{}; x != map[y].size(); ++x) {
            if (map[y][x] != '.')
            {
                antennas[map[y][x]].emplace_back(x, y);
            }
        }
    }
    for (auto const& as: antennas) {
        for (auto const& a: as.second)
            for (auto const& b: as.second)
                if (a != b)
                {
                    int xpos = a.x + (a.x - b.x);
                    int ypos = a.y + (a.y - b.y);
                    if (0 <= xpos && xpos < int(map[0].size())
                        && 0 <= ypos && ypos < int(map.size()))
                       map[ypos][xpos] = '#';
                }
    }
    int count{};
    for (auto l: map)
    {
        count += std::ranges::count(l, '#');
    }
    std::cout << count << "\n";
}
