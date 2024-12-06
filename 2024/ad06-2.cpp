#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    //auto print{[&map]{
    //    std::copy(map.begin(), map.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    //}};
    int x{}, y{};
    for (; y < (int)map.size(); ++y)
        if ((x = map[y].find('^')) != int(std::string::npos))
            break;

    auto walk{[](int x, int y, auto map)mutable{
            auto lprint{[&map]{
                for (std::size_t i{}; i != map[0].size(); ++i)
                {
                    for (std::size_t j{}; j != map.size(); ++j)
                        switch (map[i][j]) {
                        default:
                            std::cout << "0123456789ABCDEF"[(int)map[i][j]];
                            break;
                        case '.':
                        case '#':
                        case '^':
                            std::cout << map[i][j];
                            break;
                        }
                     std::cout << "\n";
                }
                std::cout << "\n";
                return true;
            }};
        struct direction { int x; int y; };
        direction dirs[] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} };
        int current{2};
        while (0 <= x && x < int(map[0].size()) && 0 <= y && y < int(map.size()))
        {
            if (map[y][x] == '.' || map[y][x] == '^') {
                map[y][x] = 1 << current;
            }
            else if (map[y][x] & (1 << current)) {
                 //lprint();
                 return true;
            }
            else {
                map[y][x] |= 1 << current;
            }
            int nx = x + dirs[current].x;
            int ny = y + dirs[current].y;
            if (map[ny][nx] == '#')
                current = (current + 1) % 4;
            else {
                x = nx;
                y = ny;
            }
        }
        //lprint();
        return false;
    }};
    int count {};
    for (int yy{}; yy < (int)map.size(); ++yy)
        for (int xx{}; xx < (int)map.size(); ++xx)
            if (map[yy][xx] == '.') {
                map[yy][xx] = '#';
                count += walk(x, y, map);
                map[yy][xx] = '.';
            }
    std::cout << count << "\n";
}
