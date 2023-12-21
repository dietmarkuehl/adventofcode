#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>{std::cin}, {}};
    for (auto& m: map) m = '#' + m + '#';
    map.push_back(std::string(map[0].size(), '#'));
    map.insert(map.begin(), std::string(map[0].size(), '#'));
    std::size_t sx{}, sy{1};
    for (; sy != map.size(); ++sy)
    {
        auto it{std::find(map[sy].begin(), map[sy].end(), 'S')};
        if (it != map[sy].end())
        {
            sx = it - map[sy].begin();
            break;
        }
    }
    std::vector<std::pair<std::size_t, std::size_t>> s{{sx, sy}};
    std::vector<std::pair<std::size_t, std::size_t>> t{{sx, sy}};
    auto move = [&map, &t](auto x, auto y){
        if (map[y][x] != '#')
            t.emplace_back(x, y);
    };
    for (std::size_t c{0}; c != 64; ++c)
    {
        t.clear();
        while (!s.empty())
        {
            auto[x, y] = s.back();
            s.pop_back();
            move(x - 1, y);
            move(x + 1, y);
            move(x, y - 1);
            move(x, y + 1);
        }
        std::sort(t.begin(), t.end());
        t.erase(std::unique(t.begin(), t.end()), t.end());
        std::swap(s, t);
    }
    std::cout << s.size() << "\n";
}
