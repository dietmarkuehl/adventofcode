#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

enum direction
{
    left,
    right,
    up,
    down
};
std::istream& operator>> (std::istream& in, direction& dir)
{
    char c;
    if (in >> c)
        switch (c)
        {
        default: in.setstate(std::ios_base::failbit); break;
        case 'L': dir = direction::left; break;
        case 'R': dir = direction::right; break;
        case 'U': dir = direction::up; break;
        case 'D': dir = direction::down; break;
        }
   return in;
}
std::ostream& operator<< (std::ostream& out, direction dir)
{
    switch (dir)
    {
    case direction::left: return out << "left";
    case direction::right: return out << "right";
    case direction::up: return out << "up";
    case direction::down: return out << "down";
    }
}

int main() {
    direction   dir;
    unsigned    len{};
    std::string rgb;
    std::vector<std::tuple<direction, unsigned>> plan;
    int maxx{}, minx{}, maxy{}, miny{};
    int x{}, y{};
    while (std::cin >> dir >> len >> rgb)
    {
        plan.emplace_back(dir, len);
        switch (dir)
        {
        case direction::up: y -= len; miny = std::min(y, miny); break;
        case direction::down: y += len; maxy = std::max(y, maxy); break;
        case direction::left: x -= len; minx = std::min(x, minx); break;
        case direction::right: x += len; maxx = std::max(x, maxx); break;
        }
    }
    std::vector<std::string> area(maxy - miny + 1, std::string(maxx - minx + 1, '.'));
    x = 0 - minx;
    y = 0 - miny;
    auto dig{[&area, &x, &y](unsigned len, int xoff, int yoff){ 
        while (len--)
        {
            area.at(y += yoff).at(x += xoff) = '#';
        }
    }};
    for (auto[dir, len]: plan)
    {
        switch (dir)
        {
        case direction::up: dig(len, 0, -1); break;
        case direction::down: dig(len, 0, 1); break;
        case direction::left: dig(len, -1, 0); break;
        case direction::right: dig(len, 1, 0); break;
        }
    }
    for (auto r: area)
    {
        std::cout << r << "\n";
    }

    auto it{std::find(area[1].begin(), area[1].end(), '#')};
    std::vector<std::pair<int, int>> stack{{it - area[1].begin() + 1, 1}};
    auto push{[&area, &stack](auto x, auto y){
        if (area[y][x] == '.')
        {
            area[y][x] = '@';
            stack.emplace_back(x, y);
        }
    }};
    while (not stack.empty())
    {
        auto[x, y] = stack.back();
        stack.pop_back();
        area[y][x] = '@';
        push(x, y - 1);
        push(x, y + 1);
        push(x - 1, y);
        push(x + 1, y);
    }

    unsigned sum{};
    for (auto& r: area)
    {
        sum += std::count_if(r.begin(), r.end(), [](auto c){ return c != '.'; });
    }
    std::cout << sum << "\n";
}
