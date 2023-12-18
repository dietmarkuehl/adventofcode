#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>

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

int main(int ac, char* av[]) {
    std::cout << std::unitbuf;
    std::istream  in(std::cin.rdbuf());
    std::ifstream fin;
    if (ac == 2)
    {
        fin.open(av[1]);
        in.rdbuf(fin.rdbuf());
    }


    direction   dir;
    unsigned    len{};
    std::string rgb;
    std::vector<std::tuple<direction, unsigned>> plan;
    int maxx{}, minx{}, maxy{}, miny{};
    int x{}, y{};
    std::vector<int> xcoords{};
    std::vector<int> ycoords{};
    xcoords.push_back(0);
    ycoords.push_back(0);
    while (in >> dir >> len >> rgb)
    {
        if (true)
        {
            switch (rgb[7])
            {
            case '0': dir = direction::right; break;
            case '1': dir = direction::down; break;
            case '2': dir = direction::left; break;
            case '3': dir = direction::up; break;
            }
            len = std::strtol(rgb.substr(2, 5).c_str(), nullptr, 16);
        }

        plan.emplace_back(dir, len);
        switch (dir)
        {
        case direction::up:
            y -= len;
            ycoords.push_back(y);
            ycoords.push_back(y + 1);
            miny = std::min(y, miny);
            break;
        case direction::down:
            y += len;
            ycoords.push_back(y);
            ycoords.push_back(y + 1);
            maxy = std::max(y, maxy);
            break;
        case direction::left:
            x -= len;
            xcoords.push_back(x);
            xcoords.push_back(x + 1);
            minx = std::min(x, minx);
            break;
        case direction::right:
            x += len;
            xcoords.push_back(x);
            xcoords.push_back(x + 1);
            maxx = std::max(x, maxx);
            break;
        }
    }

    std::sort(xcoords.begin(), xcoords.end());
    xcoords.push_back(xcoords.back() + 1);
    xcoords.erase(std::unique(xcoords.begin(), xcoords.end()), xcoords.end());
    std::unordered_map<int, unsigned> xmap;
    x = 0;
    for (auto ox: xcoords)
    {
        xmap[ox] = x++;
    }

    std::sort(ycoords.begin(), ycoords.end());
    ycoords.push_back(ycoords.back() + 1);
    ycoords.erase(std::unique(ycoords.begin(), ycoords.end()), ycoords.end());
    std::unordered_map<int, unsigned> ymap;
    y = 0;
    for (auto oy: ycoords)
    {
        ymap[oy] = y++;
    }

    std::vector<std::string> area(ycoords.size() + 3, std::string(xcoords.size() + 3, '.'));
    x = xmap[0];
    y = ymap[0];
    int ox{};
    int oy{};
    auto dig{[&xmap, &ymap, &area, &ox, &oy, &x, &y](unsigned len, int xoff, int yoff){ 
        if (xoff != 0)
        {
            ox += xoff * len;
            int nx = xmap[ox];
            len = xoff * (nx - x);
        }
        else
        {
            oy += yoff * len;
            int ny = ymap[oy];
            len = yoff * (ny - y);
        }
        while (len--)
        {
            if (0 <= y + yoff && y + yoff < int(area.size())
                && 0 <= x + xoff && x + xoff < int(area[0].size()))
            {
                area[1 + (y += yoff)][1 + (x += xoff)] = '#';
            }
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

    std::vector<std::pair<int, int>> stack{{0, 0}};
    auto push{[&area, &stack](auto x, auto y){
        if (area[y][x] == '.')
        {
            area[y][x] = '+';
            stack.emplace_back(x, y);
        }
    }};
    while (not stack.empty())
    {
        auto[x, y] = stack.back();
        stack.pop_back();
        push(x, y - 1);
        push(x, y + 1);
        push(x - 1, y);
        push(x + 1, y);
    }

    std::int64_t sum{};
    for (std::size_t r{0}; r < area.size(); r += 1)
    {
        auto const& row(area[r]);

        std::int64_t rsum{};
        for (std::size_t c{0}; c < area[0].size(); ++c)
        {
            if (row[c] != '+')
            {
                rsum += xcoords[c] - xcoords[c - 1];
            }
        }
        std::int64_t tsum{rsum * std::max(0, (ycoords[r] - ycoords[r - 1]))};
        sum += tsum;
    }
    std::cout << sum << "\n";
}
