#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> map{std::istream_iterator<std::string>(std::cin), {}};
    int x{}, y{};
    for (; y < (int)map.size(); ++y)
        if ((x = map[y].find('^')) != int(std::string::npos))
            break;

    struct direction { int x; int y; };
    direction dirs[] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} };
    auto marks(map);
    auto mark([&dirs, &marks, x, y]() mutable{
        int current{2};
        while (0 <= x && x < int(marks[0].size()) && 0 <= y && y < int(marks.size()))
        {
            marks[y][x] = 'X';
            int nx = x + dirs[current].x;
            int ny = y + dirs[current].y;
            if (marks[ny][nx] == '#')
                current = (current + 1) % 4;
            else {
                x = nx;
                y = ny;
            }
        }
    });
    mark();

    auto walk{[&dirs](int x, int y, auto map)mutable{
        int current{2};
        while (0 <= x && x < int(map[0].size()) && 0 <= y && y < int(map.size()))
        {
            if (map[y][x] == '.' || map[y][x] == '^') {
                map[y][x] = 1 << current;
            }
            else if (map[y][x] & (1 << current)) {
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
        return false;
    }};
    int count {};
    for (int yy{}; yy < (int)map.size(); ++yy)
        for (int xx{}; xx < (int)map.size(); ++xx)
            if (marks[yy][xx] == 'X' && map[yy][xx] == '.') {
                map[yy][xx] = '#';
                count += walk(x, y, map);
                map[yy][xx] = '.';
            }
    std::cout << count << "\n";
}
