#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>{std::cin}, {}};
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
    auto access{[&map, w=int(map[0].size()), h=int(map.size())](auto x, auto y){
        if (x < 0) x += w * (1 - x / w);
        if (w <= x) x %= w;
        if (y < 0) y += h * (1 - y / h);
        if (h <= y) y %= h;
        return map[y][x];
    }};
    std::vector<std::pair<int, int>> s{{sx, sy}};
    std::vector<std::pair<int, int>> t{{sx, sy}};
    auto move = [&access, &t](auto x, auto y){
        if (access(x, y) != '#')
            t.emplace_back(x, y);
    };
    std::vector<double> x;
    std::vector<long double> y;
    for (std::size_t c{1}; c != 700; ++c)
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

        if ((c - map.size() / 2) % map.size() == 0)
        {
            x.push_back(c);
            y.push_back(s.size());
            if (x.size() == 3u)
                break;
        }
    }
    long double v{26501365};
    long double c(y[0]);
    long double b((y[1] - y[0]) / (x[1] - x[0]));
    long double a(((y[2] - y[1]) / (x[2] - x[1]) - (y[1] - y[0]) / (x[1] - x[0])) / (x[2] - x[0]));
    std::cout << std::uint64_t(a * (v - x[0]) * (v - x[1]) + b * (v - x[0]) + c) << "\n";
}
