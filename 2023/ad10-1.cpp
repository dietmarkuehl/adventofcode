#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

enum direction { south, north, west, east };
std::ostream& operator<<(std::ostream& out, direction dir)
{
    switch (dir)
    {
    case south: return out << "south";
    case north: return out << "north";
    case west: return out << "west";
    case east: return out << "east";
    }
}

int main() {
    std::cout << std::unitbuf;
    std::vector<std::string> map;
    for (std::string line; std::getline(std::cin, line); )
    {
        map.push_back('.' + line + '.');
    }
    map.insert(map.begin(), std::string(map.front().size(), '.'));
    map.insert(map.end(), std::string(map.front().size(), '.'));

    auto[x,y] = std::invoke([&map]()->std::pair<unsigned, unsigned>{
        for (auto const& l: map)
        {
            auto it{std::find(l.begin(), l.end(), 'S')};
            if (it != l.end())
                return { it - l.begin(), &l - &map.front() };
        }
        return {};
    });

    std::vector<std::tuple<unsigned, unsigned, int, int, direction>> cursor;
    auto at = [](auto& map, auto x, auto y){ return map[y][x]; };
    auto[mx, my, dir] = std::invoke([&at, &map, x, y]{
        switch(at(map, x-1, y))
        {
            case '-':
            case 'L':
            case 'F':
                return std::tuple(-1, 0, west);
        }
        switch(at(map, x+1, y))
        {
            case '-':
            case 'J':
            case '7':
                return std::tuple(1, 0, east); 
        }
        switch(at(map, x, y-1))
        {
            case '|':
            case '7':
            case 'F':
                return std::tuple(0, -1, north);
        }
        switch(at(map, x, y+1))
        {
            case '|':
            case 'J':
            case 'L':
                return std::tuple(0, 1, south);
        }
        return std::tuple(0, 0, west);
    });
    unsigned count{0};
    unsigned sx(x), sy(y);
    do
    {
        ++count;
        x += mx;
        y += my;
        switch (at(map, x, y))
        {
            case '|':
                mx = 0;
                my = dir == north? -1: 1;
                //dir = dir;
                break;
            case '-':
                mx = dir == west? 1: -1;
                my = 0;
                //dir = dir;
                break;
            case 'L':
                mx = dir == south? 1: 0;
                my = dir == south? 0: -1;
                dir = dir == south? west: north;
                break;
            case 'J':
                mx = dir == south? -1: 0;
                my = dir == south? 0: -1;
                dir = dir == south? east: north;
                break;
            case '7':
                mx = dir == north? -1: 0;
                my = dir == north? 0: 1;
                dir = dir == north? east: south;
                break;
            case 'F':
                mx = dir == north? 1: 0;
                my = dir == north? 0: 1;
                dir = dir == north? west: south;
                break;
        }
    }
    while (x != sx || y != sy);

    std::cout << (count / 2) << "\n";
}
